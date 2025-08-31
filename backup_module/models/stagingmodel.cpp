// Project includes
#include "stagingmodel.h"
#include "../constants/backupconstants.h"
#include "../../../../constants/interface_config.h"

// Qt includes
#include <QFileIconProvider>
#include <QFileInfo>
#include <QStorageInfo>

StagingModel::StagingModel(QObject* parent)
    : QAbstractItemModel(parent) {
    stagedPaths.reserve(10);
}

QFileIconProvider& StagingModel::iconProvider() {
    static QFileIconProvider provider;
    return provider;
}

// Model structure methods

QModelIndex StagingModel::index(int row, int column, const QModelIndex& parent) const {
    if (!parent.isValid() && row >= 0 && row < stagedPaths.size() && column == 0) {
        return createIndex(row, column);
    }
    return {};
}

QModelIndex StagingModel::parent(const QModelIndex&) const {
    return {};
}

int StagingModel::columnCount(const QModelIndex&) const {
    return 1;
}

int StagingModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : stagedPaths.size();
}

QVariant StagingModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return UI::TreeView::k_STAGING_COLUMN_NAME;
    }
    return {};
}

// Data retrieval for display, tooltip, and decoration roles

QVariant StagingModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= stagedPaths.size()) {
        return {};
    }

    const QString& path = stagedPaths.at(index.row());
    const QFileInfo fileInfo(path);

    switch (role) {
    case Qt::DisplayRole:
        if (fileInfo.isRoot()) {
            const QStorageInfo storageInfo(path);
            const QString volumeLabel = storageInfo.displayName().isEmpty()
                                            ? Backup::Drive::DefaultLabel
                                            : storageInfo.displayName();
            return QString("%1 (%2)").arg(volumeLabel, path.left(2));
        }
        return fileInfo.fileName().isEmpty() ? path : fileInfo.fileName();
    case Qt::ToolTipRole:
        return path;
    case Qt::DecorationRole:
        return iconProvider().icon(fileInfo);
    default:
        return {};
    }
}

// Path modification methods

void StagingModel::addPath(const QString& path) {
    if (!path.isEmpty() && !stagedPathsSet.contains(path)) {
        const int newRow = stagedPaths.size();
        beginInsertRows({}, newRow, newRow);
        stagedPaths.append(path);
        stagedPathsSet.insert(path);
        endInsertRows();
    }
}

void StagingModel::addPaths(const QStringList& paths) {
    for (const QString& path : paths) {
        addPath(path);
    }
}

void StagingModel::removePath(const QString& path) {
    const int index = stagedPaths.indexOf(path);
    if (index != -1) {
        beginRemoveRows({}, index, index);
        stagedPaths.removeAt(index);
        stagedPathsSet.remove(path);
        endRemoveRows();
    }
}

void StagingModel::clear() {
    beginResetModel();
    stagedPaths.clear();
    stagedPathsSet.clear();
    endResetModel();
}

// Path query methods

QStringList StagingModel::getStagedPaths() const {
    return QStringList(stagedPaths.begin(), stagedPaths.end());
}

bool StagingModel::containsPath(const QString& path) const {
    return stagedPathsSet.contains(path);
}
