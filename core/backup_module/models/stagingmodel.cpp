// Project includes
#include "../../../config/configsettings/_settings.h"
#include "stagingmodel.h"

// Qt includes
#include <QFileInfo>
#include <QFileIconProvider>
#include <QStorageInfo>

// Constructor
StagingModel::StagingModel(QObject* parent)
    : QAbstractItemModel(parent) {
    stagedPaths.reserve(10);
}

// Return index for an item in the flat model
QModelIndex StagingModel::index(int row, int column, const QModelIndex& parent) const {
    if (!parent.isValid() && row >= 0 && row < stagedPaths.size() && column == 0) {
        return createIndex(row, column);
    }
    return {};
}

// Return parent index (flat model has no hierarchy)
QModelIndex StagingModel::parent(const QModelIndex&) const {
    return {};
}

// Return number of rows (staged items)
int StagingModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : stagedPaths.size();
}

// Return number of columns (always 1)
int StagingModel::columnCount(const QModelIndex&) const {
    return 1;
}

// Return data for display, tooltip, or decoration
QVariant StagingModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= stagedPaths.size()) {
        return {};
    }

    static QFileIconProvider iconProvider;
    const QString& path = stagedPaths.at(index.row());
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

// Return column header text
QVariant StagingModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return UISettings::TreeView::k_STAGING_COLUMN_NAME;
    }
    return {};
}

// Add a new path to the staging list
void StagingModel::addPath(const QString& path) {
    if (!path.isEmpty() && !stagedPathsSet.contains(path)) {
        beginInsertRows({}, stagedPaths.size(), stagedPaths.size());
        stagedPaths.append(path);
        stagedPathsSet.insert(path);
        endInsertRows();
    }
}

// Remove a path from the staging list
void StagingModel::removePath(const QString& path) {
    int index = stagedPaths.indexOf(path);
    if (index != -1) {
        beginRemoveRows({}, index, index);
        stagedPaths.removeAt(index);
        stagedPathsSet.remove(path);
        endRemoveRows();
    }
}

// Return a copy of all staged paths
QStringList StagingModel::getStagedPaths() const {
    return stagedPaths.toList();
}
