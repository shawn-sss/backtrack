#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QProgressBar>
#include <QStringList>

// Forward declarations for custom classes
class QTreeView;
class BackupService;
class StagingModel;
class FileWatcher;
class BackupController;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Main application window class
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    // Constructor and Destructor
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Handles the window close event
    void closeEvent(QCloseEvent *event) override;

private:
    // UI Setup Methods
    void setupDestinationView();
    void setupSourceTreeView();
    void setupBackupStagingTreeView();

    // Backup Management
    void updateLastBackupInfo();
    void refreshBackupStatus();

    // File and Directory Monitoring
    void startWatchingBackupDirectory(const QString &path);
    void updateFileWatcher();

    // UI Updates
    void updateBackupStatusLabel(bool backupFound);
    void updateBackupLocationLabel(const QString &location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void updateBackupLocationStatusLabel(const QString &location);
    void removeAllColumnsFromTreeView(QTreeView *treeView);

private slots:
    // UI Interaction Handlers
    void onAddToBackupClicked();
    void onChangeBackupDestinationClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void onAboutButtonClicked();

    // File and Directory Watcher Handlers
    void onBackupDirectoryChanged();
    void onFileChanged(const QString &path);

private:
    // UI Components and Models
    Ui::MainWindow *ui;
    QFileSystemModel *destinationModel;
    QFileSystemModel *sourceModel;
    QProgressBar *progressBar;

    // Backup-Related Objects
    FileWatcher *fileWatcher;
    BackupService *backupService;
    StagingModel *stagingModel;
    BackupController *backupController;
};

#endif // MAINWINDOW_H
