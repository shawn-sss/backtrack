#include "toolbarmanager.h"
#include "toolbarmanagerstyling.h"

#include "../../config/_constants.h"
#include "../../config/configmanager/configmanager.h"
#include "../../ui/settingsdialog/settingsdialog.h"

#include <QApplication>
#include <QToolBar>
#include <QToolButton>
#include <QSizePolicy>
#include <QCursor>
#include <QIcon>
#include <QMessageBox>
#include <QWidget>

ToolbarManager::ToolbarManager(QObject *parent)
    : QObject(parent),
    actions{new QAction(this), new QAction(this), new QAction(this), new QAction(this)} {}

void ToolbarManager::initialize(QToolBar *toolBar) {
    setupAppearance(toolBar);
    createActions();
    addActions(toolBar);
    applyCursorStyle(toolBar);
}

void ToolbarManager::setupAppearance(QToolBar *toolBar) {
    toolBar->setOrientation(Qt::Vertical);
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setVisible(true);
    toolBar->setIconSize(ToolbarStyles::DEFAULT_ICON_SIZE);
    toolBar->setStyleSheet(ToolbarStyles::DEFAULT_STYLE);
    toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

void ToolbarManager::createActions() {
    // Settings
    actions[0]->setText(Labels::Toolbar::SETTINGS);
    actions[0]->setIcon(QIcon(Resources::Toolbar::SETTINGS_ICON_PATH));
    connect(actions[0], &QAction::triggered, this, &ToolbarManager::showSettings);

    // Exit
    actions[1]->setText(Labels::Toolbar::EXIT);
    actions[1]->setIcon(QIcon(Resources::Toolbar::EXIT_ICON_PATH));
    connect(actions[1], &QAction::triggered, qApp, &QApplication::quit);

    // Help
    actions[2]->setText(Labels::Toolbar::HELP);
    actions[2]->setIcon(QIcon(Resources::Toolbar::HELP_ICON_PATH));
    connect(actions[2], &QAction::triggered, this, &ToolbarManager::showHelp);

    // About
    actions[3]->setText(Labels::Toolbar::ABOUT);
    actions[3]->setIcon(QIcon(Resources::Toolbar::ABOUT_ICON_PATH));
    connect(actions[3], &QAction::triggered, this, &ToolbarManager::showAbout);
}

void ToolbarManager::addActions(QToolBar *toolBar) {
    toolBar->clear();
    toolBar->addAction(actions[0]); // Settings
    toolBar->addAction(actions[2]); // Help
    toolBar->addAction(actions[3]); // About

    // Spacer to push Exit to bottom
    QWidget *spacer = new QWidget(toolBar);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    toolBar->addAction(actions[1]); // Exit
}

void ToolbarManager::applyCursorStyle(QToolBar *toolBar) {
    for (QAction *action : actions) {
        if (QWidget *w = toolBar->widgetForAction(action)) {
            w->setCursor(Qt::PointingHandCursor);
        }
    }
}

// ---------- Internal Dialog Handlers ----------

void ToolbarManager::showSettings() {
    SettingsDialog dialog(qobject_cast<QWidget*>(parent()));
    dialog.exec();
}

void ToolbarManager::showHelp() {
    const QString extendedMessage = QString(HelpInfo::HELP_EXTENDED_MESSAGE)
    .arg(ConfigManager::getInstance().getAppInstallDirPublic());

    QMessageBox::information(
        qobject_cast<QWidget*>(parent()),
        HelpInfo::HELP_WINDOW_TITLE,
        HelpInfo::HELP_WINDOW_MESSAGE + extendedMessage
        );
}

void ToolbarManager::showAbout() {
    const QString aboutText = QString(AboutInfo::ABOUT_WINDOW_MESSAGE)
    .arg(AppInfo::APP_VERSION,
         AppInfo::APP_DISPLAY_TITLE,
         AppInfo::AUTHOR_NAME);

    QMessageBox::information(
        qobject_cast<QWidget*>(parent()),
        AboutInfo::ABOUT_WINDOW_TITLE,
        aboutText
        );
}

// ---------- Public Access to Actions ----------

QAction* ToolbarManager::getActionOpenSettings() const { return actions[0]; }
QAction* ToolbarManager::getActionExit() const         { return actions[1]; }
QAction* ToolbarManager::getActionHelp() const         { return actions[2]; }
QAction* ToolbarManager::getActionAbout() const        { return actions[3]; }
