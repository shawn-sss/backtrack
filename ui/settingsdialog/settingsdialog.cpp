// Project includes
#include "settingsdialog.h"
#include "settingsdialogstyling.h"
#include "../../config/_constants.h"
#include "../../config/configmanager/configmanager.h"
#include "../../config/thememanager/thememanager.h"

// Qt includes
#include <QFontMetrics>
#include <QLabel>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QStackedWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>

using namespace SettingsDialogStyling;

// Constructor for the settings dialog
SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog);
    setMinimumSize(QSize(600, 400));
    setupLayout();
}

// Destructor
SettingsDialog::~SettingsDialog() = default;

// Sets up the layout and widgets for the dialog
void SettingsDialog::setupLayout() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(10);

    // Category list on the left
    categoryList = new QListWidget(centralWidget);
    categoryList->addItem("User Settings");
    categoryList->addItem("System Settings");
    categoryList->setFixedWidth(150);
    centralLayout->addWidget(categoryList);

    // Settings stack on the right
    settingsStack = new QStackedWidget(centralWidget);
    settingsStack->addWidget(createUserSettingsPage());
    settingsStack->addWidget(createSystemSettingsPage());
    centralLayout->addWidget(settingsStack);

    connect(categoryList, &QListWidget::currentRowChanged, settingsStack, &QStackedWidget::setCurrentIndex);
    categoryList->setCurrentRow(0);

    // Only include the OK button (no Cancel)
    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    saveButton = buttonBox->button(QDialogButtonBox::Ok);
    saveButton->setText("Save");
    saveButton->setCursor(Qt::PointingHandCursor);

    // Set fixed width to accommodate icon + text when updating label
    QFontMetrics fm(saveButton->font());
    int saveWidth = fm.horizontalAdvance("🟢 Saved") + 40;
    saveButton->setFixedWidth(saveWidth);

    // Connect Save button signal
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);

    // Setup cooldown timer for save feedback
    saveCooldownTimer = new QTimer(this);
    saveCooldownTimer->setSingleShot(true);
    connect(saveCooldownTimer, &QTimer::timeout, this, [this]() {
        saveButton->setText("Save");
        saveButton->setEnabled(true);
        saveButton->setStyleSheet(QString());
    });

    mainLayout->addWidget(centralWidget);
    mainLayout->addWidget(buttonBox);
}

// Creates the user settings page
QWidget* SettingsDialog::createUserSettingsPage() {
    auto* widget = new QWidget();
    auto* layout = new QFormLayout(widget);

    backupPrefixEdit = new QLineEdit(widget);
    backupPrefixEdit->setText(ConfigManager::getInstance().getBackupPrefix());
    layout->addRow("Backup Prefix:", backupPrefixEdit);

    layout->addRow(new QLabel("Modify the prefix used when creating backups."));
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    return widget;
}

// Creates the system settings page
QWidget* SettingsDialog::createSystemSettingsPage() {
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    layout->addWidget(new QLabel("Theme:"));

    themeComboBox = new QComboBox(widget);
    themeComboBox->addItem("System Default", static_cast<int>(UserThemePreference::Auto));
    themeComboBox->addItem("Light Mode", static_cast<int>(UserThemePreference::Light));
    themeComboBox->addItem("Dark Mode", static_cast<int>(UserThemePreference::Dark));
    layout->addWidget(themeComboBox);

    // Restore saved preference
    UserThemePreference savedPref = ConfigManager::getInstance().getThemePreference();
    int index = themeComboBox->findData(static_cast<int>(savedPref));
    if (index != -1) {
        QSignalBlocker blocker(themeComboBox);
        themeComboBox->setCurrentIndex(index);
    }

    layout->addStretch();
    return widget;
}

// Handles the Save button click logic
void SettingsDialog::onSaveClicked() {
    backupPrefixEdit->clearFocus();
    QString newPrefix = backupPrefixEdit->text().trimmed();

    // Save new settings to config
    ConfigManager::getInstance().setBackupPrefix(newPrefix);

    auto selectedTheme = static_cast<UserThemePreference>(themeComboBox->currentData().toInt());
    ConfigManager::getInstance().setThemePreference(selectedTheme);
    ThemeManager::applyTheme();

    // Update save button to give visual feedback
    saveButton->setText("✔️ Saved");
    saveButton->setEnabled(false);
    saveButton->setStyleSheet(COOLDOWN_BUTTON_STYLE);

    saveCooldownTimer->start(3000);
}
