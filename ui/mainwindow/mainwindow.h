#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "../../../../constants/system_constants.h"
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

// Forward declaration (UI class)
class BackupScanResult;
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
    // UI and layout setup
    void configureWindow();
    void initializeUI();
    void setupLayout();
    void setupConnections();
    void applyButtonCursors();

    // Backup system setup
    void initializeBackupSystem();
    void connectBackupSignals();

    // File system views
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void setupDestinationView();
    void removeAllColumnsFromTreeView(QTreeView* treeView);
    void configureTreeView(QTreeView* treeView,
                           QAbstractItemModel* model,
                           QAbstractItemView::SelectionMode selectionMode,
                           bool stretchLastColumn,
                           bool showHeader = true);

    // File watcher handling
    void initializeFileWatcher();
    void refreshFileWatcherRoots();
    void startWatchingBackupDirectory(const QString& path);
    void resetFileWatcherAndDestinationView();
    QStringList getWatchedRoots() const;

    // Backup status and labels
    QString checkInstallIntegrityStatus();
    void updateApplicationStatusLabel();
    void refreshBackupStatus();
    void updateLastBackupInfo();
    void updateBackupStatusLabel(const QString& statusColor);
    void updateBackupLocationLabel(const QString& location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void updateBackupLocationStatusLabel(const QString& location);
    void updateBackupLabels(const BackupScanResult& scan);
    void notifyOrphanOrBrokenBackupIssues(const BackupScanResult& scan);
    QPair<QString, QString> statusVisualsForColor(const QString& color) const;

    // Notification system
    void setupNotificationButton();
    void updateNotificationButtonState();
    void showNotificationDialog();
    void feedbackNotificationButton();
    void displayNotificationPopup(const NotificationServiceStruct& notif);
    void finishNotificationQueue();
    void showNextNotification();

    // Button feedback
    void resetCreateBackupButtonState();
    void triggerButtonFeedback(QPushButton* button,
                               const QString& feedbackText,
                               const QString& originalText,
                               int durationMs = System::Timing::k_BUTTON_FEEDBACK_DURATION_MS);

private slots:
    void onAddToBackupClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void onDeleteAllBackupsClicked();
    void onChangeBackupDestinationClicked();
    void onBackupDirectoryChanged();
    void onFileChanged(const QString& path);
    void onBackupCompleted();
    void onBackupError(const QString& error);
    void onCooldownFinished();
    void onNotificationButtonClicked();
    void onUninstallClicked();
    void refreshFileWatcher();

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
