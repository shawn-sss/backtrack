#ifndef STAGINGMODEL_H
#define STAGINGMODEL_H

// Built-in Qt includes
#include <QVector>
#include <QAbstractItemModel>

// Manages staged backup paths
class StagingModel : public QAbstractItemModel {
    Q_OBJECT

public:
    // Constructor
    explicit StagingModel(QObject *parent = nullptr);
    Q_DISABLE_COPY(StagingModel)

    // Model interface overrides
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Path management
    void addPath(const QString &path);
    void removePath(const QString &path);
    QStringList getStagedPaths() const;

private:
    // Internal data storage
    QVector<QString> stagedPaths;
    QSet<QString> stagedPathsSet;
};

#endif // STAGINGMODEL_H
