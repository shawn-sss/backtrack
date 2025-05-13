#ifndef HELPDIALOG_H
#define HELPDIALOG_H

// Qt includes
#include <QDialog>

// Forward declarations (Qt class)
class QTabWidget;
class QTextBrowser;

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget* parent = nullptr);
    ~HelpDialog();

private:
    // UI elements
    QTabWidget* tabWidget;
    QTextBrowser* gettingStartedText;
    QTextBrowser* featuresText;
    QTextBrowser* faqText;
};

#endif // HELPDIALOG_H
