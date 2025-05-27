#ifndef SNAPLISTDIALOG_H
#define SNAPLISTDIALOG_H

// Project includes
#include "../../services/ServiceManagers/SnapListServiceManager/snaplistservicemanager.h"

// Qt includes
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class SnapListDialog : public QDialog {
    Q_OBJECT

public:
    explicit SnapListDialog(SnapListServiceManager* service, QWidget* parent = nullptr);

signals:
    // Emitted when a snap list is loaded
    void snapListLoaded(const QVector<QString>& paths, const QString& name);

private slots:
    // Slot for loading a selected snap list
    void onLoadClicked();

    // Slot for deleting a selected snap list
    void onDeleteClicked();

    // Slot for saving a new snap list
    void onSaveClicked();

private:
    // Populates the list widget with existing snap list names
    void populateSnapList();

    // Service manager for accessing snap list functionality
    SnapListServiceManager* snapService;

    // UI elements
    QListWidget* listWidget;
    QPushButton* loadButton;
    QPushButton* deleteButton;
    QPushButton* saveButton;
};

#endif // SNAPLISTDIALOG_H
