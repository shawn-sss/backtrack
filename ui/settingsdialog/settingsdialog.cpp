// Project includes same directory
#include "settingsdialog.h"

// Project includes different directory
#include "../../config/_constants.h"
#include "../../config/configmanager/configmanager.h"

// Built-in Qt includes
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QLineEdit>

// Constructor
SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowFlags(Qt::Dialog);
    setMinimumSize(QSize(600, 400));
    setupLayout();
}

// Destructor
SettingsDialog::~SettingsDialog() = default;

// Sets up the layout for the settings dialog
void SettingsDialog::setupLayout() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    auto *centralWidget = new QWidget(this);
    auto *centralLayout = new QHBoxLayout(centralWidget);
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

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::onCancelClicked);

    mainLayout->addWidget(centralWidget);
    mainLayout->addWidget(buttonBox);
}

// User Settings Page
QWidget* SettingsDialog::createUserSettingsPage() {
    auto *widget = new QWidget();
    auto *layout = new QFormLayout(widget);

    backupPrefixEdit = new QLineEdit(widget);
    backupPrefixEdit->setText(ConfigManager::getInstance().getBackupPrefix());
    layout->addRow("Backup Prefix:", backupPrefixEdit);

    layout->addRow(new QLabel("Modify the prefix used when creating backups."));
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    return widget;
}

// System Settings Page (placeholder)
QWidget* SettingsDialog::createSystemSettingsPage() {
    auto *widget = new QWidget();
    auto *layout = new QVBoxLayout(widget);
    layout->addWidget(new QLabel("System Settings go here."));
    layout->addStretch();
    return widget;
}

// Handles saving and closing the dialog
void SettingsDialog::onSaveClicked() {
    QString newPrefix = backupPrefixEdit->text().trimmed();
    ConfigManager::getInstance().setBackupPrefix(newPrefix);
    accept();
}

// Handles canceling and closing the dialog
void SettingsDialog::onCancelClicked() {
    reject();
}
