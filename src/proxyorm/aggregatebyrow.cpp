#include "aggregatebyrow.h"

ProxyOrm::AggregateByRow::AggregateByRow(const QAbstractListModel *sourceModel,
                                         const int sourceRole,
                                         const int aggregateRole,
                                         const TypeAggregate type,
                                         const int role)
    : ISource()
    , sourceModel(sourceModel)
    , sourceRole(sourceRole)
    , aggregateRole(aggregateRole)
    , typeAggregate(type)
    , role(role)
{
    connect(sourceModel, &QAbstractListModel::modelReset, this, &AggregateByRow::invalidate);
    connect(sourceModel, &QAbstractListModel::rowsInserted, this, &AggregateByRow::invalidate);
    connect(sourceModel, &QAbstractListModel::rowsRemoved, this, &AggregateByRow::invalidate);
    connect(sourceModel, &QAbstractListModel::dataChanged, this, &AggregateByRow::invalidate);
}

ProxyOrm::AggregateByRow::AggregateByRow(const AggregateByRow &other)
    : ISource()
    , sourceModel(other.sourceModel)
    , sourceRole(other.sourceRole)
    , aggregateRole(other.aggregateRole)
    , typeAggregate(other.typeAggregate)
    , role(other.role)
{}

ProxyOrm::AggregateByRow &ProxyOrm::AggregateByRow::operator=(const AggregateByRow &other)
{
    if (this == &other) {
        return *this;
    }
    return *new (this) AggregateByRow(other);
}

QVariant ProxyOrm::AggregateByRow::templateData(int row, int) const
{
    QVariant whereValue = sourceModel->data(sourceModel->index(row, 0), this->sourceRole);
    return aggregate(whereValue);
}

template<typename T>
T ProxyOrm::AggregateByRow::typedAggregate(T a, T b) const
{
    return a + b;
}

QList<int> ProxyOrm::AggregateByRow::roles() const
{
    return {role};
}

QVariant ProxyOrm::AggregateByRow::aggregate(QVariant whereValue) const
{
    QVariant result{};
    int count = 0;

    for (int i = 0; i < sourceModel->rowCount(); i++) {
        auto test = sourceModel->data(sourceModel->index(i, 0), sourceRole);
        if (test != whereValue) {
            continue;
        }
        auto value = sourceModel->data(sourceModel->index(i, 0), aggregateRole);
        if (typeAggregate == TypeAggregate::Count) {
            result = result.toInt() + 1;
        } else if (typeAggregate == TypeAggregate::Sum || typeAggregate == TypeAggregate::Avg) {
            count++;
            int type = value.typeId();
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
            case QMetaType::QString:
                result = typedAggregate(result.toString(), value.toString());
                break;
            }
        } else if (typeAggregate == TypeAggregate::Min) {
            if (!result.isValid()) {
                result = value;
            }
            if (!isMoreComprasion(value, result)) {
                result = value;
            }
        } else if (typeAggregate == TypeAggregate::Max) {
            if (!result.isValid()) {
                result = value;
            }
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

bool ProxyOrm::AggregateByRow::isMoreComprasion(QVariant a, QVariant b) const
{
    if (a.typeId() != b.typeId())
        return false;

    switch (a.typeId()) {
    case QMetaType::Int:
        return a.toInt() > b.toInt();
    case QMetaType::Float:
        return a.toFloat() > b.toFloat();
    case QMetaType::Double:
        return a.toDouble() > b.toDouble();
    case QMetaType::QString:
        return a.toString() > b.toString();
    }
    return false;
}

void ProxyOrm::AggregateByRow::invalidate()
{
    invalidateCache();
    emit changed(roles());
}
