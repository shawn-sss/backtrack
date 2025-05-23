// Project includes
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindowlabels.h"
#include "mainwindowmessages.h"
#include "mainwindowstyling.h"

#include "../../../../constants/app_info.h"
#include "../../../../constants/backup_config.h"
#include "../../../../constants/interface_config.h"
#include "../../../../constants/kvp_info.h"
#include "../../../../constants/system_constants.h"
#include "../../../../constants/window_config.h"

#include "../../core/shared/fileoperations.h"
#include "../../core/shared/filewatcher.h"
#include "../../core/shared/formatutils.h"
#include "../../core/shared/stagingutils.h"
#include "../../core/shared/uiutils.h"

#include "../../core/backup/models/destinationproxymodel.h"
#include "../../core/backup/models/stagingmodel.h"
#include "../../core/backup/service/backupservice.h"
#include "../../core/backup/controller/backupcontroller.h"

#include "../../services/ServiceDirector/ServiceDirector.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceStruct.h"
#include "../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"
#include "../../services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.h"
#include "../../services/ServiceManagers/ToolbarServiceManager/ToolbarServiceManager.h"

#include "../../ui/notificationsdialog/notificationsdialog.h"

// Qt includes
#include <QElapsedTimer>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>
#include <QProcess>

// Constructor: Sets up main window, UI, and all services
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    sourceModel(new QFileSystemModel(this)),
    destinationModel(new QFileSystemModel(this)),
    stagingModel(new StagingModel(this)),
    fileWatcher(new FileWatcher(this)),
    backupService(new BackupService("")),
    backupController(new BackupController(backupService, this)),
    toolBar(new QToolBar(this)),
    toolbarManager(new ToolbarServiceManager(this)),
    createBackupCooldownTimer(new QTimer(this)) {

    ui->setupUi(this);

    configureWindow();
    initializeUI();
    setupLayout();
    updateApplicationStatusLabel();

    const QString savedBackupDir = ServiceDirector::getInstance().getBackupDirectory();
    PathServiceManager::setBackupDirectory(savedBackupDir);

    backupService = new BackupService(savedBackupDir);
    backupController = new BackupController(backupService, this);

    addToolBar(Qt::LeftToolBarArea, toolBar);
    toolbarManager->initialize(toolBar);

    applyButtonCursors();
    initializeBackupSystem();
    refreshBackupStatus();
    setupConnections();
    setupNotificationButton();

    ui->BackupViewContainer->setStyleSheet(MainWindowStyling::Styles::BackupViewContainer::STYLE);
    ui->BackupViewLayout->setContentsMargins(0, 0, 0, 0);
    ui->BackupViewLayout->setSpacing(3);
    ui->BackupViewLayout->setStretch(0, 1);
    ui->BackupViewLayout->setStretch(1, 1);
    ui->BackupViewLayout->setStretch(2, 1);

    ui->DashboardContainer->setStyleSheet(MainWindowStyling::Styles::DashboardContainer::STYLE);
    ui->DashboardDetails->setContentsMargins(0, 0, 0, 0);
    ui->DashboardDetails->setSpacing(3);
    ui->DashboardDetails->setStretch(0, 1);
    ui->DashboardDetails->setStretch(1, 1);
    ui->DashboardDetails->setStretch(2, 1);
    ui->DashboardLabel->setStyleSheet(MainWindowStyling::Styles::DashboardLabel::STYLE);

    ui->DriveTreeView->setObjectName("DriveTreeView");
    ui->BackupStagingTreeView->setObjectName("BackupStagingTreeView");
    ui->BackupDestinationView->setObjectName("BackupDestinationView");

    createBackupCooldownTimer->setSingleShot(true);
    connect(createBackupCooldownTimer, &QTimer::timeout, this, [this]() {
        ui->CreateBackupButton->setEnabled(true);
    });

    initializeFileWatcher();

    connect(&ThemeServiceManager::instance(), &ThemeServiceManager::themeChanged,
            this, &MainWindow::onThemeChanged);
}

// Destructor: Cleans up UI resources
MainWindow::~MainWindow() {
    delete ui;
}

// Qt event overrides

//  Intercepts close event; prevents closing if backup is in progress
void MainWindow::closeEvent(QCloseEvent *event) {
    if (backupController->isBackupInProgress()) {
        QMessageBox::warning(this, ErrorMessages::k_ERROR_OPERATION_IN_PROGRESS,
                             WarningMessages::k_WARNING_OPERATION_STILL_RUNNING);
        event->ignore();
        return;
    }
    QMainWindow::closeEvent(event);
}

// Window and UI setup

// Sets initial window size and position
void MainWindow::configureWindow() {
    setMinimumSize(App::Window::k_MINIMUM_WINDOW_SIZE);
    resize(App::Window::k_DEFAULT_WINDOW_SIZE);
    setMaximumSize(App::Window::k_MAXIMUM_WINDOW_SIZE);

    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        const QRect screenGeometry = screen->availableGeometry();
        const QPoint center = screenGeometry.center() -
                              QPoint(App::Window::k_DEFAULT_WINDOW_SIZE.width() / 2,
                                     App::Window::k_DEFAULT_WINDOW_SIZE.height() / 2);
        move(center);
    }
}

// Initializes progress bar and backup system UI
void MainWindow::initializeUI() {
    Shared::UI::setupProgressBar(ui->TransferProgressBar,
                                 UI::Progress::k_PROGRESS_BAR_MIN_VALUE,
                                 UI::Progress::k_PROGRESS_BAR_MAX_VALUE,
                                 UI::ProgressDetails::k_PROGRESS_BAR_HEIGHT,
                                 UI::Progress::k_PROGRESS_BAR_TEXT_VISIBLE);

    if (ui->TransferProgressBar->value() == 0) {
        ui->TransferProgressBar->setVisible(false);
        ui->TransferProgressText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TransferProgressText->setText(UI::Progress::k_PROGRESS_BAR_INITIAL_MESSAGE);
    }

    initializeBackupSystem();
}

// Defines main window layout
void MainWindow::setupLayout() {
    auto *container = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget *contentContainer = centralWidget();
    contentContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(contentContainer, 1);

    setCentralWidget(container);
}

// Applies tooltips and cursors to main window buttons
void MainWindow::applyButtonCursors() {
    const QList<QPair<QPushButton *, QString>> buttons = {
        {ui->AddToBackupButton, MainWindowStyling::Styles::ToolTips::k_ADD_TO_BACKUP},
        {ui->RemoveFromBackupButton, MainWindowStyling::Styles::ToolTips::k_REMOVE_FROM_BACKUP},
        {ui->CreateBackupButton, MainWindowStyling::Styles::ToolTips::k_CREATE_BACKUP},
        {ui->ChangeBackupDestinationButton, MainWindowStyling::Styles::ToolTips::k_CHANGE_DESTINATION},
        {ui->DeleteBackupButton, MainWindowStyling::Styles::ToolTips::k_DELETE_BACKUP},
        {ui->NotificationButton, MainWindowStyling::Styles::ToolTips::k_NOTIFICATIONS},
        {ui->UnlockDriveButton, MainWindowStyling::Styles::k_UNLOCK_DRIVE}
    };

    for (const auto &[button, tooltip] : buttons) {
        if (!button) continue;
        button->setCursor(Qt::PointingHandCursor);
        button->setToolTip(tooltip);
    }
}

// Connects UI buttons to their corresponding slots
void MainWindow::setupConnections() {
    connectBackupSignals();

    const QList<QPair<QPushButton *, void (MainWindow::*)()>> connections = {
        {ui->AddToBackupButton, &MainWindow::onAddToBackupClicked},
        {ui->ChangeBackupDestinationButton, &MainWindow::onChangeBackupDestinationClicked},
        {ui->RemoveFromBackupButton, &MainWindow::onRemoveFromBackupClicked},
        {ui->CreateBackupButton, &MainWindow::onCreateBackupClicked},
        {ui->DeleteBackupButton, &MainWindow::onDeleteBackupClicked},
        {ui->UnlockDriveButton, &MainWindow::onUnlockDriveClicked}
    };

    for (const auto &[button, slot] : connections) {
        if (button) {
            connect(button, &QPushButton::clicked, this, slot);
        }
    }

    connect(ui->DriveTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onDriveSelectionChanged);
}

// Connects backup signals to response handlers
void MainWindow::connectBackupSignals() {
    connect(backupController, &BackupController::backupCreated, this,
            &MainWindow::refreshBackupStatus);
    connect(backupController, &BackupController::backupCreated, this,
            &MainWindow::onBackupCompleted);

    connect(backupController, &BackupController::backupDeleted, this,
            &MainWindow::refreshBackupStatus);

    connect(backupController, &BackupController::errorOccurred, this,
            &MainWindow::onBackupError);
    connect(backupController, &BackupController::errorOccurred, this,
            [this](const QString &error) {
                QMessageBox::critical(this, ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE, error);
            });

    connect(backupController, &BackupController::backupCreated, this, [this]() {
        QTimer::singleShot(100, this, [this]() { refreshBackupStatus(); });
    });
}

// Backup system initialization

// Sets up backup directory and tree views
void MainWindow::initializeBackupSystem() {
    const QString savedBackupDir = ServiceDirector::getInstance().getBackupDirectory();

    PathServiceManager::setBackupDirectory(savedBackupDir);
    backupService->setBackupRoot(savedBackupDir);

    if (!FileOperations::createDirectory(savedBackupDir)) {
        QMessageBox::critical(this,
                              ErrorMessages::k_BACKUP_INITIALIZATION_FAILED_TITLE,
                              ErrorMessages::k_ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY);
    }

    setupDestinationView(savedBackupDir);

    setupSourceTreeView();
    setupBackupStagingTreeView();
    refreshBackupStatus();
    applyCustomTreePalette(ui->DriveTreeView);
    applyCustomTreePalette(ui->BackupStagingTreeView);
    applyCustomTreePalette(ui->BackupDestinationView);
}

// Sets up the source file browser view
void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath("");
    sourceModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);

    configureTreeView(ui->DriveTreeView, sourceModel,
                      QAbstractItemView::ExtendedSelection, true);
    ui->DriveTreeView->setRootIndex(sourceModel->index(""));
    ui->DriveTreeView->clearSelection();
    ui->DriveTreeView->setCurrentIndex(QModelIndex());
}

// Sets up the staging area tree view
void MainWindow::setupBackupStagingTreeView() {
    configureTreeView(ui->BackupStagingTreeView, stagingModel,
                      QAbstractItemView::ExtendedSelection, true);
}

// Sets up the destination backup view
void MainWindow::setupDestinationView() {
    setupDestinationView(backupService->getBackupRoot());
}

// Sets up the destination backup view (with location)
void MainWindow::setupDestinationView(const QString& backupDir) {
    destinationModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    destinationModel->setNameFilters(QStringList() << "*");
    destinationModel->setNameFilterDisables(false);

    if (destinationProxyModel) {
        delete destinationProxyModel;
    }

    destinationProxyModel = new DestinationProxyModel(this);
    destinationProxyModel->setSourceModel(destinationModel);
    destinationProxyModel->setExcludedFolderName(App::Items::k_BACKUP_SETUP_CONFIG_FOLDER);

    QModelIndex sourceRootIndex = destinationModel->setRootPath(backupDir);
    QModelIndex proxyRootIndex = destinationProxyModel->mapFromSource(sourceRootIndex);

    configureTreeView(ui->BackupDestinationView, destinationProxyModel,
                      QAbstractItemView::SingleSelection, true);
    ui->BackupDestinationView->setModel(destinationProxyModel);
    ui->BackupDestinationView->setRootIndex(proxyRootIndex);

    applyCustomTreePalette(ui->BackupDestinationView);
    destinationProxyModel->sort(0);
}

// Tree view utilities

// Configures tree view with model, selection mode, and layout
void MainWindow::configureTreeView(QTreeView *treeView, QAbstractItemModel *model,
                                   QAbstractItemView::SelectionMode selectionMode,
                                   bool stretchLastColumn, bool showHeader) {
    if (!treeView || !model) return;

    treeView->setModel(model);
    treeView->setSelectionMode(selectionMode);
    treeView->header()->setVisible(showHeader);
    treeView->header()->setStretchLastSection(stretchLastColumn);

    for (int i = UI::TreeView::k_START_HIDDEN_COLUMN;
         i < UI::TreeView::k_DEFAULT_COLUMN_COUNT; ++i) {
        treeView->setColumnHidden(i, true);
    }
}

// Hides all additional columns in a tree view
void MainWindow::removeAllColumnsFromTreeView(QTreeView *treeView) {
    if (!treeView) return;
    if (QAbstractItemModel *model = treeView->model(); model) {
        for (int i = UI::TreeView::k_START_HIDDEN_COLUMN;
             i < UI::TreeView::k_DEFAULT_COLUMN_COUNT; ++i) {
            treeView->setColumnHidden(i, true);
        }
    }
}

// Applies theme-based highlight and text colors to the given tree view if not already set
void MainWindow::applyCustomTreePalette(QTreeView* treeView) {
    if (!treeView) return;

    QPalette currentPalette = treeView->palette();
    const ThemeServiceConstants::AppTheme theme = ThemeServiceManager::instance().currentTheme();

    QColor expectedHighlightColor;
    QColor expectedTextColor;

    if (theme == ThemeServiceConstants::AppTheme::Dark) {
        expectedHighlightColor = QColor(94, 61, 116);
        expectedTextColor = QColor(212, 170, 255);
    } else {
        expectedHighlightColor = QColor(51, 153, 255);
        expectedTextColor = QColor(0, 74, 153);
    }

    if (currentPalette.color(QPalette::Highlight) == expectedHighlightColor &&
        currentPalette.color(QPalette::HighlightedText) == expectedTextColor) {
        return;
    }

    currentPalette.setColor(QPalette::Highlight, expectedHighlightColor);
    currentPalette.setColor(QPalette::HighlightedText, expectedTextColor);
    treeView->setPalette(currentPalette);
}

// Theme and Appearance Handling

// Applies updated theme styling to all tree views when the application theme changes
void MainWindow::onThemeChanged() {
    applyCustomTreePalette(ui->DriveTreeView);
    applyCustomTreePalette(ui->BackupStagingTreeView);
    applyCustomTreePalette(ui->BackupDestinationView);
}

// File watcher and directory monitoring

// Starts file watcher on key app directories
void MainWindow::initializeFileWatcher() {
    const QStringList roots = getWatchedRoots();
    fileWatcher->startWatchingMultiple(roots);

    connect(fileWatcher, &FileWatcher::fileChanged, this, [this](const QString &path) {
        fileWatcher->addPath(path);

        const QString appConfigDir = PathServiceManager::appConfigFolderPath();
        const QString backupDir = PathServiceManager::backupDataRootDir();

        if (path.startsWith(appConfigDir)) {
            updateApplicationStatusLabel();
        } else if (path.startsWith(backupDir)) {
            refreshBackupStatus();
        }
    });

    connect(fileWatcher, &FileWatcher::directoryChanged, this, [this](const QString &path) {
        const QString appConfigDir = PathServiceManager::appConfigFolderPath();
        const QString backupDir = PathServiceManager::backupDataRootDir();

        if (path.startsWith(appConfigDir)) {
            updateApplicationStatusLabel();
        } else if (path.startsWith(backupDir)) {
            refreshBackupStatus();
        }

    });
}

// Restarts file watcher and updates destination tree view
void MainWindow::resetFileWatcherAndDestinationView() {
    const QStringList watchedRoots = getWatchedRoots();
    fileWatcher->startWatchingMultiple(watchedRoots);

    setupDestinationView(PathServiceManager::backupSetupFolderPath());
}

// Starts watching a specific backup directory
void MainWindow::startWatchingBackupDirectory(const QString &path) {
    fileWatcher->startWatchingMultiple(QStringList() << path);
    connect(fileWatcher, &FileWatcher::directoryChanged, this,
            &MainWindow::onBackupDirectoryChanged);
}

// Returns list of all directories to watch for changes
QStringList MainWindow::getWatchedRoots() const {
    QStringList watchedRoots;

    const QString appConfigDir = PathServiceManager::appConfigFolderPath();
    const QString backupDataRoot = PathServiceManager::backupDataRootDir();
    const QString backupConfigDir = PathServiceManager::backupConfigFolderPath();

    if (QDir(appConfigDir).exists()) watchedRoots << appConfigDir;
    if (QDir(backupDataRoot).exists()) watchedRoots << backupDataRoot;
    if (QDir(backupConfigDir).exists()) watchedRoots << backupConfigDir;

    return watchedRoots;
}

// Responds to file changes by updating UI
void MainWindow::onFileChanged(const QString &path) {
    fileWatcher->addPath(path);

    const QString appConfigDir = PathServiceManager::appConfigFolderPath();
    const QString backupDir = PathServiceManager::backupDataRootDir();

    if (path.startsWith(appConfigDir)) {
        updateApplicationStatusLabel();
    }
    if (path.startsWith(backupDir)) {
        refreshBackupStatus();
    }
}

// Responds to directory changes in the backup directory
void MainWindow::onBackupDirectoryChanged() {
    initializeFileWatcher();
    refreshBackupStatus();
}

// Restarts the file watcher with current root paths
void MainWindow::refreshFileWatcher() {
    fileWatcher->startWatchingMultiple(getWatchedRoots());
}

// Backup validation and error handling

// Handles backup error: resets UI and button state
void MainWindow::onBackupError(const QString &error) {
    Q_UNUSED(error);
    ui->TransferProgressText->setText(
        UI::Progress::k_PROGRESS_BAR_INITIAL_MESSAGE);
    ui->TransferProgressText->setVisible(true);

    const int elapsed = backupStartTimer.elapsed();
    const int delay =
        qMax(0, System::Timing::k_BUTTON_FEEDBACK_DURATION_MS - elapsed);

    QTimer::singleShot(delay, this, [this]() {
        ui->CreateBackupButton->setText(
            Labels::Backup::k_CREATE_BACKUP_BUTTON_TEXT);
        ui->CreateBackupButton->setStyleSheet(QString());
        ui->CreateBackupButton->setEnabled(true);
    });
}

// Handles successful backup completion
void MainWindow::onBackupCompleted() {
    ui->TransferProgressText->setText(UI::Progress::k_PROGRESS_BAR_COMPLETION_MESSAGE);
    ui->TransferProgressText->setVisible(true);

    const int elapsed = backupStartTimer.elapsed();
    const int delay = qMax(0, System::Timing::k_BUTTON_FEEDBACK_DURATION_MS - elapsed);

    QTimer::singleShot(delay, this, [this]() {
        ui->CreateBackupButton->setText(Labels::Backup::k_CREATE_BACKUP_BUTTON_TEXT);
        ui->CreateBackupButton->setStyleSheet(QString());
        ui->CreateBackupButton->setEnabled(true);

        QTimer::singleShot(System::Timing::k_BUTTON_FEEDBACK_DURATION_MS, this, [this]() {
            ui->TransferProgressText->setText(UI::Progress::k_PROGRESS_BAR_INITIAL_MESSAGE);
        });

        refreshBackupStatus();
        refreshFileWatcher();

        const QString backupRoot = backupService->getBackupRoot();
        QDir dir(backupRoot);
        dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        dir.setSorting(QDir::Time | QDir::Reversed);

        const QFileInfoList entries = dir.entryInfoList();
        if (!entries.isEmpty()) {
            const QString newestFolderPath = entries.last().absoluteFilePath();
            QModelIndex sourceIndex = destinationModel->index(newestFolderPath);
            QModelIndex proxyIndex = destinationProxyModel->mapFromSource(sourceIndex);
            if (proxyIndex.isValid()) {
                ui->BackupDestinationView->setCurrentIndex(proxyIndex);
                ui->BackupDestinationView->selectionModel()->select(
                    proxyIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                ui->BackupDestinationView->scrollTo(proxyIndex);
            }
        }
    });
    ui->BackupDestinationView->clearSelection();
    ui->BackupDestinationView->setCurrentIndex(QModelIndex());
}

// Re-enables backup button after cooldown
void MainWindow::onCooldownFinished() {
    ui->CreateBackupButton->setEnabled(true);
}

// Backup status and UI label updates

// Refreshes backup state and updates all related UI elements
void MainWindow::refreshBackupStatus() {
    latestBackupScan = backupService->scanForBackupStatus();
    updateBackupLabels();

    if (!latestBackupScan.isBroken() && orphanLogNotified) {
        orphanLogNotified = false;
    }

    notifyOrphanOrBrokenBackupIssues(latestBackupScan);
}

// Updates backup-related labels including location, size, and status
void MainWindow::updateBackupLabels() {
    updateBackupLocationLabel(backupService->getBackupRoot());
    updateBackupTotalCountLabel();
    updateBackupTotalSizeLabel();
    updateBackupLocationStatusLabel(backupService->getBackupRoot());
    updateLastBackupInfo();
    handleSpecialBackupLabelStates(latestBackupScan);

    const QString statusColor =
        !latestBackupScan.structureExists
            ? MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_NOT_FOUND
            : latestBackupScan.isBroken()
                  ? MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_WARNING
                  : MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_FOUND;

    updateBackupStatusLabel(statusColor);
}

// Updates metadata labels for the last backup
void MainWindow::updateLastBackupInfo() {
    const QJsonObject metadata = backupService->getLastBackupMetadata();

    if (metadata.isEmpty()) {
        ui->LastBackupNameLabel->setText(Labels::LastBackup::k_NO_BACKUPS_MESSAGE);
        ui->LastBackupTimestampLabel->clear();
        ui->LastBackupDurationLabel->clear();
        ui->LastBackupSizeLabel->clear();
        return;
    }

    const QString name = metadata.value(App::KVP::BackupMetadata::k_NAME).toString();
    const QString timestampRaw = metadata.value(App::KVP::BackupMetadata::k_TIMESTAMP).toString();
    const int durationSec = metadata.value(App::KVP::BackupMetadata::k_DURATION).toInt();
    const QString sizeStr = metadata.value(App::KVP::BackupMetadata::k_SIZE_READABLE).toString();

    const QString formattedTimestamp = Shared::Formatting::formatTimestamp(
        QDateTime::fromString(timestampRaw, Qt::ISODate),
        Backup::Timestamps::k_BACKUP_TIMESTAMP_DISPLAY_FORMAT);

    const QString formattedDuration = Shared::Formatting::formatDuration(durationSec);

    ui->LastBackupNameLabel->setText(Labels::LastBackup::k_NAME + name);
    ui->LastBackupTimestampLabel->setText(Labels::LastBackup::k_TIMESTAMP + formattedTimestamp);
    ui->LastBackupDurationLabel->setText(Labels::LastBackup::k_DURATION + formattedDuration);
    ui->LastBackupSizeLabel->setText(Labels::LastBackup::k_SIZE + sizeStr);
}

// Updates the backup status label with emoji and color-coded text
void MainWindow::updateBackupStatusLabel(const QString& statusColor) {
    const auto [emoji, text] = statusVisualsForColor(statusColor);

    ui->BackupStatusLabel->setText(Labels::Backup::k_STATUS_LABEL.arg(emoji, text));
    ui->BackupStatusLabel->setTextFormat(Qt::RichText);

    for (QLabel* label : {
             ui->LastBackupNameLabel,
             ui->LastBackupTimestampLabel,
             ui->LastBackupDurationLabel,
             ui->LastBackupSizeLabel
         }) {
        label->setVisible(true);
    }
}

// Maps status color to emoji and label text
QPair<QString, QString> MainWindow::statusVisualsForColor(const QString &color) const {
    if (color == MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_FOUND) {
        return {Labels::Emoji::k_GREEN, Labels::Backup::k_READY_LABEL};
    } else if (color == MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_WARNING) {
        return {Labels::Emoji::k_YELLOW, Labels::Backup::k_WARNING_LABEL};
    } else {
        return {Labels::Emoji::k_RED, Labels::Backup::k_NOT_INITIALIZED};
    }
}

// Updates label with current backup directory path
void MainWindow::updateBackupLocationLabel(const QString &location) {
    const int maxChars = 45;

    QString displayText = location;
    if (location.length() > maxChars) {
        int half = maxChars / 2;
        displayText = location.left(half) + "..." + location.right(half);
    }

    ui->BackupLocationLabel->setText(displayText);
    ui->BackupLocationLabel->setToolTip(location);
}

// Updates label showing total number of backups
void MainWindow::updateBackupTotalCountLabel() {
    ui->BackupTotalCountLabel->setText(
        Labels::Backup::k_TOTAL_COUNT +
        QString::number(backupService->getBackupCount()));
}

// Updates label showing total backup size
void MainWindow::updateBackupTotalSizeLabel() {
    ui->BackupTotalSizeLabel->setText(
        Labels::Backup::k_TOTAL_SIZE +
        Shared::Formatting::formatSize(backupService->getTotalBackupSize()));
}

// Updates label showing write status of backup folder
void MainWindow::updateBackupLocationStatusLabel(const QString &location) {
    QFileInfo dirInfo(location);
    const QString status =
        dirInfo.exists()
            ? (dirInfo.isWritable() ? Labels::DirectoryStatus::k_WRITABLE
                                    : Labels::DirectoryStatus::k_READ_ONLY)
            : Labels::DirectoryStatus::k_UNKNOWN;
    ui->BackupLocationStatusLabel->setText(Labels::Backup::k_LOCATION_ACCESS +
                                           status);
}

// Notifies user of orphaned or broken backup issues
void MainWindow::notifyOrphanOrBrokenBackupIssues(const BackupScanResult &scan) {
    static bool notifiedAppStructure = false;
    static bool notifiedBackupStructure = false;

    if (scan.hasOrphanedLogs && !notifiedBackupStructure) {
        NotificationServiceManager::instance().addNotification(
            NotificationMessages::k_ORPHANED_LOGS_MESSAGE);
        notifiedBackupStructure = true;
    }

    if (scan.hasMissingLogs && !notifiedBackupStructure) {
        NotificationServiceManager::instance().addNotification(
            NotificationMessages::k_MISSING_LOGS_MESSAGE);
        notifiedBackupStructure = true;
    }

    if (!scan.validBackupStructure && !notifiedBackupStructure) {
        NotificationServiceManager::instance().addNotification(
            NotificationMessages::k_BROKEN_BACKUP_STRUCTURE_MESSAGE);
        notifiedBackupStructure = true;
    }

    if (!scan.validAppStructure && !notifiedAppStructure) {
        NotificationServiceManager::instance().addNotification(
            NotificationMessages::k_BROKEN_APP_STRUCTURE_MESSAGE);
        notifiedAppStructure = true;
    }

    if (scan.validAppStructure && scan.validBackupStructure &&
        !scan.hasOrphanedLogs && !scan.hasMissingLogs) {
        notifiedAppStructure = false;
        notifiedBackupStructure = false;
    }
}

// Adjusts backup summary labels for no backups, uninitialized location, or warnings
void MainWindow::handleSpecialBackupLabelStates(const BackupScanResult& scan) {
    const int backupCount = backupService->getBackupCount();
    const QString rootPath = backupService->getBackupRoot();
    const QFileInfo rootInfo(rootPath);
    const QDir backupDir(rootPath);

    const bool locationInitialized = rootInfo.exists();
    const bool backupDirEmpty = locationInitialized &&
                                backupDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).isEmpty();

    const bool hasAppIssue = !scan.validAppStructure;

    const bool isUninitialized =
        !scan.structureExists &&
        !scan.validBackupStructure &&
        !scan.hasMissingLogs &&
        !scan.hasOrphanedLogs &&
        backupCount == 0;

    const bool hasBackupIssue =
        !isUninitialized && (
            !scan.validBackupStructure ||
            scan.hasMissingLogs ||
            scan.hasOrphanedLogs
            );

    const QString redStyle = MainWindowStyling::Styles::GeneralText::k_RED_BOLD_STYLE;
    const QString infoLine = Labels::Backup::k_SEE_NOTIFICATIONS_LABEL;

    if (hasAppIssue && hasBackupIssue) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_WARNING_SUMMARY_LABEL);
        ui->BackupTotalSizeLabel->setText(Labels::Backup::k_APP_AND_BACKUP_ISSUE_LABEL);
        ui->BackupLocationStatusLabel->setText(infoLine);
    }
    else if (hasBackupIssue) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_WARNING_SUMMARY_LABEL);
        ui->BackupTotalSizeLabel->setText(Labels::Backup::k_BACKUP_ISSUE_LABEL);
        ui->BackupLocationStatusLabel->setText(infoLine);
    }
    else if (hasAppIssue) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_WARNING_SUMMARY_LABEL);
        ui->BackupTotalSizeLabel->setText(Labels::Backup::k_APP_ISSUE_LABEL);
        ui->BackupLocationStatusLabel->setText(infoLine);
    }
    else if (isUninitialized || (backupCount == 0 && (backupDirEmpty || !locationInitialized))) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_NO_BACKUPS_COUNT_LABEL);
        ui->BackupTotalCountLabel->setStyleSheet(redStyle);
        ui->BackupTotalSizeLabel->setText("");
        ui->BackupLocationStatusLabel->setText("");

        ui->BackupTotalSizeLabel->hide();
        ui->BackupLocationStatusLabel->hide();
        return;
    }
    else {
        ui->BackupTotalCountLabel->setStyleSheet("");
        ui->BackupTotalSizeLabel->setStyleSheet("");
        ui->BackupLocationStatusLabel->setStyleSheet("");

        updateBackupTotalCountLabel();
        updateBackupTotalSizeLabel();
        updateBackupLocationStatusLabel(backupService->getBackupRoot());

        ui->BackupTotalSizeLabel->show();
        ui->BackupLocationStatusLabel->show();
        return;
    }

    ui->BackupTotalCountLabel->setStyleSheet(redStyle);
    ui->BackupTotalSizeLabel->setStyleSheet(redStyle);
    ui->BackupLocationStatusLabel->setStyleSheet(redStyle);

    ui->BackupTotalCountLabel->show();
    ui->BackupTotalSizeLabel->show();
    ui->BackupLocationStatusLabel->show();
}

// Re-scans backup and app structure, updates UI, and queues notifications if needed
void MainWindow::revalidateBackupAndAppStatus() {
    latestBackupScan = backupService->scanForBackupStatus();
    const QString appStatus = checkInstallIntegrityStatus();
    const bool validApp = appStatus == InfoMessages::k_INSTALL_OK;
    latestBackupScan.validAppStructure = validApp;
    updateBackupLabels();
    notifyOrphanOrBrokenBackupIssues(latestBackupScan);
}

// Application status and integrity

// Checks and updates the app installation status label
void MainWindow::updateApplicationStatusLabel() {
    const QString status = checkInstallIntegrityStatus();

    QString emoji, label;
    if (status == InfoMessages::k_INSTALL_OK) {
        emoji = Labels::Emoji::k_GREEN;
        label = Labels::ApplicationStatus::k_HEALTHY;
    } else {
        emoji = Labels::Emoji::k_RED;
        label = Labels::ApplicationStatus::k_INVALID;
    }

    ui->ApplicationStatusLabel->setText(
        Labels::ApplicationStatus::k_STATUS_LABEL.arg(emoji, label));
    ui->ApplicationStatusLabel->setTextFormat(Qt::RichText);

    static bool appIntegrityNotified = false;
    if (status != InfoMessages::k_INSTALL_OK && !appIntegrityNotified) {
        NotificationServiceManager::instance().addNotification(
            NotificationMessages::k_BROKEN_APP_STRUCTURE_MESSAGE);
        appIntegrityNotified = true;
    }

    if (status == InfoMessages::k_INSTALL_OK) {
        appIntegrityNotified = false;
    }

    revalidateBackupAndAppStatus();
}


// Checks for missing expected configuration files
QString MainWindow::checkInstallIntegrityStatus() {
    const QString configDir = PathServiceManager::appConfigFolderPath();

    int missingCount = 0;
    for (const QString &file : App::Files::k_EXPECTED_CONFIG_FILES) {
        const QString fullPath = configDir + "/" + file;
        if (!QFile::exists(fullPath)) {
            ++missingCount;
        }
    }

    if (missingCount == 0) {
        return InfoMessages::k_INSTALL_OK;
    }
    if (missingCount < App::Files::k_EXPECTED_CONFIG_FILES.size()) {
        return InfoMessages::k_INSTALL_PARTIAL;
    }
    return InfoMessages::k_INSTALL_BROKEN;
}

// UI button slot handlers

// Adds selected files to staging area
void MainWindow::onAddToBackupClicked() {
    const QModelIndexList selectedIndexes = ui->DriveTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this,
                             ErrorMessages::k_BACKUP_SELECTION_REQUIRED_TITLE,
                             ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_BACKUP);
        return;
    }

    QStringList alreadyStaged;
    QStringList toStage;
    QStringList notReadable;

    for (const QModelIndex &index : selectedIndexes) {
        if (!index.isValid() || index.column() != 0)
            continue;

        const QString path = sourceModel->filePath(index);

        QFileInfo fileInfo(path);
        if (!fileInfo.isReadable()) {
            notReadable << path;
            qDebug() << "Read access denied for:" << path;
            continue;
        }

        if (stagingModel->containsPath(path)) {
            alreadyStaged << path;
        } else {
            toStage << path;
        }
    }

    if (!notReadable.isEmpty()) {
        QMessageBox::warning(this,
                             ErrorMessages::k_READ_ACCESS_DENIED_TITLE,
                             ErrorMessages::k_READ_ACCESS_DENIED_BODY.arg(notReadable.join("\n")));
    }

    if (!alreadyStaged.isEmpty()) {
        QMessageBox::warning(this,
                             ErrorMessages::k_ALREADY_STAGED_TITLE,
                             ErrorMessages::k_ERROR_ALREADY_IN_STAGING.arg(alreadyStaged.join("\n")));
    }

    if (!toStage.isEmpty()) {
        stagingModel->addPaths(toStage);

        ui->BackupStagingTreeView->clearSelection();
        ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());

        triggerButtonFeedback(ui->AddToBackupButton,
                              Labels::Backup::k_ADD_TO_BACKUP_BUTTON_TEXT,
                              Labels::Backup::k_ADD_TO_BACKUP_ORIGINAL_TEXT);
    }
}

// Removes selected items from staging
void MainWindow::onRemoveFromBackupClicked() {
    const QModelIndexList selectedIndexes = ui->BackupStagingTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_REMOVE_SELECTION_REQUIRED_TITLE,
                             ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL);
        return;
    }

    Shared::Backup::removeSelectedPathsFromStaging(ui->BackupStagingTreeView, stagingModel);
    triggerButtonFeedback(ui->RemoveFromBackupButton,
                          Labels::Backup::k_REMOVE_FROM_BACKUP_BUTTON_TEXT,
                          Labels::Backup::k_REMOVE_FROM_BACKUP_ORIGINAL_TEXT);
}

// Opens directory picker to change backup destination
void MainWindow::onChangeBackupDestinationClicked() {
    const QString selectedDir = QFileDialog::getExistingDirectory(
        this, InfoMessages::k_SELECT_BACKUP_DESTINATION_TITLE, QDir::rootPath());

    if (selectedDir.isEmpty()) {
        QMessageBox::warning(this,
                             ErrorMessages::k_BACKUP_LOCATION_REQUIRED_TITLE,
                             ErrorMessages::k_ERROR_NO_BACKUP_LOCATION_PATH_SELECTED);
        return;
    }

    if (!FileOperations::createDirectory(selectedDir)) {
        QMessageBox::critical(this,
                              ErrorMessages::k_BACKUP_DIRECTORY_ERROR_TITLE,
                              ErrorMessages::k_ERROR_CREATING_BACKUP_DIRECTORY);
        return;
    }

    backupService->setBackupRoot(selectedDir);
    ServiceDirector::getInstance().setBackupDirectory(selectedDir);
    PathServiceManager::setBackupDirectory(selectedDir);

    setupDestinationView(selectedDir);

    refreshBackupStatus();
    refreshFileWatcher();

    triggerButtonFeedback(ui->ChangeBackupDestinationButton,
                          Labels::Backup::k_CHANGE_DESTINATION_BUTTON_TEXT,
                          Labels::Backup::k_CHANGE_DESTINATION_ORIGINAL_TEXT);
}

// Initiates the backup process
void MainWindow::onCreateBackupClicked() {
    const QStringList pathsToBackup = stagingModel->getStagedPaths();
    if (pathsToBackup.isEmpty()) {
        QMessageBox::warning(this,
                             ErrorMessages::k_NO_ITEMS_STAGED_FOR_BACKUP_TITLE,
                             ErrorMessages::k_ERROR_NO_ITEMS_STAGED_FOR_BACKUP);
        return;
    }

    const QString backupRoot = destinationModel->rootPath();
    QString errorMessage;
    if (!FileOperations::createBackupInfrastructure(backupRoot, errorMessage)) {
        QMessageBox::critical(this,
                              ErrorMessages::k_ERROR_BACKUP_ALREADY_IN_PROGRESS,
                              errorMessage);
        return;
    }

    backupStartTimer.start();

    triggerButtonFeedback(ui->CreateBackupButton,
                          Labels::Backup::k_BACKING_UP_BUTTON_TEXT,
                          Labels::Backup::k_CREATE_BACKUP_BUTTON_TEXT,
                          System::Timing::k_BUTTON_FEEDBACK_DURATION_MS);

    ui->TransferProgressBar->setValue(UI::Progress::k_PROGRESS_BAR_MIN_VALUE);
    ui->TransferProgressBar->setVisible(true);
    ui->TransferProgressText->setVisible(false);

    backupController->createBackup(backupRoot, pathsToBackup, ui->TransferProgressBar);
}

// Deletes selected backup
void MainWindow::onDeleteBackupClicked() {
    if (!destinationModel) {
        QMessageBox::critical(this, ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
                              ErrorMessages::k_ERROR_DESTINATION_MODEL_NULL);
        return;
    }

    const QModelIndex selectedIndex = ui->BackupDestinationView->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
                             ErrorMessages::k_ERROR_BACKUP_DELETE_FAILED);
        return;
    }

    auto fsModel = qobject_cast<QFileSystemModel *>(destinationModel);
    if (!fsModel) {
        QMessageBox::critical(this, ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
                              ErrorMessages::k_ERROR_MODEL_TYPE_INVALID);
        return;
    }

    const QString selectedPath =
        fsModel->filePath(destinationProxyModel->mapToSource(selectedIndex));
    if (selectedPath.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
                             ErrorMessages::k_ERROR_SELECTED_PATH_INVALID);
        return;
    }

    if (QMessageBox::question(
            this, WarningMessages::k_WARNING_CONFIRM_BACKUP_DELETION,
            QString(WarningMessages::k_MESSAGE_CONFIRM_BACKUP_DELETION).arg(selectedPath),
            QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    handleBackupDeletion(selectedPath, "single");

    triggerButtonFeedback(ui->DeleteBackupButton,
                          Labels::Backup::k_DELETE_BACKUP_BUTTON_TEXT,
                          Labels::Backup::k_DELETE_BACKUP_ORIGINAL_TEXT);
}

// Enables the unlock button when drive selection changes
void MainWindow::onDriveSelectionChanged() {
    ui->UnlockDriveButton->setEnabled(true);
}

// Unlocks the selected drive
void MainWindow::onUnlockDriveClicked() {
    QString driveLetter = getSelectedDriveLetter();
    if (driveLetter.isEmpty()) {
        QMessageBox::warning(this,
                             BitLockerMessages::k_NO_DRIVE_SELECTED_TITLE,
                             BitLockerMessages::k_NO_DRIVE_SELECTED_MESSAGE);
        return;
    }

    QString drivePath = driveLetter + ":/";
    QDir driveDir(drivePath);

    if (driveDir.exists() && driveDir.isReadable()) {
        return;
    }

    triggerButtonFeedback(ui->UnlockDriveButton,
                          Labels::Backup::k_UNLOCKING_FEEDBACK_TEXT,
                          Labels::Backup::k_UNLOCK_DRIVE_ORIGINAL_TEXT);

    QProcess taskKill;
    taskKill.start("taskkill", QStringList() << "/IM" << "manage-bde.exe" << "/F");
    taskKill.waitForFinished(2000);

    const QString script =
        QStringLiteral("Start-Process manage-bde -ArgumentList '-unlock %1: -password' -Verb runAs")
            .arg(driveLetter.toUpper());

    bool started = QProcess::startDetached("powershell", QStringList() << "-Command" << script);

    if (!started) {
        QMessageBox::critical(this,
                              BitLockerMessages::k_UNLOCK_FAILED_TITLE,
                              BitLockerMessages::k_UNLOCK_FAILED_MESSAGE);
        return;
    }

    QTimer::singleShot(10000, this, [this]() {
        refreshBackupStatus();
        refreshFileWatcher();
        ui->DriveTreeView->setModel(sourceModel);
        setupSourceTreeView();
    });
}

// Notification handling

// Initializes notification button and its badge
void MainWindow::setupNotificationButton() {
    ui->NotificationButton->setText(Labels::Backup::k_NOTIFICATION_BUTTON_TEXT);

    connect(ui->NotificationButton, &QPushButton::clicked, this,
            &MainWindow::onNotificationButtonClicked);
    connect(&NotificationServiceManager::instance(),
            &NotificationServiceManager::notificationsUpdated, this,
            &MainWindow::updateNotificationButtonState);

    notificationBadge = new QLabel(ui->NotificationButton);
    notificationBadge->setObjectName("NotificationBadge");
    notificationBadge->setFixedSize(10, 10);

    int offset = 2;
    notificationBadge->move(ui->NotificationButton->width() -
                                notificationBadge->width() - offset,
                            offset);
    notificationBadge->hide();
    notificationBadge->raise();

    updateNotificationButtonState();
}

// Handles click on notification button
void MainWindow::onNotificationButtonClicked() {
    showNotificationDialog();
    feedbackNotificationButton();
}

// Updates badge based on unread notifications
void MainWindow::updateNotificationButtonState() {
    const bool hasUnread =
        !NotificationServiceManager::instance().unreadNotifications().isEmpty();
    if (notificationBadge) {
        notificationBadge->setVisible(hasUnread);
    }
}

// Shows the next queued popup notification
void MainWindow::showNextNotification() {
    if (notificationQueue.isEmpty()) {
        finishNotificationQueue();
        return;
    }

    isNotificationPopupVisible = true;
    const NotificationServiceStruct notif = notificationQueue.takeFirst();
    displayNotificationPopup(notif);
}

// Displays a single notification popup
void MainWindow::displayNotificationPopup(const NotificationServiceStruct &notif) {
    const QString message =
        QString("[%1]\n%2")
            .arg(notif.timestamp.toLocalTime().toString(Backup::Timestamps::k_NOTIFICATION_TIMESTAMP_DISPLAY_FORMAT),
                 notif.message);

    QMessageBox *box = new QMessageBox(this);
    box->setWindowTitle(InfoMessages::k_NOTIFICATION_POPUP_TITLE);
    box->setText(message);
    box->setAttribute(Qt::WA_DeleteOnClose);
    connect(box, &QMessageBox::finished, this,
            [this](int) { showNextNotification(); });
    box->show();
}

// Marks notifications as read and hides badge
void MainWindow::finishNotificationQueue() {
    isNotificationPopupVisible = false;
    NotificationServiceManager::instance().markAllAsRead();
    updateNotificationButtonState();
}

// Opens full notification dialog
void MainWindow::showNotificationDialog() {
    const QList<NotificationServiceStruct> notifications =
        NotificationServiceManager::instance().allNotifications();

    NotificationsDialog dialog(notifications, this);
    dialog.exec();

    NotificationServiceManager::instance().markAllAsRead();
    updateNotificationButtonState();
}

// Provides feedback when notification button is clicked
void MainWindow::feedbackNotificationButton() {
    triggerButtonFeedback(ui->NotificationButton,
                          Labels::Backup::k_NOTIFICATION_FEEDBACK_TEXT,
                          Labels::Backup::k_NOTIFICATION_BUTTON_TEXT,
                          System::Timing::k_BUTTON_FEEDBACK_DURATION_MS);
}

// General UI feedback

// Provides temporary visual feedback on button interaction
void MainWindow::triggerButtonFeedback(QPushButton *button,
                                       const QString &feedbackText,
                                       const QString &originalText,
                                       int durationMs) {
    if (!button)
        return;

    button->setCheckable(true);
    button->setChecked(true);
    button->setText(feedbackText);
    button->setEnabled(false);

    QTimer::singleShot(durationMs, this, [button, originalText]() {
        button->setChecked(false);
        button->setText(originalText);
        button->setEnabled(true);
    });
}

// Accessors

// Returns the main details tab widget
QTabWidget* MainWindow::getDetailsTabWidget() {
    return ui->DetailsTabWidget;
}

// Returns the selected drive letter from the drive tree view
QString MainWindow::getSelectedDriveLetter() const {
    QModelIndex selectedIndex = ui->DriveTreeView->currentIndex();
    if (!selectedIndex.isValid()) {
        return "";
    }

    QString fullPath = sourceModel->filePath(selectedIndex);
    if (fullPath.length() < 2 || fullPath[1] != ':') {
        return "";
    }

    return fullPath.left(1).toUpper();
}

// Backup Maintenance and Cleanup

// Handles backup deletion/reset based on the provided delete type
void MainWindow::handleBackupDeletion(const QString& path, const QString& deleteType) {
    const QString correctBackupDir = backupService->getBackupRoot();

    ui->BackupDestinationView->setModel(nullptr);

    delete destinationModel;
    destinationModel = new QFileSystemModel(this);

    delete destinationProxyModel;
    destinationProxyModel = nullptr;

    if (deleteType == "reset") {
        if (fileWatcher) {
            fileWatcher->removeAllPaths();
        }

        backupController->resetBackupArchive(path);

        if (fileWatcher) {
            fileWatcher->startWatchingMultiple({ correctBackupDir });
        }

    } else if (deleteType == "single") {
        backupController->deleteBackup(path);
    }

    setupDestinationView(correctBackupDir);
    refreshFileWatcher();
    refreshBackupStatus();
}

// Handles app data clearing and shutdown process
void MainWindow::handleAppDataClear() {
    if (fileWatcher) {
        fileWatcher->removeAllPaths();
    }

    ui->BackupDestinationView->setModel(nullptr);

    delete destinationModel;
    destinationModel = new QFileSystemModel(this);

    delete destinationProxyModel;
    destinationProxyModel = nullptr;

    NotificationServiceManager::instance().suspendNotifications(true);
    bool success = ServiceDirector::getInstance().uninstallAppWithConfirmation(this);
    NotificationServiceManager::instance().suspendNotifications(false);

    if (success) {
        QApplication::quit();
    }
}

// Utility Methods

// Checks if the specified directory is writable by attempting to create and delete a temporary file
bool canWriteToDir(const QString& path) {
    QFile testFile(path + "/.writeTest");
    if (testFile.open(QIODevice::WriteOnly)) {
        testFile.remove();
        return true;
    }
    return false;
}
