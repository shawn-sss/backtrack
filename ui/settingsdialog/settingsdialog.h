#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Qt includes
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QStackedWidget>
#include <QDialog>

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    // Constructor and destructor
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog() override;

private:
    // Layout and UI setup methods
    void setupLayout();
    QWidget* createUserSettingsPage();
    QWidget* createSystemSettingsPage();

    // Core UI elements
    QListWidget* categoryList;
    QStackedWidget* settingsStack;
    QLineEdit* backupPrefixEdit;
    QComboBox* themeComboBox;
    QPushButton* saveButton;
    QTimer* saveCooldownTimer;

private slots:
    // Event handlers
    void onSaveClicked();
    void onCancelClicked();
};

#endif // SETTINGSDIALOG_H
