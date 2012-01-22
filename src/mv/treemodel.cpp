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

#include "src/mv/treemodel.h"
#include "src/mv/treeitem.h"

#include <QObject>
#include <QModelIndex>
#include <QVariant>
#include <QList>

TreeModel::TreeModel(const QList <QVariant> &headerData, QObject *parent) :
    QAbstractItemModel(parent)
{
    rootItem = new TreeItem(headerData);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

//

QModelIndex TreeModel::index(int row, int column,
                             const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) )
    {
        return QModelIndex();
    }

    TreeItem *parentItem;

    if ( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>( parent.internalPointer() );

    TreeItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if ( !index.isValid() )
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if ( !index.isValid() )
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>( index.internalPointer() );
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();
    else
        return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;

    if (parent.column() > 0)
        return 0;

    if ( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>( parent.internalPointer() );

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
        return static_cast<TreeItem*>(
                    parent.internalPointer() )->columnCount();
    else
        return rootItem->columnCount();
}

//

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>( index.internalPointer() );
    return item->data( index.column() );
}

QVariant TreeModel::pureData(const QModelIndex &index) const
{
    return TreeModel::data(index, Qt::DisplayRole);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (Qt::Horizontal == orientation && Qt::DisplayRole == role)
        return rootItem->data(section);
    else
        return QVariant();
}
