#ifndef PROXYORMVALUE_H
#define PROXYORMVALUE_H

#include <QAbstractItemModel>
#include <QObject>

#include "where.h"

namespace ProxyOrm {

class ProxyOrmValue : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value NOTIFY changed FINAL)
public:
    enum TypeAggregate { Count = 0, Sum, Avg, Min, Max, Custom };

    QVariant value();

protected:
    ProxyOrmValue(QAbstractItemModel *sourceModel,
                  TypeAggregate type,
                  int role,
                  QObject *parent = nullptr);

    void where(int whereRole, Where::TypeComparison type, QVariant condition);
    void enabled(bool enabled);

    virtual QVariant customArggregate(const QList<QModelIndex> &list);

    QAbstractItemModel *sourceModel;

private:
    QVariant mValue;
    QMap<int, Where> whereMap;
    QList<QModelIndex> filteredIndex;

    TypeAggregate type;
    int role = -1;

    bool mEnabled{true};
    bool needToInvalidate{false};

signals:
    void changed();

protected slots:
    void invalidate();
};

} // namespace ProxyOrm

#endif // PROXYORMVALUE_H
