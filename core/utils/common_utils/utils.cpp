#include "utils.h"

#include "../../../../config/_constants.h"
#include "../../../ui/customtitlebar/customtitlebar.h"
#include "../../../core/backup_module/models/stagingmodel.h"

#include <QVBoxLayout>
#include <QMainWindow>
#include <QToolBar>
#include <QSet>
#include <QBuffer>
#include <QVector>
#include <QStringList>
#include <QAbstractItemModel>
#include <QFileSystemModel>
#include <QModelIndexList>
#include <QPainter>
#include <QBrush>
#include <QColor>

namespace Utils {

namespace UI {
// Hide columns in tree view
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

// Configure progress bar settings
void setupProgressBar(QProgressBar *progressBar, int minValue, int maxValue, int height, bool textVisible) {
    if (!progressBar) return;
    progressBar->setRange(minValue, maxValue);
    progressBar->setValue(ProgressConfig::PROGRESS_BAR_DEFAULT_VISIBILITY ? minValue : maxValue);
    progressBar->setTextVisible(textVisible);
    progressBar->setFixedHeight(height);
}

// Create a circular status light
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
}

namespace Formatting {
// Convert size to readable format
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

// Convert milliseconds to readable time
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

// Format timestamp with custom string
QString formatTimestamp(const QDateTime &datetime, const QString &format) {
    return datetime.toString(format);
}

// Format timestamp with Qt format
QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format) {
    return datetime.toString(format);
}
}

namespace Backup {
// Add selected paths to staging
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

// Remove selected paths from staging
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
}

// Setup custom title bar
QPointer<CustomTitleBar> setupCustomTitleBar(QWidget *window, TitleBarMode mode) {
    if (!window) return nullptr;
    window->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    auto existingTitleBar = window->findChild<CustomTitleBar *>();
    if (existingTitleBar) return existingTitleBar;
    auto titleBar = new CustomTitleBar(window);
    titleBar->setFixedHeight(30);
    QObject::connect(titleBar, &CustomTitleBar::minimizeRequested, window, &QWidget::showMinimized);
    QObject::connect(titleBar, &CustomTitleBar::closeRequested, window, &QWidget::close);
    return titleBar;
}

} // namespace Utils
