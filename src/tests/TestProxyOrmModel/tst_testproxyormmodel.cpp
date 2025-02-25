#include <QStandardItemModel>
#include <QtTest>

// add necessary includes here
#include "proxyormmodel.h"
#include "standardlistmodel.h"

class ProxyOrmModelText : public ProxyOrm::ProxyOrmModel
{
    Q_OBJECT
public:
    ProxyOrmModelText(const QAbstractListModel *model, QMap<int, int> roles)
        : ProxyOrm::ProxyOrmModel(model, roles)
    {}
};

class TestProxyOrmModel : public QObject
{
    Q_OBJECT

public:
    TestProxyOrmModel();
    ~TestProxyOrmModel();

private slots:
    void testDataRetrieval();
};

TestProxyOrmModel::TestProxyOrmModel() {}

TestProxyOrmModel::~TestProxyOrmModel() {}

void TestProxyOrmModel::testDataRetrieval()
{
    StandardListModel sourceModel(3);
    sourceModel.setData(sourceModel.index(0, 0), "Row 0", Qt::DisplayRole);
    sourceModel.setData(sourceModel.index(1, 0), "Row 1", Qt::DisplayRole);
    sourceModel.setData(sourceModel.index(2, 0), "Row 2", Qt::DisplayRole);

    QMap<int, int> roles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrmModelText model(&sourceModel, roles);

    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole).toString(), "Row 0");
    QCOMPARE(model.data(model.index(1, 0), Qt::DisplayRole).toString(), "Row 1");
    QCOMPARE(model.data(model.index(2, 0), Qt::DisplayRole).toString(), "Row 2");
}

QTEST_APPLESS_MAIN(TestProxyOrmModel)

#include "tst_testproxyormmodel.moc"
