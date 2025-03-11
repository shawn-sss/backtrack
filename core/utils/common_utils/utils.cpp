// Project includes same directory
#include "utils.h"

// Project includes different directory
#include "../../../../config/_constants.h"
#include "../../../core/backup_module/models/stagingmodel.h"

// Built-in Qt includes
#include <QMouseEvent>
#include <QProgressBar>
#include <QPainter>
#include <QSet>
#include <QTreeView>
#include <QFileSystemModel>


namespace Utils {

// UI-related utilities
namespace UI {

// Handles window dragging on mouse press
void handleMousePress(QWidget *window, QMouseEvent *event, bool &dragging, QPoint &lastMousePosition) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        lastMousePosition = event->globalPosition().toPoint();
        event->accept();
    }
}

// Handles window movement on mouse drag
void handleMouseMove(QWidget *window, QMouseEvent *event, bool &dragging, QPoint &lastMousePosition) {
    if (dragging && window) {
        QPoint delta = event->globalPosition().toPoint() - lastMousePosition;
        window->move(window->pos() + delta);
        lastMousePosition = event->globalPosition().toPoint();
    }
}

// Handles mouse release event
void handleMouseRelease(QMouseEvent *event, bool &dragging) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    }
}

// Hides specific columns in a QTreeView
void removeAllColumnsFromTreeView(QTreeView *treeView, int startColumn, int columnCount) {
    if (!treeView || !treeView->model()) return;

    for (int i = startColumn; i < columnCount; ++i) {
        treeView->setColumnHidden(i, true);
    }
}

// Configures a progress bar
void setupProgressBar(QProgressBar *progressBar, int minValue, int maxValue, int height, bool textVisible) {
    if (!progressBar) return;

    progressBar->setRange(minValue, maxValue);
    progressBar->setValue(minValue);
    progressBar->setTextVisible(textVisible);
    progressBar->setFixedHeight(height);
}

// Creates a status light indicator
QPixmap createStatusLightPixmap(const QString &color, int size) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(color));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, size, size);

    return pixmap;
}

} // namespace UI

// Formatting utilities
namespace Formatting {

// Formats file size into readable units
QString formatSize(qint64 size) {
    constexpr std::array units = {
        Units::FileSize::SIZE_UNIT_BYTES,
        Units::FileSize::SIZE_UNIT_KILOBYTES,
        Units::FileSize::SIZE_UNIT_MEGABYTES,
        Units::FileSize::SIZE_UNIT_GIGABYTES
    };

    int unitIndex = 0;
    double sizeInUnits = size;

    while (sizeInUnits >= Units::FileSize::SIZE_CONVERSION_FACTOR && unitIndex < units.size() - 1) {
        sizeInUnits /= Units::FileSize::SIZE_CONVERSION_FACTOR;
        ++unitIndex;
    }

    return QString::number(sizeInUnits, 'f', 2) + " " + units[unitIndex];
}

// Formats duration into human-readable format
QString formatDuration(qint64 milliseconds) {
    constexpr qint64 MS_IN_SECOND = 1000;
    constexpr qint64 SECONDS_IN_MINUTE = 60;
    constexpr qint64 MINUTES_IN_HOUR = 60;
    constexpr qint64 HOURS_IN_DAY = 24;

    if (milliseconds < MS_IN_SECOND)
        return QString::number(milliseconds) + Units::Time::UNIT_MILLISECONDS;

    qint64 seconds = milliseconds / MS_IN_SECOND;
    if (seconds < SECONDS_IN_MINUTE)
        return QString::number(seconds) + Units::Time::UNIT_SECONDS;

    qint64 minutes = seconds / SECONDS_IN_MINUTE;
    if (minutes < MINUTES_IN_HOUR)
        return QString::number(minutes) + Units::Time::UNIT_MINUTES;

    qint64 hours = minutes / MINUTES_IN_HOUR;
    if (hours < HOURS_IN_DAY)
        return QString::number(hours) + Units::Time::UNIT_HOURS;

    qint64 days = hours / HOURS_IN_DAY;
    return QString::number(days) + Units::Time::UNIT_DAYS;
}

// Formats timestamps
QString formatTimestamp(const QDateTime &datetime, const QString &format) {
    return datetime.toString(format);
}

QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format) {
    return datetime.toString(format);
}

} // namespace Formatting

// Backup utilities
namespace Backup {

// Adds selected paths from a QTreeView to the staging model
void addSelectedPathsToStaging(QTreeView *treeView, StagingModel *stagingModel) {
    if (!treeView || !stagingModel || !treeView->selectionModel()) return;

    QSet<QString> uniquePaths;
    const auto selectedIndexes = treeView->selectionModel()->selectedIndexes();

    for (const auto &index : selectedIndexes) {
        QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
        if (!filePath.isEmpty() && !uniquePaths.contains(filePath)) {
            uniquePaths.insert(filePath);
            stagingModel->addPath(filePath);
        }
    }
}

// Removes selected paths from the staging model
void removeSelectedPathsFromStaging(QTreeView *treeView, StagingModel *stagingModel) {
    if (!treeView || !stagingModel || !treeView->selectionModel()) return;

    QSet<QString> uniquePathsToRemove;
    const auto selectedIndexes = treeView->selectionModel()->selectedIndexes();

    for (const auto &index : selectedIndexes) {
        QString filePath = stagingModel->data(index, Qt::ToolTipRole).toString();
        if (!filePath.isEmpty()) {
            uniquePathsToRemove.insert(filePath);
        }
    }

    for (const auto &filePath : uniquePathsToRemove) {
        stagingModel->removePath(filePath);
    }
}

} // namespace Backup

} // namespace Utils
