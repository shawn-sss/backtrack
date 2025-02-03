#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDateTime>
#include <QTreeView>
#include <QProgressBar>
#include <QPixmap>

// Forward-declaration to avoid including the entire header:
class StagingModel;

namespace Utils {

namespace Formatting {
QString formatSize(qint64 size);
QString formatDuration(qint64 milliseconds);
QString formatTimestamp(const QDateTime &datetime, const QString &format);
QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format);
}

namespace UI {
void removeAllColumnsFromTreeView(QTreeView *treeView, int startColumn, int columnCount);
void setupProgressBar(QProgressBar *progressBar, int minValue, int maxValue, int height, bool textVisible);

QPixmap createStatusLightPixmap(const QString &color, int size);
}

namespace Backup {
void addSelectedPathsToStaging(QTreeView *treeView, StagingModel *stagingModel);
void removeSelectedPathsFromStaging(QTreeView *treeView, StagingModel *stagingModel);
}

}

#endif // UTILS_H
