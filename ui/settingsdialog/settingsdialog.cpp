#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "../../config/_constants.h"
#include "../../core/utils/common_utils/utils.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>

// Constructor
SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent), dragging(false) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setMinimumSize(600, 400);

    setupLayout();
    setupConnections();
}

// Destructor
SettingsDialog::~SettingsDialog() {}

// Sets up the dialog layout, including title bar and content area
void SettingsDialog::setupLayout() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    titleBar = setupCustomTitleBar(this, TitleBarMode::Dialog);
    mainLayout->addWidget(titleBar);

    auto *contentArea = new QWidget(this);
    auto *contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(10, 10, 10, 10);
    contentLayout->setSpacing(10);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::onCancelClicked);

    contentLayout->addStretch();
    contentLayout->addWidget(buttonBox);

    mainLayout->addWidget(contentArea, 1);
}

// Sets up signal connections to handle window controls
void SettingsDialog::setupConnections() {
    if (titleBar) {
        connect(titleBar, &CustomTitleBar::minimizeRequested, this, &SettingsDialog::showMinimized);
        connect(titleBar, &CustomTitleBar::closeRequested, this, &SettingsDialog::close);
    }
}

// Accepts and saves the dialog
void SettingsDialog::onSaveClicked() {
    accept();
}

// Cancels and closes the dialog
void SettingsDialog::onCancelClicked() {
    reject();
}

// Handles mouse press for window dragging
void SettingsDialog::mousePressEvent(QMouseEvent *event) {
    Utils::UI::handleMousePress(this, event, dragging, lastMousePosition);
}

// Handles mouse move for window dragging
void SettingsDialog::mouseMoveEvent(QMouseEvent *event) {
    Utils::UI::handleMouseMove(this, event, dragging, lastMousePosition);
}

// Handles mouse release for window dragging
void SettingsDialog::mouseReleaseEvent(QMouseEvent *event) {
    Utils::UI::handleMouseRelease(event, dragging);
}
