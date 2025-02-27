#ifndef ISOURCE_H
#define ISOURCE_H

#include <QObject>
#include <QVariant>

namespace ProxyOrm {

class ISource : public QObject
{
    Q_OBJECT
public:
    ISource();

    QVariant data(int row, int role) const;
    virtual QList<int> roles() const = 0;
    void invalidateCache(int row = -1, int role = -1);

protected:
    virtual QVariant templateData(int row, int role) const = 0;

    bool enabledCache{true};

private:
    mutable QHash<int, QHash<int, QVariant>> cache;

signals:
    void changed(QList<int> roles);
};

} // namespace ProxyOrm

#endif // ISOURCE_H
