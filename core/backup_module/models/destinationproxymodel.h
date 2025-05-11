#ifndef DESTINATIONPROXYMODEL_H
#define DESTINATIONPROXYMODEL_H

// Qt includes
#include <QRegularExpression>
#include <QSortFilterProxyModel>
#include <QString>

// Proxy model that filters and sorts destination folders
class DestinationProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    // Constructor
    explicit DestinationProxyModel(QObject* parent = nullptr);

    // Sets folder name to be excluded
    void setExcludedFolderName(const QString& folderName);

    // Filters rows based on folder name
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

    // Sorts folders by timestamp if available
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

private:
    // Folder to exclude from view
    QString excludedFolderName;
};

#endif // DESTINATIONPROXYMODEL_H
