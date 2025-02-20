#include <QtTest>

// add necessary includes here
#include "where.h"

class TestWhere : public QObject
{
    Q_OBJECT

public:
    TestWhere();
    ~TestWhere();

private slots:
    void testEquals();
    void testNotEquals();
    void testLessThen();
    void testLike();
    void testILike();
    void testIsNull();
};

TestWhere::TestWhere() {}

TestWhere::~TestWhere() {}

void TestWhere::testEquals()
{
    ProxyOrm::Where where(ProxyOrm::Where::Equals, 10);
    QVERIFY(where.isAccepted(10));
    QVERIFY(!where.isAccepted(20));
}

void TestWhere::testNotEquals()
{
    ProxyOrm::Where where(ProxyOrm::Where::NotEquals, 10);
    QVERIFY(!where.isAccepted(10));
    QVERIFY(where.isAccepted(20));
}

void TestWhere::testLessThen()
{
    ProxyOrm::Where where(ProxyOrm::Where::LessThen, 10);
    QVERIFY(where.isAccepted(5));
    QVERIFY(!where.isAccepted(10));
    QVERIFY(!where.isAccepted(15));
}

void TestWhere::testLike()
{
    ProxyOrm::Where where(ProxyOrm::Where::Like, "hello");
    QVERIFY(where.isAccepted("hello world"));
    QVERIFY(!where.isAccepted("world world"));
}

void TestWhere::testILike()
{
    ProxyOrm::Where where(ProxyOrm::Where::ILike, "hello");
    QVERIFY(!where.isAccepted("hello world"));
    QVERIFY(where.isAccepted("world world"));
}

void TestWhere::testIsNull()
{
    ProxyOrm::Where where(ProxyOrm::Where::IsNull, QVariant());
    QVERIFY(where.isAccepted(QVariant()));
    QVERIFY(!where.isAccepted(10));
}

QTEST_APPLESS_MAIN(TestWhere)

#include "tst_testwhere.moc"
