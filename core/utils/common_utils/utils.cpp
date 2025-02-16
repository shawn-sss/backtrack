#include "utils.h"
#include "../../../core/backup_module/models/stagingmodel.h"
#include "../../../core/config/_constants.h"

#include <QAbstractItemModel>
#include <QVector>
#include <QFileSystemModel>
#include <QModelIndexList>
#include <QStringList>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QBuffer>
#include <QSet>

namespace Utils {

// Formatting Functions
namespace Formatting {

// Formats file size into human-readable units
QString formatSize(qint64 size) {
    static const QVector<QString> units{
        Numbers::SIZE_UNIT_BYTES,
        Numbers::SIZE_UNIT_KILOBYTES,
        Numbers::SIZE_UNIT_MEGABYTES,
        Numbers::SIZE_UNIT_GIGABYTES
    };

    int unitIndex = 0;
    double sizeInUnits = size;

    while (sizeInUnits >= Numbers::SIZE_CONVERSION_FACTOR && unitIndex < units.size() - 1) {
        sizeInUnits /= Numbers::SIZE_CONVERSION_FACTOR;
        ++unitIndex;
    }

    return QString::number(sizeInUnits, 'f', 2) + units[unitIndex];
}

// Formats duration into human-readable time units
QString formatDuration(qint64 milliseconds) {
    if (milliseconds < 1000) return QString::number(milliseconds) + TimeUnits::UNIT_MILLISECONDS;
    qint64 seconds = milliseconds / 1000;
    if (seconds < 60) return QString::number(seconds) + TimeUnits::UNIT_SECONDS;
    qint64 minutes = seconds / 60;
    if (minutes < 60) return QString::number(minutes) + TimeUnits::UNIT_MINUTES;
    qint64 hours = minutes / 60;
    if (hours < 24) return QString::number(hours) + TimeUnits::UNIT_HOURS;
    qint64 days = hours / 24;
    return QString::number(days) + TimeUnits::UNIT_DAYS;
}

// Formats timestamp using a custom format string
QString formatTimestamp(const QDateTime &datetime, const QString &format) {
    return datetime.toString(format);
}

// Formats timestamp using a predefined Qt DateFormat
QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format) {
    return datetime.toString(format);
}

} // namespace Formatting

// UI Functions
namespace UI {

// Hides columns in a QTreeView from a given start column
void removeAllColumnsFromTreeView(QTreeView *treeView, int startColumn, int columnCount) {
    if (!treeView) return;

    QAbstractItemModel *model = treeView->model();
    if (model) {
        for (int i = startColumn; i < columnCount; ++i) {
            if (!treeView->isColumnHidden(i)) {
                treeView->setColumnHidden(i, true);
            }
        }
    }
}

// Sets up a QProgressBar with given settings
void setupProgressBar(QProgressBar *progressBar, int minValue, int maxValue, int height, bool textVisible) {
    if (!progressBar) return;

    progressBar->setRange(minValue, maxValue);
    progressBar->setValue(ProgressConfig::PROGRESS_BAR_DEFAULT_VISIBILITY ? minValue : maxValue);
    progressBar->setTextVisible(textVisible);
    progressBar->setFixedHeight(height);
}

// Creates a circular status light icon with a specified color and size
QPixmap createStatusLightPixmap(const QString &color, int size) {
    QPixmap pixmap(size, size);
    pixmap.fill(Colors::COLOR_TRANSPARENT);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(QColor(color)));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, size, size);

    return pixmap;
}

} // namespace UI

// Backup Functions
namespace Backup {

// Adds selected file paths from a QTreeView to the StagingModel
void addSelectedPathsToStaging(QTreeView *treeView, StagingModel *stagingModel) {
    if (!treeView || !stagingModel) return;

    QModelIndexList selectedIndexes = treeView->selectionModel()->selectedIndexes();
    QSet<QString> uniquePaths;

    for (const QModelIndex &index : std::as_const(selectedIndexes)) {
        QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
        if (!filePath.isEmpty() && !uniquePaths.contains(filePath)) {
            uniquePaths.insert(filePath);
            stagingModel->addPath(filePath);
        }
    }
}

// Removes selected file paths from a QTreeView in the StagingModel
void removeSelectedPathsFromStaging(QTreeView *treeView, StagingModel *stagingModel) {
    if (!treeView || !stagingModel) return;

    QModelIndexList selectedIndexes = treeView->selectionModel()->selectedIndexes();
    QSet<QString> uniquePathsToRemove;

    for (const QModelIndex &index : std::as_const(selectedIndexes)) {
        QString filePath = stagingModel->data(index, Qt::ToolTipRole).toString();
        if (!filePath.isEmpty() && !uniquePathsToRemove.contains(filePath)) {
            uniquePathsToRemove.insert(filePath);
        }
    }

    for (const QString &filePath : uniquePathsToRemove) {
        stagingModel->removePath(filePath);
    }
}

} // namespace Backup

} // namespace Utils
