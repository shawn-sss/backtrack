#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Project includes

// Qt includes
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>

// C++ includes

// Forward declaration (Custom class)

// Forward declaration (Qt class)

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog() override;

private:
    // Layout and UI construction
    void setupLayout();
    QWidget* createUserSettingsPage();
    QWidget* createSystemSettingsPage();
    void applyButtonCursorsAndTooltips();

private slots:
    // Event handling
    void onSaveClicked();

private:
    // Core UI elements
    QListWidget* categoryList;
    QStackedWidget* settingsStack;
    QLineEdit* backupPrefixEdit;
    QComboBox* themeComboBox;
    QPushButton* saveButton;
    QTimer* saveCooldownTimer;

    // Action buttons
    QPushButton* clearAppDataButton = nullptr;
    QPushButton* resetBackupArchiveButton = nullptr;
};

#endif // SETTINGSDIALOG_H
