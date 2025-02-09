#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QSet>

namespace FileOperations {

// Directory and File Management
bool copyDirectoryRecursively(const QString &source, const QString &destination);
quint64 calculateDirectorySize(const QString &path);
bool createDirectory(const QString &path);
bool deleteDirectory(const QString &path);

// JSON File Handling
bool writeJsonToFile(const QString &filePath, const QJsonObject &jsonObject);
QJsonObject readJsonFromFile(const QString &filePath);

// File Collection and Traversal
void collectFilesRecursively(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray);

} // namespace FileOperations

#endif // FILEOPERATIONS_H
