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
    explicit DestinationProxyModel(QObject* parent = nullptr);

    // Sets the name of the folder to exclude from filtering
    void setExcludedFolderName(const QString& folderName);

protected:
    // Filters out rows matching the excluded folder name
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

    // Sorts items by timestamp embedded in folder names (descending)
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

private:
    QString excludedFolderName;
    static const QRegularExpression timestampRegex;
};

#endif // DESTINATIONPROXYMODEL_H
