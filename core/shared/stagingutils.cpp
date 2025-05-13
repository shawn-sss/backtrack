// Project includes
#include "stagingutils.h"
#include "../backup/models/stagingmodel.h"

// Qt includes
#include <QFileSystemModel>
#include <QSet>

// Adds selected file system paths from a QTreeView to the staging model
namespace Shared::Backup {

void addSelectedPathsToStaging(QTreeView* treeView, StagingModel* stagingModel) {
    if (!treeView || !stagingModel || !treeView->selectionModel()) return;

    QSet<QString> uniquePaths;
    const auto selectedIndexes = treeView->selectionModel()->selectedIndexes();

    for (const auto& index : selectedIndexes) {
        QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
        if (!filePath.isEmpty() && !uniquePaths.contains(filePath)) {
            uniquePaths.insert(filePath);
            stagingModel->addPath(filePath);
        }
    }
}

// Removes selected staged paths from the staging model
void removeSelectedPathsFromStaging(QTreeView* treeView, StagingModel* stagingModel) {
    if (!treeView || !stagingModel || !treeView->selectionModel()) return;

    QSet<QString> uniquePathsToRemove;
    const auto selectedIndexes = treeView->selectionModel()->selectedIndexes();

    for (const auto& index : selectedIndexes) {
        QString filePath = stagingModel->data(index, Qt::ToolTipRole).toString();
        if (!filePath.isEmpty()) {
            uniquePathsToRemove.insert(filePath);
        }
    }

    for (const auto& filePath : uniquePathsToRemove) {
        stagingModel->removePath(filePath);
    }
}

} // namespace Shared::Backup
