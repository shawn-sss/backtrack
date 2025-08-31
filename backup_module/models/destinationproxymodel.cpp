// Project includes
#include "destinationproxymodel.h"
#include "../constants/backupconstants.h"

// Qt includes
#include <QFileSystemModel>
#include <QRegularExpressionMatch>
#include <QStringView>

// Constructor
DestinationProxyModel::DestinationProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent) {}

// Set excluded folder name
void DestinationProxyModel::setExcludedFolderName(const QString& folderName) {
    excludedFolderName = folderName.toLower();
}

// Filter out excluded folder
bool DestinationProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const {
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    const QString folderName = sourceModel()->data(index, Qt::DisplayRole).toString();
    return folderName.compare(excludedFolderName, Qt::CaseInsensitive) != 0;
}

// Sorting: folders first, then by timestamp if available
bool DestinationProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const {
    auto* model = qobject_cast<QFileSystemModel*>(sourceModel());
    if (!model) return false;

    const QString leftName  = model->fileName(left);
    const QString rightName = model->fileName(right);

    const bool leftIsDir  = model->isDir(left);
    const bool rightIsDir = model->isDir(right);

    if (leftIsDir != rightIsDir) {
        return leftIsDir;
    }

    const QRegularExpressionMatch leftMatch  = Backup::Timestamps::FolderRegex.match(leftName);
    const QRegularExpressionMatch rightMatch = Backup::Timestamps::FolderRegex.match(rightName);

    if (leftMatch.hasMatch() && rightMatch.hasMatch()) {
        const QStringView leftTimestamp  = leftMatch.capturedView(1);
        const QStringView rightTimestamp = rightMatch.capturedView(1);
        return leftTimestamp > rightTimestamp;
    }

    return leftName < rightName;
}
