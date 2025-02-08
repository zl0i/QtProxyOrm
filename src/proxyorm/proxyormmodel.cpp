#include "proxyormmodel.h"
#include "fromsource.h"

ProxyOrm::ProxyOrmModel::ProxyOrmModel(QAbstractListModel *sourceModel,
                                       QMap<int, int> roles,
                                       QObject *parent)
    : QAbstractListModel{parent}
    , sourceModel(sourceModel)
{
    ProxyOrm::FromSource *source = new ProxyOrm::FromSource(sourceModel, roles);

    for (auto role : roles) {
        sourceMap.insert(role, source);
    }

    connect(sourceModel, &QAbstractListModel::modelReset, [this, source]() {
        source->invalidateCache();
        invalidate();
    });

    connect(sourceModel, &QAbstractListModel::rowsInserted, [this, source]() {
        source->invalidateCache();
        invalidate();
    });
    connect(sourceModel, &QAbstractListModel::rowsRemoved, [this, source]() {
        source->invalidateCache();
        invalidate();
    });
    connect(sourceModel, &QAbstractListModel::dataChanged, [this, source]() {
        source->invalidateCache();
        invalidate();
    });
}

void ProxyOrm::ProxyOrmModel::addSource(ISource *source)
{
    for (auto role : source->roles()) {
        sourceMap.insert(role, source);
    }
    connect(source, &ISource::changed, this, &ProxyOrmModel::sourceChanged);
}

int ProxyOrm::ProxyOrmModel::rowCount(const QModelIndex &) const
{
    if (!sourceModel) {
        return 0;
    }
    if (isFiltered) {
        return sortedFilteredIndex.count();
    }
    return sourceModel->rowCount();
}

QVariant ProxyOrm::ProxyOrmModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    int sourceRow = isFiltered ? sortedFilteredIndex.at(index.row()).row() : index.row();

    auto source = sourceMap.value(role);
    if (source) {
        return source->data(sourceRow, role);
    }
    return QVariant{};
}

void ProxyOrm::ProxyOrmModel::andWhere(int whereRole, Where where)
{
    whereMap.insert(whereRole, OrWhere({where}));
}

void ProxyOrm::ProxyOrmModel::andWhere(int whereRole, OrWhere where)
{
    whereMap.insert(whereRole, where);
}

void ProxyOrm::ProxyOrmModel::clearWhere(int role)
{
    whereMap.remove(role);
}

void ProxyOrm::ProxyOrmModel::clearWhere()
{
    whereMap.clear();
}

void ProxyOrm::ProxyOrmModel::sort(int role, Qt::SortOrder type)
{
    this->sortRole = role;
    sortFunc.type = type;
}

void ProxyOrm::ProxyOrmModel::groupBy(int role)
{
    groupByRole = role;
}

int ProxyOrm::ProxyOrmModel::partition(int low, int high)
{
    QVariant pivot = data(index(high), sortRole);
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        QVariant value = data(index(j), sortRole);
        if (sortFunc.compare(value, pivot)) {
            ++i;
            if (i != j) {
                sortedFilteredIndex.swapItemsAt(i, j);
            }
        }
    }
    sortedFilteredIndex.swapItemsAt(i + 1, high);
    return i + 1;
}

void ProxyOrm::ProxyOrmModel::quickSortRecursive(int low, int high)
{
    if (low < high) {
        int pi = partition(low, high);

        quickSortRecursive(low, pi - 1);
        quickSortRecursive(pi + 1, high);
    }
}

bool ProxyOrm::ProxyOrmModel::isSortGroupFilterRole(int role)
{
    if (role == sortRole) {
        return true;
    }
    if (role == groupByRole) {
        return true;
    }
    return whereMap.contains(role);
}

bool ProxyOrm::ProxyOrmModel::isSortGroupFilterRole(QList<int> roles)
{
    if (roles.contains(sortRole)) {
        return true;
    }
    if (roles.contains(groupByRole)) {
        return true;
    }
    for (auto [role, where] : whereMap.asKeyValueRange()) {
        if (roles.contains(role)) {
            return true;
        }
    }
    return false;
}

void ProxyOrm::ProxyOrmModel::sourceChanged(QList<int> role)
{
    if (!mEnabled) {
        needToInvalidate = true;
        return;
    }

    if (isSortGroupFilterRole(role)) {
        qDebug() << "source changed -> invalidate";
        invalidate();
    } else {
        qDebug() << "source changed -> dataChanged";
        emit dataChanged(this->index(0, 0), this->index(this->rowCount() - 1, 0), role);
    }
}

void ProxyOrm::ProxyOrmModel::invalidate()
{
    if (!mEnabled) {
        needToInvalidate = true;
        return;
    }

    qDebug() << "invalidate";

    beginSoftResetModel();

    isFiltered = false;
    sortedFilteredIndex.clear();
    for (int i = 0; i < sourceModel->rowCount(); i++) {
        if (!whereMap.empty()) {
            bool include = true;
            for (auto [key, where] : whereMap.asKeyValueRange()) {
                auto value = this->data(index(i, 0), key);
                if (!where.isAccepted(value)) {
                    include = false;
                }
            }
            if (include) {
                sortedFilteredIndex.append(index(i, 0));
            }
        } else {
            sortedFilteredIndex.append(index(i, 0));
        }
    }
    isFiltered = true;

    if (sortedFilteredIndex.count() > 1 && groupByRole != -1) {
        QHash<QString, QList<QModelIndex>> groups;
        for (int i = 0; i < this->rowCount(); ++i) {
            QModelIndex index = this->index(i, 0);
            QString key = this->data(index, groupByRole).toString();
            if (!groups.contains(key)) {
                groups[key] = QList<QModelIndex>{index};
            } else {
                auto list = groups.value(key);
                list.append(index);
                groups[key] = list;
                // groups[key].append(index);
            }
        }

        sortedFilteredIndex.clear();
        // Переносим сгруппированные индексы в общий список
        for (const auto &group : groups) {
            sortedFilteredIndex.append(group.at(0));
        }
    }

    if (sortedFilteredIndex.count() > 1 && sortRole != -1) {
        quickSortRecursive(0, rowCount() - 1);
    }

    qDebug() << "invalidate end" << sortedFilteredIndex.count();
    endSoftResetModel();
}

void ProxyOrm::ProxyOrmModel::enabled(bool enabled)
{
    this->mEnabled = enabled;
    if (mEnabled && needToInvalidate) {
        invalidate();
    }
}
