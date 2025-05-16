#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "../../../../constants/system_constants.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceStruct.h"

// Qt includes
#include <QMainWindow>
#include <QElapsedTimer>
#include <QPoint>
#include <QTimer>
#include <QString>

// Forward declaration (Custom class)
class BackupController;
class BackupService;
class CustomTitleBar;
class DestinationProxyModel;
class FileWatcher;
class NotificationServiceManager;
class StagingModel;
class ToolbarServiceManager;

// Forward declaration (Qt class)
class QCloseEvent;
class QFileSystemModel;
class QLabel;
class QMouseEvent;
class QPushButton;
class QToolBar;
class QTreeView;
class QWidget;

// Forward declaration (UI class)
namespace Ui { class MainWindow; }

// Main application window controller
class MainWindow final : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    // Accessors
    QTabWidget* getDetailsTabWidget();

protected:
    // Qt event overrides
    void closeEvent(QCloseEvent* event) override;

private:
    const QStringList requiredConfigFiles = {
        "app_init.json",
        "app_notifications.json",
        "user_settings.json"
    };
    QString checkInstallIntegrityStatus();
    void updateApplicationStatusLabel();
    // UI setup
    void configureWindow();
    void initializeUI();
    void setupLayout();
    void setupConnections();
    void applyButtonCursors();

    // Backup system
    void initializeBackupSystem();
    void connectBackupSignals();

    // Tree views
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void setupDestinationView();
    void removeAllColumnsFromTreeView(QTreeView* treeView);

    // Backup status display
    void refreshBackupStatus();
    void updateLastBackupInfo();
    void updateBackupStatusLabel(const QString& statusColor);
    void updateBackupLocationLabel(const QString& location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void updateBackupLocationStatusLabel(const QString& location);

    // File system watching
    void updateFileWatcher();
    void startWatchingBackupDirectory(const QString& path);

    // Backup button feedback
    void resetCreateBackupButtonState();
    void triggerButtonFeedback(QPushButton* button,
                               const QString& feedbackText,
                               const QString& originalText,
                               int durationMs = System::Timing::k_BUTTON_FEEDBACK_DURATION_MS);

    // Notifications
    void setupNotificationButton();
    void updateNotificationButtonState();
    void showNextNotification();

private slots:
    // Backup actions
    void onAddToBackupClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void onDeleteAllBackupsClicked();
    void onChangeBackupDestinationClicked();

    // File system watchers
    void onBackupDirectoryChanged();
    void onFileChanged(const QString& path);

    // Backup lifecycle
    void onBackupCompleted();
    void onBackupError(const QString& error);

    // Notification interactions
    void onNotificationButtonClicked();
    void onCooldownFinished();

    // Uninstall interaction
    void onUninstallClicked();

private:
    Ui::MainWindow* ui = nullptr;
    CustomTitleBar* titleBar = nullptr;
    QToolBar* toolBar = nullptr;
    QPushButton* notificationButton = nullptr;
    QLabel* notificationBadge = nullptr;

    QFileSystemModel* sourceModel = nullptr;
    StagingModel* stagingModel = nullptr;
    QFileSystemModel* destinationModel = nullptr;
    DestinationProxyModel* destinationProxyModel = nullptr;

    BackupService* backupService = nullptr;
    BackupController* backupController = nullptr;
    FileWatcher* fileWatcher = nullptr;

    ToolbarServiceManager* toolbarManager = nullptr;
    QTimer* createBackupCooldownTimer = nullptr;
    QElapsedTimer backupStartTimer;

    QList<NotificationServiceStruct> notificationQueue;
    bool isNotificationPopupVisible = false;
    bool orphanLogNotified = false;
};

#endif // MAINWINDOW_H
