// Project includes
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindowconstants.h"
#include "mainwindowlabels.h"
#include "mainwindowmessages.h"
#include "mainwindowstyling.h"
#include "../systemtraycontextmenu.h"
#include "../settingsdialog/settingsdialog.h"
#include "../../ui/notificationsdialog/notificationsdialog.h"
#include "../../ui/templatedialog/templatedialog.h"
#include "../../ui/scheduledialog/scheduledialog.h"
#include "../../ui/promptdialog/promptdialog.h"
#include "../../backup_module/constants/backupconstants.h"
#include "../../backup_module/models/destinationproxymodel.h"
#include "../../backup_module/models/stagingmodel.h"
#include "../../backup_module/controller/backupcontroller.h"
#include "../../backup_module/service/fileoperations.h"
#include "../../backup_module/service/stagingutils.h"
#include "../../services/ServiceDirector/ServiceDirector.h"
#include "../../services/ServiceManagers/FilewatcherServiceManager/FilewatcherServiceManager.h"
#include "../../services/ServiceManagers/FormatUtilsServiceManager/FormatUtilsServiceManager.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"
#include "../../services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.h"
#include "../../services/ServiceManagers/ToolbarServiceManager/ToolbarServiceManager.h"
#include "../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h"
#include "../../services/ServiceManagers/UninstallServiceManager/UninstallServiceManager.h"
#include "../../services/ServiceManagers/UserServiceManager/UserServiceManager.h"
#include "../../services/ServiceManagers/UserServiceManager/UserServiceConstants.h"
#include "../../services/ServiceManagers/TemplateServiceManager/TemplateServiceManager.h"
#include "../../services/ServiceManagers/ScheduleServiceManager/ScheduleServiceManager.h"
#include "../../../../services/ServiceManagers/BackupServiceManager/BackupServiceManager.h"
#include "../../../../constants/interface_config.h"
#include "../../../../constants/window_config.h"

// Qt includes
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QAction>
#include <QCloseEvent>
#include <QColor>
#include <QCursor>
#include <QDateTime>
#include <QDialog>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QItemSelectionModel>
#include <QJsonObject>
#include <QLabel>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QModelIndex>
#include <QPair>
#include <QPalette>
#include <QProcess>
#include <QPushButton>
#include <QScreen>
#include <QSizePolicy>
#include <QSystemTrayIcon>
#include <QTabWidget>
#include <QTimer>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

// C++ includes
#include <array>

// Initialization
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    templateServiceManager(PathServiceManager::appConfigFolderPath() + "/" + App::Items::k_APPDATA_SETUP_USER_TEMPLATES_FILE, this) {
    ui->setupUi(this);
    initializeModels();
    initializeServices();
    initializeUiAndLayout();
    initializeToolbar();
    initializeBackupUi();
    initializeThemeHandling();
    setupTrayIcon();
}

// Destruction
MainWindow::~MainWindow() {
    delete ui;
}

// Initialization: models
void MainWindow::initializeModels() {
    sourceModel = new QFileSystemModel(this);
    destinationModel = new QFileSystemModel(this);
    stagingModel = new StagingModel(this);
}

// Initialization: services
void MainWindow::initializeServices() {
    fileWatcher = new FileWatcher(this);
    createBackupCooldownTimer = new QTimer(this);
    toolBar = new QToolBar(this);
    toolbarManager = new ToolbarServiceManager(this);

    const QString savedBackupDir =
        ServiceDirector::getInstance().getBackupServiceManager()->getBackupDirectory();
    PathServiceManager::setBackupDirectory(savedBackupDir);
    backupService = new BackupService(savedBackupDir);
    backupController = new BackupController(backupService, this);

    scheduleService_ = new ScheduleServiceManager(this);

    connect(scheduleService_, &ScheduleServiceManager::fired, this, [this]() {
        QTimer::singleShot(k_SCHEDULE_FIRE_DELAY_MS, this, [this]() {
            onCreateBackupClicked();
            PromptDialog::showDialog(
                this,
                PromptDialog::Information,
                tr("Scheduled Task"),
                tr("SCHEDULE MET"),
                QString(),
                PromptDialog::Ok,
                PromptDialog::Ok);
        });
    });

    connect(scheduleService_, &ScheduleServiceManager::configChanged, this, [this](const ScheduleServiceManager::Config& cfg) {
        if (!openScheduleDialog_) return;

        ScheduleDialog::ScheduleConfig dlgCfg;
        dlgCfg.enabled = cfg.enabled;
        dlgCfg.when    = cfg.when;
        switch (cfg.recur) {
        case ScheduleServiceManager::Recurrence::Once:
            dlgCfg.recur = ScheduleDialog::Recurrence::Once;    break;
        case ScheduleServiceManager::Recurrence::Daily:
            dlgCfg.recur = ScheduleDialog::Recurrence::Daily;   break;
        case ScheduleServiceManager::Recurrence::Weekly:
            dlgCfg.recur = ScheduleDialog::Recurrence::Weekly;  break;
        case ScheduleServiceManager::Recurrence::Monthly:
            dlgCfg.recur = ScheduleDialog::Recurrence::Monthly; break;
        }
        openScheduleDialog_->setConfig(dlgCfg);
    });
}

// Initialization: toolbar
void MainWindow::initializeToolbar() {
    addToolBar(Qt::LeftToolBarArea, toolBar);
    toolbarManager->initialize(toolBar);
}

// Initialization: UI and layout
void MainWindow::initializeUiAndLayout() {
    configureWindow();
    setupLayout();
    initializeUI();
    applyButtonCursors();
    updateApplicationStatusLabel();
}

// Layout: central container
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

// Wiring: UI signal/slot connections
void MainWindow::setupConnections() {
    connectBackupSignals();

    const QList<QPair<QPushButton *, void (MainWindow::*)()>> connections = {
        {ui->AddToBackupButton, &MainWindow::onAddToBackupClicked},
        {ui->ChangeBackupDestinationButton, &MainWindow::onChangeBackupDestinationClicked},
        {ui->RemoveFromBackupButton, &MainWindow::onRemoveFromBackupClicked},
        {ui->CreateBackupButton, &MainWindow::onCreateBackupClicked},
        {ui->DeleteBackupButton, &MainWindow::onDeleteBackupClicked},
        {ui->UnlockDriveButton, &MainWindow::onUnlockDriveClicked},
        {ui->TemplateButton, &MainWindow::onTemplateButtonClicked},
        {ui->ScheduleButton, &MainWindow::onScheduleButtonClicked}
    };

    for (const auto &[button, slot] : connections) {
        if (button) connect(button, &QPushButton::clicked, this, slot);
    }
}

// Wiring: backup controller signals
void MainWindow::connectBackupSignals() {
    connect(backupController, &BackupController::backupCreated, this, [this]() {
        onBackupCompleted();
    });

    connect(backupController, &BackupController::backupDeleted, this, &MainWindow::ensureBackupStatusUpdated);

    connect(backupController, &BackupController::errorOccurred, this, [this](const QString &error) {
        onBackupError(error);
        PromptDialog::showDialog(
            this,
            PromptDialog::Critical,
            ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
            error,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
    });
}

// Scheduling: open schedule dialog
void MainWindow::onScheduleButtonClicked() {
    ScheduleDialog dlg(this);

    openScheduleDialog_ = &dlg;
    connect(&dlg, &QDialog::finished, this, [this](int) { openScheduleDialog_.clear(); });

    {
        const auto cfg = scheduleService_->config();
        ScheduleDialog::ScheduleConfig dlgCfg;
        dlgCfg.enabled = cfg.enabled;
        dlgCfg.when    = cfg.when;
        switch (cfg.recur) {
        case ScheduleServiceManager::Recurrence::Once:
            dlgCfg.recur = ScheduleDialog::Recurrence::Once;    break;
        case ScheduleServiceManager::Recurrence::Daily:
            dlgCfg.recur = ScheduleDialog::Recurrence::Daily;   break;
        case ScheduleServiceManager::Recurrence::Weekly:
            dlgCfg.recur = ScheduleDialog::Recurrence::Weekly;  break;
        case ScheduleServiceManager::Recurrence::Monthly:
            dlgCfg.recur = ScheduleDialog::Recurrence::Monthly; break;
        }
        dlg.setConfig(dlgCfg);
    }

    connect(&dlg, &ScheduleDialog::scheduleSaved, this, [this](const ScheduleDialog::ScheduleConfig& dlgCfg) {
        ScheduleServiceManager::Config svcCfg;
        svcCfg.enabled = dlgCfg.enabled;
        svcCfg.when    = dlgCfg.when;
        switch (dlgCfg.recur) {
        case ScheduleDialog::Recurrence::Once:
            svcCfg.recur = ScheduleServiceManager::Recurrence::Once;    break;
        case ScheduleDialog::Recurrence::Daily:
            svcCfg.recur = ScheduleServiceManager::Recurrence::Daily;   break;
        case ScheduleDialog::Recurrence::Weekly:
            svcCfg.recur = ScheduleServiceManager::Recurrence::Weekly;  break;
        case ScheduleDialog::Recurrence::Monthly:
            svcCfg.recur = ScheduleServiceManager::Recurrence::Monthly; break;
        }

        scheduleService_->setConfig(svcCfg, true);
        PromptDialog::showDialog(
            this,
            PromptDialog::Information,
            tr("Schedule"),
            tr("Schedule saved."),
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
    });

    dlg.exec();
}

// UI: controls and staging
void MainWindow::initializeUI() {
    // Progress bar
    Shared::UI::setupProgressBar(
        ui->TransferProgressBar,
        UI::Progress::k_PROGRESS_BAR_MIN_VALUE,
        UI::Progress::k_PROGRESS_BAR_MAX_VALUE,
        UI::ProgressDetails::k_PROGRESS_BAR_HEIGHT,
        UI::Progress::k_PROGRESS_BAR_TEXT_VISIBLE);

    if (ui->TransferProgressBar->value() == 0) {
        ui->TransferProgressBar->setVisible(false);
        ui->TransferProgressText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TransferProgressText->setText(UI::Progress::k_PROGRESS_BAR_INITIAL_MESSAGE);
    }

    // Staging title row (label + reset button)
    stagingTitleLayout = new QHBoxLayout(ui->StagingListTitleContainer);
    stagingTitleLayout->setContentsMargins(0, 0, 0, 0);
    stagingTitleLayout->setSpacing(6);
    stagingTitleLayout->setAlignment(Qt::AlignLeft);

    stagingTitleLabel = new QLabel(Labels::Backup::k_STAGING_TITLE, ui->StagingListTitleContainer);
    stagingTitleLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    stagingTitleLayout->addWidget(stagingTitleLabel);

    // Tiny red reset button with an ✖
    templateResetButton = new QPushButton(QStringLiteral("✖"), ui->StagingListTitleContainer);

    // Apply official styling: sets cursor, tooltip, objectName = "TemplateResetButton", and stylesheet
    Shared::UI::UIUtilsServiceManager::applyTemplateResetButtonStyling(templateResetButton);

    // Keep it tiny regardless of layout/theme
    templateResetButton->setFixedSize(16, 16);
    templateResetButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Hidden until a template is selected
    templateResetButton->setVisible(false);

    connect(templateResetButton, &QPushButton::clicked, this, [this]() {
        stagingModel->clear();
        ui->BackupStagingTreeView->clearSelection();
        ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());
        updateBackupStagingTitle(QString());  // clears title & hides the reset button
    });

    stagingTitleLayout->addWidget(templateResetButton);

    // Rest of UI init
    initializeBackupSystem();
    setInitialButtonTexts();
}

// UI: default button texts
void MainWindow::setInitialButtonTexts() {
    ui->AddToBackupButton->setText(Labels::Backup::k_ADD_TO_BACKUP_ORIGINAL_TEXT);
    ui->RemoveFromBackupButton->setText(Labels::Backup::k_REMOVE_FROM_BACKUP_ORIGINAL_TEXT);
    ui->CreateBackupButton->setText(Labels::Backup::k_CREATE_BACKUP_ORIGINAL_TEXT);
    ui->ChangeBackupDestinationButton->setText(Labels::Backup::k_CHANGE_DESTINATION_ORIGINAL_TEXT);
    ui->DeleteBackupButton->setText(Labels::Backup::k_DELETE_BACKUP_ORIGINAL_TEXT);
    ui->NotificationButton->setText(Labels::Backup::k_NOTIFICATION_ORIGINAL_TEXT);
    ui->UnlockDriveButton->setText(Labels::Backup::k_UNLOCK_DRIVE_ORIGINAL_TEXT);
    ui->TemplateButton->setText(Labels::Backup::k_TEMPLATE_ORIGINAL_TEXT);
    ui->ScheduleButton->setText(Labels::Backup::k_SCHEDULE_ORIGINAL_TEXT);
}

// UI: cursors, tooltips, and styles
void MainWindow::applyButtonCursors() {
    const QList<QPair<QPushButton *, QString>> buttons = {
        {ui->AddToBackupButton,       k_ADD_TO_BACKUP},
        {ui->RemoveFromBackupButton,  k_REMOVE_FROM_BACKUP},
        {ui->CreateBackupButton,      k_CREATE_BACKUP},
        {ui->ChangeBackupDestinationButton, k_CHANGE_DESTINATION},
        {ui->DeleteBackupButton,      k_DELETE_BACKUP},
        {ui->NotificationButton,      k_NOTIFICATIONS},
        {ui->UnlockDriveButton,       k_UNLOCK_DRIVE},
        {ui->TemplateButton,          k_TEMPLATE},
        {ui->ScheduleButton,          k_SCHEDULE}
    };

    Shared::UI::applyButtonStyling(buttons);
}

// Theming: initialization
void MainWindow::initializeThemeHandling() {
    connect(&ThemeServiceManager::instance(), &ThemeServiceManager::themeChanged, this, &MainWindow::onThemeChanged);
}

// Theming: handle theme change
void MainWindow::onThemeChanged() {
    applyCustomPalettesToAllTreeViews();
}

// Tray: icon and menu
void MainWindow::setupTrayIcon() {
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(UI::Resources::k_ICON_PATH));
    trayIcon->setToolTip(App::Info::k_APP_NAME);

    auto* systemTrayContextMenu = new SystemTrayContextMenu(this);
    trayMenu = systemTrayContextMenu;

    QAction* actionOpen = systemTrayContextMenu->addAction(k_TRAY_ACTION_OPEN);
    QAction* actionSettings = systemTrayContextMenu->addAction(k_TRAY_ACTION_SETTINGS);
    systemTrayContextMenu->addSeparator();
    QAction* actionExit = systemTrayContextMenu->addAction(k_TRAY_ACTION_EXIT);

    connect(systemTrayContextMenu, &SystemTrayContextMenu::safeTriggered, this, [this](QAction* action) {
        const QString text = action->text().trimmed();
        if (text == k_TRAY_ACTION_OPEN) {
            this->showNormal();
            this->activateWindow();
        } else if (text == k_TRAY_ACTION_SETTINGS) {
            if (settingsDialog && settingsDialog->isVisible()) {
                settingsDialog->raise();
                settingsDialog->activateWindow();
                return;
            }

            settingsDialog = new SettingsDialog(this);
            connect(settingsDialog, &SettingsDialog::requestBackupReset, this, [this](const QString& path, const QString& type) {
                this->handleBackupDeletion(path, type);
            });
            connect(settingsDialog, &SettingsDialog::requestAppDataClear, this, [this]() {
                this->handleAppDataClear();
            });

            settingsDialog->exec();
            delete settingsDialog;
            settingsDialog = nullptr;
        } else if (text == k_TRAY_ACTION_EXIT) {
            QApplication::quit();
        }
    });

    trayIcon->setContextMenu(systemTrayContextMenu);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, [this, systemTrayContextMenu](QSystemTrayIcon::ActivationReason reason) {
        QPoint cursorPos = QCursor::pos();
        switch (reason) {
        case QSystemTrayIcon::Trigger:
            this->showNormal();
            this->activateWindow();
            break;
        case QSystemTrayIcon::Context:
            systemTrayContextMenu->popup(cursorPos);
            break;
        case QSystemTrayIcon::DoubleClick:
        default:
            break;
        }
    });
}

// File watcher: initialize
void MainWindow::initializeFileWatcher() {
    const QStringList roots = getWatchedRoots();
    fileWatcher->startWatchingMultiple(roots);

    connect(fileWatcher, &FileWatcher::fileChanged, this, &MainWindow::handleWatchedPathChanged);
    connect(fileWatcher, &FileWatcher::directoryChanged, this, &MainWindow::handleWatchedPathChanged);
}

// File watcher: refresh
void MainWindow::refreshFileWatcher() {
    fileWatcher->startWatchingMultiple(getWatchedRoots());
}

// File watcher: derive roots
QStringList MainWindow::getWatchedRoots() const {
    QStringList watchedRoots;

    const QString appConfigDir = PathServiceManager::appConfigFolderPath();
    const QString backupDataRoot = PathServiceManager::backupDataRootDir();
    const QString backupConfigDir = PathServiceManager::backupConfigFolderPath();

    if (QDir(appConfigDir).exists()) watchedRoots << appConfigDir;
    if (QDir(backupDataRoot).exists()) watchedRoots << backupDataRoot;
    if (QDir(backupConfigDir).exists()) watchedRoots << backupConfigDir;

    return watchedRoots;
}

// File watcher: path changed
void MainWindow::handleWatchedPathChanged(const QString &path) {
    fileWatcher->addPath(path);

    const QString appConfigDir = PathServiceManager::appConfigFolderPath();
    const QString backupDir = PathServiceManager::backupDataRootDir();

    if (path.startsWith(appConfigDir)) updateApplicationStatusLabel();
    if (path.startsWith(backupDir)) ensureBackupStatusUpdated();

    checkStagingForReadAccessLoss();
}

// File watcher: backup directory changed
void MainWindow::onBackupDirectoryChanged() {
    refreshFileWatcher();
    ensureBackupStatusUpdated();
}

// File watcher: drop unreadable staged items
void MainWindow::checkStagingForReadAccessLoss() {
    QStringList lostAccess;
    const QStringList &stagedPaths = stagingModel->getStagedPaths();

    for (const QString &path : stagedPaths) {
        QFileInfo info(path);
        if (!info.exists() || !info.isReadable()) lostAccess << path;
    }

    if (!lostAccess.isEmpty()) {
        for (const auto &path : std::as_const(lostAccess)) stagingModel->removePath(path);

        NotificationServiceManager::instance().addNotification(
            QString(NotificationMessages::k_READ_ACCESS_LOST_MESSAGE).arg(lostAccess.join("\n")));

        ui->BackupStagingTreeView->clearSelection();
        ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());
    }
}

// Window: geometry and placement
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

// Theming: apply palette to a QTreeView
void MainWindow::applyCustomTreePalette(QTreeView *treeView) {
    if (!treeView) return;

    QPalette palette = treeView->palette();
    const auto theme = ThemeServiceManager::instance().currentTheme();

    QColor highlight = (theme == ThemeServiceConstants::AppTheme::Dark)
                           ? QColor(MainWindowStyling::Styles::ThemeColors::DARK_HIGHLIGHT)
                           : QColor(MainWindowStyling::Styles::ThemeColors::LIGHT_HIGHLIGHT);

    QColor text = (theme == ThemeServiceConstants::AppTheme::Dark)
                      ? QColor(MainWindowStyling::Styles::ThemeColors::DARK_TEXT)
                      : QColor(MainWindowStyling::Styles::ThemeColors::LIGHT_TEXT);

    palette.setColor(QPalette::Highlight, highlight);
    palette.setColor(QPalette::HighlightedText, text);
    treeView->setPalette(palette);
}

// Theming: apply palettes to all tree views
void MainWindow::applyCustomPalettesToAllTreeViews() {
    std::array<QTreeView *, 3> trees = { ui->DriveTreeView, ui->BackupStagingTreeView, ui->BackupDestinationView };
    for (QTreeView *tree : trees) applyCustomTreePalette(tree);
}

// Tree view: generic configuration
void MainWindow::configureTreeView(QTreeView *treeView, QAbstractItemModel *model,
                                   QAbstractItemView::SelectionMode selectionMode,
                                   bool stretchLastColumn, bool showHeader) {
    if (!treeView || !model) return;

    treeView->setModel(model);
    treeView->setSelectionMode(selectionMode);
    treeView->header()->setVisible(showHeader);
    treeView->header()->setStretchLastSection(stretchLastColumn);

    for (int i = UI::TreeView::k_START_HIDDEN_COLUMN; i < UI::TreeView::k_DEFAULT_COLUMN_COUNT; ++i) {
        treeView->setColumnHidden(i, true);
    }
}

// Tree view: source (drive)
void MainWindow::setupSourceTreeView() {
    sourceModel->setRootPath("");
    sourceModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
    configureTreeView(ui->DriveTreeView, sourceModel, QAbstractItemView::ExtendedSelection, true);
    ui->DriveTreeView->setRootIndex(sourceModel->index(""));
    ui->DriveTreeView->clearSelection();
    ui->DriveTreeView->setCurrentIndex(QModelIndex());
}

// Tree view: staging
void MainWindow::setupBackupStagingTreeView() {
    configureTreeView(ui->BackupStagingTreeView, stagingModel, QAbstractItemView::ExtendedSelection, true);
}

// Tree view: destination (overload)
void MainWindow::setupDestinationView() {
    setupDestinationView(backupService->getBackupRoot());
}

// Tree view: destination with root
void MainWindow::setupDestinationView(const QString &backupDir) {
    destinationModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    destinationModel->setNameFilters(QStringList() << "*");
    destinationModel->setNameFilterDisables(false);

    delete destinationProxyModel;
    destinationProxyModel = new DestinationProxyModel(this);
    destinationProxyModel->setSourceModel(destinationModel);
    destinationProxyModel->setExcludedFolderName(App::Items::k_BACKUP_SETUP_CONFIG_FOLDER);

    QModelIndex sourceRootIndex = destinationModel->setRootPath(backupDir);
    QModelIndex proxyRootIndex = destinationProxyModel->mapFromSource(sourceRootIndex);

    configureTreeView(ui->BackupDestinationView, destinationProxyModel, QAbstractItemView::SingleSelection, true);
    ui->BackupDestinationView->setModel(destinationProxyModel);
    ui->BackupDestinationView->setRootIndex(proxyRootIndex);
    applyCustomTreePalette(ui->BackupDestinationView);
    destinationProxyModel->sort(0);
}

// Backup UI: initialize
void MainWindow::initializeBackupUi() {
    initializeBackupSystem();
    setupConnections();
    setupNotificationButton();
    initializeFileWatcher();

    createBackupCooldownTimer->setSingleShot(true);
    connect(createBackupCooldownTimer, &QTimer::timeout, this, [this]() {
        ui->CreateBackupButton->setEnabled(true);
    });
}

// Backup system: initialize
void MainWindow::initializeBackupSystem() {
    const QString savedBackupDir = ServiceDirector::getInstance().getBackupServiceManager()->getBackupDirectory();
    PathServiceManager::setBackupDirectory(savedBackupDir);
    backupService->setBackupRoot(savedBackupDir);

    if (!FileOperations::createDirectory(savedBackupDir)) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Critical,
            ErrorMessages::k_BACKUP_INITIALIZATION_FAILED_TITLE,
            ErrorMessages::k_ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
    }

    setupDestinationView(savedBackupDir);
    setupSourceTreeView();
    setupBackupStagingTreeView();
    ensureBackupStatusUpdated();
    applyCustomPalettesToAllTreeViews();
    ui->BackupViewContainer->setStyleSheet(MainWindowStyling::Styles::BackupViewContainer::STYLE);
}

// Backup status: serialized refresh
void MainWindow::ensureBackupStatusUpdated() {
    static bool isRefreshing = false;
    if (isRefreshing) return;
    isRefreshing = true;
    refreshBackupStatus();
    isRefreshing = false;
}

// Backup status: re-scan and update
void MainWindow::refreshBackupStatus() {
    latestBackupScan = backupService->scanForBackupStatus();
    updateBackupLabels();

    if (!latestBackupScan.isBroken() && orphanLogNotified) {
        orphanLogNotified = false;
    }

    notifyOrphanOrBrokenBackupIssues(latestBackupScan);
}

// Backup status: update labels
void MainWindow::updateBackupLabels() {
    updateBackupMetadataLabels();
    updateLastBackupInfo();
    handleSpecialBackupLabelStates(latestBackupScan);

    const QString statusColor = !latestBackupScan.structureExists
                                    ? MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_NOT_FOUND
                                    : latestBackupScan.isBroken()
                                          ? MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_WARNING
                                          : MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_FOUND;

    updateBackupStatusLabel(statusColor);
}

// Backup status: status label styling
void MainWindow::updateBackupStatusLabel(const QString &statusColor) {
    const auto [emoji, text] = statusVisualsForColor(statusColor);
    QString style;

    if (statusColor == MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_WARNING ||
        statusColor == MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_NOT_FOUND) {
        style = MainWindowStyling::Styles::LabelStyles::k_WARNING_LABEL_STYLE;
    } else {
        style = MainWindowStyling::Styles::GeneralText::k_DEFAULT_STYLE;
    }

    setStatusLabel(ui->BackupStatusLabel, emoji, text, style);

    for (QLabel *label : { ui->LastBackupNameLabel, ui->LastBackupTimestampLabel, ui->LastBackupDurationLabel, ui->LastBackupSizeLabel }) {
        label->setVisible(true);
    }
}

// Backup status: map color to emoji/text
QPair<QString, QString> MainWindow::statusVisualsForColor(const QString &color) const {
    static const QMap<QString, QPair<QString, QString>> statusMap = {
        {MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_FOUND,
         {Labels::Emoji::k_GREEN, Labels::Backup::k_READY_LABEL}},
        {MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_WARNING,
         {Labels::Emoji::k_YELLOW, Labels::Backup::k_WARNING_LABEL}},
        {MainWindowStyling::Styles::Visuals::BACKUP_STATUS_COLOR_NOT_FOUND,
         {Labels::Emoji::k_RED, Labels::Backup::k_NOT_INITIALIZED}}
    };

    return statusMap.value(color, {Labels::Emoji::k_RED, Labels::Backup::k_NOT_INITIALIZED});
}

// Backup status: apply text/style
void MainWindow::setStatusLabel(QLabel *label, const QString &emoji, const QString &text, const QString &style) {
    if (!label) return;
    label->setText(Labels::Backup::k_STATUS_LABEL.arg(emoji, text));
    label->setTextFormat(Qt::RichText);
    label->setStyleSheet(style);
}

// Backup status: metadata labels
void MainWindow::updateBackupMetadataLabels() {
    updateBackupLocationLabel(backupService->getBackupRoot());
    updateBackupTotalCountLabel();
    updateBackupTotalSizeLabel();
    updateBackupLocationStatusLabel(backupService->getBackupRoot());
}

// Backup status: last-backup info
void MainWindow::updateLastBackupInfo() {
    const QJsonObject metadata = backupService->getLastBackupMetadata();

    if (metadata.isEmpty()) {
        ui->LastBackupNameLabel->setText(Labels::LastBackup::k_NO_BACKUPS_MESSAGE);
        ui->LastBackupTimestampLabel->clear();
        ui->LastBackupDurationLabel->clear();
        ui->LastBackupSizeLabel->clear();
        return;
    }

    const QString name = metadata.value(BackupMetadata::k_NAME).toString();
    const QString timestampRaw = metadata.value(BackupMetadata::k_TIMESTAMP).toString();
    const int durationSec = metadata.value(BackupMetadata::k_DURATION).toInt();
    const QString sizeStr = metadata.value(BackupMetadata::k_SIZE_READABLE).toString();

    const QString formattedTimestamp = Shared::Formatting::formatTimestamp(
        QDateTime::fromString(timestampRaw, Qt::ISODate),
        Backup::Timestamps::k_BACKUP_TIMESTAMP_DISPLAY_FORMAT);

    const QString formattedDuration = Shared::Formatting::formatDuration(durationSec);

    ui->LastBackupNameLabel->setText(Labels::LastBackup::k_NAME + name);
    ui->LastBackupTimestampLabel->setText(Labels::LastBackup::k_TIMESTAMP + formattedTimestamp);
    ui->LastBackupDurationLabel->setText(Labels::LastBackup::k_DURATION + formattedDuration);
    ui->LastBackupSizeLabel->setText(Labels::LastBackup::k_SIZE + sizeStr);
}

// Backup status: location label
void MainWindow::updateBackupLocationLabel(const QString &location) {
    QString displayText = location;

    if (location.length() > k_BACKUP_LOCATION_MAXCHARS) {
        int half = k_BACKUP_LOCATION_MAXCHARS / 2;
        displayText = location.left(half) + Labels::General::k_ELLIPSIS + location.right(half);
    }

    ui->BackupLocationLabel->setText(displayText);
    ui->BackupLocationLabel->setToolTip(location);
}

// Backup status: totals
void MainWindow::updateBackupTotalCountLabel() {
    ui->BackupTotalCountLabel->setText(Labels::Backup::k_TOTAL_COUNT + QString::number(backupService->getBackupCount()));
}

void MainWindow::updateBackupTotalSizeLabel() {
    ui->BackupTotalSizeLabel->setText(Labels::Backup::k_TOTAL_SIZE + Shared::Formatting::formatSize(backupService->getTotalBackupSize()));
}

// Backup status: location access
void MainWindow::updateBackupLocationStatusLabel(const QString &location) {
    QFileInfo dirInfo(location);
    const QString status = dirInfo.exists()
                               ? (dirInfo.isWritable() ? Labels::DirectoryStatus::k_WRITABLE
                                                       : Labels::DirectoryStatus::k_READ_ONLY)
                               : Labels::DirectoryStatus::k_UNKNOWN;

    ui->BackupLocationStatusLabel->setText(Labels::Backup::k_LOCATION_ACCESS + status);
}

// Backup status: special states
void MainWindow::handleSpecialBackupLabelStates(const BackupScanResult &scan) {
    const int backupCount = backupService->getBackupCount();
    const QString rootPath = backupService->getBackupRoot();
    const QFileInfo rootInfo(rootPath);
    const QDir backupDir(rootPath);
    const bool locationInitialized = rootInfo.exists();
    const bool backupDirEmpty = locationInitialized &&
                                backupDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).isEmpty();
    const bool hasAppIssue = !scan.validAppStructure;

    const bool isUninitialized =
        !scan.structureExists && !scan.validBackupStructure &&
        !scan.hasMissingLogs && !scan.hasOrphanedLogs && backupCount == 0;

    const bool hasBackupIssue =
        !isUninitialized && (!scan.validBackupStructure || scan.hasMissingLogs || scan.hasOrphanedLogs);

    const QString infoLine = Labels::Backup::k_SEE_NOTIFICATIONS_LABEL;

    if (hasAppIssue && hasBackupIssue) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_WARNING_SUMMARY_LABEL);
        ui->BackupTotalSizeLabel->setText(Labels::Backup::k_APP_AND_BACKUP_ISSUE_LABEL);
        ui->BackupLocationStatusLabel->setText(infoLine);
    } else if (hasBackupIssue) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_WARNING_SUMMARY_LABEL);
        ui->BackupTotalSizeLabel->setText(Labels::Backup::k_BACKUP_ISSUE_LABEL);
        ui->BackupLocationStatusLabel->setText(infoLine);
    } else if (hasAppIssue) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_WARNING_SUMMARY_LABEL);
        ui->BackupTotalSizeLabel->setText(Labels::Backup::k_APP_ISSUE_LABEL);
        ui->BackupLocationStatusLabel->setText(infoLine);
    } else if (isUninitialized || (backupCount == 0 && (backupDirEmpty || !locationInitialized))) {
        ui->BackupTotalCountLabel->setText(Labels::Backup::k_NO_BACKUPS_COUNT_LABEL);
        ui->BackupTotalCountLabel->setStyleSheet(MainWindowStyling::Styles::LabelStyles::k_WARNING_LABEL_STYLE);
        ui->BackupTotalSizeLabel->setText("");
        ui->BackupLocationStatusLabel->setText("");
        ui->BackupTotalSizeLabel->hide();
        ui->BackupLocationStatusLabel->hide();
        return;
    } else {
        ui->BackupTotalCountLabel->setStyleSheet(MainWindowStyling::Styles::GeneralText::k_DEFAULT_STYLE);
        ui->BackupTotalSizeLabel->setStyleSheet(MainWindowStyling::Styles::GeneralText::k_DEFAULT_STYLE);
        ui->BackupLocationStatusLabel->setStyleSheet(MainWindowStyling::Styles::GeneralText::k_DEFAULT_STYLE);
        updateBackupTotalCountLabel();
        updateBackupTotalSizeLabel();
        updateBackupLocationStatusLabel(backupService->getBackupRoot());
        ui->BackupTotalSizeLabel->show();
        ui->BackupLocationStatusLabel->show();
        return;
    }

    ui->BackupTotalCountLabel->setStyleSheet(MainWindowStyling::Styles::LabelStyles::k_WARNING_LABEL_STYLE);
    ui->BackupTotalSizeLabel->setStyleSheet(MainWindowStyling::Styles::LabelStyles::k_WARNING_LABEL_STYLE);
    ui->BackupLocationStatusLabel->setStyleSheet(MainWindowStyling::Styles::LabelStyles::k_WARNING_LABEL_STYLE);
    ui->BackupTotalCountLabel->show();
    ui->BackupTotalSizeLabel->show();
    ui->BackupLocationStatusLabel->show();
}

// Backup status: notifications for issues
void MainWindow::notifyOrphanOrBrokenBackupIssues(const BackupScanResult &scan) {
    static bool notifiedBackupStructure = false;
    const int backupCount = backupService->getBackupCount();

    const bool isUninitialized =
        !scan.structureExists && !scan.validBackupStructure &&
        !scan.hasMissingLogs && !scan.hasOrphanedLogs && backupCount == 0;

    if (isUninitialized) {
        notifiedBackupStructure = false;
        return;
    }

    if (scan.hasOrphanedLogs && !notifiedBackupStructure) {
        NotificationServiceManager::instance().addNotification(NotificationMessages::k_ORPHANED_LOGS_MESSAGE);
        notifiedBackupStructure = true;
    }

    if (scan.hasMissingLogs && !notifiedBackupStructure) {
        NotificationServiceManager::instance().addNotification(NotificationMessages::k_MISSING_LOGS_MESSAGE);
        notifiedBackupStructure = true;
    }

    if (!scan.validBackupStructure && !notifiedBackupStructure) {
        NotificationServiceManager::instance().addNotification(NotificationMessages::k_BROKEN_BACKUP_STRUCTURE_MESSAGE);
        notifiedBackupStructure = true;
    }

    if (scan.validBackupStructure && !scan.hasOrphanedLogs && !scan.hasMissingLogs) {
        notifiedBackupStructure = false;
    }
}

// Backup status: revalidate app and backup
void MainWindow::revalidateBackupAndAppStatus() {
    latestBackupScan = backupService->scanForBackupStatus();
    const QString appStatus = checkInstallIntegrityStatus();
    latestBackupScan.validAppStructure = (appStatus == InfoMessages::k_INSTALL_OK);
    updateBackupLabels();
    notifyOrphanOrBrokenBackupIssues(latestBackupScan);
}

// Backup status: revalidate with provided app status
void MainWindow::revalidateBackupAndAppStatus(const QString &appStatus) {
    latestBackupScan = backupService->scanForBackupStatus();
    latestBackupScan.validAppStructure = (appStatus == InfoMessages::k_INSTALL_OK);
    updateBackupLabels();
    notifyOrphanOrBrokenBackupIssues(latestBackupScan);
}

// Actions: add to staging
void MainWindow::onAddToBackupClicked() {
    const QModelIndexList selectedIndexes = ui->DriveTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            ErrorMessages::k_BACKUP_SELECTION_REQUIRED_TITLE,
            ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_BACKUP,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    QStringList alreadyStaged, toStage, notReadable;

    for (const QModelIndex &index : selectedIndexes) {
        if (!index.isValid() || index.column() != 0) continue;

        const QString path = sourceModel->filePath(index);
        QFileInfo fileInfo(path);

        if (!fileInfo.isReadable()) {
            notReadable << path;
        } else if (stagingModel->containsPath(path)) {
            alreadyStaged << path;
        } else {
            toStage << path;
        }
    }

    if (!notReadable.isEmpty()) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            ErrorMessages::k_READ_ACCESS_DENIED_TITLE,
            ErrorMessages::k_READ_ACCESS_DENIED_BODY.arg(notReadable.join("\n")),
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
    }

    if (!alreadyStaged.isEmpty()) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            ErrorMessages::k_ALREADY_STAGED_TITLE,
            ErrorMessages::k_ERROR_ALREADY_IN_STAGING.arg(alreadyStaged.join("\n")),
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
    }

    if (!toStage.isEmpty()) {
        stagingModel->addPaths(toStage);
        templateServiceManager.setCurrentStagingEntries(stagingModel->getStagedPaths());
        ui->BackupStagingTreeView->clearSelection();
        ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());
    }
}

// Actions: remove from staging
void MainWindow::onRemoveFromBackupClicked() {
    const QModelIndexList selectedIndexes = ui->BackupStagingTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            ErrorMessages::k_REMOVE_SELECTION_REQUIRED_TITLE,
            ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    Shared::Backup::removeSelectedPathsFromStaging(ui->BackupStagingTreeView, stagingModel);
    templateServiceManager.setCurrentStagingEntries(stagingModel->getStagedPaths());
}

// Actions: change backup destination
void MainWindow::onChangeBackupDestinationClicked() {
    const QString selectedDir = QFileDialog::getExistingDirectory(
        this, InfoMessages::k_SELECT_BACKUP_DESTINATION_TITLE, QDir::rootPath());

    if (selectedDir.isEmpty()) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            ErrorMessages::k_BACKUP_LOCATION_REQUIRED_TITLE,
            ErrorMessages::k_ERROR_NO_BACKUP_LOCATION_PATH_SELECTED,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    if (!FileOperations::createDirectory(selectedDir)) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Critical,
            ErrorMessages::k_BACKUP_DIRECTORY_ERROR_TITLE,
            ErrorMessages::k_ERROR_CREATING_BACKUP_DIRECTORY,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    backupService->setBackupRoot(selectedDir);
    ServiceDirector::getInstance().getBackupServiceManager()->setBackupDirectory(selectedDir);
    PathServiceManager::setBackupDirectory(selectedDir);

    setupDestinationView(selectedDir);
    ensureBackupStatusUpdated();
    refreshFileWatcher();
}

// Actions: create backup
void MainWindow::onCreateBackupClicked() {
    const QStringList pathsToBackup = stagingModel->getStagedPaths();
    if (pathsToBackup.isEmpty()) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            ErrorMessages::k_NO_ITEMS_STAGED_FOR_BACKUP_TITLE,
            ErrorMessages::k_ERROR_NO_ITEMS_STAGED_FOR_BACKUP,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    QStringList unreadablePaths;
    for (const auto &path : pathsToBackup) {
        QFileInfo fileInfo(path);
        if (!fileInfo.exists() || !fileInfo.isReadable()) unreadablePaths << path;
    }

    if (!unreadablePaths.isEmpty()) {
        const QStringList pathsToRemove = unreadablePaths;
        for (const auto &path : pathsToRemove) stagingModel->removePath(path);

        NotificationServiceManager::instance().addNotification(
            QString(NotificationMessages::k_READ_ACCESS_LOST_MESSAGE).arg(unreadablePaths.join("\n")));

        ui->BackupStagingTreeView->clearSelection();
        ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());

        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            ErrorMessages::k_READ_ACCESS_DENIED_TITLE,
            ErrorMessages::k_READ_ACCESS_DENIED_BODY.arg(unreadablePaths.join("\n")),
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    const QString backupRoot = destinationModel->rootPath();
    QString errorMessage;
    if (!FileOperations::createBackupInfrastructure(backupRoot, errorMessage)) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Critical,
            ErrorMessages::k_ERROR_BACKUP_ALREADY_IN_PROGRESS,
            errorMessage,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    backupStartTimer.start();

    ui->TransferProgressBar->setValue(UI::Progress::k_PROGRESS_BAR_MIN_VALUE);
    ui->TransferProgressBar->setVisible(true);
    ui->TransferProgressText->setVisible(false);

    backupController->createBackup(backupRoot, pathsToBackup, ui->TransferProgressBar);
}

// Actions: delete backup
void MainWindow::onDeleteBackupClicked() {
    if (!destinationModel) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Critical,
            ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
            ErrorMessages::k_ERROR_DESTINATION_MODEL_NULL,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    const QModelIndex selectedIndex = ui->BackupDestinationView->currentIndex();
    if (!selectedIndex.isValid()) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
            ErrorMessages::k_ERROR_BACKUP_DELETE_FAILED,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    auto fsModel = qobject_cast<QFileSystemModel *>(destinationModel);
    if (!fsModel) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Critical,
            ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
            ErrorMessages::k_ERROR_MODEL_TYPE_INVALID,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    const QString selectedPath = fsModel->filePath(destinationProxyModel->mapToSource(selectedIndex));

    if (selectedPath.isEmpty()) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            ErrorMessages::k_BACKUP_DELETION_ERROR_TITLE,
            ErrorMessages::k_ERROR_SELECTED_PATH_INVALID,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    const auto confirm = PromptDialog::showDialog(
        this,
        PromptDialog::Question,
        WarningMessages::k_WARNING_CONFIRM_BACKUP_DELETION,
        QString(WarningMessages::k_MESSAGE_CONFIRM_BACKUP_DELETION).arg(selectedPath),
        QString(),
        PromptDialog::Yes | PromptDialog::No,
        PromptDialog::No);
    if (confirm != PromptDialog::Yes) return;

    ui->BackupDestinationView->setModel(nullptr);
    resetDestinationModels();

    handleBackupDeletion(selectedPath, "single");
}

// Actions: backup error
void MainWindow::onBackupError(const QString &error) {
    Q_UNUSED(error);
    ui->TransferProgressText->setText(UI::Progress::k_PROGRESS_BAR_INITIAL_MESSAGE);
    ui->TransferProgressText->setVisible(true);
    ui->CreateBackupButton->setEnabled(true);
}

// Actions: backup completed
void MainWindow::onBackupCompleted() {
    ui->TransferProgressText->setText(UI::Progress::k_PROGRESS_BAR_COMPLETION_MESSAGE);
    ui->TransferProgressText->setVisible(true);
    ui->CreateBackupButton->setEnabled(true);

    QTimer::singleShot(System::Timing::k_BUTTON_FEEDBACK_DURATION_MS, this, [this]() {
        ui->TransferProgressText->setText(UI::Progress::k_PROGRESS_BAR_INITIAL_MESSAGE);
    });

    ensureBackupStatusUpdated();
    refreshFileWatcher();

    const QString backupRoot = backupService->getBackupRoot();
    QDir dir(backupRoot);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Time | QDir::Reversed);

    const QFileInfoList entries = dir.entryInfoList();
    if (!entries.isEmpty()) {
        const QString newestFolderPath = entries.last().absoluteFilePath();
        QModelIndex sourceIndex = destinationModel->index(newestFolderPath);
        QModelIndex proxyIndex = destinationProxyModel->mapFromSource(sourceIndex);

        if (proxyIndex.isValid()) {
            ui->BackupDestinationView->setCurrentIndex(proxyIndex);
            ui->BackupDestinationView->selectionModel()->select(
                proxyIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            ui->BackupDestinationView->scrollTo(proxyIndex);
        }
    }

    ui->BackupDestinationView->clearSelection();
    ui->BackupDestinationView->setCurrentIndex(QModelIndex());
}

// Actions: cooldown finished
void MainWindow::onCooldownFinished() {
    ui->CreateBackupButton->setEnabled(true);
}

// UI helper: button feedback
void MainWindow::triggerButtonFeedback(QPushButton *button,
                                       const QString &feedbackText,
                                       const QString &originalText,
                                       int durationMs) {
    if (!button) return;

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

// Template: open dialog
void MainWindow::onTemplateButtonClicked() {
    auto* dialog = new TemplateDialog(&templateServiceManager, this);

    connect(dialog, &TemplateDialog::templateLoaded, this, [this](const QStringList& paths, const QString& name) {
        stagingModel->clear();
        stagingModel->addPaths(paths);
        ui->BackupStagingTreeView->clearSelection();
        ui->BackupStagingTreeView->setCurrentIndex(QModelIndex());
        updateBackupStagingTitle(name);
        templateServiceManager.setCurrentStagingEntries(paths);
    });

    dialog->exec();
    dialog->deleteLater();
}

// Template: update title and reset button
void MainWindow::updateBackupStagingTitle(const QString &templateName) {
    if (templateName.isEmpty()) {
        stagingTitleLabel->setText(Labels::Backup::k_STAGING_TITLE);
        templateResetButton->setVisible(false);
    } else {
        stagingTitleLabel->setText(QString("%1 - %2").arg(Labels::Backup::k_STAGING_TITLE, templateName));
        templateResetButton->setVisible(true);
    }
}

// Encryption: unlock drive
void MainWindow::onUnlockDriveClicked() {
    QString driveLetter = getSelectedDriveLetter();
    if (driveLetter.isEmpty()) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Warning,
            EncryptionMessages::k_NO_DRIVE_SELECTED_TITLE,
            EncryptionMessages::k_NO_DRIVE_SELECTED_MESSAGE,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    QString drivePath = driveLetter + ":/";
    QDir driveDir(drivePath);

    if (driveDir.exists() && driveDir.isReadable()) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Information,
            EncryptionMessages::k_DRIVE_ALREADY_UNLOCKED_TITLE,
            EncryptionMessages::k_DRIVE_ALREADY_UNLOCKED_MESSAGE.arg(driveLetter),
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    QProcess taskKill;
    taskKill.start(k_TASKKILL_CMD, QStringList() << "/IM" << k_MANAGE_BDE_EXE << "/F");
    taskKill.waitForFinished(2000);

    QString script = QStringLiteral("Start-Process %1 -ArgumentList '%2' -Verb runAs")
                         .arg(QString::fromUtf8(k_MANAGE_BDE_EXE))
                         .arg(QString(k_UNLOCK_ARGS).arg(driveLetter.toUpper()));

    if (!QProcess::startDetached(k_POWERSHELL_CMD, QStringList() << "-Command" << script)) {
        PromptDialog::showDialog(
            this,
            PromptDialog::Critical,
            EncryptionMessages::k_UNLOCK_FAILED_TITLE,
            EncryptionMessages::k_UNLOCK_FAILED_MESSAGE,
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok);
        return;
    }

    QTimer::singleShot(k_DRIVE_UNLOCK_DELAY_MS, this, [this]() {
        ensureBackupStatusUpdated();
        refreshFileWatcher();
        ui->DriveTreeView->setModel(sourceModel);
        setupSourceTreeView();
    });
}

// Encryption: selected drive letter
QString MainWindow::getSelectedDriveLetter() const {
    QModelIndex selectedIndex = ui->DriveTreeView->currentIndex();
    if (!selectedIndex.isValid()) return "";

    QString fullPath = sourceModel->filePath(selectedIndex);
    if (fullPath.length() < 2 || fullPath[1] != ':') return "";

    return fullPath.left(1).toUpper();
}

// Notifications: dialog
void MainWindow::showNotificationDialog() {
    QList<NotificationServiceStruct> notifications = NotificationServiceManager::instance().allNotifications();

    NotificationsDialog dialog(notifications, this);
    dialog.exec();

    NotificationServiceManager::instance().markAllAsRead();
    updateNotificationButtonState();
}

// Notifications: finalize queue
void MainWindow::finishNotificationQueue() {
    isNotificationPopupVisible = false;
    NotificationServiceManager::instance().markAllAsRead();
    updateNotificationButtonState();
}

// Notifications: show next in queue
void MainWindow::showNextNotification() {
    if (notificationQueue.isEmpty()) {
        finishNotificationQueue();
        return;
    }

    isNotificationPopupVisible = true;
    displayNotificationPopup(notificationQueue.takeFirst());
}

// Notifications: display one popup
void MainWindow::displayNotificationPopup(const NotificationServiceStruct &notif) {
    QString message = Labels::Notification::k_TIMESTAMP_TEMPLATE.arg(
        notif.timestamp.toLocalTime().toString(Backup::Timestamps::k_NOTIFICATION_TIMESTAMP_DISPLAY_FORMAT));

    auto *dlg = new PromptDialog(this);
    dlg->setWindowTitle(InfoMessages::k_NOTIFICATION_POPUP_TITLE);
    dlg->setIcon(PromptDialog::Information);
    dlg->setMessageText(message);
    dlg->setInformativeText(notif.message);
    dlg->setStandardButtons(PromptDialog::Ok);
    dlg->setDefaultButton(PromptDialog::Ok);

    connect(dlg, &QDialog::finished, this, [this, dlg](int) {
        dlg->deleteLater();
        showNextNotification();
    });

    dlg->open();
}

// Notifications: badge visibility
void MainWindow::updateNotificationButtonState() {
    bool hasUnread = !NotificationServiceManager::instance().unreadNotifications().isEmpty();
    if (notificationBadge) notificationBadge->setVisible(hasUnread);
}

// Notifications: button + badge
void MainWindow::setupNotificationButton() {
    ui->NotificationButton->setText(Labels::Backup::k_NOTIFICATION_ORIGINAL_TEXT);

    connect(ui->NotificationButton, &QPushButton::clicked, this, &MainWindow::onNotificationButtonClicked);
    connect(&NotificationServiceManager::instance(), &NotificationServiceManager::notificationsUpdated, this, &MainWindow::updateNotificationButtonState);

    notificationBadge = new QLabel(ui->NotificationButton);
    notificationBadge->setObjectName("NotificationBadge");
    notificationBadge->setFixedSize(10, 10);
    notificationBadge->move(ui->NotificationButton->width() - 12, 2);
    notificationBadge->hide();
    notificationBadge->raise();

    updateNotificationButtonState();
}

// Notifications: button feedback
void MainWindow::feedbackNotificationButton() {
    triggerButtonFeedback(ui->NotificationButton,
                          Labels::Backup::k_NOTIFICATION_BUTTON_TEXT,
                          Labels::Backup::k_NOTIFICATION_ORIGINAL_TEXT,
                          System::Timing::k_BUTTON_FEEDBACK_DURATION_MS);
}

// Notifications: button clicked
void MainWindow::onNotificationButtonClicked() {
    showNotificationDialog();
}

// Application status: update label and revalidate
void MainWindow::updateApplicationStatusLabel() {
    const QString status = checkInstallIntegrityStatus();

    const QString emoji = (status == InfoMessages::k_INSTALL_OK) ? Labels::Emoji::k_GREEN : Labels::Emoji::k_RED;
    const QString label = (status == InfoMessages::k_INSTALL_OK) ? Labels::ApplicationStatus::k_HEALTHY
                                                                 : Labels::ApplicationStatus::k_INVALID;

    ui->ApplicationStatusLabel->setText(Labels::ApplicationStatus::k_STATUS_LABEL.arg(emoji, label));
    ui->ApplicationStatusLabel->setTextFormat(Qt::RichText);

    static bool appIntegrityNotified = false;
    if (status != InfoMessages::k_INSTALL_OK && !appIntegrityNotified) {
        NotificationServiceManager::instance().addNotification(NotificationMessages::k_BROKEN_APP_STRUCTURE_MESSAGE);
        appIntegrityNotified = true;
    } else if (status == InfoMessages::k_INSTALL_OK) {
        appIntegrityNotified = false;
    }

    revalidateBackupAndAppStatus(status);
}

// Application status: verify install integrity
QString MainWindow::checkInstallIntegrityStatus() {
    static const QStringList expectedConfigFiles = {
        App::Items::k_APPDATA_SETUP_INFO_FILE,
        App::Items::k_APPDATA_SETUP_NOTIFICATIONS_FILE,
        App::Items::k_APPDATA_SETUP_USER_SETTINGS_FILE
    };

    const QString configDir = PathServiceManager::appConfigFolderPath();
    int missingCount = 0;

    for (const QString &file : expectedConfigFiles) {
        if (!QFile::exists(configDir + "/" + file)) ++missingCount;
    }

    if (missingCount == 0) return InfoMessages::k_INSTALL_OK;
    if (missingCount < expectedConfigFiles.size()) return InfoMessages::k_INSTALL_PARTIAL;
    return InfoMessages::k_INSTALL_BROKEN;
}

// Backup deletion: reset or single delete
void MainWindow::handleBackupDeletion(const QString &path, const QString &deleteType) {
    const QString correctBackupDir = backupService->getBackupRoot();

    if (deleteType == "reset") {
        if (fileWatcher) fileWatcher->removeAllPaths();

        ui->BackupDestinationView->setModel(nullptr);
        resetDestinationModels();
        backupController->resetBackupArchive(path);

        if (fileWatcher) fileWatcher->startWatchingMultiple({correctBackupDir});

        setupDestinationView(correctBackupDir);
        refreshFileWatcher();
        ensureBackupStatusUpdated();
    } else if (deleteType == "single") {
        backupController->deleteBackup(path);
        QTimer::singleShot(0, this, [this, correctBackupDir]() {
            resetDestinationModels();
            setupDestinationView(correctBackupDir);
            refreshFileWatcher();
            ensureBackupStatusUpdated();
        });
    }
}

// Uninstall: clear app data
void MainWindow::handleAppDataClear() {
    NotificationServiceManager::instance().suspendNotifications(true);

    UninstallServiceManager uninstallService;
    if (!uninstallService.confirmUninstall(this)) {
        NotificationServiceManager::instance().suspendNotifications(false);
        return;
    }

    if (fileWatcher) fileWatcher->removeAllPaths();
    ui->BackupDestinationView->setModel(nullptr);
    resetDestinationModels();

    bool success = uninstallService.performUninstall();
    NotificationServiceManager::instance().suspendNotifications(false);

    if (success) QApplication::quit();
}

// Events: close
void MainWindow::closeEvent(QCloseEvent* event) {
    bool minimizeOnClose = ServiceDirector::getInstance()
    .getUserServiceManager()
        ->settings()
        .value(UserServiceKeys::k_MINIMIZE_ON_CLOSE_KEY)
        .toBool(true);

    if (minimizeOnClose) {
        hide();
        event->ignore();
        return;
    }

    if (trayIcon) {
        trayIcon->setContextMenu(nullptr);
        trayIcon->hide();
        trayIcon->disconnect();
        trayIcon->deleteLater();
        trayIcon = nullptr;
    }

    if (trayMenu) {
        trayMenu->hide();
        trayMenu->deleteLater();
        trayMenu = nullptr;
    }

    QMainWindow::closeEvent(event);
}

// Models: reset destination models
void MainWindow::resetDestinationModels() {
    delete destinationModel;
    destinationModel = new QFileSystemModel(this);

    delete destinationProxyModel;
    destinationProxyModel = nullptr;
}

// Accessors
QTabWidget* MainWindow::getDetailsTabWidget() {
    return ui->DetailsTabWidget;
}
