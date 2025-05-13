#ifndef STAGINGUTILS_H
#define STAGINGUTILS_H

// Qt includes
#include <QTreeView>

// Forward declarations (Custom class)
class StagingModel;

namespace Shared::Backup {

// Adds selected file system paths from a QTreeView to the staging model
void addSelectedPathsToStaging(QTreeView* treeView, StagingModel* stagingModel);

// Removes selected staged paths from the staging model
void removeSelectedPathsFromStaging(QTreeView* treeView, StagingModel* stagingModel);

} // namespace Shared::Backup

#endif // STAGINGUTILS_H
