#ifndef USERMODEL_H
#define USERMODEL_H

#include <QAbstractListModel>
#include <QObject>

struct User
{
    int id = 0;
    QString firstname;
    QString lastname;
    QString email;
    QString inn;
    QString pin;
    int staffId = 0;
};

class UserModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit UserModel(QObject *parent = nullptr);

    enum UserRoles {
        IdRole = Qt::UserRole + 1,
        FirstnameRole,
        LastnameRole,
        PinRole,
        EmailRole,
        InnRole,
        StaffIdRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<User> list;

public slots:
    void update();
    void append();
    void remove();
    void dataChange();
    void reset();
    void clear();
};

#endif // USERMODEL_H
