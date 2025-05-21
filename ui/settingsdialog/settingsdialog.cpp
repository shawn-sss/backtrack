// Project includes
#include "settingsdialog.h"
#include "settingsdialogconstants.h"
#include "settingsdialogstyling.h"
#include "../../services/ServiceDirector/ServiceDirector.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../../services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.h"

// Qt includes
#include <QApplication>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileInfo>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QSignalBlocker>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

// C++ includes
#include <memory>

using namespace SettingsDialogConstants;
using namespace SettingsDialogStyling;
using ThemeServiceConstants::UserThemePreference;

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowFlags(Qt::Dialog);
    setMinimumSize(QSize(k_DIALOG_MIN_WIDTH, k_DIALOG_MIN_HEIGHT));
    setupLayout();
}

SettingsDialog::~SettingsDialog() = default;

// Constructs and lays out the full settings dialog
void SettingsDialog::setupLayout() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN);
    mainLayout->setSpacing(k_MAIN_SPACING);

    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(k_MAIN_SPACING);

    categoryList = new QListWidget(centralWidget);
    categoryList->addItem("User Settings");
    categoryList->addItem("System Settings");
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

    applyButtonCursorsAndTooltips();

    QFontMetrics fm(saveButton->font());
    int saveWidth = fm.horizontalAdvance(k_BUTTON_SAVE_WIDTH_TEXT) + 40;
    saveButton->setFixedWidth(saveWidth);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);

    saveCooldownTimer = new QTimer(this);
    saveCooldownTimer->setSingleShot(true);
    connect(saveCooldownTimer, &QTimer::timeout, this, [this]() {
        saveButton->setText(k_BUTTON_SAVE_TEXT);
        saveButton->setEnabled(true);
        saveButton->setStyleSheet({});
    });

    mainLayout->addWidget(centralWidget);
    mainLayout->addWidget(buttonBox);
}

// Constructs the user settings form
QWidget* SettingsDialog::createUserSettingsPage() {
    auto* widget = new QWidget();
    auto* layout = new QFormLayout(widget);

    auto* descriptionLabel = new QLabel("Backup Prefix Description:");
    QFont labelFont = descriptionLabel->font();
    labelFont.setBold(true);
    descriptionLabel->setFont(labelFont);
    layout->addRow(descriptionLabel);

    auto* subtitleLabel = new QLabel("This is the first part of each backup name to help group and identify them.");
    subtitleLabel->setStyleSheet("color: gray; font-size: 11px;");
    subtitleLabel->setWordWrap(true);
    layout->addRow(subtitleLabel);

    backupPrefixEdit = new QLineEdit(widget);
    backupPrefixEdit->setText(ServiceDirector::getInstance().getBackupPrefix());
    backupPrefixEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Za-z0-9]{0,12}$"), this));
    backupPrefixEdit->setMaxLength(12);
    layout->addRow(k_LABEL_BACKUP_PREFIX, backupPrefixEdit);

    auto* prefixInfoLabel = new QLabel("Allowed: letters (A–Z, a–z) and digits (0–9), up to 12 characters.");
    prefixInfoLabel->setStyleSheet("color: gray; font-size: 11px;");
    layout->addRow(prefixInfoLabel);

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    return widget;
}

// Constructs the system settings form
QWidget* SettingsDialog::createSystemSettingsPage() {
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    layout->addWidget(new QLabel(k_LABEL_THEME));

    themeComboBox = new QComboBox(widget);
    themeComboBox->addItem(k_LABEL_THEME_SYSTEM_DEFAULT, static_cast<int>(UserThemePreference::Auto));
    themeComboBox->addItem(k_LABEL_THEME_LIGHT_MODE, static_cast<int>(UserThemePreference::Light));
    themeComboBox->addItem(k_LABEL_THEME_DARK_MODE, static_cast<int>(UserThemePreference::Dark));
    layout->addWidget(themeComboBox);

    int index = themeComboBox->findData(static_cast<int>(ServiceDirector::getInstance().getThemePreference()));
    if (index != -1) {
        QSignalBlocker blocker(themeComboBox);
        themeComboBox->setCurrentIndex(index);
    }

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    auto* buttonRow = new QWidget(widget);
    auto* rowLayout = new QHBoxLayout(buttonRow);
    rowLayout->setSpacing(12);
    rowLayout->setContentsMargins(0, 24, 0, 0);

    resetBackupArchiveButton = new QPushButton("📁 Reset Backup Archive", buttonRow);
    resetBackupArchiveButton->setCursor(Qt::PointingHandCursor);
    resetBackupArchiveButton->setToolTip("Delete all backups and logs from the backup directory");
    resetBackupArchiveButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    resetBackupArchiveButton->setStyleSheet(k_RESET_BACKUP_BUTTON_STYLE);
    rowLayout->addWidget(resetBackupArchiveButton);

    clearAppDataButton = new QPushButton("🗑️ Clear App Data", buttonRow);
    clearAppDataButton->setCursor(Qt::PointingHandCursor);
    clearAppDataButton->setToolTip("Remove all data created by this app from your system");
    clearAppDataButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    clearAppDataButton->setStyleSheet(k_CLEAR_APP_BUTTON_STYLE);
    rowLayout->addWidget(clearAppDataButton);

    layout->addWidget(buttonRow);

    connect(clearAppDataButton, &QPushButton::clicked, this, [this]() {
        NotificationServiceManager::instance().suspendNotifications(true);
        const bool success = ServiceDirector::getInstance().uninstallAppWithConfirmation(this);
        NotificationServiceManager::instance().suspendNotifications(false);
        if (success) {
            QApplication::quit();
        }
    });

    connect(resetBackupArchiveButton, &QPushButton::clicked, this, [this]() {
        const QString backupLocation = ServiceDirector::getInstance().getBackupDirectory();
        if (backupLocation.isEmpty() || !QDir(backupLocation).exists()) {
            QMessageBox::warning(this, k_WARNING_INVALID_PATH_TITLE, k_WARNING_INVALID_PATH_MESSAGE);
            return;
        }

        const QMessageBox::StandardButton confirm = QMessageBox::warning(
            this,
            k_CONFIRM_RESET_TITLE,
            k_CONFIRM_RESET_MESSAGE.arg(backupLocation),
            QMessageBox::Yes | QMessageBox::No
            );

        if (confirm != QMessageBox::Yes) return;

        QDir dir(backupLocation);
        const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

        bool success = true;
        for (const QFileInfo& entry : entries) {
            const QString path = entry.absoluteFilePath();
            if (entry.isDir()) {
                if (!QDir(path).removeRecursively()) success = false;
            } else {
                if (!QFile::remove(path)) success = false;
            }
        }

        if (success) {
            QMessageBox::information(this, k_RESET_SUCCESS_TITLE, k_RESET_SUCCESS_MESSAGE);
        } else {
            QMessageBox::critical(this, k_RESET_FAILURE_TITLE, k_RESET_FAILURE_MESSAGE);
        }
    });

    return widget;
}

// Saves user input and applies system settings
void SettingsDialog::onSaveClicked() {
    backupPrefixEdit->clearFocus();
    QString newPrefix = backupPrefixEdit->text().trimmed();

    static const QRegularExpression prefixRegex("^[A-Za-z0-9]+$");
    if (!prefixRegex.match(newPrefix).hasMatch()) {
        QMessageBox::warning(this, "Invalid Prefix", "Only letters (A–Z, a–z) and digits (0–9) are allowed.");
        return;
    }

    ServiceDirector::getInstance().setBackupPrefix(newPrefix);

    auto selectedTheme = static_cast<UserThemePreference>(themeComboBox->currentData().toInt());
    ServiceDirector::getInstance().setThemePreference(selectedTheme);
    ThemeServiceManager::instance().applyTheme();

    saveButton->setText(k_BUTTON_SAVED_TEXT);
    saveButton->setEnabled(false);
    saveButton->setStyleSheet(COOLDOWN_BUTTON_STYLE);

    saveCooldownTimer->start(k_SAVE_FEEDBACK_COOLDOWN_MS);
}

// Configures save button interactivity
void SettingsDialog::applyButtonCursorsAndTooltips() {
    saveButton->setCursor(Qt::PointingHandCursor);
    saveButton->setToolTip("Save your settings and apply changes");
}
