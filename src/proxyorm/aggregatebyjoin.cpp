#include "aggregatebyjoin.h"

ProxyOrm::AggregateByJoin::AggregateByJoin(const QAbstractListModel *sourceModel,
                                           const int sourceRole,
                                           const QAbstractListModel *joinModel,
                                           const int joinRole,
                                           const int aggregateRole,
                                           const TypeAggregate type,
                                           const int role,
                                           const QMap<int, Where> whereMap)
    : AggregateByRow(sourceModel, sourceRole, aggregateRole, type, role)
    , joinModel(joinModel)
    , joinRole(joinRole)
    , whereMap(whereMap)
{}

ProxyOrm::AggregateByJoin::AggregateByJoin(const AggregateByJoin &other)
    : AggregateByRow(other.sourceModel,
                     other.sourceRole,
                     other.aggregateRole,
                     other.typeAggregate,
                     other.role)
    , joinModel(other.joinModel)
    , joinRole(other.joinRole)
    , whereMap(other.whereMap)
{}

ProxyOrm::AggregateByJoin &ProxyOrm::AggregateByJoin::operator=(const AggregateByJoin &other)
{
    if (this == &other) {
        return *this;
    }
    return *new (this) AggregateByJoin(other);
}

QVariant ProxyOrm::AggregateByJoin::aggregate(QVariant whereValue) const
{
    QVariant result{};
    int count = 0;

    int type;
    for (int i = 0; i < joinModel->rowCount(); i++, count++) {
        if (!whereMap.empty()) {
            bool include = true;
            for (auto [key, where] : whereMap.asKeyValueRange()) {
                auto test = joinModel->data(joinModel->index(i, 0), key);
                if (!where.isAccepted(test)) {
                    include = false;
                }
            }
            if (!include) {
                continue;
            }
        }
        auto test = joinModel->data(joinModel->index(i, 0), joinRole);
        if (test != whereValue) {
            continue;
        }
        auto value = joinModel->data(joinModel->index(i, 0), aggregateRole);
        if (typeAggregate == TypeAggregate::Count) {
            result = result.toInt() + 1;
        } else if (typeAggregate == TypeAggregate::Sum) {
            type = value.typeId();
            switch (type) {
            case QMetaType::Int:
                result = typedAggregate(result.toInt(), value.toInt());
                break;
            case QMetaType::Float:
                result = typedAggregate(result.toFloat(), value.toFloat());
                break;
            case QMetaType::Double:
                result = typedAggregate(result.toDouble(), value.toDouble());
                break;
            }
        } else if (typeAggregate == TypeAggregate::Min) {
            if (!isMoreComprasion(value, result)) {
                result = value;
            }
        } else if (typeAggregate == TypeAggregate::Max) {
            if (isMoreComprasion(value, result)) {
                result = value;
            }
        } else if (typeAggregate == TypeAggregate::First) {
            return value;
        } else if (typeAggregate == TypeAggregate::Last) {
            result = value;
        }
    }
    if (typeAggregate == TypeAggregate::Avg) {
        return QVariant{result.toDouble() / count};
    }
    return result;
}
