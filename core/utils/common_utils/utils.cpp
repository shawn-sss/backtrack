#include "utils.h"

#include "../../../../config/_constants.h"
#include "../../../ui/customtitlebar/customtitlebar.h"
#include "../../../core/backup_module/models/stagingmodel.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolBar>
#include <QMouseEvent>
#include <QSet>
#include <QBuffer>
#include <array>
#include <QStringList>
#include <QAbstractItemModel>
#include <QFileSystemModel>
#include <QModelIndexList>
#include <QPainter>
#include <QBrush>
#include <QColor>

namespace Utils {

// UI handling methods
namespace UI {

void handleMousePress(QWidget *window, QMouseEvent *event, bool &dragging, QPoint &lastMousePosition) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        lastMousePosition = event->globalPosition().toPoint() - window->frameGeometry().topLeft();
#else
        lastMousePosition = event->globalPos() - window->frameGeometry().topLeft();
#endif
        event->accept();
    }
}

void handleMouseMove(QWidget *window, QMouseEvent *event, bool &dragging, const QPoint &lastMousePosition) {
    if (dragging && (event->buttons() & Qt::LeftButton)) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        window->move(event->globalPosition().toPoint() - lastMousePosition);
#else
        window->move(event->globalPos() - lastMousePosition);
#endif
        event->accept();
    }
}

void handleMouseRelease(QMouseEvent *event, bool &dragging) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    }
}

void removeAllColumnsFromTreeView(QTreeView *treeView, int startColumn, int columnCount) {
    if (!treeView) return;

    QAbstractItemModel *model = treeView->model();
    if (!model) return;

    for (int i = startColumn; i < columnCount; ++i) {
        if (!treeView->isColumnHidden(i)) {
            treeView->setColumnHidden(i, true);
        }
    }
}

void setupProgressBar(QProgressBar *progressBar, int minValue, int maxValue, int height, bool textVisible) {
    if (!progressBar) return;
    progressBar->setRange(minValue, maxValue);
    progressBar->setValue(ProgressSettings::PROGRESS_BAR_DEFAULT_VISIBILITY ? minValue : maxValue);
    progressBar->setTextVisible(textVisible);
    progressBar->setFixedHeight(height);
}

QPixmap createStatusLightPixmap(const QString &color, int size) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(QColor(color)));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, size, size);

    return pixmap;
}

} // namespace UI

// Formatting methods
namespace Formatting {

QString formatSize(qint64 size) {
    constexpr std::array<const char*, 4> units = {
        Units::FileSize::SIZE_UNIT_BYTES,
        Units::FileSize::SIZE_UNIT_KILOBYTES,
        Units::FileSize::SIZE_UNIT_MEGABYTES,
        Units::FileSize::SIZE_UNIT_GIGABYTES
    };

    int unitIndex = 0;
    double sizeInUnits = static_cast<double>(size);
    while (sizeInUnits >= Units::FileSize::SIZE_CONVERSION_FACTOR && unitIndex < units.size() - 1) {
        sizeInUnits /= Units::FileSize::SIZE_CONVERSION_FACTOR;
        ++unitIndex;
    }
    return QString::number(sizeInUnits, 'f', 2) + " " + units[unitIndex];
}

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

QString formatTimestamp(const QDateTime &datetime, const QString &format) {
    return datetime.toString(format);
}

QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format) {
    return datetime.toString(format);
}

} // namespace Formatting

// Backup staging methods
namespace Backup {

void addSelectedPathsToStaging(QTreeView *treeView, StagingModel *stagingModel) {
    if (!treeView || !stagingModel) return;

    QItemSelectionModel *selectionModel = treeView->selectionModel();
    if (!selectionModel) return;

    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    QSet<QString> uniquePaths;

    for (const QModelIndex &index : std::as_const(selectedIndexes)) {
        QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
        if (!filePath.isEmpty() && !uniquePaths.contains(filePath)) {
            uniquePaths.insert(filePath);
            stagingModel->addPath(filePath);
        }
    }
}

void removeSelectedPathsFromStaging(QTreeView *treeView, StagingModel *stagingModel) {
    if (!treeView || !stagingModel) return;

    QItemSelectionModel *selectionModel = treeView->selectionModel();
    if (!selectionModel) return;

    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
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
