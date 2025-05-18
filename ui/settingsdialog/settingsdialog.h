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

// SettingsDialog provides an interface for modifying user and system preferences
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog() override;

private:
    void setupLayout();
    QWidget* createUserSettingsPage();
    QWidget* createSystemSettingsPage();
    void applyButtonCursorsAndTooltips();

private slots:
    void onSaveClicked();

private:
    QListWidget* categoryList;
    QStackedWidget* settingsStack;
    QLineEdit* backupPrefixEdit;
    QComboBox* themeComboBox;
    QPushButton* saveButton;
    QTimer* saveCooldownTimer;
};

#endif // SETTINGSDIALOG_H
