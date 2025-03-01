#include "stagingmodel.h"
#include "../../../config/_constants.h"

#include <QDir>
#include <QSet>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QStorageInfo>

// Constructor
StagingModel::StagingModel(QObject *parent)
    : QAbstractItemModel(parent) {
    stagedPaths.reserve(10);
}

// Creates index for a given row and column
QModelIndex StagingModel::index(int row, int column, const QModelIndex &parent) const {
    return (!parent.isValid() && row >= 0 && row < stagedPaths.size() && column == 0)
    ? createIndex(row, column)
    : QModelIndex();
}

// Returns parent index (always invalid for flat model)
QModelIndex StagingModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

// Returns number of rows (equal to number of staged paths)
int StagingModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : stagedPaths.size();
}

// Returns number of columns (always 1)
int StagingModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

// Returns data for a given index and role
QVariant StagingModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= stagedPaths.size()) {
        return QVariant();
    }

    static QFileIconProvider iconProvider;
    const QString &path = stagedPaths.at(index.row());
    QFileInfo fileInfo(path);

    switch (role) {
    case Qt::DisplayRole:
        if (fileInfo.isRoot()) {
            QStorageInfo storageInfo(path);
            const QString volumeLabel = storageInfo.displayName().isEmpty()
                                            ? BackupConfiguration::DEFAULT_DRIVE_LABEL
                                            : storageInfo.displayName();
            return QString("%1 (%2)").arg(volumeLabel, path.left(2));
        }
        return fileInfo.fileName().isEmpty() ? path : fileInfo.fileName();

    case Qt::ToolTipRole:
        return path;

    case Qt::DecorationRole:
        return iconProvider.icon(fileInfo);

    default:
        return QVariant();
    }
}

// Returns column header data
QVariant StagingModel::headerData(int section, Qt::Orientation orientation, int role) const {
    return (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
    ? UISettings::TreeView::STAGING_COLUMN_NAME
    : QVariant();
}

// Adds a new path to the staged list if not already present
void StagingModel::addPath(const QString &path) {
    static QSet<QString> pathSet;
    if (!path.isEmpty() && !pathSet.contains(path)) {
        beginInsertRows(QModelIndex(), stagedPaths.size(), stagedPaths.size());
        stagedPaths.append(path);
        pathSet.insert(path);
        endInsertRows();
    }
}

// Removes a path from the staged list if it exists
void StagingModel::removePath(const QString &path) {
    static QSet<QString> pathSet;
    int index = stagedPaths.indexOf(path);
    if (index != -1) {
        beginRemoveRows(QModelIndex(), index, index);
        stagedPaths.removeAt(index);
        pathSet.remove(path);
        endRemoveRows();
    }
}

// Returns the current list of staged paths
QStringList StagingModel::getStagedPaths() const {
    return stagedPaths;
}
