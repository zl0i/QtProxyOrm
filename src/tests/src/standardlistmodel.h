#ifndef STANDARDLISTMODEL_H
#define STANDARDLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QStandardItemModel>

#include "indexedabstractmodel.h"

class StandardListModel : public ProxyOrm::IndexedAbstractModel
{
public:
    StandardListModel(int row, QObject *parent = nullptr);

    ~StandardListModel();

    int rowCount(const QModelIndex &index = QModelIndex{}) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool insertRow(int row, QStandardItem *item);

    // Добавление элемента в конец
    void appendRow(QStandardItem *item);

    // Удаление строки
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    // Очистка модели
    void clear();

    QModelIndex byIndex(QVariant) const override;

private:
    QList<QStandardItem *> list;
};

#endif // STANDARDLISTMODEL_H
