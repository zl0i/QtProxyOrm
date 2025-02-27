#include <QtTest>

// add necessary includes here
#include "../src/standardlistmodel.h"
#include "joinindexed.h"

class TestJoinIndexed : public QObject
{
    Q_OBJECT

public:
    TestJoinIndexed();
    ~TestJoinIndexed();

private slots:
    void testBasicJoin();
    void testNoMatch();
    void testCaching();
    void testJoinModelChange();
    void testSourceModelChange();
};

TestJoinIndexed::TestJoinIndexed() {}

TestJoinIndexed::~TestJoinIndexed() {}

void TestJoinIndexed::testBasicJoin()
{
    StandardListModel sourceModel(3);
    sourceModel.setData(sourceModel.index(0, 0), 1, Qt::UserRole);
    sourceModel.setData(sourceModel.index(1, 0), 2, Qt::UserRole);
    sourceModel.setData(sourceModel.index(2, 0), 3, Qt::UserRole);

    StandardListModel joinModel(3);
    joinModel.setData(joinModel.index(0, 0), 1, Qt::UserRole);
    joinModel.setData(joinModel.index(0, 0), "One", Qt::DisplayRole);
    joinModel.setData(joinModel.index(1, 0), 2, Qt::UserRole);
    joinModel.setData(joinModel.index(1, 0), "Two", Qt::DisplayRole);
    joinModel.setData(joinModel.index(2, 0), 3, Qt::UserRole);
    joinModel.setData(joinModel.index(2, 0), "Three", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::JoinIndexed join(&sourceModel, Qt::UserRole, &joinModel, mapRoles);

    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "One");
    QCOMPARE(join.data(1, Qt::DisplayRole).toString(), "Two");
    QCOMPARE(join.data(2, Qt::DisplayRole).toString(), "Three");
}

void TestJoinIndexed::testNoMatch()
{
    StandardListModel sourceModel(1);
    sourceModel.setData(sourceModel.index(0, 0), 1, Qt::UserRole);

    StandardListModel joinModel(2);
    joinModel.setData(joinModel.index(0, 0), 2, Qt::UserRole);
    joinModel.setData(joinModel.index(0, 0), "Two", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::JoinIndexed join(&sourceModel, Qt::UserRole, &joinModel, mapRoles);

    QCOMPARE(join.data(0, Qt::DisplayRole), QVariant());
}

void TestJoinIndexed::testCaching()
{
    StandardListModel sourceModel(1);
    sourceModel.setData(sourceModel.index(0, 0), 1, Qt::UserRole);

    StandardListModel joinModel(1);
    joinModel.setData(joinModel.index(0, 0), 1, Qt::UserRole);
    joinModel.setData(joinModel.index(0, 0), "Cached", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::JoinIndexed join(&sourceModel, Qt::UserRole, &joinModel, mapRoles);

    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "Cached"); // Первый вызов
    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "Cached"); // Из кэша

    joinModel.setData(joinModel.index(0, 0), "Updated", Qt::DisplayRole); // Изменение данных
    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "Updated");        // После сброса кэша
}

void TestJoinIndexed::testJoinModelChange()
{
    StandardListModel sourceModel(1);
    sourceModel.setData(sourceModel.index(0, 0), 1, Qt::UserRole);

    StandardListModel joinModel(1);
    joinModel.setData(joinModel.index(0, 0), 1, Qt::UserRole);
    joinModel.setData(joinModel.index(0, 0), "Original", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::JoinIndexed join(&sourceModel, Qt::UserRole, &joinModel, mapRoles);

    QSignalSpy spy(&join, &ProxyOrm::ISource::changed);

    joinModel.setData(joinModel.index(0, 0), "Changed", Qt::DisplayRole);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "Changed");
}

void TestJoinIndexed::testSourceModelChange()
{
    StandardListModel sourceModel(1);
    sourceModel.setData(sourceModel.index(0, 0), 1, Qt::UserRole);

    StandardListModel joinModel(2);
    joinModel.setData(joinModel.index(0, 0), 1, Qt::UserRole);
    joinModel.setData(joinModel.index(0, 0), "One", Qt::DisplayRole);
    joinModel.setData(joinModel.index(1, 0), 2, Qt::UserRole);
    joinModel.setData(joinModel.index(1, 0), "Two", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::JoinIndexed join(&sourceModel, Qt::UserRole, &joinModel, mapRoles);

    QSignalSpy spy(&join, &ProxyOrm::ISource::changed);

    sourceModel.setData(sourceModel.index(0, 0), 2, Qt::UserRole);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "Two");
}

QTEST_APPLESS_MAIN(TestJoinIndexed)

#include "tst_testjoinindexed.moc"
