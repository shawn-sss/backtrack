#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Built-in Qt includes
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMouseEvent>

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

private slots:
    // Saves and accepts the dialog
    void onSaveClicked();

    // Cancels and closes the dialog
    void onCancelClicked();
};

#endif // SETTINGSDIALOG_H
