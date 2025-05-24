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

// Handles mouse press for initiating window drag
void handleMousePress(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);

// Handles mouse move to perform window drag
void handleMouseMove(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);

// Handles mouse release to end window drag
void handleMouseRelease(QMouseEvent* event, bool& dragging);

// Hides tree view columns starting at a specific index
void removeAllColumnsFromTreeView(QTreeView* treeView, int startColumn, int columnCount);

// Configures a progress bar with range, height, and text visibility
void setupProgressBar(QProgressBar* progressBar, int minValue, int maxValue, int height, bool textVisible);

// Sets cursor to pointing hand for a tab widget's tab bar
void setTabWidgetCursorToPointer(QTabWidget* tabWidget);

// Creates a circular colored pixmap for status indication
QPixmap createStatusLightPixmap(const QString& color, int size);

} // namespace Shared::UI

#endif // UIUTILS_H
