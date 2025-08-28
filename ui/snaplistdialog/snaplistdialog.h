// filename: snaplistdialog.h

#ifndef SNAPLISTDIALOG_H
#define SNAPLISTDIALOG_H

// Qt includes
#include <QDialog>
#include <QVector>
#include <QString>

// Forward declaration (Custom class)
class SnapListServiceManager;

// Forward declaration (Qt class)
class QListWidget;
class QPushButton;

class SnapListDialog : public QDialog {
    Q_OBJECT

public:
    explicit SnapListDialog(SnapListServiceManager* service, QWidget* parent = nullptr);

signals:
    void snapListLoaded(const QVector<QString>& paths, const QString& name);

private slots:
    void onLoadClicked();
    void onDeleteClicked();
    void onSaveClicked();

private:
    void populateSnapList();

    SnapListServiceManager* snapService = nullptr;

    QListWidget* listWidget   = nullptr;
    QPushButton* loadButton   = nullptr;
    QPushButton* deleteButton = nullptr;
    QPushButton* saveButton   = nullptr;

    Q_DISABLE_COPY_MOVE(SnapListDialog)
};

#endif // SNAPLISTDIALOG_H
