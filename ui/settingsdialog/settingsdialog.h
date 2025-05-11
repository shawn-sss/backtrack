#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Qt includes
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>

// Dialog for managing user and system application settings
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    // Constructor and destructor
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog() override;

private:
    // Layout setup
    void setupLayout();

    // Page creation
    QWidget* createUserSettingsPage();
    QWidget* createSystemSettingsPage();

    // UI setup for Save button
    void applyButtonCursorsAndTooltips();

    // Core widgets
    QListWidget* categoryList;
    QStackedWidget* settingsStack;

    // Input fields
    QLineEdit* backupPrefixEdit;
    QComboBox* themeComboBox;

    // Controls
    QPushButton* saveButton;
    QTimer* saveCooldownTimer;

private slots:
    // Save button handler
    void onSaveClicked();
};

#endif // SETTINGSDIALOG_H
