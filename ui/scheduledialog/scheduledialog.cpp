// filename: scheduledialog.cpp

// Project includes
#include "scheduledialog.h"
#include "scheduledialogconstants.h"
#include "scheduledialogstyling.h"
#include "../promptdialog/promptdialog.h"

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

constexpr int IDX_ONCE    = 0;
constexpr int IDX_DAILY   = 1;
constexpr int IDX_WEEKLY  = 2;
constexpr int IDX_MONTHLY = 3;

} // namespace

// Constructs ScheduleDialog and initializes UI
ScheduleDialog::ScheduleDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle(tr("%1").arg(k_WINDOW_TITLE));
    setMinimumSize(k_MIN_WIDTH, k_MIN_HEIGHT);

    enableCheck = new QCheckBox(tr("%1").arg(k_ENABLE_LABEL), this);
    enableCheck->hide();
    connect(enableCheck, &QCheckBox::toggled, this, &ScheduleDialog::onEnableToggled);

    hintLabel = new QLabel(this);
    hintLabel->setTextFormat(Qt::RichText);
    hintLabel->setOpenExternalLinks(false);
    hintLabel->setTextInteractionFlags(Qt::TextBrowserInteraction | Qt::LinksAccessibleByMouse);
    hintLabel->setStyleSheet(ScheduleDialogStyling::Styles::HINT_LABEL_STYLE);
    connect(hintLabel, &QLabel::linkActivated, this, &ScheduleDialog::onHintLinkActivated);

    settingsGroup = new QGroupBox(tr("%1").arg(k_GROUP_TITLE), this);
    settingsGroup->setStyleSheet(ScheduleDialogStyling::Styles::GROUPBOX_STYLE);

    dateTimeLabel  = new QLabel(tr("%1").arg(k_DATETIME_LABEL), this);
    dateTimeEdit   = new QDateTimeEdit(this);
    dateTimeEdit->setDisplayFormat(k_DATETIME_DISPLAY_FORMAT);
    dateTimeEdit->setCalendarPopup(true);
    {
        const QDateTime minAllowed = nextMinuteFromNow();
        dateTimeEdit->setMinimumDateTime(minAllowed);
        dateTimeEdit->setDateTime(minAllowed);
    }

    recurrenceLabel = new QLabel(tr("%1").arg(k_RECURRENCE_LABEL), this);
    recurrenceBox   = new QComboBox(this);
    recurrenceBox->addItem(tr("%1").arg(k_RECURRENCE_ONCE));
    recurrenceBox->addItem(tr("%1").arg(k_RECURRENCE_DAILY));
    recurrenceBox->addItem(tr("%1").arg(k_RECURRENCE_WEEKLY));
    recurrenceBox->addItem(tr("%1").arg(k_RECURRENCE_MONTHLY));
    connect(recurrenceBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ScheduleDialog::onRecurrenceChanged);

    auto* form = new QFormLayout;
    form->addRow(dateTimeLabel,   dateTimeEdit);
    form->addRow(recurrenceLabel, recurrenceBox);
    settingsGroup->setLayout(form);

    const QString lockedTip = tr("%1").arg(k_TOOLTIP_LOCKED);
    dateTimeEdit->setToolTip(lockedTip);
    recurrenceBox->setToolTip(lockedTip);
    dateTimeLabel->setToolTip(lockedTip);
    recurrenceLabel->setToolTip(lockedTip);

    okButton = new QPushButton(tr("%1").arg(k_BUTTON_OK_TEXT), this);
    connect(okButton, &QPushButton::clicked, this, &ScheduleDialog::onAcceptClicked);

    auto* buttons = new QHBoxLayout;
    buttons->addStretch();
    buttons->addWidget(okButton);

    auto* main = new QVBoxLayout(this);
    main->addWidget(hintLabel);
    main->addWidget(settingsGroup);
    main->addStretch();
    main->addLayout(buttons);

    enableCheck->setChecked(false);
    onEnableToggled(enableCheck->isChecked());
}

// Applies a ScheduleConfig to the dialog
void ScheduleDialog::setConfig(const ScheduleConfig& cfg) {
    const QSignalBlocker blockEnable(enableCheck);
    const QSignalBlocker blockRecur(recurrenceBox);

    enableCheck->setChecked(cfg.enabled);

    QDateTime dt = forceSecondsZero(cfg.when);
    dt = clampToNextMinute(dt);

    const QDateTime minAllowed = nextMinuteFromNow();
    dateTimeEdit->setMinimumDateTime(minAllowed);
    dateTimeEdit->setDateTime(dt);

    recurrenceBox->setCurrentIndex(recurrenceToIndex(cfg.recur));
    onEnableToggled(cfg.enabled);
}

// Reads the current ScheduleConfig from the dialog
ScheduleDialog::ScheduleConfig ScheduleDialog::config() const {
    ScheduleConfig out;
    out.enabled = enableCheck->isChecked();
    const QDateTime chosen = forceSecondsZero(dateTimeEdit->dateTime());
    out.when = chosen;
    out.recur = indexToRecurrence(recurrenceBox->currentIndex());
    return out;
}

// Handles accept button click
void ScheduleDialog::onAcceptClicked() {
    if (!enableCheck->isChecked()) {
        emit scheduleSaved(config());
        return;
    }

    const QDateTime minAllowed = nextMinuteFromNow();
    const QDateTime chosen = forceSecondsZero(dateTimeEdit->dateTime());

    if (chosen < minAllowed) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            tr("%1").arg(k_TIME_INVALID_TITLE),
            tr("%1").arg(k_TIME_INVALID_BODY),
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

// Handles recurrence combo change
void ScheduleDialog::onRecurrenceChanged(int /*index*/) {
    // Reserved for future behavior
}

// Handles enable checkbox toggle
void ScheduleDialog::onEnableToggled(bool checked) {
    settingsGroup->setEnabled(checked);
    updateStatusBadge(checked);
    updateHint(checked);
}

// Handles clickable hint link
void ScheduleDialog::onHintLinkActivated(const QString& link) {
    if (link == k_LINK_ENABLE) {
        enableCheck->setChecked(true);
    } else if (link == k_LINK_DISABLE) {
        enableCheck->setChecked(false);
    }
}

// Updates badge (currently placeholder)
void ScheduleDialog::updateStatusBadge(bool /*enabled*/) {
    // Placeholder for future badge/indicator
}

// Updates hint label
void ScheduleDialog::updateHint(bool enabled) {
    hintLabel->setText(enabled ? tr("%1").arg(k_HINT_ENABLED_HTML)
                               : tr("%1").arg(k_HINT_DISABLED_HTML));
    hintLabel->setVisible(true);
}

// Normalizes a QDateTime to zero seconds
QDateTime ScheduleDialog::forceSecondsZero(const QDateTime& dt) {
    QDateTime out = dt;
    const QTime t = dt.time();
    out.setTime(QTime(t.hour(), t.minute(), 0));
    return out;
}

// Maps index to Recurrence enum
ScheduleDialog::Recurrence ScheduleDialog::indexToRecurrence(int idx) {
    static constexpr std::array<Recurrence, 4> map = {
        Recurrence::Once, Recurrence::Daily, Recurrence::Weekly, Recurrence::Monthly
    };
    if (idx >= 0 && idx < static_cast<int>(map.size())) {
        return map[static_cast<std::size_t>(idx)];
    }
    return Recurrence::Once;
}

// Maps Recurrence enum to combo index
int ScheduleDialog::recurrenceToIndex(Recurrence r) {
    switch (r) {
    case Recurrence::Once:    return IDX_ONCE;
    case Recurrence::Daily:   return IDX_DAILY;
    case Recurrence::Weekly:  return IDX_WEEKLY;
    case Recurrence::Monthly: return IDX_MONTHLY;
    }
    return IDX_ONCE;
}
