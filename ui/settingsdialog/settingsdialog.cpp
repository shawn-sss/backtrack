// filename: settingsdialog.cpp

// Project includes
#include "settingsdialog.h"
#include "settingsdialogconstants.h"
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
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QSignalBlocker>
#include <QSpacerItem>
#include <QStackedWidget>
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
    label->setStyleSheet(QStringLiteral("color: gray; font-size: 11px;"));
    label->setWordWrap(true);
    return label;
}

} // namespace

// Constructs SettingsDialog and sets up layout
SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowFlags(Qt::Dialog);
    setMinimumSize(QSize(SettingsDialogConstants::k_DIALOG_MIN_WIDTH,
                         SettingsDialogConstants::k_DIALOG_MIN_HEIGHT));
    setupLayout();
}

// Sets up main dialog layout
void SettingsDialog::setupLayout() {
    using namespace SettingsDialogConstants;

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN);
    mainLayout->setSpacing(k_MAIN_SPACING);

    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(k_MAIN_SPACING);

    categoryList = new QListWidget(centralWidget);
    categoryList->addItem(tr(k_CATEGORY_USER));
    categoryList->addItem(tr(k_CATEGORY_SYSTEM));
    categoryList->setFixedWidth(k_CATEGORY_LIST_WIDTH);
    centralLayout->addWidget(categoryList);

    settingsStack = new QStackedWidget(centralWidget);
    settingsStack->addWidget(createUserSettingsPage());
    settingsStack->addWidget(createSystemSettingsPage());
    centralLayout->addWidget(settingsStack);

    connect(categoryList, &QListWidget::currentRowChanged, settingsStack, &QStackedWidget::setCurrentIndex);
    categoryList->setCurrentRow(0);

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

    mainLayout->addWidget(centralWidget);
    mainLayout->addWidget(buttonBox);
}

// Creates user settings page
QWidget* SettingsDialog::createUserSettingsPage() {
    using namespace SettingsDialogConstants;

    auto* widget = new QWidget();
    auto* layout = new QFormLayout(widget);

    layout->addRow(createBoldLabel(tr(k_LABEL_BACKUP_PREFIX)));
    layout->addRow(createGraySmallLabel(k_DESC_BACKUP_SUBTITLE));

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
    layout->addRow(tr(k_LABEL_BACKUP_PREFIX), backupPrefixEdit);

    layout->addRow(createGraySmallLabel(k_DESC_BACKUP_INFO));
    layout->addItem(new QSpacerItem(0, 12, QSizePolicy::Minimum, QSizePolicy::Fixed));

    layout->addRow(createBoldLabel(tr(k_LABEL_CLOSE_BEHAVIOR)));
    layout->addRow(createGraySmallLabel(tr(k_LABEL_CLOSE_BEHAVIOR_DESC)));

    minimizeOnCloseCheckbox = new QCheckBox(tr(k_LABEL_MINIMIZE_ON_CLOSE));
    {
        auto* userMgr = ServiceDirector::getInstance().getUserServiceManager();
        const bool currentMinimizeSetting = userMgr->settings()
                                                .value(UserServiceKeys::MinimizeOnCloseKey)
                                                .toBool(true);
        minimizeOnCloseCheckbox->setChecked(currentMinimizeSetting);
    }
    layout->addRow(minimizeOnCloseCheckbox);

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    return widget;
}

// Creates system settings page
QWidget* SettingsDialog::createSystemSettingsPage() {
    using namespace SettingsDialogConstants;

    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    layout->addWidget(new QLabel(tr(k_LABEL_THEME)));

    themeComboBox = new QComboBox(widget);
    themeComboBox->addItem(tr(k_LABEL_THEME_SYSTEM_DEFAULT), static_cast<int>(UserThemePreference::Auto));
    themeComboBox->addItem(tr(k_LABEL_THEME_LIGHT_MODE),     static_cast<int>(UserThemePreference::Light));
    themeComboBox->addItem(tr(k_LABEL_THEME_DARK_MODE),      static_cast<int>(UserThemePreference::Dark));
    layout->addWidget(themeComboBox);

    {
        const int index = themeComboBox->findData(static_cast<int>(ServiceDirector::getInstance().getThemePreference()));
        if (index != -1) {
            QSignalBlocker blocker(themeComboBox);
            themeComboBox->setCurrentIndex(index);
        }
    }

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    auto* buttonRow = new QWidget(widget);
    auto* rowLayout = new QHBoxLayout(buttonRow);
    rowLayout->setSpacing(12);
    rowLayout->setContentsMargins(0, 24, 0, 0);

    resetBackupArchiveButton = new QPushButton(k_BUTTON_RESET_BACKUP, buttonRow);
    clearAppDataButton       = new QPushButton(k_BUTTON_CLEAR_APP,   buttonRow);

    Shared::UI::applyButtonTooltipAndCursor(resetBackupArchiveButton, k_TOOLTIP_RESET_BACKUP);
    Shared::UI::applyButtonTooltipAndCursor(clearAppDataButton,       k_TOOLTIP_CLEAR_APP);

    resetBackupArchiveButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    clearAppDataButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    rowLayout->addWidget(resetBackupArchiveButton);
    rowLayout->addWidget(clearAppDataButton);
    layout->addWidget(buttonRow);

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
                PromptDialog::Ok
                );
            return;
        }

        const auto confirm = PromptDialog::showDialog(
            this,
            PromptDialog::Question,
            k_CONFIRM_RESET_TITLE,
            k_CONFIRM_RESET_MESSAGE.arg(backupLocation),
            QString(),
            PromptDialog::Yes | PromptDialog::No,
            PromptDialog::No
            );
        if (confirm != PromptDialog::Yes) return;

        emit requestBackupReset(backupLocation, QStringLiteral("reset"));
    });

    return widget;
}

// Handles save button clicked
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
            PromptDialog::Ok
            );
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
