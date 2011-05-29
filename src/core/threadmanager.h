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

#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "src/core/imageboardthread.h"
#include "src/mv/treemodel.h"
#include "src/mv/threadmodel.h"
#include "src/gui/infowidget.h"
#include "src/mv/categorymodel.h"

#include <QObject>
#include <QAbstractItemModel>
#include <QList>
#include <QString>
#include <QSettings>
#include <QWidget>
#include <QModelIndex>

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    static const QString GROUP_THREADS;
    static const QString SUB_GROUP_DEFAULT;
    static const QString ARRAY_THREADS;

    explicit ThreadManager(QObject *parent = 0);
    ~ThreadManager();

    ThreadModel *threadModel();
    CategoryModel *categoryModel();

public slots:
    void requestAddThread(const ImageboardThread::Parameters &param,
                          bool start);
    void requestBackup(const QString &fileName);
    void requestRemoveThread(int index, bool del);
    void requestStartThread(int index);
    void requestStopThread(int index);
    void requestOpenDir(int index);
    void requestOpenUrl(int index);
    void requestSetObservedThread(int index, InfoWidget *widget);
    void requestModifyParameters(const QList<int> &indexes,
                                 const ImageboardThread::Modifiable &modParam);
    void requestRetranslate();
    void requestWriteSettings();

private:
    ThreadModel *mThreadModel;
    CategoryModel *mCategoryModel;
    QList<ImageboardThread*> threadList;

    void readSettings();

};

#endif // THREADMANAGER_H
