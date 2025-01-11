#include "ordermodel.h"

OrderModel::OrderModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int OrderModel::rowCount(const QModelIndex &) const
{
    return list.count();
}

QVariant OrderModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= list.count())
        return QVariant();

    switch (role) {
    case IdRole:
        return list.at(index.row()).id;
    case NumberRole:
        return list.at(index.row()).number;
    case UuidRole:
        return list.at(index.row()).uuid;
    case CreatedByRole:
        return list.at(index.row()).createdBy;
    case TotalRole:
        return list.at(index.row()).total;
    case CreatedAtRole:
        return list.at(index.row()).createdAt;
    }
    return QVariant();
}

void OrderModel::append(int userId, double total)
{
    beginResetModel();
    list.append({15,
                 "15",
                 QUuid::createUuid().toString(),
                 userId,
                 total,
                 QDateTime::currentSecsSinceEpoch()});
    endResetModel();
}

void OrderModel::update()
{
    beginResetModel();
    list.clear();
    list.append({1, "1", "uuid_1_user_1", 1, 1000.25, 176000000});
    list.append({2, "2", "uuid_2_user_2", 2, 500.25, 176000000});
    list.append({3, "3", "uuid_3_user_1", 1, 933.25, 176000000});
    list.append({4, "4", "uuid_4_user_3", 3, 10526.25, 176000000});
    list.append({5, "5", "uuid_5_user_1", 1, 15001.205, 176000000});
    list.append({6, "6", "uuid_6_user_4", 4, 1000.25, 176000000});
    list.append({7, "7", "uuid_7_user_1", 1, 1002.25, 176000000});
    list.append({8, "8", "uuid_8_user_10", 10, 1010.25, 176000000});
    list.append({9, "9", "uuid_9_user_5", 5, 1000.25, 176000000});
    list.append({10, "10", "uuid_10_user_5", 5, 1000.25, 176000000});
    list.append({11, "11", "uuid_11_user_8", 8, 1100.25, 176000000});
    list.append({12, "12", "uuid_12_user_9", 9, 1005.25, 176000000});
    list.append({13, "13", "uuid_13_user_1", 1, 1000.25, 176000000});
    list.append({14, "14", "uuid_14_user_6", 6, 1001.25, 176000000});
    list.append({15, "15", "uuid_15_user_1", 10, 1001.25, 176000000});
    endResetModel();
}

void OrderModel::append()
{
    beginInsertRows(QModelIndex{}, list.count(), list.count());
    list.append({14, "14", "uuid_14_user_11", 11, 1001, 176000000});
    endInsertRows();
}

void OrderModel::remove()
{
    beginRemoveRows(QModelIndex{}, 0, 0);
    list.remove(0);
    endRemoveRows();
}

void OrderModel::dataChange()
{
    list[0].total += list.at(0).total;
    list[0].id += list.at(0).id;
    emit dataChanged(index(0, 0),
                     index(0, 0),
                     QVector<int>{OrderModel::TotalRole, OrderModel::IdRole});
}

void OrderModel::reset()
{
    beginResetModel();
    endResetModel();
}

void OrderModel::clear()
{
    beginResetModel();
    list.clear();
    endResetModel();
}

QHash<int, QByteArray> OrderModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "idRole";
    roles[NumberRole] = "numberRole";
    roles[UuidRole] = "uuidRole";
    roles[CreatedByRole] = "createdByRole";
    roles[TotalRole] = "totalRole";
    roles[CreatedAtRole] = "createdAtRole";
    return roles;
}
