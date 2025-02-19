#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QProgressBar>
#include <QStringList>
#include <QToolBar>
#include <QAction>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include "../customtitlebar/customtitlebar.h"
#include "../../core/utils/common_utils/utils.h"

// Forward Declarations
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
    // Constructor and Destructor
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    // Window Events
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // Initialization Methods
    void initializeUI();
    void initializeBackupSystem();
    void setupConnections();

    // UI Setup
    void setupCustomTitleBar();
    void setupToolBar();
    void setupToolbarActions();
    void addToolbarSpacer();
    void connectToolbarActions(QAction *helpAction, QAction *aboutAction);

    // Backup-Related UI Setup
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

    // Signal Connections
    void connectBackupSignals();

private slots:
    // UI Event Handlers
    void onAddToBackupClicked();
    void onChangeBackupDestinationClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void openSettings();
    void exitApplication();
    void showHelpDialog();
    void onAboutButtonClicked();

    // File Watcher Event Handlers
    void onBackupDirectoryChanged();
    void onFileChanged(const QString &path);

private:
    Ui::MainWindow *ui{nullptr};

    // Models and UI Components
    QFileSystemModel *destinationModel{nullptr};
    QFileSystemModel *sourceModel{nullptr};
    QProgressBar *progressBar{nullptr};

    // Toolbar and Actions
    QToolBar *customToolBar{nullptr};
    QAction *actionOpenSettings{nullptr};
    QAction *actionExit{nullptr};
    QAction *actionHelp{nullptr};
    QAction *actionAbout{nullptr};

    // Backup-Related Objects
    FileWatcher *fileWatcher{nullptr};
    BackupService *backupService{nullptr};
    StagingModel *stagingModel{nullptr};
    BackupController *backupController{nullptr};

    // Custom Title Bar
    CustomTitleBar *titleBar{nullptr};

    // Dragging Support
    QPoint lastMousePosition;
    bool dragging{false};
};

#endif // MAINWINDOW_H
