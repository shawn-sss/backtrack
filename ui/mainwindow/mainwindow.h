#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "../../../../constants/system_constants.h"
#include "../../core/backup/service/backupservice.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceStruct.h"

// Qt includes
#include <QAbstractItemView>
#include <QElapsedTimer>
#include <QItemSelectionModel>
#include <QMainWindow>
#include <QPair>
#include <QPoint>
#include <QString>
#include <QStringList>
#include <QTimer>

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
class QAbstractItemModel;
class QCloseEvent;
class QFileSystemModel;
class QLabel;
class QMouseEvent;
class QPushButton;
class QTabWidget;
class QToolBar;
class QTreeView;
class QWidget;

namespace Ui { class MainWindow; }

class MainWindow final : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    QTabWidget* getDetailsTabWidget();

    // Public utility methods
    void handleBackupDeletion(const QString& path, const QString& deleteType);
    void handleAppDataClear();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    // Initialization
    void configureWindow();
    void initializeUI();
    void setupLayout();
    void setupConnections();
    void initializeBackupSystem();
    void connectBackupSignals();
    void initializeFileWatcher();
    void setupNotificationButton();

    // Tree View setup
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void setupDestinationView();
    void setupDestinationView(const QString& backupDir);
    void configureTreeView(QTreeView* treeView,
                           QAbstractItemModel* model,
                           QAbstractItemView::SelectionMode selectionMode,
                           bool stretchLastColumn,
                           bool showHeader = true);
    void removeAllColumnsFromTreeView(QTreeView* treeView);

    // File watcher
    void refreshFileWatcherRoots();
    void startWatchingBackupDirectory(const QString& path);
    void resetFileWatcherAndDestinationView();
    QStringList getWatchedRoots() const;

    // Backup status and labels
    void updateApplicationStatusLabel();
    void updateBackupStatusLabel(const QString& statusColor);
    void updateBackupLocationLabel(const QString& location);
    void updateBackupLocationStatusLabel(const QString& location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void updateBackupLabels();
    void refreshBackupStatus();
    void updateLastBackupInfo();
    void handleSpecialBackupLabelStates(const BackupScanResult& scan);
    void notifyOrphanOrBrokenBackupIssues(const BackupScanResult& scan);
    QPair<QString, QString> statusVisualsForColor(const QString& color) const;
    QString checkInstallIntegrityStatus();
    void revalidateBackupAndAppStatus();

    // Notifications
    void updateNotificationButtonState();
    void showNotificationDialog();
    void feedbackNotificationButton();
    void displayNotificationPopup(const NotificationServiceStruct& notif);
    void finishNotificationQueue();
    void showNextNotification();

    // UI interactions and styling
    void applyButtonCursors();
    void resetCreateBackupButtonState();
    void triggerButtonFeedback(QPushButton* button,
                               const QString& feedbackText,
                               const QString& originalText,
                               int durationMs = System::Timing::k_BUTTON_FEEDBACK_DURATION_MS);
    void applyCustomTreePalette(QTreeView* treeView);

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
    void onCooldownFinished();
    void onNotificationButtonClicked();
    void refreshFileWatcher();
    void onThemeChanged();

private:
    // Core UI
    Ui::MainWindow* ui = nullptr;
    CustomTitleBar* titleBar = nullptr;
    QToolBar* toolBar = nullptr;
    QPushButton* notificationButton = nullptr;
    QLabel* notificationBadge = nullptr;

    // Models and services
    QFileSystemModel* sourceModel = nullptr;
    StagingModel* stagingModel = nullptr;
    QFileSystemModel* destinationModel = nullptr;
    DestinationProxyModel* destinationProxyModel = nullptr;
    BackupService* backupService = nullptr;
    BackupController* backupController = nullptr;
    FileWatcher* fileWatcher = nullptr;
    ToolbarServiceManager* toolbarManager = nullptr;

    // Timers
    QTimer* createBackupCooldownTimer = nullptr;
    QElapsedTimer backupStartTimer;

    // Notifications
    QList<NotificationServiceStruct> notificationQueue;
    bool isNotificationPopupVisible = false;
    bool orphanLogNotified = false;

    // Cache
    BackupScanResult latestBackupScan;
};

#endif // MAINWINDOW_H
