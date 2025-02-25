#include <QtTest>

// add necessary includes here
#include "../src/standardlistmodel.h"
#include "fromsource.h"

class TestFromSource : public QObject
{
    Q_OBJECT

public:
    TestFromSource();
    ~TestFromSource();

private slots:
    void testDataRetrievalAndCaching();
    void testMapRoles();
    void testDataChanged();
    void testRowsInserted();
    void testEmptyModel();
    void testInvalidRow();
    void testInvalidRole();
};

TestFromSource::TestFromSource() {}

TestFromSource::~TestFromSource() {}

void TestFromSource::testDataRetrievalAndCaching()
{
    StandardListModel model(3);
    model.setData(model.index(0, 0), "Row 0", Qt::DisplayRole);
    model.setData(model.index(1, 0), "Row 1", Qt::DisplayRole);
    model.setData(model.index(2, 0), "Row 2", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::FromSource source(&model, mapRoles);

    // Первый вызов data -> должен вызвать templateData
    QCOMPARE(source.data(0, Qt::DisplayRole).toString(), "Row 0");

    // Второй вызов -> должен вернуть из кэша
    QCOMPARE(source.data(0, Qt::DisplayRole).toString(), "Row 0");

    // Проверяем, что данные для другой строки тоже корректны
    QCOMPARE(source.data(1, Qt::DisplayRole).toString(), "Row 1");
}

void TestFromSource::testMapRoles()
{
    StandardListModel model(1);
    model.setData(model.index(0, 0), "Display", Qt::DisplayRole);
    model.setData(model.index(0, 0), "User", Qt::UserRole + 1);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole},
                               {Qt::UserRole + 1, Qt::UserRole + 1}};
    ProxyOrm::FromSource source(&model, mapRoles);

    // Проверяем список доступных ролей
    QCOMPARE(source.roles(), QList<int>({Qt::DisplayRole, Qt::UserRole + 1}));

    // Проверяем данные для разных ролей
    QCOMPARE(source.data(0, Qt::DisplayRole).toString(), "Display");
    QCOMPARE(source.data(0, Qt::UserRole + 1).toString(), "User");
}

void TestFromSource::testDataChanged()
{
    StandardListModel model(1);
    model.setData(model.index(0, 0), "Old", Qt::DisplayRole);

    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::FromSource source(&model, mapRoles);

    QSignalSpy spy(&source, &ProxyOrm::ISource::changed);

    // Меняем данные в модели
    model.setData(model.index(0, 0), "New", Qt::DisplayRole);

    // Проверяем, что сигнал changed был излучен
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().toList().first(), Qt::DisplayRole);

    // Проверяем, что данные обновились
    QCOMPARE(source.data(0, Qt::DisplayRole).toString(), "New");
}

void TestFromSource::testRowsInserted()
{
    StandardListModel model(0);
    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::FromSource source(&model, mapRoles);

    QSignalSpy spy(&source, &ProxyOrm::ISource::changed);

    // Вставляем строку
    model.insertRow(0, new QStandardItem{});
    model.setData(model.index(0, 0), "Inserted", Qt::DisplayRole);

    // Проверяем, что сигнал changed был излучен
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.first().first().toList().first(), Qt::DisplayRole);

    // Проверяем данные
    QCOMPARE(source.data(0, Qt::DisplayRole).toString(), "Inserted");
}

void TestFromSource::testEmptyModel()
{
    StandardListModel model(0);
    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::FromSource source(&model, mapRoles);
    QCOMPARE(source.data(0, Qt::DisplayRole), QVariant());
}

void TestFromSource::testInvalidRow()
{
    StandardListModel model(1);
    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::FromSource source(&model, mapRoles);
    QCOMPARE(source.data(-1, Qt::DisplayRole), QVariant());
    QCOMPARE(source.data(1, Qt::DisplayRole), QVariant());
}

void TestFromSource::testInvalidRole()
{
    StandardListModel model(1);
    model.setData(model.index(0, 0), "Data", Qt::DisplayRole);
    QMap<int, int> mapRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
    ProxyOrm::FromSource source(&model, mapRoles);
    QCOMPARE(source.data(0, Qt::UserRole), QVariant());
}

QTEST_APPLESS_MAIN(TestFromSource)

#include "tst_testfromsource.moc"
