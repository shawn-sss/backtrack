// Project includes
#include "filewatcher.h"
#include "../../../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"

// Qt includes
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QSet>

// C++ includes
#include <algorithm>

// Required application config files
namespace AppConfigFiles {
static const QStringList Required = {
    "app_init.json",
    "app_notifications.json",
    "user_settings.json"
};
}

// Anonymous namespace for utilities
namespace {

QString normalizeAndCheckPath(const QString& path, bool allowNonexistentDir) {
    QString normalized = QDir::fromNativeSeparators(path.trimmed());
    QFileInfo info(normalized);
    if (info.exists()) return normalized;
    if (allowNonexistentDir && info.isDir()) return normalized;
    return {};
}

QStringList buildCompleteWatchList(const QStringList& roots) {
    QSet<QString> pathsToWatch;

    const QString appConfigFolder = PathServiceManager::appConfigFolderPath();
    const QString backupConfigFolder = PathServiceManager::backupConfigFolderPath();
    const QString backupRoot = backupConfigFolder;
    const QString backupInit = PathServiceManager::backupInitMetadataFilePath();
    const QString logsFolder = PathServiceManager::backupLogsFolderPath();

    for (const QString& root : roots) {
        const QString normalizedRoot = QDir::cleanPath(root);
        if (normalizedRoot.isEmpty()) continue;

        pathsToWatch.insert(normalizedRoot);

        if (normalizedRoot == QDir::cleanPath(backupRoot)) {
            pathsToWatch.insert(QDir::cleanPath(backupInit));
            pathsToWatch.insert(QDir::cleanPath(logsFolder));
            pathsToWatch.insert(QDir::cleanPath(backupConfigFolder));

            QDir logDir(logsFolder);
            const QFileInfoList logFiles = logDir.entryInfoList(QStringList() << "*.json", QDir::Files);
            for (const QFileInfo& fileInfo : logFiles) {
                pathsToWatch.insert(fileInfo.absoluteFilePath());
            }
        }
    }

    for (const QString& file : AppConfigFiles::Required) {
        const QString fullPath = QDir(appConfigFolder).filePath(file);
        pathsToWatch.insert(QDir::cleanPath(fullPath));
    }

    return pathsToWatch.values();
}

} // namespace

// Initializes file system watcher and connects signals
FileWatcher::FileWatcher(QObject* parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Updates the watch list with new paths
void FileWatcher::updateWatchList(const QStringList& paths) {
    removeAllPaths();
    addPaths(paths);
}

// Adds a single path to the watcher
void FileWatcher::addPath(const QString& path) {
    const QString normalized = normalizeAndCheckPath(path, true);
    if (normalized.isEmpty()) return;

    if (!watcher->directories().contains(normalized) && !watcher->files().contains(normalized)) {
        watcher->addPath(normalized);
    }
}

// Adds multiple paths to the watcher
void FileWatcher::addPaths(const QStringList& paths) {
    QSet<QString> currentWatched;

    for (const QString& dir : watcher->directories())
        currentWatched.insert(dir);
    for (const QString& file : watcher->files())
        currentWatched.insert(file);

    QStringList newPaths;
    for (const QString& path : paths) {
        QString normalized = QDir::fromNativeSeparators(path.trimmed());
        if (!normalized.isEmpty() && !currentWatched.contains(normalized)) {
            newPaths.append(normalized);
        }
    }

    if (!newPaths.isEmpty()) {
        watcher->addPaths(newPaths);
    }
}

// Removes a single path from the watcher
void FileWatcher::removePath(const QString& path) {
    const QString normalized = QDir::fromNativeSeparators(path.trimmed());
    if (watcher->directories().contains(normalized) || watcher->files().contains(normalized)) {
        watcher->removePath(normalized);
    }
}

// Removes all watched paths
void FileWatcher::removeAllPaths() {
    const QStringList allPaths = watcher->directories() + watcher->files();
    if (!allPaths.isEmpty()) {
        watcher->removePaths(allPaths);
    }
}

// Returns currently watched directories
QStringList FileWatcher::watchedDirectories() const {
    return watcher->directories();
}

// Returns currently watched files
QStringList FileWatcher::watchedFiles() const {
    return watcher->files();
}

// Starts watching all relevant paths based on given root paths
void FileWatcher::startWatchingMultiple(const QStringList& roots) {
    static QStringList lastWatched;

    QStringList sortedRoots = roots;
    std::sort(sortedRoots.begin(), sortedRoots.end());

    const QStringList currentPathsToWatch = buildCompleteWatchList(roots);

    const QStringList watchedDirs = watcher->directories();
    const QStringList watchedFiles = watcher->files();

    QSet<QString> newPathSet(currentPathsToWatch.begin(), currentPathsToWatch.end());
    QSet<QString> currentWatchedSet;
    currentWatchedSet.unite(QSet<QString>(watchedDirs.begin(), watchedDirs.end()));
    currentWatchedSet.unite(QSet<QString>(watchedFiles.begin(), watchedFiles.end()));

    if (sortedRoots == lastWatched && newPathSet == currentWatchedSet) return;

    lastWatched = sortedRoots;

    removeAllPaths();
    addPaths(currentPathsToWatch);
}
