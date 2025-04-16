// Project includes same directory
#include "mainwindow.h"
#include "ui_mainwindow.h"

// Project includes different directory
#include "../../config/_constants.h"
#include "../../config/configmanager/configmanager.h"
#include "../../ui/toolbarmanager/toolbarmanager.h"
#include "../../core/backup_module/controller/backupcontroller.h"
#include "../../core/backup_module/service/backupservice.h"
#include "../../core/backup_module/styles/backup_styling.h"
#include "../../core/backup_module/models/destinationproxymodel.h"
#include "../../core/utils/file_utils/filewatcher.h"
#include "../../core/utils/file_utils/fileoperations.h"
#include "../../core/utils/common_utils/utils.h"

// Built-in Qt includes
#include <QFileSystemModel>
#include <QFileDialog>
#include <QBuffer>
#include <QMessageBox>
#include <QTimer>

// Constructor
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

// Destructor
MainWindow::~MainWindow() {
    delete ui;
}

// ## Window and Layout Configuration ##

// Configure basic window properties
void MainWindow::configureWindow()
{
    setMinimumSize(AppConfig::k_MINIMUM_WINDOW_SIZE);
    resize(AppConfig::k_DEFAULT_WINDOW_SIZE);
    setMaximumSize(AppConfig::k_MAXIMUM_WINDOW_SIZE);

    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        const QRect screenGeometry = screen->availableGeometry();
        const QPoint center = screenGeometry.center() - QPoint(AppConfig::k_DEFAULT_WINDOW_SIZE.width() / 2,
                                                               AppConfig::k_DEFAULT_WINDOW_SIZE.height() / 2);
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
}

// Initializes the backup system
void MainWindow::initializeBackupSystem() {
    const QString backupDirectory = ConfigManager::getInstance().getBackupDirectory();
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
                QMessageBox::critical(this, ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE, error);
            });
}

// ## File & View Setup ##

// Sets up the source view
void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath(Backup::Paths::k_DEFAULT_ROOT_PATH);
    sourceModel->setFilter(Backup::FileSystem::k_FILE_SYSTEM_FILTER);

    ui->DriveTreeView->setModel(sourceModel);
    ui->DriveTreeView->setRootIndex(sourceModel->index(Backup::Paths::k_DEFAULT_ROOT_PATH));
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
    destinationModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    destinationModel->setNameFilters(QStringList() << "*");
    destinationModel->setNameFilterDisables(false);
    destinationModel->sort(0, Qt::DescendingOrder);

    if (!destinationProxyModel) {
        destinationProxyModel = new DestinationProxyModel(this);
        destinationProxyModel->setSourceModel(destinationModel);
        destinationProxyModel->setExcludedFolderName(Backup::Infrastructure::k_BACKUP_SETUP_FOLDER);
    }

    ui->BackupDestinationView->setModel(destinationProxyModel);

    QString backupDir = ConfigManager::getInstance().getBackupDirectory();
    QModelIndex sourceRootIndex = destinationModel->setRootPath(backupDir);
    QModelIndex proxyRootIndex = destinationProxyModel->mapFromSource(sourceRootIndex);
    ui->BackupDestinationView->setRootIndex(proxyRootIndex);

    removeAllColumnsFromTreeView(ui->BackupDestinationView);
}

// Removes unnecessary columns from a tree view
void MainWindow::removeAllColumnsFromTreeView(QTreeView *treeView) {
    if (!treeView) return;

    if (QAbstractItemModel *model = treeView->model(); model) {
        for (int i = UISettings::TreeView::k_START_HIDDEN_COLUMN;
             i < UISettings::TreeView::k_DEFAULT_COLUMN_COUNT; ++i) {
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
    ui->TransferProgressText->setText(ProgressSettings::k_PROGRESS_BAR_COMPLETION_MESSAGE);
    ui->TransferProgressText->setVisible(true);
    QTimer::singleShot(3000, this, [this]() {
        ui->TransferProgressText->setText(ProgressSettings::k_PROGRESS_BAR_INITIAL_MESSAGE);
    });
}

// Handles backup error
void MainWindow::onBackupError(const QString &error) {
    Q_UNUSED(error);
    ui->TransferProgressText->setText(ProgressSettings::k_PROGRESS_BAR_INITIAL_MESSAGE);
    ui->TransferProgressText->setVisible(true);
}

// ## Backup Operations ##

// Adds selected files to backup staging
void MainWindow::onAddToBackupClicked() {
    const QModelIndexList selectedIndexes = ui->DriveTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_BACKUP_SELECTION_REQUIRED_TITLE,
                             ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_BACKUP);
        return;
    }
    Utils::Backup::addSelectedPathsToStaging(ui->DriveTreeView, stagingModel);
}

// Removes selected files from backup staging
void MainWindow::onRemoveFromBackupClicked() {
    const QModelIndexList selectedIndexes = ui->BackupStagingTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::k_REMOVE_SELECTION_REQUIRED_TITLE,
                             ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL);
        return;
    }
    Utils::Backup::removeSelectedPathsFromStaging(ui->BackupStagingTreeView, stagingModel);
}

// Starts the backup process
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

    ui->TransferProgressBar->setValue(ProgressSettings::k_PROGRESS_BAR_MIN_VALUE);
    ui->TransferProgressBar->setVisible(true);
    ui->TransferProgressText->setVisible(false);
    backupController->createBackup(backupRoot, pathsToBackup, ui->TransferProgressBar);
}

// Deletes the selected backup
void MainWindow::onDeleteBackupClicked() {
    const QModelIndex selectedIndex = ui->BackupDestinationView->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
                             ErrorMessages::k_ERROR_BACKUP_DELETE_FAILED);
        return;
    }
    const QString selectedPath = destinationModel->filePath(selectedIndex);
    if (QMessageBox::question(this, WarningMessages::k_WARNING_CONFIRM_BACKUP_DELETION,
                              QString(WarningMessages::k_MESSAGE_CONFIRM_BACKUP_DELETION).arg(selectedPath),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        backupController->deleteBackup(selectedPath);
    }
}

// Changes the backup destination
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

// Updates last backup information
void MainWindow::updateLastBackupInfo() {
    const QJsonObject metadata = backupService->getLastBackupMetadata();
    if (metadata.isEmpty()) {
        for (auto* label : {ui->LastBackupNameLabel, ui->LastBackupTimestampLabel,
                            ui->LastBackupDurationLabel, ui->LastBackupSizeLabel}) {
            label->setText(Utilities::k_DEFAULT_VALUE_NOT_AVAILABLE);
        }
        return;
    }

    ui->LastBackupNameLabel->setText(Labels::LastBackup::k_NAME + metadata.value(Backup::MetadataKeys::k_NAME).toString());

    ui->LastBackupTimestampLabel->setText(Labels::LastBackup::k_TIMESTAMP +
                                          Utils::Formatting::formatTimestamp(QDateTime::fromString(
                                                                                 metadata.value(Backup::MetadataKeys::k_TIMESTAMP).toString(), Qt::ISODate),
                                                                             Backup::Timestamps::k_BACKUP_TIMESTAMP_DISPLAY_FORMAT));

    ui->LastBackupDurationLabel->setText(Labels::LastBackup::k_DURATION +
                                         Utils::Formatting::formatDuration(metadata.value(Backup::MetadataKeys::k_DURATION).toInt()));

    ui->LastBackupSizeLabel->setText(Labels::LastBackup::k_SIZE +
                                     metadata.value(Backup::MetadataKeys::k_SIZE_READABLE).toString());
}

// Updates the backup status label
void MainWindow::updateBackupStatusLabel(const QString &statusColor) {
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

// Updates backup location label
void MainWindow::updateBackupLocationLabel(const QString &location) {
    ui->BackupLocationLabel->setText(Labels::Backup::k_LOCATION + location);
}

// Updates backup count label
void MainWindow::updateBackupTotalCountLabel() {
    ui->BackupTotalCountLabel->setText(Labels::Backup::k_TOTAL_COUNT + QString::number(backupService->getBackupCount()));
}

// Updates backup size label
void MainWindow::updateBackupTotalSizeLabel() {
    ui->BackupTotalSizeLabel->setText(Labels::Backup::k_TOTAL_SIZE +
                                      Utils::Formatting::formatSize(backupService->getTotalBackupSize()));
}

// Updates backup location access label
void MainWindow::updateBackupLocationStatusLabel(const QString &location) {
    QFileInfo dirInfo(location);
    const QString status = dirInfo.exists()
                               ? (dirInfo.isWritable() ? Labels::DirectoryStatus::k_WRITABLE : Labels::DirectoryStatus::k_READ_ONLY)
                               : Labels::DirectoryStatus::k_UNKNOWN;
    ui->BackupLocationStatusLabel->setText(Labels::Backup::k_LOCATION_ACCESS + status);
}

// ## Event Handlers ##

// Handles the close event
void MainWindow::closeEvent(QCloseEvent *event) {
    if (backupController->isBackupInProgress()) {
        QMessageBox::warning(this, ErrorMessages::k_ERROR_OPERATION_IN_PROGRESS,
                             WarningMessages::k_WARNING_OPERATION_STILL_RUNNING);
        event->ignore();
        return;
    }
    QMainWindow::closeEvent(event);
}
