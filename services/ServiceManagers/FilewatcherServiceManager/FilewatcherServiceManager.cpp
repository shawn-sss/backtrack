// Project includes
#include "FileWatcherServiceManager.h"
#include "FileWatcherServiceConstants.h"
#include "../../../constants/app_info.h"
#include "../PathServiceManager/PathServiceManager.h"

// Qt includes
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QSet>

// C++ includes
#include <algorithm>

// Required application config files
namespace AppConfigFiles {
inline const QStringList& Required() {
    static const QStringList files = {
        App::Items::k_APPDATA_SETUP_INFO_FILE,
        App::Items::k_APPDATA_SETUP_NOTIFICATIONS_FILE,
        App::Items::k_APPDATA_SETUP_USER_SETTINGS_FILE
    };
    return files;
}
}

namespace {
inline QString normalizePath(const QString& path) {
    return QDir::cleanPath(QDir::fromNativeSeparators(path.trimmed()));
}

QStringList buildCompleteWatchList(const QStringList& roots) {
    QSet<QString> pathsToWatch;

    const QString appConfigFolder = PathServiceManager::appConfigFolderPath();
    const QString backupRoot = normalizePath(PathServiceManager::backupConfigFolderPath());
    const QString backupInit = normalizePath(PathServiceManager::backupInitMetadataFilePath());
    const QString logsFolder = normalizePath(PathServiceManager::backupLogsFolderPath());

    for (const QString& root : roots) {
        const QString normalizedRoot = normalizePath(root);
        if (normalizedRoot.isEmpty()) continue;

        pathsToWatch.insert(normalizedRoot);

        if (normalizedRoot == backupRoot) {
            pathsToWatch.insert(backupInit);
            pathsToWatch.insert(logsFolder);
            pathsToWatch.insert(backupRoot);

            QDir logDir(logsFolder);
            const QFileInfoList fileList = logDir.entryInfoList(
                { FileWatcherConstants::LOG_GLOB_JSON }, QDir::Files);
            for (const QFileInfo& fi : fileList) {
                pathsToWatch.insert(fi.absoluteFilePath());
            }
        }
    }

    const QDir cfgDir(appConfigFolder);
    for (const QString& file : AppConfigFiles::Required()) {
        pathsToWatch.insert(normalizePath(cfgDir.filePath(file)));
    }

    return pathsToWatch.values();
}
}

// Lifecycle: construct watcher and connect signals
FileWatcher::FileWatcher(QObject* parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Watchlist: replace all paths
void FileWatcher::updateWatchList(const QStringList& paths) {
    removeAllPaths();
    addPaths(paths);
}

// Watchlist: add single path
void FileWatcher::addPath(const QString& path) {
    const QString normalized = normalizePath(path);
    if (normalized.isEmpty()) return;

    const auto& dirs = watcher->directories();
    const auto& files = watcher->files();
    if (!dirs.contains(normalized) && !files.contains(normalized)) {
        watcher->addPath(normalized);
    }
}

// Watchlist: add multiple paths
void FileWatcher::addPaths(const QStringList& paths) {
    QSet<QString> currentWatched;
    const QStringList dirs = watcher->directories();
    const QStringList files = watcher->files();
    currentWatched.unite(QSet<QString>(dirs.begin(), dirs.end()));
    currentWatched.unite(QSet<QString>(files.begin(), files.end()));

    QStringList newPaths;
    newPaths.reserve(paths.size());
    for (const QString& path : paths) {
        const QString normalized = normalizePath(path);
        if (!normalized.isEmpty() && !currentWatched.contains(normalized)) {
            newPaths.append(normalized);
        }
    }

    if (!newPaths.isEmpty()) {
        watcher->addPaths(newPaths);
    }
}

// Watchlist: remove single path
void FileWatcher::removePath(const QString& path) {
    const QString normalized = normalizePath(path);
    if (watcher->directories().contains(normalized) || watcher->files().contains(normalized)) {
        watcher->removePath(normalized);
    }
}

// Watchlist: remove all paths
void FileWatcher::removeAllPaths() {
    const QStringList allPaths = watcher->directories() + watcher->files();
    if (!allPaths.isEmpty()) {
        watcher->removePaths(allPaths);
    }
}

// Query: watched directories
QStringList FileWatcher::watchedDirectories() const {
    return watcher->directories();
}

// Query: watched files
QStringList FileWatcher::watchedFiles() const {
    return watcher->files();
}

// Watchlist: start watching multiple roots and related files
void FileWatcher::startWatchingMultiple(const QStringList& roots) {
    static QStringList lastWatched;

    QStringList sortedRoots = roots;
    std::sort(sortedRoots.begin(), sortedRoots.end());

    const QStringList currentPathsToWatch = buildCompleteWatchList(roots);
    const QSet<QString> newPathSet(currentPathsToWatch.begin(), currentPathsToWatch.end());

    const QStringList dirs = watcher->directories();
    const QStringList files = watcher->files();

    QSet<QString> currentWatchedSet;
    currentWatchedSet.unite(QSet<QString>(dirs.begin(), dirs.end()));
    currentWatchedSet.unite(QSet<QString>(files.begin(), files.end()));

    if (sortedRoots == lastWatched && newPathSet == currentWatchedSet)
        return;

    lastWatched = sortedRoots;
    removeAllPaths();
    addPaths(currentPathsToWatch);
}
