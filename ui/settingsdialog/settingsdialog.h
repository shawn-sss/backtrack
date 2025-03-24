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
    // Constructor and Destructor
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;

private:
    // Initializes UI layout
    void setupLayout();

private slots:
    // Handles save action
    void onSaveClicked();

    // Handles cancel action
    void onCancelClicked();
};

#endif // SETTINGSDIALOG_H
