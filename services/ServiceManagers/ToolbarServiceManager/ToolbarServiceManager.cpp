// Project includes
#include "ToolbarServiceManager.h"
#include "ToolbarServiceConstants.h"
#include "ToolbarServiceStyling.h"

#include "../../../ui/mainwindow/mainwindow.h"
#include "../../../ui/settingsdialog/settingsdialog.h"
#include "../../../ui/helpdialog/helpdialog.h"
#include "../../../ui/aboutdialog/aboutdialog.h"

// Qt includes
#include <QApplication>
#include <QIcon>
#include <QSizePolicy>
#include <QToolButton>
#include <QWidget>

using namespace ToolbarServiceStyling;

// Lifecycle
ToolbarServiceManager::ToolbarServiceManager(QObject* parent)
    : QObject(parent),
    actions{ new QAction(this), new QAction(this), new QAction(this), new QAction(this) } {}

// Initialization
void ToolbarServiceManager::initialize(QToolBar* toolBar) {
    setupAppearance(toolBar);
    createActions();
    addActions(toolBar);
    applyCursorStyle(toolBar);
}

// Setup
void ToolbarServiceManager::setupAppearance(QToolBar* toolBar) {
    toolBar->setObjectName(QStringLiteral("MainToolBar"));
    toolBar->setOrientation(Qt::Vertical);
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    toolBar->setVisible(true);
    toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBar->setStyleSheet(BaseStyle);
    toolBar->setIconSize(DefaultIconSize);
}

// Actions
void ToolbarServiceManager::createActions() {
    actions[0]->setText(ToolbarLabels::Settings);
    actions[0]->setIcon(QIcon(ToolbarIcons::Settings));
    connect(actions[0], &QAction::triggered, this, &ToolbarServiceManager::showSettings);

    actions[1]->setText(ToolbarLabels::Exit);
    actions[1]->setIcon(QIcon(ToolbarIcons::Exit));
    connect(actions[1], &QAction::triggered, qApp, &QApplication::quit);

    actions[2]->setText(ToolbarLabels::Help);
    actions[2]->setIcon(QIcon(ToolbarIcons::Help));
    connect(actions[2], &QAction::triggered, this, &ToolbarServiceManager::showHelp);

    actions[3]->setText(ToolbarLabels::About);
    actions[3]->setIcon(QIcon(ToolbarIcons::About));
    connect(actions[3], &QAction::triggered, this, &ToolbarServiceManager::showAbout);
}

void ToolbarServiceManager::addActions(QToolBar* toolBar) {
    toolBar->clear();
    toolBar->addAction(actions[0]); // Settings
    toolBar->addAction(actions[2]); // Help
    toolBar->addAction(actions[3]); // About

    auto* spacer = new QWidget(toolBar);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    toolBar->addAction(actions[1]); // Exit at bottom
}

void ToolbarServiceManager::applyCursorStyle(QToolBar* toolBar) {
    static constexpr const char* ObjectNames[4] = {
        "SettingsButton", "ExitButton", "HelpButton", "AboutButton"
    };

    for (int i = 0; i < static_cast<int>(actions.size()); ++i) {
        if (QWidget* w = toolBar->widgetForAction(actions[i])) {
            w->setCursor(Qt::PointingHandCursor);
            if (QToolButton* btn = qobject_cast<QToolButton*>(w)) {
                btn->setObjectName(QString::fromLatin1(ObjectNames[i]));
            }
        }
    }
}

// Slots
void ToolbarServiceManager::showSettings() {
    auto* parentWidget = qobject_cast<QWidget*>(parent());
    SettingsDialog dialog(parentWidget);

    if (MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget)) {
        connect(&dialog, &SettingsDialog::requestBackupReset, mainWindow,
                [mainWindow](const QString& path, const QString& type) {
                    mainWindow->handleBackupDeletion(path, type);
                });

        connect(&dialog, &SettingsDialog::requestAppDataClear, mainWindow, [mainWindow]() {
            mainWindow->handleAppDataClear();
        });
    }

    dialog.exec();
}

void ToolbarServiceManager::showHelp() {
    HelpDialog dialog(qobject_cast<QWidget*>(parent()));
    dialog.exec();
}

void ToolbarServiceManager::showAbout() {
    AboutDialog dialog(qobject_cast<QWidget*>(parent()));
    dialog.exec();
}

// Query
QAction* ToolbarServiceManager::getActionOpenSettings() const { return actions[0]; }
QAction* ToolbarServiceManager::getActionExit() const          { return actions[1]; }
QAction* ToolbarServiceManager::getActionHelp() const          { return actions[2]; }
QAction* ToolbarServiceManager::getActionAbout() const         { return actions[3]; }
