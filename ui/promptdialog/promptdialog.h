#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

// Qt
#include <QDialog>
#include <QDialogButtonBox>

class QLabel;
class QVBoxLayout;
class QAbstractButton;
class QScrollArea;

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

    // Modal convenience API
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

    // Layout/sizing helpers
    void resizeToContent();                // compute final size from content
    int  computeTargetDialogWidth() const; // hybrid width (widest line + base)
    int  computeTextColumnWidth(int targetDialogW) const;

    // Helpers
    QPixmap iconPixmap(Icon iconType);

    // ---- Inline helpers (fixes unresolved externals) ----
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

    // Widgets
    QLabel *iconDisplay = nullptr;
    QLabel *messageLabel = nullptr;
    QLabel *detailLabel = nullptr;

    QScrollArea *textScrollArea = nullptr;
    QDialogButtonBox *buttonBox = nullptr;
    QVBoxLayout *mainLayout = nullptr;

    Button userChoice = None;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PromptDialog::Buttons)

#endif // PROMPTDIALOG_H
