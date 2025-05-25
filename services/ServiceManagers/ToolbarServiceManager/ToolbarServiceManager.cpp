// Project includes
#include "ToolbarServiceManager.h"
#include "ToolbarServiceConstants.h"
#include "ToolbarServiceStyling.h"

#include "../../../ui/mainwindow/mainwindow.h"
#include "../../../ui/mainwindow/mainwindowlabels.h"
#include "../../../ui/settingsdialog/settingsdialog.h"
#include "../../../ui/helpdialog/helpdialog.h"
#include "../../../ui/aboutdialog/aboutdialog.h"

// Qt includes
#include <QApplication>
#include <QCursor>
#include <QIcon>
#include <QMessageBox>
#include <QSizePolicy>
#include <QToolButton>
#include <QWidget>

// Constructs the toolbar service manager
ToolbarServiceManager::ToolbarServiceManager(QObject* parent)
    : QObject(parent),
    actions{new QAction(this), new QAction(this), new QAction(this), new QAction(this)} {}

// Initializes the toolbar and its actions
void ToolbarServiceManager::initialize(QToolBar* toolBar) {
    setupAppearance(toolBar);
    createActions();
    addActions(toolBar);
    applyCursorStyle(toolBar);
}

// Sets up the appearance and behavior of the toolbar
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

// Creates the toolbar actions and sets their properties
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

// Adds toolbar actions and a spacer widget to organize layout
void ToolbarServiceManager::addActions(QToolBar* toolBar) {
    toolBar->clear();
    toolBar->addAction(actions[0]);
    toolBar->addAction(actions[2]);
    toolBar->addAction(actions[3]);

    auto* spacer = new QWidget(toolBar);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    toolBar->addAction(actions[1]);
}

// Applies cursor style and object names to toolbar buttons
void ToolbarServiceManager::applyCursorStyle(QToolBar* toolBar) {
    const QStringList objectNames = {
        "SettingsButton", "ExitButton", "HelpButton", "AboutButton"
    };

    for (int i = 0; i < actions.size(); ++i) {
        if (QWidget* w = toolBar->widgetForAction(actions[i])) {
            w->setCursor(Qt::PointingHandCursor);
            if (QToolButton* btn = qobject_cast<QToolButton*>(w)) {
                btn->setObjectName(objectNames.value(i));
            }
        }
    }
}

// Shows the Settings dialog with connections to MainWindow slots
void ToolbarServiceManager::showSettings() {
    auto* parentWidget = qobject_cast<QWidget*>(parent());
    SettingsDialog dialog(parentWidget);

    if (MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget)) {
        connect(&dialog, &SettingsDialog::requestBackupReset, mainWindow, [mainWindow](const QString& path, const QString& type) {
            mainWindow->handleBackupDeletion(path, type);
        });

        connect(&dialog, &SettingsDialog::requestAppDataClear, mainWindow, [mainWindow]() {
            mainWindow->handleAppDataClear();
        });
    }

    dialog.exec();
}

// Shows the Help dialog
void ToolbarServiceManager::showHelp() {
    HelpDialog dialog(qobject_cast<QWidget*>(parent()));
    dialog.exec();
}

// Shows the About dialog
void ToolbarServiceManager::showAbout() {
    AboutDialog dialog(qobject_cast<QWidget*>(parent()));
    dialog.exec();
}

// Provides access to the toolbar actions
QAction* ToolbarServiceManager::getActionOpenSettings() const { return actions[0]; }
QAction* ToolbarServiceManager::getActionExit() const          { return actions[1]; }
QAction* ToolbarServiceManager::getActionHelp() const          { return actions[2]; }
QAction* ToolbarServiceManager::getActionAbout() const         { return actions[3]; }
