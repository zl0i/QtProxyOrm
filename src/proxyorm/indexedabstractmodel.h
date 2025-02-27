#ifndef INDEXEDABSTRACTMODEL_H
#define INDEXEDABSTRACTMODEL_H

#include <QAbstractListModel>

namespace ProxyOrm {

class IndexedAbstractModel : public QAbstractListModel
{
public:
    IndexedAbstractModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {}

    virtual QModelIndex byIndex(QVariant) const = 0;
};
} // namespace ProxyOrm
#endif // INDEXEDABSTRACTMODEL_H
