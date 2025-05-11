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
    // Starts the transfer
    void startTransfer();

    // Stops the transfer
    void stopTransfer();

signals:
    // Transfer progress and status signals
    void progressUpdated(int progress);
    void transferComplete();
    void errorOccurred(const QString& error);
    void finished();

private:
    // Helpers for specific transfer scenarios
    bool processDriveRoot(const QString& driveRoot);
    bool processFileOrFolder(const QString& filePath);
    bool copyItem(const QFileInfo& fileInfo, const QString& destinationPath);

    // Transfer state
    QStringList files;
    QString destination;
    std::atomic<bool> stopRequested{false};
};

#endif // TRANSFERWORKER_H
