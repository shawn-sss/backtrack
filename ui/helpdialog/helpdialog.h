#ifndef HELPDIALOG_H
#define HELPDIALOG_H

// Qt includes
#include <QDialog>

// Forward declaration (Qt class)
class QTabWidget;
class QTextBrowser;

class HelpDialog : public QDialog {
    Q_OBJECT

public:
    // Constructs the help dialog
    explicit HelpDialog(QWidget* parent = nullptr);

private:
    // Creates and returns a styled QTextBrowser with given HTML
    QTextBrowser* createTextBrowser(const QString& html);

    QTabWidget* tabWidget = nullptr;
    QTextBrowser* gettingStartedText = nullptr;
    QTextBrowser* featuresText = nullptr;
    QTextBrowser* faqText = nullptr;
};

#endif // HELPDIALOG_H
