#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDateTime>
#include <QTreeView>
#include <QProgressBar>

// Utility class for general-purpose helper methods
class Utils {
public:
    // Methods for formatting sizes, durations, and timestamps
    static QString formatSize(qint64 size);
    static QString formatDuration(qint64 milliseconds);
    static QString formatTimestamp(const QDateTime &datetime, const QString &format);
    static QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format);

    // Methods for managing UI components
    static void removeAllColumnsFromTreeView(QTreeView *treeView, int startColumn, int columnCount);
    static void setupProgressBar(QProgressBar *progressBar, int minValue, int maxValue, int height, bool textVisible);
};

#endif // UTILS_H
