// Project includes
#include "UIUtilsServiceManager.h"

// Qt includes
#include <QCursor>
#include <QFileSystemModel>
#include <QPainter>
#include <QTabBar>

namespace Shared::UI {

// Handles window dragging on mouse press
void handleMousePress(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition) {
    if (!window || !event || event->button() != Qt::LeftButton)
        return;

    dragging = true;
    lastMousePosition = event->globalPosition().toPoint();
    event->accept();
}

// Moves window while mouse is dragged
void handleMouseMove(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition) {
    if (!window || !event || !dragging)
        return;

    const QPoint globalPos = event->globalPosition().toPoint();
    const QPoint delta = globalPos - lastMousePosition;
    window->move(window->pos() + delta);
    lastMousePosition = globalPos;
}

// Ends window dragging on mouse release
void handleMouseRelease(QMouseEvent* event, bool& dragging) {
    if (!event || event->button() != Qt::LeftButton)
        return;

    dragging = false;
    event->accept();
}

// Hides columns in a QTreeView starting from a given index
void removeAllColumnsFromTreeView(QTreeView* treeView, int startColumn, int columnCount) {
    if (!treeView || !treeView->model())
        return;

    for (int i = startColumn; i < columnCount; ++i) {
        treeView->setColumnHidden(i, true);
    }
}

// Configures a QProgressBar with range, height, and text visibility
void setupProgressBar(QProgressBar* progressBar, int minValue, int maxValue, int height, bool textVisible) {
    if (!progressBar)
        return;

    progressBar->setRange(minValue, maxValue);
    progressBar->setValue(minValue);
    progressBar->setTextVisible(textVisible);
    progressBar->setFixedHeight(height);
}

// Changes the cursor on QTabWidget to a pointing hand
void setTabWidgetCursorToPointer(QTabWidget* tabWidget) {
    if (tabWidget && tabWidget->tabBar()) {
        tabWidget->tabBar()->setCursor(Qt::PointingHandCursor);
    }
}

// Creates a circular colored pixmap (status light)
QPixmap createStatusLightPixmap(const QString& color, int size) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(color));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, size, size);

    return pixmap;
}

} // namespace Shared::UI
