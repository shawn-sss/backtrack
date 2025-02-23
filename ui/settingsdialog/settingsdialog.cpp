#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "../../core/utils/common_utils/utils.h"

#include <QVBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent), ui(std::make_unique<Ui::SettingsDialog>()) {
    ui->setupUi(this);
    titleBar = Utils::setupCustomTitleBar(this, TitleBarMode::Dialog);
    setupConnections();
}

SettingsDialog::~SettingsDialog() {}

// Initialize UI components
void SettingsDialog::setupUIComponents() {
    QWidget *titleBarContainer = findChild<QWidget *>("titleBarContainer");
    if (!titleBarContainer || titleBarContainer->layout()) return;

    auto *layout = new QVBoxLayout(titleBarContainer);
    layout->setContentsMargins(0, 0, 0, 0);
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
