#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../customtitlebar/customtitlebar.h"

#include <QLabel>
#include <QStringList>
#include <QMainWindow>
#include <QToolBar>
#include <QPushButton>
#include <QProgressBar>
#include <QWidget>
#include <QAction>
#include <QMouseEvent>
#include <QFileSystemModel>

//Forward declarations
class QTreeView;
class BackupService;
class StagingModel;
class FileWatcher;
class BackupController;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Main application window
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    // Window event handling
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // Initialization
    void initializeUI();
    void initializeBackupSystem();
    void setupConnections();

    // UI Components
    void setupToolBar();
    void setupToolbarActions();
    void addToolbarSpacer();
    void connectToolbarActions(QAction *helpAction, QAction *aboutAction);

    // Backup UI
    void setupDestinationView();
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void removeAllColumnsFromTreeView(QTreeView *treeView);

    // Backup Management
    void refreshBackupStatus();
    void updateLastBackupInfo();

    // File Monitoring
    void startWatchingBackupDirectory(const QString &path);
    void updateFileWatcher();

    // UI Updates
    void updateBackupStatusLabel(const QString &statusColor);
    void updateBackupLocationLabel(const QString &location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void updateBackupLocationStatusLabel(const QString &location);

    // Dragging Support
    void handleMouseEvent(QMouseEvent *event, bool isPress);

    // Signal Handling
    void connectBackupSignals();

private slots:
    // UI Actions
    void onAddToBackupClicked();
    void onChangeBackupDestinationClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void openSettings();
    void exitApplication();
    void showHelpDialog();
    void onAboutButtonClicked();

    // File Watcher Handling
    void onBackupDirectoryChanged();
    void onFileChanged(const QString &path);

private:
    Ui::MainWindow *ui{nullptr};
    QFileSystemModel *destinationModel{nullptr};
    QFileSystemModel *sourceModel{nullptr};
    QProgressBar *progressBar{nullptr};

    // Toolbar
    QToolBar *customToolBar{nullptr};
    QAction *actionOpenSettings{nullptr};
    QAction *actionExit{nullptr};
    QAction *actionHelp{nullptr};
    QAction *actionAbout{nullptr};

    // Backup System
    FileWatcher *fileWatcher{nullptr};
    BackupService *backupService{nullptr};
    StagingModel *stagingModel{nullptr};
    BackupController *backupController{nullptr};

    // Custom Title Bar
    CustomTitleBar *titleBar{nullptr};

    // Dragging
    QPoint lastMousePosition;
    bool dragging{false};
};

#endif // MAINWINDOW_H
