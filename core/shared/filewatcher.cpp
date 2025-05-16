#include "filewatcher.h"
#include "../../../../constants/backup_config.h"

#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QSet>
#include <QDebug>

FileWatcher::FileWatcher(QObject* parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

void FileWatcher::updateWatchList(const QStringList& paths) {
    removeAllPaths();
    addPaths(paths);
}

static QString normalizeAndCheckPath(const QString& path) {
    QString normalized = QDir::fromNativeSeparators(path.trimmed());
    return QFileInfo::exists(normalized) ? normalized : QString();
}

void FileWatcher::addPath(const QString& path) {
    const QString normalizedPath = normalizeAndCheckPath(path);
    if (!normalizedPath.isEmpty() &&
        !watcher->directories().contains(normalizedPath) &&
        !watcher->files().contains(normalizedPath)) {
        watcher->addPath(normalizedPath);
        qDebug() << "[FileWatcher] Watching path:" << normalizedPath;
    }
}

void FileWatcher::addPaths(const QStringList& paths) {
    QStringList newPaths;
    QSet<QString> currentWatched(watcher->directories().begin(), watcher->directories().end());
    currentWatched.unite(QSet<QString>(watcher->files().begin(), watcher->files().end()));

    for (const QString& path : paths) {
        const QString normalizedPath = normalizeAndCheckPath(path);
        if (!normalizedPath.isEmpty() && !currentWatched.contains(normalizedPath)) {
            newPaths.append(normalizedPath);
        }
    }

    if (!newPaths.isEmpty()) {
        watcher->addPaths(newPaths);
        qDebug() << "[FileWatcher] Added paths:" << newPaths;
    }
}

void FileWatcher::removePath(const QString& path) {
    const QString normalizedPath = QDir::fromNativeSeparators(path.trimmed());
    if (watcher->directories().contains(normalizedPath) || watcher->files().contains(normalizedPath)) {
        watcher->removePath(normalizedPath);
        qDebug() << "[FileWatcher] Removed path:" << normalizedPath;
    }
}

void FileWatcher::removeAllPaths() {
    QStringList allPaths = watcher->directories() + watcher->files();
    if (!allPaths.isEmpty()) {
        watcher->removePaths(allPaths);
        qDebug() << "[FileWatcher] Removed all watched paths";
    }
}

QStringList FileWatcher::watchedDirectories() const {
    return watcher->directories();
}

QStringList FileWatcher::watchedFiles() const {
    return watcher->files();
}

void FileWatcher::startWatchingMultiple(const QStringList&) {
    removeAllPaths();
    QSet<QString> pathsToWatch;

    // === AppInstall paths ===
    QString appInstallRoot     = "C:/Users/shawn/AppData/Local/MyDataBackupApp";
    QString appConfigFolder    = "C:/Users/shawn/AppData/Local/MyDataBackupApp/app_config";
    QString appInit            = "C:/Users/shawn/AppData/Local/MyDataBackupApp/app_config/app_init.json";
    QString appNotifications   = "C:/Users/shawn/AppData/Local/MyDataBackupApp/app_config/app_notifications.json";
    QString userSettings       = "C:/Users/shawn/AppData/Local/MyDataBackupApp/app_config/user_settings.json";

    // === BackupLocation paths ===
    QString rootBackup         = "C:/temp";
    QString backupFolder       = "C:/temp/_MyDataBackupApp";
    QString backupInit         = "C:/temp/_MyDataBackupApp/backup_init.json";
    QString logsFolder         = "C:/temp/_MyDataBackupApp/logs";

    // Add all 9 important paths
    QStringList criticalPaths = {
        appInstallRoot,
        appConfigFolder,
        appInit,
        appNotifications,
        userSettings,
        rootBackup,
        backupFolder,
        backupInit,
        logsFolder
    };

    for (const QString& path : criticalPaths) {
        const QString valid = normalizeAndCheckPath(path);
        if (!valid.isEmpty()) {
            pathsToWatch.insert(valid);
            qDebug() << "[FileWatcher] Added path:" << valid;
        } else {
            qDebug() << "[FileWatcher] Skipped missing path:" << path;
        }
    }

    addPaths(pathsToWatch.values());
    qDebug() << "[FileWatcher] Now watching" << pathsToWatch.size() << "paths.";
}
