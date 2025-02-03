#include "utils.h"

#include <QAbstractItemModel>
#include <QVector>
#include <QFileSystemModel>
#include <QModelIndexList>
#include <QStringList>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QBuffer>
#include <utility> // for std::as_const

#include "../BackupModule/stagingmodel.h"
#include "constants.h"

namespace Utils {

namespace Formatting {

QString formatSize(qint64 size) {
    static const QVector<QString> units{" B", " KB", " MB", " GB"};
    int unitIndex = 0;
    double sizeInUnits = size;

    while (sizeInUnits >= 1024 && unitIndex < units.size() - 1) {
        sizeInUnits /= 1024.0;
        ++unitIndex;
    }

    return QString::number(sizeInUnits, 'f', 2) + units[unitIndex];
}

QString formatDuration(qint64 milliseconds) {
    if (milliseconds < 1000) {
        return QString::number(milliseconds) + " milliseconds";
    }

    qint64 seconds = milliseconds / 1000;
    if (seconds < 60) {
        return QString::number(seconds) + " seconds";
    }

    qint64 minutes = seconds / 60;
    if (minutes < 60) {
        return QString::number(minutes) + " minutes";
    }

    qint64 hours = minutes / 60;
    if (hours < 24) {
        return QString::number(hours) + " hours";
    }

    qint64 days = hours / 24;
    return QString::number(days) + " days";
}

QString formatTimestamp(const QDateTime &datetime, const QString &format) {
    return datetime.toString(format);
}

QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format) {
    return datetime.toString(format);
}

}

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

void setupProgressBar(QProgressBar *progressBar,
                      int minValue,
                      int maxValue,
                      int height,
                      bool textVisible) {
    if (!progressBar) return;

    progressBar->setRange(minValue, maxValue);
    progressBar->setValue(0);
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

}

namespace Backup {

void addSelectedPathsToStaging(QTreeView *treeView, StagingModel *stagingModel)
{
    if (!treeView || !stagingModel) return;

    QModelIndexList selectedIndexes = treeView->selectionModel()->selectedIndexes();
    for (const QModelIndex &index : std::as_const(selectedIndexes)) {
        QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
        stagingModel->addPath(filePath);
    }
}

void removeSelectedPathsFromStaging(QTreeView *treeView, StagingModel *stagingModel)
{
    if (!treeView || !stagingModel) return;

    QModelIndexList selectedIndexes = treeView->selectionModel()->selectedIndexes();
    QStringList filePathsToRemove;
    for (const QModelIndex &index : std::as_const(selectedIndexes)) {
        QString filePath = stagingModel->data(index, Qt::ToolTipRole).toString();
        filePathsToRemove.append(filePath);
    }

    for (const QString &filePath : filePathsToRemove) {
        stagingModel->removePath(filePath);
    }
}

}

}
