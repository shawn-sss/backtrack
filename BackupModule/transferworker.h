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
    // Signals for transfer progress and status
    void progressUpdated(int progress);
    void transferComplete();
    void errorOccurred(const QString &error);

private:
    // Member variables
    QStringList files;
    QString destination;
};

#endif // TRANSFERWORKER_H
