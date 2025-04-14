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

// Creates an index for a row in the flat model
QModelIndex StagingModel::index(int row, int column, const QModelIndex &parent) const {
    if (!parent.isValid() && row >= 0 && row < stagedPaths.size() && column == 0) {
        return createIndex(row, column);
    }
    return {};
}

// Returns the parent index (none, as model is flat)
QModelIndex StagingModel::parent(const QModelIndex &) const {
    return {};
}

// Returns the number of rows in the model
int StagingModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : stagedPaths.size();
}

// Returns number of columns in the model (always 1)
int StagingModel::columnCount(const QModelIndex &) const {
    return 1;
}

// Provides data for the given index and role
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
                                            ? Backup::DriveConfig::k_DEFAULT_DRIVE_LABEL
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

// Provides header label for the staging list view
QVariant StagingModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return UISettings::TreeView::k_STAGING_COLUMN_NAME;
    }
    return {};
}

// Adds a new file path to the staging list
void StagingModel::addPath(const QString &path) {
    if (!path.isEmpty() && !stagedPathsSet.contains(path)) {
        beginInsertRows({}, stagedPaths.size(), stagedPaths.size());
        stagedPaths.append(path);
        stagedPathsSet.insert(path);
        endInsertRows();
    }
}

// Removes a file path from the staging list
void StagingModel::removePath(const QString &path) {
    int index = stagedPaths.indexOf(path);
    if (index != -1) {
        beginRemoveRows({}, index, index);
        stagedPaths.removeAt(index);
        stagedPathsSet.remove(path);
        endRemoveRows();
    }
}

// Returns the list of currently staged paths
QStringList StagingModel::getStagedPaths() const {
    return stagedPaths.toList();
}
