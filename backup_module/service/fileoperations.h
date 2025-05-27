#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

// Qt includes
#include <QDir>
#include <QJsonArray>
#include <QSet>
#include <QString>

namespace FileOperations {

// Recursively copy a directory and its contents
bool copyDirectoryRecursively(const QString& sourcePath, const QString& destinationPath);

// Delete a directory and all its contents
bool deleteDirectory(const QString& path);

// Create a directory if it does not exist
bool createDirectory(const QString& path);

// Calculate total size of a directory
quint64 calculateDirectorySize(const QDir& dir);

// Recursively collect file paths in a directory
void collectFilesRecursively(const QString& dirPath, QSet<QString>& uniqueFiles, QJsonArray& filesArray);

// Recursively collect directory paths
void collectDirectoriesRecursively(const QString& dirPath, QSet<QString>& uniqueFolders, QJsonArray& foldersArray);

// Create backup folder structure and logs directory
bool createBackupInfrastructure(const QString& backupDir, QString& errorMessage);

} // namespace FileOperations

#endif // FILEOPERATIONS_H
