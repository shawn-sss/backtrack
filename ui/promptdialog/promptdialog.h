// filename: promptdialog.h

#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

// Qt includes
#include <QDialog>
#include <QDialogButtonBox>

// Forward declaration (Qt class)
class QLabel;
class QVBoxLayout;
class QAbstractButton;
class QScrollArea;
class QPixmap;

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

    // Setters
    void setMessageText(const QString &text);
    void setInformativeText(const QString &text);
    void setIcon(Icon icon);
    void setStandardButtons(Buttons buttons);
    void setDefaultButton(Button button);

    // Modal dialog helper
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
    void initializeUI();
    void createWidgets();
    void setupLayouts();
    void applyStyling();
    void configureConnections();

    void resizeToContent();
    int  computeTargetDialogWidth() const;
    int  computeTextColumnWidth(int targetDialogW) const;

    QPixmap iconPixmap(Icon iconType);

    static inline QDialogButtonBox::StandardButtons toStandardButtons(Buttons buttons) {
        QDialogButtonBox::StandardButtons stdButtons;
        if (buttons.testFlag(Ok))     stdButtons |= QDialogButtonBox::Ok;
        if (buttons.testFlag(Cancel)) stdButtons |= QDialogButtonBox::Cancel;
        if (buttons.testFlag(Yes))    stdButtons |= QDialogButtonBox::Yes;
        if (buttons.testFlag(No))     stdButtons |= QDialogButtonBox::No;
        return stdButtons;
    }
    static inline QDialogButtonBox::StandardButton toStandardButton(Button button) {
        switch (button) {
        case Ok:     return QDialogButtonBox::Ok;
        case Cancel: return QDialogButtonBox::Cancel;
        case Yes:    return QDialogButtonBox::Yes;
        case No:     return QDialogButtonBox::No;
        default:     return QDialogButtonBox::NoButton;
        }
    }

    QLabel *iconDisplay          = nullptr;
    QLabel *messageLabel         = nullptr;
    QLabel *detailLabel          = nullptr;
    QScrollArea *textScrollArea  = nullptr;
    QDialogButtonBox *buttonBox  = nullptr;
    QVBoxLayout *mainLayout      = nullptr;

    Button userChoice = None;

    Q_DISABLE_COPY_MOVE(PromptDialog)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PromptDialog::Buttons)

#endif // PROMPTDIALOG_H
