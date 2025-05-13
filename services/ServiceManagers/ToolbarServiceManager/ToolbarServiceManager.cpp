// Project includes
#include "ToolbarServiceManager.h"
#include "ToolbarServiceConstants.h"
#include "ToolbarServiceStyling.h"
#include "../../../ui/mainwindow/mainwindowlabels.h"
#include "../../../ui/settingsdialog/settingsdialog.h"
#include "../../../ui/helpdialog/helpdialog.h"
#include "../../../ui/aboutdialog/aboutdialog.h"

// Qt includes
#include <QAction>
#include <QApplication>
#include <QCursor>
#include <QIcon>
#include <QMessageBox>
#include <QSizePolicy>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>

// Constructor that creates toolbar service actions
ToolbarServiceManager::ToolbarServiceManager(QObject* parent)
    : QObject(parent),
    actions{new QAction(this), new QAction(this), new QAction(this), new QAction(this)} {}

// Initializes toolbar service style and contents
void ToolbarServiceManager::initialize(QToolBar* toolBar) {
    setupAppearance(toolBar);
    createActions();
    addActions(toolBar);
    applyCursorStyle(toolBar);
}

// Configures toolbar service style and layout settings
void ToolbarServiceManager::setupAppearance(QToolBar* toolBar) {
    toolBar->setObjectName("MainToolBar");
    toolBar->setOrientation(Qt::Vertical);
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    toolBar->setVisible(true);
    toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBar->setStyleSheet(ToolbarServiceStyling::BASE_STYLE);
    toolBar->setIconSize(ToolbarServiceStyling::DEFAULT_ICON_SIZE);
}

// Creates and connects toolbar service actions
void ToolbarServiceManager::createActions() {
    actions[0]->setText(Labels::Toolbar::k_SETTINGS);
    actions[0]->setIcon(QIcon(Resources::ToolbarService::k_SETTINGS_ICON_PATH));
    connect(actions[0], &QAction::triggered, this, &ToolbarServiceManager::showSettings);

    actions[1]->setText(Labels::Toolbar::k_EXIT);
    actions[1]->setIcon(QIcon(Resources::ToolbarService::k_EXIT_ICON_PATH));
    connect(actions[1], &QAction::triggered, qApp, &QApplication::quit);

    actions[2]->setText(Labels::Toolbar::k_HELP);
    actions[2]->setIcon(QIcon(Resources::ToolbarService::k_HELP_ICON_PATH));
    connect(actions[2], &QAction::triggered, this, &ToolbarServiceManager::showHelp);

    actions[3]->setText(Labels::Toolbar::k_ABOUT);
    actions[3]->setIcon(QIcon(Resources::ToolbarService::k_ABOUT_ICON_PATH));
    connect(actions[3], &QAction::triggered, this, &ToolbarServiceManager::showAbout);
}

// Adds actions and spacer to the toolbar service
void ToolbarServiceManager::addActions(QToolBar* toolBar) {
    toolBar->clear();
    toolBar->addAction(actions[0]);
    toolBar->addAction(actions[2]);
    toolBar->addAction(actions[3]);

    QWidget* spacer = new QWidget(toolBar);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    toolBar->addAction(actions[1]);
}

// Applies hand cursor and object names to buttons
void ToolbarServiceManager::applyCursorStyle(QToolBar* toolBar) {
    for (int i = 0; i < actions.size(); ++i) {
        QAction* action = actions[i];
        if (QWidget* w = toolBar->widgetForAction(action)) {
            w->setCursor(Qt::PointingHandCursor);
            if (QToolButton* btn = qobject_cast<QToolButton*>(w)) {
                switch (i) {
                case 0: btn->setObjectName("SettingsButton"); break;
                case 1: btn->setObjectName("ExitButton"); break;
                case 2: btn->setObjectName("HelpButton"); break;
                case 3: btn->setObjectName("AboutButton"); break;
                }
            }
        }
    }
}

// Opens the settings dialog
void ToolbarServiceManager::showSettings() {
    SettingsDialog dialog(qobject_cast<QWidget*>(parent()));
    dialog.exec();
}

// Shows help message with extended details
void ToolbarServiceManager::showHelp() {
    HelpDialog helpDialog(qobject_cast<QWidget*>(parent()));
    helpDialog.exec();
}

// Shows about dialog with app info
void ToolbarServiceManager::showAbout() {
    AboutDialog aboutDialog(qobject_cast<QWidget*>(parent()));
    aboutDialog.exec();
}

// Action getters
QAction* ToolbarServiceManager::getActionOpenSettings() const { return actions[0]; }
QAction* ToolbarServiceManager::getActionExit() const          { return actions[1]; }
QAction* ToolbarServiceManager::getActionHelp() const          { return actions[2]; }
QAction* ToolbarServiceManager::getActionAbout() const         { return actions[3]; }
