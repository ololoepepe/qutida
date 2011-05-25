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

#include "src/mv/categorymodel.h"
#include "src/mv/treemodel.h"
#include "src/tr.h"
#include "src/core/imageboardthread.h"
#include "src/core/threadinfo.h"

#include <QObject>
#include <QList>
#include <QVariant>
#include <QModelIndex>

CategoryModel::CategoryModel(QObject *parent) :
    TreeModel(QList<QVariant>(), parent)
{
    beginInsertRows(QModelIndex(), Tr::CM::CategState, Tr::CM::CategBoard);
    QList<QVariant> data;
    data << Tr::CM::CategState;
    TreeItem *stateItem = new TreeItem(data, rootItem);
    rootItem->appendChild(stateItem);
    data.replace(0, Tr::CM::CategHost);
    rootItem->appendChild( new TreeItem(data, rootItem) );
    data.replace(0, Tr::CM::CategBoard);
    rootItem->appendChild( new TreeItem(data, rootItem) );
    endInsertRows();
    beginInsertRows(index(Tr::CM::CategState, 0),
                    Tr::CM::SubAll, Tr::CM::SubError);
    data.replace(0, Tr::CM::SubAll);
    stateItem->appendChild( new TreeItem(data, stateItem) );
    data.replace(0, Tr::CM::SubActive);
    stateItem->appendChild( new TreeItem(data, stateItem) );
    data.replace(0, Tr::CM::SubWaiting);
    stateItem->appendChild( new TreeItem(data, stateItem) );
    data.replace(0, Tr::CM::SubReady);
    stateItem->appendChild( new TreeItem(data, stateItem) );
    data.replace(0, Tr::CM::SubError);
    stateItem->appendChild( new TreeItem(data, stateItem) );
    endInsertRows();
    countAll = 0;
    countActive = 0;
    countWaiting = 0;
    countReady = 0;
    countError = 0;
}

//

int CategoryModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant CategoryModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    if ( 0 != index.column() )
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>( index.internalPointer() );
    TreeItem *parent = item->parent();

    if (rootItem == parent)
    {
        return Tr::CM::category(
                    static_cast<Tr::CM::Category>( item->data(0).toInt() ) );
    }
    else if (parent)
    {
        if ( Tr::CM::CategState ==
                static_cast<Tr::CM::Category>( parent->data(0).toInt() ) )
        {
            Tr::CM::SubCategory categ =
                    static_cast<Tr::CM::SubCategory>( item->data(0).toInt() );
            int count;

            switch (categ)
            {
            case Tr::CM::SubAll:
                count = countAll;
                break;
            case Tr::CM::SubActive:
                count = countActive;
                break;
            case Tr::CM::SubWaiting:
                count = countWaiting;
                break;
            case Tr::CM::SubReady:
                count = countReady;
                break;
            case Tr::CM::SubError:
                count = countError;
                break;
            default:
                count = 0;
                break;
            }

            return Tr::CM::subCategory(categ, count);
        }
        else
        {
            Tr::CM::Category categ =
                    static_cast<Tr::CM::Category>( parent->data(0).toInt() );
            QString str = item->data(0).toString();
            int count = 0;

            if (Tr::CM::CategHost == categ)
                count = hostList.count(str);
            else if (Tr::CM::CategBoard == categ)
                count = boardList.count(str);

            return str + " (" + QString::number(count) + ")";
        }
    }
    else
    {
        return QVariant();
    }
}

QVariant CategoryModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    return QVariant();
}

//

void CategoryModel::tryAddCategory(ImageboardThread *thread)
{
    if (!thread)
        return;

    connect( thread,
             SIGNAL( infoChanged(ThreadInfo::Enum, QVariant) ),
             this,
             SLOT( threadInfoChanged(ThreadInfo::Enum, QVariant) ) );

    ++countAll;
    QModelIndex stateIndex = index(Tr::CM::CategState, 0);
    QModelIndex subIndex = index(Tr::CM::SubAll, 0, stateIndex);
    emit dataChanged(subIndex, subIndex);
    checkState(thread->extendedState(), true);
    QString host = Common::getHost(thread->parameters().url);
    QString board = Common::getBoard(thread->parameters().url);
    hostList << host;
    boardList << board;

    if (hostList.count(host) == 1)
    {
        TreeItem *parent =  rootItem->child(Tr::CM::CategHost);
        beginInsertRows( index(Tr::CM::CategHost, 0), parent->childCount(),
                         parent->childCount() );
        QList<QVariant> data;
        data << host;
        parent->appendChild( new TreeItem(data, parent) );
        endInsertRows();
    }

    if (boardList.count(board) == 1)
    {
        TreeItem *parent =  rootItem->child(Tr::CM::CategBoard);
        beginInsertRows( index(Tr::CM::CategBoard, 0), parent->childCount(),
                         parent->childCount() );
        QList<QVariant> data;
        data << board;
        parent->appendChild( new TreeItem(data, parent) );
        endInsertRows();
    }
}

void CategoryModel::tryRemoveCategory(ImageboardThread *thread)
{
    if (!thread)
        return;

    disconnect( thread,
                SIGNAL( infoChanged(ThreadInfo::Enum, QVariant) ),
                this,
                SLOT( threadInfoChanged(ThreadInfo::Enum, QVariant) ) );

    --countAll;
    QModelIndex stateIndex = index(Tr::CM::CategState, 0);
    QModelIndex subIndex = index(Tr::CM::SubAll, 0, stateIndex);
    emit dataChanged(subIndex, subIndex);
    checkState(thread->extendedState(), false);
    QString host = Common::getHost(thread->parameters().url);
    QString board = Common::getBoard(thread->parameters().url);
    hostList.removeOne(host);
    boardList.removeOne(board);

    if ( hostList.contains(host) )
    {
        TreeItem *parent =  rootItem->child(Tr::CM::CategHost);

        for (int i = 0; i < parent->childCount(); ++i)
        {
            if (parent->child(i)->data(0).toString() == host)
            {
                QModelIndex modelIndex =
                        index( i, 0, index(Tr::CM::CategHost, 0) );
                emit dataChanged(modelIndex, modelIndex);
                break;
            }
        }
    }
    else
    {
        TreeItem *parent =  rootItem->child(Tr::CM::CategHost);

        for (int i = 0; i < parent->childCount(); ++i)
        {
            if (parent->child(i)->data(0).toString() == host)
            {
                beginRemoveRows(index(Tr::CM::CategHost, 0), i, i);
                parent->removeChild(i);
                endRemoveRows();
                break;
            }
        }
    }

    if ( boardList.contains(board) )
    {
        TreeItem *parent =  rootItem->child(Tr::CM::CategBoard);

        for (int i = 0; i < parent->childCount(); ++i)
        {
            if (parent->child(i)->data(0).toString() == board)
            {
                QModelIndex modelIndex =
                        index( i, 0, index(Tr::CM::CategBoard, 0) );
                emit dataChanged(modelIndex, modelIndex);
                break;
            }
        }
    }
    else
    {
        TreeItem *parent =  rootItem->child(Tr::CM::CategBoard);

        for (int i = 0; i < parent->childCount(); ++i)
        {
            if (parent->child(i)->data(0).toString() == board)
            {
                beginRemoveRows(index(Tr::CM::CategBoard, 0), i, i);
                parent->removeChild(i);
                endRemoveRows();
                break;
            }
        }
    }
}

void CategoryModel::checkState(ImageboardThread::ExtendedState state, bool add)
{
    QModelIndex stateIndex = index(Tr::CM::CategState, 0);

    if (ImageboardThread::ExtWaiting == state)
    {
        countWaiting = add ? (countWaiting + 1) : (countWaiting - 1);
        QModelIndex subIndex = index(Tr::CM::SubWaiting,
                                     Tr::CM::SubWaiting, stateIndex);
        emit dataChanged(subIndex, subIndex);
    }
    else if (ImageboardThread::ExtActiveDownloading == state ||
             ImageboardThread::ExtActiveStarted == state)
    {
        countActive = add ? (countActive + 1) : (countActive - 1);
        QModelIndex subIndex = index(Tr::CM::SubActive,
                                     Tr::CM::SubActive, stateIndex);
        emit dataChanged(subIndex, subIndex);
    }
    else if (ImageboardThread::ExtReadyFailed == state)
    {
        countError = add ? (countError + 1) : (countError - 1);
        QModelIndex subIndex = index(Tr::CM::SubError,
                                     Tr::CM::SubError, stateIndex);
        emit dataChanged(subIndex, subIndex);
    }
    else
    {
        countReady = add ? (countReady + 1) : (countReady - 1);
        QModelIndex subIndex = index(Tr::CM::SubReady,
                                     Tr::CM::SubReady, stateIndex);
        emit dataChanged(subIndex, subIndex);
    }
}

void CategoryModel::retranslate()
{
    QModelIndex topLeft( index( 0, 0, QModelIndex() ) );
    QModelIndex bottomRight(
                index( rootItem->child(
                          rootItem->childCount() - 1)->childCount() - 1,
                       rootItem->child(
                          rootItem->childCount() - 1)->childCount() - 1,
                       QModelIndex() ) );
    emit dataChanged(topLeft, bottomRight);
}

Tr::CM::Category CategoryModel::categoryForIndex(
    const QModelIndex &index) const
{
    return static_cast<Tr::CM::Category>( index.row() );
}

//

void CategoryModel::threadInfoChanged(ThreadInfo::Enum key,
                                      const QVariant &data)
{
    switch (key)
    {
    case ThreadInfo::ExtendedState:
    {
        ImageboardThread::ExtendedState state =
                static_cast<ImageboardThread::ExtendedState>( data.toInt() );
        checkState(state, true);
        break;
    }
    case ThreadInfo::ExtendedStatePrev:
    {
        ImageboardThread::ExtendedState state =
                static_cast<ImageboardThread::ExtendedState>( data.toInt() );
        checkState(state, false);
    }
    default:
        return;
    }
}
