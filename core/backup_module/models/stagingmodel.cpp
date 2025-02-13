#include "stagingmodel.h"
#include "../../config/constants.h"

#include <QFileIconProvider>
#include <QFileInfo>
#include <QStorageInfo>
#include <QDir>

// Constructor
StagingModel::StagingModel(QObject *parent)
    : QAbstractItemModel(parent) {}

// QModelIndex Management
QModelIndex StagingModel::index(int row, int column, const QModelIndex &parent) const {
    return (!parent.isValid() && row >= 0 && row < stagedPaths.size() && column == 0)
    ? createIndex(row, column)
    : QModelIndex();
}

QModelIndex StagingModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

// Row and Column Counts
int StagingModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : stagedPaths.size();
}

int StagingModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

// Data Handling
QVariant StagingModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= stagedPaths.size()) {
        return QVariant();
    }

    const QString &path = stagedPaths.at(index.row());
    QFileInfo fileInfo(path);

    switch (role) {
    case Qt::DisplayRole: {
        if (fileInfo.isRoot()) {
            QStorageInfo storageInfo(path);
            QString volumeLabel = storageInfo.displayName().isEmpty() ? "Local Disk" : storageInfo.displayName();
            return QString("%1 (%2)").arg(volumeLabel, path.left(2));
        }
        return fileInfo.fileName().isEmpty() ? path : fileInfo.fileName();
    }
    case Qt::ToolTipRole:
        return path;
    case Qt::DecorationRole:
        return QFileIconProvider().icon(fileInfo);
    default:
        return QVariant();
    }
}

// Header Management
QVariant StagingModel::headerData(int section, Qt::Orientation orientation, int role) const {
    return (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
    ? UIConfig::STAGING_COLUMN_NAME
    : QVariant();
}

// Path Management
void StagingModel::addPath(const QString &path) {
    if (!path.isEmpty() && !stagedPaths.contains(path)) {
        beginInsertRows(QModelIndex(), stagedPaths.size(), stagedPaths.size());
        stagedPaths.append(path);
        endInsertRows();
    }
}

void StagingModel::removePath(const QString &path) {
    int index = stagedPaths.indexOf(path);
    if (index != -1) {
        beginRemoveRows(QModelIndex(), index, index);
        stagedPaths.removeAt(index);
        endRemoveRows();
    }
}

QStringList StagingModel::getStagedPaths() const {
    return stagedPaths;
}
