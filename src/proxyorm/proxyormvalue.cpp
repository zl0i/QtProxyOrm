#include "proxyormvalue.h"

ProxyOrm::ProxyOrmValue::ProxyOrmValue(const QAbstractItemModel *sourceModel,
                                       const TypeAggregate type,
                                       const int role,
                                       QObject *parent)
    : QObject(parent)
    , sourceModel(sourceModel)
    , type(type)
    , role(role)
{
    connect(sourceModel, &QAbstractItemModel::modelReset, this, &ProxyOrmValue::invalidate);
    connect(sourceModel, &QAbstractItemModel::rowsInserted, this, &ProxyOrmValue::invalidate);
    connect(sourceModel, &QAbstractItemModel::rowsRemoved, this, &ProxyOrmValue::invalidate);
    connect(sourceModel,
            &QAbstractItemModel::dataChanged,
            this,
            [this](const QModelIndex &, const QModelIndex &, const QList<int> &roles) {
                if (roles.isEmpty() || roles.contains(this->role)) {
                    invalidate();
                }
            });
}

void ProxyOrm::ProxyOrmValue::where(int whereRole, Where::TypeComparison type, QVariant condition)
{
    whereMap.insert(whereRole, Where{type, condition});
}

void ProxyOrm::ProxyOrmValue::invalidate()
{
    if (!mEnabled) {
        needToInvalidate = true;
        return;
    }

    if (futureInvalidate.isRunning()) {
        futureInvalidate.cancel();
        futureInvalidate.waitForFinished();
    }

    if (isAsync) {
        futureInvalidate = QtConcurrent::run(QThreadPool::globalInstance(), [this]() {
                               performInvalidation();
                           }).then(qApp, [this]() { emit changed(); });
    } else {
        performInvalidation();
        emit changed();
    }
}

void ProxyOrm::ProxyOrmValue::enabled(bool enabled)
{
    this->mEnabled = enabled;
    if (mEnabled && needToInvalidate) {
        invalidate();
    }
}

void ProxyOrm::ProxyOrmValue::enabledAsync(bool enabled)
{
    isAsync = enabled;
}

QVariant ProxyOrm::ProxyOrmValue::value()
{
    return mValue;
}

QVariant ProxyOrm::ProxyOrmValue::customArggregate(const QList<QModelIndex> &)
{
    return QVariant{};
}

void ProxyOrm::ProxyOrmValue::performInvalidation()
{
    filteredIndex.clear();
    for (int i = 0; i < sourceModel->rowCount(); i++) {
        if (futureInvalidate.isCanceled()) {
            return;
        }
        if (!whereMap.empty()) {
            bool include = true;
            for (auto [key, where] : whereMap.asKeyValueRange()) {
                auto value = sourceModel->data(sourceModel->index(i, 0), key);
                if (!where.isAccepted(value)) {
                    include = false;
                }
            }
            if (include) {
                filteredIndex.append(sourceModel->index(i, 0));
            }
        } else {
            filteredIndex.append(sourceModel->index(i, 0));
        }
    }

    if (type == TypeAggregate::Count) {
        mValue = filteredIndex.count();
    } else if (type == TypeAggregate::Sum) {
        double sum = 0;
        for (int i = 0; i < filteredIndex.count(); i++) {
            if (futureInvalidate.isCanceled()) {
                return;
            }

            auto index = filteredIndex.at(i);
            sum += sourceModel->data(index, role).toDouble();
        }
        mValue = sum;
    } else if (type == TypeAggregate::Avg) {
        double sum = 0;
        for (int i = 0; i < filteredIndex.count(); i++) {
            if (futureInvalidate.isCanceled()) {
                return;
            }
            auto index = filteredIndex.at(i);
            sum += sourceModel->data(index, role).toDouble();
        }
        mValue = sum / sourceModel->rowCount();
    } else if (type == TypeAggregate::Min) {
        double min = sourceModel->data(filteredIndex.at(0), role).toDouble();
        for (int i = 1; i < filteredIndex.count(); i++) {
            auto index = filteredIndex.at(i);
            if (min > sourceModel->data(index, role).toDouble()) {
                if (futureInvalidate.isCanceled()) {
                    return;
                }
                min = sourceModel->data(index, role).toDouble();
            }
        }
        mValue = min;
    } else if (type == TypeAggregate::Max) {
        double max = sourceModel->data(filteredIndex.at(0), role).toDouble();
        for (int i = 1; i < filteredIndex.count(); i++) {
            auto index = filteredIndex.at(i);
            if (max < sourceModel->data(index, role).toDouble()) {
                if (futureInvalidate.isCanceled()) {
                    return;
                }
                max = sourceModel->data(index, role).toDouble();
            }
        }
        mValue = max;
    } else if (type == TypeAggregate::Custom) {
        mValue = customArggregate(filteredIndex);
    }
}
