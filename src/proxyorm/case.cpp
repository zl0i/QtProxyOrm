#include "case.h"

ProxyOrm::Case::Case(QAbstractListModel *sourceModel,
                     int sourceRole,
                     QList<QVariantPair> list,
                     int role,
                     QVariant defaultValue)
    : sourceModel(sourceModel)
    , sourceRole(sourceRole)
    , role(role)
    , list(list)
    , defaultValue(defaultValue)
{
    connect(sourceModel, &QAbstractListModel::modelReset, this, [this]() { invalidateCache(); });

    connect(sourceModel, &QAbstractListModel::rowsRemoved, this, [this]() { invalidateCache(); });

    connect(sourceModel,
            &QAbstractListModel::dataChanged,
            this,
            [this](const QModelIndex &topLeft,
                   const QModelIndex &bottomRight,
                   const QList<int> &roles) {
                for (int i = topLeft.row(); i < bottomRight.row(); i++) {
                    for (int j = 0; j < roles.count(); j++) {
                        invalidateCache(i);
                    }
                }
            });
}

QList<int> ProxyOrm::Case::roles() const
{
    return {role};
}

QVariant ProxyOrm::Case::templateData(int row, int) const
{
    auto index = sourceModel->index(row, 0);
    auto value = sourceModel->data(index, sourceRole);
    for (int i = 0; i < list.count(); i++) {
        auto pair = list.at(i);
        if (pair.first == value) {
            return pair.second;
        }
    }
    return defaultValue;
}
