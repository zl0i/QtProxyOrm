#ifndef STAFFMODEL_H
#define STAFFMODEL_H

#include <QAbstractListModel>
#include <QObject>

struct Staff
{
    int id = 0;
    QString name;
    QString title;
    double rate = 0;
};

class StaffModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit StaffModel(QObject *parent = nullptr);

    enum StaffRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        TitleRole,
        RateRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<Staff> list;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void update();
    void append();
    void remove();
    void dataChange();
    void reset();
    void clear();
};

#endif // STAFFMODEL_H
