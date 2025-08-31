#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

// Qt includes
#include <QDir>
#include <QJsonArray>
#include <QSet>
#include <QString>

namespace FileOperations {

// Directory management
bool copyDirectoryRecursively(const QString& sourcePath, const QString& destinationPath);
bool deleteDirectory(const QString& path);
bool createDirectory(const QString& path);

// Directory size calculation
quint64 calculateDirectorySize(const QDir& dir);

// File and folder collection
void collectFilesRecursively(const QString& dirPath,
                             QSet<QString>& uniqueFiles,
                             QJsonArray& filesArray);
void collectDirectoriesRecursively(const QString& dirPath,
                                   QSet<QString>& uniqueFolders,
                                   QJsonArray& foldersArray);

// Backup infrastructure setup
bool createBackupInfrastructure(const QString& backupDir, QString& errorMessage);

} // namespace FileOperations

#endif // FILEOPERATIONS_H
