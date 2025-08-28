// filename: welcomedialog.h

#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

// Qt includes
#include <QDialog>
#include <QVector>
#include <QString>

// Forward declaration (Qt class)
class QLabel;
class QDialogButtonBox;

class WelcomeDialog : public QDialog {
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget* parent = nullptr);

private:
    QLabel*           logoLabel   = nullptr;
    QLabel*           textLabel   = nullptr;
    QDialogButtonBox* buttonBox   = nullptr;

    Q_DISABLE_COPY_MOVE(WelcomeDialog)
};

#endif // WELCOMEDIALOG_H
