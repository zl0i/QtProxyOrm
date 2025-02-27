#include <QtTest>
#include "aggregatebyrow.h"
#include "case.h"
#include "join.h"
#include "proxyormmodel.h"
#include "standardlistmodel.h"

class PublicProxyOrmModel : public ProxyOrm::ProxyOrmModel
{
public:
    PublicProxyOrmModel(const QAbstractListModel *sourceModel, QMap<int, int> roles)
        : ProxyOrm::ProxyOrmModel(sourceModel, roles) {};

    void addSource(const ProxyOrm::ISource *source) { ProxyOrm::ProxyOrmModel::addSource(source); };

    void andWhere(int whereRole, ProxyOrm::Where where)
    {
        ProxyOrm::ProxyOrmModel::andWhere(whereRole, where);
    };
    void andWhere(int whereRole, ProxyOrm::OrWhere where)
    {
        ProxyOrm::ProxyOrmModel::andWhere(whereRole, where);
    }
    void clearWhere(int role) { ProxyOrm::ProxyOrmModel::clearWhere(role); };
    void clearWhere() { ProxyOrm::ProxyOrmModel::clearWhere(); };
    void sort(int role, Qt::SortOrder type) override { ProxyOrm::ProxyOrmModel::sort(role, type); };
    void groupBy(int role) { ProxyOrm::ProxyOrmModel::groupBy(role); };
};

class ProxyOrmModelBenchmark : public QObject
{
    Q_OBJECT

private:
    StandardListModel *createTestModel(int rowCount)
    {
        StandardListModel *model = new StandardListModel(rowCount, this);
        for (int i = 0; i < rowCount; ++i) {
            QModelIndex idx = model->index(i, 0);
            model->setData(idx,
                           QString("Category%1").arg(i % 100),
                           Qt::UserRole);                 // 100 уникальных категорий
            model->setData(idx, i * 10, Qt::DisplayRole); // Значения 0, 10, 20, ...
        }
        return model;
    }

    const int ROW_COUNT = 10000; // Количество строк для теста

private slots:
    void initTestCase()
    {
        qDebug() << "Starting ProxyOrmModel benchmark with" << ROW_COUNT << "rows";
    }

    void benchmarkFilter()
    {
        StandardListModel *source = createTestModel(ROW_COUNT);
        QMap<int, int> roles = {{Qt::UserRole, Qt::UserRole}};
        PublicProxyOrmModel model(source, roles);

        QBENCHMARK
        {
            model.andWhere(Qt::UserRole, ProxyOrm::Where(ProxyOrm::Where::Equals, "Category50"));
            model.invalidate();
        }
    }

    void benchmarkSort()
    {
        StandardListModel *source = createTestModel(ROW_COUNT);
        QMap<int, int> roles = {{Qt::UserRole, Qt::UserRole}};
        PublicProxyOrmModel model(source, roles);

        QBENCHMARK
        {
            model.sort(Qt::UserRole, Qt::AscendingOrder);
            model.invalidate();
        }
    }

    void benchmarkGroupBy()
    {
        StandardListModel *source = createTestModel(ROW_COUNT);
        QMap<int, int> roles = {{Qt::UserRole, Qt::UserRole}};
        PublicProxyOrmModel model(source, roles);

        QBENCHMARK
        {
            model.groupBy(Qt::UserRole);
            model.invalidate();
        }
    }

    void benchmarkFromSource()
    {
        StandardListModel *source = createTestModel(ROW_COUNT);
        QMap<int, int> roles = {{Qt::UserRole, Qt::UserRole}, {Qt::DisplayRole, Qt::DisplayRole}};
        PublicProxyOrmModel model(source, roles);

        QBENCHMARK
        {
            for (int i = 0; i < ROW_COUNT; ++i) {
                model.data(model.index(i, 0), Qt::DisplayRole);
            }
        }
    }

    void benchmarkJoin()
    {
        StandardListModel *source = createTestModel(ROW_COUNT);
        StandardListModel *joinSource = createTestModel(ROW_COUNT / 10); // Меньше строк для реализма
        QMap<int, int> joinRoles = {{Qt::DisplayRole, Qt::DisplayRole}};
        ProxyOrm::Join join(source, Qt::UserRole, joinSource, Qt::UserRole, joinRoles);
        QMap<int, int> roles = {{Qt::UserRole, Qt::UserRole}};
        PublicProxyOrmModel model(source, roles);
        model.addSource(&join);

        QBENCHMARK
        {
            for (int i = 0; i < ROW_COUNT; ++i) {
                model.data(model.index(i, 0), Qt::DisplayRole);
            }
        }
    }

    void benchmarkAggregateByRow()
    {
        StandardListModel *source = createTestModel(ROW_COUNT);
        ProxyOrm::AggregateByRow agg(source,
                                     Qt::UserRole,
                                     Qt::DisplayRole,
                                     ProxyOrm::Sum,
                                     Qt::UserRole + 1);
        QMap<int, int> roles = {{Qt::UserRole, Qt::UserRole}};
        PublicProxyOrmModel model(source, roles);
        model.addSource(&agg);

        QBENCHMARK
        {
            for (int i = 0; i < ROW_COUNT; ++i) {
                model.data(model.index(i, 0), Qt::UserRole + 1);
            }
        }
    }

    void benchmarkCase()
    {
        StandardListModel *source = createTestModel(ROW_COUNT);
        QList<QPair<QVariant, QVariant>> conditions;
        for (int i = 0; i < 100; ++i) {
            conditions.append({QString("Category%1").arg(i), QString("Label%1").arg(i)});
        }
        ProxyOrm::Case caseObj(source, Qt::UserRole, conditions, Qt::UserRole + 2, "Unknown");
        QMap<int, int> roles = {{Qt::UserRole, Qt::UserRole}};
        PublicProxyOrmModel model(source, roles);
        model.addSource(&caseObj);

        QBENCHMARK
        {
            for (int i = 0; i < ROW_COUNT; ++i) {
                model.data(model.index(i, 0), Qt::UserRole + 2);
            }
        }
    }
};

QTEST_MAIN(ProxyOrmModelBenchmark)

#include "tst_benchmark.moc"
