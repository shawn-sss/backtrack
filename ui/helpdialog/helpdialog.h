#ifndef HELPDIALOG_H
#define HELPDIALOG_H

// Qt includes
#include <QDialog>

// Forward declaration (Qt class)
class QTabWidget;
class QTextBrowser;

// HelpDialog provides a tabbed help window with Getting Started, Features, and FAQ sections
class HelpDialog : public QDialog {
    Q_OBJECT

public:
    explicit HelpDialog(QWidget* parent = nullptr);

private:
    // Widgets for tabbed layout and content display
    QTabWidget* tabWidget;
    QTextBrowser* gettingStartedText;
    QTextBrowser* featuresText;
    QTextBrowser* faqText;
};

#endif // HELPDIALOG_H
