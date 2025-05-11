// Main application window controller
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "../../config/configsettings/app_settings.h"
#include "../../config/ConfigManagers/NotificationConfigManager/NotificationConfigStruct.h"

// Qt includes
#include <QString>
#include <QPoint>
#include <QTimer>
#include <QMainWindow>
#include <QElapsedTimer>

// Forward Declaration (Qt class)
class QCloseEvent;
class QFileSystemModel;
class QLabel;
class QMouseEvent;
class QPushButton;
class QToolBar;
class QTreeView;
class QWidget;

// Forward Declaration (Custom class)
class BackupController;
class BackupService;
class CustomTitleBar;
class DestinationProxyModel;
class FileWatcher;
class NotificationConfigManager;
class StagingModel;
class ToolbarManager;

// Forward Declaration (UI class)
namespace Ui { class MainWindow; }

class MainWindow final : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    // Constructor and destructor
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    // Window event handling
    void closeEvent(QCloseEvent* event) override;

private:
    // UI configuration
    void configureWindow();
    void initializeUI();
    void setupLayout();
    void setupConnections();
    void applyButtonCursors();

    // Backup initialization
    void initializeBackupSystem();
    void connectBackupSignals();

    // Tree and list view configuration
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void setupDestinationView();
    void removeAllColumnsFromTreeView(QTreeView* treeView);

    // Backup status management
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

    // Backup button state and feedback
    void resetCreateBackupButtonState();
    void triggerButtonFeedback(QPushButton* button,
                               const QString& feedbackText,
                               const QString& originalText,
                               int durationMs = Timing::k_BUTTON_FEEDBACK_DURATION_MS);

    // Notification handling
    void setupNotificationButton();
    void updateNotificationButtonState();
    void showNextNotification();

private slots:
    // Backup actions
    void onAddToBackupClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void onChangeBackupDestinationClicked();

    // File monitoring events
    void onBackupDirectoryChanged();
    void onFileChanged(const QString& path);

    // Backup completion and errors
    void onBackupCompleted();
    void onBackupError(const QString& error);

    // Notification events
    void onNotificationButtonClicked();

    // Cooldown timer events
    void onCooldownFinished();

private:
    // UI elements
    Ui::MainWindow* ui = nullptr;
    CustomTitleBar* titleBar = nullptr;
    QToolBar* toolBar = nullptr;
    QPushButton* notificationButton = nullptr;
    QLabel* notificationBadge = nullptr;

    // Data models
    QFileSystemModel* sourceModel = nullptr;
    StagingModel* stagingModel = nullptr;
    QFileSystemModel* destinationModel = nullptr;
    DestinationProxyModel* destinationProxyModel = nullptr;

    // Backup system
    BackupService* backupService = nullptr;
    BackupController* backupController = nullptr;
    FileWatcher* fileWatcher = nullptr;

    // Toolbar and timer management
    ToolbarManager* toolbarManager = nullptr;
    QTimer* createBackupCooldownTimer = nullptr;
    QElapsedTimer backupStartTimer;

    // Notification system
    QList<NotificationConfigStruct> notificationQueue;
    bool isNotificationPopupVisible = false;
};

#endif // MAINWINDOW_H
