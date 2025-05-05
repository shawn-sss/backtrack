// Project includes
#include "mainwindow.h"
#include "mainwindowstyling.h"
#include "ui_mainwindow.h"

#include "../../config/configsettings/_settings.h"
#include "../../config/configdirector/configdirector.h"
#include "../../config/configmanagers/notificationsmanager/notificationstruct.h"
#include "../../config/configmanagers/notificationsmanager/notificationsmanager.h"

#include "../../core/backup_module/controller/backupcontroller.h"
#include "../../core/backup_module/models/destinationproxymodel.h"
#include "../../core/backup_module/models/stagingmodel.h"
#include "../../core/backup_module/service/backupservice.h"
#include "../../core/backup_module/styles/backup_styling.h"

#include "../../core/utils/common_utils/utils.h"
#include "../../core/utils/file_utils/fileoperations.h"
#include "../../core/utils/file_utils/filewatcher.h"
#include "../../ui/toolbarmanager/toolbarmanager.h"
#include "../../ui/notificationsdialog/notificationsdialog.h"

// Qt includes
#include <QBuffer>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QTimer>
#include <QVBoxLayout>

// Constructs the main application window and initializes components
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    sourceModel(new QFileSystemModel(this)),
    destinationModel(new QFileSystemModel(this)),
    fileWatcher(new FileWatcher(this)),
    backupService(new BackupService(ConfigDirector::getInstance().getBackupDirectory())),
    stagingModel(new StagingModel(this)),
    backupController(new BackupController(backupService, this)),
    toolBar(new QToolBar(this)),
    toolbarManager(new ToolbarManager(this)),
    createBackupCooldownTimer(new QTimer(this))
{
    ui->setupUi(this);

    configureWindow();
    initializeUI();
    setupLayout();
    addToolBar(Qt::LeftToolBarArea, toolBar);
    toolbarManager->initialize(toolBar);

    applyButtonCursors();
    initializeBackupSystem();
    setupConnections();
    setupNotificationButton();

    ui->BackupViewContainer->setStyleSheet(Styles::BackupViewContainer::STYLE);
    ui->BackupViewLayout->setContentsMargins(0, 0, 0, 0);
    ui->BackupViewLayout->setSpacing(3);
    ui->BackupViewLayout->setStretch(0, 1);
    ui->BackupViewLayout->setStretch(1, 1);
    ui->BackupViewLayout->setStretch(2, 1);

    ui->DashboardContainer->setStyleSheet(Styles::DashboardContainer::STYLE);
    ui->DashboardDetails->setContentsMargins(0, 0, 0, 0);
    ui->DashboardDetails->setSpacing(3);
    ui->DashboardDetails->setStretch(0, 1);
    ui->DashboardDetails->setStretch(1, 1);
    ui->DashboardDetails->setStretch(2, 1);

    ui->DashboardLabel->setStyleSheet(Styles::DashboardLabel::STYLE);

    createBackupCooldownTimer->setSingleShot(true);
    connect(createBackupCooldownTimer, &QTimer::timeout, this, [this]() {
        ui->CreateBackupButton->setEnabled(true);
    });
}

// Destroys the main window and cleans up UI resources
MainWindow::~MainWindow() {
    delete ui;
}

// Event handlers

// Handle window close event
void MainWindow::closeEvent(QCloseEvent* event) {
    if (backupController->isBackupInProgress()) {
        QMessageBox::warning(this, ErrorMessages::k_ERROR_OPERATION_IN_PROGRESS,
                             WarningMessages::k_WARNING_OPERATION_STILL_RUNNING);
        event->ignore();
        return;
    }
    QMainWindow::closeEvent(event);
}

// UI setup and layout configuration

// Configure basic window properties
void MainWindow::configureWindow() {
    setMinimumSize(AppConfig::k_MINIMUM_WINDOW_SIZE);
    resize(AppConfig::k_DEFAULT_WINDOW_SIZE);
    setMaximumSize(AppConfig::k_MAXIMUM_WINDOW_SIZE);

    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        const QRect screenGeometry = screen->availableGeometry();
        const QPoint center = screenGeometry.center() - QPoint(AppConfig::k_DEFAULT_WINDOW_SIZE.width() / 2,
                                                               AppConfig::k_DEFAULT_WINDOW_SIZE.height() / 2);
        move(center);
    }
}

// Set up main layout structure
void MainWindow::setupLayout() {
    auto* container = new QWidget(this);
    auto* mainLayout = new QHBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget* contentContainer = centralWidget();
    contentContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(contentContainer, 1);

    setCentralWidget(container);
}

// Initialize the UI
void MainWindow::initializeUI() {
    Utils::UI::setupProgressBar(
        ui->TransferProgressBar,
        ProgressSettings::k_PROGRESS_BAR_MIN_VALUE,
        ProgressSettings::k_PROGRESS_BAR_MAX_VALUE,
        ProgressUI::k_PROGRESS_BAR_HEIGHT,
        ProgressSettings::k_PROGRESS_BAR_TEXT_VISIBLE
        );

    if (ui->TransferProgressBar->value() == 0) {
        ui->TransferProgressBar->setVisible(false);
        ui->TransferProgressText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TransferProgressText->setText(ProgressSettings::k_PROGRESS_BAR_INITIAL_MESSAGE);
    }

    // ✅ Preload hidden QMessageBox to fix white screen on first use
    QMessageBox preloadBox(this);
    preloadBox.setWindowTitle(" ");
    preloadBox.setText(" ");
    preloadBox.setStandardButtons(QMessageBox::NoButton);
    preloadBox.setAttribute(Qt::WA_DontShowOnScreen); // Don't flash on screen
    preloadBox.show(); // Force layout/render
    preloadBox.hide(); // Immediately hide
}


// Apply pointing hand cursor to buttons
void MainWindow::applyButtonCursors() {
    const QList<QPushButton*> buttons = {
        ui->AddToBackupButton,
        ui->RemoveFromBackupButton,
        ui->CreateBackupButton,
        ui->ChangeBackupDestinationButton,
        ui->DeleteBackupButton
    };

    for (auto* button : buttons) {
        button->setCursor(Qt::PointingHandCursor);
    }
}

// Setup connections between UI and logic
void MainWindow::setupConnections() {
    connectBackupSignals();

    const struct {
        QPushButton* button;
        void (MainWindow::*slot)();
    } buttonConnections[] = {
        { ui->AddToBackupButton, &MainWindow::onAddToBackupClicked },
        { ui->ChangeBackupDestinationButton, &MainWindow::onChangeBackupDestinationClicked },
        { ui->RemoveFromBackupButton, &MainWindow::onRemoveFromBackupClicked },
        { ui->CreateBackupButton, &MainWindow::onCreateBackupClicked },
        { ui->DeleteBackupButton, &MainWindow::onDeleteBackupClicked }
    };

    for (const auto& conn : buttonConnections) {
        connect(conn.button, &QPushButton::clicked, this, conn.slot);
    }

    connect(createBackupCooldownTimer, &QTimer::timeout, this, &MainWindow::resetCreateBackupButtonState);
}

// Connect backup-related signals
void MainWindow::connectBackupSignals() {
    connect(backupController, &BackupController::backupCreated, this, &MainWindow::refreshBackupStatus);
    connect(backupController, &BackupController::backupCreated, this, &MainWindow::onBackupCompleted);

    connect(backupController, &BackupController::backupDeleted, this, &MainWindow::refreshBackupStatus);

    connect(backupController, &BackupController::errorOccurred, this, &MainWindow::onBackupError);
    connect(backupController, &BackupController::errorOccurred, this,
            [this](const QString& error) {
                QMessageBox::critical(this, ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE, error);
            });
}

// Setup UI for Notifications Button
void MainWindow::setupNotificationButton() {
    ui->NotificationButton->setText(Labels::Backup::k_NOTIFICATION_BUTTON_TEXT);
    ui->NotificationButton->setCursor(Qt::PointingHandCursor);
    ui->NotificationButton->setToolTip("View Notifications");

    connect(ui->NotificationButton, &QPushButton::clicked, this, &MainWindow::onNotificationButtonClicked);
    connect(&NotificationsManager::instance(), &NotificationsManager::notificationsUpdated,
            this, &MainWindow::updateNotificationButtonState);

    // Badge setup
    notificationBadge = new QLabel(ui->NotificationButton);
    notificationBadge->setFixedSize(10, 10);
    notificationBadge->setStyleSheet("background-color: red; border-radius: 5px;");

    int offset = 2;
    notificationBadge->move(ui->NotificationButton->width() - notificationBadge->width() - offset, offset);
    notificationBadge->hide();
    notificationBadge->raise();

    updateNotificationButtonState();
}


// Update UI for Notifications Button
void MainWindow::updateNotificationButtonState() {
    const bool hasUnread = !NotificationsManager::instance().unreadNotifications().isEmpty();
    if (notificationBadge) {
        notificationBadge->setVisible(hasUnread);
    }
}


// Backup system initialization and view setup

// Initialize the backup system
void MainWindow::initializeBackupSystem() {
    const QString backupDirectory = ConfigDirector::getInstance().getBackupDirectory();
    if (!FileOperations::createDirectory(backupDirectory)) {
        QMessageBox::critical(this, ErrorMessages::k_BACKUP_INITIALIZATION_FAILED_TITLE,
                              ErrorMessages::k_ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY);
    }

    setupDestinationView();
    setupSourceTreeView();
    setupBackupStagingTreeView();
    refreshBackupStatus();

    if (!fileWatcher->watchedDirectories().contains(backupDirectory)) {
        startWatchingBackupDirectory(backupDirectory);
    }
}

// Set up source tree view
void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath(Backup::Paths::k_DEFAULT_ROOT_PATH);
    sourceModel->setFilter(Backup::FileSystem::k_FILE_SYSTEM_FILTER);

    ui->DriveTreeView->setModel(sourceModel);
    ui->DriveTreeView->setRootIndex(sourceModel->index(Backup::Paths::k_DEFAULT_ROOT_PATH));
    ui->DriveTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->DriveTreeView);

    ui->DriveTreeView->clearSelection();
    ui->DriveTreeView->setCurrentIndex(QModelIndex());
}

// Set up backup staging tree view
void MainWindow::setupBackupStagingTreeView() {
    ui->BackupStagingTreeView->setModel(stagingModel);
    ui->BackupStagingTreeView->header()->setVisible(true);
    ui->BackupStagingTreeView->header()->setStretchLastSection(true);
    ui->BackupStagingTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->BackupStagingTreeView);
}

// Set up destination view
void MainWindow::setupDestinationView() {
    destinationModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    destinationModel->setNameFilters(QStringList() << "*");
    destinationModel->setNameFilterDisables(false);
    destinationModel->sort(0, Qt::DescendingOrder);

    if (!destinationProxyModel) {
        destinationProxyModel = new DestinationProxyModel(this);
        destinationProxyModel->setSourceModel(destinationModel);
        destinationProxyModel->setExcludedFolderName(Backup::Infrastructure::k_BACKUP_SETUP_FOLDER);
    }

    ui->BackupDestinationView->setModel(destinationProxyModel);

    QString backupDir = ConfigDirector::getInstance().getBackupDirectory();
    QModelIndex sourceRootIndex = destinationModel->setRootPath(backupDir);
    QModelIndex proxyRootIndex = destinationProxyModel->mapFromSource(sourceRootIndex);
    ui->BackupDestinationView->setRootIndex(proxyRootIndex);

    removeAllColumnsFromTreeView(ui->BackupDestinationView);
}

// Remove unnecessary columns from tree view
void MainWindow::removeAllColumnsFromTreeView(QTreeView* treeView) {
    if (!treeView) return;

    if (QAbstractItemModel* model = treeView->model(); model) {
        for (int i = UISettings::TreeView::k_START_HIDDEN_COLUMN;
             i < UISettings::TreeView::k_DEFAULT_COLUMN_COUNT; ++i) {
            treeView->setColumnHidden(i, true);
        }
    }
}

// File watcher setup and monitoring

// Start watching a directory for changes
void MainWindow::startWatchingBackupDirectory(const QString& path) {
    fileWatcher->startWatching(path);
    connect(fileWatcher, &FileWatcher::directoryChanged, this, &MainWindow::onBackupDirectoryChanged);
}

// Update file watcher
void MainWindow::updateFileWatcher() {
    const QString watchPath = destinationModel->rootPath();
    if (!fileWatcher->watchedDirectories().contains(watchPath)) {
        fileWatcher->startWatching(watchPath);
    }
}

// Handle backup directory change event
void MainWindow::onBackupDirectoryChanged() {
    updateFileWatcher();
    refreshBackupStatus();
}

// Handle file change event
void MainWindow::onFileChanged(const QString& path) {
    Q_UNUSED(path);
    refreshBackupStatus();
}

// Backup operation handlers

// Add selected files to backup staging
void MainWindow::onAddToBackupClicked() {
    const QModelIndexList selectedIndexes = ui->DriveTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_BACKUP_SELECTION_REQUIRED_TITLE,
                             ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_BACKUP);
        return;
    }

    Utils::Backup::addSelectedPathsToStaging(ui->DriveTreeView, stagingModel);

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

    Utils::Backup::removeSelectedPathsFromStaging(ui->BackupStagingTreeView, stagingModel);
    triggerButtonFeedback(ui->RemoveFromBackupButton,
                          Labels::Backup::k_REMOVE_FROM_BACKUP_BUTTON_TEXT,
                          Labels::Backup::k_REMOVE_FROM_BACKUP_ORIGINAL_TEXT);
}

// Start the backup process
void MainWindow::onCreateBackupClicked() {
    const QStringList pathsToBackup = stagingModel->getStagedPaths();
    if (pathsToBackup.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_NO_ITEMS_STAGED_FOR_BACKUP_TITLE,
                             ErrorMessages::k_ERROR_NO_ITEMS_STAGED_FOR_BACKUP);
        return;
    }

    const QString backupRoot = destinationModel->rootPath();
    QString errorMessage;
    if (!FileOperations::createBackupInfrastructure(backupRoot, errorMessage)) {
        QMessageBox::critical(this, ErrorMessages::k_ERROR_BACKUP_ALREADY_IN_PROGRESS, errorMessage);
        return;
    }

    backupStartTimer.start();

    ui->CreateBackupButton->setText(Labels::Backup::k_BACKING_UP_BUTTON_TEXT);
    ui->CreateBackupButton->setStyleSheet(MainWindowStyling::BUTTON_FEEDBACK_STYLE);
    ui->CreateBackupButton->setEnabled(false);

    ui->TransferProgressBar->setValue(ProgressSettings::k_PROGRESS_BAR_MIN_VALUE);
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

    auto fsModel = qobject_cast<QFileSystemModel*>(destinationModel);
    if (!fsModel) {
        QMessageBox::critical(this, ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
                              ErrorMessages::k_ERROR_MODEL_TYPE_INVALID);
        return;
    }

    const QString selectedPath = fsModel->filePath(destinationProxyModel->mapToSource(selectedIndex));
    if (selectedPath.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
                             ErrorMessages::k_ERROR_SELECTED_PATH_INVALID);
        return;
    }

    if (QMessageBox::question(this, WarningMessages::k_WARNING_CONFIRM_BACKUP_DELETION,
                              QString(WarningMessages::k_MESSAGE_CONFIRM_BACKUP_DELETION).arg(selectedPath),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        backupController->deleteBackup(selectedPath);
        triggerButtonFeedback(ui->DeleteBackupButton,
                              Labels::Backup::k_DELETE_BACKUP_BUTTON_TEXT,
                              Labels::Backup::k_DELETE_BACKUP_ORIGINAL_TEXT);
    }
}

// Display the Notifications
void MainWindow::onNotificationButtonClicked() {
    const QList<AppNotification> all = NotificationsManager::instance().allNotifications();

    triggerButtonFeedback(ui->NotificationButton,
                          Labels::Backup::k_NOTIFICATION_FEEDBACK_TEXT,
                          Labels::Backup::k_NOTIFICATION_BUTTON_TEXT,
                          Timing::k_BUTTON_FEEDBACK_DURATION_MS);

    NotificationsDialog* dialog = new NotificationsDialog(all, this);
    dialog->exec();

    NotificationsManager::instance().markAllAsRead();
    updateNotificationButtonState();
}

// Display the next Notifications
void MainWindow::showNextNotification() {
    if (notificationQueue.isEmpty()) {
        isNotificationPopupVisible = false;
        NotificationsManager::instance().markAllAsRead();
        updateNotificationButtonState();
        return;
    }

    isNotificationPopupVisible = true;
    const AppNotification notif = notificationQueue.takeFirst();
    QString message = QString("[%1]\n%2")
                          .arg(notif.timestamp.toLocalTime().toString("yyyy-MM-dd HH:mm:ss"))
                          .arg(notif.message);

    QMessageBox* box = new QMessageBox(this);
    box->setWindowTitle("Notification");
    box->setText(message);
    box->setAttribute(Qt::WA_DeleteOnClose);
    connect(box, &QMessageBox::finished, this, [this](int) {
        showNextNotification();
    });
    box->show();
}

// Change the backup destination
void MainWindow::onChangeBackupDestinationClicked() {
    const QString selectedDir = QFileDialog::getExistingDirectory(
        this,
        InfoMessages::k_SELECT_BACKUP_DESTINATION_TITLE,
        Backup::Paths::k_DEFAULT_FILE_DIALOG_ROOT
        );

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

// Backup feedback and cooldown handling

// Display button feedback
void MainWindow::triggerButtonFeedback(QPushButton* button,
                                       const QString& feedbackText,
                                       const QString& originalText,
                                       int durationMs) {
    if (!button) return;

    button->setText(feedbackText);
    button->setStyleSheet(MainWindowStyling::BUTTON_FEEDBACK_STYLE);
    button->setEnabled(false);

    QTimer::singleShot(durationMs, this, [button, originalText]() {
        button->setText(originalText);
        button->setStyleSheet(QString());
        button->setEnabled(true);
    });
}


// Reset "Create Backup" button to initial state
void MainWindow::resetCreateBackupButtonState() {
    ui->CreateBackupButton->setText(Labels::Backup::k_CREATE_BACKUP_BUTTON_TEXT);
    ui->CreateBackupButton->setEnabled(true);
    ui->CreateBackupButton->setStyleSheet(QString());
}

// Enable backup button after cooldown
void MainWindow::onCooldownFinished() {
    ui->CreateBackupButton->setEnabled(true);
}

// Handle backup completion
void MainWindow::onBackupCompleted() {
    ui->TransferProgressText->setText(ProgressSettings::k_PROGRESS_BAR_COMPLETION_MESSAGE);
    ui->TransferProgressText->setVisible(true);

    const int elapsed = backupStartTimer.elapsed();
    const int delay = qMax(0, Timing::k_BUTTON_FEEDBACK_DURATION_MS - elapsed);

    QTimer::singleShot(delay, this, [this]() {
        ui->CreateBackupButton->setText(Labels::Backup::k_CREATE_BACKUP_BUTTON_TEXT);
        ui->CreateBackupButton->setStyleSheet(QString());
        ui->CreateBackupButton->setEnabled(true);

        QTimer::singleShot(Timing::k_BUTTON_FEEDBACK_DURATION_MS, this, [this]() {
            ui->TransferProgressText->setText(ProgressSettings::k_PROGRESS_BAR_INITIAL_MESSAGE);
        });
    });
}

// Handle backup error
void MainWindow::onBackupError(const QString& error) {
    Q_UNUSED(error);
    ui->TransferProgressText->setText(ProgressSettings::k_PROGRESS_BAR_INITIAL_MESSAGE);
    ui->TransferProgressText->setVisible(true);

    const int elapsed = backupStartTimer.elapsed();
    const int delay = qMax(0, Timing::k_BUTTON_FEEDBACK_DURATION_MS - elapsed);

    QTimer::singleShot(delay, this, [this]() {
        ui->CreateBackupButton->setText(Labels::Backup::k_CREATE_BACKUP_BUTTON_TEXT);
        ui->CreateBackupButton->setStyleSheet(QString());
        ui->CreateBackupButton->setEnabled(true);
    });
}

// Backup status and label updates

// Refresh the backup status
void MainWindow::refreshBackupStatus() {
    const BackupStatus status = backupService->scanForBackupStatus();
    const QString statusColor = [status]() -> QString {
        switch (status) {
        case BackupStatus::Valid:
            return UIColors::k_BACKUP_STATUS_COLOR_FOUND;
        case BackupStatus::Broken:
            return UIColors::k_BACKUP_STATUS_COLOR_WARNING;
        default:
            return UIColors::k_BACKUP_STATUS_COLOR_NOT_FOUND;
        }
    }();
    updateBackupStatusLabel(statusColor);
    updateBackupLocationLabel(backupService->getBackupRoot());
    updateBackupTotalCountLabel();
    updateBackupTotalSizeLabel();
    updateBackupLocationStatusLabel(backupService->getBackupRoot());
    updateLastBackupInfo();
}

// Update last backup information
void MainWindow::updateLastBackupInfo() {
    const QJsonObject metadata = backupService->getLastBackupMetadata();
    if (metadata.isEmpty()) {
        for (auto* label : {ui->LastBackupNameLabel, ui->LastBackupTimestampLabel,
                            ui->LastBackupDurationLabel, ui->LastBackupSizeLabel}) {
            label->setText(Utilities::k_DEFAULT_VALUE_NOT_AVAILABLE);
        }
        return;
    }

    ui->LastBackupNameLabel->setText(Labels::LastBackup::k_NAME +
                                     metadata.value(Backup::MetadataKeys::k_NAME).toString());
    ui->LastBackupTimestampLabel->setText(Labels::LastBackup::k_TIMESTAMP +
                                          Utils::Formatting::formatTimestamp(
                                              QDateTime::fromString(metadata.value(Backup::MetadataKeys::k_TIMESTAMP).toString(), Qt::ISODate),
                                              Backup::Timestamps::k_BACKUP_TIMESTAMP_DISPLAY_FORMAT));
    ui->LastBackupDurationLabel->setText(Labels::LastBackup::k_DURATION +
                                         Utils::Formatting::formatDuration(metadata.value(Backup::MetadataKeys::k_DURATION).toInt()));
    ui->LastBackupSizeLabel->setText(Labels::LastBackup::k_SIZE +
                                     metadata.value(Backup::MetadataKeys::k_SIZE_READABLE).toString());
}

// Update backup status label
void MainWindow::updateBackupStatusLabel(const QString& statusColor) {
    const QPixmap pixmap = Utils::UI::createStatusLightPixmap(statusColor, ProgressUI::k_STATUS_LIGHT_SIZE);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, Labels::Backup::k_STATUS_LIGHT_IMAGE_FORMAT);

    const QString pixmapHtml = QString(HTMLTemplates::k_STATUS_LIGHT_ICON).arg(QString::fromUtf8(ba.toBase64()));
    const QString combinedHtml = QString(HTMLTemplates::k_STATUS_LABEL_HTML).arg(Labels::Backup::k_FOUND, pixmapHtml);
    ui->BackupStatusLabel->setTextFormat(Qt::RichText);
    ui->BackupStatusLabel->setText(combinedHtml);

    const bool backupExists = (statusColor == UIColors::k_BACKUP_STATUS_COLOR_FOUND);
    for (QLabel* label : {ui->LastBackupNameLabel, ui->LastBackupTimestampLabel,
                          ui->LastBackupDurationLabel, ui->LastBackupSizeLabel}) {
        label->setVisible(backupExists);
    }
}

// Update backup location label
void MainWindow::updateBackupLocationLabel(const QString& location) {
    ui->BackupLocationLabel->setText(Labels::Backup::k_LOCATION + location);
}

// Update backup total count label
void MainWindow::updateBackupTotalCountLabel() {
    ui->BackupTotalCountLabel->setText(Labels::Backup::k_TOTAL_COUNT +
                                       QString::number(backupService->getBackupCount()));
}

// Update backup total size label
void MainWindow::updateBackupTotalSizeLabel() {
    ui->BackupTotalSizeLabel->setText(Labels::Backup::k_TOTAL_SIZE +
                                      Utils::Formatting::formatSize(backupService->getTotalBackupSize()));
}

// Update backup location status label
void MainWindow::updateBackupLocationStatusLabel(const QString& location) {
    QFileInfo dirInfo(location);
    const QString status = dirInfo.exists()
                               ? (dirInfo.isWritable() ? Labels::DirectoryStatus::k_WRITABLE : Labels::DirectoryStatus::k_READ_ONLY)
                               : Labels::DirectoryStatus::k_UNKNOWN;
    ui->BackupLocationStatusLabel->setText(Labels::Backup::k_LOCATION_ACCESS + status);
}
