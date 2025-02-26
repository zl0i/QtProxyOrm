#include "case.h"

ProxyOrm::Case::Case(const QAbstractListModel *sourceModel,
                     int sourceRole,
                     QList<QVariantPair> list,
                     int role,
                     QVariant defaultValue)
    : sourceModel(sourceModel)
    , sourceRole(sourceRole)
    , role(role)
    , defaultValue(defaultValue)
{
    connect(sourceModel, &QAbstractListModel::modelReset, this, &Case::invalidate);
    connect(sourceModel, &QAbstractListModel::rowsRemoved, this, &Case::invalidate);
    connect(sourceModel, &QAbstractListModel::dataChanged, this, &Case::invalidate);

    for (const auto &pair : list) {
        map[pair.first.toString()] = pair.second;
    }
}

QList<int> ProxyOrm::Case::roles() const
{
    return {role};
}

QVariant ProxyOrm::Case::templateData(int row, int) const
{
    if (!sourceModel || row < 0 || row >= sourceModel->rowCount()) {
        return defaultValue;
    }

    auto index = sourceModel->index(row, 0);
    auto value = sourceModel->data(index, sourceRole);
    return map.value(value.toString(), defaultValue);
}

void ProxyOrm::Case::invalidate()
{
    invalidateCache();
}
