#ifndef CASE_H
#define CASE_H

#include <QObject>

#include "isource.h"
#include "qabstractitemmodel.h"

namespace ProxyOrm {

class Case : public ProxyOrm::ISource
{
    Q_OBJECT
public:
    Case(const QAbstractListModel *sourceModel,
         int sourceRole,
         QList<QVariantPair> list,
         int role,
         QVariant defaultValue = QVariant{});

    QList<int> roles() const override;

protected:
    QVariant templateData(int row, int role) const override;

private:
    const QAbstractListModel *sourceModel;
    const int sourceRole;
    const int role;
    QVariantHash map;
    const QVariant defaultValue;

private slots:
    void invalidate();
};

} // namespace ProxyOrm

#endif // CASE_H
