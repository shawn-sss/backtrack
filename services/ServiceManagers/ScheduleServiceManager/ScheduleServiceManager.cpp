// ScheduleServiceManager.cpp

// Project includes
#include "ScheduleServiceManager.h"
#include "ScheduleServiceConstants.h"
#include "../../../services/ServiceDirector/ServiceDirector.h"
#include "../../../services/ServiceManagers/UserServiceManager/UserServiceManager.h"

// Qt includes
#include <QDateTime>
#include <QJsonObject>
#include <QTimer>
#include <QTimeZone>
#include <QtGlobal>

// C++ includes

// Forward declaration (Custom class)

// Forward declaration (Qt class)

using namespace ScheduleServiceManagerConstants;

// Keys for the nested schedule node in user settings
namespace SchedKeys {
constexpr char kNode[]    = "schedule";
constexpr char kEnabled[] = "enabled";
constexpr char kWhenIso[] = "when_iso";
constexpr char kRecur[]   = "recur";
constexpr char kLegacyEnabled[] = "schedule_enabled";
constexpr char kLegacyWhenIso[] = "schedule_when_iso";
constexpr char kLegacyRecur[]   = "schedule_recur";
}

// Static helpers
QDateTime ScheduleServiceManager::forceSecondsZero(const QDateTime& dt) {
    const QDate d = dt.date();
    const QTime t = dt.time();
    return QDateTime(d, QTime(t.hour(), t.minute(), 0), dt.timeZone());
}

QDateTime ScheduleServiceManager::nextMinuteFrom(const QDateTime& from) {
    const QDateTime floorFrom(from.date(), QTime(from.time().hour(), from.time().minute(), 0), from.timeZone());
    return floorFrom.addSecs(k_NEXT_MINUTE_STEP_SECONDS);
}

QDateTime ScheduleServiceManager::nextMinuteFromNow() {
    return nextMinuteFrom(QDateTime::currentDateTime());
}

QDateTime ScheduleServiceManager::advanceByRecurrence(QDateTime dt, Recurrence r) {
    switch (r) {
    case Recurrence::Daily:   return dt.addDays(1);
    case Recurrence::Weekly:  return dt.addDays(7);
    case Recurrence::Monthly: return dt.addMonths(1);
    case Recurrence::Once:
    default:                  return dt;
    }
}

// Constructor
ScheduleServiceManager::ScheduleServiceManager(QObject* parent)
    : QObject(parent)
{
    timer_ = new QTimer(this);
    timer_->setSingleShot(true);
    connect(timer_, &QTimer::timeout, this, &ScheduleServiceManager::onTimeout);
    loadFromUserSettings();
}

// Persistence: load from user settings (with legacy migration)
void ScheduleServiceManager::loadFromUserSettings() {
    auto& root = ServiceDirector::getInstance().getUserServiceManager()->settings();

    QJsonObject sched = root.value(SchedKeys::kNode).toObject();

    if (sched.isEmpty() &&
        (root.contains(SchedKeys::kLegacyEnabled) ||
         root.contains(SchedKeys::kLegacyWhenIso) ||
         root.contains(SchedKeys::kLegacyRecur))) {

        Config migrated;
        migrated.enabled = root.value(SchedKeys::kLegacyEnabled).toBool(false);

        const QString whenIsoLegacy = root.value(SchedKeys::kLegacyWhenIso).toString();
        if (!whenIsoLegacy.isEmpty()) {
            const QDateTime dt = QDateTime::fromString(whenIsoLegacy, Qt::ISODate);
            if (dt.isValid()) migrated.when = dt;
        }

        const int recurLegacy = root.value(SchedKeys::kLegacyRecur).toInt(static_cast<int>(Recurrence::Once));
        migrated.recur = static_cast<Recurrence>(qBound(0, recurLegacy, 3));

        QJsonObject out;
        out.insert(SchedKeys::kEnabled, migrated.enabled);
        out.insert(SchedKeys::kWhenIso, migrated.when.isValid()
                                            ? forceSecondsZero(migrated.when).toString(Qt::ISODate)
                                            : QString());
        out.insert(SchedKeys::kRecur, static_cast<int>(migrated.recur));
        root[SchedKeys::kNode] = out;

        root.remove(SchedKeys::kLegacyEnabled);
        root.remove(SchedKeys::kLegacyWhenIso);
        root.remove(SchedKeys::kLegacyRecur);

        ServiceDirector::getInstance().getUserServiceManager()->save();

        setConfig(migrated, true);
        return;
    }

    Config c;
    c.enabled = sched.value(SchedKeys::kEnabled).toBool(false);

    const QString whenIso = sched.value(SchedKeys::kWhenIso).toString();
    if (!whenIso.isEmpty()) {
        const QDateTime dt = QDateTime::fromString(whenIso, Qt::ISODate);
        if (dt.isValid()) c.when = dt;
    }

    const int recurInt = sched.value(SchedKeys::kRecur).toInt(static_cast<int>(Recurrence::Once));
    c.recur = static_cast<Recurrence>(qBound(0, recurInt, 3));

    setConfig(c, true);
}

// Persistence: save to user settings
void ScheduleServiceManager::saveToUserSettings() const {
    auto& root = ServiceDirector::getInstance().getUserServiceManager()->settings();

    QJsonObject sched;
    sched.insert(SchedKeys::kEnabled, cfg_.enabled);
    sched.insert(SchedKeys::kWhenIso, cfg_.when.isValid()
                                          ? forceSecondsZero(cfg_.when).toString(Qt::ISODate)
                                          : QString());
    sched.insert(SchedKeys::kRecur, static_cast<int>(cfg_.recur));

    root[SchedKeys::kNode] = sched;
    ServiceDirector::getInstance().getUserServiceManager()->save();
}

// Configuration: get current config
ScheduleServiceManager::Config ScheduleServiceManager::config() const {
    return cfg_;
}

// Configuration: set new config
void ScheduleServiceManager::setConfig(const Config& in, bool rearmTimer) {
    Config normalized = in;

    if (normalized.when.isValid())
        normalized.when = forceSecondsZero(normalized.when);

    if (normalized.enabled) {
        const QDateTime minAllowed = nextMinuteFromNow();
        if (!normalized.when.isValid() || normalized.when < minAllowed)
            normalized.when = minAllowed;
    }

    cfg_ = normalized;
    emit configChanged(cfg_);

    if (rearmTimer)
        scheduleOrRescheduleTimer();

    saveToUserSettings();
}

// Configuration: enable scheduling
void ScheduleServiceManager::enable(bool rearmTimer) {
    if (!cfg_.enabled) {
        cfg_.enabled = true;

        const QDateTime minAllowed = nextMinuteFromNow();
        if (!cfg_.when.isValid() || cfg_.when < minAllowed)
            cfg_.when = minAllowed;

        emit configChanged(cfg_);

        if (rearmTimer)
            scheduleOrRescheduleTimer();

        saveToUserSettings();
    }
}

// Configuration: disable scheduling
void ScheduleServiceManager::disable(bool clearWhen) {
    if (cfg_.enabled) {
        cfg_.enabled = false;
        if (clearWhen) cfg_.when = QDateTime();
        timer_->stop();
        emit configChanged(cfg_);
        saveToUserSettings();
    }
}

// Scheduling helper: compute next occurrence strictly after now
QDateTime ScheduleServiceManager::nextOccurrence(const Config& c, const QDateTime& now) const {
    QDateTime candidate = c.when.isValid() ? forceSecondsZero(c.when) : nextMinuteFrom(now);

    if (c.recur == Recurrence::Once)
        return candidate;

    while (candidate <= now) {
        candidate = forceSecondsZero(advanceByRecurrence(candidate, c.recur));
    }
    return candidate;
}

// Scheduling helper: compute next due time with clamps
QDateTime ScheduleServiceManager::computeNextDue(const Config& c, const QDateTime& now) const {
    QDateTime due = nextOccurrence(c, now);
    const QDateTime minAllowed = nextMinuteFrom(now);
    if (due < minAllowed)
        due = minAllowed;
    return forceSecondsZero(due);
}

// Scheduling helper: expose current due time
QDateTime ScheduleServiceManager::dueTime(const QDateTime& now) const {
    if (!cfg_.enabled) return QDateTime();
    return computeNextDue(cfg_, now);
}

// Scheduling helper: milliseconds until due
qint64 ScheduleServiceManager::msUntilDue(const QDateTime& now) const {
    if (!cfg_.enabled) return -1;
    return now.msecsTo(computeNextDue(cfg_, now));
}

// Timer control: (re)arm internal timer
void ScheduleServiceManager::scheduleOrRescheduleTimer() {
    timer_->stop();

    if (!cfg_.enabled)
        return;

    const QDateTime now = QDateTime::currentDateTime();
    const QDateTime due = computeNextDue(cfg_, now);
    qint64 msec = now.msecsTo(due);

    if (msec <= 0)
        msec = k_MINIMUM_REARM_FALLBACK_MS;

    timer_->start(static_cast<int>(msec));
}

// Slot: timer timeout handler
void ScheduleServiceManager::onTimeout() {
    emit fired();

    const QDateTime now = QDateTime::currentDateTime();

    if (cfg_.recur == Recurrence::Once) {
        cfg_.enabled = false;
        cfg_.when = QDateTime();
        emit configChanged(cfg_);
        saveToUserSettings();
        return;
    }

    cfg_.when = computeNextDue(cfg_, now);
    emit configChanged(cfg_);
    saveToUserSettings();
    scheduleOrRescheduleTimer();
}
