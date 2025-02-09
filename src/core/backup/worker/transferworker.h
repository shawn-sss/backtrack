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
    // Starts the file transfer process
    void startTransfer();

signals:
    // Signals for progress and status updates
    void progressUpdated(int progress);
    void transferComplete();
    void errorOccurred(const QString &error);

private:
    // Processes drive root before transfer
    bool processDriveRoot(const QString &driveRoot);

    // Processes individual file or folder transfer
    bool processFileOrFolder(const QString &filePath);

    // File transfer data
    QStringList files;
    QString destination;
};

#endif // TRANSFERWORKER_H
