#ifndef TRANSFERWORKER_H
#define TRANSFERWORKER_H

// Qt includes
#include <QObject>
#include <QStringList>
#include <QFileInfo>

// C++ includes
#include <atomic>

// Executes file/folder transfers for backup operations
class TransferWorker : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit TransferWorker(const QStringList& files, const QString& destination, QObject* parent = nullptr);
    Q_DISABLE_COPY(TransferWorker)

public slots:
    // Starts the transfer operation
    void startTransfer();

    // Stops the transfer operation
    void stopTransfer();

signals:
    // Transfer status signals
    void progressUpdated(int progress);
    void transferComplete();
    void errorOccurred(const QString& error);
    void finished();

private:
    // Transfer helpers
    bool processDriveRoot(const QString& driveRoot);
    bool processFileOrFolder(const QString& filePath);
    bool copyItem(const QFileInfo& fileInfo, const QString& destinationPath);

    // Internal state
    QStringList files;
    QString destination;
    std::atomic<bool> stopRequested{false};
};

#endif // TRANSFERWORKER_H
