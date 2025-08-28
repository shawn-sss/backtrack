// filename: settingsdialog.h

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Qt includes
#include <QDialog>

// Forward declaration (Qt class)
class QListWidget;
class QStackedWidget;
class QLineEdit;
class QComboBox;
class QPushButton;
class QTimer;
class QCheckBox;
class QString;

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);

signals:
    void requestBackupReset(const QString& path, const QString& deleteType);
    void requestAppDataClear();

private slots:
    void onSaveClicked();

private:
    void     setupLayout();
    QWidget* createUserSettingsPage();
    QWidget* createSystemSettingsPage();

    QListWidget*    categoryList            = nullptr;
    QStackedWidget* settingsStack           = nullptr;
    QLineEdit*      backupPrefixEdit        = nullptr;
    QComboBox*      themeComboBox           = nullptr;
    QPushButton*    saveButton              = nullptr;
    QTimer*         saveCooldownTimer       = nullptr;
    QCheckBox*      minimizeOnCloseCheckbox = nullptr;
    QPushButton*    clearAppDataButton      = nullptr;
    QPushButton*    resetBackupArchiveButton= nullptr;

    Q_DISABLE_COPY_MOVE(SettingsDialog)
};

#endif // SETTINGSDIALOG_H
