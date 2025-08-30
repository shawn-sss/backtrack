#ifndef SCHEDULESERVICEMANAGER_H
#define SCHEDULESERVICEMANAGER_H

// Qt includes
#include <QObject>
#include <QDateTime>

// Forward declaration (Qt class)
class QTimer;

// Centralized scheduler logic
class ScheduleServiceManager : public QObject {
    Q_OBJECT

public:
    enum class Recurrence { Once, Daily, Weekly, Monthly };

    struct Config {
        bool enabled = false;
        QDateTime when;
        Recurrence recur = Recurrence::Once;
    };

    explicit ScheduleServiceManager(QObject* parent = nullptr);
    ~ScheduleServiceManager() override = default;

    // Configuration
    Config config() const;
    void setConfig(const Config& cfg, bool rearmTimer = true);
    void enable(bool rearmTimer = true);
    void disable(bool clearWhen = true);
    bool isEnabled() const { return cfg_.enabled; }

    // Scheduling
    QDateTime nextOccurrence(const Config& cfg,
                             const QDateTime& now = QDateTime::currentDateTime()) const;
    QDateTime dueTime(const QDateTime& now = QDateTime::currentDateTime()) const;
    qint64 msUntilDue(const QDateTime& now = QDateTime::currentDateTime()) const;

    // Utility
    static QDateTime forceSecondsZero(const QDateTime& dt);
    static QDateTime nextMinuteFrom(const QDateTime& from);
    static QDateTime nextMinuteFromNow();

public slots:
    void scheduleOrRescheduleTimer();

signals:
    void fired();
    void configChanged(const ScheduleServiceManager::Config& cfg);

private slots:
    void onTimeout();

private:
    QTimer* timer_ = nullptr;
    Config cfg_{};

    void loadFromUserSettings();
    void saveToUserSettings() const;
    QDateTime computeNextDue(const Config& cfg, const QDateTime& now) const;
    static QDateTime advanceByRecurrence(QDateTime dt, Recurrence r);
};

#endif
