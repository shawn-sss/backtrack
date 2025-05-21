#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Qt includes
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>

// Settings dialog for user and system configuration
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    // Constructs the settings dialog
    explicit SettingsDialog(QWidget* parent = nullptr);

    // Destroys the settings dialog
    ~SettingsDialog() override;

private:
    // Sets up the overall layout
    void setupLayout();

    // Creates the user settings page
    QWidget* createUserSettingsPage();

    // Creates the system settings page
    QWidget* createSystemSettingsPage();

    // Applies cursors and tooltips to buttons
    void applyButtonCursorsAndTooltips();

private slots:
    // Handles the save button click
    void onSaveClicked();

private:
    QListWidget* categoryList;
    QStackedWidget* settingsStack;
    QLineEdit* backupPrefixEdit;
    QComboBox* themeComboBox;
    QPushButton* saveButton;
    QTimer* saveCooldownTimer;

    // Buttons for data cleanup features
    QPushButton* clearAppDataButton = nullptr;
    QPushButton* resetBackupArchiveButton = nullptr;
};

#endif // SETTINGSDIALOG_H
