#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt includes
#include <QString>
#include <QPoint>
#include <QMainWindow>

// Forward declarations (Qt classes)
class QTreeView;
class QFileSystemModel;
class QLabel;
class QPushButton;
class QWidget;
class QMouseEvent;
class QCloseEvent;
class QTimer;

// Forward declarations (custom classes)
class BackupService;
class StagingModel;
class DestinationProxyModel;
class FileWatcher;
class BackupController;
class CustomTitleBar;
class ToolbarManager;

// Forward declaration (UI)
namespace Ui { class MainWindow; }

// Main application window
class MainWindow final : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    explicit MainWindow(QWidget* parent = nullptr); // Constructor
    ~MainWindow() override; // Destructor

protected:
    void closeEvent(QCloseEvent* event) override; // Handle window close event

private:
    // UI Initialization and setup
    void configureWindow();
    void setupLayout();
    void applyButtonCursors();
    void initializeUI();
    void initializeBackupSystem();
    void setupConnections();

    // Backup view setup
    void setupDestinationView();
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void removeAllColumnsFromTreeView(QTreeView* treeView);

    // Backup system monitoring
    void refreshBackupStatus();
    void updateLastBackupInfo();
    void updateBackupStatusLabel(const QString& statusColor);
    void updateBackupLocationLabel(const QString& location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void updateBackupLocationStatusLabel(const QString& location);
    void updateFileWatcher();
    void startWatchingBackupDirectory(const QString& path);
    void connectBackupSignals();

private slots:
    // Backup operation handlers
    void onAddToBackupClicked();
    void onChangeBackupDestinationClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void onCooldownFinished();

    // File watcher event handlers
    void onBackupDirectoryChanged();
    void onFileChanged(const QString& path);

    // Backup feedback handlers
    void onBackupCompleted();
    void onBackupError(const QString& error);

private:
    // UI elements
    Ui::MainWindow* ui = nullptr;
    CustomTitleBar* titleBar = nullptr;

    // File models
    QFileSystemModel* sourceModel = nullptr;
    StagingModel* stagingModel = nullptr;
    QFileSystemModel* destinationModel = nullptr;
    DestinationProxyModel* destinationProxyModel = nullptr;

    // Backup system components
    FileWatcher* fileWatcher = nullptr;
    BackupService* backupService = nullptr;
    BackupController* backupController = nullptr;

    // Timing components
    QTimer* createBackupCooldownTimer = nullptr;
    QTimer* backupCooldownTimer = nullptr;

    // Toolbar components
    QToolBar* toolBar = nullptr;
    ToolbarManager* toolbarManager = nullptr;
};

#endif // MAINWINDOW_H
