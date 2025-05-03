#ifndef UTILS_H
#define UTILS_H

// Qt includes
#include <QDateTime>
#include <QPixmap>
#include <QProgressBar>
#include <QMouseEvent>
#include <QTreeView>
#include <QWidget>
#include <QString>

// Forward declarations (Custom classes)
class StagingModel;

namespace Utils {

namespace UI {
// UI utility functions for mouse events and tree/progress widgets
void handleMousePress(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);
void handleMouseMove(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);
void handleMouseRelease(QMouseEvent* event, bool& dragging);
void removeAllColumnsFromTreeView(QTreeView* treeView, int startColumn, int columnCount);
void setupProgressBar(QProgressBar* progressBar, int minValue, int maxValue, int height, bool textVisible);
QPixmap createStatusLightPixmap(const QString& color, int size);
}

namespace Formatting {
// Functions for formatting data (size, time, etc.)
QString formatSize(qint64 size);
QString formatDuration(qint64 milliseconds);
QString formatTimestamp(const QDateTime& datetime, const QString& format);
QString formatTimestamp(const QDateTime& datetime, Qt::DateFormat format);
}

namespace Backup {
// Backup-related functions for staging model interaction
void addSelectedPathsToStaging(QTreeView* treeView, StagingModel* stagingModel);
void removeSelectedPathsFromStaging(QTreeView* treeView, StagingModel* stagingModel);
}

} // namespace Utils

#endif // UTILS_H
