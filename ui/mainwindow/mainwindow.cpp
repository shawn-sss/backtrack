// Project includes same directory
#include "mainwindow.h"
#include "ui_mainwindow.h"

// Project includes different directory
#include "../../config/_constants.h"
#include "../../config/configmanager/configmanager.h"
#include "../../ui/toolbarmanager/toolbarmanager.h"
#include "../../core/backup_module/controller/backupcontroller.h"
#include "../../core/backup_module/service/backupservice.h"
#include "../../core/utils/file_utils/filewatcher.h"
#include "../../core/utils/file_utils/fileoperations.h"
#include "../../core/utils/common_utils/utils.h"
#include "../../core/backup_module/styles/backup_styling.h"

// Built-in Qt includes
#include <QFileSystemModel>
#include <QFileDialog>
#include <QBuffer>
#include <QMessageBox>
#include <QTimer>

// Constructor - Initializes main window components
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    sourceModel(new QFileSystemModel(this)),
    destinationModel(new QFileSystemModel(this)),
    fileWatcher(new FileWatcher(this)),
    backupService(new BackupService(ConfigManager::getInstance().getBackupDirectory())),
    stagingModel(new StagingModel(this)),
    backupController(new BackupController(backupService, this)),
    toolBar(new QToolBar(this)),
    toolbarManager(new ToolbarManager(this)) {

    ui->setupUi(this);

    configureWindow();
    initializeUI();
    setupLayout();

    addToolBar(Qt::LeftToolBarArea, toolBar);
    toolbarManager->initialize(toolBar);

    applyButtonCursors();
    initializeBackupSystem();
    setupConnections();

    ui->BackupViewContainer->setStyleSheet(Styles::BackupViewContainer::STYLE);
    ui->BackupViewLayout->setContentsMargins(0, 0, 0, 0);
    ui->BackupViewLayout->setSpacing(3);
    ui->BackupViewLayout->setStretch(0, 1);
    ui->BackupViewLayout->setStretch(1, 1);
    ui->BackupViewLayout->setStretch(2, 1);
}

// Destructor - Cleans up allocated resources
MainWindow::~MainWindow() {
    delete ui;
}

// ## Window and Layout Configuration ##

// Configure basic window properties
void MainWindow::configureWindow()
{
    setMinimumSize(AppConfig::kMinimumWindowSize);
    resize(AppConfig::kDefaultWindowSize);
    setMaximumSize(AppConfig::kMaximumWindowSize);

    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        const QRect screenGeometry = screen->availableGeometry();
        const QPoint center = screenGeometry.center() - QPoint(AppConfig::kDefaultWindowSize.width() / 2,
                                                               AppConfig::kDefaultWindowSize.height() / 2);
        move(center);
    }
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

// Applies pointing hand cursor to buttons
void MainWindow::applyButtonCursors() {
    const QList<QPushButton*> buttons = {
        ui->AddToBackupButton,
        ui->RemoveFromBackupButton,
        ui->CreateBackupButton,
        ui->ChangeBackupDestinationButton,
        ui->DeleteBackupButton
    };

    for (auto *button : buttons) {
        button->setCursor(Qt::PointingHandCursor);
    }
}

// ## UI Initialization and Setup ##

// Initializes the UI
void MainWindow::initializeUI() {
    Utils::UI::setupProgressBar(
        ui->TransferProgressBar,
        ProgressSettings::PROGRESS_BAR_MIN_VALUE,
        ProgressSettings::PROGRESS_BAR_MAX_VALUE,
        ProgressSettings::PROGRESS_BAR_HEIGHT,
        ProgressSettings::PROGRESS_BAR_TEXT_VISIBLE
        );
    if (ui->TransferProgressBar->value() == 0) {
        ui->TransferProgressBar->setVisible(false);
        ui->TransferProgressText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TransferProgressText->setText(ProgressSettings::PROGRESS_BAR_INITIAL_MESSAGE);
    }
}

// Initializes the backup system
void MainWindow::initializeBackupSystem() {
    const QString backupDirectory = ConfigManager::getInstance().getBackupDirectory();
    if (!FileOperations::createDirectory(backupDirectory)) {
        QMessageBox::critical(this, ErrorMessages::BACKUP_INITIALIZATION_FAILED_TITLE,
                              ErrorMessages::ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY);
    }
    setupDestinationView();
    setupSourceTreeView();
    setupBackupStagingTreeView();
    refreshBackupStatus();
    if (!fileWatcher->watchedDirectories().contains(backupDirectory)) {
        startWatchingBackupDirectory(backupDirectory);
    }
}

// Connects signals and slots
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

}

// Connects backup-related signals
void MainWindow::connectBackupSignals() {
    connect(backupController, &BackupController::backupCreated, this, &MainWindow::refreshBackupStatus);
    connect(backupController, &BackupController::backupCreated, this, &MainWindow::onBackupCompleted);

    connect(backupController, &BackupController::backupDeleted, this, &MainWindow::refreshBackupStatus);

    connect(backupController, &BackupController::errorOccurred, this, &MainWindow::onBackupError);
    connect(backupController, &BackupController::errorOccurred, this,
            [this](const QString &error) {
                QMessageBox::critical(this, ErrorMessages::BACKUP_DELETION_ERROR_TITLE, error);
            });
}

// ## File & View Setup ##

// Sets up the source view
void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath(BackupPaths::DEFAULT_ROOT_PATH);
    sourceModel->setFilter(FileSystemSettings::FILE_SYSTEM_FILTER);

    ui->DriveTreeView->setModel(sourceModel);
    ui->DriveTreeView->setRootIndex(sourceModel->index(BackupPaths::DEFAULT_ROOT_PATH));
    ui->DriveTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->DriveTreeView);
}

// Sets up the backup staging view
void MainWindow::setupBackupStagingTreeView() {
    ui->BackupStagingTreeView->setModel(stagingModel);
    ui->BackupStagingTreeView->header()->setVisible(true);
    ui->BackupStagingTreeView->header()->setStretchLastSection(true);
    ui->BackupStagingTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->BackupStagingTreeView);
}

// Sets up the destination view
void MainWindow::setupDestinationView() {
    destinationModel->setFilter(FileSystemSettings::FILE_SYSTEM_FILTER);
    destinationModel->sort(0, Qt::DescendingOrder);

    ui->BackupDestinationView->setModel(destinationModel);
    ui->BackupDestinationView->setRootIndex(
        destinationModel->setRootPath(ConfigManager::getInstance().getBackupDirectory())
        );

    removeAllColumnsFromTreeView(ui->BackupDestinationView);
}

// Removes unnecessary columns from a tree view
void MainWindow::removeAllColumnsFromTreeView(QTreeView *treeView) {
    if (!treeView) return;

    if (QAbstractItemModel *model = treeView->model(); model) {
        for (int i = UISettings::TreeView::START_HIDDEN_COLUMN;
             i < UISettings::TreeView::DEFAULT_COLUMN_COUNT; ++i) {
            treeView->setColumnHidden(i, true);
        }
    }
}

// ## File Watcher Management ##

// Starts watching a directory for changes
void MainWindow::startWatchingBackupDirectory(const QString &path) {
    fileWatcher->startWatching(path);
    connect(fileWatcher, &FileWatcher::directoryChanged, this, &MainWindow::onBackupDirectoryChanged);
}

// Updates the file watcher
void MainWindow::updateFileWatcher() {
    const QString watchPath = destinationModel->rootPath();
    if (!fileWatcher->watchedDirectories().contains(watchPath)) {
        fileWatcher->startWatching(watchPath);
    }
}

// Responds to file change event
void MainWindow::onFileChanged(const QString &path) {
    Q_UNUSED(path);
    refreshBackupStatus();
}

// Responds to backup directory change event
void MainWindow::onBackupDirectoryChanged() {
    updateFileWatcher();
    refreshBackupStatus();
}

// ## Backup Feedback ##

// Handles backup completion
void MainWindow::onBackupCompleted() {
    ui->TransferProgressText->setText(ProgressSettings::PROGRESS_BAR_COMPLETION_MESSAGE);
    ui->TransferProgressText->setVisible(true);
    QTimer::singleShot(3000, this, [this]() {
        ui->TransferProgressText->setText(ProgressSettings::PROGRESS_BAR_INITIAL_MESSAGE);
    });
}

// Handles backup error
void MainWindow::onBackupError(const QString &error) {
    Q_UNUSED(error);
    ui->TransferProgressText->setText(ProgressSettings::PROGRESS_BAR_INITIAL_MESSAGE);
    ui->TransferProgressText->setVisible(true);
}

// ## Backup Operations ##

// Adds selected files to backup staging
void MainWindow::onAddToBackupClicked() {
    const QModelIndexList selectedIndexes = ui->DriveTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::BACKUP_SELECTION_REQUIRED_TITLE,
                             ErrorMessages::ERROR_NO_ITEMS_SELECTED_FOR_BACKUP);
        return;
    }
    Utils::Backup::addSelectedPathsToStaging(ui->DriveTreeView, stagingModel);
}

// Removes selected files from backup staging
void MainWindow::onRemoveFromBackupClicked() {
    const QModelIndexList selectedIndexes = ui->BackupStagingTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::REMOVE_SELECTION_REQUIRED_TITLE,
                             ErrorMessages::ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL);
        return;
    }
    Utils::Backup::removeSelectedPathsFromStaging(ui->BackupStagingTreeView, stagingModel);
}

// Starts the backup process
void MainWindow::onCreateBackupClicked() {
    const QStringList pathsToBackup = stagingModel->getStagedPaths();
    if (pathsToBackup.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::NO_ITEMS_STAGED_FOR_BACKUP_TITLE,
                             ErrorMessages::ERROR_NO_ITEMS_STAGED_FOR_BACKUP);
        return;
    }

    const QString backupRoot = destinationModel->rootPath();
    QString errorMessage;
    if (!FileOperations::createBackupInfrastructure(backupRoot, errorMessage)) {
        QMessageBox::critical(this, ErrorMessages::ERROR_BACKUP_ALREADY_IN_PROGRESS, errorMessage);
        return;
    }

    ui->TransferProgressBar->setValue(ProgressSettings::PROGRESS_BAR_MIN_VALUE);
    ui->TransferProgressBar->setVisible(true);
    ui->TransferProgressText->setVisible(false);
    backupController->createBackup(backupRoot, pathsToBackup, ui->TransferProgressBar);
}

// Deletes the selected backup
void MainWindow::onDeleteBackupClicked() {
    const QModelIndex selectedIndex = ui->BackupDestinationView->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, ErrorMessages::BACKUP_DELETION_ERROR_TITLE,
                             ErrorMessages::ERROR_BACKUP_DELETE_FAILED);
        return;
    }
    const QString selectedPath = destinationModel->filePath(selectedIndex);
    if (QMessageBox::question(this, WarningMessages::WARNING_CONFIRM_BACKUP_DELETION,
                              QString(WarningMessages::MESSAGE_CONFIRM_BACKUP_DELETION).arg(selectedPath),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        backupController->deleteBackup(selectedPath);
    }
}

// Changes the backup destination
void MainWindow::onChangeBackupDestinationClicked() {
    const QString selectedDir = QFileDialog::getExistingDirectory(
        this,
        InfoMessages::SELECT_BACKUP_DESTINATION_TITLE,
        BackupPaths::DEFAULT_FILE_DIALOG_ROOT
        );
    if (selectedDir.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::BACKUP_LOCATION_REQUIRED_TITLE,
                             ErrorMessages::ERROR_NO_BACKUP_LOCATION_PATH_SELECTED);
        return;
    }
    if (!FileOperations::createDirectory(selectedDir)) {
        QMessageBox::critical(this, ErrorMessages::BACKUP_DIRECTORY_ERROR_TITLE,
                              ErrorMessages::ERROR_CREATING_BACKUP_DIRECTORY);
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

// ## Backup System Status & UI Updates ##

// Refreshes the backup status
void MainWindow::refreshBackupStatus() {
    const BackupStatus status = backupService->scanForBackupStatus();
    const QString statusColor = [status]() -> QString {
        switch (status) {
        case BackupStatus::Valid:
            return Colors::BACKUP_STATUS_COLOR_FOUND;
        case BackupStatus::Broken:
            return Colors::BACKUP_STATUS_COLOR_WARNING;
        default:
            return Colors::BACKUP_STATUS_COLOR_NOT_FOUND;
        }
    }();
    updateBackupStatusLabel(statusColor);
    updateBackupLocationLabel(backupService->getBackupRoot());
    updateBackupTotalCountLabel();
    updateBackupTotalSizeLabel();
    updateBackupLocationStatusLabel(backupService->getBackupRoot());
    updateLastBackupInfo();
}

// Updates last backup information
void MainWindow::updateLastBackupInfo() {
    const QJsonObject metadata = backupService->getLastBackupMetadata();
    if (metadata.isEmpty()) {
        for (auto* label : {ui->LastBackupNameLabel, ui->LastBackupTimestampLabel,
                            ui->LastBackupDurationLabel, ui->LastBackupSizeLabel}) {
            label->setText(Utilities::DEFAULT_VALUE_NOT_AVAILABLE);
        }
        return;
    }

    ui->LastBackupNameLabel->setText(Labels::LastBackup::NAME + metadata.value(BackupMetadataKeys::NAME).toString());

    ui->LastBackupTimestampLabel->setText(Labels::LastBackup::TIMESTAMP +
                                          Utils::Formatting::formatTimestamp(QDateTime::fromString(
                                                                                 metadata.value(BackupMetadataKeys::TIMESTAMP).toString(), Qt::ISODate),
                                                                             TimestampFormats::BACKUP_TIMESTAMP_DISPLAY_FORMAT));

    ui->LastBackupDurationLabel->setText(Labels::LastBackup::DURATION +
                                         Utils::Formatting::formatDuration(metadata.value(BackupMetadataKeys::DURATION).toInt()));

    ui->LastBackupSizeLabel->setText(Labels::LastBackup::SIZE +
                                     metadata.value(BackupMetadataKeys::SIZE_READABLE).toString());
}

// Updates the backup status label
void MainWindow::updateBackupStatusLabel(const QString &statusColor) {
    const QPixmap pixmap = Utils::UI::createStatusLightPixmap(statusColor, ProgressSettings::STATUS_LIGHT_SIZE);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, Labels::Backup::STATUS_LIGHT_IMAGE_FORMAT);

    const QString pixmapHtml = QString(HTMLTemplates::STATUS_LIGHT_ICON).arg(QString::fromUtf8(ba.toBase64()));
    const QString combinedHtml = QString(HTMLTemplates::STATUS_LABEL_HTML).arg(Labels::Backup::FOUND, pixmapHtml);
    ui->BackupStatusLabel->setTextFormat(Qt::RichText);
    ui->BackupStatusLabel->setText(combinedHtml);

    const bool backupExists = (statusColor == Colors::BACKUP_STATUS_COLOR_FOUND);
    for (QLabel* label : {ui->LastBackupNameLabel, ui->LastBackupTimestampLabel,
                          ui->LastBackupDurationLabel, ui->LastBackupSizeLabel}) {
        label->setVisible(backupExists);
    }
}

// Updates backup location label
void MainWindow::updateBackupLocationLabel(const QString &location) {
    ui->BackupLocationLabel->setText(Labels::Backup::LOCATION + location);
}

// Updates backup count label
void MainWindow::updateBackupTotalCountLabel() {
    ui->BackupTotalCountLabel->setText(Labels::Backup::TOTAL_COUNT + QString::number(backupService->getBackupCount()));
}

// Updates backup size label
void MainWindow::updateBackupTotalSizeLabel() {
    ui->BackupTotalSizeLabel->setText(Labels::Backup::TOTAL_SIZE +
                                      Utils::Formatting::formatSize(backupService->getTotalBackupSize()));
}

// Updates backup location access label
void MainWindow::updateBackupLocationStatusLabel(const QString &location) {
    QFileInfo dirInfo(location);
    const QString status = dirInfo.exists()
                               ? (dirInfo.isWritable() ? DirectoryStatus::WRITABLE : DirectoryStatus::READ_ONLY)
                               : DirectoryStatus::UNKNOWN;
    ui->BackupLocationStatusLabel->setText(Labels::Backup::LOCATION_ACCESS + status);
}

// ## Event Handlers ##

// Handles the close event
void MainWindow::closeEvent(QCloseEvent *event) {
    if (backupController->isBackupInProgress()) {
        QMessageBox::warning(this, ErrorMessages::ERROR_OPERATION_IN_PROGRESS,
                             WarningMessages::WARNING_OPERATION_STILL_RUNNING);
        event->ignore();
        return;
    }
    QMainWindow::closeEvent(event);
}
