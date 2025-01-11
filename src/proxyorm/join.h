#ifndef JOIN_H
#define JOIN_H

#include <QAbstractListModel>
#include <QObject>

#include "isource.h"

namespace ProxyOrm {

class Join : public ISource
{
    Q_OBJECT
public:
    Join(const QAbstractListModel *sourceModel,
         const int sourceRole,
         const QAbstractListModel *joinModel,
         const int joinRole,
         const QMap<int, int> mapRoles);

    QList<int> roles() const override;

protected:
    QVariant templateData(int row, int role) const override;

private:
    const QAbstractListModel *sourceModel;
    const int sourceRole = 0; // роль по которой сравниваем в base модели
    const QAbstractListModel *joinModel;
    const int joinRole = 0;        // роль по которой сравниваем в join модели
    const QMap<int, int> mapRoles; // <роль в прокси модели, роль в join модели>
};

} // namespace ProxyOrm
#endif // JOIN_H
