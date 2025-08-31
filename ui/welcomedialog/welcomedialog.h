#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

// Qt includes
#include <QDialog>

// Forward declaration (Qt class)
class QLabel;

// Welcome dialog window
class WelcomeDialog : public QDialog {
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget* parent = nullptr);

private:
    QLabel* logoLabel = nullptr;
    QLabel* textLabel = nullptr;

    Q_DISABLE_COPY_MOVE(WelcomeDialog)
};

#endif // WELCOMEDIALOG_H
