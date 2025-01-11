#ifndef ORDERTOTALVALUE_H
#define ORDERTOTALVALUE_H

#include <QObject>

#include "ordermodel.h"
#include "proxyormvalue.h"

class OrderTotalValue : public ProxyOrm::ProxyOrmValue
{
    Q_OBJECT
public:
    explicit OrderTotalValue(OrderModel *model, QObject *parent = nullptr);
};

#endif // ORDERTOTALVALUE_H
