// Project includes same directory
#include "destinationproxymodel.h"

// Built-in Qt includes
#include <QModelIndex>

// Constructor
DestinationProxyModel::DestinationProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {}

// Sets the name of the folder to exclude from filtering
void DestinationProxyModel::setExcludedFolderName(const QString &folderName) {
    excludedFolderName = folderName.toLower();
}

// Determines whether a row should be included based on folder name
bool DestinationProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QString folderName = sourceModel()->data(index, Qt::DisplayRole).toString().toLower();

    return folderName != excludedFolderName;
}
