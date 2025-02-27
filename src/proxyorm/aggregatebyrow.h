#ifndef AGGREGATEBYROW_H
#define AGGREGATEBYROW_H

#include <QAbstractListModel>

#include "isource.h"

namespace ProxyOrm {

enum TypeAggregate { Count = 0, Sum, Avg, Min, Max, First, Last };

template<typename T>
T typedAggregate(T a, T b)
{
    return a + b;
};

class AggregateByRow : public ISource
{
    Q_OBJECT
public:
    AggregateByRow(const QAbstractListModel *sourceModel,
                   const int sourceRole,
                   const int aggregateRole,
                   const TypeAggregate type,
                   const int role);

    AggregateByRow(const AggregateByRow &other);
    AggregateByRow &operator=(const AggregateByRow &other);

    QVariant templateData(int row, int role) const override;
    QList<int> roles() const override;

protected:
    const QAbstractListModel *sourceModel;
    const int sourceRole = 0;
    const int aggregateRole = 0;

    const TypeAggregate typeAggregate;
    const int role;

    virtual QVariant aggregate(QVariant value) const;

    bool isMoreComprasion(QVariant a, QVariant b) const;

private:
    mutable QHash<QString, QVariant> hash;

private slots:
    void invalidate();
};

} // namespace ProxyOrm
#endif // AGGREGATEBYROW_H
