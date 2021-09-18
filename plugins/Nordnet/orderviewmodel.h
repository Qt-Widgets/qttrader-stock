#ifndef ORDERVIEWMODEL_H
#define ORDERVIEWMODEL_H

#include <QAbstractTableModel>
#include "order.h"
class OrderViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit OrderViewModel(QObject *parent = 0);
    /*
    QModelIndex parent ( const QModelIndex & index ) const;
    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const ;
    */
    QVariant data ( const QModelIndex & index, int  = Qt::DisplayRole ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    void append(Order* order);
signals:
    
public slots:

private:
    QList<Order*> m_Orders;
};

#endif // ORDERVIEWMODEL_H
