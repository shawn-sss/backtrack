// Project includes
#include "destinationproxymodel.h"

// Qt includes
#include <QModelIndex>
#include <QFileSystemModel>
#include <QRegularExpression>

// Constructor
DestinationProxyModel::DestinationProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent) {}

// Set the name of the folder to exclude from filtering
void DestinationProxyModel::setExcludedFolderName(const QString& folderName) {
    excludedFolderName = folderName.toLower();
}

// Filter out rows matching the excluded folder name
bool DestinationProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const {
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QString folderName = sourceModel()->data(index, Qt::DisplayRole).toString().toLower();

    return folderName != excludedFolderName;
}

// Custom sorting logic to order folders by embedded timestamp
bool DestinationProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const {
    QFileSystemModel* model = qobject_cast<QFileSystemModel*>(sourceModel());

    QString leftName = model->fileName(left);
    QString rightName = model->fileName(right);

    bool leftIsDir = model->isDir(left);
    bool rightIsDir = model->isDir(right);

    if (leftIsDir != rightIsDir)
        return leftIsDir;

    static const QRegularExpression re(R"((\d{8}_\d{6}))");
    QRegularExpressionMatch leftMatch = re.match(leftName);
    QRegularExpressionMatch rightMatch = re.match(rightName);

    if (leftMatch.hasMatch() && rightMatch.hasMatch()) {
        QString leftTimestamp = leftMatch.captured(1);
        QString rightTimestamp = rightMatch.captured(1);

        return leftTimestamp > rightTimestamp;
    }

    return leftName < rightName;
}
