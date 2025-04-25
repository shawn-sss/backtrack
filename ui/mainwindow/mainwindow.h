#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt includes
#include <QMainWindow>
#include <QPoint>
#include <QString>

// Forward declarations Qt classes
class QTreeView;
class QFileSystemModel;
class QLabel;
class QPushButton;
class QWidget;
class QMouseEvent;
class QCloseEvent;
class QTimer;

// Forward declarations custom classes
class BackupService;
class StagingModel;
class DestinationProxyModel;
class FileWatcher;
class BackupController;
class CustomTitleBar;
class ToolbarManager;

// Forward declaration UI class
namespace Ui {class MainWindow;}

// Main application window
class MainWindow final : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    // Constructor and Destructor
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    // Handles window close event
    void closeEvent(QCloseEvent *event) override;

private:
    // UI Initialization and Setup
    void configureWindow();
    void setupLayout();
    void applyButtonCursors();
    void initializeUI();
    void initializeBackupSystem();
    void setupConnections();

    // Backup view setup and management
    void setupDestinationView();
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void removeAllColumnsFromTreeView(QTreeView *treeView);

    // Backup status and monitoring
    void refreshBackupStatus();
    void updateLastBackupInfo();
    void updateBackupStatusLabel(const QString &statusColor);
    void updateBackupLocationLabel(const QString &location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void updateBackupLocationStatusLabel(const QString &location);
    void updateFileWatcher();
    void startWatchingBackupDirectory(const QString &path);
    void connectBackupSignals();

private slots:
    // Backup operations
    void onAddToBackupClicked();
    void onChangeBackupDestinationClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void onCooldownFinished();

    // File watcher callbacks
    void onBackupDirectoryChanged();
    void onFileChanged(const QString &path);

    // Backup feedback
    void onBackupCompleted();
    void onBackupError(const QString &error);

private:
    // UI Elements
    Ui::MainWindow *ui = nullptr;
    CustomTitleBar *titleBar = nullptr;

    // File models
    QFileSystemModel *sourceModel = nullptr;
    StagingModel *stagingModel = nullptr;
    QFileSystemModel *destinationModel = nullptr;
    DestinationProxyModel *destinationProxyModel = nullptr;

    // Backup & File watcher
    FileWatcher *fileWatcher = nullptr;
    BackupService *backupService = nullptr;
    BackupController *backupController = nullptr;
    QTimer* createBackupCooldownTimer;
    QTimer *backupCooldownTimer;

    // Toolbar utilization
    QToolBar *toolBar;
    ToolbarManager *toolbarManager;
};

#endif // MAINWINDOW_H
