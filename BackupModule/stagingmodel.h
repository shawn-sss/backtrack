#ifndef STAGINGMODEL_H
#define STAGINGMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

// Model class for managing staged file paths
class StagingModel : public QAbstractItemModel {
    Q_OBJECT

public:
    // Constructor
    explicit StagingModel(QObject *parent = nullptr);

    // Overridden methods for QAbstractItemModel functionality
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Methods for managing staged file paths
    void addPath(const QString &path);
    void removePath(const QString &path);
    QStringList getStagedPaths() const;

private:
    // Member variable storing staged file paths
    QStringList stagedPaths;
};

#endif // STAGINGMODEL_H
