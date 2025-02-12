#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QProgressBar>
#include <QStringList>

// Forward declarations
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
    // Window close event
    void closeEvent(QCloseEvent *event) override;

private:
    // UI setup
    void setupDestinationView();
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void removeAllColumnsFromTreeView(QTreeView *treeView);

    // Backup management
    void refreshBackupStatus();
    void updateLastBackupInfo();

    // File monitoring
    void startWatchingBackupDirectory(const QString &path);
    void updateFileWatcher();

    // UI updates
    void updateBackupStatusLabel(const QString &statusColor);
    void updateBackupLocationLabel(const QString &location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void updateBackupLocationStatusLabel(const QString &location);

    // Signal connections
    void connectBackupSignals();

private slots:
    // UI event handlers
    void onAddToBackupClicked();
    void onChangeBackupDestinationClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void onAboutButtonClicked();

    // File watcher event handlers
    void onBackupDirectoryChanged();
    void onFileChanged(const QString &path);

private:
    Ui::MainWindow *ui;

    // Models and UI components
    QFileSystemModel *destinationModel;
    QFileSystemModel *sourceModel;
    QProgressBar *progressBar;

    // Backup-related objects
    FileWatcher *fileWatcher;
    BackupService *backupService;
    StagingModel *stagingModel;
    BackupController *backupController;
};

#endif // MAINWINDOW_H
