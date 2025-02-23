#ifndef TRANSFERWORKER_H
#define TRANSFERWORKER_H

#include <QDir>
#include <QStringList>
#include <QFileInfo>
#include <QObject>

// Handles file transfer operations
class TransferWorker : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit TransferWorker(const QStringList &files, const QString &destination, QObject *parent = nullptr);
    Q_DISABLE_COPY(TransferWorker)

public slots:
    // Transfer control
    void startTransfer();
    void stopTransfer();

signals:
    // Transfer progress and status signals
    void progressUpdated(int progress);
    void transferComplete();
    void errorOccurred(const QString &error);
    void finished();

private:
    // File processing utilities
    bool processDriveRoot(const QString &driveRoot);
    bool processFileOrFolder(const QString &filePath);
    bool copyItem(const QFileInfo &fileInfo, const QString &destinationPath);

    // Private member variables
    const QStringList files;
    const QString destination;
    bool stopRequested = false;
};

#endif // TRANSFERWORKER_H
