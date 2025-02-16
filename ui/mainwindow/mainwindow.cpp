#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../core/config/_constants.h"
#include "../../core/config/resources_settings.h"
#include "../../core/backup_module/controller/backupcontroller.h"
#include "../../core/backup_module/service/backupservice.h"
#include "../../core/backup_module/models/stagingmodel.h"
#include "../../core/utils/file_utils/fileoperations.h"
#include "../../core/utils/common_utils/utils.h"
#include "../../core/utils/file_utils/filewatcher.h"
#include "../../ui/settingsdialog/settingsdialog.h"

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
#include <QMenu>
#include <QLayout>
#include <QToolBar>
#include <QWidgetAction>
#include <QApplication>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

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
    setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    this->setStatusBar(nullptr);

    initializeUI();
    initializeBackupSystem();
    setupConnections();

    ui->AddToBackupButton->setCursor(Qt::PointingHandCursor);
    ui->RemoveFromBackupButton->setCursor(Qt::PointingHandCursor);
    ui->CreateBackupButton->setCursor(Qt::PointingHandCursor);
    ui->ChangeBackupDestinationButton->setCursor(Qt::PointingHandCursor);
    ui->DeleteBackupButton->setCursor(Qt::PointingHandCursor);
}

MainWindow::~MainWindow() {
    delete ui;
}

// **Initialization Methods**
void MainWindow::initializeUI() {
    setupCustomTitleBar();
    setupToolBar();

    Utils::UI::setupProgressBar(ui->TransferProgressBar,
                                ProgressConfig::PROGRESS_BAR_MIN_VALUE,
                                ProgressConfig::PROGRESS_BAR_MAX_VALUE,
                                ProgressConfig::PROGRESS_BAR_HEIGHT,
                                ProgressConfig::PROGRESS_BAR_TEXT_VISIBLE);
    ui->TransferProgressBar->setVisible(false);

    ui->AddToBackupButton->setStyleSheet(Styling::BUTTON_STYLE);
    ui->RemoveFromBackupButton->setStyleSheet(Styling::BUTTON_STYLE);
    ui->CreateBackupButton->setStyleSheet(Styling::BUTTON_STYLE);
    ui->ChangeBackupDestinationButton->setStyleSheet(Styling::BUTTON_STYLE);
    ui->DeleteBackupButton->setStyleSheet(Styling::BUTTON_STYLE);
}

void MainWindow::initializeBackupSystem() {
    if (!FileOperations::createDirectory(UserConfig::DEFAULT_BACKUP_DIRECTORY)) {
        QMessageBox::critical(this, ErrorMessages::BACKUP_INITIALIZATION_FAILED_TITLE,
                              ErrorMessages::ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY);
    }
    setupDestinationView();
    setupSourceTreeView();
    setupBackupStagingTreeView();
    refreshBackupStatus();
    startWatchingBackupDirectory(UserConfig::DEFAULT_BACKUP_DIRECTORY);
    updateFileWatcher();
}

void MainWindow::setupConnections() {
    connectBackupSignals();
    connect(ui->AddToBackupButton, &QPushButton::clicked, this, &MainWindow::onAddToBackupClicked);
    connect(ui->ChangeBackupDestinationButton, &QPushButton::clicked, this, &MainWindow::onChangeBackupDestinationClicked);
    connect(ui->RemoveFromBackupButton, &QPushButton::clicked, this, &MainWindow::onRemoveFromBackupClicked);
    connect(ui->CreateBackupButton, &QPushButton::clicked, this, &MainWindow::onCreateBackupClicked);
    connect(ui->DeleteBackupButton, &QPushButton::clicked, this, &MainWindow::onDeleteBackupClicked);
    connect(ui->actionOpenSettings, &QAction::triggered, this, &MainWindow::openSettings);
}

// **Toolbar Setup**
void MainWindow::setupToolBar() {
    if (!ui->mainToolBar) return;

    ui->mainToolBar->setFloatable(false);
    ui->mainToolBar->setMovable(false);
    ui->mainToolBar->setVisible(true);
    ui->mainToolBar->setIconSize(Styling::TOOLBAR_ICON_SIZE);

    if (!ui->actionOpenSettings)
        ui->actionOpenSettings = new QAction(QIcon(BackupResources::SETTINGS_ICON_PATH), UIConfig::MENU_SETTINGS_LABEL, this);
    else
        ui->actionOpenSettings->setIcon(QIcon(BackupResources::SETTINGS_ICON_PATH));

    if (!ui->actionExit)
        ui->actionExit = new QAction(QIcon(BackupResources::EXIT_ICON_PATH), UIConfig::MENU_EXIT_LABEL, this);
    else
        ui->actionExit->setIcon(QIcon(BackupResources::EXIT_ICON_PATH));

    QAction *helpAction = new QAction(QIcon(BackupResources::HELP_ICON_PATH), "Help", this);
    QAction *aboutAction = new QAction(QIcon(BackupResources::ABOUT_ICON_PATH), "About", this);

    ui->mainToolBar->clear();

    ui->mainToolBar->addAction(ui->actionOpenSettings);
    ui->mainToolBar->addAction(helpAction);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QWidgetAction* spacerAction = new QWidgetAction(this);
    spacerAction->setDefaultWidget(spacer);
    ui->mainToolBar->addAction(spacerAction);

    ui->mainToolBar->addAction(aboutAction);
    ui->mainToolBar->addAction(ui->actionExit);

    const QList<QAction*> actionsList = ui->mainToolBar->actions();
    for (QAction* action : actionsList) {
        QWidget* widget = ui->mainToolBar->widgetForAction(action);
        if (widget) {
            widget->setCursor(Qt::PointingHandCursor);
        }
    }

    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitApplication);
    connect(helpAction, &QAction::triggered, this, &MainWindow::showHelpDialog);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAboutButtonClicked);
}

void MainWindow::setupToolbarActions() {
    if (!ui->actionOpenSettings)
        ui->actionOpenSettings = new QAction(UIConfig::MENU_SETTINGS_LABEL, this);
    ui->actionOpenSettings->setIcon(QIcon(BackupResources::SETTINGS_ICON_PATH));

    if (!ui->actionExit)
        ui->actionExit = new QAction(UIConfig::MENU_EXIT_LABEL, this);
    ui->actionExit->setIcon(QIcon(BackupResources::EXIT_ICON_PATH));
}

void MainWindow::addToolbarSpacer() {
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QWidgetAction* spacerAction = new QWidgetAction(this);
    spacerAction->setDefaultWidget(spacer);
    ui->mainToolBar->addAction(spacerAction);
}

void MainWindow::connectToolbarActions(QAction* helpAction, QAction* aboutAction) {
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitApplication);
    connect(helpAction, &QAction::triggered, this, &MainWindow::showHelpDialog);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAboutButtonClicked);
}

// **Mouse Event Handling (Refactored)**
void MainWindow::handleMouseEvent(QMouseEvent *event, bool isPress) {
    if (event->button() == Qt::LeftButton) {
        dragging = isPress;
        if (isPress) {
            lastMousePosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        }
        event->accept();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    handleMouseEvent(event, true);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - lastMousePosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    handleMouseEvent(event, false);
}

// **Backup Management**
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

// **Change Backup Destination**
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

    if (QMessageBox::question(this, WarningMessages::WARNING_CONFIRM_BACKUP_DELETION,
                              QString(WarningMessages::MESSAGE_CONFIRM_BACKUP_DELETION).arg(selectedPath),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        backupController->deleteBackup(selectedPath);
    }
}

// **File and Directory Monitoring**
void MainWindow::startWatchingBackupDirectory(const QString &path) {
    fileWatcher->startWatching(path);
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

// **UI Updates**
void MainWindow::updateBackupStatusLabel(const QString &statusColor) {
    QPixmap pixmap = Utils::UI::createStatusLightPixmap(statusColor, ProgressConfig::STATUS_LIGHT_SIZE);

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    QString pixmapHtml = QString(Styling::STATUS_LIGHT_ICON_TEMPLATE)
                             .arg(QString::fromUtf8(ba.toBase64()));

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

// **Last Backup Info Update**
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

// **Close Event Handling**
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

// **UI Setup**
void MainWindow::setupCustomTitleBar() {
    titleBar = new QWidget(this);
    titleBar->setFixedHeight(UIConfig::TITLE_BAR_HEIGHT);
    titleBar->setStyleSheet(Styling::TITLE_BAR_STYLESHEET);

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(5, 0, 5, 0);

    titleLabel = new QLabel(AppInfo::APP_DISPLAY_TITLE, titleBar);
    titleLabel->setStyleSheet(Styling::TITLE_LABEL_STYLESHEET);

    minimizeButton = new QPushButton(Styling::WINDOW_MINIMIZE_BUTTON_LABEL, titleBar);
    closeButton = new QPushButton(Styling::WINDOW_CLOSE_BUTTON_LABEL, titleBar);

    minimizeButton->setToolTip("Minimize Window");
    closeButton->setToolTip("Close Application");

    minimizeButton->setFixedSize(30, 25);
    closeButton->setFixedSize(30, 25);

    minimizeButton->setStyleSheet(Styling::MINIMIZE_BUTTON_STYLESHEET);
    closeButton->setStyleSheet(Styling::CLOSE_BUTTON_STYLESHEET);

    minimizeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setCursor(Qt::PointingHandCursor);

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(minimizeButton);
    titleLayout->addWidget(closeButton);

    QWidget *mainContainer = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(titleBar);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    if (ui->mainToolBar) {
        ui->mainToolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        contentLayout->addWidget(ui->mainToolBar);
    }

    contentLayout->addWidget(ui->centralwidget);
    mainLayout->addLayout(contentLayout);

    setCentralWidget(mainContainer);

    connect(minimizeButton, &QPushButton::clicked, this, &MainWindow::showMinimized);
    connect(closeButton, &QPushButton::clicked, this, &MainWindow::close);
}

void MainWindow::setupDestinationView() {
    destinationModel->setFilter(BackupInfo::FILE_SYSTEM_FILTER);
    destinationModel->sort(0, Qt::DescendingOrder);
    ui->BackupDestinationView->setModel(destinationModel);
    ui->BackupDestinationView->setRootIndex(destinationModel->setRootPath(UserConfig::DEFAULT_BACKUP_DIRECTORY));

    removeAllColumnsFromTreeView(ui->BackupDestinationView);
}

void MainWindow::setupBackupStagingTreeView() {
    ui->BackupStagingTreeView->setModel(stagingModel);
    ui->BackupStagingTreeView->header()->setVisible(true);
    ui->BackupStagingTreeView->header()->setStretchLastSection(true);
    ui->BackupStagingTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->BackupStagingTreeView);
}

// **Fix for Undefined References**
void MainWindow::openSettings() {
    SettingsDialog settingsDialog(this);
    settingsDialog.exec();
}

void MainWindow::exitApplication() {
    QApplication::quit();
}

void MainWindow::showHelpDialog() {
    QMessageBox::information(this, HelpInfo::HELP_WINDOW_TITLE, HelpInfo::HELP_WINDOW_MESSAGE);
}

void MainWindow::onAboutButtonClicked() {
    QMessageBox::information(this, AboutInfo::ABOUT_WINDOW_TITLE, QString(AboutInfo::ABOUT_WINDOW_MESSAGE).arg(AppInfo::APP_VERSION, AppInfo::APP_DISPLAY_TITLE));
}

void MainWindow::connectBackupSignals() {
    connect(backupController, &BackupController::backupCreated, this, &MainWindow::refreshBackupStatus);
    connect(backupController, &BackupController::backupDeleted, this, &MainWindow::refreshBackupStatus);
    connect(backupController, &BackupController::errorOccurred, this, [this](const QString &error) {
        QMessageBox::critical(this, ErrorMessages::BACKUP_DELETION_ERROR_TITLE, error);
    });
}

// **Setup Source Tree View**
void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath(BackupInfo::DEFAULT_ROOT_PATH);
    sourceModel->setFilter(BackupInfo::FILE_SYSTEM_FILTER);
    ui->DriveTreeView->setModel(sourceModel);
    ui->DriveTreeView->setRootIndex(sourceModel->index(BackupInfo::DEFAULT_ROOT_PATH));
    ui->DriveTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    removeAllColumnsFromTreeView(ui->DriveTreeView);
}

// **Remove Unnecessary Columns from QTreeView**
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
