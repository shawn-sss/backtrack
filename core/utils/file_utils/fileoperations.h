#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

// Qt includes
#include <QDir>
#include <QJsonArray>
#include <QSet>
#include <QString>

namespace FileOperations {

// Recursively copies a directory and its contents
bool copyDirectoryRecursively(const QString& sourcePath, const QString& destinationPath);

// Deletes a directory and all its contents
bool deleteDirectory(const QString& path);

// Creates a directory if it does not exist
bool createDirectory(const QString& path);

// Calculates total size of all contents in a directory
quint64 calculateDirectorySize(const QDir& dir);

// Recursively collects all file paths in a directory
void collectFilesRecursively(const QString& dirPath, QSet<QString>& uniqueFiles, QJsonArray& filesArray);

// Recursively collects all folder paths in a directory
void collectDirectoriesRecursively(const QString& dirPath, QSet<QString>& uniqueFolders, QJsonArray& foldersArray);

// Creates base folders and log folder for backup infrastructure
bool createBackupInfrastructure(const QString& backupDir, QString& errorMessage);

} // namespace FileOperations

#endif // FILEOPERATIONS_H
