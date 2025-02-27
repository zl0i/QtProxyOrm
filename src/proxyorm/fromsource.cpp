#include "fromsource.h"
#include "qvariant.h"

ProxyOrm::FromSource::FromSource(const QAbstractListModel *sourceModel,
                                 const QMap<int, int> mapRoles)
    : ISource()
    , sourceModel(sourceModel)
    , mapRoles(mapRoles)
{
    enabledCache = false;

    connect(sourceModel, &QAbstractListModel::modelReset, this, [this]() {
        invalidateCache();
        emit changed(this->mapRoles.keys());
    });

    connect(sourceModel, &QAbstractListModel::rowsInserted, this, [this]() {
        emit changed(this->mapRoles.keys());
    });

    connect(sourceModel, &QAbstractListModel::rowsRemoved, this, [this]() {
        invalidateCache();
        emit changed(this->mapRoles.keys());
    });

    connect(sourceModel,
            &QAbstractListModel::dataChanged,
            this,
            [this](const QModelIndex &topLeft,
                   const QModelIndex &bottomRight,
                   const QList<int> &roles) {
                for (int i = topLeft.row(); i < bottomRight.row(); i++) {
                    for (int j = 0; j < roles.count(); j++) {
                        invalidateCache(i, this->mapRoles[roles.at(j)]);
                    }
                }
                emit changed(this->mapRoles.keys());
            });
}

QVariant ProxyOrm::FromSource::templateData(int row, int role) const
{
    auto index = sourceModel->index(row, 0);
    return sourceModel->data(index, role);
}

QList<int> ProxyOrm::FromSource::roles() const
{
    return mapRoles.keys();
}
