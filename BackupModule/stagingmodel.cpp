#include "stagingmodel.h"
#include "../Utils/constants.h"
#include <QFileIconProvider>
#include <QFileInfo>

// Constructor
StagingModel::StagingModel(QObject *parent)
    : QAbstractItemModel(parent) {}

// QModelIndex creation for rows
QModelIndex StagingModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() || row < 0 || row >= stagedPaths.size() || column != 0) {
        return QModelIndex();
    }
    return createIndex(row, column);
}

// Parent index (flat structure, no hierarchy)
QModelIndex StagingModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

// Number of rows (staged paths)
int StagingModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : stagedPaths.size();
}

// Number of columns (always one)
int StagingModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

// Data for display, tooltip, and decoration
QVariant StagingModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= stagedPaths.size()) {
        return QVariant();
    }

    const QString &path = stagedPaths.at(index.row());
    QFileInfo fileInfo(path);

    switch (role) {
    case Qt::DisplayRole:
        return fileInfo.fileName().isEmpty() ? path : fileInfo.fileName();
    case Qt::ToolTipRole:
        return path;
    case Qt::DecorationRole: // File icon
        return QFileIconProvider().icon(fileInfo);
    default:
        return QVariant();
    }
}

// Header data for the column
QVariant StagingModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return Constants::STAGING_COLUMN_NAME;
    }
    return QVariant();
}

// Add a path to the staged list
void StagingModel::addPath(const QString &path) {
    if (!stagedPaths.contains(path)) {
        beginInsertRows(QModelIndex(), stagedPaths.size(), stagedPaths.size());
        stagedPaths.append(path);
        endInsertRows();
    }
}

// Remove a path from the staged list
void StagingModel::removePath(const QString &path) {
    int index = stagedPaths.indexOf(path);
    if (index != -1) {
        beginRemoveRows(QModelIndex(), index, index);
        stagedPaths.removeAt(index);
        endRemoveRows();
    }
}

// Retrieve all staged paths
QStringList StagingModel::getStagedPaths() const {
    return stagedPaths;
}
