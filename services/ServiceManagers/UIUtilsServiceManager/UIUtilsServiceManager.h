#ifndef UIUTILS_H
#define UIUTILS_H

// Qt includes
#include <QMouseEvent>
#include <QPixmap>
#include <QProgressBar>
#include <QTabWidget>
#include <QTreeView>
#include <QWidget>
#include <QString>

namespace Shared::UI {

// Window dragging helpers
void handleMousePress(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);
void handleMouseMove(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);
void handleMouseRelease(QMouseEvent* event, bool& dragging);

// Tree view column visibility
void removeAllColumnsFromTreeView(QTreeView* treeView, int startColumn, int columnCount);

// Progress bar setup
void setupProgressBar(QProgressBar* progressBar, int minValue, int maxValue, int height, bool textVisible);

// Tab widget cursor customization
void setTabWidgetCursorToPointer(QTabWidget* tabWidget);

// Status light pixmap creation
QPixmap createStatusLightPixmap(const QString& color, int size);

} // namespace Shared::UI

#endif // UIUTILS_H
