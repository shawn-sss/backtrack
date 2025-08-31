// Project includes
#include "destinationproxymodel.h"
#include "../constants/backupconstants.h"

// Qt includes
#include <QFileSystemModel>
#include <QModelIndex>
#include <QStringView>

// Static timestamp regex definition
const QRegularExpression DestinationProxyModel::timestampRegex(
    Backup::Timestamps::k_BACKUP_FOLDER_TIMESTAMP_REGEX);

// Constructor
DestinationProxyModel::DestinationProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent) {}

// Sets the folder name to exclude from the view
void DestinationProxyModel::setExcludedFolderName(const QString& folderName) {
    excludedFolderName = folderName.toLower();
}

// Filters out rows that match the excluded folder name
bool DestinationProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const {
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    const QString folderName = sourceModel()->data(index, Qt::DisplayRole).toString();
    return folderName.compare(excludedFolderName, Qt::CaseInsensitive) != 0;
}

// Sorts folders based on embedded timestamps (descending)
bool DestinationProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const {
    auto* model = qobject_cast<QFileSystemModel*>(sourceModel());
    if (!model)
        return false;

    const QString leftName = model->fileName(left);
    const QString rightName = model->fileName(right);

    const bool leftIsDir = model->isDir(left);
    const bool rightIsDir = model->isDir(right);

    // Always sort directories before files
    if (leftIsDir != rightIsDir)
        return leftIsDir;

    // Compare based on timestamp regex if both match
    const QRegularExpressionMatch leftMatch  = timestampRegex.match(leftName);
    const QRegularExpressionMatch rightMatch = timestampRegex.match(rightName);

    if (leftMatch.hasMatch() && rightMatch.hasMatch()) {
        const QStringView leftTimestamp  = leftMatch.capturedView(1);
        const QStringView rightTimestamp = rightMatch.capturedView(1);
        return leftTimestamp > rightTimestamp; // descending order
    }

    // Fallback to alphabetical
    return leftName < rightName;
}
