#ifndef TRANSFERWORKER_H
#define TRANSFERWORKER_H

#include <QDir>
#include <QStringList>
#include <QFileInfo>
#include <QObject>

// TransferWorker handles file transfer operations
class TransferWorker : public QObject {
    Q_OBJECT

public:
    explicit TransferWorker(const QStringList &files, const QString &destination, QObject *parent = nullptr);
    Q_DISABLE_COPY(TransferWorker)

    // Transfer control
public slots:
    void startTransfer();
    void stopTransfer();

    // Transfer progress and status signals
signals:
    void progressUpdated(int progress);
    void transferComplete();
    void errorOccurred(const QString &error);
    void finished();

    // Internal file processing
private:
    bool processDriveRoot(const QString &driveRoot);
    bool processFileOrFolder(const QString &filePath);
    bool copyItem(const QFileInfo &fileInfo, const QString &destinationPath);

    // Internal variables
private:
    const QStringList files;
    const QString destination;
    bool stopRequested = false;
};

#endif // TRANSFERWORKER_H
