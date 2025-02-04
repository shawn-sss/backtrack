#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../BackupModule/fileoperations.h"
#include "../Utils/constants.h"
#include "../Utils/utils.h"
#include "../Utils/filewatcher.h"
#include "../BackupModule/stagingmodel.h"
#include "../BackupModule/backupcontroller.h"
#include "../BackupModule/backupservice.h"

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
// --------------------------

// Constructor to initialize the MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    destinationModel(new QFileSystemModel(this)),
    sourceModel(new QFileSystemModel(this)),
    fileWatcher(new FileWatcher(this)),
    backupService(new BackupService(Constants::DEFAULT_BACKUP_DESTINATION)),
    stagingModel(new StagingModel(this)),
    backupController(new BackupController(backupService, this)) {
    ui->setupUi(this);

    // Configure progress bar
    Utils::UI::setupProgressBar(ui->TransferProgressBar,
                                Constants::PROGRESS_BAR_MIN_VALUE,
                                Constants::PROGRESS_BAR_MAX_VALUE,
                                Constants::PROGRESS_BAR_HEIGHT,
                                Constants::PROGRESS_BAR_TEXT_VISIBLE);
    ui->TransferProgressBar->setVisible(false);

    // Ensure the backup directory exists
    if (!FileOperations::createDirectory(Constants::DEFAULT_BACKUP_DESTINATION)) {
        QMessageBox::critical(this, Constants::INITIALIZATION_ERROR_TITLE, Constants::INITIALIZATION_ERROR_MESSAGE);
    }

    // Connect backup signals
    connect(backupController, &BackupController::backupCreated, this, [this]() {
        QMessageBox::information(this, Constants::SUCCESS_BACKUP_CREATED, Constants::BACKUP_COMPLETE_MESSAGE);
        refreshBackupStatus();
    });
    connect(backupController, &BackupController::backupDeleted, this, [this]() {
        QMessageBox::information(this, Constants::SUCCESS_BACKUP_DELETED, Constants::BACKUP_DELETE_SUCCESS);
        refreshBackupStatus();
    });
    connect(backupController, &BackupController::errorOccurred, this, [this](const QString &error) {
        QMessageBox::critical(this, Constants::BACKUP_ERROR_TITLE, error);
    });

    // Setup UI components
    setupDestinationView();
    setupSourceTreeView();
    setupBackupStagingTreeView();

    // Initialize backup and file watcher
    refreshBackupStatus();
    startWatchingBackupDirectory(Constants::DEFAULT_BACKUP_DESTINATION);
    updateFileWatcher();

    // Connect UI buttons to slots
    connect(ui->AddToBackupButton, &QPushButton::clicked, this, &MainWindow::onAddToBackupClicked);
    connect(ui->ChangeBackupDestinationButton, &QPushButton::clicked, this, &MainWindow::onChangeBackupDestinationClicked);
    connect(ui->RemoveFromBackupButton, &QPushButton::clicked, this, &MainWindow::onRemoveFromBackupClicked);
    connect(ui->CreateBackupButton, &QPushButton::clicked, this, &MainWindow::onCreateBackupClicked);
    connect(ui->DeleteBackupButton, &QPushButton::clicked, this, &MainWindow::onDeleteBackupClicked);
    connect(ui->AboutButton, &QPushButton::clicked, this, &MainWindow::onAboutButtonClicked);
}

// Destructor to clean up resources
MainWindow::~MainWindow() {
    delete ui;
}

// UI Setup
// --------

// Setup the destination view
void MainWindow::setupDestinationView() {
    destinationModel->setFilter(Constants::FILE_SYSTEM_FILTER);
    destinationModel->sort(0, Qt::DescendingOrder);
    ui->BackupDestinationView->setModel(destinationModel);

    QModelIndex rootIndex = destinationModel->setRootPath(Constants::DEFAULT_BACKUP_DESTINATION);
    ui->BackupDestinationView->setRootIndex(rootIndex);

    removeAllColumnsFromTreeView(ui->BackupDestinationView);
}

// Setup the source tree view
void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath(Constants::DEFAULT_ROOT_PATH);
    sourceModel->setFilter(Constants::FILE_SYSTEM_FILTER);
    ui->DriveTreeView->setModel(sourceModel);
    ui->DriveTreeView->setRootIndex(sourceModel->index(Constants::DEFAULT_ROOT_PATH));

    ui->DriveTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->DriveTreeView);
}

// Setup the backup staging tree view
void MainWindow::setupBackupStagingTreeView() {
    ui->BackupStagingTreeView->setModel(stagingModel);
    ui->BackupStagingTreeView->header()->setVisible(true);
    ui->BackupStagingTreeView->header()->setStretchLastSection(true);

    ui->BackupStagingTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->BackupStagingTreeView);
}

// Setup the backup staging tree view
void MainWindow::removeAllColumnsFromTreeView(QTreeView *treeView) {
    if (!treeView) return;

    QAbstractItemModel *model = treeView->model();
    if (model) {
        for (int i = Constants::TREE_VIEW_START_HIDDEN_COLUMN;
             i < Constants::TREE_VIEW_DEFAULT_COLUMN_COUNT; ++i) {
            treeView->setColumnHidden(i, true);
        }
    }
}

// Backup Management
// ------------------

// Add selected items to the backup staging area
void MainWindow::onAddToBackupClicked() {
    QModelIndexList selectedIndexes = ui->DriveTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, Constants::NO_BACKUP_ITEMS_SELECTED_TITLE, Constants::ERROR_INVALID_SELECTION);
        return;
    }
    Utils::Backup::addSelectedPathsToStaging(ui->DriveTreeView, stagingModel);
}

// Remove selected items from the backup staging area
void MainWindow::onRemoveFromBackupClicked() {
    QModelIndexList selectedIndexes = ui->BackupStagingTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, Constants::NO_FILES_SELECTED_TITLE, Constants::ERROR_INVALID_SELECTION);
        return;
    }
    Utils::Backup::removeSelectedPathsFromStaging(ui->BackupStagingTreeView, stagingModel);
}

// Change the backup destination directory
void MainWindow::onChangeBackupDestinationClicked() {
    QString selectedDir = QFileDialog::getExistingDirectory(this,
                                                            Constants::SELECT_BACKUP_DESTINATION_TITLE,
                                                            Constants::DEFAULT_FILE_DIALOG_ROOT);
    if (selectedDir.isEmpty()) {
        QMessageBox::warning(this, Constants::LOCATION_SELECTION_TITLE, Constants::ERROR_EMPTY_PATH_SELECTION);
        return;
    }

    if (!FileOperations::createDirectory(selectedDir)) {
        QMessageBox::critical(this, Constants::BACKUP_ERROR_TITLE, Constants::ERROR_SET_BACKUP_DESTINATION);
        return;
    }

    QModelIndex rootIndex = destinationModel->setRootPath(selectedDir);
    ui->BackupDestinationView->setRootIndex(rootIndex);

    backupService->setBackupRoot(selectedDir);
    refreshBackupStatus();
    startWatchingBackupDirectory(selectedDir);
    updateFileWatcher();
}

// Create a backup using the selected paths
void MainWindow::onCreateBackupClicked() {
    QStringList pathsToBackup = stagingModel->getStagedPaths();
    if (pathsToBackup.isEmpty()) {
        QMessageBox::warning(this, Constants::EMPTY_SELECTION_TITLE, Constants::EMPTY_STAGING_WARNING_MESSAGE);
        return;
    }

    ui->TransferProgressBar->setValue(Constants::PROGRESS_BAR_MIN_VALUE);
    ui->TransferProgressBar->setVisible(true);

    backupController->createBackup(destinationModel->rootPath(), pathsToBackup, ui->TransferProgressBar);
}

// Delete the selected backup
void MainWindow::onDeleteBackupClicked() {
    QModelIndex selectedIndex = ui->BackupDestinationView->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, Constants::BACKUP_ERROR_TITLE, Constants::ERROR_BACKUP_DELETION_FAILED);
        return;
    }

    QString selectedPath = destinationModel->filePath(selectedIndex);
    QDir backupDir(selectedPath);
    QString summaryFilePath = backupDir.filePath(Constants::BACKUP_SUMMARY_FILENAME);

    if (!QFile::exists(summaryFilePath)) {
        QMessageBox::warning(this, Constants::INVALID_BACKUP_TITLE, Constants::INVALID_BACKUP_MESSAGE);
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        Constants::DELETE_BACKUP_WARNING_TITLE,
        QString(Constants::DELETE_BACKUP_WARNING_MESSAGE).arg(selectedPath),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        backupController->deleteBackup(selectedPath);
    }
}

// Display the about dialog
void MainWindow::onAboutButtonClicked() {
    QMessageBox::information(
        this,
        Constants::ABOUT_DIALOG_TITLE,
        QString(Constants::ABOUT_DIALOG_MESSAGE)
            .arg(Constants::APP_VERSION, Constants::WINDOW_TITLE)
        );
}

// File and Directory Monitoring
// -----------------------------

// Start watching the backup directory
void MainWindow::startWatchingBackupDirectory(const QString &path) {
    fileWatcher->startWatching(path);
    connect(fileWatcher, &FileWatcher::directoryChanged, this, &MainWindow::onBackupDirectoryChanged);
}

// Update the file watcher to refresh watched paths
void MainWindow::updateFileWatcher() {
    fileWatcher->startWatching(destinationModel->rootPath());
}

// Handle file change events
void MainWindow::onFileChanged(const QString &path) {
    Q_UNUSED(path);
    refreshBackupStatus();
}

// Handle backup directory change events
void MainWindow::onBackupDirectoryChanged() {
    updateFileWatcher();
    refreshBackupStatus();
}

// Status Updates
// ---------------

// Update the backup status label
void MainWindow::updateBackupStatusLabel(bool backupFound) {
    QString color = backupFound
                        ? Constants::BACKUP_STATUS_COLOR_FOUND
                        : Constants::BACKUP_STATUS_COLOR_NOT_FOUND;

    QPixmap pixmap = Utils::UI::createStatusLightPixmap(color, Constants::STATUS_LIGHT_SIZE);

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    QString pixmapHtml = QStringLiteral("<img src='data:image/png;base64,%1' style='%2'>")
                             .arg(QString::fromUtf8(ba.toBase64()),
                                  QString(Constants::ICON_STYLE_TEMPLATE)
                                      .arg(Constants::STATUS_LIGHT_SIZE));

    QString combinedHtml = QStringLiteral(
                               "<div style='display:flex; align-items:center;'>"
                               "<span>%1</span><span style='margin-left:4px;'>%2</span>"
                               "</div>")
                               .arg(Constants::LABEL_BACKUP_FOUND, pixmapHtml);

    ui->BackupStatusLabel->setTextFormat(Qt::RichText);
    ui->BackupStatusLabel->setText(combinedHtml);

    ui->LastBackupNameLabel->setVisible(backupFound);
    ui->LastBackupTimestampLabel->setVisible(backupFound);
    ui->LastBackupDurationLabel->setVisible(backupFound);
    ui->LastBackupSizeLabel->setVisible(backupFound);
}

// Update the backup location label
void MainWindow::updateBackupLocationLabel(const QString &location) {
    ui->BackupLocationLabel->setText(Constants::LABEL_BACKUP_LOCATION + location);
}

// Update the backup total count label
void MainWindow::updateBackupTotalCountLabel() {
    ui->BackupTotalCountLabel->setText(Constants::LABEL_BACKUP_TOTAL_COUNT +
                                       QString::number(backupService->getBackupCount()));
}

// Update the backup total size label
void MainWindow::updateBackupTotalSizeLabel() {
    quint64 totalSize = backupService->getTotalBackupSize();
    QString humanReadableSize = Utils::Formatting::formatSize(totalSize);
    ui->BackupTotalSizeLabel->setText(Constants::LABEL_BACKUP_TOTAL_SIZE + humanReadableSize);
}

// Update the backup location status label
void MainWindow::updateBackupLocationStatusLabel(const QString &location) {
    QFileInfo dirInfo(location);
    QString status = dirInfo.exists() ? (dirInfo.isWritable() ? Constants::DIRECTORY_STATUS_WRITABLE
                                                              : Constants::DIRECTORY_STATUS_READ_ONLY)
                                      : Constants::DIRECTORY_STATUS_UNKNOWN;

    ui->BackupLocationStatusLabel->setText(Constants::LABEL_BACKUP_LOCATION_ACCESS + status);
}

// Refresh the backup status
void MainWindow::refreshBackupStatus() {
    bool backupFound = backupService->scanForBackupSummary();
    updateBackupStatusLabel(backupFound);
    updateBackupLocationLabel(Constants::DEFAULT_BACKUP_DESTINATION);
    updateBackupTotalCountLabel();
    updateBackupTotalSizeLabel();
    updateBackupLocationStatusLabel(Constants::DEFAULT_BACKUP_DESTINATION);
    updateLastBackupInfo();
}

// Update information about the last backup
void MainWindow::updateLastBackupInfo() {
    QJsonObject metadata = backupService->getLastBackupMetadata();
    if (metadata.isEmpty()) {
        ui->LastBackupNameLabel->setText(Constants::LABEL_LAST_BACKUP_NAME + Constants::DEFAULT_VALUE_NOT_AVAILABLE);
        ui->LastBackupTimestampLabel->setText(Constants::LABEL_LAST_BACKUP_TIMESTAMP + Constants::DEFAULT_VALUE_NOT_AVAILABLE);
        ui->LastBackupDurationLabel->setText(Constants::LABEL_LAST_BACKUP_DURATION + Constants::DEFAULT_VALUE_NOT_AVAILABLE);
        ui->LastBackupSizeLabel->setText(Constants::LABEL_LAST_BACKUP_SIZE + Constants::DEFAULT_VALUE_NOT_AVAILABLE);
        return;
    }

    ui->LastBackupNameLabel->setText(Constants::LABEL_LAST_BACKUP_NAME +
                                     metadata.value("backup_name").toString(Constants::DEFAULT_VALUE_NOT_AVAILABLE));

    QDateTime backupTimestamp = QDateTime::fromString(metadata.value("backup_timestamp").toString(), Qt::ISODate);
    ui->LastBackupTimestampLabel->setText(Constants::LABEL_LAST_BACKUP_TIMESTAMP +
                                          Utils::Formatting::formatTimestamp(backupTimestamp, Constants::BACKUP_TIMESTAMP_DISPLAY_FORMAT));

    ui->LastBackupDurationLabel->setText(Constants::LABEL_LAST_BACKUP_DURATION +
                                         Utils::Formatting::formatDuration(metadata.value("backup_duration").toInt(0)));

    ui->LastBackupSizeLabel->setText(Constants::LABEL_LAST_BACKUP_SIZE +
                                     metadata.value("total_size_readable").toString(Constants::DEFAULT_VALUE_NOT_AVAILABLE));
}

// Event Handlers
// --------------

// Handle the close event for the MainWindow
void MainWindow::closeEvent(QCloseEvent *event) {
    if (backupController && backupController->isBackupInProgress()) {
        QMessageBox::warning(this,
                             Constants::MESSAGE_OPERATION_IN_PROGRESS,
                             Constants::MESSAGE_OPERATION_WARNING);
        event->ignore();
        return;
    }
    QMainWindow::closeEvent(event);
}
