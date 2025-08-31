// Project includes
#include "settingsdialog.h"
#include "settingsdialogconstants.h"
#include "settingsdialogstyling.h"
#include "../promptdialog/promptdialog.h"
#include "../../services/ServiceDirector/ServiceDirector.h"
#include "../../services/ServiceManagers/UserServiceManager/UserServiceConstants.h"
#include "../../services/ServiceManagers/UserServiceManager/UserServiceManager.h"
#include "../../services/ServiceManagers/BackupServiceManager/BackupServiceManager.h"
#include "../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h"

// Qt includes
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QSignalBlocker>
#include <QSpacerItem>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

// C++ includes
#include <QString>

using ThemeServiceConstants::UserThemePreference;

namespace {

QLabel* createBoldLabel(const QString& text, QWidget* parent = nullptr) {
    auto* label = new QLabel(text, parent);
    QFont font = label->font();
    font.setBold(true);
    label->setFont(font);
    return label;
}

QLabel* createGraySmallLabel(const QString& text, QWidget* parent = nullptr) {
    auto* label = new QLabel(text, parent);
    label->setStyleSheet(SettingsDialogStyling::Styles::DESCRIPTION_LABEL_STYLE);
    label->setWordWrap(true);
    return label;
}

} // namespace

// Construct settings dialog window
SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowFlags(Qt::Dialog);
    setMinimumSize(QSize(SettingsDialogConstants::k_DIALOG_MIN_WIDTH,
                         SettingsDialogConstants::k_DIALOG_MIN_HEIGHT));
    setupLayout();
}

// Setup main layout
void SettingsDialog::setupLayout() {
    using namespace SettingsDialogConstants;

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN);
    mainLayout->setSpacing(k_MAIN_SPACING);

    auto* settingsPage = createSettingsPage();
    mainLayout->addWidget(settingsPage);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    saveButton = buttonBox->button(QDialogButtonBox::Ok);
    saveButton->setText(k_BUTTON_SAVE_TEXT);
    Shared::UI::applyButtonTooltipAndCursor(saveButton, k_TOOLTIP_SAVE_BUTTON);

    QFontMetrics fm(saveButton->font());
    const int saveWidth = fm.horizontalAdvance(k_BUTTON_SAVE_WIDTH_TEXT) + 40;
    saveButton->setMinimumWidth(saveWidth);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);

    saveCooldownTimer = new QTimer(this);
    saveCooldownTimer->setSingleShot(true);
    connect(saveCooldownTimer, &QTimer::timeout, this, [this]() {
        saveButton->setText(k_BUTTON_SAVE_TEXT);
        saveButton->setEnabled(true);
    });

    mainLayout->addWidget(buttonBox, 0, Qt::AlignHCenter);
}

// Create settings page
QWidget* SettingsDialog::createSettingsPage() {
    using namespace SettingsDialogConstants;

    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);
    layout->setSpacing(20);

    auto* userGroup = new QGroupBox(tr(k_CATEGORY_USER_PREFERENCES), widget);
    auto* userLayout = new QFormLayout(userGroup);

    auto* themeBox = new QVBoxLayout();
    themeBox->addWidget(createBoldLabel(tr(k_LABEL_THEME)));
    themeComboBox = new QComboBox(widget);
    themeComboBox->addItem(tr(k_LABEL_THEME_SYSTEM_DEFAULT), static_cast<int>(UserThemePreference::Auto));
    themeComboBox->addItem(tr(k_LABEL_THEME_LIGHT_MODE),     static_cast<int>(UserThemePreference::Light));
    themeComboBox->addItem(tr(k_LABEL_THEME_DARK_MODE),      static_cast<int>(UserThemePreference::Dark));
    themeBox->addWidget(themeComboBox);

    {
        const int index = themeComboBox->findData(static_cast<int>(ServiceDirector::getInstance().getThemePreference()));
        if (index != -1) {
            QSignalBlocker blocker(themeComboBox);
            themeComboBox->setCurrentIndex(index);
        }
    }
    userLayout->addRow(themeBox);

    backupPrefixEdit = new QLineEdit(widget);
    {
        auto* backupMgr = ServiceDirector::getInstance().getBackupServiceManager();
        backupPrefixEdit->setText(backupMgr->getBackupPrefix());
    }
    auto* validator = new QRegularExpressionValidator(
        QRegularExpression(k_BACKUP_PREFIX_REGEX_PATTERN),
        backupPrefixEdit);
    backupPrefixEdit->setValidator(validator);
    backupPrefixEdit->setMaxLength(12);

    auto* backupPrefixBox = new QVBoxLayout();
    backupPrefixBox->addWidget(createBoldLabel(tr(k_LABEL_BACKUP_PREFIX)));
    backupPrefixBox->addWidget(backupPrefixEdit);
    backupPrefixBox->addWidget(createGraySmallLabel(k_DESC_BACKUP_SUBTITLE));
    backupPrefixBox->addWidget(createGraySmallLabel(k_DESC_BACKUP_INFO));
    userLayout->addRow(backupPrefixBox);

    minimizeOnCloseCheckbox = new QCheckBox(tr(k_LABEL_MINIMIZE_ON_CLOSE));
    {
        auto* userMgr = ServiceDirector::getInstance().getUserServiceManager();
        const bool currentMinimizeSetting = userMgr->settings()
                                                .value(UserServiceKeys::MinimizeOnCloseKey)
                                                .toBool(true);
        minimizeOnCloseCheckbox->setChecked(currentMinimizeSetting);
    }

    auto* closeBehaviorBox = new QVBoxLayout();
    closeBehaviorBox->addWidget(createBoldLabel(tr(k_LABEL_CLOSE_BEHAVIOR)));
    closeBehaviorBox->addWidget(createGraySmallLabel(tr(k_LABEL_CLOSE_BEHAVIOR_DESC)));
    closeBehaviorBox->addWidget(minimizeOnCloseCheckbox);
    userLayout->addRow(closeBehaviorBox);

    layout->addWidget(userGroup);

    auto* advancedGroup = new QGroupBox(tr(k_CATEGORY_ADVANCED), widget);
    auto* advancedLayout = new QVBoxLayout(advancedGroup);

    auto* buttonRow = new QWidget(widget);
    auto* rowLayout = new QHBoxLayout(buttonRow);
    rowLayout->setSpacing(12);
    rowLayout->setContentsMargins(0, 0, 0, 0);

    resetBackupArchiveButton = new QPushButton(k_BUTTON_RESET_BACKUP, buttonRow);
    clearAppDataButton       = new QPushButton(k_BUTTON_CLEAR_APP,   buttonRow);

    Shared::UI::applyButtonTooltipAndCursor(resetBackupArchiveButton, k_TOOLTIP_RESET_BACKUP);
    Shared::UI::applyButtonTooltipAndCursor(clearAppDataButton,       k_TOOLTIP_CLEAR_APP);

    resetBackupArchiveButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    clearAppDataButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    rowLayout->addWidget(resetBackupArchiveButton);
    rowLayout->addWidget(clearAppDataButton);
    advancedLayout->addWidget(buttonRow);

    connect(clearAppDataButton, &QPushButton::clicked, this, [this]() {
        emit requestAppDataClear();
    });

    connect(resetBackupArchiveButton, &QPushButton::clicked, this, [this]() {
        auto* backupMgr = ServiceDirector::getInstance().getBackupServiceManager();
        const QString backupLocation = backupMgr->getBackupDirectory();
        if (backupLocation.isEmpty() || !QDir(backupLocation).exists()) {
            PromptDialog::showDialog(
                this,
                PromptDialog::Warning,
                k_WARNING_INVALID_PATH_TITLE,
                k_WARNING_INVALID_PATH_MESSAGE,
                QString(),
                PromptDialog::Ok,
                PromptDialog::Ok);
            return;
        }

        const auto confirm = PromptDialog::showDialog(
            this,
            PromptDialog::Question,
            k_CONFIRM_RESET_TITLE,
            k_CONFIRM_RESET_MESSAGE.arg(backupLocation),
            QString(),
            PromptDialog::Yes | PromptDialog::No,
            PromptDialog::No);
        if (confirm != PromptDialog::Yes) return;

        emit requestBackupReset(backupLocation, QStringLiteral("reset"));
    });

    layout->addWidget(advancedGroup);
    return widget;
}

// Handle save button click
void SettingsDialog::onSaveClicked() {
    using namespace SettingsDialogConstants;

    backupPrefixEdit->clearFocus();
    const QString newPrefix = backupPrefixEdit->text().trimmed();

    if (!QRegularExpression(k_BACKUP_PREFIX_REGEX_PATTERN).match(newPrefix).hasMatch()) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            k_WARNING_INVALID_PREFIX_TITLE,
            k_WARNING_INVALID_PREFIX_MESSAGE,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    auto* sd = &ServiceDirector::getInstance();

    sd->getBackupServiceManager()->setBackupPrefix(newPrefix);

    const auto selectedTheme = static_cast<UserThemePreference>(themeComboBox->currentData().toInt());
    sd->setThemePreference(selectedTheme);
    sd->applyTheme();

    const bool minimizeSetting = minimizeOnCloseCheckbox->isChecked();
    auto* userMgr = sd->getUserServiceManager();
    auto& settings = userMgr->settings();
    settings[UserServiceKeys::MinimizeOnCloseKey] = minimizeSetting;
    userMgr->save();

    saveButton->setText(k_BUTTON_SAVED_TEXT);
    saveButton->setEnabled(false);
    saveCooldownTimer->start(k_SAVE_FEEDBACK_COOLDOWN_MS);
}
