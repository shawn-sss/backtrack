#include "UIUtilsServiceManager.h"

// Qt includes
#include <QCursor>
#include <QFileSystemModel>
#include <QPainter>
#include <QTabBar>

namespace Shared::UI {

// Window dragging helpers
void handleMousePress(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition) {
    if (!window || !event || event->button() != Qt::LeftButton)
        return;
    dragging = true;
    lastMousePosition = event->globalPosition().toPoint();
    event->accept();
}

void handleMouseMove(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition) {
    if (!window || !event || !dragging)
        return;
    const QPoint globalPos = event->globalPosition().toPoint();
    const QPoint delta = globalPos - lastMousePosition;
    window->move(window->pos() + delta);
    lastMousePosition = globalPos;
}

void handleMouseRelease(QMouseEvent* event, bool& dragging) {
    if (!event || event->button() != Qt::LeftButton)
        return;
    dragging = false;
    event->accept();
}

// Tree view utilities
void removeAllColumnsFromTreeView(QTreeView* treeView, int startColumn, int columnCount) {
    if (!treeView || !treeView->model())
        return;
    for (int i = startColumn; i < columnCount; ++i)
        treeView->setColumnHidden(i, true);
}

// Progress bar setup
void setupProgressBar(QProgressBar* progressBar, int minValue, int maxValue, int height, bool textVisible) {
    if (!progressBar)
        return;
    progressBar->setRange(minValue, maxValue);
    progressBar->setValue(minValue);
    progressBar->setTextVisible(textVisible);
    progressBar->setFixedHeight(height);
}

// Tab widget utilities
void setTabWidgetCursorToPointer(QTabWidget* tabWidget) {
    if (tabWidget && tabWidget->tabBar())
        tabWidget->tabBar()->setCursor(Qt::PointingHandCursor);
}

// Pixmap generation
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

// Button styling utilities
void applyButtonTooltipAndCursor(QPushButton* button, const QString& tooltip) {
    if (!button)
        return;
    button->setCursor(Qt::PointingHandCursor);
    button->setToolTip(tooltip);
}

void applyButtonStyling(const QList<QPair<QPushButton*, QString>>& buttonTooltipPairs) {
    for (const auto& pair : buttonTooltipPairs) {
        applyButtonTooltipAndCursor(pair.first, pair.second);
    }
}

void applyButtonStylingWithObjectName(QPushButton* button, const QString& tooltip, const QString& objectName) {
    if (!button)
        return;
    button->setCursor(Qt::PointingHandCursor);
    button->setToolTip(tooltip);
    button->setObjectName(objectName);
}

} // namespace Shared::UI
