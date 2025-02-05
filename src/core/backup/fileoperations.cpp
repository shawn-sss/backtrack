#include "fileoperations.h"

#include <QDir>
#include <QFileInfoList>
#include <QJsonDocument>

namespace FileOperations {

// Directory and File Management

// Copy a directory and its contents recursively
bool copyDirectoryRecursively(const QString &source, const QString &destination) {
    QDir sourceDir(source);
    if (!sourceDir.exists()) {
        return false;
    }

    QDir destinationDir(destination);
    if (!destinationDir.exists() && !destinationDir.mkpath(".")) {
        return false;
    }

    QFileInfoList entries = sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    for (const QFileInfo &entry : entries) {
        QString destPath = destinationDir.filePath(entry.fileName());

        if (entry.isDir()) {
            if (!copyDirectoryRecursively(entry.absoluteFilePath(), destPath)) {
                return false;
            }
        } else if (entry.isFile()) {
            if (!QFile::copy(entry.absoluteFilePath(), destPath)) {
                return false;
            }
        }
    }
    return true;
}

// Calculate the total size of a directory
quint64 calculateDirectorySize(const QString &path) {
    quint64 totalSize = 0;
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    for (const QFileInfo &entry : entries) {
        totalSize += entry.isDir() ? calculateDirectorySize(entry.absoluteFilePath()) : entry.size();
    }
    return totalSize;
}

// Create a directory if it does not exist
bool createDirectory(const QString &path) {
    QDir dir(path);
    return dir.exists() || dir.mkpath(".");
}

// Delete a directory and its contents
bool deleteDirectory(const QString &path) {
    QDir dir(path);
    return dir.exists() && dir.removeRecursively();
}

// JSON File Handling

// Write a QJsonObject to a file
bool writeJsonToFile(const QString &filePath, const QJsonObject &jsonObject) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(jsonObject);
        file.write(doc.toJson());
        file.close();
        return true;
    }
    return false;
}

// Read a QJsonObject from a file
QJsonObject readJsonFromFile(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        return doc.isObject() ? doc.object() : QJsonObject();
    }
    return QJsonObject();
}

// File Collection

// Collect files from a directory recursively and store them in a QJsonArray
void collectFilesRecursively(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) {
    QDir dir(dirPath);

    // Collect files in the current directory
    QFileInfoList fileEntries = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    for (const QFileInfo &entry : fileEntries) {
        QString fullPath = entry.absoluteFilePath();
        if (!uniqueFiles.contains(fullPath)) {
            uniqueFiles.insert(fullPath);
            filesArray.append(fullPath);
        }
    }

    // Recurse into subdirectories
    QFileInfoList subDirEntries = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &subDir : subDirEntries) {
        collectFilesRecursively(subDir.absoluteFilePath(), uniqueFiles, filesArray);
    }
}

} // namespace FileOperations
