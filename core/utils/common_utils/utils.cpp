// Project includes
#include "../../../../config/configsettings/_settings.h"
#include "../../../core/backup_module/models/stagingmodel.h"
#include "utils.h"

// Qt includes
#include <QSet>
#include <QMouseEvent>
#include <QPainter>
#include <QProgressBar>
#include <QTreeView>
#include <QFileSystemModel>

namespace Utils {

namespace UI {

// Enables dragging a window
void handleMousePress(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        lastMousePosition = event->globalPosition().toPoint();
        event->accept();
    }
}

// Moves the window during drag
void handleMouseMove(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition) {
    if (dragging && window) {
        QPoint delta = event->globalPosition().toPoint() - lastMousePosition;
        window->move(window->pos() + delta);
        lastMousePosition = event->globalPosition().toPoint();
    }
}

// Ends dragging on mouse release
void handleMouseRelease(QMouseEvent* event, bool& dragging) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    }
}

// Hides tree view columns starting from a column index
void removeAllColumnsFromTreeView(QTreeView* treeView, int startColumn, int columnCount) {
    if (!treeView || !treeView->model()) return;
    for (int i = startColumn; i < columnCount; ++i) {
        treeView->setColumnHidden(i, true);
    }
}

// Configures a progress bar's appearance and behavior
void setupProgressBar(QProgressBar* progressBar, int minValue, int maxValue, int height, bool textVisible) {
    if (!progressBar) return;
    progressBar->setRange(minValue, maxValue);
    progressBar->setValue(minValue);
    progressBar->setTextVisible(textVisible);
    progressBar->setFixedHeight(height);
}

// Returns a circular status light pixmap
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

} // namespace UI

namespace Formatting {

// Converts bytes to human-readable size string
QString formatSize(qint64 size) {
    constexpr std::array units = {
        Units::FileSize::k_SIZE_UNIT_BYTES,
        Units::FileSize::k_SIZE_UNIT_KILOBYTES,
        Units::FileSize::k_SIZE_UNIT_MEGABYTES,
        Units::FileSize::k_SIZE_UNIT_GIGABYTES
    };

    int unitIndex = 0;
    double sizeInUnits = size;

    while (sizeInUnits >= Units::FileSize::k_SIZE_CONVERSION_FACTOR && unitIndex < units.size() - 1) {
        sizeInUnits /= Units::FileSize::k_SIZE_CONVERSION_FACTOR;
        ++unitIndex;
    }

    return QString::number(sizeInUnits, 'f', 2) + " " + units[unitIndex];
}

// Converts milliseconds to readable duration
QString formatDuration(qint64 milliseconds) {
    constexpr qint64 MS_IN_SECOND = 1000;
    constexpr qint64 SECONDS_IN_MINUTE = 60;
    constexpr qint64 MINUTES_IN_HOUR = 60;
    constexpr qint64 HOURS_IN_DAY = 24;

    if (milliseconds < MS_IN_SECOND)
        return QString::number(milliseconds) + Units::Time::k_UNIT_MILLISECONDS;

    qint64 seconds = milliseconds / MS_IN_SECOND;
    if (seconds < SECONDS_IN_MINUTE)
        return QString::number(seconds) + Units::Time::k_UNIT_SECONDS;

    qint64 minutes = seconds / SECONDS_IN_MINUTE;
    if (minutes < MINUTES_IN_HOUR)
        return QString::number(minutes) + Units::Time::k_UNIT_MINUTES;

    qint64 hours = minutes / MINUTES_IN_HOUR;
    if (hours < HOURS_IN_DAY)
        return QString::number(hours) + Units::Time::k_UNIT_HOURS;

    qint64 days = hours / HOURS_IN_DAY;
    return QString::number(days) + Units::Time::k_UNIT_DAYS;
}

// Converts QDateTime to formatted string
QString formatTimestamp(const QDateTime& datetime, const QString& format) {
    return datetime.toString(format);
}

// Converts QDateTime using Qt::DateFormat
QString formatTimestamp(const QDateTime& datetime, Qt::DateFormat format) {
    return datetime.toString(format);
}

} // namespace Formatting

namespace Backup {

// Adds selected items from tree to staging
void addSelectedPathsToStaging(QTreeView* treeView, StagingModel* stagingModel) {
    if (!treeView || !stagingModel || !treeView->selectionModel()) return;

    QSet<QString> uniquePaths;
    const auto selectedIndexes = treeView->selectionModel()->selectedIndexes();

    for (const auto& index : selectedIndexes) {
        QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
        if (!filePath.isEmpty() && !uniquePaths.contains(filePath)) {
            uniquePaths.insert(filePath);
            stagingModel->addPath(filePath);
        }
    }
}

// Removes selected items from staging
void removeSelectedPathsFromStaging(QTreeView* treeView, StagingModel* stagingModel) {
    if (!treeView || !stagingModel || !treeView->selectionModel()) return;

    QSet<QString> uniquePathsToRemove;
    const auto selectedIndexes = treeView->selectionModel()->selectedIndexes();

    for (const auto& index : selectedIndexes) {
        QString filePath = stagingModel->data(index, Qt::ToolTipRole).toString();
        if (!filePath.isEmpty()) {
            uniquePathsToRemove.insert(filePath);
        }
    }

    for (const auto& filePath : uniquePathsToRemove) {
        stagingModel->removePath(filePath);
    }
}

} // namespace Backup

} // namespace Utils
