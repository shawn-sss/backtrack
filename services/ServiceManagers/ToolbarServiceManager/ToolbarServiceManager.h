#ifndef TOOLBARSERVICEMANAGER_H
#define TOOLBARSERVICEMANAGER_H

// Qt includes
#include <QObject>
#include <QAction>
#include <QToolBar>

// C++ includes
#include <array>

// Manages toolbar setup and user interaction
class ToolbarServiceManager : public QObject {
    Q_OBJECT

public:
    explicit ToolbarServiceManager(QObject* parent = nullptr);

    // Initializes the toolbar with style and actions
    void initialize(QToolBar* toolBar);

    // Access to individual toolbar actions
    QAction* getActionOpenSettings() const;
    QAction* getActionExit() const;
    QAction* getActionHelp() const;
    QAction* getActionAbout() const;

private:
    // Toolbar setup and customization
    void setupAppearance(QToolBar* toolBar);
    void createActions();
    void addActions(QToolBar* toolBar);
    void applyCursorStyle(QToolBar* toolBar);

    std::array<QAction*, 4> actions;

private slots:
    // Action handlers for toolbar buttons
    void showSettings();
    void showHelp();
    void showAbout();
};

#endif // TOOLBARSERVICEMANAGER_H
