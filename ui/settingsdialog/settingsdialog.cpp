// Project includes
#include "settingsdialog.h"
#include "settingsdialogconstants.h"
#include "settingsdialogstyling.h"
#include "../../services/ServiceDirector/ServiceDirector.h"
#include "../../services/ServiceManagers/UserServiceManager/UserServiceConstants.h"
#include "../../services/ServiceManagers/UserServiceManager/UserServiceManager.h"
#include "../../../../services/ServiceManagers/BackupServiceManager/BackupServiceManager.h"

// Qt includes
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileInfo>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QSignalBlocker>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

// C++ includes
#include <QString>

using namespace SettingsDialogConstants;
using namespace SettingsDialogStyling;
using ThemeServiceConstants::UserThemePreference;

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowFlags(Qt::Dialog);
    setMinimumSize(QSize(k_DIALOG_MIN_WIDTH, k_DIALOG_MIN_HEIGHT));
    setupLayout();
}

SettingsDialog::~SettingsDialog() = default;

// Creates a bold label
inline QLabel* createBoldLabel(const QString& text, QWidget* parent = nullptr) {
    auto* label = new QLabel(text, parent);
    QFont font = label->font();
    font.setBold(true);
    label->setFont(font);
    return label;
}

// Creates a small gray label
inline QLabel* createGraySmallLabel(const QString& text, QWidget* parent = nullptr) {
    auto* label = new QLabel(text, parent);
    label->setStyleSheet("color: gray; font-size: 11px;");
    label->setWordWrap(true);
    return label;
}

// Sets up the main layout and stacks
void SettingsDialog::setupLayout() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN);
    mainLayout->setSpacing(k_MAIN_SPACING);

    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(k_MAIN_SPACING);

    categoryList = new QListWidget(centralWidget);
    categoryList->addItem("User Settings");
    categoryList->addItem("System Settings");
    categoryList->setFixedWidth(k_CATEGORY_LIST_WIDTH);
    centralLayout->addWidget(categoryList);

    settingsStack = new QStackedWidget(centralWidget);
    settingsStack->addWidget(createUserSettingsPage());
    settingsStack->addWidget(createSystemSettingsPage());
    centralLayout->addWidget(settingsStack);

    connect(categoryList, &QListWidget::currentRowChanged, settingsStack, &QStackedWidget::setCurrentIndex);
    categoryList->setCurrentRow(0);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    saveButton = buttonBox->button(QDialogButtonBox::Ok);
    saveButton->setText(k_BUTTON_SAVE_TEXT);
    saveButton->setToolTip(k_TOOLTIP_SAVE_BUTTON);
    saveButton->setCursor(Qt::PointingHandCursor);

    QFontMetrics fm(saveButton->font());
    int saveWidth = fm.horizontalAdvance(k_BUTTON_SAVE_WIDTH_TEXT) + 40;
    saveButton->setFixedWidth(saveWidth);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);

    saveCooldownTimer = new QTimer(this);
    saveCooldownTimer->setSingleShot(true);
    connect(saveCooldownTimer, &QTimer::timeout, this, [this]() {
        saveButton->setText(k_BUTTON_SAVE_TEXT);
        saveButton->setEnabled(true);
        saveButton->setStyleSheet({});
    });

    mainLayout->addWidget(centralWidget);
    mainLayout->addWidget(buttonBox);
}

// Constructs the user settings page
QWidget* SettingsDialog::createUserSettingsPage() {
    auto* widget = new QWidget();
    auto* layout = new QFormLayout(widget);

    layout->addRow(createBoldLabel("Backup Prefix:"));
    layout->addRow(createGraySmallLabel(k_DESC_BACKUP_SUBTITLE));

    backupPrefixEdit = new QLineEdit(widget);
    backupPrefixEdit->setText(ServiceDirector::getInstance().getBackupServiceManager()->getBackupPrefix());
    backupPrefixEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Za-z0-9]{0,12}$"), this));
    backupPrefixEdit->setMaxLength(12);
    layout->addRow(k_LABEL_BACKUP_PREFIX, backupPrefixEdit);

    layout->addRow(createGraySmallLabel(k_DESC_BACKUP_INFO));
    layout->addItem(new QSpacerItem(0, 12, QSizePolicy::Minimum, QSizePolicy::Fixed));

    layout->addRow(createBoldLabel("Close Behavior:"));
    layout->addRow(createGraySmallLabel("Choose what happens when you close the app window."));

    minimizeOnCloseCheckbox = new QCheckBox("Minimize to system tray instead of exiting");
    bool currentMinimizeSetting = ServiceDirector::getInstance()
                                      .getUserServiceManager()
                                      ->settings()
                                      .value(UserServiceKeys::k_MINIMIZE_ON_CLOSE_KEY)
                                      .toBool(true);
    minimizeOnCloseCheckbox->setChecked(currentMinimizeSetting);
    layout->addRow(minimizeOnCloseCheckbox);

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    return widget;
}

// Constructs the system settings page
QWidget* SettingsDialog::createSystemSettingsPage() {
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    layout->addWidget(new QLabel(k_LABEL_THEME));

    themeComboBox = new QComboBox(widget);
    themeComboBox->addItem(k_LABEL_THEME_SYSTEM_DEFAULT, static_cast<int>(UserThemePreference::Auto));
    themeComboBox->addItem(k_LABEL_THEME_LIGHT_MODE, static_cast<int>(UserThemePreference::Light));
    themeComboBox->addItem(k_LABEL_THEME_DARK_MODE, static_cast<int>(UserThemePreference::Dark));
    layout->addWidget(themeComboBox);

    int index = themeComboBox->findData(static_cast<int>(ServiceDirector::getInstance().getThemePreference()));
    if (index != -1) {
        QSignalBlocker blocker(themeComboBox);
        themeComboBox->setCurrentIndex(index);
    }

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    auto* buttonRow = new QWidget(widget);
    auto* rowLayout = new QHBoxLayout(buttonRow);
    rowLayout->setSpacing(12);
    rowLayout->setContentsMargins(0, 24, 0, 0);

    resetBackupArchiveButton = new QPushButton(k_BUTTON_RESET_BACKUP, buttonRow);
    resetBackupArchiveButton->setCursor(Qt::PointingHandCursor);
    resetBackupArchiveButton->setToolTip(k_TOOLTIP_RESET_BACKUP);
    resetBackupArchiveButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    resetBackupArchiveButton->setStyleSheet(k_RESET_BACKUP_BUTTON_STYLE);
    rowLayout->addWidget(resetBackupArchiveButton);

    clearAppDataButton = new QPushButton(k_BUTTON_CLEAR_APP, buttonRow);
    clearAppDataButton->setCursor(Qt::PointingHandCursor);
    clearAppDataButton->setToolTip(k_TOOLTIP_CLEAR_APP);
    clearAppDataButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    clearAppDataButton->setStyleSheet(k_CLEAR_APP_BUTTON_STYLE);
    rowLayout->addWidget(clearAppDataButton);

    layout->addWidget(buttonRow);

    connect(clearAppDataButton, &QPushButton::clicked, this, [this]() {
        emit requestAppDataClear();
    });

    connect(resetBackupArchiveButton, &QPushButton::clicked, this, [this]() {
        const QString backupLocation = ServiceDirector::getInstance().getBackupServiceManager()->getBackupDirectory();
        if (backupLocation.isEmpty() || !QDir(backupLocation).exists()) {
            QMessageBox::warning(this, k_WARNING_INVALID_PATH_TITLE, k_WARNING_INVALID_PATH_MESSAGE);
            return;
        }

        const QMessageBox::StandardButton confirm = QMessageBox::warning(
            this,
            k_CONFIRM_RESET_TITLE,
            k_CONFIRM_RESET_MESSAGE.arg(backupLocation),
            QMessageBox::Yes | QMessageBox::No
            );

        if (confirm != QMessageBox::Yes) return;

        emit requestBackupReset(backupLocation, "reset");
    });

    return widget;
}

// Applies user-entered settings
void SettingsDialog::onSaveClicked() {
    backupPrefixEdit->clearFocus();
    QString newPrefix = backupPrefixEdit->text().trimmed();

    static const QRegularExpression prefixRegex("^[A-Za-z0-9]+$");
    if (!prefixRegex.match(newPrefix).hasMatch()) {
        QMessageBox::warning(this, k_WARNING_INVALID_PREFIX_TITLE, k_WARNING_INVALID_PREFIX_MESSAGE);
        return;
    }

    ServiceDirector::getInstance().getBackupServiceManager()->setBackupPrefix(newPrefix);

    auto selectedTheme = static_cast<UserThemePreference>(themeComboBox->currentData().toInt());
    ServiceDirector::getInstance().setThemePreference(selectedTheme);
    ServiceDirector::getInstance().applyTheme();

    bool minimizeSetting = minimizeOnCloseCheckbox->isChecked();
    auto& settings = ServiceDirector::getInstance().getUserServiceManager()->settings();
    settings[UserServiceKeys::k_MINIMIZE_ON_CLOSE_KEY] = minimizeSetting;
    ServiceDirector::getInstance().getUserServiceManager()->save();

    saveButton->setText(k_BUTTON_SAVED_TEXT);
    saveButton->setEnabled(false);
    saveButton->setStyleSheet(COOLDOWN_BUTTON_STYLE);

    saveCooldownTimer->start(k_SAVE_FEEDBACK_COOLDOWN_MS);
}
