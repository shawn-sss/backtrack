#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

// Qt includes
#include <QObject>
#include <QAction>
#include <QToolBar>

// C++ includes
#include <array>

// Manages the application's toolbar appearance and behavior
class ToolbarManager : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit ToolbarManager(QObject *parent = nullptr);

    // Initializes the toolbar with appearance and actions
    void initialize(QToolBar *toolBar);

    // Accessor methods for individual toolbar actions
    QAction* getActionOpenSettings() const;
    QAction* getActionExit() const;
    QAction* getActionHelp() const;
    QAction* getActionAbout() const;

private:
    // Internal setup helpers
    void createActions();
    void setupAppearance(QToolBar *toolBar);
    void addActions(QToolBar *toolBar);
    void applyCursorStyle(QToolBar *toolBar);

    // Storage for toolbar actions
    std::array<QAction*, 4> actions;

private slots:
    // Slots for handling toolbar actions
    void showSettings();
    void showHelp();
    void showAbout();
};

#endif // TOOLBARMANAGER_H
