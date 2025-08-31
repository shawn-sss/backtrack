#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "../../ui/scheduledialog/scheduledialog.h"
#include "../../services/ServiceManagers/TemplateServiceManager/TemplateServiceManager.h"
#include "../../backup_module/service/backupservice.h"
#include "../../../../constants/system_constants.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceConstants.h" // ✅ needed for NotificationSettings::NotificationServiceStruct

// Qt includes
#include <QMainWindow>
#include <QFileSystemModel>
#include <QSystemTrayIcon>
#include <QElapsedTimer>
#include <QPointer>
#include <QPushButton>
#include <QHBoxLayout>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QCloseEvent>
#include <QLabel>
#include <QLayout>
#include <QList>
#include <QMenu>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QTabWidget>
#include <QTimer>
#include <QToolBar>
#include <QTreeView>

// Forward declaration (Custom class)
class BackupController;
class DestinationProxyModel;
class FileWatcher;
class NotificationServiceManager;
class StagingModel;
class ToolbarServiceManager;
class SettingsDialog;
class ScheduleServiceManager;

// Forward declaration (Qt class)
namespace Ui { class MainWindow; }

class MainWindow final : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    QTabWidget* getDetailsTabWidget();
    void handleBackupDeletion(const QString& path, const QString& deleteType);
    void handleAppDataClear();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    // Initialization
    void configureWindow();
    void initializeUI();
    void initializeUiAndLayout();
    void initializeModels();
    void initializeServices();
    void initializeThemeHandling();
    void setupLayout();
    void setupConnections();
    void setupTrayIcon();

    // Toolbar & views
    void initializeToolbar();
    void initializeBackupUi();
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void setupDestinationView();
    void setupDestinationView(const QString& backupDir);
    void configureTreeView(QTreeView* treeView, QAbstractItemModel* model,
                           QAbstractItemView::SelectionMode selectionMode,
                           bool stretchLastColumn, bool showHeader = true);

    // File watcher
    void initializeFileWatcher();
    void refreshFileWatcher();
    QStringList getWatchedRoots() const;
    void handleWatchedPathChanged(const QString& path);
    void checkStagingForReadAccessLoss();

    // Backup system & status
    void initializeBackupSystem();
    void connectBackupSignals();
    void updateBackupLabels();
    void updateBackupMetadataLabels();
    void updateLastBackupInfo();
    void updateBackupStatusLabel(const QString& statusColor);
    void updateBackupLocationLabel(const QString& location);
    void updateBackupLocationStatusLabel(const QString& location);
    void updateBackupTotalCountLabel();
    void updateBackupTotalSizeLabel();
    void handleSpecialBackupLabelStates(const BackupScanResult& scan);
    void ensureBackupStatusUpdated();
    void refreshBackupStatus();
    void notifyOrphanOrBrokenBackupIssues(const BackupScanResult& scan);
    void revalidateBackupAndAppStatus();
    void revalidateBackupAndAppStatus(const QString& appStatus);

    // UI utilities
    void applyButtonCursors();
    void applyCustomTreePalette(QTreeView* treeView);
    void applyCustomPalettesToAllTreeViews();
    void triggerButtonFeedback(QPushButton* button,
                               const QString& feedbackText,
                               const QString& originalText,
                               int durationMs = System::Timing::k_BUTTON_FEEDBACK_DURATION_MS);
    QPair<QString, QString> statusVisualsForColor(const QString& color) const;
    QString checkInstallIntegrityStatus();
    QString getSelectedDriveLetter() const;
    void resetDestinationModels();
    void updateApplicationStatusLabel();

    // Notifications
    void setupNotificationButton();
    void updateNotificationButtonState();
    void showNotificationDialog();
    void feedbackNotificationButton();
    void displayNotificationPopup(const NotificationSettings::NotificationServiceStruct& notif); // ✅ fixed type
    void showNextNotification();
    void finishNotificationQueue();

    // Template
    void updateBackupStagingTitle(const QString& name);
    void setInitialButtonTexts();

private slots:
    void onAddToBackupClicked();
    void onRemoveFromBackupClicked();
    void onCreateBackupClicked();
    void onDeleteBackupClicked();
    void onChangeBackupDestinationClicked();
    void onUnlockDriveClicked();
    void onTemplateButtonClicked();
    void onBackupDirectoryChanged();
    void onBackupCompleted();
    void onBackupError(const QString& error);
    void onCooldownFinished();
    void onNotificationButtonClicked();
    void onThemeChanged();
    void setStatusLabel(QLabel* label, const QString& emoji,
                        const QString& text, const QString& style = "");
    void onScheduleButtonClicked();

private:
    Ui::MainWindow* ui = nullptr;
    QToolBar* toolBar = nullptr;
    QLabel* notificationBadge = nullptr;
    void restoreStagingFromService();

    QVector<TemplateEntry> savedStagingBeforeTemplate;

    // Models & Services
    QFileSystemModel* sourceModel = nullptr;
    QFileSystemModel* destinationModel = nullptr;
    DestinationProxyModel* destinationProxyModel = nullptr;
    StagingModel* stagingModel = nullptr;
    BackupService* backupService = nullptr;
    BackupController* backupController = nullptr;
    FileWatcher* fileWatcher = nullptr;
    ToolbarServiceManager* toolbarManager = nullptr;
    TemplateServiceManager templateServiceManager;

    // Backup state
    BackupScanResult latestBackupScan;
    QElapsedTimer backupStartTimer;
    QTimer* createBackupCooldownTimer = nullptr;

    // Tray
    QSystemTrayIcon* trayIcon = nullptr;
    QMenu* trayMenu = nullptr;

    // Dialogs
    SettingsDialog* settingsDialog = nullptr;

    // Notifications
    QList<NotificationSettings::NotificationServiceStruct> notificationQueue; // ✅ fixed type
    bool isNotificationPopupVisible = false;
    bool orphanLogNotified = false;

    // Template / title bar
    QLabel* stagingTitleLabel = nullptr;
    QPushButton* templateResetButton = nullptr;
    QHBoxLayout* stagingTitleLayout = nullptr;

    QString loadedTemplateName;
    bool suppressNextMenuClick = false;
    bool ignoreTriggerAfterContext = false;

    // Scheduling
    ScheduleServiceManager* scheduleService_ = nullptr;
    QPointer<ScheduleDialog> openScheduleDialog_;
};

#endif // MAINWINDOW_H
