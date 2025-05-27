// Project includes
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindowlabels.h"
#include "mainwindowmessages.h"
#include "mainwindowstyling.h"
#include "../systemtraycontextmenu.h"
#include "../settingsdialog/settingsdialog.h"
#include "../../ui/notificationsdialog/notificationsdialog.h"
#include "../../ui/snaplistdialog/snaplistdialog.h"
#include "../../core/backup/constants/backupconstants.h"
#include "../../core/backup/models/destinationproxymodel.h"
#include "../../core/backup/models/stagingmodel.h"
#include "../../core/backup/controller/backupcontroller.h"
#include "../../core/backup/service/fileoperations.h"
#include "../../core/backup/service/stagingutils.h"
#include "../../services/ServiceDirector/ServiceDirector.h"
#include "../../services/ServiceManagers/FilewatcherServiceManager/FilewatcherServiceManager.h"
#include "../../services/ServiceManagers/FormatUtilsServiceManager/FormatUtilsServiceManager.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"
#include "../../services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.h"
#include "../../services/ServiceManagers/ToolbarServiceManager/ToolbarServiceManager.h"
#include "../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h"
#include "../../services/ServiceManagers/UninstallServiceManager/UninstallServiceManager.h"
#include "../../services/ServiceManagers/UserServiceManager/UserServiceManager.h"
#include "../../services/ServiceManagers/UserServiceManager/UserServiceConstants.h"
#include "../../services/ServiceManagers/SnapListServiceManager/snaplistservicemanager.h"
#include "../../../../services/ServiceManagers/BackupServiceManager/BackupServiceManager.h"
#include "../../../../constants/interface_config.h"
#include "../../../../constants/window_config.h"

// Qt includes
#include <QElapsedTimer>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QScreen>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QVBoxLayout>

// Constructor: Sets up main window, UI, and all services
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    snapListServiceManager(PathServiceManager::appConfigFolderPath() + "/user_lists.json", this) {
    ui->setupUi(this);
    initializeModels();
    initializeServices();
    initializeUiAndLayout();
    initializeToolbar();
    initializeBackupUi();
    initializeThemeHandling();
    setupTrayIcon();
}

// Destructor: Cleans up UI resources
MainWindow::~MainWindow() {
    delete ui;
}

// Initializes models used for source, destination, and staging
void MainWindow::initializeModels() {
    sourceModel = new QFileSystemModel(this);
    destinationModel = new QFileSystemModel(this);
    stagingModel = new StagingModel(this);
}

// Initializes required service managers and backup controller
void MainWindow::initializeServices() {
    fileWatcher = new FileWatcher(this);
    createBackupCooldownTimer = new QTimer(this);
    toolBar = new QToolBar(this);
    toolbarManager = new ToolbarServiceManager(this);

    const QString savedBackupDir = ServiceDirector::getInstance().getBackupServiceManager()->getBackupDirectory();
    PathServiceManager::setBackupDirectory(savedBackupDir);
    backupService = new BackupService(savedBackupDir);
    backupController = new BackupController(backupService, this);
}

// Initializes window settings and main layout
void MainWindow::initializeUiAndLayout() {
    configureWindow();
    setupLayout();
    initializeUI();
    applyButtonCursors();
    updateApplicationStatusLabel();
}

// Adds and configures main window toolbar
void MainWindow::initializeToolbar() {
    addToolBar(Qt::LeftToolBarArea, toolBar);
    toolbarManager->initialize(toolBar);
}

// Configures progress bar and backup status UI
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

    stagingTitleLayout = new QHBoxLayout(ui->StagingListTitleContainer);
    stagingTitleLayout->setContentsMargins(0, 0, 0, 0);
    stagingTitleLayout->setSpacing(6);
    stagingTitleLayout->setAlignment(Qt::AlignLeft);

    stagingTitleLabel = new QLabel("Backup Staging", ui->StagingListTitleContainer);
    stagingTitleLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    stagingTitleLayout->addWidget(stagingTitleLabel);

    snapListResetButton = new QPushButton("✕", ui->StagingListTitleContainer);
    snapListResetButton->setToolTip("Unload SnapList");
    snapListResetButton->setCursor(Qt::PointingHandCursor);
    snapListResetButton->setStyleSheet(MainWindowStyling::Styles::ResetSnapListButton::STYLE);
    snapListResetButton->setFixedSize(16, 16);
    snapListResetButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    snapListResetButton->setVisible(false);

    connect(snapListResetButton, &QPushButton::clicked, this, [this]() {
        stagingModel->clear();
        ui->BackupStagingTreeView->clearSelection();
        ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());
        updateBackupStagingTitle("");
    });

    stagingTitleLayout->addWidget(snapListResetButton);
    initializeBackupSystem();
}

// Sets tooltips and cursors for window buttons
void MainWindow::applyButtonCursors() {
    const QList<QPair<QPushButton *, QString>> buttons = {
        {ui->AddToBackupButton, MainWindowStyling::Styles::ToolTips::k_ADD_TO_BACKUP},
        {ui->RemoveFromBackupButton, MainWindowStyling::Styles::ToolTips::k_REMOVE_FROM_BACKUP},
        {ui->CreateBackupButton, MainWindowStyling::Styles::ToolTips::k_CREATE_BACKUP},
        {ui->ChangeBackupDestinationButton, MainWindowStyling::Styles::ToolTips::k_CHANGE_DESTINATION},
        {ui->DeleteBackupButton, MainWindowStyling::Styles::ToolTips::k_DELETE_BACKUP},
        {ui->NotificationButton, MainWindowStyling::Styles::ToolTips::k_NOTIFICATIONS},
        {ui->UnlockDriveButton, MainWindowStyling::Styles::ToolTips::k_UNLOCK_DRIVE}
    };

    std::for_each(buttons.begin(), buttons.end(), [](const auto &pair) {
        if (auto *button = pair.first) {
            button->setCursor(Qt::PointingHandCursor);
            button->setToolTip(pair.second);
        }
    });
}

// Connects theme change signal to update handler
void MainWindow::initializeThemeHandling() {
    connect(&ThemeServiceManager::instance(), &ThemeServiceManager::themeChanged,
            this, &MainWindow::onThemeChanged);
}

// Defines layout container and central widget
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

// Initializes the system tray icon and its menu
void MainWindow::setupTrayIcon() {
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(UI::Resources::k_ICON_PATH));
    trayIcon->setToolTip(App::Info::k_APP_NAME);

    auto* systemTrayContextMenu = new SystemTrayContextMenu(this);
    trayMenu = systemTrayContextMenu;

    QAction* actionOpen = systemTrayContextMenu->addAction("Open");
    QAction* actionSettings = systemTrayContextMenu->addAction("Settings...");
    systemTrayContextMenu->addSeparator();
    QAction* actionExit = systemTrayContextMenu->addAction("Exit");

    connect(systemTrayContextMenu, &SystemTrayContextMenu::safeTriggered, this, [this](QAction* action) {
        const QString text = action->text().trimmed();

        if (text == "Open") {
            this->showNormal();
            this->activateWindow();
        } else if (text == "Settings...") {
            if (settingsDialog && settingsDialog->isVisible()) {
                settingsDialog->raise();
                settingsDialog->activateWindow();
                return;
            }

            settingsDialog = new SettingsDialog(this);
            connect(settingsDialog, &SettingsDialog::requestBackupReset, this,
                    [this](const QString& path, const QString& type) {
                        this->handleBackupDeletion(path, type);
                    });
            connect(settingsDialog, &SettingsDialog::requestAppDataClear, this,
                    [this]() {
                        this->handleAppDataClear();
                    });

            settingsDialog->exec();
            delete settingsDialog;
            settingsDialog = nullptr;
        } else if (text == "Exit") {
            QApplication::quit();
        }
    });

    trayIcon->setContextMenu(systemTrayContextMenu);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this,
            [this, systemTrayContextMenu](QSystemTrayIcon::ActivationReason reason) {
                QPoint cursorPos = QCursor::pos();

                switch (reason) {
                case QSystemTrayIcon::Trigger:
                    this->showNormal();
                    this->activateWindow();
                    break;
                case QSystemTrayIcon::Context:
                    systemTrayContextMenu->popup(cursorPos);
                    break;
                case QSystemTrayIcon::DoubleClick:
                default:
                    break;
                }
            });
}

// Service and Feature Setup

// Initializes backup system and connects related components
void MainWindow::initializeBackupUi() {
    initializeBackupSystem();
    setupConnections();
    setupNotificationButton();
    initializeFileWatcher();

    createBackupCooldownTimer->setSingleShot(true);
    connect(createBackupCooldownTimer, &QTimer::timeout, this, [this]() {
        ui->CreateBackupButton->setEnabled(true);
    });
}

// Sets up backup directory, models, tree views, and UI state
void MainWindow::initializeBackupSystem() {
    const QString savedBackupDir = ServiceDirector::getInstance().getBackupServiceManager()->getBackupDirectory();
    PathServiceManager::setBackupDirectory(savedBackupDir);
    backupService->setBackupRoot(savedBackupDir);

    if (!FileOperations::createDirectory(savedBackupDir)) {
        QMessageBox::critical(this, ErrorMessages::k_BACKUP_INITIALIZATION_FAILED_TITLE,
                              ErrorMessages::k_ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY);
    }

    setupDestinationView(savedBackupDir);
    setupSourceTreeView();
    setupBackupStagingTreeView();
    ensureBackupStatusUpdated();
    applyCustomPalettesToAllTreeViews();
    ui->BackupViewContainer->setStyleSheet(MainWindowStyling::Styles::BackupViewContainer::STYLE);
}

// Event Handling

// Intercepts close event to prevent closing during active backup
void MainWindow::closeEvent(QCloseEvent* event) {
    bool minimizeOnClose = ServiceDirector::getInstance()
    .getUserServiceManager()
        ->settings()
        .value(UserServiceKeys::k_MINIMIZE_ON_CLOSE_KEY)
        .toBool(true);

    if (minimizeOnClose) {
        hide();
        event->ignore();
        return;
    }

    if (trayIcon) {
        trayIcon->setContextMenu(nullptr);
        trayIcon->hide();
        trayIcon->disconnect();
        trayIcon->deleteLater();
        trayIcon = nullptr;
    }

    if (trayMenu) {
        trayMenu->hide();
        trayMenu->deleteLater();
        trayMenu = nullptr;
    }

    QMainWindow::closeEvent(event);
}

// Window and Tree View Layout

// Refreshes all views when theme changes
void MainWindow::onThemeChanged() {
    applyCustomPalettesToAllTreeViews();
}

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

// Sets up the source directory view
void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath("");
    sourceModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
    configureTreeView(ui->DriveTreeView, sourceModel, QAbstractItemView::ExtendedSelection, true);
    ui->DriveTreeView->setRootIndex(sourceModel->index(""));
    ui->DriveTreeView->clearSelection();
    ui->DriveTreeView->setCurrentIndex(QModelIndex());
}

// Sets up the staging view for files queued for backup
void MainWindow::setupBackupStagingTreeView() {
    configureTreeView(ui->BackupStagingTreeView, stagingModel, QAbstractItemView::ExtendedSelection, true);
}

// Sets up destination backup view with current root path
void MainWindow::setupDestinationView() {
    setupDestinationView(backupService->getBackupRoot());
}

// Sets up destination backup view with given directory
void MainWindow::setupDestinationView(const QString &backupDir) {
    destinationModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    destinationModel->setNameFilters(QStringList() << "*");
    destinationModel->setNameFilterDisables(false);

    delete destinationProxyModel;
    destinationProxyModel = new DestinationProxyModel(this);
    destinationProxyModel->setSourceModel(destinationModel);
    destinationProxyModel->setExcludedFolderName(App::Items::k_BACKUP_SETUP_CONFIG_FOLDER);

    QModelIndex sourceRootIndex = destinationModel->setRootPath(backupDir);
    QModelIndex proxyRootIndex = destinationProxyModel->mapFromSource(sourceRootIndex);

    configureTreeView(ui->BackupDestinationView, destinationProxyModel, QAbstractItemView::SingleSelection, true);
    ui->BackupDestinationView->setModel(destinationProxyModel);
    ui->BackupDestinationView->setRootIndex(proxyRootIndex);
    applyCustomTreePalette(ui->BackupDestinationView);
    destinationProxyModel->sort(0);
}

// Tree View Utilities

// Configures a tree view for consistent layout and behavior
void MainWindow::configureTreeView(QTreeView *treeView, QAbstractItemModel *model,
                                   QAbstractItemView::SelectionMode selectionMode,
                                   bool stretchLastColumn, bool showHeader) {
    if (!treeView || !model) return;

    treeView->setModel(model);
    treeView->setSelectionMode(selectionMode);
    treeView->header()->setVisible(showHeader);
    treeView->header()->setStretchLastSection(stretchLastColumn);

    for (int i = UI::TreeView::k_START_HIDDEN_COLUMN; i < UI::TreeView::k_DEFAULT_COLUMN_COUNT; ++i) {
        treeView->setColumnHidden(i, true);
    }
}

// Applies theme-specific palette to a tree view
void MainWindow::applyCustomTreePalette(QTreeView *treeView) {
    if (!treeView) return;

    QPalette palette = treeView->palette();
    const auto theme = ThemeServiceManager::instance().currentTheme();

    QColor highlight = (theme == ThemeServiceConstants::AppTheme::Dark)
                           ? QColor(94, 61, 116)
                           : QColor(51, 153, 255);
    QColor text = (theme == ThemeServiceConstants::AppTheme::Dark)
                      ? QColor(212, 170, 255)
                      : QColor(0, 74, 153);

    palette.setColor(QPalette::Highlight, highlight);
    palette.setColor(QPalette::HighlightedText, text);
    treeView->setPalette(palette);
}

// Applies theme palettes to all tree views in the UI
void MainWindow::applyCustomPalettesToAllTreeViews() {
    std::array<QTreeView *, 3> trees = {
        ui->DriveTreeView,
        ui->BackupStagingTreeView,
        ui->BackupDestinationView
    };
    for (QTreeView *tree : trees) {
        applyCustomTreePalette(tree);
    }
}


// File Watcher and Path Monitoring

void MainWindow::initializeFileWatcher() {
    const QStringList roots = getWatchedRoots();
    fileWatcher->startWatchingMultiple(roots);

    connect(fileWatcher, &FileWatcher::fileChanged, this, &MainWindow::handleWatchedPathChanged);
    connect(fileWatcher, &FileWatcher::directoryChanged, this, &MainWindow::handleWatchedPathChanged);
}

// Handles path change events and updates backup state
void MainWindow::handleWatchedPathChanged(const QString &path) {
    fileWatcher->addPath(path);

    const QString appConfigDir = PathServiceManager::appConfigFolderPath();
    const QString backupDir = PathServiceManager::backupDataRootDir();

    if (path.startsWith(appConfigDir)) updateApplicationStatusLabel();
    if (path.startsWith(backupDir)) ensureBackupStatusUpdated();

    checkStagingForReadAccessLoss();
}

// Checks for unreadable staged files and removes them
void MainWindow::checkStagingForReadAccessLoss() {
    QStringList lostAccess;
    const QStringList &stagedPaths = stagingModel->getStagedPaths();
    const int pathCount = stagedPaths.size();

    for (int i = 0; i < pathCount; ++i) {
        const QString &path = stagedPaths.at(i);
        QFileInfo info(path);
        if (!info.exists() || !info.isReadable()) {
            lostAccess << path;
        }
    }

    if (!lostAccess.isEmpty()) {
        const int lostCount = lostAccess.size();
        for (int i = 0; i < lostCount; ++i) {
            const QString &path = lostAccess.at(i);
            stagingModel->removePath(path);
        }

        NotificationServiceManager::instance().addNotification(
            QString(NotificationMessages::k_READ_ACCESS_LOST_MESSAGE).arg(lostAccess.join("\n")));

        ui->BackupStagingTreeView->clearSelection();
        ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());
    }
}

// Resets file watcher and reloads destination view
void MainWindow::resetFileWatcherAndDestinationView() {
    fileWatcher->startWatchingMultiple(getWatchedRoots());
    setupDestinationView(PathServiceManager::backupSetupFolderPath());
}

// Starts file watcher on specified path
void MainWindow::startWatchingBackupDirectory(const QString &path) {
    fileWatcher->startWatchingMultiple(QStringList() << path);
    connect(fileWatcher, &FileWatcher::directoryChanged, this, &MainWindow::onBackupDirectoryChanged);
}

// Returns key directories to monitor for file system changes
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

// Responds to file change notifications
void MainWindow::onFileChanged(const QString &path) {
    fileWatcher->addPath(path);

    const QString appConfigDir = PathServiceManager::appConfigFolderPath();
    const QString backupDir = PathServiceManager::backupDataRootDir();

    if (path.startsWith(appConfigDir)) updateApplicationStatusLabel();
    if (path.startsWith(backupDir)) ensureBackupStatusUpdated();
}

// Responds to directory changes within the backup directory
void MainWindow::onBackupDirectoryChanged() {
    refreshFileWatcher();
    ensureBackupStatusUpdated();
}

// Refreshes file watcher to rewatch necessary paths
void MainWindow::refreshFileWatcher() {
    fileWatcher->startWatchingMultiple(getWatchedRoots());
}

// Error and Completion Handling

// Handles backup error: resets progress text and re-enables the backup button
void MainWindow::onBackupError(const QString &error) {
    Q_UNUSED(error);
    ui->TransferProgressText->setText(UI::Progress::k_PROGRESS_BAR_INITIAL_MESSAGE);
    ui->TransferProgressText->setVisible(true);

    const int elapsed = backupStartTimer.elapsed();
    const int delay = qMax(0, System::Timing::k_BUTTON_FEEDBACK_DURATION_MS - elapsed);

    QTimer::singleShot(delay, this, [this]() {
        ui->CreateBackupButton->setText(Labels::Backup::k_CREATE_BACKUP_BUTTON_TEXT);
        ui->CreateBackupButton->setStyleSheet(QString());
        ui->CreateBackupButton->setEnabled(true);
    });
}

// Handles backup completion: resets UI, updates view, and highlights latest backup
void MainWindow::onBackupCompleted() {
    ui->TransferProgressText->setText(UI::Progress::k_PROGRESS_BAR_COMPLETION_MESSAGE);
    ui->TransferProgressText->setVisible(true);

    const int elapsed = backupStartTimer.elapsed();
    const int delay = qMax(0, System::Timing::k_BUTTON_FEEDBACK_DURATION_MS - elapsed);

    QTimer::singleShot(delay, this, [this]() {
        ui->CreateBackupButton->setText(Labels::Backup::k_CREATE_BACKUP_BUTTON_TEXT);
        ui->CreateBackupButton->setStyleSheet(MainWindowStyling::Styles::GeneralText::k_DEFAULT_STYLE);
        ui->CreateBackupButton->setEnabled(true);

        QTimer::singleShot(System::Timing::k_BUTTON_FEEDBACK_DURATION_MS, this, [this]() {
            ui->TransferProgressText->setText(UI::Progress::k_PROGRESS_BAR_INITIAL_MESSAGE);
        });

        ensureBackupStatusUpdated();
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


// Re-enables backup button after cooldown timer
void MainWindow::onCooldownFinished() {
    ui->CreateBackupButton->setEnabled(true);
}

// Refreshing and Updating Status

// Refreshes backup status if not already in progress
void MainWindow::ensureBackupStatusUpdated() {
    static bool isRefreshing = false;
    if (isRefreshing) return;
    isRefreshing = true;
    refreshBackupStatus();
    isRefreshing = false;
}

// Refreshes overall backup status and updates associated UI
void MainWindow::refreshBackupStatus() {
    latestBackupScan = backupService->scanForBackupStatus();
    updateBackupLabels();

    if (!latestBackupScan.isBroken() && orphanLogNotified) {
        orphanLogNotified = false;
    }

    notifyOrphanOrBrokenBackupIssues(latestBackupScan);
}

// Updates labels for backup status, count, size, and last backup info
void MainWindow::updateBackupLabels() {
    updateBackupMetadataLabels();
    updateLastBackupInfo();
    handleSpecialBackupLabelStates(latestBackupScan);

    const QString statusColor = !latestBackupScan.structureExists
                                    ? MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_NOT_FOUND
                                    : latestBackupScan.isBroken()
                                          ? MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_WARNING
                                          : MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_FOUND;

    updateBackupStatusLabel(statusColor);
}

// Updates backup status label with styled message
void MainWindow::setStatusLabel(QLabel *label, const QString &emoji,
                                const QString &text, const QString &style) {
    if (!label) return;

    label->setText(Labels::Backup::k_STATUS_LABEL.arg(emoji, text));
    label->setTextFormat(Qt::RichText);
    label->setStyleSheet(style);
}

// Updates all metadata labels related to the current backup state
void MainWindow::updateBackupMetadataLabels() {
    updateBackupLocationLabel(backupService->getBackupRoot());
    updateBackupTotalCountLabel();
    updateBackupTotalSizeLabel();
    updateBackupLocationStatusLabel(backupService->getBackupRoot());
}

// Displays last backup metadata in the UI
void MainWindow::updateLastBackupInfo() {
    const QJsonObject metadata = backupService->getLastBackupMetadata();

    if (metadata.isEmpty()) {
        ui->LastBackupNameLabel->setText(Labels::LastBackup::k_NO_BACKUPS_MESSAGE);
        ui->LastBackupTimestampLabel->clear();
        ui->LastBackupDurationLabel->clear();
        ui->LastBackupSizeLabel->clear();
        return;
    }

    const QString name = metadata.value(BackupMetadata::k_NAME).toString();
    const QString timestampRaw = metadata.value(BackupMetadata::k_TIMESTAMP).toString();
    const int durationSec = metadata.value(BackupMetadata::k_DURATION).toInt();
    const QString sizeStr = metadata.value(BackupMetadata::k_SIZE_READABLE).toString();

    const QString formattedTimestamp = Shared::Formatting::formatTimestamp(
        QDateTime::fromString(timestampRaw, Qt::ISODate),
        Backup::Timestamps::k_BACKUP_TIMESTAMP_DISPLAY_FORMAT);

    const QString formattedDuration = Shared::Formatting::formatDuration(durationSec);

    ui->LastBackupNameLabel->setText(Labels::LastBackup::k_NAME + name);
    ui->LastBackupTimestampLabel->setText(Labels::LastBackup::k_TIMESTAMP + formattedTimestamp);
    ui->LastBackupDurationLabel->setText(Labels::LastBackup::k_DURATION + formattedDuration);
    ui->LastBackupSizeLabel->setText(Labels::LastBackup::k_SIZE + sizeStr);
}

// Updates backup status label with emoji and message
void MainWindow::updateBackupStatusLabel(const QString &statusColor) {
    const auto [emoji, text] = statusVisualsForColor(statusColor);
    QString style;

    if (statusColor == MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_WARNING ||
        statusColor == MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_NOT_FOUND) {
        style = MainWindowStyling::Styles::GeneralText::k_RED_BOLD_STYLE;
    }

    setStatusLabel(ui->BackupStatusLabel, emoji, text, style);

    for (QLabel *label : {
                          ui->LastBackupNameLabel,
                          ui->LastBackupTimestampLabel,
                          ui->LastBackupDurationLabel,
                          ui->LastBackupSizeLabel }) {
        label->setVisible(true);
    }
}


// Returns emoji and label message based on backup status color
QPair<QString, QString> MainWindow::statusVisualsForColor(const QString &color) const {
    static const QMap<QString, QPair<QString, QString>> statusMap = {
        {MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_FOUND,
         {Labels::Emoji::k_GREEN, Labels::Backup::k_READY_LABEL}},
        {MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_WARNING,
         {Labels::Emoji::k_YELLOW, Labels::Backup::k_WARNING_LABEL}},
        {MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_NOT_FOUND,
         {Labels::Emoji::k_RED, Labels::Backup::k_NOT_INITIALIZED}}
    };

    return statusMap.value(color, {Labels::Emoji::k_RED, Labels::Backup::k_NOT_INITIALIZED});
}

// Updates backup location label and tooltip
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

// Updates label with total backup count
void MainWindow::updateBackupTotalCountLabel() {
    ui->BackupTotalCountLabel->setText(
        Labels::Backup::k_TOTAL_COUNT + QString::number(backupService->getBackupCount()));
}

// Updates label with total backup size
void MainWindow::updateBackupTotalSizeLabel() {
    ui->BackupTotalSizeLabel->setText(
        Labels::Backup::k_TOTAL_SIZE + Shared::Formatting::formatSize(backupService->getTotalBackupSize()));
}

// Updates label with backup directory accessibility
void MainWindow::updateBackupLocationStatusLabel(const QString &location) {
    QFileInfo dirInfo(location);
    const QString status = dirInfo.exists()
                               ? (dirInfo.isWritable() ? Labels::DirectoryStatus::k_WRITABLE
                                                       : Labels::DirectoryStatus::k_READ_ONLY)
                               : Labels::DirectoryStatus::k_UNKNOWN;

    ui->BackupLocationStatusLabel->setText(Labels::Backup::k_LOCATION_ACCESS + status);
}

// Updates UI to reflect special cases like no backups or errors
void MainWindow::handleSpecialBackupLabelStates(const BackupScanResult &scan) {
    const int backupCount = backupService->getBackupCount();
    const QString rootPath = backupService->getBackupRoot();
    const QFileInfo rootInfo(rootPath);
    const QDir backupDir(rootPath);
    const bool locationInitialized = rootInfo.exists();
    const bool backupDirEmpty = locationInitialized &&
                                backupDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).isEmpty();
    const bool hasAppIssue = !scan.validAppStructure;

    const bool isUninitialized =
        !scan.structureExists && !scan.validBackupStructure &&
        !scan.hasMissingLogs && !scan.hasOrphanedLogs && backupCount == 0;

    const bool hasBackupIssue =
        !isUninitialized && (!scan.validBackupStructure || scan.hasMissingLogs || scan.hasOrphanedLogs);

    const QString infoLine = Labels::Backup::k_SEE_NOTIFICATIONS_LABEL;

    if (hasAppIssue && hasBackupIssue) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_WARNING_SUMMARY_LABEL);
        ui->BackupTotalSizeLabel->setText(Labels::Backup::k_APP_AND_BACKUP_ISSUE_LABEL);
        ui->BackupLocationStatusLabel->setText(infoLine);
    } else if (hasBackupIssue) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_WARNING_SUMMARY_LABEL);
        ui->BackupTotalSizeLabel->setText(Labels::Backup::k_BACKUP_ISSUE_LABEL);
        ui->BackupLocationStatusLabel->setText(infoLine);
    } else if (hasAppIssue) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_WARNING_SUMMARY_LABEL);
        ui->BackupTotalSizeLabel->setText(Labels::Backup::k_APP_ISSUE_LABEL);
        ui->BackupLocationStatusLabel->setText(infoLine);
    } else if (isUninitialized || (backupCount == 0 && (backupDirEmpty || !locationInitialized))) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_NO_BACKUPS_COUNT_LABEL);
        ui->BackupTotalCountLabel->setStyleSheet(MainWindowStyling::Styles::LabelStyles::k_WARNING_LABEL_STYLE);
        ui->BackupTotalSizeLabel->setText("");
        ui->BackupLocationStatusLabel->setText("");

        ui->BackupTotalSizeLabel->hide();
        ui->BackupLocationStatusLabel->hide();
        return;
    } else {
        ui->BackupTotalCountLabel->setStyleSheet(MainWindowStyling::Styles::GeneralText::k_DEFAULT_STYLE);
        ui->BackupTotalSizeLabel->setStyleSheet(MainWindowStyling::Styles::GeneralText::k_DEFAULT_STYLE);
        ui->BackupLocationStatusLabel->setStyleSheet(MainWindowStyling::Styles::GeneralText::k_DEFAULT_STYLE);

        updateBackupTotalCountLabel();
        updateBackupTotalSizeLabel();
        updateBackupLocationStatusLabel(backupService->getBackupRoot());

        ui->BackupTotalSizeLabel->show();
        ui->BackupLocationStatusLabel->show();
        return;
    }

    ui->BackupTotalCountLabel->setStyleSheet(MainWindowStyling::Styles::LabelStyles::k_WARNING_LABEL_STYLE);
    ui->BackupTotalSizeLabel->setStyleSheet(MainWindowStyling::Styles::LabelStyles::k_WARNING_LABEL_STYLE);
    ui->BackupLocationStatusLabel->setStyleSheet(MainWindowStyling::Styles::LabelStyles::k_WARNING_LABEL_STYLE);

    ui->BackupTotalCountLabel->show();
    ui->BackupTotalSizeLabel->show();
    ui->BackupLocationStatusLabel->show();
}


// Displays backup integrity notifications only if the structure is partially broken and not uninitialized.
void MainWindow::notifyOrphanOrBrokenBackupIssues(const BackupScanResult &scan) {
    static bool notifiedBackupStructure = false;

    const int backupCount = backupService->getBackupCount();
    const bool isUninitialized =
        !scan.structureExists && !scan.validBackupStructure &&
        !scan.hasMissingLogs && !scan.hasOrphanedLogs && backupCount == 0;

    if (isUninitialized) {
        notifiedBackupStructure = false;
        return;
    }

    if (scan.hasOrphanedLogs && !notifiedBackupStructure) {
        NotificationServiceManager::instance().addNotification(NotificationMessages::k_ORPHANED_LOGS_MESSAGE);
        notifiedBackupStructure = true;
    }

    if (scan.hasMissingLogs && !notifiedBackupStructure) {
        NotificationServiceManager::instance().addNotification(NotificationMessages::k_MISSING_LOGS_MESSAGE);
        notifiedBackupStructure = true;
    }

    if (!scan.validBackupStructure && !notifiedBackupStructure) {
        NotificationServiceManager::instance().addNotification(NotificationMessages::k_BROKEN_BACKUP_STRUCTURE_MESSAGE);
        notifiedBackupStructure = true;
    }

    if (scan.validBackupStructure && !scan.hasOrphanedLogs && !scan.hasMissingLogs) {
        notifiedBackupStructure = false;
    }
}

// Revalidates backup and app structures, updates UI, and triggers notifications
void MainWindow::revalidateBackupAndAppStatus() {
    latestBackupScan = backupService->scanForBackupStatus();
    const QString appStatus = checkInstallIntegrityStatus();
    const bool validApp = appStatus == InfoMessages::k_INSTALL_OK;
    latestBackupScan.validAppStructure = validApp;

    updateBackupLabels();
    notifyOrphanOrBrokenBackupIssues(latestBackupScan);
}

// Revalidates backup and app structures using installation status
void MainWindow::revalidateBackupAndAppStatus(const QString &appStatus) {
    latestBackupScan = backupService->scanForBackupStatus();
    latestBackupScan.validAppStructure = (appStatus == InfoMessages::k_INSTALL_OK);
    updateBackupLabels();
    notifyOrphanOrBrokenBackupIssues(latestBackupScan);
}

// Application Integrity

// Updates the application installation status label and notifies on failure
void MainWindow::updateApplicationStatusLabel() {
    const QString status = checkInstallIntegrityStatus();

    QString emoji = (status == InfoMessages::k_INSTALL_OK)
                        ? Labels::Emoji::k_GREEN
                        : Labels::Emoji::k_RED;

    QString label = (status == InfoMessages::k_INSTALL_OK)
                        ? Labels::ApplicationStatus::k_HEALTHY
                        : Labels::ApplicationStatus::k_INVALID;

    ui->ApplicationStatusLabel->setText(
        Labels::ApplicationStatus::k_STATUS_LABEL.arg(emoji, label));
    ui->ApplicationStatusLabel->setTextFormat(Qt::RichText);

    static bool appIntegrityNotified = false;
    if (status != InfoMessages::k_INSTALL_OK && !appIntegrityNotified) {
        NotificationServiceManager::instance().addNotification(
            NotificationMessages::k_BROKEN_APP_STRUCTURE_MESSAGE);
        appIntegrityNotified = true;
    } else if (status == InfoMessages::k_INSTALL_OK) {
        appIntegrityNotified = false;
    }

    revalidateBackupAndAppStatus(status);
}

// Checks for missing expected configuration files
QString MainWindow::checkInstallIntegrityStatus() {
    static const QStringList expectedConfigFiles = {
        App::Items::k_APPDATA_SETUP_INFO_FILE,
        App::Items::k_APPDATA_SETUP_NOTIFICATIONS_FILE,
        App::Items::k_APPDATA_SETUP_USER_SETTINGS_FILE
    };

    const QString configDir = PathServiceManager::appConfigFolderPath();
    int missingCount = 0;

    for (const QString &file : expectedConfigFiles) {
        if (!QFile::exists(configDir + "/" + file)) {
            ++missingCount;
        }
    }

    if (missingCount == 0) return InfoMessages::k_INSTALL_OK;
    if (missingCount < expectedConfigFiles.size()) return InfoMessages::k_INSTALL_PARTIAL;
    return InfoMessages::k_INSTALL_BROKEN;
}

// UI Interactions and Connections

// Connects buttons and tree view selection to their respective handlers
void MainWindow::setupConnections() {
    connectBackupSignals();

    const QList<QPair<QPushButton *, void (MainWindow::*)()>> connections = {
        {ui->AddToBackupButton, &MainWindow::onAddToBackupClicked},
        {ui->ChangeBackupDestinationButton, &MainWindow::onChangeBackupDestinationClicked},
        {ui->RemoveFromBackupButton, &MainWindow::onRemoveFromBackupClicked},
        {ui->CreateBackupButton, &MainWindow::onCreateBackupClicked},
        {ui->DeleteBackupButton, &MainWindow::onDeleteBackupClicked},
        {ui->UnlockDriveButton, &MainWindow::onUnlockDriveClicked},
        {ui->SnapListButton, &MainWindow::onSnapListButtonClicked}

    };

    for (const auto &[button, slot] : connections) {
        if (button) {
            connect(button, &QPushButton::clicked, this, slot);
        }
    }

    connect(ui->DriveTreeView->selectionModel(),
            &QItemSelectionModel::currentChanged, this,
            &MainWindow::onDriveSelectionChanged);
}

// Binds backup-related signals to appropriate actions
void MainWindow::connectBackupSignals() {
    connect(backupController, &BackupController::backupCreated, this, [this]() {
        onBackupCompleted();
    });

    connect(backupController, &BackupController::backupDeleted, this,
            &MainWindow::ensureBackupStatusUpdated);

    connect(backupController, &BackupController::errorOccurred, this,
            [this](const QString &error) {
                onBackupError(error);
                QMessageBox::critical(this, ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE, error);
            });
}

// Provides visual feedback on button action for limited time
void MainWindow::triggerButtonFeedback(QPushButton *button,
                                       const QString &feedbackText,
                                       const QString &originalText,
                                       int durationMs) {
    if (!button) return;

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

// UI Slot Handlers

// Adds selected files from source view to staging
void MainWindow::onAddToBackupClicked() {
    const QModelIndexList selectedIndexes = ui->DriveTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_BACKUP_SELECTION_REQUIRED_TITLE,
                             ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_BACKUP);
        return;
    }

    QStringList alreadyStaged, toStage, notReadable;

    for (const QModelIndex &index : selectedIndexes) {
        if (!index.isValid() || index.column() != 0) continue;

        const QString path = sourceModel->filePath(index);
        QFileInfo fileInfo(path);

        if (!fileInfo.isReadable()) {
            notReadable << path;
        } else if (stagingModel->containsPath(path)) {
            alreadyStaged << path;
        } else {
            toStage << path;
        }
    }

    if (!notReadable.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_READ_ACCESS_DENIED_TITLE,
                             ErrorMessages::k_READ_ACCESS_DENIED_BODY.arg(notReadable.join("\n")));
    }

    if (!alreadyStaged.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_ALREADY_STAGED_TITLE,
                             ErrorMessages::k_ERROR_ALREADY_IN_STAGING.arg(alreadyStaged.join("\n")));
    }

    if (!toStage.isEmpty()) {
        stagingModel->addPaths(toStage);
        snapListServiceManager.setCurrentStagingEntries(stagingModel->getStagedPaths()); // ✅ ADD THIS LINE
        ui->BackupStagingTreeView->clearSelection();
        ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());

        triggerButtonFeedback(ui->AddToBackupButton,
                              Labels::Backup::k_ADD_TO_BACKUP_BUTTON_TEXT,
                              Labels::Backup::k_ADD_TO_BACKUP_ORIGINAL_TEXT);
    }
}


// Removes selected files from the staging view
void MainWindow::onRemoveFromBackupClicked() {
    const QModelIndexList selectedIndexes = ui->BackupStagingTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_REMOVE_SELECTION_REQUIRED_TITLE,
                             ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL);
        return;
    }

    Shared::Backup::removeSelectedPathsFromStaging(ui->BackupStagingTreeView, stagingModel);
    snapListServiceManager.setCurrentStagingEntries(stagingModel->getStagedPaths()); // ✅ ADD THIS LINE

    triggerButtonFeedback(ui->RemoveFromBackupButton,
                          Labels::Backup::k_REMOVE_FROM_BACKUP_BUTTON_TEXT,
                          Labels::Backup::k_REMOVE_FROM_BACKUP_ORIGINAL_TEXT);
}


// Opens dialog to change backup destination directory
void MainWindow::onChangeBackupDestinationClicked() {
    const QString selectedDir = QFileDialog::getExistingDirectory(
        this, InfoMessages::k_SELECT_BACKUP_DESTINATION_TITLE, QDir::rootPath());

    if (selectedDir.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_BACKUP_LOCATION_REQUIRED_TITLE,
                             ErrorMessages::k_ERROR_NO_BACKUP_LOCATION_PATH_SELECTED);
        return;
    }

    if (!FileOperations::createDirectory(selectedDir)) {
        QMessageBox::critical(this, ErrorMessages::k_BACKUP_DIRECTORY_ERROR_TITLE,
                              ErrorMessages::k_ERROR_CREATING_BACKUP_DIRECTORY);
        return;
    }

    backupService->setBackupRoot(selectedDir);
    ServiceDirector::getInstance().getBackupServiceManager()->setBackupDirectory(selectedDir);
    PathServiceManager::setBackupDirectory(selectedDir);

    setupDestinationView(selectedDir);
    ensureBackupStatusUpdated();
    refreshFileWatcher();

    triggerButtonFeedback(ui->ChangeBackupDestinationButton,
                          Labels::Backup::k_CHANGE_DESTINATION_BUTTON_TEXT,
                          Labels::Backup::k_CHANGE_DESTINATION_ORIGINAL_TEXT);
}

// Creates backup using staged files
void MainWindow::onCreateBackupClicked() {
    QStringList pathsToBackup = stagingModel->getStagedPaths();
    if (pathsToBackup.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_NO_ITEMS_STAGED_FOR_BACKUP_TITLE,
                             ErrorMessages::k_ERROR_NO_ITEMS_STAGED_FOR_BACKUP);
        return;
    }

    QStringList unreadablePaths;
    const int pathCount = pathsToBackup.size();
    for (int i = 0; i < pathCount; ++i) {
        const QString &path = pathsToBackup.at(i);
        QFileInfo fileInfo(path);
        if (!fileInfo.exists() || !fileInfo.isReadable()) {
            unreadablePaths << path;
        }
    }

    if (!unreadablePaths.isEmpty()) {
        const int unreadableCount = unreadablePaths.size();
        for (int i = 0; i < unreadableCount; ++i) {
            const QString &path = unreadablePaths.at(i);
            stagingModel->removePath(path);
        }

        NotificationServiceManager::instance().addNotification(
            QString(NotificationMessages::k_READ_ACCESS_LOST_MESSAGE).arg(unreadablePaths.join("\n")));

        ui->BackupStagingTreeView->clearSelection();
        ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());

        QMessageBox::warning(this, ErrorMessages::k_READ_ACCESS_DENIED_TITLE,
                             ErrorMessages::k_READ_ACCESS_DENIED_BODY.arg(unreadablePaths.join("\n")));
        return;
    }

    const QString backupRoot = destinationModel->rootPath();
    QString errorMessage;
    if (!FileOperations::createBackupInfrastructure(backupRoot, errorMessage)) {
        QMessageBox::critical(this, ErrorMessages::k_ERROR_BACKUP_ALREADY_IN_PROGRESS, errorMessage);
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

// Deletes selected backup from destination view
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

    if (QMessageBox::question(this, WarningMessages::k_WARNING_CONFIRM_BACKUP_DELETION,
                              QString(WarningMessages::k_MESSAGE_CONFIRM_BACKUP_DELETION)
                                  .arg(selectedPath),
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    ui->BackupDestinationView->setModel(nullptr);
    resetDestinationModels();

    handleBackupDeletion(selectedPath, "single");

    triggerButtonFeedback(ui->DeleteBackupButton,
                          Labels::Backup::k_DELETE_BACKUP_BUTTON_TEXT,
                          Labels::Backup::k_DELETE_BACKUP_ORIGINAL_TEXT);
}

// On Open SnapList management click
void MainWindow::onSnapListButtonClicked() {
    auto* dialog = new SnapListDialog(&snapListServiceManager, this);

    connect(dialog, &SnapListDialog::snapListLoaded, this,
            [this](const QVector<QString>& paths, const QString& name) {
                stagingModel->clear();
                stagingModel->addPaths(paths);
                ui->BackupStagingTreeView->clearSelection();
                ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());
                updateBackupStagingTitle(name);

                snapListServiceManager.setCurrentStagingEntries(paths);
            });

    dialog->exec();
    dialog->deleteLater();
}

void MainWindow::updateBackupStagingTitle(const QString& name) {
    const bool hasSnapList = !name.isEmpty();

    if (stagingTitleLabel) {
        stagingTitleLabel->setText(hasSnapList ? name : "Backup Staging");
        stagingTitleLabel->setStyleSheet(hasSnapList
                                             ? MainWindowStyling::Styles::StagingTitleLabel::STYLE
                                             : MainWindowStyling::Styles::GeneralText::k_DEFAULT_STYLE);
    }

    if (snapListResetButton) {
        snapListResetButton->setVisible(hasSnapList);
    }
}





// Drive Selection and Unlocking

// Enables the unlock button when a drive is selected
void MainWindow::onDriveSelectionChanged() {
    ui->UnlockDriveButton->setEnabled(true);
}

// Unlocks the selected BitLocker-protected drive
void MainWindow::onUnlockDriveClicked() {
    QString driveLetter = getSelectedDriveLetter();
    if (driveLetter.isEmpty()) {
        QMessageBox::warning(this, BitLockerMessages::k_NO_DRIVE_SELECTED_TITLE,
                             BitLockerMessages::k_NO_DRIVE_SELECTED_MESSAGE);
        return;
    }

    QString drivePath = driveLetter + ":/";
    QDir driveDir(drivePath);
    if (driveDir.exists() && driveDir.isReadable()) return;

    triggerButtonFeedback(ui->UnlockDriveButton,
                          Labels::Backup::k_UNLOCKING_FEEDBACK_TEXT,
                          Labels::Backup::k_UNLOCK_DRIVE_ORIGINAL_TEXT);

    QProcess taskKill;
    taskKill.start("taskkill", QStringList() << "/IM" << "manage-bde.exe" << "/F");
    taskKill.waitForFinished(2000);

    QString script = QStringLiteral(
                         "Start-Process manage-bde -ArgumentList '-unlock %1: -password' -Verb runAs")
                         .arg(driveLetter.toUpper());

    if (!QProcess::startDetached("powershell", QStringList() << "-Command" << script)) {
        QMessageBox::critical(this, BitLockerMessages::k_UNLOCK_FAILED_TITLE,
                              BitLockerMessages::k_UNLOCK_FAILED_MESSAGE);
        return;
    }

    QTimer::singleShot(7500, this, [this]() {
        ensureBackupStatusUpdated();
        refreshFileWatcher();
        ui->DriveTreeView->setModel(sourceModel);
        setupSourceTreeView();
    });
}

// Notifications

// Sets up the notification button and badge
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
    notificationBadge->move(ui->NotificationButton->width() - 12, 2);
    notificationBadge->hide();
    notificationBadge->raise();

    updateNotificationButtonState();
}

// Opens the full notification dialog
void MainWindow::onNotificationButtonClicked() {
    showNotificationDialog();
    feedbackNotificationButton();
}

// Updates the badge visibility based on unread notifications
void MainWindow::updateNotificationButtonState() {
    bool hasUnread = !NotificationServiceManager::instance().unreadNotifications().isEmpty();
    if (notificationBadge) {
        notificationBadge->setVisible(hasUnread);
    }
}

// Shows the next popup notification from the queue
void MainWindow::showNextNotification() {
    if (notificationQueue.isEmpty()) {
        finishNotificationQueue();
        return;
    }

    isNotificationPopupVisible = true;
    displayNotificationPopup(notificationQueue.takeFirst());
}

// Displays a single notification popup
void MainWindow::displayNotificationPopup(const NotificationServiceStruct &notif) {
    QString message = QString("[%1]\n%2")
    .arg(notif.timestamp.toLocalTime().toString(Backup::Timestamps::k_NOTIFICATION_TIMESTAMP_DISPLAY_FORMAT),
         notif.message);

    auto *box = new QMessageBox(this);
    box->setWindowTitle(InfoMessages::k_NOTIFICATION_POPUP_TITLE);
    box->setText(message);
    box->setAttribute(Qt::WA_DeleteOnClose);
    connect(box, &QMessageBox::finished, this, [this](int) {
        showNextNotification();
    });
    box->show();
}

// Marks notifications as read and resets state
void MainWindow::finishNotificationQueue() {
    isNotificationPopupVisible = false;
    NotificationServiceManager::instance().markAllAsRead();
    updateNotificationButtonState();
}

// Opens full-screen notification dialog
void MainWindow::showNotificationDialog() {
    QList<NotificationServiceStruct> notifications =
        NotificationServiceManager::instance().allNotifications();

    NotificationsDialog dialog(notifications, this);
    dialog.exec();

    NotificationServiceManager::instance().markAllAsRead();
    updateNotificationButtonState();
}

// Provides visual feedback when notification button is clicked
void MainWindow::feedbackNotificationButton() {
    triggerButtonFeedback(ui->NotificationButton,
                          Labels::Backup::k_NOTIFICATION_FEEDBACK_TEXT,
                          Labels::Backup::k_NOTIFICATION_BUTTON_TEXT,
                          System::Timing::k_BUTTON_FEEDBACK_DURATION_MS);
}

// Utility and Accessor Methods

// Returns the tab widget displaying backup details
QTabWidget *MainWindow::getDetailsTabWidget() {
    return ui->DetailsTabWidget;
}

// Extracts selected drive letter from tree view path
QString MainWindow::getSelectedDriveLetter() const {
    QModelIndex selectedIndex = ui->DriveTreeView->currentIndex();
    if (!selectedIndex.isValid()) return "";

    QString fullPath = sourceModel->filePath(selectedIndex);
    if (fullPath.length() < 2 || fullPath[1] != ':') return "";

    return fullPath.left(1).toUpper();
}

// Checks if a directory is writable by testing a temporary file
bool canWriteToDir(const QString &path) {
    QFile testFile(path + "/.writeTest");
    if (testFile.open(QIODevice::WriteOnly)) {
        testFile.remove();
        return true;
    }
    return false;
}

// Backup Maintenance

// Handles backup deletion or archive reset
void MainWindow::handleBackupDeletion(const QString &path, const QString &deleteType) {
    const QString correctBackupDir = backupService->getBackupRoot();

    if (deleteType == "reset") {
        if (fileWatcher) fileWatcher->removeAllPaths();

        ui->BackupDestinationView->setModel(nullptr);
        resetDestinationModels();

        backupController->resetBackupArchive(path);

        if (fileWatcher) fileWatcher->startWatchingMultiple({correctBackupDir});

        setupDestinationView(correctBackupDir);
        refreshFileWatcher();
        ensureBackupStatusUpdated();
    } else if (deleteType == "single") {

        backupController->deleteBackup(path);
        QTimer::singleShot(0, this, [this, correctBackupDir]() {
            resetDestinationModels();
            setupDestinationView(correctBackupDir);
            refreshFileWatcher();
            ensureBackupStatusUpdated();
        });
    }
}

// Clears app data and performs uninstall sequence
void MainWindow::handleAppDataClear() {
    NotificationServiceManager::instance().suspendNotifications(true);

    UninstallServiceManager uninstallService;
    if (!uninstallService.confirmUninstall(this)) {
        NotificationServiceManager::instance().suspendNotifications(false);
        return;
    }

    if (fileWatcher) fileWatcher->removeAllPaths();
    ui->BackupDestinationView->setModel(nullptr);
    resetDestinationModels();

    bool success = uninstallService.performUninstall();
    NotificationServiceManager::instance().suspendNotifications(false);

    if (success) QApplication::quit();
}

// Resets and reinitializes destination models
void MainWindow::resetDestinationModels() {
    delete destinationModel;
    destinationModel = new QFileSystemModel(this);

    delete destinationProxyModel;
    destinationProxyModel = nullptr;
}
