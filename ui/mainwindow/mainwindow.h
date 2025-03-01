#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QString>

// Forward declarations (Qt classes)
class QTreeView;
class QProgressBar;
class QAction;
class QFileSystemModel;
class QLabel;
class QPushButton;
class QWidget;
class QMouseEvent;
class QCloseEvent;

// Forward declarations (custom classes)
class ToolbarManager;
class BackupService;
class StagingModel;
class FileWatcher;
class BackupController;
class CustomTitleBar;

// Forward declaration for auto-generated UI class
namespace Ui {
class MainWindow;
}

// MainWindow class definition
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    // Event handling
protected:
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    // Initialization and setup
private:
    void configureWindow();
    void configureToolBar();
    void setupLayout();
    void setupToolBar();
    void applyButtonCursors();
    void initializeUI();
    void initializeBackupSystem();
    void setupConnections();
    void setupToolbarActions();
    void setupDestinationView();
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void removeAllColumnsFromTreeView(QTreeView *treeView);

    // Backup status and file watching
private:
    void refreshBackupStatus();
    void updateLastBackupInfo();
    void startWatchingBackupDirectory(const QString &path);
    void updateFileWatcher();
    void updateBackupStatusLabel(const QString &statusColor);
    void updateBackupLocationLabel(const QString &location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void updateBackupLocationStatusLabel(const QString &location);
    void connectBackupSignals();

    // Backup operation slots
private slots:
    void onAddToBackupClicked();
    void onChangeBackupDestinationClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();

    // Menu and toolbar actions
private slots:
    void openSettings();
    void exitApplication();
    void showHelpDialog();
    void onAboutButtonClicked();

    // File watcher callbacks
private slots:
    void onBackupDirectoryChanged();
    void onFileChanged(const QString &path);

    // Internal variables
private:
    Ui::MainWindow *ui = nullptr;

    QFileSystemModel *destinationModel = nullptr;
    QFileSystemModel *sourceModel = nullptr;

    FileWatcher *fileWatcher = nullptr;
    BackupService *backupService = nullptr;
    StagingModel *stagingModel = nullptr;
    BackupController *backupController = nullptr;

    CustomTitleBar *titleBar = nullptr;

    // Mouse dragging state
private:
    QPoint lastMousePosition;
    bool dragging = false;
};

#endif // MAINWINDOW_H
