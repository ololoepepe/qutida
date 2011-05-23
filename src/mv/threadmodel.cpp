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
}

//

QVariant ThreadModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>( index.internalPointer() );

    if ( ImageboardThread::InfoStateExtended == index.column() )
    {
        return Tr::IT::threadExtendedState(
                    static_cast<ImageboardThread::ExtendedState>(
                        item->data( index.column() ).toInt() ) );
    }
    else if ( ImageboardThread::InfoAdded == index.column() )
    {
        return item->data( index.column() ).toDateTime().toString(
                    "dd.MM.yyyy hh:mm:ss");
    }
    else
    {
        return item->data( index.column() );
    }
}

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

void ThreadModel::addItem(ImageboardThread *thread)
{
    if (!thread)
        return;

    beginInsertRows( QModelIndex(), rootItem->childCount(),
                     rootItem->childCount() );
    ThreadModelItem *item = new ThreadModelItem(thread, rootItem);
    connect( item, SIGNAL( dataChanged(int, int) ),
             this, SLOT( itemDataChanged(int, int) ) );
    rootItem->appendChild(item);
    endInsertRows();
}

bool ThreadModel::removeItem(const ImageboardThread *thread)
{
    if (!thread)
        return false;

    for (int i = 0; i < rootItem->childCount(); ++i)
    {
        ThreadModelItem *item =
                dynamic_cast<ThreadModelItem*>( rootItem->child(i) );

        if (item)
        {
            if ( item->relatedThread()->url() == thread->url() )
            {
                beginRemoveRows(QModelIndex(), i, i);
                rootItem->removeChild(i);
                endRemoveRows();
                return true;
            }
        }
    }

    return false;
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

ImageboardThread *ThreadModel::threadForRow(int row)
{
    if (row >= rootItem->childCount() || row < 0)
        return 0;

    ThreadModelItem *item =
            dynamic_cast<ThreadModelItem*>( rootItem->child(row) );

    if (!item)
        return 0;
    else
        return item->relatedThread();
}

//

void ThreadModel::itemDataChanged(int row, int column)
{
    if (row >= rootItem->childCount() || row < 0)
        return;

    if (column >= rootItem->columnCount()  || column < 0)
        return;

    QModelIndex ind( index(row, column) );
    emit dataChanged(ind, ind);
}
