// Project includes
#include "toolbarmanager.h"
#include "toolbarmanagerstyling.h"
#include "toolbarmanagerconstants.h"
#include "../../config/configsettings/app_settings.h"
#include "../../config/configdirector/configdirector.h"
#include "../../ui/settingsdialog/settingsdialog.h"
#include "../../ui/mainwindow/mainwindowlabels.h"
#include "../../ui/mainwindow/mainwindowmessages.h"

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

// Constructor that creates toolbar actions
ToolbarManager::ToolbarManager(QObject* parent)
    : QObject(parent),
    actions{new QAction(this), new QAction(this), new QAction(this), new QAction(this)} {}

// Initializes toolbar style and contents
void ToolbarManager::initialize(QToolBar* toolBar) {
    setupAppearance(toolBar);
    createActions();
    addActions(toolBar);
    applyCursorStyle(toolBar);
}

// Configures toolbar style and layout settings
void ToolbarManager::setupAppearance(QToolBar* toolBar) {
    toolBar->setObjectName("MainToolBar");
    toolBar->setOrientation(Qt::Vertical);
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    toolBar->setVisible(true);
    toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBar->setStyleSheet(ToolbarStyling::BASE_STYLE);
    toolBar->setIconSize(ToolbarStyling::DEFAULT_ICON_SIZE);
}

// Creates and connects toolbar actions
void ToolbarManager::createActions() {
    actions[0]->setText(Labels::Toolbar::k_SETTINGS);
    actions[0]->setIcon(QIcon(Resources::Toolbar::k_SETTINGS_ICON_PATH));
    connect(actions[0], &QAction::triggered, this, &ToolbarManager::showSettings);

    actions[1]->setText(Labels::Toolbar::k_EXIT);
    actions[1]->setIcon(QIcon(Resources::Toolbar::k_EXIT_ICON_PATH));
    connect(actions[1], &QAction::triggered, qApp, &QApplication::quit);

    actions[2]->setText(Labels::Toolbar::k_HELP);
    actions[2]->setIcon(QIcon(Resources::Toolbar::k_HELP_ICON_PATH));
    connect(actions[2], &QAction::triggered, this, &ToolbarManager::showHelp);

    actions[3]->setText(Labels::Toolbar::k_ABOUT);
    actions[3]->setIcon(QIcon(Resources::Toolbar::k_ABOUT_ICON_PATH));
    connect(actions[3], &QAction::triggered, this, &ToolbarManager::showAbout);
}

// Adds actions and spacer to the toolbar
void ToolbarManager::addActions(QToolBar* toolBar) {
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
void ToolbarManager::applyCursorStyle(QToolBar* toolBar) {
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
void ToolbarManager::showSettings() {
    SettingsDialog dialog(qobject_cast<QWidget*>(parent()));
    dialog.exec();
}

// Shows help message with extended details
void ToolbarManager::showHelp() {
    const QString extendedMessage = QString(HelpInfo::k_HELP_EXTENDED_MESSAGE)
    .arg(ConfigDirector::getInstance().getAppInstallDirPublic());

    QMessageBox::information(
        qobject_cast<QWidget*>(parent()),
        HelpInfo::k_HELP_WINDOW_TITLE,
        HelpInfo::k_HELP_WINDOW_MESSAGE + extendedMessage
        );
}

// Shows about dialog with app info
void ToolbarManager::showAbout() {
    const QString aboutText = QString(AboutInfo::k_ABOUT_WINDOW_MESSAGE)
    .arg(AppInfo::k_APP_VERSION,
         AppInfo::k_APP_NAME,
         AppInfo::k_APP_AUTHOR_NAME);

    QMessageBox::information(
        qobject_cast<QWidget*>(parent()),
        AboutInfo::k_ABOUT_WINDOW_TITLE,
        aboutText
        );
}

// Returns Settings action
QAction* ToolbarManager::getActionOpenSettings() const {
    return actions[0];
}

// Returns Exit action
QAction* ToolbarManager::getActionExit() const {
    return actions[1];
}

// Returns Help action
QAction* ToolbarManager::getActionHelp() const {
    return actions[2];
}

// Returns About action
QAction* ToolbarManager::getActionAbout() const {
    return actions[3];
}
