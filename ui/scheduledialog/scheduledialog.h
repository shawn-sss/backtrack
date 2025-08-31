#ifndef SCHEDULEDIALOG_H
#define SCHEDULEDIALOG_H

// Qt includes
#include <QDateTime>
#include <QDialog>

// Forward declaration (Qt class)
class QComboBox;
class QDateTimeEdit;
class QGroupBox;
class QLabel;
class QPushButton;
class QString;

// Schedule dialog window
class ScheduleDialog : public QDialog {
    Q_OBJECT

public:
    enum class Recurrence { Once, Daily, Weekly, Monthly };

    struct ScheduleConfig {
        bool       enabled = false;
        QDateTime  when;
        Recurrence recur = Recurrence::Once;
    };

    explicit ScheduleDialog(QWidget* parent = nullptr);

    void setConfig(const ScheduleConfig& cfg);
    ScheduleConfig config() const;

signals:
    void scheduleSaved(const ScheduleConfig& cfg);

private slots:
    void onAcceptClicked();
    void onRecurrenceChanged(int index);
    void onEnableToggled(bool checked);
    void onHintLinkActivated(const QString& link);

private:
    QLabel*        hintLabel       = nullptr;
    QGroupBox*     settingsGroup   = nullptr;
    QLabel*        dateTimeLabel   = nullptr;
    QDateTimeEdit* dateTimeEdit    = nullptr;
    QLabel*        recurrenceLabel = nullptr;
    QComboBox*     recurrenceBox   = nullptr;
    QPushButton*   okButton        = nullptr;

    static Recurrence indexToRecurrence(int idx);
    static int        recurrenceToIndex(Recurrence r);
    static QDateTime  forceSecondsZero(const QDateTime& dt);

    void updateStatusBadge(bool enabled);
    void updateHint(bool enabled);

    Q_DISABLE_COPY_MOVE(ScheduleDialog)
};

#endif // SCHEDULEDIALOG_H
