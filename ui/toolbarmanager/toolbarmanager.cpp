// Project includes same directory
#include "toolbarmanager.h"
#include "toolbarmanagerstyling.h"

// Project includes different directory
#include "../../config/_constants.h"
#include "../../config/resources_settings.h"

// Built-in Qt includes
#include <QFile>
#include <QPixmap>
#include <QPainter>
#include <QSvgRenderer>
#include <QToolButton>

// Built-in standard C++ library
#include <array>

// Constructor - Initializes the toolbar manager and actions
ToolbarManager::ToolbarManager(QObject *parent)
    : QObject(parent), actions{new QAction(this), new QAction(this), new QAction(this), new QAction(this)} {}

// Initializes the toolbar with appearance settings and actions
void ToolbarManager::initialize(QToolBar *toolBar) {
    setupAppearance(toolBar);
    addActions(toolBar);
}

// Configures the appearance of the toolbar
void ToolbarManager::setupAppearance(QToolBar *toolBar) {
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setVisible(true);
    toolBar->setIconSize(ToolbarStyles::DEFAULT_ICON_SIZE);
    toolBar->setStyleSheet(ToolbarStyles::DEFAULT_STYLE);
    toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

// Adds actions to the toolbar
void ToolbarManager::addActions(QToolBar *toolBar) {
    toolBar->clear();

    // Define labels and icons for each action
    const std::array<std::pair<QString, QString>, 4> actionData = {{
        {Labels::Toolbar::SETTINGS, Resources::Toolbar::SETTINGS_ICON_PATH},
        {Labels::Toolbar::EXIT, Resources::Toolbar::EXIT_ICON_PATH},
        {Labels::Toolbar::HELP, Resources::Toolbar::HELP_ICON_PATH},
        {Labels::Toolbar::ABOUT, Resources::Toolbar::ABOUT_ICON_PATH}
    }};

    // Add actions to the toolbar
    for (size_t i = 0; i < actions.size(); ++i) {
        actions[i]->setText(actionData[i].first);
        actions[i]->setIcon(QIcon(actionData[i].second));
        toolBar->addAction(actions[i]);
    }

    // Add a spacer to align the exit button to the right
    QWidget *spacer = new QWidget(toolBar);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    // Move the exit button to the right-most position
    toolBar->addAction(actions[1]);
}

// Returns the action for opening settings
QAction* ToolbarManager::getActionOpenSettings() const {
    return actions[0];
}

// Returns the action for exiting the application
QAction* ToolbarManager::getActionExit() const {
    return actions[1];
}

// Returns the action for accessing help
QAction* ToolbarManager::getActionHelp() const {
    return actions[2];
}

// Returns the action for displaying the about dialog
QAction* ToolbarManager::getActionAbout() const {
    return actions[3];
}
