// Project includes
#include "mainwindow.h"
#include "mainwindowlabels.h"
#include "mainwindowmessages.h"
#include "mainwindowstyling.h"
#include "ui_mainwindow.h"

#include "../../../../constants/app_info.h"
#include "../../../../constants/backup_config.h"
#include "../../../../constants/interface_config.h"
#include "../../../../constants/kvp_info.h"
#include "../../../../constants/system_constants.h"
#include "../../../../constants/window_config.h"

#include "../../core/backup/controller/backupcontroller.h"
#include "../../core/backup/models/destinationproxymodel.h"
#include "../../core/backup/models/stagingmodel.h"
#include "../../core/backup/service/backupservice.h"
#include "../../core/shared/fileoperations.h"
#include "../../core/shared/filewatcher.h"
#include "../../core/shared/formatutils.h"
#include "../../core/shared/stagingutils.h"
#include "../../core/shared/uiutils.h"

#include "../../services/ServiceDirector/ServiceDirector.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceStruct.h"
#include "../../services/ServiceManagers/ToolbarServiceManager/ToolbarServiceManager.h"
#include "../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"
#include "../../ui/notificationsdialog/notificationsdialog.h"

// Qt includes
#include <QBuffer>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QTimer>
#include <QVBoxLayout>

// Constructs the main application window and initializes components
MainWindow::MainWindow(QWidget *parent)
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

    createBackupCooldownTimer->setSingleShot(true);
    connect(createBackupCooldownTimer, &QTimer::timeout, this, [this]() {
        ui->CreateBackupButton->setEnabled(true);
    });
}


// Destroys the main window
MainWindow::~MainWindow() {
    delete ui;
}

// Handle window close event
void MainWindow::closeEvent(QCloseEvent *event) {
    if (backupController->isBackupInProgress()) {
        QMessageBox::warning(this, ErrorMessages::k_ERROR_OPERATION_IN_PROGRESS,
                             WarningMessages::k_WARNING_OPERATION_STILL_RUNNING);
        event->ignore();
        return;
    }
    QMainWindow::closeEvent(event);
}

// Returns a pointer to the main details tab widget
QTabWidget *MainWindow::getDetailsTabWidget() {
    return ui->DetailsTabWidget;
}

// Configure basic window properties
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

// Initialize the UI
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

    const QString appConfigDir = PathServiceManager::appConfigFolderPath();
    const QString backupDir = PathServiceManager::backupDataRootDir();

    QStringList watchedRoots;
    if (QDir(appConfigDir).exists()) watchedRoots << appConfigDir;
    if (QDir(backupDir).exists()) watchedRoots << backupDir;

    fileWatcher->startWatchingMultiple(watchedRoots);

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

        QStringList updatedRoots = {appConfigDir, backupDir};
        fileWatcher->startWatchingMultiple(updatedRoots);
    });

    initializeBackupSystem();
}

// Set up main layout structure
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

// Set pointing hand cursors and tooltips for all main window buttons
void MainWindow::applyButtonCursors() {
    const QList<QPair<QPushButton *, QString>> buttons = {
        {ui->AddToBackupButton, "Add selected items to the backup staging area"},
        {ui->RemoveFromBackupButton, "Remove selected items from the backup staging area"},
        {ui->CreateBackupButton, "Start the backup process"},
        {ui->ChangeBackupDestinationButton, "Change the destination folder for backups"},
        {ui->DeleteBackupButton, "Delete the selected backup from the destination view"},
        {ui->ResetDestinationButton, "Delete ALL contents from the backup destination"},
        {ui->NotificationButton, "View backup notifications"},
        {ui->UninstallButton, "Uninstall the application and delete all local data"}
    };

    for (const auto &[button, tooltip] : buttons) {
        if (!button) continue;
        button->setCursor(Qt::PointingHandCursor);
        button->setToolTip(tooltip);
    }
}

// Configure tree view display settings
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

// Remove all extra columns from a tree view
void MainWindow::removeAllColumnsFromTreeView(QTreeView *treeView) {
    if (!treeView) return;
    if (QAbstractItemModel *model = treeView->model(); model) {
        for (int i = UI::TreeView::k_START_HIDDEN_COLUMN;
             i < UI::TreeView::k_DEFAULT_COLUMN_COUNT; ++i) {
            treeView->setColumnHidden(i, true);
        }
    }
}

// Setup connections between UI elements and internal logic
void MainWindow::setupConnections() {
    connectBackupSignals();

    const QList<QPair<QPushButton *, void (MainWindow::*)()>> connections = {
        {ui->AddToBackupButton, &MainWindow::onAddToBackupClicked},
        {ui->ChangeBackupDestinationButton, &MainWindow::onChangeBackupDestinationClicked},
        {ui->RemoveFromBackupButton, &MainWindow::onRemoveFromBackupClicked},
        {ui->CreateBackupButton, &MainWindow::onCreateBackupClicked},
        {ui->DeleteBackupButton, &MainWindow::onDeleteBackupClicked},
        {ui->ResetDestinationButton, &MainWindow::onDeleteAllBackupsClicked},
        {ui->UninstallButton, &MainWindow::onUninstallClicked}
    };

    for (const auto &[button, slot] : connections) {
        if (button) {
            connect(button, &QPushButton::clicked, this, slot);
        }
    }
}

// Connect backup-related signals and slots
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

// Start watching a directory for changes
void MainWindow::startWatchingBackupDirectory(const QString &path) {
    fileWatcher->startWatchingMultiple(QStringList() << path);
    connect(fileWatcher, &FileWatcher::directoryChanged, this,
            &MainWindow::onBackupDirectoryChanged);
}

// Update the file watcher with the current destination path
void MainWindow::updateFileWatcher() {
    const QStringList watchedRoots = getWatchedRoots();
    fileWatcher->startWatchingMultiple(watchedRoots);
}

// Handle file change event
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


// Handle backup directory change event
void MainWindow::onBackupDirectoryChanged() {
    updateFileWatcher();
    refreshBackupStatus();
}

// Get the list of directories to watch
QStringList MainWindow::getWatchedRoots() const {
    QStringList watchedRoots;
    const QString appConfigDir = PathServiceManager::appConfigFolderPath();
    const QString backupDir = PathServiceManager::backupDataRootDir();
    if (QDir(appConfigDir).exists()) watchedRoots << appConfigDir;
    if (QDir(backupDir).exists()) watchedRoots << backupDir;
    return watchedRoots;
}

// Reset file watcher and refresh backup destination view
void MainWindow::resetFileWatcherAndDestinationView() {
    const QStringList watchedRoots = getWatchedRoots();
    fileWatcher->startWatchingMultiple(watchedRoots);

    QString backupViewDir = PathServiceManager::backupSetupFolderPath();
    QModelIndex sourceRootIndex = destinationModel->setRootPath(backupViewDir);
    QModelIndex proxyRootIndex = destinationProxyModel->mapFromSource(sourceRootIndex);
    ui->BackupDestinationView->setRootIndex(proxyRootIndex);
}

// UI & Backup Initialization

// Initialize the backup system
void MainWindow::initializeBackupSystem() {
    const QString savedBackupDir = ServiceDirector::getInstance().getBackupDirectory();

    PathServiceManager::setBackupDirectory(savedBackupDir);
    backupService->setBackupRoot(savedBackupDir);

    if (!FileOperations::createDirectory(savedBackupDir)) {
        QMessageBox::critical(
            this, ErrorMessages::k_BACKUP_INITIALIZATION_FAILED_TITLE,
            ErrorMessages::k_ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY);
    }

    setupDestinationView();

    QModelIndex sourceRootIndex = destinationModel->setRootPath(savedBackupDir);
    QModelIndex proxyRootIndex = destinationProxyModel->mapFromSource(sourceRootIndex);
    ui->BackupDestinationView->setRootIndex(proxyRootIndex);

    setupSourceTreeView();
    setupBackupStagingTreeView();
    refreshBackupStatus();

    if (!fileWatcher->watchedDirectories().contains(savedBackupDir)) {
        startWatchingBackupDirectory(savedBackupDir);
        updateFileWatcher();
    }
}


// Set up source tree view
void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath("");
    sourceModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);

    configureTreeView(ui->DriveTreeView, sourceModel,
                      QAbstractItemView::ExtendedSelection, true);
    ui->DriveTreeView->setRootIndex(sourceModel->index(""));
    ui->DriveTreeView->clearSelection();
    ui->DriveTreeView->setCurrentIndex(QModelIndex());
}

// Set up backup staging tree view
void MainWindow::setupBackupStagingTreeView() {
    configureTreeView(ui->BackupStagingTreeView, stagingModel,
                      QAbstractItemView::ExtendedSelection, true);
}

// Set up destination view
void MainWindow::setupDestinationView() {
    destinationModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    destinationModel->setNameFilters(QStringList() << "*");
    destinationModel->setNameFilterDisables(false);

    if (!destinationProxyModel) {
        destinationProxyModel = new DestinationProxyModel(this);
        destinationProxyModel->setSourceModel(destinationModel);
        destinationProxyModel->setExcludedFolderName(
            App::Items::k_BACKUP_SETUP_CONFIG_FOLDER);
    }

    QString backupDir = PathServiceManager::backupSetupFolderPath();
    QModelIndex sourceRootIndex = destinationModel->setRootPath(backupDir);
    QModelIndex proxyRootIndex = destinationProxyModel->mapFromSource(sourceRootIndex);

    configureTreeView(ui->BackupDestinationView, destinationProxyModel,
                      QAbstractItemView::SingleSelection, true);
    ui->BackupDestinationView->setRootIndex(proxyRootIndex);
    destinationProxyModel->sort(0);
}

// Button Click Slots

// Add selected files to backup staging
void MainWindow::onAddToBackupClicked() {
    const QModelIndexList selectedIndexes = ui->DriveTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_BACKUP_SELECTION_REQUIRED_TITLE,
                             ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_BACKUP);
        return;
    }

    Shared::Backup::addSelectedPathsToStaging(ui->DriveTreeView, stagingModel);

    ui->BackupStagingTreeView->clearSelection();
    ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());

    triggerButtonFeedback(ui->AddToBackupButton,
                          Labels::Backup::k_ADD_TO_BACKUP_BUTTON_TEXT,
                          Labels::Backup::k_ADD_TO_BACKUP_ORIGINAL_TEXT);
}

// Remove selected files from backup staging
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

// Start the backup process
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
        QMessageBox::critical(
            this, ErrorMessages::k_ERROR_BACKUP_ALREADY_IN_PROGRESS, errorMessage);
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

// Delete selected backup
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
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        backupController->deleteBackup(selectedPath);
        triggerButtonFeedback(ui->DeleteBackupButton,
                              Labels::Backup::k_DELETE_BACKUP_BUTTON_TEXT,
                              Labels::Backup::k_DELETE_BACKUP_ORIGINAL_TEXT);
    }
}

// Delete all backups
void MainWindow::onDeleteAllBackupsClicked() {
    const QString backupLocation = destinationModel->rootPath();

    if (backupLocation.isEmpty() || !QDir(backupLocation).exists()) {
        QMessageBox::warning(this, ErrorMessages::k_BACKUP_LOCATION_INVALID_TITLE,
                             ErrorMessages::k_BACKUP_LOCATION_INVALID_MESSAGE);
        return;
    }

    const QMessageBox::StandardButton confirm = QMessageBox::warning(
        this, WarningMessages::k_DELETE_ALL_BACKUPS_WARNING_TITLE,
        WarningMessages::k_DELETE_ALL_BACKUPS_WARNING_MESSAGE.arg(backupLocation),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (confirm != QMessageBox::Yes)
        return;

    fileWatcher->removeAllPaths();

    QDir dir(backupLocation);
    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    bool success = true;

    for (const QFileInfo &entry : entries) {
        const QString entryPath = entry.absoluteFilePath();
        if (entry.isDir()) {
            if (!QDir(entryPath).removeRecursively()) {
                success = false;
            }
        } else {
            if (!QFile::remove(entryPath)) {
                success = false;
            }
        }
    }

    const QString configFolderName = App::Items::k_BACKUP_SETUP_CONFIG_FOLDER;
    const QString configFolderPath = QDir(backupLocation).filePath(configFolderName);
    if (QDir(configFolderPath).exists()) {
        if (!QDir(configFolderPath).removeRecursively()) {
            success = false;
        }
    }

    if (success) {
        QMessageBox::information(this, InfoMessages::k_DELETE_ALL_SUCCESS_TITLE,
                                 InfoMessages::k_DELETE_ALL_SUCCESS_MESSAGE);
    } else {
        QMessageBox::critical(this, ErrorMessages::k_DELETE_ALL_FAILED_TITLE,
                              ErrorMessages::k_DELETE_ALL_FAILED_MESSAGE);
    }

    resetFileWatcherAndDestinationView();
    refreshBackupStatus();
}

// Display the warning and Uninstall App
void MainWindow::onUninstallClicked() {
    fileWatcher->removeAllPaths();
    fileWatcher->disconnect();

    const bool success = ServiceDirector::getInstance().uninstallAppWithConfirmation(this);
    if (success) {
        QApplication::quit();
    }
}

// Change the backup destination
void MainWindow::onChangeBackupDestinationClicked() {
    const QString selectedDir = QFileDialog::getExistingDirectory(
        this, InfoMessages::k_SELECT_BACKUP_DESTINATION_TITLE, QDir::rootPath());

    if (selectedDir.isEmpty()) {
        QMessageBox::warning(
            this, ErrorMessages::k_BACKUP_LOCATION_REQUIRED_TITLE,
            ErrorMessages::k_ERROR_NO_BACKUP_LOCATION_PATH_SELECTED);
        return;
    }

    if (!FileOperations::createDirectory(selectedDir)) {
        QMessageBox::critical(this, ErrorMessages::k_BACKUP_DIRECTORY_ERROR_TITLE,
                              ErrorMessages::k_ERROR_CREATING_BACKUP_DIRECTORY);
        return;
    }

    backupService->setBackupRoot(selectedDir);
    ServiceDirector::getInstance().setBackupDirectory(selectedDir);
    PathServiceManager::setBackupDirectory(selectedDir);

    QModelIndex sourceRootIndex = destinationModel->setRootPath(selectedDir);
    QModelIndex proxyRootIndex = destinationProxyModel->mapFromSource(sourceRootIndex);
    ui->BackupDestinationView->setModel(destinationProxyModel);
    ui->BackupDestinationView->setRootIndex(proxyRootIndex);

    refreshBackupStatus();
    startWatchingBackupDirectory(selectedDir);
    updateFileWatcher();

    triggerButtonFeedback(ui->ChangeBackupDestinationButton,
                          Labels::Backup::k_CHANGE_DESTINATION_BUTTON_TEXT,
                          Labels::Backup::k_CHANGE_DESTINATION_ORIGINAL_TEXT);
}

// Notification handling

// Handles the click event on the notification button.
void MainWindow::onNotificationButtonClicked() {
    showNotificationDialog();
    feedbackNotificationButton();
}

// Sets up the notification button with text, connections, and badge.
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

// Updates the visibility of the notification badge based on unread notifications.
void MainWindow::updateNotificationButtonState() {
    const bool hasUnread =
        !NotificationServiceManager::instance().unreadNotifications().isEmpty();
    if (notificationBadge) {
        notificationBadge->setVisible(hasUnread);
    }
}

// Displays the next notification in the queue, or finishes the queue if empty.
void MainWindow::showNextNotification() {
    if (notificationQueue.isEmpty()) {
        finishNotificationQueue();
        return;
    }

    isNotificationPopupVisible = true;
    const NotificationServiceStruct notif = notificationQueue.takeFirst();
    displayNotificationPopup(notif);
}

// Shows a popup dialog for a single notification.
void MainWindow::displayNotificationPopup(const NotificationServiceStruct &notif) {
    const QString message =
        QString("[%1]\n%2")
            .arg(notif.timestamp.toLocalTime().toString("yyyy-MM-dd HH:mm:ss"),
                 notif.message);

    QMessageBox *box = new QMessageBox(this);
    box->setWindowTitle(InfoMessages::k_NOTIFICATION_POPUP_TITLE);
    box->setText(message);
    box->setAttribute(Qt::WA_DeleteOnClose);
    connect(box, &QMessageBox::finished, this,
            [this](int) { showNextNotification(); });
    box->show();
}

// Marks all notifications as read and updates badge state.
void MainWindow::finishNotificationQueue() {
    isNotificationPopupVisible = false;
    NotificationServiceManager::instance().markAllAsRead();
    updateNotificationButtonState();
}

// Opens the full notification dialog with a list of all notifications.
void MainWindow::showNotificationDialog() {
    const QList<NotificationServiceStruct> notifications =
        NotificationServiceManager::instance().allNotifications();

    NotificationsDialog dialog(notifications, this);
    dialog.exec();

    NotificationServiceManager::instance().markAllAsRead();
    updateNotificationButtonState();
}

// Temporarily changes button text to show feedback after interaction.
void MainWindow::feedbackNotificationButton() {
    triggerButtonFeedback(ui->NotificationButton,
                          Labels::Backup::k_NOTIFICATION_FEEDBACK_TEXT,
                          Labels::Backup::k_NOTIFICATION_BUTTON_TEXT,
                          System::Timing::k_BUTTON_FEEDBACK_DURATION_MS);
}

// Backup Feedback

// Temporarily changes a button’s text and state for feedback purposes.
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

// Re-enables the Create Backup button after cooldown period.
void MainWindow::onCooldownFinished() {
    ui->CreateBackupButton->setEnabled(true);
}

// Handles actions to take when a backup completes successfully.
void MainWindow::onBackupCompleted() {
    ui->TransferProgressText->setText(
        UI::Progress::k_PROGRESS_BAR_COMPLETION_MESSAGE);
    ui->TransferProgressText->setVisible(true);

    const int elapsed = backupStartTimer.elapsed();
    const int delay =
        qMax(0, System::Timing::k_BUTTON_FEEDBACK_DURATION_MS - elapsed);

    QTimer::singleShot(delay, this, [this]() {
        ui->CreateBackupButton->setText(
            Labels::Backup::k_CREATE_BACKUP_BUTTON_TEXT);
        ui->CreateBackupButton->setStyleSheet(QString());
        ui->CreateBackupButton->setEnabled(true);

        QTimer::singleShot(System::Timing::k_BUTTON_FEEDBACK_DURATION_MS, this,
                           [this]() {
                               ui->TransferProgressText->setText(
                                   UI::Progress::k_PROGRESS_BAR_INITIAL_MESSAGE);
                           });
    });

    startWatchingBackupDirectory(backupService->getBackupRoot());
    updateFileWatcher();
    refreshBackupStatus();
}

// Handles UI state and messaging when a backup fails.
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

// Backup Status & Labels

// Refreshes the backup status and labels based on current data.
void MainWindow::refreshBackupStatus() {
    if (backupController->isBackupInProgress()) {
        updateBackupStatusLabel(
            MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_FOUND);
        return;
    }

    const BackupScanResult scan = backupService->scanForBackupStatus();

    updateBackupLabels(scan);
    notifyOrphanOrBrokenBackupIssues(scan);

    if (!scan.isBroken()) {
        orphanLogNotified = false;
    }
}

// Updates all the UI labels related to backup metadata and structure.
void MainWindow::updateBackupLabels(const BackupScanResult &scan) {
    updateBackupLocationLabel(backupService->getBackupRoot());
    updateBackupTotalCountLabel();
    updateBackupTotalSizeLabel();
    updateBackupLocationStatusLabel(backupService->getBackupRoot());
    updateLastBackupInfo();

    const QString statusColor =
        !scan.structureExists
            ? MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_NOT_FOUND
            : scan.isBroken()
                  ? MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_WARNING
                  : MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_FOUND;

    updateBackupStatusLabel(statusColor);
}

// Adds notifications for any orphaned or broken backup structure issues.
void MainWindow::notifyOrphanOrBrokenBackupIssues(const BackupScanResult &scan) {
    if (!scan.isBroken() || orphanLogNotified)
        return;

    if (scan.hasOrphanedLogs) {
        NotificationServiceManager::instance().addNotification(
            NotificationMessages::k_ORPHANED_LOGS_MESSAGE);
    }

    if (scan.hasMissingLogs) {
        NotificationServiceManager::instance().addNotification(
            NotificationMessages::k_MISSING_LOGS_MESSAGE);
    }

    if (!scan.validStructure) {
        NotificationServiceManager::instance().addNotification(
            NotificationMessages::k_BROKEN_STRUCTURE_MESSAGE);
    }

    orphanLogNotified = true;
}

// Updates labels showing information about the last backup.
void MainWindow::updateLastBackupInfo() {
    const QJsonObject metadata = backupService->getLastBackupMetadata();

    if (metadata.isEmpty()) {
        ui->LastBackupNameLabel->setText(
            tr("No backups yet. Create one to see details here."));
        ui->LastBackupTimestampLabel->clear();
        ui->LastBackupDurationLabel->clear();
        ui->LastBackupSizeLabel->clear();
        return;
    }

    const QString name =
        metadata.value(App::KVP::BackupMetadata::k_NAME).toString();
    const QString timestampRaw =
        metadata.value(App::KVP::BackupMetadata::k_TIMESTAMP).toString();
    const int durationSec =
        metadata.value(App::KVP::BackupMetadata::k_DURATION).toInt();
    const QString sizeStr =
        metadata.value(App::KVP::BackupMetadata::k_SIZE_READABLE).toString();

    const QString formattedTimestamp = Shared::Formatting::formatTimestamp(
        QDateTime::fromString(timestampRaw, Qt::ISODate),
        Backup::Timestamps::k_BACKUP_TIMESTAMP_DISPLAY_FORMAT);

    const QString formattedDuration =
        Shared::Formatting::formatDuration(durationSec);

    ui->LastBackupNameLabel->setText(Labels::LastBackup::k_NAME + name);
    ui->LastBackupTimestampLabel->setText(Labels::LastBackup::k_TIMESTAMP +
                                          formattedTimestamp);
    ui->LastBackupDurationLabel->setText(Labels::LastBackup::k_DURATION +
                                         formattedDuration);
    ui->LastBackupSizeLabel->setText(Labels::LastBackup::k_SIZE + sizeStr);
}

// Returns an emoji and label string based on the backup status color.
QPair<QString, QString>
MainWindow::statusVisualsForColor(const QString &color) const {
    if (color == MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_FOUND) {
        return {"🟢", tr("Ready")};
    } else if (color ==
               MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_WARNING) {
        return {"🟡", tr("Warning")};
    } else {
        return {"🔴", tr("Not Initialized")};
    }
}

// Updates the backup status label with appropriate emoji and text.
void MainWindow::updateBackupStatusLabel(const QString &statusColor) {
    const auto [emoji, text] = statusVisualsForColor(statusColor);

    ui->BackupStatusLabel->setText(
        tr("<b>Backup Location Status:</b> %1 %2").arg(emoji, text));
    ui->BackupStatusLabel->setTextFormat(Qt::RichText);

    for (QLabel *label : {ui->LastBackupNameLabel, ui->LastBackupTimestampLabel,
                          ui->LastBackupDurationLabel, ui->LastBackupSizeLabel}) {
        label->setVisible(true);
    }
}

// Updates the label showing the backup directory location.
void MainWindow::updateBackupLocationLabel(const QString &location) {
    ui->BackupLocationLabel->setText(Labels::Backup::k_LOCATION + location);
}

// Updates the label showing the total number of backups.
void MainWindow::updateBackupTotalCountLabel() {
    ui->BackupTotalCountLabel->setText(
        Labels::Backup::k_TOTAL_COUNT +
        QString::number(backupService->getBackupCount()));
}

// Updates the label showing the total size of backups.
void MainWindow::updateBackupTotalSizeLabel() {
    ui->BackupTotalSizeLabel->setText(
        Labels::Backup::k_TOTAL_SIZE +
        Shared::Formatting::formatSize(backupService->getTotalBackupSize()));
}

// Updates the label indicating the write status of the backup location.
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

// Application Status

// Updates the application files integrity status in the UI.
void MainWindow::updateApplicationStatusLabel() {

    const QString status = checkInstallIntegrityStatus();
    QString emoji, label;

    if (status == "ok") {
        emoji = "🟢";
        label = tr("Ready");
    } else if (status == "partial") {
        emoji = "🟡";
        label = tr("Warning");
    } else {
        emoji = "🔴";
        label = tr("Corrupted");
    }

    ui->ApplicationStatusLabel->setText(
        tr("<b>Application Files Status:</b> %1 %2").arg(emoji, label));
    ui->ApplicationStatusLabel->setTextFormat(Qt::RichText);
}


// Checks for the presence of expected app configuration files.
QString MainWindow::checkInstallIntegrityStatus() {
    const QString configDir = PathServiceManager::appConfigFolderPath();

    const QStringList expectedFiles = {"app_init.json", "app_notifications.json",
                                       "user_settings.json"};

    int missingCount = 0;
    for (const QString &file : expectedFiles) {
        const QString fullPath = configDir + "/" + file;
        if (!QFile::exists(fullPath)) {
            ++missingCount;
        }
    }

    if (missingCount == 0) {
        return "ok";
    }
    if (missingCount < expectedFiles.size()) {
        return "partial";
    }
    return "broken";
}
