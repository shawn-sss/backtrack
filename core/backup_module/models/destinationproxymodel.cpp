// DestinationProxyModel.cpp
#include "destinationproxymodel.h"
#include <QModelIndex>

DestinationProxyModel::DestinationProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {}

void DestinationProxyModel::setExcludedFolderName(const QString &folderName) {
    excludedFolderName = folderName.toLower();
}

bool DestinationProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QString folderName = sourceModel()->data(index, Qt::DisplayRole).toString().toLower();

    return folderName != excludedFolderName;
}
