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

#include "src/mv/treeitem.h"

#include <QList>
#include <QVariant>

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent) :
    QObject(parent)
{
    parentItem = parent;
    itemData = data;
}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}

//

int TreeItem::columnCount() const
{
    return itemData.count();
}

TreeItem *TreeItem::parent()
{
    return parentItem;
}

int TreeItem::row() const
{
    if (!parentItem)
        return -1;
    else
        return parentItem->indexOf( const_cast<TreeItem*>(this) );
}

TreeItem *TreeItem::child(int row)
{
    return childItems.at(row);
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::indexOf(TreeItem *child)
{
    return childItems.indexOf(child);
}

bool TreeItem::appendChild(TreeItem *child)
{
    if (!child)
    {
        return false;
    }

    childItems.append(child);
    return true;
}

bool TreeItem::removeChild(int row)
{
    if (row >= childItems.count() || row < 0)
    {
        return false;
    }

    TreeItem *child = childItems.value(row);

    if (child)
        child->deleteLater();

    childItems.removeAt(row);
    return true;
}

void TreeItem::rearrangeChildren(TreeItem *child1, TreeItem *child2)
{
    if ( childItems.isEmpty() )
        return;

    if (!child1 || !child2)
        return;

    if ( !childItems.contains(child1) || !childItems.contains(child2) )
        return;

    int index1 = childItems.indexOf(child1);
    int index2 = childItems.indexOf(child2);
    childItems.replace(index1, child2);
    childItems.replace(index2, child1);
}

//

QVariant TreeItem::data(int column) const
{
    return itemData.at(column);
}

void TreeItem::modifyData(int column, const QVariant &data)
{
    if ( column >= itemData.count() )
        return;

    itemData.replace(column, data);
}
