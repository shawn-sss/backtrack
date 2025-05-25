#ifndef STAGINGUTILS_H
#define STAGINGUTILS_H

// Qt includes
#include <QTreeView>

// Forward declaration (Custom class)
class StagingModel;

namespace Shared::Backup {

// Add selected file system paths from a tree view to the staging model
void addSelectedPathsToStaging(QTreeView* treeView, StagingModel* stagingModel);

// Remove selected staged paths from the staging model
void removeSelectedPathsFromStaging(QTreeView* treeView, StagingModel* stagingModel);

} // namespace Shared::Backup

#endif // STAGINGUTILS_H
