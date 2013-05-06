#ifndef TRADEVIEWMODEL_H
#define TRADEVIEWMODEL_H

#include <QAbstractTableModel>
#include "trade.h"

class TradeViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TradeViewModel(QObject *parent = 0);
    QVariant data ( const QModelIndex & index, int  = Qt::DisplayRole ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    void append(Trade* order);

signals:
    
public slots:
private:
    QList<Trade*> m_trades;
};

#endif // TRADEVIEWMODEL_H
