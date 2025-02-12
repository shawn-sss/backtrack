#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDateTime>
#include <QTreeView>
#include <QProgressBar>
#include <QPixmap>

// Forward declaration to avoid unnecessary includes
class StagingModel;

namespace Utils {

// Formatting utilities
namespace Formatting {
QString formatSize(qint64 size);
QString formatDuration(qint64 milliseconds);
QString formatTimestamp(const QDateTime &datetime, const QString &format);
QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format);
}

// UI utilities
namespace UI {
void removeAllColumnsFromTreeView(QTreeView *treeView, int startColumn, int columnCount);
void setupProgressBar(QProgressBar *progressBar, int minValue, int maxValue, int height, bool textVisible);
QPixmap createStatusLightPixmap(const QString &color, int size);
}

// Backup utilities
namespace Backup {
void addSelectedPathsToStaging(QTreeView *treeView, StagingModel *stagingModel);
void removeSelectedPathsFromStaging(QTreeView *treeView, StagingModel *stagingModel);
}

} // namespace Utils

#endif // UTILS_H
