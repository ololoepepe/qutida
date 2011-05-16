/****************************************************************************
** Copyright (C) 2011 Andrew Bogdanov.
**
** This file is part of qutida.
**
** qutida is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** qutida is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with qutida.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#include "src/mv/threadmodel.h"
#include "src/mv/threadmodelitem.h"
#include "src/core/imageboardthread.h"
#include "src/tr.h"

#include <QObject>
#include <QVariant>
#include <QList>
#include <QModelIndex>

ThreadModel::ThreadModel(const QList<QVariant> &headerData, QObject *parent) :
    TreeModel(headerData, parent)
{
    connect( rootItem, SIGNAL( childrenRearranged(int, int) ),
             this, SIGNAL( itemsRearranged(int, int) ) );
}

//

QVariant ThreadModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const
{
    if (Qt::Horizontal != orientation || Qt::DisplayRole != role)
        return QVariant();

    return Tr::IT::threadModelHeader(
                static_cast<ImageboardThread::Info>(
                    rootItem->data(section).toInt() ) );
}

void ThreadModel::sort(int column, Qt::SortOrder order)
{
    if (column >= rootItem->columnCount() || column < 0)
        return;

    int count = rootItem->childCount();

    if (count < 2)
        return;

    emit layoutAboutToBeChanged();
    bool ascending = (Qt::AscendingOrder == order);

    for (int i = 0; i < count - 1; ++i)
    {
        for (int j = i + 1; j < count; ++j)
        {
            TreeItem *item1 = rootItem->child(i);
            TreeItem *item2 = rootItem->child(j);
            int res = TreeItem::compare(*item1, *item2, column);

            if ( (ascending && res > 0) || (!ascending && res < 0) )
            {
                rootItem->rearrangeChildren(item1, item2);
            }
        }
    }

    emit layoutChanged();
}

//

void ThreadModel::addItem(const ImageboardThread *thread)
{
    if (!thread)
        return;

    beginInsertRows( QModelIndex(), rootItem->childCount(),
                     rootItem->childCount() );
    ThreadModelItem *item =
            new ThreadModelItem( dataFromInfoMap(
                                    ImageboardThread::infoFromParameters(
                                        thread->parameters() ) ), rootItem);
    connect( thread, SIGNAL( infoChanged(ImageboardThread::Info, QVariant) ),
             item,
             SLOT( threadInfoChanged(ImageboardThread::Info, QVariant) ) );
    connect( thread, SIGNAL( destroyed() ), item, SLOT( planDeletion() ) );
    connect( item, SIGNAL( requestDeletion(int) ),
             this, SLOT( requestDeletion(int) ) );
    connect( item, SIGNAL( dataChanged(int, int) ),
             this, SLOT( itemDataChanged(int, int) ) );
    rootItem->appendChild(item);
    endInsertRows();
}

void ThreadModel::retranslate()
{
    emit headerDataChanged( Qt::Horizontal, 0, columnCount() );
    QModelIndex topLeft( index( 0, 0, QModelIndex() ) );
    QModelIndex bottomRight ( index( rootItem->childCount() - 1,
                                     rootItem->columnCount() - 1,
                                     QModelIndex() ) );
    emit dataChanged(topLeft, bottomRight);
}

//

void ThreadModel::itemDataChanged(int row, int column)
{
    if ( row >= rootItem->childCount() )
        return;

    if ( column >= rootItem->columnCount() )
        return;

    TreeItem *item = rootItem->child(row);

    if (!item)
    {
        return;
    }

    QModelIndex ind ( index( row, column, QModelIndex() ) );
    emit dataChanged(ind, ind);
}

void ThreadModel::requestDeletion(int row)
{
    if ( row >= rootItem->childCount() )
        return;

    beginRemoveRows(QModelIndex(), row, row);
    rootItem->removeChild(row);
    endRemoveRows();
}
