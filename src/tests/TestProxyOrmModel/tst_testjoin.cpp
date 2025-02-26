#include <QtTest>

// add necessary includes here
#include "../src/standardlistmodel.h"
#include "join.h"

class TestJoin : public QObject
{
    Q_OBJECT

public:
    TestJoin();
    ~TestJoin();

private slots:
    void testBasicJoin();         // Проверка базового объединения
    void testNoMatch();           // Проверка отсутствия совпадений
    void testCaching();           // Проверка кэширования
    void testJoinModelChange();   // Проверка реакции на изменения в joinModel
    void testSourceModelChange(); // Проверка реакции на изменения в sourceModel
};

TestJoin::TestJoin() {}

TestJoin::~TestJoin() {}

void TestJoin::testBasicJoin()
{
    StandardListModel sourceModel(3);
    sourceModel.setData(sourceModel.index(0, 0), 1, Qt::UserRole); // Ключ
    sourceModel.setData(sourceModel.index(1, 0), 2, Qt::UserRole);
    sourceModel.setData(sourceModel.index(2, 0), 3, Qt::UserRole);

    StandardListModel joinModel(3);
    joinModel.setData(joinModel.index(0, 0), 1, Qt::UserRole); // Ключ
    joinModel.setData(joinModel.index(0, 0), "One", Qt::DisplayRole);
    joinModel.setData(joinModel.index(1, 0), 2, Qt::UserRole);
    joinModel.setData(joinModel.index(1, 0), "Two", Qt::DisplayRole);
    joinModel.setData(joinModel.index(2, 0), 3, Qt::UserRole);
    joinModel.setData(joinModel.index(2, 0), "Three", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::Join join(&sourceModel, Qt::UserRole, &joinModel, Qt::UserRole, mapRoles);

    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "One");
    QCOMPARE(join.data(1, Qt::DisplayRole).toString(), "Two");
    QCOMPARE(join.data(2, Qt::DisplayRole).toString(), "Three");
}

void TestJoin::testNoMatch()
{
    StandardListModel sourceModel(1);
    sourceModel.setData(sourceModel.index(0, 0), 1, Qt::UserRole);

    StandardListModel joinModel(1);
    joinModel.setData(joinModel.index(0, 0), 2, Qt::UserRole); // Нет совпадений
    joinModel.setData(joinModel.index(0, 0), "Two", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::Join join(&sourceModel, Qt::UserRole, &joinModel, Qt::UserRole, mapRoles);

    QCOMPARE(join.data(0, Qt::DisplayRole), QVariant());
}

void TestJoin::testCaching()
{
    StandardListModel sourceModel(1);
    sourceModel.setData(sourceModel.index(0, 0), 1, Qt::UserRole);

    StandardListModel joinModel(1);
    joinModel.setData(joinModel.index(0, 0), 1, Qt::UserRole);
    joinModel.setData(joinModel.index(0, 0), "Cached", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::Join join(&sourceModel, Qt::UserRole, &joinModel, Qt::UserRole, mapRoles);

    // Первый вызов: из templateData
    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "Cached");
    // Второй вызов: из кэша
    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "Cached");

    // Изменяем joinModel и проверяем сброс кэша
    joinModel.setData(joinModel.index(0, 0), "Updated", Qt::DisplayRole);
    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "Updated");
}

void TestJoin::testJoinModelChange()
{
    StandardListModel sourceModel(1);
    sourceModel.setData(sourceModel.index(0, 0), 1, Qt::UserRole);

    StandardListModel joinModel(1);
    joinModel.setData(joinModel.index(0, 0), 1, Qt::UserRole);
    joinModel.setData(joinModel.index(0, 0), "Original", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::Join join(&sourceModel, Qt::UserRole, &joinModel, Qt::UserRole, mapRoles);

    QSignalSpy spy(&join, &ProxyOrm::ISource::changed);

    joinModel.setData(joinModel.index(0, 0), "Changed", Qt::DisplayRole);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "Changed");
}

void TestJoin::testSourceModelChange()
{
    StandardListModel sourceModel(1);
    sourceModel.setData(sourceModel.index(0, 0), 1, Qt::UserRole);

    StandardListModel joinModel(2);
    joinModel.setData(joinModel.index(0, 0), 1, Qt::UserRole);
    joinModel.setData(joinModel.index(0, 0), "One", Qt::DisplayRole);
    joinModel.setData(joinModel.index(1, 0), 2, Qt::UserRole);
    joinModel.setData(joinModel.index(1, 0), "Two", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::Join join(&sourceModel, Qt::UserRole, &joinModel, Qt::UserRole, mapRoles);

    QSignalSpy spy(&join, &ProxyOrm::ISource::changed);

    // Изменяем sourceModel (после добавления подключения сигналов)
    sourceModel.setData(sourceModel.index(0, 0), 2, Qt::UserRole);
    QCOMPARE(spy.count(), 1); // Требуется подключение сигнала sourceModel
    QCOMPARE(join.data(0, Qt::DisplayRole).toString(), "Two");
}

QTEST_APPLESS_MAIN(TestJoin)

#include "tst_testjoin.moc"
