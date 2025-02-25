#include "standardlistmodel.h"

StandardListModel::StandardListModel(int row, QObject *parent)
    : QAbstractListModel(parent)
{
    list.resize(row);
}

StandardListModel::~StandardListModel()
{
    qDeleteAll(list); // Удаляем все элементы при уничтожении модели
}

int StandardListModel::rowCount(const QModelIndex &index) const
{
    return list.count();
}

bool StandardListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= list.size())
        return false;

    QStandardItem *item = list.at(index.row());
    if (item) {
        item->setData(value, role);
    } else {
        item = new QStandardItem();
        item->setData(value, role);
        list.replace(index.row(), item);
    };
    emit dataChanged(index, index, {role});
    return true;
}

bool StandardListModel::insertRow(int row, QStandardItem *item)
{
    if (!item || row < 0 || row > list.size())
        return false;

    beginInsertRows(QModelIndex(), row, row);
    list.insert(row, item);
    endInsertRows();
    return true;
}

void StandardListModel::appendRow(QStandardItem *item)
{
    insertRow(list.size(), item);
}

bool StandardListModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if (row < 0 || row >= list.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row);
    delete list.takeAt(row); // Удаляем объект и убираем из списка
    endRemoveRows();
    return true;
}

void StandardListModel::clear()
{
    beginResetModel();
    qDeleteAll(list);
    list.clear();
    endResetModel();
}
