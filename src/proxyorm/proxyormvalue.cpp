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
                if (roles.isEmpty() || isAggWhereRole(roles)) {
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

    if (isStart) {
        isCancel = true;
        futureInvalidate.cancel();
        futureInvalidate.waitForFinished();
        isCancel = false;
    }

    if (isAsync) {
        isStart = true;
        futureInvalidate = QtConcurrent::run(QThreadPool::globalInstance(), [this]() {
                               QPair<bool, QVariant> pair;
                               pair.second = performInvalidation();
                               pair.first = !isCancel;
                               return pair;
                           }).then([this](QPair<bool, QVariant> pair) {
            if (pair.first) {
                mValue = pair.second;
                emit changed();
            }
        });
    } else {
        mValue = performInvalidation();
        futureInvalidate = QtFuture::makeReadyVoidFuture();
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
    futureInvalidate = QtFuture::makeReadyVoidFuture();
}

QVariant ProxyOrm::ProxyOrmValue::value()
{
    return mValue;
}

QVariant ProxyOrm::ProxyOrmValue::customArggregate(const QList<QModelIndex> &)
{
    return QVariant{};
}

QVariant ProxyOrm::ProxyOrmValue::performInvalidation()
{
    QList<QModelIndex> filteredIndex;
    for (int i = 0; i < sourceModel->rowCount() && !isCancel; i++) {
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
        return filteredIndex.count();
    } else if (type == TypeAggregate::Sum) {
        double sum = 0;
        for (int i = 0; i < filteredIndex.count() && !isCancel; i++) {
            auto index = filteredIndex.at(i);
            sum += sourceModel->data(index, role).toDouble();
        }
        return sum;
    } else if (type == TypeAggregate::Avg) {
        double sum = 0;
        for (int i = 0; i < filteredIndex.count() && !isCancel; i++) {
            auto index = filteredIndex.at(i);
            sum += sourceModel->data(index, role).toDouble();
        }
        return sum / filteredIndex.count();
    } else if (type == TypeAggregate::Min) {
        if (filteredIndex.isEmpty()) {
            return QVariant();
        } else {
            double min = sourceModel->data(filteredIndex.at(0), role).toDouble();
            for (int i = 1; i < filteredIndex.count() && !isCancel; i++) {
                auto index = filteredIndex.at(i);
                if (min > sourceModel->data(index, role).toDouble()) {                    
                    min = sourceModel->data(index, role).toDouble();
                }
            }
            return min;
        }
    } else if (type == TypeAggregate::Max) {
        if (filteredIndex.isEmpty()) {
            mValue = QVariant();
        } else {
            double max = sourceModel->data(filteredIndex.at(0), role).toDouble();
            for (int i = 1; i < filteredIndex.count() && !isCancel; i++) {
                auto index = filteredIndex.at(i);
                if (max < sourceModel->data(index, role).toDouble()) {                   
                    max = sourceModel->data(index, role).toDouble();
                }
            }
            return max;
        }
    } else if (type == TypeAggregate::Custom) {
        return customArggregate(filteredIndex);
    }
    return QVariant{};
}

bool ProxyOrm::ProxyOrmValue::isAggWhereRole(const QList<int> &roles) const
{
    return roles.contains(this->role)
           || std::any_of(whereMap.keyBegin(), whereMap.keyEnd(), [&roles](const auto &r) {
                  return roles.contains(r);
              });
}
