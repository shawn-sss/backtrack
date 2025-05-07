// Project includes
#include "settingsdialog.h"
#include "settingsdialogstyling.h"
#include "../../config/configsettings/_settings.h"
#include "../../config/configdirector/configdirector.h"
#include "../../config/configmanagers/thememanager/thememanager.h"

// Qt includes
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFontMetrics>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QMessageBox>

// Required namespaces for styling and configuration constants
using namespace SettingsDialogStyling;
using namespace SettingsDialogConfig;

// Constructs the settings dialog and sets base properties
SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowFlags(Qt::Dialog);
    setMinimumSize(QSize(k_DIALOG_MIN_WIDTH, k_DIALOG_MIN_HEIGHT));
    setupLayout();
}

// Destructor for cleanup
SettingsDialog::~SettingsDialog() = default;

// Sets up the layout, page structure, and connections for the dialog
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

    applyButtonCursorsAndTooltips();

    QFontMetrics fm(saveButton->font());
    int saveWidth = fm.horizontalAdvance(k_BUTTON_SAVE_WIDTH_TEXT) + 40;
    saveButton->setFixedWidth(saveWidth);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveClicked);

    saveCooldownTimer = new QTimer(this);
    saveCooldownTimer->setSingleShot(true);
    connect(saveCooldownTimer, &QTimer::timeout, this, [this]() {
        saveButton->setText(k_BUTTON_SAVE_TEXT);
        saveButton->setEnabled(true);
        saveButton->setStyleSheet(QString());
    });

    mainLayout->addWidget(centralWidget);
    mainLayout->addWidget(buttonBox);
}

// Creates the user settings configuration page
QWidget* SettingsDialog::createUserSettingsPage() {
    auto* widget = new QWidget();
    auto* layout = new QFormLayout(widget);

    auto* descriptionLabel = new QLabel("Backup Prefix Description:");
    QFont labelFont = descriptionLabel->font();
    labelFont.setBold(true);
    descriptionLabel->setFont(labelFont);
    layout->addRow(descriptionLabel);

    auto* subtitleLabel = new QLabel("This is the first part of each backup name to help group and identify them.");
    subtitleLabel->setStyleSheet("color: gray; font-size: 11px;");
    subtitleLabel->setWordWrap(true);
    layout->addRow(subtitleLabel);

    backupPrefixEdit = new QLineEdit(widget);
    backupPrefixEdit->setText(ConfigDirector::getInstance().getBackupPrefix());

    QRegularExpression regex("^[A-Za-z0-9]{0,12}$");
    backupPrefixEdit->setValidator(new QRegularExpressionValidator(regex, this));
    backupPrefixEdit->setMaxLength(12);

    layout->addRow(k_LABEL_BACKUP_PREFIX, backupPrefixEdit);

    auto* prefixInfoLabel = new QLabel("Allowed: letters (A–Z, a–z) and digits (0–9), up to 12 characters.");
    prefixInfoLabel->setStyleSheet("color: gray; font-size: 11px;");
    layout->addRow(prefixInfoLabel);

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    return widget;
}

// Creates the system settings configuration page
QWidget* SettingsDialog::createSystemSettingsPage() {
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    layout->addWidget(new QLabel(k_LABEL_THEME));

    themeComboBox = new QComboBox(widget);
    themeComboBox->addItem(k_LABEL_THEME_SYSTEM_DEFAULT, static_cast<int>(UserThemePreference::Auto));
    themeComboBox->addItem(k_LABEL_THEME_LIGHT_MODE, static_cast<int>(UserThemePreference::Light));
    themeComboBox->addItem(k_LABEL_THEME_DARK_MODE, static_cast<int>(UserThemePreference::Dark));
    layout->addWidget(themeComboBox);

    UserThemePreference savedPref = ConfigDirector::getInstance().getThemePreference();
    int index = themeComboBox->findData(static_cast<int>(savedPref));
    if (index != -1) {
        QSignalBlocker blocker(themeComboBox);
        themeComboBox->setCurrentIndex(index);
    }

    layout->addStretch();
    return widget;
}

// Handles saving updated user/system settings and visual feedback
void SettingsDialog::onSaveClicked() {
    backupPrefixEdit->clearFocus();
    QString newPrefix = backupPrefixEdit->text().trimmed();

    static const QRegularExpression prefixRegex("^[A-Za-z0-9]+$");
    if (!prefixRegex.match(newPrefix).hasMatch()) {
        QMessageBox::warning(this, "Invalid Prefix", "Only letters (A–Z, a–z) and digits (0–9) are allowed.");
        return;
    }

    ConfigDirector::getInstance().setBackupPrefix(newPrefix);

    auto selectedTheme = static_cast<UserThemePreference>(themeComboBox->currentData().toInt());
    ConfigDirector::getInstance().setThemePreference(selectedTheme);
    ThemeManager::applyTheme();

    saveButton->setText(k_BUTTON_SAVED_TEXT);
    saveButton->setEnabled(false);
    saveButton->setStyleSheet(COOLDOWN_BUTTON_STYLE);

    saveCooldownTimer->start(k_SAVE_FEEDBACK_COOLDOWN_MS);
}

// Set pointing hand cursor and tooltip for the Save button
void SettingsDialog::applyButtonCursorsAndTooltips() {
    saveButton->setCursor(Qt::PointingHandCursor);
    saveButton->setToolTip("Save your settings and apply changes");
}
