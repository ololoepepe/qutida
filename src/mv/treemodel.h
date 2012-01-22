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

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "src/mv/treeitem.h"

#include <QAbstractItemModel>
#include <QObject>
#include <QVariant>
#include <QList>
#include <QModelIndex>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(const QList<QVariant> &headerData, QObject *parent = 0);
    virtual ~TreeModel();

    virtual QModelIndex index( int row, int column,
                               const QModelIndex &parent =
                               QModelIndex() ) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const;

    virtual QVariant data(const QModelIndex &index, int role) const;
    QVariant pureData(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

protected:
    TreeItem *rootItem;

};

#endif // TREEMODEL_H
