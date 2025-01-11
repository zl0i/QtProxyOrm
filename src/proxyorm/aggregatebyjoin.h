#ifndef AGGREGATEBYJOIN_H
#define AGGREGATEBYJOIN_H

#include <QAbstractListModel>

#include "aggregatebyrow.h"
#include "isource.h"
#include "where.h"

namespace ProxyOrm {

class AggregateByJoin : public AggregateByRow
{
    Q_OBJECT
public:
    AggregateByJoin(const QAbstractListModel *sourceModel,
                    const int sourceRole,
                    const QAbstractListModel *joinModel,
                    const int joinRole,
                    const int aggregateRole,
                    const TypeAggregate type,
                    const int role,
                    const QMap<int, Where> whereMap = QMap<int, Where>{});
    AggregateByJoin(const AggregateByJoin &other);
    AggregateByJoin &operator=(const AggregateByJoin &other);

protected:
    const QAbstractListModel *joinModel;
    const int joinRole = 0;
    const QMap<int, Where> whereMap;

    QVariant aggregate(QVariant value) const override;
};

} // namespace ProxyOrm

#endif // AGGREGATEBYJOIN_H
