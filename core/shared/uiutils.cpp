// Project includes
#include "uiutils.h"

// Qt includes
#include <QCursor>
#include <QFileSystemModel>
#include <QPainter>

namespace Shared::UI {

void handleMousePress(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        lastMousePosition = event->globalPosition().toPoint();
        event->accept();
    }
}

void handleMouseMove(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition) {
    if (dragging && window) {
        QPoint delta = event->globalPosition().toPoint() - lastMousePosition;
        window->move(window->pos() + delta);
        lastMousePosition = event->globalPosition().toPoint();
    }
}

void handleMouseRelease(QMouseEvent* event, bool& dragging) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    }
}

void removeAllColumnsFromTreeView(QTreeView* treeView, int startColumn, int columnCount) {
    if (!treeView || !treeView->model()) return;
    for (int i = startColumn; i < columnCount; ++i) {
        treeView->setColumnHidden(i, true);
    }
}

void setupProgressBar(QProgressBar* progressBar, int minValue, int maxValue, int height, bool textVisible) {
    if (!progressBar) return;
    progressBar->setRange(minValue, maxValue);
    progressBar->setValue(minValue);
    progressBar->setTextVisible(textVisible);
    progressBar->setFixedHeight(height);
}

void setTabWidgetCursorToPointer(QTabWidget* tabWidget) {
    if (tabWidget && tabWidget->tabBar()) {
        tabWidget->tabBar()->setCursor(Qt::PointingHandCursor);
    }
}

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
