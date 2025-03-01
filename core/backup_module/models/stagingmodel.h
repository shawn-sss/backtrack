#ifndef STAGINGMODEL_H
#define STAGINGMODEL_H

#include <QVector>
#include <QAbstractItemModel>

// StagingModel class definition
class StagingModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit StagingModel(QObject *parent = nullptr);
    Q_DISABLE_COPY(StagingModel)

    // QModelIndex management
public:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    // Row and column counts
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Data handling
public:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Path management
public:
    void addPath(const QString &path);
    void removePath(const QString &path);
    QVector<QString> getStagedPaths() const;

    // Internal data
private:
    QVector<QString> stagedPaths;
};

#endif // STAGINGMODEL_H
