#include "joinindexed.h"

ProxyOrm::JoinIndexed::JoinIndexed(const QAbstractListModel *sourceModel,
                                   const int sourceRole,
                                   const IndexedAbstractModel *joinModel,
                                   const QMap<int, int> mapRoles)
    : ISource()
    , sourceModel(sourceModel)
    , sourceRole(sourceRole)
    , joinModel(joinModel)
    , mapRoles(mapRoles)
{
    connect(joinModel, &IndexedAbstractModel::modelReset, this, &ProxyOrm::JoinIndexed::invalidate);
    connect(joinModel,
            &IndexedAbstractModel::rowsInserted,
            this,
            &ProxyOrm::JoinIndexed::invalidate);
    connect(joinModel, &IndexedAbstractModel::rowsRemoved, this, &ProxyOrm::JoinIndexed::invalidate);
    connect(joinModel, &IndexedAbstractModel::dataChanged, this, &ProxyOrm::JoinIndexed::invalidate);
}

QVariant ProxyOrm::JoinIndexed::templateData(int row, int role) const
{
    if (!sourceModel || !joinModel || row < 0 || row >= sourceModel->rowCount()) {
        return QVariant{};
    }
    if (!mapRoles.contains(role)) {
        return QVariant{};
    }

    auto sourceIndex = sourceModel->index(row, 0);
    auto value = sourceModel->data(sourceIndex, sourceRole);
    auto joinIndex = joinModel->byIndex(value);
    int joinFindRole = mapRoles.value(role);
    return joinModel->data(joinIndex, joinFindRole);
}

void ProxyOrm::JoinIndexed::invalidate()
{
    invalidateCache();
    emit changed(this->mapRoles.keys());
}

QList<int> ProxyOrm::JoinIndexed::roles() const
{
    return mapRoles.keys();
}
