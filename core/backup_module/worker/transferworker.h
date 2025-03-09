#ifndef TRANSFERWORKER_H
#define TRANSFERWORKER_H

// Built-in Qt includes
#include <QObject>
#include <QFileInfo>
#include <QStringList>

// Handles file transfers for backup
class TransferWorker : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit TransferWorker(const QStringList &files, const QString &destination, QObject *parent = nullptr);
    Q_DISABLE_COPY(TransferWorker)

public slots:
    // Starts or stops transfer
    void startTransfer();
    void stopTransfer();

signals:
    // Transfer progress and status signals
    void progressUpdated(int progress);
    void transferComplete();
    void errorOccurred(const QString &error);
    void finished();

private:
    // Processing functions
    bool processDriveRoot(const QString &driveRoot);
    bool processFileOrFolder(const QString &filePath);
    bool copyItem(const QFileInfo &fileInfo, const QString &destinationPath);

    // Internal state
    QStringList files;
    QString destination;
    std::atomic<bool> stopRequested{false};
};

#endif // TRANSFERWORKER_H
