#include "positionviewmodel.h"

PositionViewModel::PositionViewModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

QVariant PositionViewModel::data ( const QModelIndex & index, int role) const
{
    if(m_positions.isEmpty() || (role != Qt::DisplayRole)){
        return QVariant();
    }

    Position* pOrder = m_positions.at(index.row());
    switch(index.column()) {
    case 0:
        return QVariant(pOrder->getAcqPrice());
    case 1:
        return QVariant(pOrder->getAcqPriceAcc());
    case 2:
        return QVariant(pOrder->getPawnPercent());
    case 3:
        return QVariant(pOrder->getQuantity());
    case 4:
        return QVariant(pOrder->getMarketValue());
    case 5:
        return QVariant(pOrder->getMarketValueAcc());
    case 6:
        return QVariant(pOrder->getMainMarketId());
    case 7:
        return QVariant(pOrder->getIdentifier());
    case 8:
        return QVariant(pOrder->getType());
    case 9:
        return QVariant(pOrder->getCurrency());
    case 10:
        return QVariant(pOrder->getMainMarketPrice());
    default:
    return QVariant();
    }
}

int PositionViewModel::columnCount ( const QModelIndex & /*parent*/ ) const
{
    return 11;
}

int PositionViewModel::rowCount ( const QModelIndex & /*parent*/ ) const
{
    return m_positions.size();
}

QVariant PositionViewModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
     if (orientation == Qt::Horizontal) {
        switch(section) {
            case 0:
                return QVariant(QString("Acquire price"));
            case 1:
                return (QString("Acquire price SEK"));
            case 2:
                return (QString("Pawn %"));
            case 3:
                return (QString("Quantity"));
            case 4:
                return (QString("Market value"));
            case 5:
                return (QString("Market value SEK"));
            case 6:
                return (QString("Nordnet market id"));
            case 7:
                return (QString("identifier"));
            case 8:
                return (QString("type"));
            case 9:
                return (QString("Currency"));
            case 10:
                return (QString("last price"));
        }
    }
     return QVariant();
}

void PositionViewModel::append(Position* pPosition)
{
    m_positions.append(pPosition);
}
