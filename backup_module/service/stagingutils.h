#ifndef STAGINGUTILS_H
#define STAGINGUTILS_H

// Project includes
#include "../models/stagingmodel.h"

// Qt includes
#include <QTreeView>

namespace Shared::Backup {

// Add selected paths from a tree view to the staging model
void addSelectedPathsToStaging(QTreeView* treeView, StagingModel* stagingModel);

// Remove selected paths from the staging model
void removeSelectedPathsFromStaging(QTreeView* treeView, StagingModel* stagingModel);

} // namespace Shared::Backup

#endif // STAGINGUTILS_H
