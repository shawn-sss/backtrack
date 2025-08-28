// filename: scheduledialog.h

#ifndef SCHEDULEDIALOG_H
#define SCHEDULEDIALOG_H

// Qt includes
#include <QDateTime>
#include <QDialog>

// Forward declaration (Qt class)
class QCheckBox;
class QComboBox;
class QDateTimeEdit;
class QGroupBox;
class QLabel;
class QPushButton;
class QString;

class ScheduleDialog : public QDialog {
    Q_OBJECT

public:
    // Recurrence options
    enum class Recurrence { Once, Daily, Weekly, Monthly };

    // Data model for scheduling
    struct ScheduleConfig {
        bool       enabled = false;
        QDateTime  when;
        Recurrence recur = Recurrence::Once;
    };

    explicit ScheduleDialog(QWidget* parent = nullptr);

    // Apply and read config
    void setConfig(const ScheduleConfig& cfg);
    ScheduleConfig config() const;

signals:
    // Emitted when the schedule is saved
    void scheduleSaved(const ScheduleConfig& cfg);

private slots:
    void onAcceptClicked();
    void onRecurrenceChanged(int index);
    void onEnableToggled(bool checked);
    void onHintLinkActivated(const QString& link);

private:
    // UI widgets
    QCheckBox*     enableCheck     = nullptr;
    QLabel*        hintLabel       = nullptr;
    QGroupBox*     settingsGroup   = nullptr;
    QLabel*        dateTimeLabel   = nullptr;
    QDateTimeEdit* dateTimeEdit    = nullptr;
    QLabel*        recurrenceLabel = nullptr;
    QComboBox*     recurrenceBox   = nullptr;
    QPushButton*   okButton        = nullptr;

    // Mapping helpers
    static Recurrence indexToRecurrence(int idx);
    static int        recurrenceToIndex(Recurrence r);

    // Time utility
    static QDateTime  forceSecondsZero(const QDateTime& dt);

    // UI update helpers
    void updateStatusBadge(bool enabled);
    void updateHint(bool enabled);

    Q_DISABLE_COPY_MOVE(ScheduleDialog)
};

#endif // SCHEDULEDIALOG_H
