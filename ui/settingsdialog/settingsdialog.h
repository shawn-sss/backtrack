#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Qt includes
#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>

// Dialog for managing user and system settings
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    // Constructor and destructor
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog() override;

private:
    // Layout initialization
    void setupLayout();

    // Page creation helpers
    QWidget* createUserSettingsPage();
    QWidget* createSystemSettingsPage();

    // Core UI components
    QListWidget* categoryList;
    QStackedWidget* settingsStack;
    QLineEdit* backupPrefixEdit;
    QComboBox* themeComboBox;
    QPushButton* saveButton;
    QTimer* saveCooldownTimer;

private slots:
    // Handles the Save button logic
    void onSaveClicked();
};

#endif // SETTINGSDIALOG_H
