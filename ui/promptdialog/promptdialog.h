// // Prompt Dialog Usage:
// PromptDialog::showDialog(
//     &mainWindow,
//     PromptDialog::Information,
//     "Message window-title",
//     "Message text",
//     "Message info-tex",
//     PromptDialog::Ok
// );

#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

// Qt includes
#include <QDialog>
#include <QDialogButtonBox>

// Forward declaration (Qt class)
class QLabel;
class QVBoxLayout;
class QAbstractButton;

// PromptDialog class for displaying styled modal messages
class PromptDialog : public QDialog {
    Q_OBJECT

public:
    enum Icon {
        NoIcon,
        Information,
        Warning,
        Critical,
        Question
    };

    enum Button {
        None   = 0x0,
        Ok     = 0x1,
        Cancel = 0x2,
        Yes    = 0x4,
        No     = 0x8
    };
    Q_DECLARE_FLAGS(Buttons, Button)

    explicit PromptDialog(QWidget *parent = nullptr);
    ~PromptDialog();

    // Setters
    void setMessageText(const QString &text);
    void setInformativeText(const QString &text);
    void setIcon(Icon icon);
    void setStandardButtons(Buttons buttons);
    void setDefaultButton(Button button);

    // Static dialog display method
    static Button showDialog(QWidget *parent,
                             Icon icon,
                             const QString &title,
                             const QString &messageText,
                             const QString &informativeText = QString(),
                             Buttons buttons = Ok,
                             Button defaultButton = Ok);

private slots:
    void handleButtonClicked(QAbstractButton *button);

private:
    // UI setup
    void initializeUI();
    void createWidgets();
    void setupLayouts();
    void applyStyling();
    void configureConnections();

    // Helpers
    QPixmap iconPixmap(Icon iconType);
    static QDialogButtonBox::StandardButtons toStandardButtons(Buttons buttons);
    static QDialogButtonBox::StandardButton toStandardButton(Button button);

    QLabel *iconDisplay = nullptr;
    QLabel *messageLabel = nullptr;
    QLabel *detailLabel = nullptr;
    QDialogButtonBox *buttonBox = nullptr;
    QVBoxLayout *mainLayout = nullptr;

    Button userChoice = None;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PromptDialog::Buttons)

#endif // PROMPTDIALOG_H
