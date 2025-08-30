#ifndef TOOLBARSERVICEMANAGER_H
#define TOOLBARSERVICEMANAGER_H

// Qt includes
#include <QObject>
#include <QAction>
#include <QToolBar>

// C++ includes
#include <array>

// Manages toolbar setup and actions
class ToolbarServiceManager : public QObject {
    Q_OBJECT

public:
    explicit ToolbarServiceManager(QObject* parent = nullptr);

    // Initialization
    void initialize(QToolBar* toolBar);

    // Query
    QAction* getActionOpenSettings() const;
    QAction* getActionExit() const;
    QAction* getActionHelp() const;
    QAction* getActionAbout() const;

private:
    // Setup
    void setupAppearance(QToolBar* toolBar);
    void createActions();
    void addActions(QToolBar* toolBar);
    void applyCursorStyle(QToolBar* toolBar);

    std::array<QAction*, 4> actions{};

private slots:
    // Slots
    void showSettings();
    void showHelp();
    void showAbout();
};

#endif
