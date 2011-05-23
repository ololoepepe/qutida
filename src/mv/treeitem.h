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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QList>
#include <QObject>
#include <QDateTime>

class TreeItem : public QObject
{
    Q_OBJECT
public:
    static int compare(const TreeItem &item1, const TreeItem &item2,
                       int column)
    {
        QVariant val1 = item1.data(column);
        QVariant val2 = item2.data(column);

        if ( val1 == val2 || val1.type() != val2.type() )
            return 0;

        switch ( val1.type() )
        {
        case QVariant::String:
            return QString::localeAwareCompare( val1.toString(),
                                                val2.toString() );
        case QVariant::Int:
        {
            int intVal1 = val1.toInt();
            int intVal2 = val2.toInt();

            if (intVal1 < intVal2)
                return -1;
            else if (intVal1 > intVal2)
                return 1;
            else
                return 0;
        }
        case QVariant::DateTime:
        {
            uint uintVal1 = val1.toDateTime().toTime_t();
            uint uintVal2 = val2.toDateTime().toTime_t();

            if (uintVal1 < uintVal2)
                return -1;
            else if (uintVal1 > uintVal2)
                return 1;
            else
                return 0;
        }
        default:
            return 0;
        }
    }

    TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
    virtual ~TreeItem();

    int columnCount() const;
    TreeItem *parent();
    int row() const;
    TreeItem *child(int row);
    int childCount() const;
    bool appendChild(TreeItem *child);
    bool removeChild(int row);
    void rearrangeChildren(TreeItem *child1, TreeItem *child2);

    virtual QVariant data(int column) const;
    void modifyData(int column, const QVariant &data);

signals:
    void dataChanged(int itemRow, int column);

protected:
    QList<QVariant> itemData;
    TreeItem *parentItem;

private:
    QList<TreeItem*> childItems;

};

#endif // TREEITEM_H
