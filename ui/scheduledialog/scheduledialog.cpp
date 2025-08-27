// Project includes
#include "scheduledialog.h"
#include "scheduledialogconstants.h"

// Qt includes
#include <QDateTime>
#include <QTime>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

// C++ includes

// Forward declaration (Custom class)

// Forward declaration (Qt class)

using namespace ScheduleDialogConstants;

// Compute the next minute boundary from "now"
static QDateTime nextMinuteFromNow()
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime floorNow = now;
    floorNow.setTime(QTime(now.time().hour(), now.time().minute(), 0));
    return floorNow.addSecs(60);
}

// Clamp a datetime to be at least the next minute from now
static QDateTime clampToNextMinute(const QDateTime& candidate)
{
    const QDateTime minAllowed = nextMinuteFromNow();
    return (candidate < minAllowed) ? minAllowed : candidate;
}

// Construct dialog and build UI
ScheduleDialog::ScheduleDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr(k_WINDOW_TITLE));
    setMinimumSize(k_MIN_WIDTH, k_MIN_HEIGHT);

    enableCheck = new QCheckBox(tr(k_ENABLE_LABEL), this);
    enableCheck->hide();
    connect(enableCheck, &QCheckBox::toggled, this, &ScheduleDialog::onEnableToggled);

    hintLabel = new QLabel(this);
    hintLabel->setTextFormat(Qt::RichText);
    hintLabel->setOpenExternalLinks(false);
    hintLabel->setTextInteractionFlags(Qt::TextBrowserInteraction | Qt::LinksAccessibleByMouse);
    connect(hintLabel, &QLabel::linkActivated, this, &ScheduleDialog::onHintLinkActivated);

    settingsGroup = new QGroupBox(tr(k_GROUP_TITLE), this);

    dateTimeLabel  = new QLabel(tr(k_DATETIME_LABEL), this);
    dateTimeEdit   = new QDateTimeEdit(this);
    dateTimeEdit->setDisplayFormat(k_DATETIME_DISPLAY_FORMAT);
    dateTimeEdit->setCalendarPopup(true);
    {
        const QDateTime minAllowed = nextMinuteFromNow();
        dateTimeEdit->setDateTime(minAllowed);
        dateTimeEdit->setMinimumDateTime(minAllowed);
    }

    recurrenceLabel = new QLabel(tr(k_RECURRENCE_LABEL), this);
    recurrenceBox   = new QComboBox(this);
    recurrenceBox->addItem(tr(k_RECURRENCE_ONCE));
    recurrenceBox->addItem(tr(k_RECURRENCE_DAILY));
    recurrenceBox->addItem(tr(k_RECURRENCE_WEEKLY));
    recurrenceBox->addItem(tr(k_RECURRENCE_MONTHLY));
    connect(recurrenceBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ScheduleDialog::onRecurrenceChanged);

    auto* form = new QFormLayout;
    form->addRow(dateTimeLabel,  dateTimeEdit);
    form->addRow(recurrenceLabel, recurrenceBox);
    settingsGroup->setLayout(form);

    dateTimeEdit->setToolTip(tr(k_TOOLTIP_LOCKED));
    recurrenceBox->setToolTip(tr(k_TOOLTIP_LOCKED));
    dateTimeLabel->setToolTip(tr(k_TOOLTIP_LOCKED));
    recurrenceLabel->setToolTip(tr(k_TOOLTIP_LOCKED));

    okButton = new QPushButton(tr(k_BUTTON_OK_TEXT), this);
    connect(okButton, &QPushButton::clicked, this, &ScheduleDialog::onAcceptClicked);

    auto* buttons = new QHBoxLayout;
    buttons->addStretch();
    buttons->addWidget(okButton);

    auto* main = new QVBoxLayout(this);
    main->addWidget(hintLabel);
    main->addWidget(settingsGroup);
    main->addStretch();
    main->addLayout(buttons);
    setLayout(main);

    enableCheck->setChecked(false);
    onEnableToggled(enableCheck->isChecked());
}

// Apply an external configuration to the dialog
void ScheduleDialog::setConfig(const ScheduleConfig& cfg) {
    enableCheck->setChecked(cfg.enabled);

    QDateTime dt = forceSecondsZero(cfg.when);
    dt = clampToNextMinute(dt);

    dateTimeEdit->setDateTime(dt);
    dateTimeEdit->setMinimumDateTime(nextMinuteFromNow());
    recurrenceBox->setCurrentIndex(recurrenceToIndex(cfg.recur));

    onEnableToggled(cfg.enabled);
}

// Read the current configuration from the dialog
ScheduleDialog::ScheduleConfig ScheduleDialog::config() const {
    ScheduleConfig out;
    out.enabled = enableCheck->isChecked();

    QDateTime chosen = forceSecondsZero(dateTimeEdit->dateTime());
    out.when = chosen;
    out.recur = indexToRecurrence(recurrenceBox->currentIndex());
    return out;
}

// Validate and emit save on OK
void ScheduleDialog::onAcceptClicked() {
    if (!enableCheck->isChecked()) {
        emit scheduleSaved(config());
        return;
    }

    const QDateTime minAllowed = nextMinuteFromNow();
    QDateTime chosen = forceSecondsZero(dateTimeEdit->dateTime());

    if (chosen < minAllowed) {
        QMessageBox::warning(this, tr(k_TIME_INVALID_TITLE), tr(k_TIME_INVALID_BODY));
        dateTimeEdit->setMinimumDateTime(minAllowed);
        dateTimeEdit->setDateTime(minAllowed);
        return;
    }

    emit scheduleSaved(config());
}

// React to recurrence selector changes
void ScheduleDialog::onRecurrenceChanged(int /*index*/) {
}

// Enable/disable editable group and refresh hint
void ScheduleDialog::onEnableToggled(bool checked) {
    settingsGroup->setEnabled(checked);
    updateStatusBadge(checked);
    updateHint(checked);
}

// Toggle enable state via hint link
void ScheduleDialog::onHintLinkActivated(const QString& link) {
    if (link == k_LINK_ENABLE) {
        enableCheck->setChecked(true);
    } else if (link == k_LINK_DISABLE) {
        enableCheck->setChecked(false);
    }
}

// Keep for compatibility with any external status badge hooks
void ScheduleDialog::updateStatusBadge(bool /*enabled*/) {
}

// Update the visible hint/status label
void ScheduleDialog::updateHint(bool enabled) {
    if (enabled) {
        hintLabel->setText(tr(k_HINT_ENABLED_HTML));
        hintLabel->setVisible(true);
        return;
    }

    hintLabel->setText(tr(k_HINT_DISABLED_HTML));
    hintLabel->setVisible(true);
}

// Normalize seconds to zero
QDateTime ScheduleDialog::forceSecondsZero(const QDateTime& dt) {
    QDateTime out = dt;
    const QTime t = dt.time();
    out.setTime(QTime(t.hour(), t.minute(), 0));
    return out;
}

// Public wrapper that guarantees at least one minute from now
QDateTime ScheduleDialog::clampToAtLeastOneMinuteFromNow(const QDateTime& candidate) {
    return clampToNextMinute(candidate);
}

// Map combo index to recurrence enum
ScheduleDialog::Recurrence ScheduleDialog::indexToRecurrence(int idx) {
    switch (idx) {
    case 0: return Recurrence::Once;
    case 1: return Recurrence::Daily;
    case 2: return Recurrence::Weekly;
    case 3: return Recurrence::Monthly;
    default: return Recurrence::Once;
    }
}

// Map recurrence enum to combo index
int ScheduleDialog::recurrenceToIndex(Recurrence r) {
    switch (r) {
    case Recurrence::Once:    return 0;
    case Recurrence::Daily:   return 1;
    case Recurrence::Weekly:  return 2;
    case Recurrence::Monthly: return 3;
    }
    return 0;
}
