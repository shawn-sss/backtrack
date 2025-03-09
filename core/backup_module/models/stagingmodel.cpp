// Project includes same directory
#include "stagingmodel.h"

// Project includes different directory
#include "../../../config/_constants.h"

// Built-in Qt includes
#include <QFileInfo>
#include <QStorageInfo>
#include <QFileIconProvider>

// Constructor
StagingModel::StagingModel(QObject *parent)
    : QAbstractItemModel(parent) {
    stagedPaths.reserve(10);
}

// Creates index for model
QModelIndex StagingModel::index(int row, int column, const QModelIndex &parent) const {
    if (!parent.isValid() && row >= 0 && row < stagedPaths.size() && column == 0) {
        return createIndex(row, column);
    }
    return {};
}

// Returns parent index (always empty, flat list)
QModelIndex StagingModel::parent(const QModelIndex &) const {
    return {};
}

// Returns row count
int StagingModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : stagedPaths.size();
}

// Returns column count
int StagingModel::columnCount(const QModelIndex &) const {
    return 1;
}

// Retrieves data for a given index and role
QVariant StagingModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= stagedPaths.size()) {
        return {};
    }

    static QFileIconProvider iconProvider;
    const QString &path = stagedPaths.at(index.row());
    QFileInfo fileInfo(path);

    switch (role) {
    case Qt::DisplayRole: {
        if (fileInfo.isRoot()) {
            QStorageInfo storageInfo(path);
            const QString volumeLabel = storageInfo.displayName().isEmpty()
                                            ? BackupConfiguration::DEFAULT_DRIVE_LABEL
                                            : storageInfo.displayName();
            return QString("%1 (%2)").arg(volumeLabel, path.left(2));
        }
        return fileInfo.fileName().isEmpty() ? path : fileInfo.fileName();
    }
    case Qt::ToolTipRole:
        return path;
    case Qt::DecorationRole:
        return iconProvider.icon(fileInfo);
    default:
        return {};
    }
}

// Retrieves header data
QVariant StagingModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return UISettings::TreeView::STAGING_COLUMN_NAME;
    }
    return {};
}

// Adds a new path to the staged list
void StagingModel::addPath(const QString &path) {
    if (!path.isEmpty() && !stagedPathsSet.contains(path)) {
        beginInsertRows({}, stagedPaths.size(), stagedPaths.size());
        stagedPaths.append(path);
        stagedPathsSet.insert(path);
        endInsertRows();
    }
}

// Removes a path from the staged list
void StagingModel::removePath(const QString &path) {
    int index = stagedPaths.indexOf(path);
    if (index != -1) {
        beginRemoveRows({}, index, index);
        stagedPaths.removeAt(index);
        stagedPathsSet.remove(path);
        endRemoveRows();
    }
}

// Returns the list of staged paths
QStringList StagingModel::getStagedPaths() const {
    return stagedPaths.toList();
}
