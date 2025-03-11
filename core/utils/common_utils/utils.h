#ifndef UTILS_H
#define UTILS_H

// Project includes
#include <QMouseEvent>
#include <QWidget>
#include <QTreeView>
#include <QProgressBar>
#include <QPixmap>
#include <QString>
#include <QDateTime>

// Include StagingModel header
#include "../../../core/backup_module/models/stagingmodel.h"

namespace Utils {

// UI-related utility functions
namespace UI {
void handleMousePress(QWidget *window, QMouseEvent *event, bool &dragging, QPoint &lastMousePosition);
void handleMouseMove(QWidget *window, QMouseEvent *event, bool &dragging, QPoint &lastMousePosition);
void handleMouseRelease(QMouseEvent *event, bool &dragging);
void removeAllColumnsFromTreeView(QTreeView *treeView, int startColumn, int columnCount);
void setupProgressBar(QProgressBar *progressBar, int minValue, int maxValue, int height, bool textVisible);
QPixmap createStatusLightPixmap(const QString &color, int size);
}

// Formatting utility functions
namespace Formatting {
QString formatSize(qint64 size);
QString formatDuration(qint64 milliseconds);
QString formatTimestamp(const QDateTime &datetime, const QString &format);
QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format);
}

// Backup-related utility functions
namespace Backup {
void addSelectedPathsToStaging(QTreeView *treeView, StagingModel *stagingModel);
void removeSelectedPathsFromStaging(QTreeView *treeView, StagingModel *stagingModel);
}

} // namespace Utils

#endif // UTILS_H
