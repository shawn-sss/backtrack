// Project includes
#include "fileoperations.h"
#include "../../../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"

// Qt includes
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QJsonDocument>
#include <QSaveFile>

// C++ includes

// Forward declaration (Custom class)

// Forward declaration (Qt class)

namespace FileOperations {

// Recursively copies a directory and its contents
bool copyDirectoryRecursively(const QString& sourcePath, const QString& destinationPath) {
    const QDir sourceDir(sourcePath);
    if (!sourceDir.exists())
        return false;

    QDir destinationDir(destinationPath);
    if (!destinationDir.exists() && !destinationDir.mkpath("."))
        return false;

    const QFileInfoList entries = sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const QFileInfo& entry : entries) {
        const QString srcFilePath = entry.absoluteFilePath();
        const QString destFilePath = destinationDir.filePath(entry.fileName());

        if (entry.isDir()) {
            if (!copyDirectoryRecursively(srcFilePath, destFilePath))
                return false;
        } else {
            if (!QFile::copy(srcFilePath, destFilePath))
                return false;
        }
    }

    return true;
}

// Deletes a directory and its contents
bool deleteDirectory(const QString& path) {
    QDir dir(path);
    return dir.exists() && dir.removeRecursively();
}

// Creates a directory if it does not exist
bool createDirectory(const QString& path) {
    QDir dir(path);
    return dir.exists() || dir.mkpath(".");
}

// Calculates the total size of a directory
quint64 calculateDirectorySize(const QDir& dir) {
    quint64 totalSize = 0;

    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const QFileInfo& entry : entries) {
        if (entry.isDir()) {
            totalSize += calculateDirectorySize(QDir(entry.absoluteFilePath()));
        } else {
            totalSize += entry.size();
        }
    }

    return totalSize;
}

// Recursively collects file paths from a directory
void collectFilesRecursively(const QString& dirPath, QSet<QString>& uniqueFiles, QJsonArray& filesArray) {
    const QDir dir(dirPath);
    if (!dir.exists())
        return;

    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
    for (const QFileInfo& entry : entries) {
        const QString fullPath = entry.absoluteFilePath();
        if (entry.isDir()) {
            collectFilesRecursively(fullPath, uniqueFiles, filesArray);
        } else if (!uniqueFiles.contains(fullPath)) {
            uniqueFiles.insert(fullPath);
            filesArray.append(fullPath);
        }
    }
}

// Recursively collects directory paths
void collectDirectoriesRecursively(const QString& dirPath, QSet<QString>& uniqueFolders, QJsonArray& foldersArray) {
    const QDir dir(dirPath);
    if (!dir.exists())
        return;

    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
    for (const QFileInfo& entry : entries) {
        const QString fullPath = entry.absoluteFilePath();
        if (!uniqueFolders.contains(fullPath)) {
            uniqueFolders.insert(fullPath);
            foldersArray.append(fullPath);
            collectDirectoriesRecursively(fullPath, uniqueFolders, foldersArray);
        }
    }
}

// Creates required backup folder infrastructure
bool createBackupInfrastructure(const QString& /* backupDir */, QString& errorMessage) {
    const QString appFolderPath = PathServiceManager::backupConfigFolderPath();
    const QString logsFolderPath = PathServiceManager::backupLogsFolderPath();

    if (!QDir().mkpath(appFolderPath)) {
        errorMessage = QString("Failed to create directory: %1").arg(appFolderPath);
        return false;
    }

    if (!QDir().mkpath(logsFolderPath)) {
        errorMessage = QString("Failed to create logs directory: %1").arg(logsFolderPath);
        return false;
    }

    return true;
}

} // namespace FileOperations
