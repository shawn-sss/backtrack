#ifndef SCHEDULESERVICEMANAGER_H
#define SCHEDULESERVICEMANAGER_H

// Project includes
#include "ScheduleServiceConstants.h"

// Qt includes
#include <QObject>
#include <QDateTime>
#include <QTimer>

// Centralized scheduler logic with no UI dependencies
class ScheduleServiceManager : public QObject {
    Q_OBJECT
public:
    enum class Recurrence { Once, Daily, Weekly, Monthly };

    struct Config {
        bool       enabled = false;
        QDateTime  when;     // Always aligned to :00 seconds; uses its own timeSpec()
        Recurrence recur = Recurrence::Once;
    };

    explicit ScheduleServiceManager(QObject* parent = nullptr);
    ~ScheduleServiceManager() override = default;

    // Configuration management
    Config config() const;
    void setConfig(const Config& cfg, bool rearmTimer = true);
    void enable(bool rearmTimer = true);
    void disable(bool clearWhen = true);
    bool isEnabled() const { return cfg_.enabled; }

    // Scheduling helpers
    QDateTime nextOccurrence(const Config& cfg, const QDateTime& now = QDateTime::currentDateTime()) const;
    QDateTime dueTime(const QDateTime& now = QDateTime::currentDateTime()) const;
    qint64 msUntilDue(const QDateTime& now = QDateTime::currentDateTime()) const;

    // Utility methods
    static QDateTime forceSecondsZero(const QDateTime& dt);
    static QDateTime nextMinuteFrom(const QDateTime& from);
    static QDateTime nextMinuteFromNow();

public slots:
    void scheduleOrRescheduleTimer();

signals:
    void fired();
    void configChanged(const ScheduleServiceManager::Config& cfg);
    void errorOccurred(const QString& message);

private slots:
    void onTimeout();

private:
    QTimer* timer_ = nullptr;
    Config  cfg_{};

    void loadFromUserSettings();
    void saveToUserSettings() const;
    QDateTime computeNextDue(const Config& cfg, const QDateTime& now) const;
    static QDateTime advanceByRecurrence(QDateTime dt, Recurrence r);
};

#endif // SCHEDULESERVICEMANAGER_H
