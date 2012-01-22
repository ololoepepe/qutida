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

#ifndef CATEGORYMODEL_H
#define CATEGORYMODEL_H

#include "src/mv/treemodel.h"
#include "src/tr.h"
#include "src/core/imageboardthread.h"
#include "src/core/threadinfo.h"

#include <QObject>
#include <QString>
#include <QVariant>
#include <QModelIndex>
#include <QList>
#include <QStringList>

class CategoryModel : public TreeModel
{
    Q_OBJECT
public:
    explicit CategoryModel(QObject *parent = 0);

    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;

    void tryAddCategory(ImageboardThread *thread);
    void tryRemoveCategory(ImageboardThread *thread);
    void checkState(ImageboardThread::ExtendedState state, bool add);
    void retranslate();
    Tr::CM::Category categoryForIndex(const QModelIndex &index) const;

signals:
    void errorCountChanged(int count);

private:
    int countAll;
    int countActive;
    int countWaiting;
    int countReady;
    int countError;
    QStringList hostList;
    QStringList boardList;

private slots:
    void threadInfoChanged(ThreadInfo::Enum key, const QVariant &data);

};

#endif // CATEGORYMODEL_H
