#include "staffmodel.h"

StaffModel::StaffModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int StaffModel::rowCount(const QModelIndex &) const
{
    return list.count();
}

QVariant StaffModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= list.count())
        return QVariant();

    switch (role) {
    case IdRole:
        return list.at(index.row()).id;
    case NameRole:
        return list.at(index.row()).name;
    case TitleRole:
        return list.at(index.row()).title;
    case RateRole:
        return list.at(index.row()).rate;
    }
    return QVariant();
}

QHash<int, QByteArray> StaffModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "idRole";
    roles[NameRole] = "nameRole";
    roles[TitleRole] = "titleRole";
    roles[RateRole] = "rateRole";
    return roles;
}

void StaffModel::update()
{
    beginResetModel();
    list.clear();
    list.append({1, "Официант", "waiter", 100});
    list.append({2, "Повар", "cook", 200});
    list.append({3, "Бармен", "Bartender", 150});
    list.append({4, "Уборщик", "Cleaner", 75});
    list.append({5, "Администратор", "Admin", 300});
    endResetModel();
}

void StaffModel::append()
{
    beginInsertRows(QModelIndex{}, list.count(), list.count());
    list.append({6, "Администратор2", "Admin2", 400});
    endInsertRows();
}

void StaffModel::remove()
{
    beginRemoveRows(QModelIndex{}, 0, 0);
    list.remove(0);
    endRemoveRows();
}

void StaffModel::dataChange()
{
    list[0].name += list.at(0).name;
    emit dataChanged(index(0, 0), index(0, 0), QVector<int>{StaffModel::NameRole});
}

void StaffModel::reset()
{
    beginResetModel();
    endResetModel();
}

void StaffModel::clear()
{
    beginResetModel();
    list.clear();
    endResetModel();
}
