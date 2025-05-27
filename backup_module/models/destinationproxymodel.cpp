// Project includes
#include "destinationproxymodel.h"

// Qt includes
#include <QFileSystemModel>
#include <QModelIndex>
#include <QStringView>

// Static timestamp regex definition
const QRegularExpression DestinationProxyModel::timestampRegex(R"((\d{8}_\d{6}))");

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

    if (leftIsDir != rightIsDir)
        return leftIsDir;

    const QRegularExpressionMatch leftMatch = timestampRegex.match(leftName);
    const QRegularExpressionMatch rightMatch = timestampRegex.match(rightName);

    if (leftMatch.hasMatch() && rightMatch.hasMatch()) {
        const QStringView leftTimestamp = leftMatch.capturedView(1);
        const QStringView rightTimestamp = rightMatch.capturedView(1);
        return leftTimestamp > rightTimestamp;
    }

    return leftName < rightName;
}
