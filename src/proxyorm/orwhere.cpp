#include "orwhere.h"

ProxyOrm::OrWhere::OrWhere(Where where1, Where where2)
    : Where(TypeComparison::Equals, QVariant{})
    , list{where1, where2}
{}

ProxyOrm::OrWhere::OrWhere(Where where1, Where where2, Where where3)
    : Where(TypeComparison::Equals, QVariant{})
    , list{where1, where2, where3}
{}

ProxyOrm::OrWhere::OrWhere(Where where1, Where where2, Where where3, Where where4)
    : Where(TypeComparison::Equals, QVariant{})
    , list{where1, where2, where3, where4}
{}

ProxyOrm::OrWhere::OrWhere(Where where1, Where where2, Where where3, Where where4, Where where5)
    : Where(TypeComparison::Equals, QVariant{})
    , list{where1, where2, where3, where4, where5}
{}

ProxyOrm::OrWhere::OrWhere(QList<Where> list)
    : Where(TypeComparison::Equals, QVariant{})
    , list(list)
{}

ProxyOrm::OrWhere::OrWhere(const OrWhere &other)
    : Where(TypeComparison::Equals, QVariant{})
    , list(other.list)
{}

ProxyOrm::OrWhere &ProxyOrm::OrWhere::operator=(const OrWhere &other)
{
    if (this == &other) {
        return *this;
    }
    return *new (this) OrWhere(other);
}

bool ProxyOrm::OrWhere::isAccepted(QVariant value) const
{
    for (int i = 0; i < list.count(); i++) {
        if (list.at(i).isAccepted(value)) {
            return true;
        }
    }
    return false;
}
