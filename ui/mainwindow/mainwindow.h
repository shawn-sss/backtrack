#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "../../../../constants/system_constants.h"
#include "../../core/backup/service/backupservice.h"

// Qt includes
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QCloseEvent>
#include <QElapsedTimer>
#include <QFileSystemModel>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTabWidget>
#include <QTimer>
#include <QToolBar>
#include <QTreeView>
#include <QList>
#include <QPair>

// Forward declaration (Custom class)
class BackupController;
class BackupService;
class DestinationProxyModel;
class FileWatcher;
class NotificationServiceManager;
class StagingModel;
class ToolbarServiceManager;
class NotificationServiceStruct;

// Forward declaration (Qt class)
class QWidget;

namespace Ui {
class MainWindow;
}

class MainWindow final : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    // UI accessors
    QTabWidget* getDetailsTabWidget();

    // User actions
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
    void initializeUiAndLayout();
    void initializeThemeHandling();

    // Model and service setup
    void initializeModels();
    void initializeServices();
    void initializeBackupSystem();
    void connectBackupSignals();

    // Toolbar and views
    void initializeToolbar();
    void setupNotificationButton();
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void setupDestinationView();
    void setupDestinationView(const QString& backupDir);
    void configureTreeView(QTreeView* treeView, QAbstractItemModel* model,
                           QAbstractItemView::SelectionMode selectionMode,
                           bool stretchLastColumn, bool showHeader = true);
    void removeAllColumnsFromTreeView(QTreeView* treeView);

    // Backup UI and status updates
    void initializeBackupUi();
    void updateBackupLabels();
    void refreshBackupStatus();
    void updateBackupMetadataLabels();
    void updateLastBackupInfo();
    void updateBackupStatusLabel(const QString& statusColor);
    void updateBackupLocationLabel(const QString& location);
    void updateBackupLocationStatusLabel(const QString& location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void handleSpecialBackupLabelStates(const BackupScanResult& scan);
    void notifyOrphanOrBrokenBackupIssues(const BackupScanResult& scan);
    void ensureBackupStatusUpdated();
    void revalidateBackupAndAppStatus();
    void revalidateBackupAndAppStatus(const QString& appStatus);

    // Notifications
    void updateNotificationButtonState();
    void showNotificationDialog();
    void feedbackNotificationButton();
    void displayNotificationPopup(const NotificationServiceStruct& notif);
    void finishNotificationQueue();
    void showNextNotification();

    // File watcher
    void initializeFileWatcher();
    void startWatchingBackupDirectory(const QString& path);
    void resetFileWatcherAndDestinationView();
    QStringList getWatchedRoots() const;
    void checkStagingForReadAccessLoss();
    void refreshFileWatcher();
    void handleWatchedPathChanged(const QString& path);

    // UI utilities
    void applyButtonCursors();
    void triggerButtonFeedback(QPushButton* button,
                               const QString& feedbackText,
                               const QString& originalText,
                               int durationMs = System::Timing::k_BUTTON_FEEDBACK_DURATION_MS);
    void applyCustomTreePalette(QTreeView* treeView);
    void applyCustomPalettesToAllTreeViews();
    void styleThreeColumnLayout(QLayout* layout);
    QPair<QString, QString> statusVisualsForColor(const QString& color) const;
    QString checkInstallIntegrityStatus();

    // Drive and reset handling
    QString getSelectedDriveLetter() const;
    void onDriveSelectionChanged();
    void resetDestinationModel();
    void resetDestinationViews();
    void resetDestinationModels();
    void updateApplicationStatusLabel();

private slots:
    // User interaction slots
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
    void onThemeChanged();
    void onUnlockDriveClicked();
    void setStatusLabel(QLabel* label, const QString& emoji, const QString& text, const QString& style = "");

private:
    // UI components
    Ui::MainWindow* ui = nullptr;
    QToolBar* toolBar = nullptr;
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

    // Notification state
    QList<NotificationServiceStruct> notificationQueue;
    bool isNotificationPopupVisible = false;
    bool orphanLogNotified = false;

    // Backup scan cache
    BackupScanResult latestBackupScan;
};

#endif // MAINWINDOW_H
