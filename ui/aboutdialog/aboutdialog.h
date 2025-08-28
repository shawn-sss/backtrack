#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

// Qt includes
#include <QDialog>

// Forward declaration (Qt class)
class QLabel;
class QDialogButtonBox;

// About dialog window
class AboutDialog : public QDialog {
    Q_OBJECT

public:
    explicit AboutDialog(QWidget* parent = nullptr);

private:
    QLabel*           logoLabel   = nullptr;
    QLabel*           textLabel   = nullptr;
    QDialogButtonBox* buttonBox   = nullptr;
};

#endif // ABOUTDIALOG_H
