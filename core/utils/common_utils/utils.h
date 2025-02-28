#ifndef UTILS_H
#define UTILS_H

#include "../../../ui/customtitlebar/customtitlebar.h"
#include "../../../ui/customtitlebar/titlebarmode.h"
#include "../../../core/backup_module/models/stagingmodel.h"

#include <QString>
#include <QDateTime>
#include <QTreeView>
#include <QProgressBar>
#include <QWidget>
#include <QPixmap>
#include <QPointer>

namespace Utils {

// Custom title bar setup
QPointer<CustomTitleBar> setupCustomTitleBar(QWidget *window, TitleBarMode mode = TitleBarMode::Default);

namespace Formatting {
// Data formatting utilities
QString formatSize(qint64 size);
QString formatDuration(qint64 milliseconds);
QString formatTimestamp(const QDateTime &datetime, const QString &format);
QString formatTimestamp(const QDateTime &datetime, Qt::DateFormat format);
}

namespace UI {
// UI components and utilities
void removeAllColumnsFromTreeView(QTreeView *treeView, int startColumn, int columnCount);
void setupProgressBar(QProgressBar *progressBar, int minValue, int maxValue, int height, bool textVisible);
QPixmap createStatusLightPixmap(const QString &color, int size);
}

namespace Backup {
// Backup-related operations
void addSelectedPathsToStaging(QTreeView *treeView, StagingModel *stagingModel);
void removeSelectedPathsFromStaging(QTreeView *treeView, StagingModel *stagingModel);
}

} // namespace Utils

#endif // UTILS_H
