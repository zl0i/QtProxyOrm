#ifndef PROXYORMMODEL_H
#define PROXYORMMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QtConcurrent/QtConcurrent>

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
    void enabled(bool enabled);

protected:
    explicit ProxyOrmModel(const QAbstractListModel *sourceModel,
                           QMap<int, int> roles,
                           QObject *parent = nullptr);

    void addSource(const ISource *source);

    void andWhere(int whereRole, Where where);
    void andWhere(int whereRole, OrWhere where);
    void clearWhere(int role);
    void clearWhere();
    void sort(int role, Qt::SortOrder type) override;
    void groupBy(int role);

    void enabledAsync(bool enabled);

private:
    const QAbstractListModel *sourceModel = nullptr;

    QMap<int, const ISource *> sourceMap;

    QMap<int, OrWhere> whereMap;

    int sortRole = -1;
    Sort sortFunc;
    bool isFiltered = false;
    QList<QModelIndex> sortedFilteredIndex;

    int groupByRole = -1;

    int partition(int low, int high);
    void quickSortRecursive(int low, int high);

    bool isSortGroupFilterRole(int role) const;
    bool isSortGroupFilterRole(QList<int> role) const;
    bool isWhereRole() const;

    int oldSize = 0;
    void beginSoftResetModel();
    void endSoftResetModel();

    bool mEnabled{true};
    bool needToInvalidate{false};

    bool isAsync{false};
    bool isStart{false};
    bool isCancel{false};
    QFuture<void> futureInvalidate = QtFuture::makeReadyVoidFuture();

private slots:
    void sourceChanged(QList<int> role);
    void performInvalidate();

public slots:
    void invalidate();
};

} // namespace ProxyOrm

#endif // PROXYORMMODEL_H
