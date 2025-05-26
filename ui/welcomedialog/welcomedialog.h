#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

// Qt includes
#include <QDialog>

// Forward declaration (Qt class)
class QLabel;
class QDialogButtonBox;

// Welcome dialog window shown on first application launch
class WelcomeDialog : public QDialog {
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget* parent = nullptr);
    ~WelcomeDialog();

private:
    QLabel* logoLabel;
    QLabel* textLabel;
    QDialogButtonBox* buttonBox;
};

#endif // WELCOMEDIALOG_H
