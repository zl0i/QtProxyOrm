#include <QtTest>

#include <QSignalSpy>
#include <QStandardItemModel>
#include <QTest>

#include "proxyormvalue.h"
#include "where.h"

// add necessary includes here

class TestProxyOrmValue : public ProxyOrm::ProxyOrmValue
{
public:
    TestProxyOrmValue(const QAbstractItemModel *sourceModel,
                      const TypeAggregate type,
                      const int role)
        : ProxyOrm::ProxyOrmValue(sourceModel, type, role)
    {}

    std::function<QVariant(const QList<QModelIndex> &)> customFunc;

    QVariant customArggregate(const QList<QModelIndex> &list) override
    {
        if (customFunc) {
            return customFunc(list);
        }
        return ProxyOrm::ProxyOrmValue::customArggregate(list);
    }

    void where(int whereRole, ProxyOrm::Where::TypeComparison type, QVariant condition)
    {
        ProxyOrm::ProxyOrmValue::where(whereRole, type, condition);
    }

    void enabledAsync(bool enabled) { ProxyOrm::ProxyOrmValue::enabledAsync(enabled); }
};

class TestProxyOrmValueTest : public QObject
{
    Q_OBJECT

public:
    TestProxyOrmValueTest();
    ~TestProxyOrmValueTest();

private slots:
    void testCountNoFilter();
    void testSumNoFilter();
    void testAvgWithFilter();
    void testMinEmptyFilter();
    void testAsyncSum();
    void testEnabledDisabled();
    void testCustomAggregation();
};

TestProxyOrmValueTest::TestProxyOrmValueTest() {}

TestProxyOrmValueTest::~TestProxyOrmValueTest() {}

void TestProxyOrmValueTest::testCountNoFilter()
{
    QStandardItemModel model(0, 1);
    TestProxyOrmValue proxy(&model, ProxyOrm::ProxyOrmValue::Count, Qt::DisplayRole);
    model.insertRows(0, 3);
    QCOMPARE(proxy.value().toInt(), 3);
}

void TestProxyOrmValueTest::testSumNoFilter()
{
    QStandardItemModel model(0, 1);
    TestProxyOrmValue proxy(&model, ProxyOrm::ProxyOrmValue::Sum, Qt::UserRole + 1);
    model.insertRows(0, 3);
    model.setData(model.index(0, 0), 10.0, Qt::UserRole + 1);
    model.setData(model.index(1, 0), 20.0, Qt::UserRole + 1);
    model.setData(model.index(2, 0), 30.0, Qt::UserRole + 1);
    QCOMPARE(proxy.value().toDouble(), 60.0);
}

void TestProxyOrmValueTest::testAvgWithFilter()
{
    QStandardItemModel model(0, 1);
    TestProxyOrmValue proxy(&model, ProxyOrm::ProxyOrmValue::Avg, Qt::UserRole + 1);
    proxy.where(Qt::UserRole + 2, ProxyOrm::Where::MoreThen, 25); // Фильтр: возраст > 25
    model.insertRows(0, 3);
    model.setData(model.index(0, 0), 20, Qt::UserRole + 2);   // Возраст
    model.setData(model.index(0, 0), 10.0, Qt::UserRole + 1); // Значение
    model.setData(model.index(1, 0), 30, Qt::UserRole + 2);
    model.setData(model.index(1, 0), 20.0, Qt::UserRole + 1);
    model.setData(model.index(2, 0), 40, Qt::UserRole + 2);
    model.setData(model.index(2, 0), 30.0, Qt::UserRole + 1);
    // Ожидаемое среднее: (20 + 30) / 2 = 25.0
    QCOMPARE(proxy.value().toDouble(), 25.0);
}

void TestProxyOrmValueTest::testMinEmptyFilter()
{
    QStandardItemModel model(0, 1);
    TestProxyOrmValue proxy(&model, ProxyOrm::ProxyOrmValue::Min, Qt::UserRole + 1);
    proxy.where(Qt::UserRole + 2, ProxyOrm::Where::Equals, 100); // Фильтр, исключающий все строки
    model.insertRows(0, 3);
    model.setData(model.index(0, 0), 10, Qt::UserRole + 2);
    model.setData(model.index(1, 0), 20, Qt::UserRole + 2);
    model.setData(model.index(2, 0), 30, Qt::UserRole + 2);
    // Должно вернуть invalid QVariant, но текущая реализация может вызвать ошибку
    QVERIFY(!proxy.value().isValid());
}

void TestProxyOrmValueTest::testAsyncSum()
{
    QStandardItemModel model(0, 1);
    TestProxyOrmValue proxy(&model, ProxyOrm::ProxyOrmValue::Sum, Qt::UserRole + 1);
    proxy.enabledAsync(true);
    QSignalSpy spy(&proxy, &ProxyOrm::ProxyOrmValue::changed);
    model.insertRows(0, 3);
    model.setData(model.index(0, 0), 10.0, Qt::UserRole + 1);
    model.setData(model.index(1, 0), 20.0, Qt::UserRole + 1);
    model.setData(model.index(2, 0), 30.0, Qt::UserRole + 1);
    spy.wait(1000);
    QCOMPARE(spy.count(), 1); //иногда может быть 2, еще реже 3
    QCOMPARE(proxy.value().toDouble(), 60.0);
}

void TestProxyOrmValueTest::testEnabledDisabled()
{
    QStandardItemModel model(0, 1);
    TestProxyOrmValue proxy(&model, ProxyOrm::ProxyOrmValue::Count, Qt::DisplayRole);
    proxy.enabled(false);
    model.insertRows(0, 3);
    QVERIFY(!proxy.value().isValid() || proxy.value().toInt() == 0);
    proxy.enabled(true);
    QCOMPARE(proxy.value().toInt(), 3);
}

void TestProxyOrmValueTest::testCustomAggregation()
{
    QStandardItemModel model(0, 1);
    TestProxyOrmValue proxy(&model, ProxyOrm::ProxyOrmValue::Custom, Qt::DisplayRole);
    proxy.customFunc = [](const QList<QModelIndex> &list) { return list.count() * 2; };
    model.insertRows(0, 3);
    QCOMPARE(proxy.value().toInt(), 6);
}

QTEST_MAIN(TestProxyOrmValueTest)

#include "tst_testproxyormvaluetest.moc"
