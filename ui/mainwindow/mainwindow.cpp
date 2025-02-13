#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../core/config/constants.h"

#include "../../core/backup_module/controller/backupcontroller.h"
#include "../../core/backup_module/service/backupservice.h"
#include "../../core/utils/file_utils/fileoperations.h"
#include "../../core/backup_module/models/stagingmodel.h"
#include "../../core/utils/common_utils/utils.h"
#include "../../core/utils/file_utils/filewatcher.h"

#include <QTreeView>
#include <QFileDialog>
#include <QPixmap>
#include <QPainter>
#include <QJsonObject>
#include <QBuffer>
#include <QMessageBox>
#include <QCloseEvent>
#include <QStringList>
#include <QFileSystemModel>
#include <QProgressBar>

// Constructor and Destructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    destinationModel(new QFileSystemModel(this)),
    sourceModel(new QFileSystemModel(this)),
    fileWatcher(new FileWatcher(this)),
    backupService(new BackupService(UserSettings::DEFAULT_BACKUP_DESTINATION)),
    stagingModel(new StagingModel(this)),
    backupController(new BackupController(backupService, this)) {

    ui->setupUi(this);

    // Configure progress bar
    Utils::UI::setupProgressBar(ui->TransferProgressBar,
                                ProgressConfig::MIN_VALUE,
                                ProgressConfig::MAX_VALUE,
                                UIConfig::PROGRESS_BAR_HEIGHT,
                                UIConfig::PROGRESS_BAR_TEXT_VISIBLE);
    ui->TransferProgressBar->setVisible(false);

    // Ensure backup directory exists
    if (!FileOperations::createDirectory(UserSettings::DEFAULT_BACKUP_DESTINATION)) {
        QMessageBox::critical(this, UIConfig::INITIALIZATION_ERROR_TITLE, UIConfig::INITIALIZATION_ERROR_MESSAGE);
    }

    // Connect backup signals
    connectBackupSignals();

    // Setup UI components
    setupDestinationView();
    setupSourceTreeView();
    setupBackupStagingTreeView();

    // Initialize backup and file watcher
    refreshBackupStatus();
    startWatchingBackupDirectory(UserSettings::DEFAULT_BACKUP_DESTINATION);
    updateFileWatcher();

    // Connect UI buttons to slots
    connect(ui->AddToBackupButton, &QPushButton::clicked, this, &MainWindow::onAddToBackupClicked);
    connect(ui->ChangeBackupDestinationButton, &QPushButton::clicked, this, &MainWindow::onChangeBackupDestinationClicked);
    connect(ui->RemoveFromBackupButton, &QPushButton::clicked, this, &MainWindow::onRemoveFromBackupClicked);
    connect(ui->CreateBackupButton, &QPushButton::clicked, this, &MainWindow::onCreateBackupClicked);
    connect(ui->DeleteBackupButton, &QPushButton::clicked, this, &MainWindow::onDeleteBackupClicked);
    connect(ui->AboutButton, &QPushButton::clicked, this, &MainWindow::onAboutButtonClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

// UI Setup
void MainWindow::setupDestinationView() {
    destinationModel->setFilter(BackupInfo::FILE_SYSTEM_FILTER);
    destinationModel->sort(0, Qt::DescendingOrder);
    ui->BackupDestinationView->setModel(destinationModel);
    ui->BackupDestinationView->setRootIndex(destinationModel->setRootPath(UserSettings::DEFAULT_BACKUP_DESTINATION));

    removeAllColumnsFromTreeView(ui->BackupDestinationView);
}

void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath(BackupInfo::DEFAULT_ROOT_PATH);
    sourceModel->setFilter(BackupInfo::FILE_SYSTEM_FILTER);
    ui->DriveTreeView->setModel(sourceModel);
    ui->DriveTreeView->setRootIndex(sourceModel->index(BackupInfo::DEFAULT_ROOT_PATH));
    ui->DriveTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->DriveTreeView);
}

void MainWindow::setupBackupStagingTreeView() {
    ui->BackupStagingTreeView->setModel(stagingModel);
    ui->BackupStagingTreeView->header()->setVisible(true);
    ui->BackupStagingTreeView->header()->setStretchLastSection(true);
    ui->BackupStagingTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->BackupStagingTreeView);
}

void MainWindow::removeAllColumnsFromTreeView(QTreeView *treeView) {
    if (!treeView) return;

    QAbstractItemModel *model = treeView->model();
    if (model) {
        for (int i = UIConfig::TREE_VIEW_START_HIDDEN_COLUMN;
             i < UIConfig::TREE_VIEW_DEFAULT_COLUMN_COUNT; ++i) {
            treeView->setColumnHidden(i, true);
        }
    }
}

// Backup Management
void MainWindow::onAddToBackupClicked() {
    QModelIndexList selectedIndexes = ui->DriveTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, UIConfig::NO_BACKUP_ITEMS_SELECTED_TITLE, UIConfig::ERROR_INVALID_SELECTION);
        return;
    }
    Utils::Backup::addSelectedPathsToStaging(ui->DriveTreeView, stagingModel);
}

void MainWindow::onRemoveFromBackupClicked() {
    QModelIndexList selectedIndexes = ui->BackupStagingTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, UIConfig::NO_FILES_SELECTED_TITLE, UIConfig::ERROR_INVALID_SELECTION);
        return;
    }
    Utils::Backup::removeSelectedPathsFromStaging(ui->BackupStagingTreeView, stagingModel);
}

void MainWindow::onChangeBackupDestinationClicked() {
    QString selectedDir = QFileDialog::getExistingDirectory(this,
                                                            UIConfig::SELECT_BACKUP_DESTINATION_TITLE,
                                                            BackupInfo::DEFAULT_FILE_DIALOG_ROOT);
    if (selectedDir.isEmpty()) {
        QMessageBox::warning(this, UIConfig::LOCATION_SELECTION_TITLE, UIConfig::ERROR_EMPTY_PATH_SELECTION);
        return;
    }

    if (!FileOperations::createDirectory(selectedDir)) {
        QMessageBox::critical(this, UIConfig::BACKUP_ERROR_TITLE, BackupInfo::ERROR_SET_BACKUP_DESTINATION);
        return;
    }

    // Update backup root in service
    backupService->setBackupRoot(selectedDir);

    // Reset and refresh the destination model
    destinationModel->setRootPath(selectedDir);
    ui->BackupDestinationView->setModel(destinationModel);
    ui->BackupDestinationView->setRootIndex(destinationModel->index(selectedDir));

    // Refresh UI & monitoring
    refreshBackupStatus();
    startWatchingBackupDirectory(selectedDir);
    updateFileWatcher();
}

void MainWindow::onCreateBackupClicked() {
    QStringList pathsToBackup = stagingModel->getStagedPaths();
    if (pathsToBackup.isEmpty()) {
        QMessageBox::warning(this, UIConfig::EMPTY_SELECTION_TITLE, UIConfig::EMPTY_STAGING_WARNING_MESSAGE);
        return;
    }

    QString backupRoot = destinationModel->rootPath();
    QString errorMessage;

    if (!FileOperations::createBackupInfrastructure(backupRoot, errorMessage)) {
        QMessageBox::critical(this, UIConfig::BACKUP_ERROR_TITLE, errorMessage);
        return;
    }

    ui->TransferProgressBar->setValue(ProgressConfig::MIN_VALUE);
    ui->TransferProgressBar->setVisible(true);

    backupController->createBackup(backupRoot, pathsToBackup, ui->TransferProgressBar);
}

void MainWindow::onDeleteBackupClicked() {
    QModelIndex selectedIndex = ui->BackupDestinationView->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, UIConfig::BACKUP_ERROR_TITLE, BackupInfo::ERROR_BACKUP_DELETION_FAILED);
        return;
    }

    QString selectedPath = destinationModel->filePath(selectedIndex);

    // Construct path to the corresponding backup log file
    QString logsFolderPath = QDir(backupService->getBackupRoot()).filePath(AppConfig::BACKUP_SETTINGS_FOLDER + "/" + AppConfig::BACKUP_LOGS_FOLDER);
    QString logFileName = QFileInfo(selectedPath).fileName() + AppConfig::BACKUP_LOG_SUFFIX;
    QString logFilePath = QDir(logsFolderPath).filePath(logFileName);

    // Check if backup log exists before allowing deletion
    if (!QFile::exists(logFilePath)) {
        QMessageBox::warning(this, BackupInfo::INVALID_BACKUP_TITLE, BackupInfo::INVALID_BACKUP_MESSAGE);
        return;
    }

    if (QMessageBox::question(this, UIConfig::DELETE_BACKUP_WARNING_TITLE,
                              QString(UIConfig::DELETE_BACKUP_WARNING_MESSAGE).arg(selectedPath),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        backupController->deleteBackup(selectedPath);
    }
}

void MainWindow::onAboutButtonClicked() {
    QMessageBox::information(this, AppConfig::ABOUT_DIALOG_TITLE, QString(AppConfig::ABOUT_DIALOG_MESSAGE).arg(AppConfig::APP_VERSION, AppConfig::WINDOW_TITLE));
}

// File and Directory Monitoring
void MainWindow::startWatchingBackupDirectory(const QString &path) {
    fileWatcher->startWatching(path);

    // Also watch _backup_settings for changes
    QString settingsFolderPath = QDir(path).filePath(AppConfig::BACKUP_SETTINGS_FOLDER);
    fileWatcher->addPath(settingsFolderPath);

    connect(fileWatcher, &FileWatcher::directoryChanged, this, &MainWindow::onBackupDirectoryChanged);
}


void MainWindow::updateFileWatcher() {
    fileWatcher->startWatching(destinationModel->rootPath());
}

void MainWindow::onFileChanged(const QString &path) {
    Q_UNUSED(path);
    refreshBackupStatus();
}

void MainWindow::onBackupDirectoryChanged() {
    updateFileWatcher();
    refreshBackupStatus(); // Ensures UI updates when backup_logs/settings.json changes
}

// Status Updates
void MainWindow::updateBackupStatusLabel(const QString &statusColor) {
    // Create a status light with the specified color
    QPixmap pixmap = Utils::UI::createStatusLightPixmap(statusColor, UIConfig::STATUS_LIGHT_SIZE);

    // Convert the pixmap into a base64-encoded image string for HTML formatting
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    // Generate HTML to embed the status light icon
    QString pixmapHtml = QString(UIConfig::STATUS_LIGHT_ICON_TEMPLATE)
                             .arg(QString::fromUtf8(ba.toBase64()),
                                  QString(UIConfig::ICON_STYLE_TEMPLATE)
                                      .arg(UIConfig::STATUS_LIGHT_SIZE));

    // Combine status label and icon for display
    QString combinedHtml = QString(UIConfig::STATUS_LABEL_HTML_TEMPLATE)
                               .arg(UIConfig::LABEL_BACKUP_FOUND, pixmapHtml);


    // Set the updated status label in the UI
    ui->BackupStatusLabel->setTextFormat(Qt::RichText);
    ui->BackupStatusLabel->setText(combinedHtml);

    // Control visibility of backup detail labels based on backup status
    bool backupExists = (statusColor == UIConfig::BACKUP_STATUS_COLOR_FOUND);
    ui->LastBackupNameLabel->setVisible(backupExists);
    ui->LastBackupTimestampLabel->setVisible(backupExists);
    ui->LastBackupDurationLabel->setVisible(backupExists);
    ui->LastBackupSizeLabel->setVisible(backupExists);
}

void MainWindow::updateBackupLocationLabel(const QString &location) {
    ui->BackupLocationLabel->setText(UIConfig::LABEL_BACKUP_LOCATION + location);
}

void MainWindow::updateBackupTotalCountLabel() {
    ui->BackupTotalCountLabel->setText(UIConfig::LABEL_BACKUP_TOTAL_COUNT +
                                       QString::number(backupService->getBackupCount()));
}

void MainWindow::updateBackupTotalSizeLabel() {
    quint64 totalSize = backupService->getTotalBackupSize();
    QString humanReadableSize = Utils::Formatting::formatSize(totalSize);
    ui->BackupTotalSizeLabel->setText(UIConfig::LABEL_BACKUP_TOTAL_SIZE + humanReadableSize);
}

void MainWindow::updateBackupLocationStatusLabel(const QString &location) {
    QFileInfo dirInfo(location);
    QString status = dirInfo.exists() ? (dirInfo.isWritable() ? UIConfig::DIRECTORY_STATUS_WRITABLE
                                                              : UIConfig::DIRECTORY_STATUS_READ_ONLY)
                                      : UIConfig::DIRECTORY_STATUS_UNKNOWN;

    ui->BackupLocationStatusLabel->setText(UIConfig::LABEL_BACKUP_LOCATION_ACCESS + status);
}

// Refresh the backup status
void MainWindow::refreshBackupStatus() {
    BackupStatus status = backupService->scanForBackupStatus();

    switch (status) {
    case BackupStatus::Valid:
        updateBackupStatusLabel(UIConfig::BACKUP_STATUS_COLOR_FOUND);
        break;
    case BackupStatus::Broken:
        updateBackupStatusLabel(UIConfig::BACKUP_STATUS_COLOR_WARNING);
        break;
    case BackupStatus::None:
    default:
        updateBackupStatusLabel(UIConfig::BACKUP_STATUS_COLOR_NOT_FOUND);
        break;
    }

    updateBackupLocationLabel(backupService->getBackupRoot());
    updateBackupTotalCountLabel();
    updateBackupTotalSizeLabel();
    updateBackupLocationStatusLabel(backupService->getBackupRoot());
    updateLastBackupInfo();
}

// Update information about the last backup
void MainWindow::updateLastBackupInfo() {
    QJsonObject metadata = backupService->getLastBackupMetadata();
    if (metadata.isEmpty()) {
        QString notAvailable = UIConfig::DEFAULT_VALUE_NOT_AVAILABLE;
        ui->LastBackupNameLabel->setText(UIConfig::LABEL_LAST_BACKUP_NAME + notAvailable);
        ui->LastBackupTimestampLabel->setText(UIConfig::LABEL_LAST_BACKUP_TIMESTAMP + notAvailable);
        ui->LastBackupDurationLabel->setText(UIConfig::LABEL_LAST_BACKUP_DURATION + notAvailable);
        ui->LastBackupSizeLabel->setText(UIConfig::LABEL_LAST_BACKUP_SIZE + notAvailable);
        return;
    }

    QString backupName = metadata.value(BackupMetadataKeys::NAME).toString(UIConfig::DEFAULT_VALUE_NOT_AVAILABLE);
    QString timestampStr = metadata.value(BackupMetadataKeys::TIMESTAMP).toString();
    int durationMs = metadata.value(BackupMetadataKeys::DURATION).toInt(0);
    QString totalSize = metadata.value(BackupMetadataKeys::SIZE_READABLE).toString(UIConfig::DEFAULT_VALUE_NOT_AVAILABLE);

    QDateTime backupTimestamp = QDateTime::fromString(timestampStr, Qt::ISODate);
    QString formattedTimestamp = Utils::Formatting::formatTimestamp(backupTimestamp, BackupInfo::BACKUP_TIMESTAMP_DISPLAY_FORMAT);
    QString formattedDuration = Utils::Formatting::formatDuration(durationMs);

    ui->LastBackupNameLabel->setText(UIConfig::LABEL_LAST_BACKUP_NAME + backupName);
    ui->LastBackupTimestampLabel->setText(UIConfig::LABEL_LAST_BACKUP_TIMESTAMP + formattedTimestamp);
    ui->LastBackupDurationLabel->setText(UIConfig::LABEL_LAST_BACKUP_DURATION + formattedDuration);
    ui->LastBackupSizeLabel->setText(UIConfig::LABEL_LAST_BACKUP_SIZE + totalSize);
}

// Event Handlers
void MainWindow::closeEvent(QCloseEvent *event) {
    if (backupController->isBackupInProgress()) {
        QMessageBox::warning(this,
                             UIConfig::MESSAGE_OPERATION_IN_PROGRESS,
                             UIConfig::MESSAGE_OPERATION_WARNING);
        event->ignore();
        return;
    }
    QMainWindow::closeEvent(event);
}

// Backup Signal Connections
void MainWindow::connectBackupSignals() {
    connect(backupController, &BackupController::backupCreated, this, &MainWindow::refreshBackupStatus);
    connect(backupController, &BackupController::backupDeleted, this, &MainWindow::refreshBackupStatus);
    connect(backupController, &BackupController::errorOccurred, this, [this](const QString &error) {
        QMessageBox::critical(this, UIConfig::BACKUP_ERROR_TITLE, error);
    });
}
