#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

// Qt includes
#include <QDialog>

// Forward declaration (Qt class)
class QLabel;
class QDialogButtonBox;

// About dialog displaying application information
class AboutDialog : public QDialog {
    Q_OBJECT

public:
    explicit AboutDialog(QWidget* parent = nullptr);

private:
    // UI elements
    QLabel* logoLabel;
    QLabel* textLabel;
    QDialogButtonBox* buttonBox;
};

#endif // ABOUTDIALOG_H
