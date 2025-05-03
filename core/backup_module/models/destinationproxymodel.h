#ifndef DESTINATIONPROXYMODEL_H
#define DESTINATIONPROXYMODEL_H

// Qt includes
#include <QString>
#include <QSortFilterProxyModel>

// Filters out a specific folder from display in views
class DestinationProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    // Constructor
    explicit DestinationProxyModel(QObject* parent = nullptr);

    // Set folder name to exclude (case-insensitive)
    void setExcludedFolderName(const QString& folderName);

    // Filter logic override
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    // Folder name to exclude
    QString excludedFolderName;
};

#endif // DESTINATIONPROXYMODEL_H
