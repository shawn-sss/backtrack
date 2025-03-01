#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../config/_constants.h"
#include "../../config/configmanager/configmanager.h"
#include "../../ui/toolbarmanager/toolbarmanagerstyling.h"
#include "../../ui/settingsdialog/settingsdialog.h"
#include "../../core/backup_module/controller/backupcontroller.h"
#include "../../core/backup_module/service/backupservice.h"
#include "../../core/utils/file_utils/fileoperations.h"
#include "../../core/utils/common_utils/utils.h"
#include "../../core/utils/file_utils/filewatcher.h"

#include <QBuffer>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileSystemModel>

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
    configureWindow();
    setupLayout();
    setupToolBar();
    initializeUI();
    initializeBackupSystem();
    setupConnections();
    applyButtonCursors();
}

// Destructor
MainWindow::~MainWindow() {
    delete ui;
}

// Configures main window properties
void MainWindow::configureWindow() {
    setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setStatusBar(nullptr);
}

// Sets up the layout
void MainWindow::setupLayout() {
    titleBar = setupCustomTitleBar(this, TitleBarMode::MainWindow);

    auto *container = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar, 0, Qt::AlignTop);

    auto *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    if (ui->toolBar) {
        configureToolBar();
        contentLayout->addWidget(ui->toolBar, 0, Qt::AlignLeft);
    }

    auto *contentContainer = centralWidget();
    contentContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    contentLayout->addWidget(contentContainer, 1);

    mainLayout->addLayout(contentLayout, 1);
    setCentralWidget(container);
}

// Configures the toolbar appearance
void MainWindow::configureToolBar() {
    ui->toolBar->setFloatable(false);
    ui->toolBar->setMovable(false);
    ui->toolBar->setVisible(true);
    ui->toolBar->setIconSize(ToolbarStyles::DEFAULT_ICON_SIZE);
    ui->toolBar->setStyleSheet(ToolbarStyles::MINIMAL_STYLE);
    ui->toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
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

// Initializes the UI
void MainWindow::initializeUI() {
    setupToolBar();
    Utils::UI::setupProgressBar(
        ui->TransferProgressBar,
        ProgressSettings::PROGRESS_BAR_MIN_VALUE,
        ProgressSettings::PROGRESS_BAR_MAX_VALUE,
        ProgressSettings::PROGRESS_BAR_HEIGHT,
        ProgressSettings::PROGRESS_BAR_TEXT_VISIBLE
        );
    ui->TransferProgressBar->setVisible(false);
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

    const struct {
        QAction* action;
        void (MainWindow::*slot)();
    } actionConnections[] = {
        { ui->actionOpenSettings, &MainWindow::openSettings },
        { ui->actionExit, &MainWindow::exitApplication },
        { ui->actionHelp, &MainWindow::showHelpDialog },
        { ui->actionAbout, &MainWindow::onAboutButtonClicked }
    };

    for (const auto& conn : actionConnections) {
        connect(conn.action, &QAction::triggered, this, conn.slot);
    }
}

// Configures toolbar actions
void MainWindow::setupToolbarActions() {
    const struct {
        QAction*& action;
        const QString& iconPath;
        const QString& label;
    } actions[] = {
        { ui->actionOpenSettings, Resources::Toolbar::SETTINGS_ICON_PATH, Labels::Toolbar::SETTINGS },
        { ui->actionExit, Resources::Toolbar::EXIT_ICON_PATH, Labels::Toolbar::EXIT },
        { ui->actionHelp, Resources::Toolbar::HELP_ICON_PATH, Labels::Toolbar::HELP },
        { ui->actionAbout, Resources::Toolbar::ABOUT_ICON_PATH, Labels::Toolbar::ABOUT }
    };

    for (const auto& action : actions) {
        if (!action.action) action.action = new QAction(this);
        action.action->setIcon(QIcon(action.iconPath));
        action.action->setText(action.label);
    }
}

// Full toolbar setup
void MainWindow::setupToolBar() {
    if (!ui->toolBar) return;
    setupToolbarActions();
    ui->toolBar->clear();
    ui->toolBar->addAction(ui->actionOpenSettings);
    ui->toolBar->addAction(ui->actionHelp);
    ui->toolBar->addAction(ui->actionAbout);

    auto *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->toolBar->addWidget(spacer);
    ui->toolBar->addAction(ui->actionExit);

    for (QAction* action : ui->toolBar->actions()) {
        if (QWidget* widget = ui->toolBar->widgetForAction(action)) {
            widget->setCursor(Qt::PointingHandCursor);
        }
    }
}

// Mouse event handling
void MainWindow::mousePressEvent(QMouseEvent *event) {
    Utils::UI::handleMousePress(this, event, dragging, lastMousePosition);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    Utils::UI::handleMouseMove(this, event, dragging, lastMousePosition);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    Utils::UI::handleMouseRelease(event, dragging);
}

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

// Opens the settings dialog
void MainWindow::openSettings() {
    SettingsDialog settingsDialog(this);
    settingsDialog.exec();
}

// Exits the application
void MainWindow::exitApplication() {
    QApplication::quit();
}

// Shows the help dialog
void MainWindow::showHelpDialog() {
    const QString extendedMessage = QString(HelpInfo::HELP_EXTENDED_MESSAGE)
    .arg(ConfigManager::getInstance().getConfigFilePathPublic());

    QMessageBox::information(this, HelpInfo::HELP_WINDOW_TITLE,
                             HelpInfo::HELP_WINDOW_MESSAGE + extendedMessage);
}

// Shows the about dialog
void MainWindow::onAboutButtonClicked() {
    QMessageBox::information(this, AboutInfo::ABOUT_WINDOW_TITLE,
                             QString(AboutInfo::ABOUT_WINDOW_MESSAGE)
                                 .arg(AppInfo::APP_VERSION,
                                      AppInfo::APP_DISPLAY_TITLE,
                                      AppInfo::AUTHOR_NAME));
}

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

// Connects backup-related signals
void MainWindow::connectBackupSignals() {
    connect(backupController, &BackupController::backupCreated, this, &MainWindow::refreshBackupStatus);
    connect(backupController, &BackupController::backupDeleted, this, &MainWindow::refreshBackupStatus);
    connect(backupController, &BackupController::errorOccurred, this,
            [this](const QString &error) {
                QMessageBox::critical(this, ErrorMessages::BACKUP_DELETION_ERROR_TITLE, error);
            });
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

// Sets up the backup staging view
void MainWindow::setupBackupStagingTreeView() {
    ui->BackupStagingTreeView->setModel(stagingModel);
    ui->BackupStagingTreeView->header()->setVisible(true);
    ui->BackupStagingTreeView->header()->setStretchLastSection(true);
    ui->BackupStagingTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->BackupStagingTreeView);
}

// Sets up the source view
void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath(BackupPaths::DEFAULT_ROOT_PATH);
    sourceModel->setFilter(FileSystemSettings::FILE_SYSTEM_FILTER);

    ui->DriveTreeView->setModel(sourceModel);
    ui->DriveTreeView->setRootIndex(sourceModel->index(BackupPaths::DEFAULT_ROOT_PATH));
    ui->DriveTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->DriveTreeView);
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
