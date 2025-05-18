// Project includes
#include "stagingmodel.h"
#include "../../../../constants/backup_config.h"
#include "../../../../constants/interface_config.h"

// Qt includes
#include <QFileIconProvider>
#include <QFileInfo>
#include <QStorageInfo>

// Constructor
StagingModel::StagingModel(QObject* parent)
    : QAbstractItemModel(parent) {
    stagedPaths.reserve(10);
}

// Singleton QFileIconProvider to avoid repeated instantiation
QFileIconProvider& StagingModel::iconProvider() {
    static QFileIconProvider provider;
    return provider;
}

// Returns model index for item
QModelIndex StagingModel::index(int row, int column, const QModelIndex& parent) const {
    if (!parent.isValid() && row >= 0 && row < stagedPaths.size() && column == 0) {
        return createIndex(row, column);
    }
    return {};
}

// This is a flat list: no hierarchy
QModelIndex StagingModel::parent(const QModelIndex&) const {
    return {};
}

// Only one column, showing all staged items
int StagingModel::columnCount(const QModelIndex&) const {
    return 1;
}

int StagingModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : stagedPaths.size();
}

// Provides data for each role
QVariant StagingModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= stagedPaths.size()) {
        return {};
    }

    const QString& path = stagedPaths.at(index.row());
    const QFileInfo fileInfo(path);

    switch (role) {
    case Qt::DisplayRole: {
        if (fileInfo.isRoot()) {
            const QStorageInfo storageInfo(path);
            const QString volumeLabel = storageInfo.displayName().isEmpty()
                                            ? Backup::Drive::k_DEFAULT_DRIVE_LABEL
                                            : storageInfo.displayName();
            return QString("%1 (%2)").arg(volumeLabel, path.left(2));
        }
        const QString name = fileInfo.fileName();
        return name.isEmpty() ? path : name;
    }
    case Qt::ToolTipRole:
        return path;
    case Qt::DecorationRole:
        return iconProvider().icon(fileInfo);
    default:
        return {};
    }
}

// Provides column header label
QVariant StagingModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return UI::TreeView::k_STAGING_COLUMN_NAME;
    }
    return {};
}

// Adds a new path if not already present
void StagingModel::addPath(const QString& path) {
    if (!path.isEmpty() && !stagedPathsSet.contains(path)) {
        const int newRow = stagedPaths.size();
        beginInsertRows({}, newRow, newRow);
        stagedPaths.append(path);
        stagedPathsSet.insert(path);
        endInsertRows();
    }
}

// Removes an existing path
void StagingModel::removePath(const QString& path) {
    const int index = stagedPaths.indexOf(path);
    if (index != -1) {
        beginRemoveRows({}, index, index);
        stagedPaths.removeAt(index);
        stagedPathsSet.remove(path);
        endRemoveRows();
    }
}

// Returns the full staged list
QStringList StagingModel::getStagedPaths() const {
    return QStringList(stagedPaths.begin(), stagedPaths.end());
}
