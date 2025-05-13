#ifndef UIUTILS_H
#define UIUTILS_H

// Qt includes
#include <QMouseEvent>
#include <QPixmap>
#include <QProgressBar>
#include <QTreeView>
#include <QTabWidget>
#include <QWidget>
#include <QString>

namespace Shared::UI {

// Mouse event handlers for draggable windows
void handleMousePress(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);
void handleMouseMove(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);
void handleMouseRelease(QMouseEvent* event, bool& dragging);

// Tree view utility
void removeAllColumnsFromTreeView(QTreeView* treeView, int startColumn, int columnCount);

// Progress bar setup utility
void setupProgressBar(QProgressBar* progressBar, int minValue, int maxValue, int height, bool textVisible);

// UI cursor customization
void setTabWidgetCursorToPointer(QTabWidget* tabWidget);

// Status light icon generator
QPixmap createStatusLightPixmap(const QString& color, int size);

} // namespace Shared::UI

#endif // UIUTILS_H
