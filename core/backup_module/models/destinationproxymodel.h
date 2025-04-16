// DestinationProxyModel.h
#ifndef DESTINATIONPROXYMODEL_H
#define DESTINATIONPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QString>

class DestinationProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit DestinationProxyModel(QObject *parent = nullptr);

    void setExcludedFolderName(const QString &folderName);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString excludedFolderName;
};

#endif // DESTINATIONPROXYMODEL_H
