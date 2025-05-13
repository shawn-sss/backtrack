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

// Dialog for managing user and system application settings
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog() override;

private:
    // Layout initialization
    void setupLayout();

    // Page creation helpers
    QWidget* createUserSettingsPage();
    QWidget* createSystemSettingsPage();

    // Save button enhancements
    void applyButtonCursorsAndTooltips();

    // UI components
    QListWidget* categoryList;
    QStackedWidget* settingsStack;
    QLineEdit* backupPrefixEdit;
    QComboBox* themeComboBox;
    QPushButton* saveButton;
    QTimer* saveCooldownTimer;

private slots:
    void onSaveClicked();
};

#endif // SETTINGSDIALOG_H
