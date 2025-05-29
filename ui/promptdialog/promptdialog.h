#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

// Qt includes
#include <QDialog>
#include <QMessageBox>
#include <QDialogButtonBox>

// Forward declaration (Qt class)
class QLabel;
class QVBoxLayout;

// Represents a custom dialog for prompts with icon, message, and buttons
class PromptDialog : public QDialog
{
    Q_OBJECT

public:
    enum Icon {
        NoIcon,
        Information,
        Warning,
        Critical,
        Question
    };

    explicit PromptDialog(QWidget *parent = nullptr);
    ~PromptDialog();

    // Setters for dialog content
    void setMessageText(const QString &text);
    void setInformativeText(const QString &text);
    void setIcon(Icon icon);
    void setStandardButtons(QMessageBox::StandardButtons buttons);
    void setDefaultButton(QMessageBox::StandardButton button);

    // Static method to show the dialog and return selected button
    static QMessageBox::StandardButton showDialog(QWidget *parent,
                                                  Icon icon,
                                                  const QString &title,
                                                  const QString &messageText,
                                                  const QString &informativeText = QString(),
                                                  QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                                  QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

private slots:
    // Handles button click events
    void handleButtonClicked(QAbstractButton *button);

private:
    // Initializes the full UI of the dialog
    void initializeUI();

    // Widget creation and layout setup
    void createWidgets();
    void setupLayouts();

    // Applies style settings and connects signals
    void applyStyling();
    void configureConnections();

    // Utility for retrieving icon pixmap
    QPixmap iconPixmap(Icon iconType);

    QLabel *iconDisplay = nullptr;
    QLabel *messageLabel = nullptr;
    QLabel *detailLabel = nullptr;
    QDialogButtonBox *buttonBox = nullptr;
    QVBoxLayout *mainLayout = nullptr;

    QMessageBox::StandardButton userChoice = QMessageBox::NoButton;
};

#endif // PROMPTDIALOG_H
