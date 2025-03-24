// Project includes same directory
#include "settingsdialog.h"

// Project includes different directory
#include "../../config/_constants.h"
#include "../../core/utils/common_utils/utils.h"

// Built-in Qt includes
#include <QVBoxLayout>
#include <QDialogButtonBox>

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

    auto *contentArea = new QWidget(this);
    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(10);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, contentArea);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::onCancelClicked);

    contentLayout->addStretch();
    contentLayout->addWidget(buttonBox);

    mainLayout->addWidget(contentArea);
}

// Handles saving and closing the dialog
void SettingsDialog::onSaveClicked() {
    accept();
}

// Handles canceling and closing the dialog
void SettingsDialog::onCancelClicked() {
    reject();
}
