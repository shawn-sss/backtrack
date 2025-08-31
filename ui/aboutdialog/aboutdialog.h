#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

// Qt includes
#include <QDialog>

// Forward declaration (Qt class)
class QLabel;

// About dialog window
class AboutDialog : public QDialog {
    Q_OBJECT

public:
    explicit AboutDialog(QWidget* parent = nullptr);

private:
    QLabel* logoLabel = nullptr;
    QLabel* textLabel = nullptr;
};

#endif // ABOUTDIALOG_H
