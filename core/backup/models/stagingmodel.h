#ifndef STAGINGMODEL_H
#define STAGINGMODEL_H

// Qt includes
#include <QAbstractItemModel>
#include <QVector>
#include <QSet>
#include <QString>
#include <QStringList>

// Manages staged paths for display in a flat list
class StagingModel : public QAbstractItemModel {
    Q_OBJECT

public:
    // Constructor
    explicit StagingModel(QObject* parent = nullptr);
    Q_DISABLE_COPY(StagingModel)

    // QAbstractItemModel overrides
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
    // List of staged paths in order
    QVector<QString> stagedPaths;

    // Set to ensure uniqueness
    QSet<QString> stagedPathsSet;
};

#endif // STAGINGMODEL_H
