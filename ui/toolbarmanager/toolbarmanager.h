#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

// Qt includes
#include <QObject>
#include <QAction>
#include <QToolBar>

// C++ includes
#include <array>

// Manages the application's toolbar appearance and actions
class ToolbarManager : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit ToolbarManager(QObject *parent = nullptr);

    // Initializes the toolbar by applying styles and adding actions
    void initialize(QToolBar *toolBar);

    // Retrieves toolbar actions
    QAction* getActionOpenSettings() const;
    QAction* getActionExit() const;
    QAction* getActionHelp() const;
    QAction* getActionAbout() const;

private:
    // Creates toolbar actions
    void createActions();

    // Configures toolbar appearance
    void setupAppearance(QToolBar *toolBar);

    // Adds all actions to the toolbar
    void addActions(QToolBar *toolBar);

    // Adds cursor styling
    void applyCursorStyle(QToolBar *toolBar);

    // Toolbar action storage
    std::array<QAction*, 4> actions;

private slots:
    // Opens the settings dialog
    void showSettings();

    // Shows help content
    void showHelp();

    // Shows the about dialog
    void showAbout();
};

#endif // TOOLBARMANAGER_H
