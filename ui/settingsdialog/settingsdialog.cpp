#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // Connect button signals to slots
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::onCancelClicked);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onSaveClicked()
{
    // TODO: Save user settings
    accept(); // Closes dialog with success status
}

void SettingsDialog::onCancelClicked()
{
    reject(); // Closes dialog without saving
}
