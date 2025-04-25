#ifndef STAGINGMODEL_H
#define STAGINGMODEL_H

// Qt includes
#include <QVector>
#include <QSet>
#include <QAbstractItemModel>

// Manages staged backup paths
class StagingModel : public QAbstractItemModel {
    Q_OBJECT

public:
    // Construction
    explicit StagingModel(QObject *parent = nullptr);
    Q_DISABLE_COPY(StagingModel)

    // Qt model interface overrides
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Adds a new file path to the staging list
    void addPath(const QString &path);

    // Removes a file path from the staging list
    void removePath(const QString &path);

    // Returns all currently staged file paths
    QStringList getStagedPaths() const;

private:
    // Internal list of staged paths (ordered)
    QVector<QString> stagedPaths;

    // Set for fast lookup of staged paths
    QSet<QString> stagedPathsSet;
};

#endif // STAGINGMODEL_H
