#ifndef UTILS_H
#define UTILS_H

// Qt includes
#include <QDateTime>
#include <QMouseEvent>
#include <QPixmap>
#include <QProgressBar>
#include <QTreeView>
#include <QWidget>
#include <QString>
#include <QTabWidget>  // Added for cursor utility

// Forward Declaration (Custom class)
class StagingModel;

namespace Utils {

namespace UI {
// UI interaction utilities
void handleMousePress(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);
void handleMouseMove(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);
void handleMouseRelease(QMouseEvent* event, bool& dragging);
void removeAllColumnsFromTreeView(QTreeView* treeView, int startColumn, int columnCount);
void setupProgressBar(QProgressBar* progressBar, int minValue, int maxValue, int height, bool textVisible);
QPixmap createStatusLightPixmap(const QString& color, int size);
void setTabWidgetCursorToPointer(QTabWidget* tabWidget);  // NEW FUNCTION
}

namespace Formatting {
// Formatting utilities for size, time, and timestamps
QString formatSize(qint64 size);
QString formatDuration(qint64 milliseconds);
QString formatTimestamp(const QDateTime& datetime, const QString& format);
QString formatTimestamp(const QDateTime& datetime, Qt::DateFormat format);
}

namespace Backup {
// Helpers for interacting with the staging model
void addSelectedPathsToStaging(QTreeView* treeView, StagingModel* stagingModel);
void removeSelectedPathsFromStaging(QTreeView* treeView, StagingModel* stagingModel);
}

} // namespace Utils

#endif // UTILS_H
