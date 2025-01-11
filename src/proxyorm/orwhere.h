#ifndef ORWHERE_H
#define ORWHERE_H

#include "where.h"

namespace ProxyOrm {

class OrWhere : public Where
{
public:
    OrWhere(Where where1, Where where2);
    OrWhere(Where where1, Where where2, Where where3);
    OrWhere(Where where1, Where where2, Where where3, Where where4);
    OrWhere(Where where1, Where where2, Where where3, Where where4, Where where5);
    OrWhere(QList<Where> list);

    OrWhere(const OrWhere &other);
    OrWhere &operator=(const OrWhere &other);

    virtual bool isAccepted(QVariant value) const override;

private:
    QList<Where> list;
};

} // namespace ProxyOrm

#endif // ORWHERE_H
