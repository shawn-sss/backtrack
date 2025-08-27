#ifndef SCHEDULEDIALOG_H
#define SCHEDULEDIALOG_H

// Project includes

// Qt includes
#include <QDateTime>
#include <QDialog>

// C++ includes

// Forward declaration (Custom class)

// Forward declaration (Qt class)
class QCheckBox;
class QComboBox;
class QDateTimeEdit;
class QGroupBox;
class QLabel;
class QPushButton;
class QString;

// Lightweight dialog for configuring a schedule
class ScheduleDialog : public QDialog {
    Q_OBJECT
public:
    // Public types
    enum class Recurrence { Once, Daily, Weekly, Monthly };

    // Public data model (seconds normalized to :00)
    struct ScheduleConfig {
        bool       enabled = false;
        QDateTime  when;
        Recurrence recur = Recurrence::Once;
    };

    // Lifecycle
    explicit ScheduleDialog(QWidget* parent = nullptr);

    // Configuration API
    void setConfig(const ScheduleConfig& cfg);
    ScheduleConfig config() const;

signals:
    // Signal emitted on save (dialog remains open)
    void scheduleSaved(const ScheduleConfig& cfg);

private slots:
    // UI slot handlers
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

    // Mapping utilities
    static Recurrence indexToRecurrence(int idx);
    static int        recurrenceToIndex(Recurrence r);

    // Time utilities
    static QDateTime  clampToAtLeastOneMinuteFromNow(const QDateTime& candidate);
    static QDateTime  forceSecondsZero(const QDateTime& dt);

    // UI updates
    void updateStatusBadge(bool enabled);
    void updateHint(bool enabled);
};

#endif // SCHEDULEDIALOG_H
