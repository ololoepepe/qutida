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

#ifndef THREADMODEL_H
#define THREADMODEL_H

#include "src/mv/treemodel.h"
#include "src/core/imageboardthread.h"

#include <QObject>
#include <QVariant>
#include <QList>
#include <QModelIndex>
#include <QMap>

class ThreadModel : public TreeModel
{
    Q_OBJECT
public:
    static QList<QVariant> dataFromInfoMap(
        const QMap<ImageboardThread::Info, QVariant> &infoMap)
    {
        QList<QVariant> data;
        data << infoMap.value(ImageboardThread::InfoThread);
        data << infoMap.value(ImageboardThread::InfoBoard);
        data << infoMap.value(ImageboardThread::InfoHost);
        data << infoMap.value(ImageboardThread::InfoStateExtended);
        data << infoMap.value(ImageboardThread::InfoProgress);
        data << infoMap.value(ImageboardThread::InfoDir);
        data << infoMap.value(ImageboardThread::InfoUrl);
        return data;
    }

    explicit ThreadModel(const QList<QVariant> &headerData,
                         QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    void sort(int column, Qt::SortOrder order);

    void addItem(const ImageboardThread *thread);
    bool sortItems(int column, bool ascending);
    void retranslate();

signals:
    void itemsRearranged(int prevIndex1, int prevIndex2);

private slots:
    void itemDataChanged(int row, int column);
    void requestDeletion(int row);

};

#endif // THREADMODEL_H
