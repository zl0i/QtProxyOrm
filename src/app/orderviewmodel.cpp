#include "orderviewmodel.h"
#include "aggregatebyrow.h"
#include "case.h"
#include "join.h"
#include "ordermodel.h"
#include "staffmodel.h"
#include "usermodel.h"

OrderViewModel::OrderViewModel(const QAbstractListModel *sourceModel,
                               const QAbstractListModel *userModel,
                               const QAbstractListModel *staffModel,
                               QObject *parent)
    : ProxyOrm::ProxyOrmModel{sourceModel,
                              {{OrderViewModel::IdRole, OrderModel::IdRole},
                               {OrderViewModel::NumberRole, OrderModel::NumberRole},
                               {OrderViewModel::UuidRole, OrderModel::UuidRole},
                               {OrderViewModel::CreatedByRole, OrderModel::CreatedByRole},
                               {OrderViewModel::TotalRole, OrderModel::TotalRole},
                               {OrderViewModel::CreatedAtRole, OrderModel::CreatedAtRole}},
                              parent}
{
    ProxyOrm::Join *userJoin
        = new ProxyOrm::Join(this,
                             OrderModel::CreatedByRole,
                             userModel,
                             UserModel::IdRole,
                             {{OrderViewModel::FirstnameRole, UserModel::FirstnameRole},
                              {OrderViewModel::LastnameRole, UserModel::LastnameRole},
                              {OrderViewModel::StaffIdRole, UserModel::StaffIdRole}});

    addSource(userJoin);

    ProxyOrm::Join *staffJoin
        = new ProxyOrm::Join(this,
                             OrderViewModel::StaffIdRole,
                             staffModel,
                             StaffModel::IdRole,
                             {{OrderViewModel::StaffNameRole, StaffModel::NameRole},
                              {OrderViewModel::StaffRateRole, StaffModel::RateRole}});

    addSource(staffJoin);

    auto userOrderTotal = new ProxyOrm::AggregateByRow(this,
                                                       OrderViewModel::CreatedByRole,
                                                       OrderViewModel::TotalRole,
                                                       ProxyOrm::TypeAggregate::Avg,
                                                       OrderViewModel::UserOrderTotalRole);

    addSource(userOrderTotal);

    auto userCountOrder = new ProxyOrm::AggregateByRow(this,
                                                       OrderViewModel::CreatedByRole,
                                                       OrderViewModel::IdRole,
                                                       ProxyOrm::TypeAggregate::Count,
                                                       OrderViewModel::CountOrderRole);
    addSource(userCountOrder);

    auto caseOrder = new ProxyOrm::Case(this,
                                        OrderViewModel::CountOrderRole,
                                        {{1, "один заказ"}, {2, "два заказа"}},
                                        OrderViewModel::CaseRole,
                                        "мало заказов");

    addSource(caseOrder);

    auto orWhere = ProxyOrm::OrWhere{ProxyOrm::Where{ProxyOrm::Where::Equals, 11},
                                     ProxyOrm::Where{ProxyOrm::Where::Equals, 1}};

    // andWhere(OrderViewModel::CreatedByRole, orWhere);
    // andWhere(OrderViewModel::TotalRole, ProxyORM::Where{ProxyORM::Where::Equals, 1000.25});
    // andWhere(OrderViewModel::StaffRateRole, ProxyORM::Where::MoreThen, 100);

    // sort(OrderViewModel::StaffRateRole, Qt::SortOrder::AscendingOrder);
    // sort(OrderViewModel::CreatedByRole, Qt::SortOrder::AscendingOrder);
    // groupBy(OrderViewModel::CreatedByRole);

    enabledAsync(true);
}

QHash<int, QByteArray> OrderViewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "idRole";
    roles[NumberRole] = "numberRole";
    roles[UuidRole] = "uuidRole";
    roles[CreatedByRole] = "createdByRole";
    roles[TotalRole] = "totalRole";
    roles[CreatedAtRole] = "createdAtRole";
    roles[FirstnameRole] = "firstnameRole";
    roles[LastnameRole] = "lastnameRole";
    roles[StaffIdRole] = "staffIdRole";
    roles[StaffNameRole] = "staffNameRole";
    roles[StaffRateRole] = "staffRateRole";
    roles[UserOrderTotalRole] = "userOrderTotalRole";
    roles[CountOrderRole] = "countOrderRole";
    roles[CaseRole] = "caseRole";
    return roles;
}
