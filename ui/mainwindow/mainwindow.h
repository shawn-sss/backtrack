#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "../../config/configsettings/_settings.h"

// Qt includes
#include <QElapsedTimer>
#include <QMainWindow>
#include <QPoint>
#include <QString>
#include <QTimer>

// Forward Declaration (Qt classes)
class QCloseEvent;
class QLabel;
class QPushButton;
class QFileSystemModel;
class QMouseEvent;
class QToolBar;
class QTreeView;
class QWidget;

// Forward Declaration (Custom classes)
class AppNotification;
class BackupController;
class BackupService;
class CustomTitleBar;
class DestinationProxyModel;
class FileWatcher;
class NotificationsManager;
class StagingModel;
class ToolbarManager;

// Forward Declaration (UI class)
namespace Ui { class MainWindow; }

// Main application window controller
class MainWindow final : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    // Handles window close event
    void closeEvent(QCloseEvent* event) override;

private:
    // UI configuration
    void configureWindow();
    void setupLayout();
    void applyButtonCursors();
    void initializeUI();
    void setupConnections();

    // Backup system initialization
    void initializeBackupSystem();
    void connectBackupSignals();

    // View configuration
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void setupDestinationView();
    void removeAllColumnsFromTreeView(QTreeView* treeView);

    // Backup status and info
    void refreshBackupStatus();
    void updateLastBackupInfo();
    void updateBackupStatusLabel(const QString& statusColor);
    void updateBackupLocationLabel(const QString& location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void updateBackupLocationStatusLabel(const QString& location);

    // File monitoring
    void updateFileWatcher();
    void startWatchingBackupDirectory(const QString& path);

    // Backup controls and feedback
    void resetCreateBackupButtonState();
    void triggerButtonFeedback(QPushButton* button,
                               const QString& feedbackText,
                               const QString& originalText,
                               int durationMs = Timing::k_BUTTON_FEEDBACK_DURATION_MS);

    // Notifications
    void setupNotificationButton();
    void updateNotificationButtonState();
    void onNotificationButtonClicked();
    void showNextNotification();

private slots:
    // Backup control handlers
    void onAddToBackupClicked();
    void onChangeBackupDestinationClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();

    // Backup monitoring
    void onBackupDirectoryChanged();
    void onFileChanged(const QString& path);

    // Backup lifecycle feedback
    void onBackupCompleted();
    void onBackupError(const QString& error);

    // Button cooldown
    void onCooldownFinished();

private:
    // UI components
    Ui::MainWindow* ui = nullptr;
    CustomTitleBar* titleBar = nullptr;
    QToolBar* toolBar = nullptr;
    QPushButton* notificationButton = nullptr;
    QLabel* notificationBadge = nullptr;

    // Models
    QFileSystemModel* sourceModel = nullptr;
    StagingModel* stagingModel = nullptr;
    QFileSystemModel* destinationModel = nullptr;
    DestinationProxyModel* destinationProxyModel = nullptr;

    // Backup services
    FileWatcher* fileWatcher = nullptr;
    BackupService* backupService = nullptr;
    BackupController* backupController = nullptr;

    // Toolbar and timers
    ToolbarManager* toolbarManager = nullptr;
    QTimer* createBackupCooldownTimer = nullptr;
    QElapsedTimer backupStartTimer;

    // Notification queue
    QList<AppNotification> notificationQueue;
    bool isNotificationPopupVisible = false;
};

#endif // MAINWINDOW_H
