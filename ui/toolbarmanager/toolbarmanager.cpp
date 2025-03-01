#include "toolbarmanager.h"
#include "toolbarmanagerstyling.h"

#include "../../config/_constants.h"
#include "../../config/resources_settings.h"

#include <QFile>
#include <QToolButton>
#include <QSvgRenderer>
#include <QPainter>
#include <QPixmap>

// Constructor
ToolbarManager::ToolbarManager(QObject *parent)
    : QObject(parent),
    actionOpenSettings(new QAction(this)),
    actionExit(new QAction(this)),
    actionHelp(new QAction(this)),
    actionAbout(new QAction(this)) {}

// Initializes the toolbar by applying appearance and adding actions
void ToolbarManager::initialize(QToolBar *toolBar) {
    setupAppearance(toolBar);
    addActions(toolBar);
}

// Sets toolbar appearance and basic behavior
void ToolbarManager::setupAppearance(QToolBar *toolBar) {
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setVisible(true);
    toolBar->setIconSize(ToolbarStyles::DEFAULT_ICON_SIZE);
    toolBar->setStyleSheet(ToolbarStyles::DEFAULT_STYLE);
    toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

// Adds all actions to the toolbar
void ToolbarManager::addActions(QToolBar *toolBar) {
    toolBar->clear();

    actionOpenSettings->setText(Labels::Toolbar::SETTINGS);
    actionExit->setText(Labels::Toolbar::EXIT);
    actionHelp->setText(Labels::Toolbar::HELP);
    actionAbout->setText(Labels::Toolbar::ABOUT);

    actionOpenSettings->setIcon(QIcon(Resources::Toolbar::SETTINGS_ICON_PATH));
    actionExit->setIcon(QIcon(Resources::Toolbar::EXIT_ICON_PATH));
    actionHelp->setIcon(QIcon(Resources::Toolbar::HELP_ICON_PATH));
    actionAbout->setIcon(QIcon(Resources::Toolbar::ABOUT_ICON_PATH));

    toolBar->addAction(actionOpenSettings);
    toolBar->addAction(actionHelp);
    toolBar->addAction(actionAbout);

    auto *spacer = new QWidget(toolBar);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    toolBar->addAction(actionExit);
}

// Returns the action for opening settings
QAction* ToolbarManager::getActionOpenSettings() const {
    return actionOpenSettings;
}

// Returns the action for exiting the app
QAction* ToolbarManager::getActionExit() const {
    return actionExit;
}

// Returns the action for opening help
QAction* ToolbarManager::getActionHelp() const {
    return actionHelp;
}

// Returns the action for opening the about dialog
QAction* ToolbarManager::getActionAbout() const {
    return actionAbout;
}
