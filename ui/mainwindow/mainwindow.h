#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "../../../../constants/system_constants.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceStruct.h"

// Qt includes
#include <QMainWindow>
#include <QString>
#include <QPoint>
#include <QTimer>
#include <QElapsedTimer>

// Forward declarations (Qt class)
class QCloseEvent;
class QFileSystemModel;
class QLabel;
class QMouseEvent;
class QPushButton;
class QToolBar;
class QTreeView;
class QWidget;

// Forward declarations (Custom class)
class BackupController;
class BackupService;
class CustomTitleBar;
class DestinationProxyModel;
class FileWatcher;
class NotificationServiceManager;
class StagingModel;
class ToolbarServiceManager;

// Forward declarations (UI class)
namespace Ui { class MainWindow; }

// Main application window controller
class MainWindow final : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    QTabWidget* getDetailsTabWidget();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    // UI setup
    void configureWindow();
    void initializeUI();
    void setupLayout();
    void setupConnections();
    void applyButtonCursors();

    // Backup setup
    void initializeBackupSystem();
    void connectBackupSignals();

    // Views configuration
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

    // File watching
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
    void onAddToBackupClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void onChangeBackupDestinationClicked();

    void onBackupDirectoryChanged();
    void onFileChanged(const QString& path);

    void onBackupCompleted();
    void onBackupError(const QString& error);

    void onNotificationButtonClicked();
    void onCooldownFinished();

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
};

#endif // MAINWINDOW_H
