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

#ifndef THREADMODELITEM_H
#define THREADMODELITEM_H

#include "src/mv/treeitem.h"
#include "src/core/imageboardthread.h"

#include <QObject>
#include <QList>
#include <QVariant>
#include <QMap>

class ThreadModelItem : public TreeItem
{
    Q_OBJECT
public:
    ThreadModelItem(const QList<QVariant> &data, TreeItem *parent = 0);

public slots:
    void threadInfoChanged(ImageboardThread::Info key, const QVariant &data);

};

#endif // THREADMODELITEM_H
