#ifndef STAGINGMODEL_H
#define STAGINGMODEL_H

// Qt includes
#include <QAbstractItemModel>
#include <QFileIconProvider>
#include <QSet>
#include <QStringList>
#include <QVector>

// Manages staged paths for display in a flat list
class StagingModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit StagingModel(QObject* parent = nullptr);
    Q_DISABLE_COPY(StagingModel)

    // Model interface overrides
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Path management
    void addPath(const QString& path);
    void removePath(const QString& path);
    QStringList getStagedPaths() const;

    // Returns true if the given path is already in the staging list
    bool containsPath(const QString& path) const;

    // Adds multiple new paths to the staging list if they are not already present
    void addPaths(const QStringList& paths);

private:
    QVector<QString> stagedPaths;
    QSet<QString> stagedPathsSet;

    static QFileIconProvider& iconProvider();
};

#endif // STAGINGMODEL_H
