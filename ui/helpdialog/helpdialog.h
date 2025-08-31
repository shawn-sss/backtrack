#ifndef HELPDIALOG_H
#define HELPDIALOG_H

// Qt includes
#include <QDialog>

// Forward declaration (Qt class)
class QTabWidget;
class QTextBrowser;

// Help dialog window
class HelpDialog : public QDialog {
    Q_OBJECT

public:
    explicit HelpDialog(QWidget* parent = nullptr);

private:
    QTextBrowser* createTextBrowser(const QString& html);

    QTabWidget*   tabWidget          = nullptr;
    QTextBrowser* gettingStartedText = nullptr;
    QTextBrowser* featuresText       = nullptr;
    QTextBrowser* faqText            = nullptr;

    Q_DISABLE_COPY_MOVE(HelpDialog)
};

#endif // HELPDIALOG_H
