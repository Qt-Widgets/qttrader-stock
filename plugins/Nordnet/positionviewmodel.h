#ifndef POSITIONVIEWMODEL_H
#define POSITIONVIEWMODEL_H

#include <QAbstractTableModel>
#include "position.h"

class PositionViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PositionViewModel(QObject *parent = 0);
    QVariant data ( const QModelIndex & index, int  = Qt::DisplayRole ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    void append(Position* order);
signals:
    
public slots:
private:
    QList<Position*> m_positions;
};

#endif // POSITIONVIEWMODEL_H
