#include "tradeviewmodel.h"

TradeViewModel::TradeViewModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

QVariant TradeViewModel::data ( const QModelIndex & index, int role) const
{
    if(m_trades.isEmpty() || (role != Qt::DisplayRole)){
        return QVariant();
    }

    Trade* pTrade = m_trades.at(index.row());
    switch(index.column()) {
    case 0:
        return QVariant(pTrade->getAccount());
    case 1:
        return QVariant(pTrade->getOrderID());
    case 2:
        return QVariant(pTrade->getMarketId());
    case 3:
        return QVariant(pTrade->getIdentifier());
    case 4:
        return QVariant(pTrade->getVolume());
    case 5:
        return QVariant(pTrade->getCurrency());
    case 6:
        return QVariant(pTrade->getValue());
    case 7:
        return QVariant(pTrade->getSide());
    case 8:
        return QVariant(pTrade->getTrade());
    case 9:
        return QVariant(pTrade->getCounterPart());
    case 10:
        return QVariant(pTrade->getTradeTime());
    default:
    return QVariant();
    }
}

int TradeViewModel::columnCount ( const QModelIndex & /*parent*/ ) const
{
    return 11;
}

int TradeViewModel::rowCount ( const QModelIndex & /*parent*/ ) const
{
    return m_trades.size();
}

QVariant TradeViewModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
     if (orientation == Qt::Horizontal) {
        switch(section) {
            case 0:
                return QString("AccountNumber");
            case 1:
                return (QString("OrderId"));
            case 2:
                return (QString("marketId"));
            case 3:
                return (QString("identifyer"));
            case 4:
                return (QString("volume"));
            case 5:
                return (QString("currency"));
            case 6:
                return (QString("price"));
            case 7:
                return (QString("side"));
            case 8:
                return (QString("tradeId"));
            case 9:
                return (QString("Counterpart"));
            case 10:
                return (QString("tradetime"));
        }
    }
     return QVariant();
}

void TradeViewModel::append(Trade* trade)
{
    m_trades.append(trade);
}
