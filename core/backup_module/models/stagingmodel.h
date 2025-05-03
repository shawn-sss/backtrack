#ifndef STAGINGMODEL_H
#define STAGINGMODEL_H

// Qt includes
#include <QSet>
#include <QVector>
#include <QAbstractItemModel>
#include <QStringList>
#include <QString>

// Manages staged backup paths for display and control
class StagingModel : public QAbstractItemModel {
    Q_OBJECT

public:
    // Constructor
    explicit StagingModel(QObject* parent = nullptr);
    Q_DISABLE_COPY(StagingModel)

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Staging management
    void addPath(const QString& path);
    void removePath(const QString& path);
    QStringList getStagedPaths() const;

private:
    // Ordered list of staged paths
    QVector<QString> stagedPaths;

    // Set for fast lookup and duplicate avoidance
    QSet<QString> stagedPathsSet;
};

#endif // STAGINGMODEL_H
