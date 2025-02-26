#include <QtTest>

// add necessary includes here
#include "../src/standardlistmodel.h"
#include "case.h"

class TestCase : public QObject
{
    Q_OBJECT

public:
    TestCase();
    ~TestCase();

private slots:
    void testMatchingCondition();
    void testNoMatch();
    void testDefaultValue();
    void testCaching();
    void testModelChange();
};

TestCase::TestCase() {}

TestCase::~TestCase() {}

void TestCase::testMatchingCondition()
{
    StandardListModel model(3);
    model.setData(model.index(0, 0), "A", Qt::UserRole);
    model.setData(model.index(1, 0), "B", Qt::UserRole);
    model.setData(model.index(2, 0), "C", Qt::UserRole);

    QList<QPair<QVariant, QVariant>> conditions = {{"A", "Alpha"}, {"B", "Beta"}};
    ProxyOrm::Case caseObj(&model, Qt::UserRole, conditions, Qt::UserRole + 1, "Unknown");

    QCOMPARE(caseObj.data(0, Qt::UserRole + 1).toString(), "Alpha");
    QCOMPARE(caseObj.data(1, Qt::UserRole + 1).toString(), "Beta");
    QCOMPARE(caseObj.data(2, Qt::UserRole + 1).toString(), "Unknown");
}

void TestCase::testNoMatch()
{
    StandardListModel model(1);
    model.setData(model.index(0, 0), "X", Qt::UserRole);

    QList<QPair<QVariant, QVariant>> conditions = {{"A", "Alpha"}};
    ProxyOrm::Case caseObj(&model, Qt::UserRole, conditions, Qt::UserRole + 1, "Unknown");

    QCOMPARE(caseObj.data(0, Qt::UserRole + 1).toString(), "Unknown");
}

void TestCase::testDefaultValue()
{
    StandardListModel model(1);
    model.setData(model.index(0, 0), "A", Qt::UserRole);

    QList<QPair<QVariant, QVariant>> conditions;
    ProxyOrm::Case caseObj(&model, Qt::UserRole, conditions, Qt::UserRole + 1, "Default");

    QCOMPARE(caseObj.data(0, Qt::UserRole + 1).toString(), "Default");
}

void TestCase::testCaching()
{
    StandardListModel model(1);
    model.setData(model.index(0, 0), "A", Qt::UserRole);

    QList<QPair<QVariant, QVariant>> conditions = {{"A", "Alpha"}};
    ProxyOrm::Case caseObj(&model, Qt::UserRole, conditions, Qt::UserRole + 1);

    QCOMPARE(caseObj.data(0, Qt::UserRole + 1).toString(), "Alpha"); // Первый вызов
    QCOMPARE(caseObj.data(0, Qt::UserRole + 1).toString(), "Alpha"); // Из кэша

    model.setData(model.index(0, 0), "B", Qt::UserRole);
    QCOMPARE(caseObj.data(0, Qt::UserRole + 1), QVariant()); // После сброса кэша
}

void TestCase::testModelChange()
{
    StandardListModel model(1);
    model.setData(model.index(0, 0), "A", Qt::UserRole);

    QList<QPair<QVariant, QVariant>> conditions = {{"A", "Alpha"}, {"B", "Beta"}};
    ProxyOrm::Case caseObj(&model, Qt::UserRole, conditions, Qt::UserRole + 1);

    QSignalSpy spy(&caseObj, &ProxyOrm::ISource::changed);

    model.setData(model.index(0, 0), "B", Qt::UserRole);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(caseObj.data(0, Qt::UserRole + 1).toString(), "Beta");
}

QTEST_APPLESS_MAIN(TestCase)

#include "tst_testcase.moc"
