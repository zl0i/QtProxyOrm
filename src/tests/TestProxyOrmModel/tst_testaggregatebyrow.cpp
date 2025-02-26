#include <QtTest>

// add necessary includes here
#include "../src/standardlistmodel.h"
#include "aggregatebyrow.h"

class TestAggregateByRow : public QObject
{
    Q_OBJECT

public:
    TestAggregateByRow();
    ~TestAggregateByRow();

private slots:
    void testCount();
    void testSum();
    void testAvg();
    void testMin();
    void testMax();
    void testFirst();
    void testLast();
    void testCaching();
};

TestAggregateByRow::TestAggregateByRow() {}

TestAggregateByRow::~TestAggregateByRow() {}

void TestAggregateByRow::testCount()
{
    StandardListModel model(4);
    model.setData(model.index(0, 0), "A", Qt::UserRole); // Ключ
    model.setData(model.index(1, 0), "A", Qt::UserRole);
    model.setData(model.index(2, 0), "B", Qt::UserRole);
    model.setData(model.index(3, 0), "A", Qt::UserRole);

    ProxyOrm::AggregateByRow agg(&model,
                                 Qt::UserRole,
                                 Qt::DisplayRole,
                                 ProxyOrm::Count,
                                 Qt::UserRole + 1);
    QCOMPARE(agg.data(0, Qt::UserRole + 1).toInt(), 3); // 3 строки с "A"
    QCOMPARE(agg.data(2, Qt::UserRole + 1).toInt(), 1); // 1 строка с "B"
}

void TestAggregateByRow::testSum()
{
    StandardListModel model(3);
    model.setData(model.index(0, 0), "A", Qt::UserRole);
    model.setData(model.index(0, 0), 10, Qt::DisplayRole);
    model.setData(model.index(1, 0), "A", Qt::UserRole);
    model.setData(model.index(1, 0), 20, Qt::DisplayRole);
    model.setData(model.index(2, 0), "B", Qt::UserRole);
    model.setData(model.index(2, 0), 30, Qt::DisplayRole);

    ProxyOrm::AggregateByRow agg(&model,
                                 Qt::UserRole,
                                 Qt::DisplayRole,
                                 ProxyOrm::Sum,
                                 Qt::UserRole + 1);
    QCOMPARE(agg.data(0, Qt::UserRole + 1).toInt(), 30); // 10 + 20
    QCOMPARE(agg.data(2, Qt::UserRole + 1).toInt(), 30); // 30
}

void TestAggregateByRow::testAvg()
{
    StandardListModel model(3);
    model.setData(model.index(0, 0), "A", Qt::UserRole);
    model.setData(model.index(0, 0), 10, Qt::DisplayRole);
    model.setData(model.index(1, 0), "A", Qt::UserRole);
    model.setData(model.index(1, 0), 20, Qt::DisplayRole);
    model.setData(model.index(2, 0), "B", Qt::UserRole);
    model.setData(model.index(2, 0), 30, Qt::DisplayRole);

    ProxyOrm::AggregateByRow agg(&model,
                                 Qt::UserRole,
                                 Qt::DisplayRole,
                                 ProxyOrm::Avg,
                                 Qt::UserRole + 1);
    QCOMPARE(agg.data(0, Qt::UserRole + 1).toDouble(), 15.0); // (10 + 20) / 2
    QCOMPARE(agg.data(2, Qt::UserRole + 1).toDouble(), 30.0); // 30 / 1
}

void TestAggregateByRow::testMin()
{
    StandardListModel model(3);
    model.setData(model.index(0, 0), "A", Qt::UserRole);
    model.setData(model.index(0, 0), 10, Qt::DisplayRole);
    model.setData(model.index(1, 0), "A", Qt::UserRole);
    model.setData(model.index(1, 0), 20, Qt::DisplayRole);
    model.setData(model.index(2, 0), "B", Qt::UserRole);
    model.setData(model.index(2, 0), 30, Qt::DisplayRole);

    ProxyOrm::AggregateByRow agg(&model,
                                 Qt::UserRole,
                                 Qt::DisplayRole,
                                 ProxyOrm::Min,
                                 Qt::UserRole + 1);
    QCOMPARE(agg.data(0, Qt::UserRole + 1).toInt(), 10);
    QCOMPARE(agg.data(2, Qt::UserRole + 1).toInt(), 30);
}

void TestAggregateByRow::testMax()
{
    StandardListModel model(3);
    model.setData(model.index(0, 0), "A", Qt::UserRole);
    model.setData(model.index(0, 0), 10, Qt::DisplayRole);
    model.setData(model.index(1, 0), "A", Qt::UserRole);
    model.setData(model.index(1, 0), 20, Qt::DisplayRole);
    model.setData(model.index(2, 0), "B", Qt::UserRole);
    model.setData(model.index(2, 0), 30, Qt::DisplayRole);

    ProxyOrm::AggregateByRow agg(&model,
                                 Qt::UserRole,
                                 Qt::DisplayRole,
                                 ProxyOrm::Max,
                                 Qt::UserRole + 1);
    QCOMPARE(agg.data(0, Qt::UserRole + 1).toInt(), 20);
    QCOMPARE(agg.data(2, Qt::UserRole + 1).toInt(), 30);
}

void TestAggregateByRow::testFirst()
{
    StandardListModel model(3);
    model.setData(model.index(0, 0), "A", Qt::UserRole);
    model.setData(model.index(0, 0), 10, Qt::DisplayRole);
    model.setData(model.index(1, 0), "A", Qt::UserRole);
    model.setData(model.index(1, 0), 20, Qt::DisplayRole);
    model.setData(model.index(2, 0), "B", Qt::UserRole);
    model.setData(model.index(2, 0), 30, Qt::DisplayRole);

    ProxyOrm::AggregateByRow agg(&model,
                                 Qt::UserRole,
                                 Qt::DisplayRole,
                                 ProxyOrm::First,
                                 Qt::UserRole + 1);
    QCOMPARE(agg.data(0, Qt::UserRole + 1).toInt(), 10);
    QCOMPARE(agg.data(2, Qt::UserRole + 1).toInt(), 30);
}

void TestAggregateByRow::testLast()
{
    StandardListModel model(3);
    model.setData(model.index(0, 0), "A", Qt::UserRole);
    model.setData(model.index(0, 0), 10, Qt::DisplayRole);
    model.setData(model.index(1, 0), "A", Qt::UserRole);
    model.setData(model.index(1, 0), 20, Qt::DisplayRole);
    model.setData(model.index(2, 0), "B", Qt::UserRole);
    model.setData(model.index(2, 0), 30, Qt::DisplayRole);

    ProxyOrm::AggregateByRow agg(&model,
                                 Qt::UserRole,
                                 Qt::DisplayRole,
                                 ProxyOrm::Last,
                                 Qt::UserRole + 1);
    QCOMPARE(agg.data(0, Qt::UserRole + 1).toInt(), 20);
    QCOMPARE(agg.data(2, Qt::UserRole + 1).toInt(), 30);
}

void TestAggregateByRow::testCaching()
{
    StandardListModel model(2);
    model.setData(model.index(0, 0), "A", Qt::UserRole);
    model.setData(model.index(0, 0), 10, Qt::DisplayRole);
    model.setData(model.index(1, 0), "A", Qt::UserRole);
    model.setData(model.index(1, 0), 20, Qt::DisplayRole);

    ProxyOrm::AggregateByRow agg(&model,
                                 Qt::UserRole,
                                 Qt::DisplayRole,
                                 ProxyOrm::Sum,
                                 Qt::UserRole + 1);
    QCOMPARE(agg.data(0, Qt::UserRole + 1).toInt(), 30); // Первый вызов
    QCOMPARE(agg.data(0, Qt::UserRole + 1).toInt(), 30); // Из кэша

    // После изменения модели (требуется invalidate)
    model.setData(model.index(1, 0), 30, Qt::DisplayRole);
    QCOMPARE(agg.data(0, Qt::UserRole + 1).toInt(), 40);
}

QTEST_APPLESS_MAIN(TestAggregateByRow)

#include "tst_testaggregatebyrow.moc"
