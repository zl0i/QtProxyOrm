#ifndef FROMSOURCE_H
#define FROMSOURCE_H

#include "isource.h"
#include "qabstractitemmodel.h"

namespace ProxyOrm {

class FromSource : public ISource
{
    Q_OBJECT
public:
    FromSource(const QAbstractListModel *sourceModel, const QMap<int, int> mapRoles);

    QList<int> roles() const override;

protected:
    virtual QVariant templateData(int row, int role) const override;

private:
    const QAbstractListModel *sourceModel;
    const QMap<int, int> mapRoles;
};

} // namespace ProxyOrm

#endif // FROMSOURCE_H
