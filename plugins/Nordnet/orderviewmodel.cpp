#include "orderviewmodel.h"

OrderViewModel::OrderViewModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

/*
QModelIndex OrderViewModel::parent ( const QModelIndex & index ) const
{

}

QModelIndex OrderViewModel::index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const
{

}
*/
QVariant OrderViewModel::data ( const QModelIndex & index, int role) const
{
    if(m_Orders.isEmpty() || (role != Qt::DisplayRole)){
        return QVariant();
    }

    Order* pOrder = m_Orders.at(index.row());
    switch(index.column()) {
    case 0:
        return QVariant(pOrder->getExchangeOrderId());
    case 1:
        return QVariant(pOrder->getOrderId());
    case 2:
        return QVariant(pOrder->getRegdate());
    case 3:
        return QVariant(pOrder->getPriceCondition());
    case 4:
        return QVariant(pOrder->getVolume());
    case 5:
        return QVariant(pOrder->getSide());
    case 6:
        return QVariant(pOrder->getTradedVolume());
    case 7:
        return QVariant(pOrder->getAccountNumber());
    case 8:
        return QVariant(pOrder->getState());
    default:
    return QVariant();
    }
}

int OrderViewModel::columnCount ( const QModelIndex & /*parent*/ ) const
{
    return 9;
}

int OrderViewModel::rowCount ( const QModelIndex & /*parent*/ ) const
{
    return m_Orders.size();
}

QVariant OrderViewModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
     if (orientation == Qt::Horizontal) {
        switch(section) {
            case 0:
                return QVariant(QString("exchangeOrderID"));
            case 1:
                return (QString("OrderId"));
            case 2:
                return (QString("regdate"));
            case 3:
                return (QString("priceCondition"));
            case 4:
                return (QString("volume"));
            case 5:
                return (QString("side"));
            case 6:
                return (QString("tradedVolume"));
            case 7:
                return (QString("account"));
            case 8:
                return (QString("status"));
        }
    }
     return QVariant();
}

void OrderViewModel::append(Order* order)
{
    m_Orders.append(order);
}
