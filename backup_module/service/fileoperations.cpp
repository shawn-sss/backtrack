// Project includes
#include "fileoperations.h"
#include "../../../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"
#include "../constants/backupconstants.h"   // ✅ include constants

// Qt includes
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QJsonDocument>
#include <QSaveFile>

// ------------------------------------------------------------
// Recursively copy a directory and its contents
// ------------------------------------------------------------
bool FileOperations::copyDirectoryRecursively(const QString& sourcePath, const QString& destinationPath) {
    const QDir sourceDir(sourcePath);
    if (!sourceDir.exists()) return false;

    QDir destinationDir(destinationPath);
    if (!destinationDir.exists() && !destinationDir.mkpath(Backup::Transfer::MkpathCurrentDir))

        return false;

    const QFileInfoList entries = sourceDir.entryInfoList(Backup::Filters::All);
    for (const QFileInfo& entry : entries) {
        const QString srcFilePath  = entry.absoluteFilePath();
        const QString destFilePath = destinationDir.filePath(entry.fileName());

        if (entry.isDir()) {
            if (!copyDirectoryRecursively(srcFilePath, destFilePath)) return false;
        } else {
            if (!QFile::copy(srcFilePath, destFilePath)) return false;
        }
    }

    return true;
}

// ------------------------------------------------------------
// Delete a directory and its contents
// ------------------------------------------------------------
bool FileOperations::deleteDirectory(const QString& path) {
    QDir dir(path);
    return dir.exists() && dir.removeRecursively();
}

// ------------------------------------------------------------
// Create a directory if it does not exist
// ------------------------------------------------------------
bool FileOperations::createDirectory(const QString& path) {
    QDir dir(path);
    return dir.exists() || dir.mkpath(Backup::Transfer::MkpathCurrentDir);
}

// ------------------------------------------------------------
// Calculate the total size of a directory
// ------------------------------------------------------------
quint64 FileOperations::calculateDirectorySize(const QDir& dir) {
    quint64 totalSize = 0;
    const QFileInfoList entries = dir.entryInfoList(Backup::Filters::All);

    for (const QFileInfo& entry : entries) {
        if (entry.isDir()) {
            totalSize += calculateDirectorySize(QDir(entry.absoluteFilePath()));
        } else {
            totalSize += entry.size();
        }
    }

    return totalSize;
}

// ------------------------------------------------------------
// Recursively collect file paths from a directory
// ------------------------------------------------------------
void FileOperations::collectFilesRecursively(const QString& dirPath,
                                             QSet<QString>& uniqueFiles,
                                             QJsonArray& filesArray) {
    const QDir dir(dirPath);
    if (!dir.exists()) return;

    const QFileInfoList entries = dir.entryInfoList(Backup::Filters::Files);
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

// ------------------------------------------------------------
// Recursively collect directory paths
// ------------------------------------------------------------
void FileOperations::collectDirectoriesRecursively(const QString& dirPath,
                                                   QSet<QString>& uniqueFolders,
                                                   QJsonArray& foldersArray) {
    const QDir dir(dirPath);
    if (!dir.exists()) return;

    const QFileInfoList entries = dir.entryInfoList(Backup::Filters::Dirs);
    for (const QFileInfo& entry : entries) {
        const QString fullPath = entry.absoluteFilePath();
        if (!uniqueFolders.contains(fullPath)) {
            uniqueFolders.insert(fullPath);
            foldersArray.append(fullPath);
            collectDirectoriesRecursively(fullPath, uniqueFolders, foldersArray);
        }
    }
}

// ------------------------------------------------------------
// Create required backup folder infrastructure
// ------------------------------------------------------------
bool FileOperations::createBackupInfrastructure(const QString& /* backupDir */, QString& errorMessage) {
    const QString appFolderPath  = PathServiceManager::backupConfigFolderPath();
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
