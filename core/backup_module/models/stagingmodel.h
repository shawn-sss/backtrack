#ifndef STAGINGMODEL_H
#define STAGINGMODEL_H

#include <QAbstractItemModel>
#include <QVector>

// Staging Model Class
class StagingModel : public QAbstractItemModel {
    Q_OBJECT

public:
    // Constructor
    explicit StagingModel(QObject *parent = nullptr);

    Q_DISABLE_COPY(StagingModel)

    // QModelIndex Management
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    // Row and Column Counts
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Data Handling
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Path Management
    void addPath(const QString &path);
    void removePath(const QString &path);
    QVector<QString> getStagedPaths() const;

private:
    // Staged File Paths
    QVector<QString> stagedPaths;
};

#endif // STAGINGMODEL_H
