#ifndef ORDERMODEL_H
#define ORDERMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <QObject>
#include <QUuid>

struct Order
{
    int id = 0;
    QString number;
    QString uuid;
    int createdBy;
    double total;
    qint64 createdAt = 0;
};

class OrderModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit OrderModel(QObject *parent = nullptr);

    enum OrderRoles {
        IdRole = Qt::UserRole + 1,
        NumberRole,
        UuidRole,
        CreatedByRole,
        TotalRole,
        CreatedAtRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Order> list;

public slots:
    void append(int userId, double total);
    void update();
    void append();
    void remove();
    void dataChange();
    void reset();
    void clear();
};

#endif // ORDERMODEL_H
