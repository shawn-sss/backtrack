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

class MainWindow final : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    // Constructs the main window
    explicit MainWindow(QWidget* parent = nullptr);

    // Destroys the main window
    ~MainWindow() override;

    // Returns the details tab widget
    QTabWidget* getDetailsTabWidget();

protected:
    // Handles window close event
    void closeEvent(QCloseEvent* event) override;

private:
    // Sets up window configuration and layout
    void configureWindow();
    void initializeUI();
    void setupLayout();
    void setupConnections();
    void applyButtonCursors();

    // Initializes the backup system
    void initializeBackupSystem();
    void connectBackupSignals();

    // Sets up file system views
    void setupSourceTreeView();
    void setupBackupStagingTreeView();
    void setupDestinationView();
    void removeAllColumnsFromTreeView(QTreeView* treeView);
    void configureTreeView(QTreeView* treeView,
                           QAbstractItemModel* model,
                           QAbstractItemView::SelectionMode selectionMode,
                           bool stretchLastColumn,
                           bool showHeader = true);

    // Initializes and manages file watcher
    void initializeFileWatcher();
    void refreshFileWatcherRoots();
    void startWatchingBackupDirectory(const QString& path);
    void resetFileWatcherAndDestinationView();
    QStringList getWatchedRoots() const;

    // Updates backup status and UI labels
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

    // Handles notification system logic
    void setupNotificationButton();
    void updateNotificationButtonState();
    void showNotificationDialog();
    void feedbackNotificationButton();
    void displayNotificationPopup(const NotificationServiceStruct& notif);
    void finishNotificationQueue();
    void showNextNotification();

    // Applies feedback styles to buttons
    void resetCreateBackupButtonState();
    void triggerButtonFeedback(QPushButton* button,
                               const QString& feedbackText,
                               const QString& originalText,
                               int durationMs = System::Timing::k_BUTTON_FEEDBACK_DURATION_MS);

    // Applies custom palette to tree views
    void applyCustomTreePalette(QTreeView* treeView);

private slots:
    // Handles backup-related actions
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

    // Handles UI updates and events
    void onCooldownFinished();
    void onNotificationButtonClicked();
    void onUninstallClicked();
    void refreshFileWatcher();
    void onThemeChanged();

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
