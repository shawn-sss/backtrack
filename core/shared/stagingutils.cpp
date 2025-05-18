// Project includes
#include "stagingutils.h"
#include "../backup/models/stagingmodel.h"

// Qt includes
#include <QFileSystemModel>
#include <QSet>
#include <QItemSelectionModel>

namespace Shared::Backup {

// Add selected file system paths from a tree view to the staging model
void addSelectedPathsToStaging(QTreeView* treeView, StagingModel* stagingModel) {
    if (!treeView || !stagingModel) return;

    QItemSelectionModel* selectionModel = treeView->selectionModel();
    if (!selectionModel) return;

    QSet<QString> uniquePaths;
    const QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

    for (const QModelIndex& index : selectedIndexes) {
        const QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
        if (!filePath.isEmpty() && !uniquePaths.contains(filePath)) {
            uniquePaths.insert(filePath);
            stagingModel->addPath(filePath);
        }
    }
}


// Remove selected paths from the staging model
void removeSelectedPathsFromStaging(QTreeView* treeView, StagingModel* stagingModel) {
    if (!treeView || !stagingModel) return;

    QItemSelectionModel* selectionModel = treeView->selectionModel();
    if (!selectionModel) return;

    QSet<QString> pathsToRemove;
    const QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

    for (const QModelIndex& index : selectedIndexes) {
        const QString path = stagingModel->data(index, Qt::ToolTipRole).toString();
        if (!path.isEmpty()) {
            pathsToRemove.insert(path);
        }
    }

    for (const QString& path : pathsToRemove) {
        stagingModel->removePath(path);
    }
}

} // namespace Shared::Backup
