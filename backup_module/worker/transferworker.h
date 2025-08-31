#ifndef TRANSFERWORKER_H
#define TRANSFERWORKER_H

// Qt includes
#include <QObject>
#include <QStringList>
#include <QFileInfo>
#include <atomic>

// Forward declaration (Qt class)
class QStorageInfo;

// Worker for transferring files and folders into a backup destination
class TransferWorker : public QObject {
    Q_OBJECT

public:
    explicit TransferWorker(const QStringList& files,
                            const QString& destination,
                            QObject* parent = nullptr);

public slots:
    void startTransfer();
    void stopTransfer();

signals:
    void progressUpdated(int percent);
    void transferComplete();
    void errorOccurred(const QString& errorMessage);
    void removeFromStaging(const QString& filePath);
    void finished();

private:
    bool shouldStop();
    bool processDriveRoot(const QString& driveRoot);
    bool processFileOrFolder(const QString& filePath);
    bool copyItem(const QFileInfo& fileInfo, const QString& destinationPath);

private:
    QStringList files;
    QString destination;
    std::atomic<bool> stopRequested{false};
};

#endif // TRANSFERWORKER_H
