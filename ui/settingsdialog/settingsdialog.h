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

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog() override;

signals:
    void requestBackupReset(const QString& path, const QString& deleteType);
    void requestAppDataClear();

private slots:
    // Event handling
    void onSaveClicked();

private:
    // Layout and UI construction
    void setupLayout();
    QWidget* createUserSettingsPage();
    QWidget* createSystemSettingsPage();
    void applyButtonCursorsAndTooltips();

    // Core UI elements
    QListWidget* categoryList = nullptr;
    QStackedWidget* settingsStack = nullptr;
    QLineEdit* backupPrefixEdit = nullptr;
    QComboBox* themeComboBox = nullptr;
    QPushButton* saveButton = nullptr;
    QTimer* saveCooldownTimer = nullptr;

    // Action buttons
    QPushButton* clearAppDataButton = nullptr;
    QPushButton* resetBackupArchiveButton = nullptr;
};

#endif // SETTINGSDIALOG_H
