#ifndef TRANSFERWORKER_H
#define TRANSFERWORKER_H

#include <QObject>
#include <QStringList>
#include <QFileInfo>
#include <QDir>

// Handles file transfer operations
class TransferWorker : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit TransferWorker(const QStringList &files, const QString &destination, QObject *parent = nullptr);

    Q_DISABLE_COPY(TransferWorker)

public slots:
    void startTransfer();
    void stopTransfer();

signals:
    void progressUpdated(int progress);
    void transferComplete();
    void errorOccurred(const QString &error);
    void finished();

private:
    // File processing
    bool processDriveRoot(const QString &driveRoot);
    bool processFileOrFolder(const QString &filePath);
    bool copyItem(const QFileInfo &fileInfo, const QString &destinationPath);

    // Transfer management
    const QStringList files;
    const QString destination;
    bool stopRequested = false;
};

#endif // TRANSFERWORKER_H
