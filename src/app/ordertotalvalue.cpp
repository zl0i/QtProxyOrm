#include "ordertotalvalue.h"

OrderTotalValue::OrderTotalValue(OrderModel *model, QObject *parent)
    : ProxyOrmValue(model, TypeAggregate::Sum, OrderModel::TotalRole)
{}
