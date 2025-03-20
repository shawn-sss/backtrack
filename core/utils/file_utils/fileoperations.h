#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

// Built-in Qt includes
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QSet>
#include <QString>

namespace FileOperations {

// Directory and file management
bool copyDirectoryRecursively(const QString &sourcePath, const QString &destinationPath);
bool deleteDirectory(const QString &path);
bool createDirectory(const QString &path);

// File size calculation
quint64 calculateDirectorySize(const QDir &dir);

// Recursive file and directory collection
void collectFilesRecursively(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray);
void collectDirectoriesRecursively(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray);

// Backup infrastructure setup
bool createBackupInfrastructure(const QString &backupDir, QString &errorMessage);

} // namespace FileOperations

#endif // FILEOPERATIONS_H
