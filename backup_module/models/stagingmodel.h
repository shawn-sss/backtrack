#ifndef STAGINGMODEL_H
#define STAGINGMODEL_H

// Qt includes
#include <QAbstractItemModel>
#include <QSet>
#include <QStringList>
#include <QFileIconProvider>

// Model for staged file and folder paths with deduplication
class StagingModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit StagingModel(QObject* parent = nullptr);

    // Qt model interface
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Modify staged paths
    void addPath(const QString& path);
    void addPaths(const QStringList& paths);
    void removePath(const QString& path);
    void clear();

    // Query staged paths
    QStringList getStagedPaths() const;
    bool containsPath(const QString& path) const;

    // File icon provider
    static QFileIconProvider& iconProvider();

private:
    QStringList stagedPaths;
    QSet<QString> stagedPathsSet;
};

#endif // STAGINGMODEL_H
