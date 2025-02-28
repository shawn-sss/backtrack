#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../config/_constants.h"
#include "../../config/configmanager/configmanager.h"
#include "../../config/resources_settings.h"
#include "../../core/backup_module/models/stagingmodel.h"
#include "../../core/backup_module/controller/backupcontroller.h"
#include "../../core/backup_module/service/backupservice.h"
#include "../../core/utils/file_utils/fileoperations.h"
#include "../../core/utils/common_utils/utils.h"
#include "../../core/utils/file_utils/filewatcher.h"
#include "../../ui/settingsdialog/settingsdialog.h"

#include <QLabel>
#include <QBuffer>
#include <QStringList>
#include <QPushButton>
#include <QTreeView>
#include <QFileDialog>
#include <QJsonObject>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>
#include <QHBoxLayout>
#include <QMenu>
#include <QLayout>
#include <QFileSystemModel>
#include <QToolBar>
#include <QProgressBar>
#include <QWidget>
#include <QWidgetAction>
#include <QPainter>
#include <QPixmap>
#include <QtSvg/QSvgRenderer>

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    destinationModel(new QFileSystemModel(this)),
    sourceModel(new QFileSystemModel(this)),
    fileWatcher(new FileWatcher(this)),
    backupService(new BackupService(ConfigManager::getInstance().getBackupDirectory())),
    stagingModel(new StagingModel(this)),
    backupController(new BackupController(backupService, this)) {

    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setStatusBar(nullptr);
    titleBar = Utils::setupCustomTitleBar(this, TitleBarMode::MainWindow);

    if (ui->toolBar) {
        ui->toolBar->setFloatable(false);
        ui->toolBar->setMovable(false);
        ui->toolBar->setVisible(true);
        ui->toolBar->setIconSize(IconStyles::TOOLBAR_SIZE);
        ui->toolBar->setStyleSheet(Styles::Toolbar::MINIMAL);
    }

    initializeUI();
    initializeBackupSystem();
    setupConnections();

    QList<QPushButton*> buttons = {
        ui->AddToBackupButton,
        ui->RemoveFromBackupButton,
        ui->CreateBackupButton,
        ui->ChangeBackupDestinationButton,
        ui->DeleteBackupButton
    };

    for (QPushButton* button : buttons) {
        button->setCursor(Qt::PointingHandCursor);
    }
}


// Destructor
MainWindow::~MainWindow() {
    delete ui;
}

// Initializes UI components
void MainWindow::initializeUI() {
    setupCustomTitleBar();
    setupToolBar();
    Utils::UI::setupProgressBar(ui->TransferProgressBar, ProgressSettings::PROGRESS_BAR_MIN_VALUE,
                                ProgressSettings::PROGRESS_BAR_MAX_VALUE, ProgressSettings::PROGRESS_BAR_HEIGHT,
                                ProgressSettings::PROGRESS_BAR_TEXT_VISIBLE);
    ui->TransferProgressBar->setVisible(false);
}

// Sets up the custom title bar
void MainWindow::setupCustomTitleBar() {
    titleBar = Utils::setupCustomTitleBar(this, TitleBarMode::MainWindow);
    if (!titleBar) return;

    QWidget *container = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar, 0, Qt::AlignTop);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    if (ui->toolBar) {
        ui->toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        contentLayout->addWidget(ui->toolBar, 0, Qt::AlignLeft);
    }

    if (centralWidget()) {
        QWidget *contentContainer = centralWidget();
        contentContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        contentLayout->addWidget(contentContainer, 1);
    }

    mainLayout->addLayout(contentLayout, 1);
    container->setLayout(mainLayout);
    setCentralWidget(container);
}

// Initializes the backup system
void MainWindow::initializeBackupSystem() {
    QString backupDirectory = ConfigManager::getInstance().getBackupDirectory();

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

// Sets up signal connections
void MainWindow::setupConnections() {
    connectBackupSignals();
    connect(ui->AddToBackupButton, &QPushButton::clicked, this, &MainWindow::onAddToBackupClicked);
    connect(ui->ChangeBackupDestinationButton, &QPushButton::clicked, this, &MainWindow::onChangeBackupDestinationClicked);
    connect(ui->RemoveFromBackupButton, &QPushButton::clicked, this, &MainWindow::onRemoveFromBackupClicked);
    connect(ui->CreateBackupButton, &QPushButton::clicked, this, &MainWindow::onCreateBackupClicked);
    connect(ui->DeleteBackupButton, &QPushButton::clicked, this, &MainWindow::onDeleteBackupClicked);
    connect(ui->actionOpenSettings, &QAction::triggered, this, &MainWindow::openSettings);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitApplication);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::showHelpDialog);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAboutButtonClicked);
}

// Configures toolbar actions
void MainWindow::setupToolbarActions() {
    if (!ui->actionOpenSettings) ui->actionOpenSettings = new QAction(this);
    if (!ui->actionExit) ui->actionExit = new QAction(this);
    if (!ui->actionHelp) ui->actionHelp = new QAction(this);
    if (!ui->actionAbout) ui->actionAbout = new QAction(this);

    ui->actionOpenSettings->setIcon(QIcon(Resources::Toolbar::SETTINGS_ICON_PATH));
    ui->actionOpenSettings->setText(Labels::Toolbar::SETTINGS);

    ui->actionExit->setIcon(QIcon(Resources::Toolbar::EXIT_ICON_PATH));
    ui->actionExit->setText(Labels::Toolbar::EXIT);

    ui->actionHelp->setIcon(QIcon(Resources::Toolbar::HELP_ICON_PATH));
    ui->actionHelp->setText(Labels::Toolbar::HELP);

    ui->actionAbout->setIcon(QIcon(Resources::Toolbar::ABOUT_ICON_PATH));
    ui->actionAbout->setText(Labels::Toolbar::ABOUT);
}

// Configures toolbar
void MainWindow::setupToolBar() {
    if (!ui->toolBar) return;
    setupToolbarActions();

    ui->toolBar->setFloatable(false);
    ui->toolBar->setMovable(false);
    ui->toolBar->setVisible(true);
    ui->toolBar->setIconSize(IconStyles::TOOLBAR_SIZE);
    ui->toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    ui->toolBar->setStyleSheet(Styles::Toolbar::GENERAL);

    ui->toolBar->clear();
    ui->toolBar->addAction(ui->actionOpenSettings);
    ui->toolBar->addAction(ui->actionHelp);
    ui->toolBar->addAction(ui->actionAbout);

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->toolBar->addWidget(spacer);
    ui->toolBar->addAction(ui->actionExit);

    for (QAction *&action : ui->toolBar->actions()) {
        QWidget *widget = ui->toolBar->widgetForAction(action);
        if (widget) widget->setCursor(Qt::PointingHandCursor);
    }
}

// Adds a spacer to the toolbar
void MainWindow::addToolbarSpacer() {
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->toolBar->addWidget(spacer);
}

// Connects toolbar actions
void MainWindow::connectToolbarActions(QAction* helpAction, QAction* aboutAction) {
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitApplication);
    connect(helpAction, &QAction::triggered, this, &MainWindow::showHelpDialog);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAboutButtonClicked);
}

// Handles mouse events for dragging
void MainWindow::handleMouseEvent(QMouseEvent *event, bool isPress) {
    if (event->button() == Qt::LeftButton) {
        dragging = isPress;
        if (isPress) {
            lastMousePosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        }
        event->accept();
    }
}

// Handles mouse press event
void MainWindow::mousePressEvent(QMouseEvent *event) {
    handleMouseEvent(event, true);
}

// Handles mouse move event
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - lastMousePosition);
        event->accept();
    }
}
// Handles mouse release events
void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    handleMouseEvent(event, false);
}

// Refreshes backup status
void MainWindow::refreshBackupStatus() {
    BackupStatus status = backupService->scanForBackupStatus();
    switch (status) {
    case BackupStatus::Valid:
        updateBackupStatusLabel(Colors::BACKUP_STATUS_COLOR_FOUND);
        break;
    case BackupStatus::Broken:
        updateBackupStatusLabel(Colors::BACKUP_STATUS_COLOR_WARNING);
        break;
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

// Changes the backup destination
void MainWindow::onChangeBackupDestinationClicked() {
    QString selectedDir = QFileDialog::getExistingDirectory(this,
                                                            InfoMessages::SELECT_BACKUP_DESTINATION_TITLE,
                                                            BackupPaths::DEFAULT_FILE_DIALOG_ROOT);
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

// Adds selected paths to backup staging
void MainWindow::onAddToBackupClicked() {
    QModelIndexList selectedIndexes = ui->DriveTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::BACKUP_SELECTION_REQUIRED_TITLE, ErrorMessages::ERROR_NO_ITEMS_SELECTED_FOR_BACKUP);
        return;
    }
    Utils::Backup::addSelectedPathsToStaging(ui->DriveTreeView, stagingModel);
}

// Removes selected paths from backup staging
void MainWindow::onRemoveFromBackupClicked() {
    QModelIndexList selectedIndexes = ui->BackupStagingTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, ErrorMessages::REMOVE_SELECTION_REQUIRED_TITLE, ErrorMessages::ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL);
        return;
    }
    Utils::Backup::removeSelectedPathsFromStaging(ui->BackupStagingTreeView, stagingModel);
}

// Initiates backup creation
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
    ui->TransferProgressBar->setValue(ProgressSettings::PROGRESS_BAR_MIN_VALUE);
    ui->TransferProgressBar->setVisible(true);
    backupController->createBackup(backupRoot, pathsToBackup, ui->TransferProgressBar);
}

// Deletes a selected backup
void MainWindow::onDeleteBackupClicked() {
    QModelIndex selectedIndex = ui->BackupDestinationView->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, ErrorMessages::BACKUP_DELETION_ERROR_TITLE, ErrorMessages::ERROR_BACKUP_DELETE_FAILED);
        return;
    }
    QString selectedPath = destinationModel->filePath(selectedIndex);
    if (QMessageBox::question(this, WarningMessages::WARNING_CONFIRM_BACKUP_DELETION,
                              QString(WarningMessages::MESSAGE_CONFIRM_BACKUP_DELETION).arg(selectedPath),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        backupController->deleteBackup(selectedPath);
    }
}

// Starts watching the backup directory
void MainWindow::startWatchingBackupDirectory(const QString &path) {
    fileWatcher->startWatching(path);
    connect(fileWatcher, &FileWatcher::directoryChanged, this, &MainWindow::onBackupDirectoryChanged);
}

// Updates the file watcher
void MainWindow::updateFileWatcher() {
    QString watchPath = destinationModel->rootPath();
    if (!fileWatcher->watchedDirectories().contains(watchPath)) {
        fileWatcher->startWatching(watchPath);
    }
}

// Handles file changes
void MainWindow::onFileChanged(const QString &path) {
    Q_UNUSED(path);
    refreshBackupStatus();
}



// Handles backup directory change
void MainWindow::onBackupDirectoryChanged() {
    updateFileWatcher();
    refreshBackupStatus();
}

// Updates backup status label
void MainWindow::updateBackupStatusLabel(const QString &statusColor) {
    QPixmap pixmap = Utils::UI::createStatusLightPixmap(statusColor, ProgressSettings::STATUS_LIGHT_SIZE);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, Labels::Backup::STATUS_LIGHT_IMAGE_FORMAT);
    QString pixmapHtml = QString(HTMLTemplates::STATUS_LIGHT_ICON).arg(QString::fromUtf8(ba.toBase64()));
    QString combinedHtml = QString(HTMLTemplates::STATUS_LABEL_HTML).arg(Labels::Backup::FOUND, pixmapHtml);
    ui->BackupStatusLabel->setTextFormat(Qt::RichText);
    ui->BackupStatusLabel->setText(combinedHtml);

    bool backupExists = (statusColor == Colors::BACKUP_STATUS_COLOR_FOUND);
    ui->LastBackupNameLabel->setVisible(backupExists);
    ui->LastBackupTimestampLabel->setVisible(backupExists);
    ui->LastBackupDurationLabel->setVisible(backupExists);
    ui->LastBackupSizeLabel->setVisible(backupExists);
}

// Updates backup location label
void MainWindow::updateBackupLocationLabel(const QString &location) {
    ui->BackupLocationLabel->setText(Labels::Backup::LOCATION + location);
}

// Updates backup total count label
void MainWindow::updateBackupTotalCountLabel() {
    ui->BackupTotalCountLabel->setText(Labels::Backup::TOTAL_COUNT + QString::number(backupService->getBackupCount()));
}

// Updates backup total size label
void MainWindow::updateBackupTotalSizeLabel() {
    quint64 totalSize = backupService->getTotalBackupSize();
    QString humanReadableSize = Utils::Formatting::formatSize(totalSize);
    ui->BackupTotalSizeLabel->setText(Labels::Backup::TOTAL_SIZE + humanReadableSize);
}

// Updates backup location status label
void MainWindow::updateBackupLocationStatusLabel(const QString &location) {
    QFileInfo dirInfo(location);
    QString status = dirInfo.exists() ? (dirInfo.isWritable() ? DirectoryStatus::WRITABLE : DirectoryStatus::READ_ONLY) : DirectoryStatus::UNKNOWN;
    ui->BackupLocationStatusLabel->setText(Labels::Backup::LOCATION_ACCESS + status);
}

// Updates last backup info
void MainWindow::updateLastBackupInfo() {
    QJsonObject metadata = backupService->getLastBackupMetadata();
    if (metadata.isEmpty()) {
        QString notAvailable = Utilities::DEFAULT_VALUE_NOT_AVAILABLE;
        ui->LastBackupNameLabel->setText(Labels::LastBackup::NAME + notAvailable);
        ui->LastBackupTimestampLabel->setText(Labels::LastBackup::TIMESTAMP + notAvailable);
        ui->LastBackupDurationLabel->setText(Labels::LastBackup::DURATION + notAvailable);
        ui->LastBackupSizeLabel->setText(Labels::LastBackup::SIZE + notAvailable);
        return;
    }

    QString backupName = metadata.value(BackupMetadataKeys::NAME).toString(Utilities::DEFAULT_VALUE_NOT_AVAILABLE);
    QString timestampStr = metadata.value(BackupMetadataKeys::TIMESTAMP).toString();
    int durationMs = metadata.value(BackupMetadataKeys::DURATION).toInt(0);
    QString totalSize = metadata.value(BackupMetadataKeys::SIZE_READABLE).toString(Utilities::DEFAULT_VALUE_NOT_AVAILABLE);

    QDateTime backupTimestamp = QDateTime::fromString(timestampStr, Qt::ISODate);
    QString formattedTimestamp = Utils::Formatting::formatTimestamp(backupTimestamp, TimestampFormats::BACKUP_TIMESTAMP_DISPLAY_FORMAT);
    QString formattedDuration = Utils::Formatting::formatDuration(durationMs);

    ui->LastBackupNameLabel->setText(Labels::LastBackup::NAME + backupName);
    ui->LastBackupTimestampLabel->setText(Labels::LastBackup::TIMESTAMP + formattedTimestamp);
    ui->LastBackupDurationLabel->setText(Labels::LastBackup::DURATION + formattedDuration);
    ui->LastBackupSizeLabel->setText(Labels::LastBackup::SIZE + totalSize);
}

// Handles close event
void MainWindow::closeEvent(QCloseEvent *event) {
    if (backupController->isBackupInProgress()) {
        QMessageBox::warning(this, ErrorMessages::ERROR_OPERATION_IN_PROGRESS, WarningMessages::WARNING_OPERATION_STILL_RUNNING);
        event->ignore();
        return;
    }
    QMainWindow::closeEvent(event);
}

// Setup methods
void MainWindow::setupDestinationView() {
    destinationModel->setFilter(FileSystemSettings::FILE_SYSTEM_FILTER);
    destinationModel->sort(0, Qt::DescendingOrder);
    ui->BackupDestinationView->setModel(destinationModel);
    ui->BackupDestinationView->setRootIndex(destinationModel->setRootPath(ConfigManager::getInstance().getBackupDirectory()));
    removeAllColumnsFromTreeView(ui->BackupDestinationView);
}

void MainWindow::setupBackupStagingTreeView() {
    ui->BackupStagingTreeView->setModel(stagingModel);
    ui->BackupStagingTreeView->header()->setVisible(true);
    ui->BackupStagingTreeView->header()->setStretchLastSection(true);
    ui->BackupStagingTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    removeAllColumnsFromTreeView(ui->BackupStagingTreeView);
}

void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath(BackupPaths::DEFAULT_ROOT_PATH);
    sourceModel->setFilter(FileSystemSettings::FILE_SYSTEM_FILTER);
    ui->DriveTreeView->setModel(sourceModel);
    ui->DriveTreeView->setRootIndex(sourceModel->index(BackupPaths::DEFAULT_ROOT_PATH));
    ui->DriveTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    removeAllColumnsFromTreeView(ui->DriveTreeView);
}

// UI dialog methods
void MainWindow::openSettings() {
    SettingsDialog settingsDialog(this);
    settingsDialog.exec();
}

void MainWindow::exitApplication() {
    QApplication::quit();
}

void MainWindow::showHelpDialog() {
    QString extendedMessage = QString(HelpInfo::HELP_EXTENDED_MESSAGE).arg(ConfigManager::getInstance().getConfigFilePathPublic());
    QMessageBox::information(this, HelpInfo::HELP_WINDOW_TITLE, HelpInfo::HELP_WINDOW_MESSAGE + extendedMessage);
}

void MainWindow::onAboutButtonClicked() {
    QMessageBox::information(this, AboutInfo::ABOUT_WINDOW_TITLE, QString(AboutInfo::ABOUT_WINDOW_MESSAGE).arg(AppInfo::APP_VERSION, AppInfo::APP_DISPLAY_TITLE, AppInfo::AUTHOR_NAME));
}

// Backup management
void MainWindow::connectBackupSignals() {
    connect(backupController, &BackupController::backupCreated, this, &MainWindow::refreshBackupStatus);
    connect(backupController, &BackupController::backupDeleted, this, &MainWindow::refreshBackupStatus);
    connect(backupController, &BackupController::errorOccurred, this, [this](const QString &error) {
        QMessageBox::critical(this, ErrorMessages::BACKUP_DELETION_ERROR_TITLE, error);
    });
}

// Utility methods
void MainWindow::removeAllColumnsFromTreeView(QTreeView *treeView) {
    if (!treeView) return;
    QAbstractItemModel *model = treeView->model();
    if (model) {
        for (int i = UISettings::TreeView::START_HIDDEN_COLUMN;
             i < UISettings::TreeView::DEFAULT_COLUMN_COUNT; ++i) {
            treeView->setColumnHidden(i, true);
        }
    }
}
