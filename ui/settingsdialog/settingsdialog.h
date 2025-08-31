#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Qt includes
#include <QDialog>

// Forward declaration (Qt class)
class QLineEdit;
class QComboBox;
class QPushButton;
class QTimer;
class QCheckBox;
class QString;

// Settings dialog window
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
    QWidget* createSettingsPage();

    QLineEdit*   backupPrefixEdit         = nullptr;
    QComboBox*   themeComboBox            = nullptr;
    QPushButton* saveButton               = nullptr;
    QTimer*      saveCooldownTimer        = nullptr;
    QCheckBox*   minimizeOnCloseCheckbox  = nullptr;
    QPushButton* clearAppDataButton       = nullptr;
    QPushButton* resetBackupArchiveButton = nullptr;

    Q_DISABLE_COPY_MOVE(SettingsDialog)
};

#endif // SETTINGSDIALOG_H
