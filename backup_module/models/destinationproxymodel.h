#ifndef DESTINATIONPROXYMODEL_H
#define DESTINATIONPROXYMODEL_H

// Qt includes
#include <QSortFilterProxyModel>
#include <QString>

// Proxy model for filtering and sorting backup destination folders
class DestinationProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit DestinationProxyModel(QObject* parent = nullptr);

    // Excluded folder handling
    void setExcludedFolderName(const QString& folderName);

protected:
    // Filtering logic
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

    // Sorting logic
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

private:
    QString excludedFolderName;
};

#endif // DESTINATIONPROXYMODEL_H
