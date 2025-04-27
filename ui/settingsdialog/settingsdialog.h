#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Qt includes
#include <QComboBox>
#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QLineEdit>

// Dialog for managing application settings
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr); // Constructor
    ~SettingsDialog() override; // Destructor

private:
    // Layout setup
    void setupLayout();

    // Settings pages creation
    QWidget* createUserSettingsPage();
    QWidget* createSystemSettingsPage();

    // UI elements
    QListWidget* categoryList;
    QStackedWidget* settingsStack;
    QLineEdit* backupPrefixEdit;
    QComboBox* themeComboBox;

private slots:
    // Button handlers
    void onSaveClicked();
    void onCancelClicked();
};

#endif // SETTINGSDIALOG_H
