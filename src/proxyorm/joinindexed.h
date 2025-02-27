#ifndef JOININDEXED_H
#define JOININDEXED_H

#include <QObject>

#include "indexedabstractmodel.h"
#include "isource.h"

namespace ProxyOrm {

class JoinIndexed : public ISource
{
    Q_OBJECT
public:
    JoinIndexed(const QAbstractListModel *sourceModel,
                const int sourceRole,
                const IndexedAbstractModel *joinModel,
                const QMap<int, int> mapRoles);

    QList<int> roles() const override;

protected:
    QVariant templateData(int row, int role) const override;

    virtual void invalidate();

private:
    const QAbstractListModel *sourceModel;
    const int sourceRole = 0; // роль по которой сравниваем в base модели
    const IndexedAbstractModel *joinModel;
    const QMap<int, int> mapRoles; // <роль в прокси модели, роль в join модели>
};

} // namespace ProxyOrm
#endif // JOININDEXED_H
