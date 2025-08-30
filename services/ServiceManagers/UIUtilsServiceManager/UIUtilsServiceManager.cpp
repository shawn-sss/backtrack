// Project includes
#include "UIUtilsServiceManager.h"
#include "UIUtilsServiceStyling.h"
#include "UIUtilsServiceConstants.h"

// Qt includes
#include <QPainter>
#include <QTabBar>
#include <QColor>

namespace Shared::UI {

// Window dragging
void handleMousePress(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition) {
    if (!window || !event || event->button() != Qt::LeftButton)
        return;
    dragging = true;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    lastMousePosition = event->globalPosition().toPoint();
#else
    lastMousePosition = event->globalPos();
#endif
    event->accept();
}

void handleMouseMove(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition) {
    if (!window || !event || !dragging)
        return;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QPoint globalPos = event->globalPosition().toPoint();
#else
    const QPoint globalPos = event->globalPos();
#endif
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

// Button styling
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

// Template reset button styling
void UIUtilsServiceManager::applyTemplateResetButtonStyling(QPushButton* button) {
    if (!button)
        return;

    applyButtonStylingWithObjectName(
        button,
        QString::fromLatin1(UIUtilsServiceConstants::TemplateResetTooltip),
        Shared::UI::Styling::Buttons::TemplateResetButtonObjectName
        );

    button->setStyleSheet(Shared::UI::Styling::Buttons::TemplateResetButtonStyle);
}

} // namespace Shared::UI
