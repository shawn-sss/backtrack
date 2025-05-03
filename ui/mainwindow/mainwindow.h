#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "../../config/configsettings/_settings.h"

// Qt includes
#include <QString>
#include <QPoint>
#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>

// Forward declarations (Qt classes)
class QTreeView;
class QFileSystemModel;
class QLabel;
class QPushButton;
class QWidget;
class QMouseEvent;
class QCloseEvent;
class QTimer;
class QToolBar;

// Forward declarations (Custom classes)
class BackupService;
class StagingModel;
class DestinationProxyModel;
class FileWatcher;
class BackupController;
class CustomTitleBar;
class ToolbarManager;

// Forward declarations (UI class)
namespace Ui { class MainWindow; }

// Main application window
class MainWindow final : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    // Constructor
    explicit MainWindow(QWidget* parent = nullptr);
    // Destructor
    ~MainWindow() override;

protected:
    // Handle close event
    void closeEvent(QCloseEvent* event) override;

private:
    // UI initialization and setup
    void configureWindow();
    void setupLayout();
    void applyButtonCursors();
    void initializeUI();
    void initializeBackupSystem();
    void setupConnections();

    // Backup tree views
    void setupDestinationView();
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void removeAllColumnsFromTreeView(QTreeView* treeView);

    // Backup status and monitoring
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
    void resetCreateBackupButtonState();

    // Unified button feedback handler
    void triggerButtonFeedback(QPushButton* button,
                               const QString& feedbackText,
                               const QString& originalText,
                               int durationMs = Timing::k_BUTTON_FEEDBACK_DURATION_MS);

private slots:
    // Backup operations
    void onAddToBackupClicked();
    void onChangeBackupDestinationClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void onCooldownFinished();

    // File watcher events
    void onBackupDirectoryChanged();
    void onFileChanged(const QString& path);

    // Backup completion and error handlers
    void onBackupCompleted();
    void onBackupError(const QString& error);

private:
    // UI components
    Ui::MainWindow* ui = nullptr;
    CustomTitleBar* titleBar = nullptr;

    // File system models
    QFileSystemModel* sourceModel = nullptr;
    StagingModel* stagingModel = nullptr;
    QFileSystemModel* destinationModel = nullptr;
    DestinationProxyModel* destinationProxyModel = nullptr;

    // Backup components
    FileWatcher* fileWatcher = nullptr;
    BackupService* backupService = nullptr;
    BackupController* backupController = nullptr;

    // Timer for tracking feedback duration
    QTimer* createBackupCooldownTimer = nullptr;
    QElapsedTimer backupStartTimer;

    // Toolbar
    QToolBar* toolBar = nullptr;
    ToolbarManager* toolbarManager = nullptr;
};

#endif // MAINWINDOW_H
