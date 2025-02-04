#ifndef TRANSFERWORKER_H
#define TRANSFERWORKER_H

#include <QObject>
#include <QStringList>

// Worker class for handling file transfers
class TransferWorker : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit TransferWorker(const QStringList &files, const QString &destination, QObject *parent = nullptr);

public slots:
    // Slot to start the file transfer process
    void startTransfer();

signals:
    // Signals for transfer progress and status updates
    void progressUpdated(int progress);
    void transferComplete();
    void errorOccurred(const QString &error);

private:
    // Helper methods for specific transfer operations
    bool processDriveRoot(const QString &driveRoot);
    bool processFileOrFolder(const QString &filePath);

    // Member variables for file transfer
    QStringList files;
    QString destination;
};

#endif // TRANSFERWORKER_H
