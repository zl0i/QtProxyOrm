#ifndef PROXYORMMODEL_H
#define PROXYORMMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "isource.h"
#include "orwhere.h"
#include "sort.h"
#include "where.h"

//    model     signal    filter_role     no_filter_role
//--------------------------------------------------------
//   source      reset      invalidate      soft_reset
//   source    row_insert   invalidate?     row_insert
//   source    row_remove   invalidate?     row_remove
//   source    row_move     invalidate      row_move
//   source    data_change  invalidate      data_change
//    join       reset      invalidate      data_change
//    join     row_insert   invalidate      data_change
//    join     row_remove   invalidate      data_change
//    join     row_move     invalidate      data_change
//    join     data_change  invalidate      data_change
// aggregate       *        invalidate?     data_change

namespace ProxyOrm {

class ProxyOrmModel : public QAbstractListModel
{
    Q_OBJECT
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;

protected:
    explicit ProxyOrmModel(QAbstractListModel *sourceModel,
                           QMap<int, int> roles,
                           QObject *parent = nullptr);

    void addSource(ISource *source);

    void andWhere(int whereRole, Where where);
    void andWhere(int whereRole, OrWhere where);
    void clearWhere(int role);
    void clearWhere();
    void sort(int role, Qt::SortOrder type) override;
    void groupBy(int role);

    void enabled(bool enabled);

private:
    const QAbstractListModel *sourceModel = nullptr;

    QMap<int, ISource *> sourceMap;

    QMap<int, OrWhere> whereMap;

    int sortRole = -1;
    Sort sortFunc;
    bool isFiltered = false;
    QList<QModelIndex> sortedFilteredIndex;

    int groupByRole = -1;

    int partition(int low, int high);
    void quickSortRecursive(int low, int high);

    bool isSortGroupFilterRole(int role);
    bool isSortGroupFilterRole(QList<int> role);
    bool isWhereRole();

    int oldSize = 0;
    void beginSoftResetModel() { oldSize = sortedFilteredIndex.count(); }
    void endSoftResetModel()
    {
        int diff = sortedFilteredIndex.count() - oldSize;
        if (diff > 0) {
            beginInsertRows(QModelIndex(), 0, diff - 1);
            endInsertRows();
        } else if (diff < 0) {
            beginRemoveRows(QModelIndex(), 0, -diff - 1);
            endRemoveRows();
        }

        if (sortedFilteredIndex.count() != 0) {
            QModelIndex topLeft = index(0, 0);
            QModelIndex bottomRight = index(sortedFilteredIndex.count() - 1, 0);
            emit dataChanged(topLeft, bottomRight);
        }
    }

    bool mEnabled{true};
    bool needToInvalidate{false};

private slots:
    void sourceChanged(QList<int> role);

public slots:
    void invalidate();
};

} // namespace ProxyOrm

#endif // PROXYORMMODEL_H
