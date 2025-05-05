#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

// Qt includes
#include <QAction>
#include <QObject>
#include <QToolBar>

// C++ includes
#include <array>

// Manages the application's toolbar appearance and behavior
class ToolbarManager : public QObject {
    Q_OBJECT

public:
    // Lifecycle
    explicit ToolbarManager(QObject* parent = nullptr);

    // Initializes the toolbar layout, actions, and style
    void initialize(QToolBar* toolBar);

    // Accessors for specific toolbar actions
    QAction* getActionOpenSettings() const;
    QAction* getActionExit() const;
    QAction* getActionHelp() const;
    QAction* getActionAbout() const;

private:
    // Helpers for configuring toolbar and actions
    void createActions();
    void setupAppearance(QToolBar* toolBar);
    void addActions(QToolBar* toolBar);
    void applyCursorStyle(QToolBar* toolBar);

    // Action storage
    std::array<QAction*, 4> actions;

private slots:
    // Action slots
    void showSettings();
    void showHelp();
    void showAbout();
};

#endif // TOOLBARMANAGER_H
