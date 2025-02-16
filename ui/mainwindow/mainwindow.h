#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QProgressBar>
#include <QStringList>
#include <QToolBar>
#include <QAction>
#include <QMouseEvent>  // Required for window dragging
#include <QLabel>
#include <QPushButton>
#include <QWidget>

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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    // Window events
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;   // Enables dragging the window
    void mouseMoveEvent(QMouseEvent *event) override;    // Enables dragging the window
    void mouseReleaseEvent(QMouseEvent *event) override; // Enables dragging the window

private:
    // **New Initialization Methods**
    void initializeUI();
    void initializeBackupSystem();
    void setupConnections();

    // UI setup
    void setupCustomTitleBar();  // Custom title bar setup
    void setupToolBar();         // Toolbar setup
    void setupToolbarActions();  // Helper function for toolbar
    void addToolbarSpacer();     // Adds spacing in toolbar
    void connectToolbarActions(QAction* helpAction, QAction* aboutAction); // Connects toolbar actions

    // Backup-related UI setup
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

    // Dragging support
    void handleMouseEvent(QMouseEvent *event, bool isPress);

    // Signal connections
    void connectBackupSignals();

private slots:
    // UI event handlers
    void onAddToBackupClicked();
    void onChangeBackupDestinationClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void openSettings();
    void exitApplication();
    void showHelpDialog();
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

    // Toolbar and Actions
    QToolBar *customToolBar;
    QAction *actionOpenSettings;
    QAction *actionExit;
    QAction *actionHelp;
    QAction *actionAbout;

    // Backup-related objects
    FileWatcher *fileWatcher;
    BackupService *backupService;
    StagingModel *stagingModel;
    BackupController *backupController;

    // Custom title bar components
    QWidget *titleBar;
    QLabel *titleLabel;
    QPushButton *minimizeButton;
    QPushButton *closeButton;

    // Dragging support
    QPoint lastMousePosition;
    bool dragging = false;
};

#endif // MAINWINDOW_H
