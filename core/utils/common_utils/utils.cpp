#include "utils.h"
#include "../../../core/backup_module/models/stagingmodel.h"
#include "../../../core/config/constants.h"

#include <QAbstractItemModel>
#include <QVector>
#include <QFileSystemModel>
#include <QModelIndexList>
#include <QStringList>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QBuffer>
#include <utility>

namespace Utils {

// Formatting Functions for sizes, durations, and timestamps
namespace Formatting {

QString formatSize(qint64 size) {
    static const QVector<QString> units{
        BackupInfo::SIZE_UNIT_BYTES,
        BackupInfo::SIZE_UNIT_KILOBYTES,
        BackupInfo::SIZE_UNIT_MEGABYTES,
        BackupInfo::SIZE_UNIT_GIGABYTES
    };

    int unitIndex = 0;
    double sizeInUnits = size;

    while (sizeInUnits >= BackupInfo::SIZE_CONVERSION_FACTOR && unitIndex < units.size() - 1) {
        sizeInUnits /= BackupInfo::SIZE_CONVERSION_FACTOR;
        ++unitIndex;
    }

    return QString::number(sizeInUnits, 'f', 2) + units[unitIndex];
}

QString formatDuration(qint64 milliseconds) {
    if (milliseconds < 1000) return QString::number(milliseconds) + " milliseconds";
    qint64 seconds = milliseconds / 1000;
    if (seconds < 60) return QString::number(seconds) + " seconds";
    qint64 minutes = seconds / 60;
    if (minutes < 60) return QString::number(minutes) + " minutes";
    qint64 hours = minutes / 60;
    if (hours < 24) return QString::number(hours) + " hours";
    qint64 days = hours / 24;
    return QString::number(days) + " days";
}

QString formatTimestamp(const QDateTime &datetime, const QString &format) {
    return datetime.toString(format);
}

QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format) {
    return datetime.toString(format);
}

} // namespace Formatting

// UI Functions for tree views, progress bars, and status lights
namespace UI {

void removeAllColumnsFromTreeView(QTreeView *treeView, int startColumn, int columnCount) {
    if (!treeView) return;

    QAbstractItemModel *model = treeView->model();
    if (model) {
        for (int i = startColumn; i < columnCount; ++i) {
            treeView->setColumnHidden(i, true);
        }
    }
}

void setupProgressBar(QProgressBar *progressBar, int minValue, int maxValue, int height, bool textVisible) {
    if (!progressBar) return;

    progressBar->setRange(minValue, maxValue);
    progressBar->setValue(ProgressConfig::DEFAULT_VISIBILITY ? minValue : 0);
    progressBar->setTextVisible(textVisible);
    progressBar->setFixedHeight(height);
}

QPixmap createStatusLightPixmap(const QString &color, int size) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(QColor(color)));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, size, size);
    painter.end();

    return pixmap;
}

} // namespace UI

// Backup Functions for managing staged paths
namespace Backup {

void addSelectedPathsToStaging(QTreeView *treeView, StagingModel *stagingModel) {
    if (!treeView || !stagingModel) return;

    QModelIndexList selectedIndexes = treeView->selectionModel()->selectedIndexes();
    for (const QModelIndex &index : std::as_const(selectedIndexes)) {
        QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
        if (!filePath.isEmpty()) {
            stagingModel->addPath(filePath);
        }
    }
}

void removeSelectedPathsFromStaging(QTreeView *treeView, StagingModel *stagingModel) {
    if (!treeView || !stagingModel) return;

    QModelIndexList selectedIndexes = treeView->selectionModel()->selectedIndexes();
    QStringList filePathsToRemove;

    for (const QModelIndex &index : std::as_const(selectedIndexes)) {
        QString filePath = stagingModel->data(index, Qt::ToolTipRole).toString();
        if (!filePath.isEmpty()) {
            filePathsToRemove.append(filePath);
        }
    }

    for (const QString &filePath : filePathsToRemove) {
        stagingModel->removePath(filePath);
    }
}

} // namespace Backup

} // namespace Utils
