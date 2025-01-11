#include "usermodel.h"

UserModel::UserModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int UserModel::rowCount(const QModelIndex &) const
{
    return list.count();
}

QVariant UserModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= list.count())
        return QVariant();

    switch (role) {
    case IdRole:
        return list.at(index.row()).id;
    case FirstnameRole:
        return list.at(index.row()).firstname;
    case LastnameRole:
        return list.at(index.row()).lastname;
    case PinRole:
        return list.at(index.row()).pin;
    case EmailRole:
        return list.at(index.row()).email;
    case InnRole:
        return list.at(index.row()).inn;
    case StaffIdRole:
        return list.at(index.row()).staffId;
    }
    return QVariant();
}

void UserModel::update()
{
    beginResetModel();
    list.clear();
    list.append({1, "firstname1", "lastname1", "1111", "test1@mail.ru", "366..1", 1});
    list.append({2, "firstname2", "lastname2", "2222", "test2@mail.ru", "366..2", 2});
    list.append({3, "firstname3", "lastname3", "3333", "test3@mail.ru", "366..3", 3});
    list.append({4, "firstname4", "lastname4", "4444", "test4@mail.ru", "366..4", 4});
    list.append({5, "firstname5", "lastname5", "5555", "test5@mail.ru", "366..5", 2});
    list.append({6, "firstname6", "lastname6", "6666", "test6@mail.ru", "366..6", 3});
    list.append({7, "firstname7", "lastname7", "7777", "test7@mail.ru", "366..7", 3});
    list.append({8, "firstname8", "lastname8", "8888", "test8@mail.ru", "366..8", 4});
    list.append({9, "firstname9", "lastname9", "9999", "test9@mail.ru", "366..9", 5});
    list.append({10, "firstname10", "lastname10", "1018", "test10@mail.ru", "366..10", 5});
    endResetModel();
}

void UserModel::append()
{
    beginInsertRows(QModelIndex{}, list.count(), list.count());
    list.append({11, "firstname11", "lastname11", "1118", "test11@mail.ru", "366..11", 6});
    endInsertRows();
}

void UserModel::remove()
{
    beginRemoveRows(QModelIndex{}, 0, 0);
    list.remove(0);
    endRemoveRows();
}

void UserModel::dataChange()
{
    list[0].firstname += list.at(0).firstname;
    emit dataChanged(index(0, 0),
                     index(0, 0),
                     QVector<int>{UserModel::FirstnameRole, UserModel::LastnameRole});
}

void UserModel::reset()
{
    beginResetModel();
    endResetModel();
}

void UserModel::clear()
{
    beginResetModel();
    list.clear();
    endResetModel();
}

QHash<int, QByteArray> UserModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "idRole";
    roles[FirstnameRole] = "firstnameRole";
    roles[LastnameRole] = "lastnameRole";
    roles[PinRole] = "pinRole";
    roles[InnRole] = "innRole";
    roles[EmailRole] = "emailRole";
    return roles;
}
