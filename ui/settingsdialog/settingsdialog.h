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
    // Lifecycle
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog() override;

private:
    // Initializes layout and connects components
    void setupLayout();

    // Creates individual configuration pages
    QWidget* createUserSettingsPage();
    QWidget* createSystemSettingsPage();

    // Sets cursor and tooltip for the Save button
    void applyButtonCursorsAndTooltips();

    // UI components
    QListWidget* categoryList;
    QStackedWidget* settingsStack;
    QLineEdit* backupPrefixEdit;
    QComboBox* themeComboBox;
    QPushButton* saveButton;
    QTimer* saveCooldownTimer;

private slots:
    // Handles Save action and visual feedback
    void onSaveClicked();
};

#endif // SETTINGSDIALOG_H
