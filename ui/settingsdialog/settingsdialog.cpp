// Project includes
#include "../../config/_constants.h"
#include "../../config/configmanager/configmanager.h"
#include "../../config/thememanager/thememanager.h"
#include "settingsdialog.h"

// Qt includes
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QStackedWidget>
#include <QSettings>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QFile>

// Constructor
SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog);
    setMinimumSize(QSize(600, 400));
    setupLayout();
}

// Destructor
SettingsDialog::~SettingsDialog() = default;

// Layout setup
void SettingsDialog::setupLayout() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(10);

    categoryList = new QListWidget(centralWidget);
    categoryList->addItem("User Settings");
    categoryList->addItem("System Settings");
    categoryList->setFixedWidth(150);
    centralLayout->addWidget(categoryList);

    settingsStack = new QStackedWidget(centralWidget);
    settingsStack->addWidget(createUserSettingsPage());
    settingsStack->addWidget(createSystemSettingsPage());
    centralLayout->addWidget(settingsStack);

    connect(categoryList, &QListWidget::currentRowChanged, settingsStack, &QStackedWidget::setCurrentIndex);
    categoryList->setCurrentRow(0);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::onCancelClicked);

    mainLayout->addWidget(centralWidget);
    mainLayout->addWidget(buttonBox);
}

// Create user settings page
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

// Create system settings page
QWidget* SettingsDialog::createSystemSettingsPage() {
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    layout->addWidget(new QLabel("Theme:"));
    themeComboBox = new QComboBox(widget);
    themeComboBox->addItem("System Default", static_cast<int>(UserThemePreference::Auto));
    themeComboBox->addItem("Light Mode", static_cast<int>(UserThemePreference::Light));
    themeComboBox->addItem("Dark Mode", static_cast<int>(UserThemePreference::Dark));
    layout->addWidget(themeComboBox);

    UserThemePreference savedPref = ConfigManager::getInstance().getThemePreference();
    int index = themeComboBox->findData(static_cast<int>(savedPref));
    if (index != -1) {
        QSignalBlocker blocker(themeComboBox);
        themeComboBox->setCurrentIndex(index);
    }

    connect(themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [](int) {
        ThemeManager::applyTheme();
    });

    layout->addStretch();
    return widget;
}

// Handle save button click
void SettingsDialog::onSaveClicked() {
    QString newPrefix = backupPrefixEdit->text().trimmed();
    ConfigManager::getInstance().setBackupPrefix(newPrefix);

    auto selectedTheme = static_cast<UserThemePreference>(themeComboBox->currentData().toInt());
    ConfigManager::getInstance().setThemePreference(selectedTheme);
    ThemeManager::applyTheme();

    accept();
}

// Handle cancel button click
void SettingsDialog::onCancelClicked() {
    reject();
}
