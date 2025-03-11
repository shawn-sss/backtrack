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
    : QDialog(parent), titleBar(nullptr), dragging(false) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setMinimumSize(QSize(600, 400));

    setupLayout();
    setupConnections();
}

// Destructor
SettingsDialog::~SettingsDialog() = default;

// Sets up the layout for the settings dialog
void SettingsDialog::setupLayout() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    titleBar = setupCustomTitleBar(this, TitleBarMode::Dialog);
    mainLayout->addWidget(titleBar);

    auto *contentArea = new QWidget(this);
    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(10, 10, 10, 10);
    contentLayout->setSpacing(10);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, contentArea);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::onCancelClicked);

    contentLayout->addStretch();
    contentLayout->addWidget(buttonBox);

    mainLayout->addWidget(contentArea, 1);
}

// Establishes signal connections for title bar actions
void SettingsDialog::setupConnections() {
    if (titleBar) {
        connect(titleBar, &CustomTitleBar::minimizeRequested, this, &SettingsDialog::showMinimized);
        connect(titleBar, &CustomTitleBar::closeRequested, this, &SettingsDialog::close);
    }
}

// Handles saving and closing the dialog
void SettingsDialog::onSaveClicked() {
    accept();
}

// Handles canceling and closing the dialog
void SettingsDialog::onCancelClicked() {
    reject();
}

// Handles mouse press event for window dragging
void SettingsDialog::mousePressEvent(QMouseEvent *event) {
    if (titleBar && titleBar->rect().contains(titleBar->mapFromGlobal(event->globalPosition().toPoint()))) {
        Utils::UI::handleMousePress(this, event, dragging, lastMousePosition);
    } else {
        event->ignore();
    }
}

// Handles mouse move event for window dragging
void SettingsDialog::mouseMoveEvent(QMouseEvent *event) {
    if (dragging) {
        Utils::UI::handleMouseMove(this, event, dragging, lastMousePosition);
    }
}

// Handles mouse release event for window dragging
void SettingsDialog::mouseReleaseEvent(QMouseEvent *event) {
    Utils::UI::handleMouseRelease(event, dragging);
}
