#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

// Qt includes
#include <QString>
#include <QSet>
#include <QDir>
#include <QJsonArray>

namespace FileOperations {

// Recursively copies a directory and its contents
bool copyDirectoryRecursively(const QString &sourcePath, const QString &destinationPath);

// Deletes a directory and its contents
bool deleteDirectory(const QString &path);

// Creates a directory if it does not exist
bool createDirectory(const QString &path);

// Calculates the total size of all contents in a directory
quint64 calculateDirectorySize(const QDir &dir);

// Recursively collects all file paths from a directory
void collectFilesRecursively(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray);

// Recursively collects all subdirectory paths from a directory
void collectDirectoriesRecursively(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray);

// Creates base directory and logs structure for backups
bool createBackupInfrastructure(const QString &backupDir, QString &errorMessage);

} // namespace FileOperations

#endif // FILEOPERATIONS_H
