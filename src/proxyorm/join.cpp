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
    connect(joinModel, &QAbstractListModel::modelReset, this, &ProxyOrm::Join::invalidate);
    connect(joinModel, &QAbstractListModel::rowsInserted, this, &ProxyOrm::Join::invalidate);
    connect(joinModel, &QAbstractListModel::rowsRemoved, this, &ProxyOrm::Join::invalidate);
    connect(joinModel, &QAbstractListModel::dataChanged, this, &ProxyOrm::Join::invalidate);
}

QVariant ProxyOrm::Join::templateData(int row, int role) const
{
    if (!sourceModel || !joinModel || row < 0 || row >= sourceModel->rowCount()) {
        return QVariant{};
    }
    if (!mapRoles.contains(role)) {
        return QVariant{};
    }

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

void ProxyOrm::Join::invalidate()
{
    invalidateCache();
    emit changed(this->mapRoles.keys());
}

QList<int> ProxyOrm::Join::roles() const
{
    return mapRoles.keys();
}
