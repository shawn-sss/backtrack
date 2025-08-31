// Project includes
#include "scheduledialog.h"
#include "scheduledialogconstants.h"
#include "scheduledialogstyling.h"
#include "../promptdialog/promptdialog.h"

// Qt includes
#include <QDateTime>
#include <QTime>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSignalBlocker>

// C++ includes
#include <array>

using namespace ScheduleDialogConstants;

namespace {

QDateTime nextMinuteFromNow() {
    const QDateTime now = QDateTime::currentDateTime();
    QDateTime floorNow = now;
    const QTime t = now.time();
    floorNow.setTime(QTime(t.hour(), t.minute(), 0));
    return floorNow.addSecs(60);
}

QDateTime clampToNextMinute(const QDateTime& candidate) {
    const QDateTime minAllowed = nextMinuteFromNow();
    return (candidate < minAllowed) ? minAllowed : candidate;
}

} // namespace

// Construct schedule dialog window
ScheduleDialog::ScheduleDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle(tr(k_WINDOW_TITLE));
    setMinimumSize(k_MIN_WIDTH, k_MIN_HEIGHT);

    hintLabel = new QLabel(this);
    hintLabel->setTextFormat(Qt::RichText);
    hintLabel->setOpenExternalLinks(false);
    hintLabel->setTextInteractionFlags(Qt::TextBrowserInteraction | Qt::LinksAccessibleByMouse);
    hintLabel->setStyleSheet(ScheduleDialogStyling::Styles::HINT_LABEL_STYLE);
    connect(hintLabel, &QLabel::linkActivated, this, &ScheduleDialog::onHintLinkActivated);

    settingsGroup = new QGroupBox(tr(k_GROUP_TITLE), this);
    settingsGroup->setStyleSheet(ScheduleDialogStyling::Styles::GROUPBOX_STYLE);

    dateTimeLabel  = new QLabel(tr(k_DATETIME_LABEL), this);
    dateTimeEdit   = new QDateTimeEdit(this);
    dateTimeEdit->setDisplayFormat(k_DATETIME_DISPLAY_FORMAT);
    dateTimeEdit->setCalendarPopup(true);
    {
        const QDateTime minAllowed = nextMinuteFromNow();
        dateTimeEdit->setMinimumDateTime(minAllowed);
        dateTimeEdit->setDateTime(minAllowed);
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
    form->addRow(dateTimeLabel,   dateTimeEdit);
    form->addRow(recurrenceLabel, recurrenceBox);
    settingsGroup->setLayout(form);

    okButton = new QPushButton(tr(k_BUTTON_OK_TEXT), this);
    okButton->setStyleSheet(ScheduleDialogStyling::Styles::BUTTON_STYLE);
    connect(okButton, &QPushButton::clicked, this, &ScheduleDialog::onAcceptClicked);

    auto* main = new QVBoxLayout(this);
    main->setContentsMargins(k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN);
    main->setSpacing(k_MAIN_SPACING);

    main->addWidget(hintLabel);
    main->addWidget(settingsGroup);
    main->addStretch();
    main->addWidget(okButton, 0, Qt::AlignHCenter);

    onEnableToggled(false);
}

// Apply config
void ScheduleDialog::setConfig(const ScheduleConfig& cfg) {
    QDateTime dt = forceSecondsZero(cfg.when);
    dt = clampToNextMinute(dt);

    const QDateTime minAllowed = nextMinuteFromNow();
    dateTimeEdit->setMinimumDateTime(minAllowed);
    dateTimeEdit->setDateTime(dt);

    recurrenceBox->setCurrentIndex(recurrenceToIndex(cfg.recur));
    onEnableToggled(cfg.enabled);
}

// Read config
ScheduleDialog::ScheduleConfig ScheduleDialog::config() const {
    ScheduleConfig out;
    out.enabled = settingsGroup->isEnabled();
    out.when    = forceSecondsZero(dateTimeEdit->dateTime());
    out.recur   = indexToRecurrence(recurrenceBox->currentIndex());
    return out;
}

// Handle accept button
void ScheduleDialog::onAcceptClicked() {
    if (!settingsGroup->isEnabled()) {
        emit scheduleSaved(config());
        return;
    }

    const QDateTime minAllowed = nextMinuteFromNow();
    const QDateTime chosen     = forceSecondsZero(dateTimeEdit->dateTime());

    if (chosen < minAllowed) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            tr(k_TIME_INVALID_TITLE),
            tr(k_TIME_INVALID_BODY),
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        dateTimeEdit->setMinimumDateTime(minAllowed);
        dateTimeEdit->setDateTime(minAllowed);
        dateTimeEdit->setFocus(Qt::TabFocusReason);
        return;
    }

    emit scheduleSaved(config());
}

// Handle recurrence change
void ScheduleDialog::onRecurrenceChanged(int /*index*/) {
}

// Handle enable toggle
void ScheduleDialog::onEnableToggled(bool checked) {
    settingsGroup->setEnabled(checked);
    updateStatusBadge(checked);
    updateHint(checked);
}

// Handle hint link
void ScheduleDialog::onHintLinkActivated(const QString& link) {
    if (link == k_LINK_ENABLE) {
        onEnableToggled(true);
    } else if (link == k_LINK_DISABLE) {
        onEnableToggled(false);
    }
}

// Update badge
void ScheduleDialog::updateStatusBadge(bool /*enabled*/) {
}

// Update hint
void ScheduleDialog::updateHint(bool enabled) {
    hintLabel->setText(enabled ? tr(k_HINT_ENABLED_HTML)
                               : tr(k_HINT_DISABLED_HTML));
    hintLabel->setVisible(true);
}

// Normalize QDateTime
QDateTime ScheduleDialog::forceSecondsZero(const QDateTime& dt) {
    QDateTime out = dt;
    const QTime t = dt.time();
    out.setTime(QTime(t.hour(), t.minute(), 0));
    return out;
}

// Index → Recurrence
ScheduleDialog::Recurrence ScheduleDialog::indexToRecurrence(int idx) {
    switch (idx) {
    case k_IDX_DAILY:   return Recurrence::Daily;
    case k_IDX_WEEKLY:  return Recurrence::Weekly;
    case k_IDX_MONTHLY: return Recurrence::Monthly;
    case k_IDX_ONCE:
    default:            return Recurrence::Once;
    }
}

// Recurrence → Index
int ScheduleDialog::recurrenceToIndex(Recurrence r) {
    switch (r) {
    case Recurrence::Daily:   return k_IDX_DAILY;
    case Recurrence::Weekly:  return k_IDX_WEEKLY;
    case Recurrence::Monthly: return k_IDX_MONTHLY;
    case Recurrence::Once:
    default:                  return k_IDX_ONCE;
    }
}
