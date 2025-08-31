// Project includes
#include "fileoperations.h"
#include "../constants/backupconstants.h"
#include "../../../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"

// Qt includes
#include <QDir>
#include <QFile>
#include <QFileInfoList>

// Copy a directory and its contents recursively
bool FileOperations::copyDirectoryRecursively(const QString& sourcePath, const QString& destinationPath) {
    const QDir sourceDir(sourcePath);
    if (!sourceDir.exists()) return false;

    QDir destinationDir(destinationPath);
    if (!destinationDir.exists() && !destinationDir.mkpath("."))
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

// Delete a directory recursively
bool FileOperations::deleteDirectory(const QString& path) {
    QDir dir(path);
    return dir.exists() && dir.removeRecursively();
}

// Create a directory if it does not exist
bool FileOperations::createDirectory(const QString& path) {
    QDir dir(path);
    return dir.exists() || dir.mkpath(".");
}

// Calculate total size of a directory recursively
quint64 FileOperations::calculateDirectorySize(const QDir& dir) {
    quint64 totalSize = 0;
    const QFileInfoList entries = dir.entryInfoList(Backup::Filters::All);

    for (const QFileInfo& entry : entries) {
        totalSize += entry.isDir()
        ? calculateDirectorySize(QDir(entry.absoluteFilePath()))
        : entry.size();
    }
    return totalSize;
}

// Collect all files recursively into a JSON array and a unique set
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

// Collect all directories recursively into a JSON array and a unique set
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

// Create required backup infrastructure directories
bool FileOperations::createBackupInfrastructure(const QString&, QString& errorMessage) {
    const QString appFolderPath  = PathServiceManager::backupConfigFolderPath();
    const QString logsFolderPath = PathServiceManager::backupLogsFolderPath();

    if (!QDir().mkpath(appFolderPath)) {
        errorMessage = Backup::Errors::Messages[Backup::ErrorCode::CreateAppFolder].arg(appFolderPath);
        return false;
    }
    if (!QDir().mkpath(logsFolderPath)) {
        errorMessage = Backup::Errors::Messages[Backup::ErrorCode::CreateLogsFolder].arg(logsFolderPath);
        return false;
    }
    return true;
}
