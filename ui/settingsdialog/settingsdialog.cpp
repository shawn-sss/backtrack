#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "../../config/_constants.h"
#include "../../core/utils/common_utils/utils.h"

#include <QVBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent) {

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setMinimumSize(600, 400);

    // Create a main layout for the dialog
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create the custom title bar
    titleBar = setupCustomTitleBar(this, TitleBarMode::Dialog);
    mainLayout->addWidget(titleBar);

    // Create content area (this can hold your actual settings widgets)
    auto *contentArea = new QWidget(this);
    auto *contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(10, 10, 10, 10);
    contentLayout->setSpacing(10);

    // Add some example content here if needed (for now just the buttons)
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::reject);

    contentLayout->addStretch();
    contentLayout->addWidget(buttonBox);

    // Add content area to the main layout
    mainLayout->addWidget(contentArea, 1);
}



SettingsDialog::~SettingsDialog() {}

// Initialize UI components
void SettingsDialog::setupUIComponents() {
    QWidget *titleBarContainer = findChild<QWidget *>("titleBarContainer");
    if (!titleBarContainer) return;

    QVBoxLayout *layout = new QVBoxLayout(titleBarContainer);
    layout->setContentsMargins(0, 0, 0, 0);

    // Explicitly enforce expansion when added
    titleBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout->addWidget(titleBar);
}

// Establish signal connections
void SettingsDialog::setupConnections() {
    if (titleBar) {
        connect(titleBar, &CustomTitleBar::minimizeRequested, this, &SettingsDialog::showMinimized);
        connect(titleBar, &CustomTitleBar::closeRequested, this, &SettingsDialog::close);
    }
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::onCancelClicked);
}

// Save action handler
void SettingsDialog::onSaveClicked() {
    accept();
}

// Cancel action handler
void SettingsDialog::onCancelClicked() {
    reject();
}

// Mouse press event handler
void SettingsDialog::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        lastMousePosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

// Mouse move event handler
void SettingsDialog::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - lastMousePosition);
        event->accept();
    }
}

// Mouse release event handler
void SettingsDialog::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    }
}
