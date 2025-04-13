#ifndef TRANSFERWORKER_H
#define TRANSFERWORKER_H

// Built-in Qt includes
#include <QObject>
#include <QFileInfo>
#include <QStringList>
#include <atomic>

// Handles file transfers for backup
class TransferWorker : public QObject {
    Q_OBJECT

public:
    // Constructs the transfer worker with files and destination
    explicit TransferWorker(const QStringList &files, const QString &destination, QObject *parent = nullptr);
    Q_DISABLE_COPY(TransferWorker)

public slots:
    // Starts the file transfer operation
    void startTransfer();

    // Requests the transfer to stop
    void stopTransfer();

signals:
    // Signals to track progress and transfer status
    void progressUpdated(int progress);
    void transferComplete();
    void errorOccurred(const QString &error);
    void finished();

private:
    // Processes an entire drive root
    bool processDriveRoot(const QString &driveRoot);

    // Processes a single file or directory
    bool processFileOrFolder(const QString &filePath);

    // Copies a file or recursively copies a directory
    bool copyItem(const QFileInfo &fileInfo, const QString &destinationPath);

    // Internal transfer parameters
    QStringList files;
    QString destination;
    std::atomic<bool> stopRequested{false};
};

#endif // TRANSFERWORKER_H
