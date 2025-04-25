#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Qt includes
#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QComboBox>

// Dialog window for application settings
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    // Constructs and initializes the dialog
    explicit SettingsDialog(QWidget *parent = nullptr);

    // Destructor
    ~SettingsDialog() override;

private:
    // Sets up the dialog's layout and controls
    void setupLayout();
    QWidget* createUserSettingsPage();
    QWidget* createSystemSettingsPage();
    QListWidget *categoryList;
    QStackedWidget *settingsStack;
    QLineEdit *backupPrefixEdit;
    QComboBox *themeComboBox;

private slots:
    // Saves and accepts the dialog
    void onSaveClicked();

    // Cancels and closes the dialog
    void onCancelClicked();
};

#endif // SETTINGSDIALOG_H
