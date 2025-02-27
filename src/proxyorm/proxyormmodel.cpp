#include "proxyormmodel.h"
#include "fromsource.h"

ProxyOrm::ProxyOrmModel::ProxyOrmModel(const QAbstractListModel *sourceModel,
                                       QMap<int, int> roles,
                                       QObject *parent)
    : QAbstractListModel{parent}
    , sourceModel(sourceModel)
{
    ProxyOrm::FromSource *source = new ProxyOrm::FromSource(sourceModel, roles);

    for (const auto &role : std::as_const(roles)) {
        sourceMap.insert(role, source);
    }

    connect(sourceModel, &QAbstractListModel::modelReset, this, [this, source]() {
        source->invalidateCache();
        invalidate();
    });

    connect(sourceModel, &QAbstractListModel::rowsInserted, this, [this, source]() {
        source->invalidateCache();
        invalidate();
    });
    connect(sourceModel, &QAbstractListModel::rowsRemoved, this, [this, source]() {
        source->invalidateCache();
        invalidate();
    });
    connect(sourceModel, &QAbstractListModel::dataChanged, this, [this, source]() {
        source->invalidateCache();
        invalidate();
    });
}

void ProxyOrm::ProxyOrmModel::addSource(const ISource *source)
{
    const auto &roles = source->roles();
    for (const auto &role : roles) {
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
    if (!source) {
        qWarning() << "No source found for role:" << role;
        return QVariant{};
    }
    return source->data(sourceRow, role);
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

void ProxyOrm::ProxyOrmModel::enabledAsync(bool enabled)
{
    isAsync = enabled;
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

bool ProxyOrm::ProxyOrmModel::isSortGroupFilterRole(int role) const
{
    if (role == sortRole) {
        return true;
    }
    if (role == groupByRole) {
        return true;
    }
    return whereMap.contains(role);
}

bool ProxyOrm::ProxyOrmModel::isSortGroupFilterRole(QList<int> roles) const
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

void ProxyOrm::ProxyOrmModel::beginSoftResetModel()
{
    oldSize = sortedFilteredIndex.count();
}

void ProxyOrm::ProxyOrmModel::endSoftResetModel()
{
    int diff = sortedFilteredIndex.count() - oldSize;
    if (diff > 0) {
        beginInsertRows(QModelIndex(), 0, diff - 1);
        endInsertRows();
    } else if (diff < 0) {
        beginRemoveRows(QModelIndex(), 0, -diff - 1);
        endRemoveRows();
    }

    if (sortedFilteredIndex.count() != 0) {
        QModelIndex topLeft = index(0, 0);
        QModelIndex bottomRight = index(sortedFilteredIndex.count() - 1, 0);
        emit dataChanged(topLeft, bottomRight);
    }
}

void ProxyOrm::ProxyOrmModel::sourceChanged(QList<int> role)
{
    if (!mEnabled) {
        needToInvalidate = true;
        return;
    }

    if (isSortGroupFilterRole(role)) {
        invalidate();
    } else {
        emit dataChanged(this->index(0, 0), this->index(this->rowCount() - 1, 0), role);
    }
}

void ProxyOrm::ProxyOrmModel::performInvalidate()
{
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
            }
        }

        sortedFilteredIndex.clear();
        for (const auto &group : std::as_const(groups)) {
            sortedFilteredIndex.append(group.at(0));
        }
    }

    if (sortedFilteredIndex.count() > 1 && sortRole != -1) {
        quickSortRecursive(0, rowCount() - 1);
    }
}

void ProxyOrm::ProxyOrmModel::invalidate()
{
    if (!mEnabled) {
        needToInvalidate = true;
        return;
    }

    if (isStart) {
        isCancel = true;
        futureInvalidate.cancel();
        futureInvalidate.waitForFinished();
    }

    if (isAsync) {
        isStart = true;
        if (!isCancel) {
            beginSoftResetModel();
            isCancel = false;
        }
        futureInvalidate = QtConcurrent::run(QThreadPool::globalInstance(), [this]() {
                               performInvalidate();
                           }).then([this]() {
            if (!isCancel) {
                endSoftResetModel();
            }
            isStart = false;
        });

    } else {
        beginSoftResetModel();
        performInvalidate();
        endSoftResetModel();
    }
}

void ProxyOrm::ProxyOrmModel::enabled(bool enabled)
{
    this->mEnabled = enabled;
    if (mEnabled && needToInvalidate) {
        invalidate();
    }
}
