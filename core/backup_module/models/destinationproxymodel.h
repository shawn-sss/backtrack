#ifndef DESTINATIONPROXYMODEL_H
#define DESTINATIONPROXYMODEL_H

// Qt includes
#include <QString>
#include <QSortFilterProxyModel>
#include <QRegularExpression>

// Proxy model that filters a folder and sorts entries by timestamp
class DestinationProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    // Constructor
    explicit DestinationProxyModel(QObject* parent = nullptr);

    // Set folder name to exclude
    void setExcludedFolderName(const QString& folderName);

    // Filter logic override
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

    // Custom sorting override to sort by timestamp within folder name
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

private:
    // Folder name to exclude
    QString excludedFolderName;
};

#endif // DESTINATIONPROXYMODEL_H
