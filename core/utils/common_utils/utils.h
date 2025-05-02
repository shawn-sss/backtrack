#ifndef UTILS_H
#define UTILS_H

// Project includes
#include "../../../core/backup_module/models/stagingmodel.h"

// Qt includes
#include <QString>
#include <QDateTime>
#include <QPixmap>
#include <QProgressBar>
#include <QMouseEvent>
#include <QTreeView>
#include <QWidget>

namespace Utils {

// UI-related utility functions
namespace UI {
// Tracks window dragging on mouse press
void handleMousePress(QWidget *window, QMouseEvent *event, bool &dragging, QPoint &lastMousePosition);

// Moves window while dragging
void handleMouseMove(QWidget *window, QMouseEvent *event, bool &dragging, QPoint &lastMousePosition);

// Ends dragging on mouse release
void handleMouseRelease(QMouseEvent *event, bool &dragging);

// Hides tree view columns from start to count
void removeAllColumnsFromTreeView(QTreeView *treeView, int startColumn, int columnCount);

// Configures a progress bar with size and range
void setupProgressBar(QProgressBar *progressBar, int minValue, int maxValue, int height, bool textVisible);

// Creates a circular status indicator pixmap
QPixmap createStatusLightPixmap(const QString &color, int size);
}

// Formatting utility functions
namespace Formatting {
// Formats size in bytes to human-readable string
QString formatSize(qint64 size);

// Formats milliseconds into readable duration
QString formatDuration(qint64 milliseconds);

// Formats timestamp with custom string format
QString formatTimestamp(const QDateTime &datetime, const QString &format);

// Formats timestamp with Qt enum format
QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format);
}

// Backup-related utility functions
namespace Backup {
// Adds selected items from tree view to staging
void addSelectedPathsToStaging(QTreeView *treeView, StagingModel *stagingModel);

// Removes selected items from staging
void removeSelectedPathsFromStaging(QTreeView *treeView, StagingModel *stagingModel);
}

} // namespace Utils

#endif // UTILS_H
