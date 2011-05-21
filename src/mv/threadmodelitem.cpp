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

#include "src/mv/threadmodelitem.h"
#include "src/core/imageboardthread.h"
#include "src/tr.h"

#include <QObject>
#include <QList>
#include <QVariant>

ThreadModelItem::ThreadModelItem(ImageboardThread *thread,
                                 TreeItem *parent) :
    TreeItem(QList<QVariant>(), parent)
{
    itemData = dataFromInfoMap(
                ImageboardThread::infoFromParameters( thread->parameters() ) );
    mRelatedThread = thread;
    connect( thread,
             SIGNAL( infoChanged(ImageboardThread::Info, QVariant) ),
             this,
             SLOT( threadInfoChanged(ImageboardThread::Info, QVariant) ) );
}

//

ImageboardThread *ThreadModelItem::relatedThread()
{
    return mRelatedThread;
}

//

void ThreadModelItem::threadInfoChanged(ImageboardThread::Info key,
                                        const QVariant &data)
{
    if (key >= itemData.count() || key < 0)
        return;

    itemData.replace(key, data);
    emit dataChanged(row(), key);
}
