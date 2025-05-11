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
    // Constructor and destructor
    explicit ToolbarManager(QObject* parent = nullptr);

    // Initializes the toolbar
    void initialize(QToolBar* toolBar);

    // Returns specific actions
    QAction* getActionOpenSettings() const;
    QAction* getActionExit() const;
    QAction* getActionHelp() const;
    QAction* getActionAbout() const;

private:
    // Toolbar setup helpers
    void setupAppearance(QToolBar* toolBar);
    void createActions();
    void addActions(QToolBar* toolBar);
    void applyCursorStyle(QToolBar* toolBar);

    // Action array
    std::array<QAction*, 4> actions;

private slots:
    // Slot for settings dialog
    void showSettings();

    // Slot for help message
    void showHelp();

    // Slot for about dialog
    void showAbout();
};

#endif // TOOLBARMANAGER_H
