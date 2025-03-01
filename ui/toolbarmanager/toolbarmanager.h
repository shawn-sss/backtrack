#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QObject>

// ToolbarManager class definition
class ToolbarManager : public QObject {
    Q_OBJECT

public:
    explicit ToolbarManager(QObject *parent = nullptr);

    void initialize(QToolBar *toolBar);

    QAction* getActionOpenSettings() const;
    QAction* getActionExit() const;
    QAction* getActionHelp() const;
    QAction* getActionAbout() const;

    // Internal setup methods
private:
    void setupAppearance(QToolBar *toolBar);
    void addActions(QToolBar *toolBar);

    // Internal actions
private:
    QAction *actionOpenSettings;
    QAction *actionExit;
    QAction *actionHelp;
    QAction *actionAbout;
};

#endif // TOOLBARMANAGER_H
