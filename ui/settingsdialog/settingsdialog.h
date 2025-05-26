#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Qt includes
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>

// SettingsDialog manages user and system settings configuration
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog() override;

signals:
    void requestBackupReset(const QString& path, const QString& deleteType);
    void requestAppDataClear();

private slots:
    // Handles Save button click
    void onSaveClicked();

private:
    // Initializes layout and UI structure
    void setupLayout();

    // Builds the user settings page
    QWidget* createUserSettingsPage();

    // Builds the system settings page
    QWidget* createSystemSettingsPage();

    // Core UI widgets
    QListWidget* categoryList = nullptr;
    QStackedWidget* settingsStack = nullptr;
    QLineEdit* backupPrefixEdit = nullptr;
    QComboBox* themeComboBox = nullptr;
    QPushButton* saveButton = nullptr;
    QTimer* saveCooldownTimer = nullptr;
    QCheckBox* minimizeOnCloseCheckbox = nullptr;

    // Action buttons for system settings
    QPushButton* clearAppDataButton = nullptr;
    QPushButton* resetBackupArchiveButton = nullptr;
};

#endif // SETTINGSDIALOG_H
