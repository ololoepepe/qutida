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
        data << infoMap.value(ImageboardThread::InfoAdded);
        return data;
    }

    ThreadModelItem(ImageboardThread *thread, TreeItem *parent = 0);

    ImageboardThread *relatedThread();

private:
    ImageboardThread *mRelatedThread;

private slots:
    void threadInfoChanged(ImageboardThread::Info key, const QVariant &data);

};

#endif // THREADMODELITEM_H
