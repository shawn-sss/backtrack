#ifndef TOOLBARSERVICEMANAGER_H
#define TOOLBARSERVICEMANAGER_H

// Qt includes
#include <QObject>
#include <QAction>
#include <QToolBar>

// C++ includes
#include <array>

// Manages the application's toolbar service appearance and behavior
class ToolbarServiceManager : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit ToolbarServiceManager(QObject* parent = nullptr);

    // Initializes the toolbar service
    void initialize(QToolBar* toolBar);

    // Action accessors
    QAction* getActionOpenSettings() const;
    QAction* getActionExit() const;
    QAction* getActionHelp() const;
    QAction* getActionAbout() const;

private:
    // Setup helpers
    void setupAppearance(QToolBar* toolBar);
    void createActions();
    void addActions(QToolBar* toolBar);
    void applyCursorStyle(QToolBar* toolBar);

    // Toolbar actions
    std::array<QAction*, 4> actions;

private slots:
    void showSettings();
    void showHelp();
    void showAbout();
};

#endif // TOOLBARSERVICEMANAGER_H
