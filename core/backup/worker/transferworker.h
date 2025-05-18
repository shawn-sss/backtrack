#ifndef TRANSFERWORKER_H
#define TRANSFERWORKER_H

// Qt includes
#include <QObject>
#include <QStringList>
#include <QFileInfo>

// C++ includes
#include <atomic>

// Executes file transfers to backup destination
class TransferWorker : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit TransferWorker(const QStringList& files, const QString& destination, QObject* parent = nullptr);

    Q_DISABLE_COPY(TransferWorker)

public slots:
    // Begin the transfer operation
    void startTransfer();

    // Request to stop the operation
    void stopTransfer();

signals:
    // Emitted to indicate updated progress percentage
    void progressUpdated(int progress);

    // Emitted when all transfers are completed
    void transferComplete();

    // Emitted when an error occurs
    void errorOccurred(const QString& error);

    // Emitted when processing has finished (regardless of success or failure)
    void finished();

private:
    // Handle root-level drive copy
    bool processDriveRoot(const QString& driveRoot);

    // Handle single file or directory copy
    bool processFileOrFolder(const QString& filePath);

    // Core logic for copying an item
    bool copyItem(const QFileInfo& fileInfo, const QString& destinationPath);

    // Check whether stop was requested
    bool shouldStop();

    // List of files to transfer
    QStringList files;

    // Backup destination path
    QString destination;

    // Atomic flag to track stop request
    std::atomic<bool> stopRequested{false};
};

#endif // TRANSFERWORKER_H
