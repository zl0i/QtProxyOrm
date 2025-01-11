#ifndef SORT_H
#define SORT_H

#include <QObject>
#include <QVariant>

namespace ProxyOrm {
struct Sort
{
    Qt::SortOrder type;

    bool compare(QVariant a, QVariant b)
    {
        switch (a.typeId()) {
        case QMetaType::Int:
            return type == Qt::SortOrder::AscendingOrder ? a.toInt() <= b.toInt()
                                                         : a.toInt() >= b.toInt();
        case QMetaType::Float:
            return type == Qt::SortOrder::AscendingOrder ? a.toFloat() <= b.toFloat()
                                                         : a.toFloat() >= b.toFloat();
        case QMetaType::Double:
            return type == Qt::SortOrder::AscendingOrder ? a.toDouble() <= b.toDouble()
                                                         : a.toDouble() >= b.toDouble();
        case QMetaType::QString:
            return type == Qt::SortOrder::AscendingOrder ? a.toString() <= b.toString()
                                                         : a.toString() >= b.toString();
        }
        return false;
    }
};
} // namespace ProxyOrm

#endif // SORT_H
