#include "join.h"

ProxyOrm::Join::Join(const QAbstractListModel *sourceModel,
                     const int sourceRole,
                     const QAbstractListModel *joinModel,
                     const int joinRole,
                     const QMap<int, int> mapRoles)
    : ISource()
    , sourceModel(sourceModel)
    , sourceRole(sourceRole)
    , joinModel(joinModel)
    , joinRole(joinRole)
    , mapRoles(mapRoles)
{
    connect(joinModel, &QAbstractListModel::modelReset, [this]() {
        invalidateCache();
        emit changed(this->mapRoles.keys());
    });

    connect(joinModel, &QAbstractListModel::rowsInserted, [this]() {
        invalidateCache();
        emit changed(this->mapRoles.keys());
    });

    connect(joinModel, &QAbstractListModel::rowsRemoved, [this]() {
        invalidateCache();
        emit changed(this->mapRoles.keys());
    });

    connect(joinModel, &QAbstractListModel::dataChanged, [this]() {
        invalidateCache();
        emit changed(this->mapRoles.keys());
    });
}

QVariant ProxyOrm::Join::templateData(int row, int role) const
{
    auto sourceIndex = sourceModel->index(row, 0);

    for (int i = 0; i < joinModel->rowCount(); i++) {
        QModelIndex joinIndex = joinModel->index(i, 0);
        if (sourceModel->data(sourceIndex, sourceRole) == joinModel->data(joinIndex, joinRole)) {
            int joinFindRole = mapRoles.value(role);
            return joinModel->data(joinIndex, joinFindRole);
        }
    }
    return QVariant{};
}

QList<int> ProxyOrm::Join::roles() const
{
    return mapRoles.keys();
}
