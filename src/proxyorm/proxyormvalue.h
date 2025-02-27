#ifndef PROXYORMVALUE_H
#define PROXYORMVALUE_H

#include <QAbstractItemModel>
#include <QObject>
#include <QtConcurrent/QtConcurrent>

#include "where.h"

namespace ProxyOrm {

class ProxyOrmValue : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value NOTIFY changed FINAL)
public:
    enum TypeAggregate { Count = 0, Sum, Avg, Min, Max, Custom };

    QVariant value();
    void enabled(bool enabled);

protected:
    ProxyOrmValue(const QAbstractItemModel *sourceModel,
                  const TypeAggregate type,
                  const int role,
                  QObject *parent = nullptr);

    void where(int whereRole, Where::TypeComparison type, QVariant condition);
    void enabledAsync(bool enabled);
    virtual QVariant customArggregate(const QList<QModelIndex> &list);

    const QAbstractItemModel *sourceModel;

private:
    QVariant mValue;
    QMap<int, Where> whereMap;
    bool isStart = false;
    bool isCancel = false;

    QFuture<void> futureInvalidate = QtFuture::makeReadyVoidFuture();

    const TypeAggregate type;
    const int role = -1;

    bool mEnabled{true};
    bool needToInvalidate{false};
    bool isAsync{false};

    QVariant performInvalidation();

    bool isAggWhereRole(const QList<int> &roles) const;

signals:
    void changed();

protected slots:
    void invalidate();
};

} // namespace ProxyOrm

#endif // PROXYORMVALUE_H
