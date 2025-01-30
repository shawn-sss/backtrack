#include "utils.h"
#include <QAbstractItemModel>
#include <QVector>

// Format disk size into a human-readable string
QString Utils::formatSize(qint64 size) {
    static const QVector<QString> units{" B", " KB", " MB", " GB"};
    int unitIndex = 0;
    double sizeInUnits = size;

    while (sizeInUnits >= 1024 && unitIndex < units.size() - 1) {
        sizeInUnits /= 1024.0;
        ++unitIndex;
    }

    return QString::number(sizeInUnits, 'f', 2) + units[unitIndex];
}

// Format duration into a human-readable string
QString Utils::formatDuration(qint64 milliseconds) {
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

// Format a timestamp using a custom string format
QString Utils::formatTimestamp(const QDateTime &datetime, const QString &format) {
    return datetime.toString(format);
}

// Overload: format timestamp using Qt::DateFormat
QString Utils::formatTimestamp(const QDateTime &datetime, Qt::DateFormat format) {
    return datetime.toString(format);
}

// Hide extra columns in QTreeView
void Utils::removeAllColumnsFromTreeView(QTreeView *treeView, int startColumn, int columnCount) {
    QAbstractItemModel *model = treeView->model();
    if (model) {
        for (int i = startColumn; i < columnCount; ++i) {
            treeView->setColumnHidden(i, true);
        }
    }
}

// Setup a progress bar with custom settings
void Utils::setupProgressBar(QProgressBar *progressBar,
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
