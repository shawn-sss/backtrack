#ifndef DESTINATIONPROXYMODEL_H
#define DESTINATIONPROXYMODEL_H

// Built-in Qt includes
#include <QString>
#include <QSortFilterProxyModel>

// DestinationProxyModel class definition
class DestinationProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    // Constructor: Initializes the proxy model with an optional parent object
    explicit DestinationProxyModel(QObject *parent = nullptr);

    // Sets the folder name to exclude during filtering (case insensitive)
    void setExcludedFolderName(const QString &folderName);

    // Determines if a row should be included based on the exclusion rule
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    // Stores the folder name to exclude from the model (case insensitive)
    QString excludedFolderName;
};

#endif // DESTINATIONPROXYMODEL_H
