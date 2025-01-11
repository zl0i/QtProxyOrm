#ifndef ORDERVIEWMODEL_H
#define ORDERVIEWMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "proxyormmodel.h"

class OrderViewModel : public ProxyOrm::ProxyOrmModel
{
    Q_OBJECT
public:
    enum OrderViewRoles {
        IdRole = Qt::UserRole + 1,
        NumberRole,
        UuidRole,
        CreatedByRole,
        TotalRole,
        CreatedAtRole = 500,

        FirstnameRole,
        LastnameRole,
        StaffIdRole,

        StaffNameRole,
        StaffRateRole,

        UserOrderTotalRole,
        CountOrderRole,
        CaseRole,
    };

    explicit OrderViewModel(QAbstractListModel *sourceModel,
                            QAbstractListModel *userModel,
                            QAbstractListModel *staffModel,
                            QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const;
};

#endif // ORDERVIEWMODEL_H
