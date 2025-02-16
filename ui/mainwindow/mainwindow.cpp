#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../core/config/_constants.h"

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
    backupService(new BackupService(UserConfig::DEFAULT_BACKUP_DIRECTORY)),
    stagingModel(new StagingModel(this)),
    backupController(new BackupController(backupService, this)) {

    ui->setupUi(this);

    // Configure progress bar
    Utils::UI::setupProgressBar(ui->TransferProgressBar,
                                ProgressConfig::PROGRESS_BAR_MIN_VALUE,
                                ProgressConfig::PROGRESS_BAR_MAX_VALUE,
                                ProgressConfig::PROGRESS_BAR_HEIGHT,
                                ProgressConfig::PROGRESS_BAR_TEXT_VISIBLE);
    ui->TransferProgressBar->setVisible(false);

    // Ensure backup directory exists
    if (!FileOperations::createDirectory(UserConfig::DEFAULT_BACKUP_DIRECTORY)) {
        QMessageBox::critical(this, ErrorMessages::BACKUP_INITIALIZATION_FAILED_TITLE, ErrorMessages::ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY);
    }

    // Connect backup signals
    connectBackupSignals();

    // Setup UI components
    setupDestinationView();
    setupSourceTreeView();
    setupBackupStagingTreeView();

    // Initialize backup and file watcher
    refreshBackupStatus();
    startWatchingBackupDirectory(UserConfig::DEFAULT_BACKUP_DIRECTORY);
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
    ui->BackupDestinationView->setRootIndex(destinationModel->setRootPath(UserConfig::DEFAULT_BACKUP_DIRECTORY));

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
        QMessageBox::warning(this, ErrorMessages::BACKUP_SELECTION_REQUIRED_TITLE, ErrorMessages::ERROR_NO_ITEMS_SELECTED_FOR_BACKUP);
        return;
    }
    Utils::Backup::addSelectedPathsToStaging(ui->DriveTreeView, stagingModel);
}

void MainWindow::onRemoveFromBackupClicked() {
    QModelIndexList selectedIndexes = ui->BackupStagingTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::REMOVE_SELECTION_REQUIRED_TITLE, ErrorMessages::ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL);
        return;
    }
    Utils::Backup::removeSelectedPathsFromStaging(ui->BackupStagingTreeView, stagingModel);
}

void MainWindow::onChangeBackupDestinationClicked() {
    QString selectedDir = QFileDialog::getExistingDirectory(this,
                                                            InfoMessages::SELECT_BACKUP_DESTINATION_TITLE,
                                                            BackupInfo::DEFAULT_FILE_DIALOG_ROOT);
    if (selectedDir.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::BACKUP_LOCATION_REQUIRED_TITLE, ErrorMessages::ERROR_NO_BACKUP_LOCATION_PATH_SELECTED);
        return;
    }

    if (!FileOperations::createDirectory(selectedDir)) {
        QMessageBox::critical(this, ErrorMessages::BACKUP_DIRECTORY_ERROR_TITLE, ErrorMessages::ERROR_CREATING_BACKUP_DIRECTORY);
        return;
    }

    backupService->setBackupRoot(selectedDir);

    destinationModel->setRootPath(selectedDir);
    ui->BackupDestinationView->setModel(destinationModel);
    ui->BackupDestinationView->setRootIndex(destinationModel->index(selectedDir));

    refreshBackupStatus();
    startWatchingBackupDirectory(selectedDir);
    updateFileWatcher();
}

void MainWindow::onCreateBackupClicked() {
    QStringList pathsToBackup = stagingModel->getStagedPaths();
    if (pathsToBackup.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::NO_ITEMS_STAGED_FOR_BACKUP_TITLE, ErrorMessages::ERROR_NO_ITEMS_STAGED_FOR_BACKUP);
        return;
    }

    QString backupRoot = destinationModel->rootPath();
    QString errorMessage;

    if (!FileOperations::createBackupInfrastructure(backupRoot, errorMessage)) {
        QMessageBox::critical(this, ErrorMessages::ERROR_BACKUP_ALREADY_IN_PROGRESS, errorMessage);
        return;
    }

    ui->TransferProgressBar->setValue(ProgressConfig::PROGRESS_BAR_MIN_VALUE);
    ui->TransferProgressBar->setVisible(true);

    backupController->createBackup(backupRoot, pathsToBackup, ui->TransferProgressBar);
}

void MainWindow::onDeleteBackupClicked() {
    QModelIndex selectedIndex = ui->BackupDestinationView->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, ErrorMessages::BACKUP_DELETION_ERROR_TITLE, ErrorMessages::ERROR_BACKUP_DELETE_FAILED);
        return;
    }

    QString selectedPath = destinationModel->filePath(selectedIndex);

    QString logsFolderPath = QDir(backupService->getBackupRoot()).filePath(AppConfig::BACKUP_CONFIG_FOLDER + "/" + AppConfig::BACKUP_LOGS_DIRECTORY);
    QString logFileName = QFileInfo(selectedPath).fileName() + AppConfig::BACKUP_LOG_FILE_SUFFIX;
    QString logFilePath = QDir(logsFolderPath).filePath(logFileName);

    if (!QFile::exists(logFilePath)) {
        QMessageBox::warning(this, ErrorMessages::BACKUP_VALIDATION_FAILED_TITLE, ErrorMessages::ERROR_INVALID_BACKUP_DIRECTORY);
        return;
    }

    if (QMessageBox::question(this, WarningMessages::WARNING_CONFIRM_BACKUP_DELETION,
                              QString(WarningMessages::MESSAGE_CONFIRM_BACKUP_DELETION).arg(selectedPath),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        backupController->deleteBackup(selectedPath);
    }
}

void MainWindow::onAboutButtonClicked() {
    QMessageBox::information(this, AboutInfo::ABOUT_WINDOW_TITLE, QString(AboutInfo::ABOUT_WINDOW_MESSAGE).arg(AppInfo::APP_VERSION, AppInfo::APP_DISPLAY_TITLE));
}

// File and Directory Monitoring
void MainWindow::startWatchingBackupDirectory(const QString &path) {
    fileWatcher->startWatching(path);

    QString settingsFolderPath = QDir(path).filePath(AppConfig::BACKUP_CONFIG_FOLDER);
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
    refreshBackupStatus();
}

// Status Updates
void MainWindow::updateBackupStatusLabel(const QString &statusColor) {

    QPixmap pixmap = Utils::UI::createStatusLightPixmap(statusColor, ProgressConfig::STATUS_LIGHT_SIZE);

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    QString pixmapHtml = QString(Styling::STATUS_LIGHT_ICON_TEMPLATE)
                             .arg(QString::fromUtf8(ba.toBase64()),
                                  QString(Styling::ICON_STYLE_TEMPLATE)
                                      .arg(ProgressConfig::STATUS_LIGHT_SIZE));

    QString combinedHtml = QString(Styling::STATUS_LABEL_HTML_TEMPLATE)
                               .arg(UIConfig::LABEL_BACKUP_FOUND, pixmapHtml);


    ui->BackupStatusLabel->setTextFormat(Qt::RichText);
    ui->BackupStatusLabel->setText(combinedHtml);

    bool backupExists = (statusColor == Colors::BACKUP_STATUS_COLOR_FOUND);
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
        updateBackupStatusLabel(Colors::BACKUP_STATUS_COLOR_FOUND);
        break;
    case BackupStatus::Broken:
        updateBackupStatusLabel(Colors::BACKUP_STATUS_COLOR_WARNING);
        break;
    case BackupStatus::None:
    default:
        updateBackupStatusLabel(Colors::BACKUP_STATUS_COLOR_NOT_FOUND);
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
        QString notAvailable = Utilities::DEFAULT_VALUE_NOT_AVAILABLE;
        ui->LastBackupNameLabel->setText(UIConfig::LABEL_LAST_BACKUP_NAME + notAvailable);
        ui->LastBackupTimestampLabel->setText(UIConfig::LABEL_LAST_BACKUP_TIMESTAMP + notAvailable);
        ui->LastBackupDurationLabel->setText(UIConfig::LABEL_LAST_BACKUP_DURATION + notAvailable);
        ui->LastBackupSizeLabel->setText(UIConfig::LABEL_LAST_BACKUP_SIZE + notAvailable);
        return;
    }

    QString backupName = metadata.value(BackupMetadataKeys::NAME).toString(Utilities::DEFAULT_VALUE_NOT_AVAILABLE);
    QString timestampStr = metadata.value(BackupMetadataKeys::TIMESTAMP).toString();
    int durationMs = metadata.value(BackupMetadataKeys::DURATION).toInt(0);
    QString totalSize = metadata.value(BackupMetadataKeys::SIZE_READABLE).toString(Utilities::DEFAULT_VALUE_NOT_AVAILABLE);

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
                             ErrorMessages::ERROR_OPERATION_IN_PROGRESS,
                             ErrorMessages::WARNING_OPERATION_STILL_RUNNING);
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
        QMessageBox::critical(this, ErrorMessages::BACKUP_DELETION_ERROR_TITLE, error);
    });
}
