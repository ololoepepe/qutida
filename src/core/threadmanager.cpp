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

#include "src/core/threadmanager.h"
#include "src/mv/threadmodel.h"
#include "src/core/imageboardthread.h"
#include "src/mv/categorymodel.h"
#include "src/common.h"

#include <QObject>
#include <QList>
#include <QVariant>
#include <QAbstractItemModel>
#include <QMap>
#include <QSettings>
#include <QUrl>
#include <QDesktopServices>
#include <QDialog>
#include <QModelIndex>

const QString ThreadManager::GROUP_THREADS = "threads";
const QString ThreadManager::SUB_GROUP_DEFAULT = "default";
const QString ThreadManager::ARRAY_THREADS = "threads";

//

ThreadManager::ThreadManager(QObject *parent) :
    QObject(parent)
{
    QList<QVariant> headerData;
    for (int i = ImageboardThread::InfoIteratorFirst;
         i <= ImageboardThread::InfoIteratorLast; ++i)
        headerData << i;
    mThreadModel = new ThreadModel(headerData, this);
    connect( mThreadModel, SIGNAL( itemsRearranged(int, int) ),
             this, SLOT( threadModelItemsRearranged(int, int) ) );
    mCategoryModel = new CategoryModel(this);
    readSettings();
}

ThreadManager::~ThreadManager()
{
    if (mThreadModel)
        mThreadModel->deleteLater();

    if (mCategoryModel)
        mCategoryModel->deleteLater();

    foreach (ImageboardThread *thread, threadList)
    {
        if (thread)
            thread->deleteLater();
    }
}

//

ThreadModel *ThreadManager::threadModel()
{
    return mThreadModel;
}

CategoryModel *ThreadManager::categoryModel()
{
    return mCategoryModel;
}

//

void ThreadManager::requestAddThread(const ImageboardThread::Parameters &param,
                                     bool start)
{
    foreach (ImageboardThread *thread, threadList)
        if (thread)
            if (thread->url() == param.url)
                return;

    ImageboardThread *thread = new ImageboardThread(param, this);
    threadList.append(thread);
    mThreadModel->addItem(thread);
    mCategoryModel->tryAddCategory(thread);

    if (start)
        thread->startDownload();
}

void ThreadManager::requestRemoveThread(int index, bool del)
{
    if (index >= threadList.count() || index < 0)
        return;

    ImageboardThread *thread = threadList.takeAt(index);

    if (!thread)
        return;

    mCategoryModel->tryRemoveCategory(thread);

    if (del)
    {
        thread->deleteWithFiles();
    }
    else
    {
        thread->stopDownload();
        thread->deleteLater();
    }
}

void ThreadManager::requestStartThread(int index)
{
    if (index >= threadList.count() || index < 0)
        return;

    ImageboardThread *thread = threadList.value(index);

    if (!thread)
        return;

    thread->startDownload();
}

void ThreadManager::requestStopThread(int index)
{
    if (index >= threadList.count() || index < 0)
        return;

    ImageboardThread *thread = threadList.value(index);

    if (!thread)
        return;

    thread->stopDownload();
}

void ThreadManager::requestOpenDir(int index)
{
    if (index >= threadList.count() || index < 0)
        return;

    ImageboardThread *thread = threadList.value(index);

    if (!thread)
        return;

    QDesktopServices::openUrl( QUrl::fromLocalFile( thread->dir() ) );
}

void ThreadManager::requestOpenUrl(int index)
{
    if (index >= threadList.count() || index < 0)
        return;

    ImageboardThread *thread = threadList.value(index);

    if (!thread)
        return;

    QDesktopServices::openUrl( QUrl(thread->url(), QUrl::TolerantMode ) );
}

void ThreadManager::requestSortThreads(int column, Qt::SortOrder order)
{
    int count = threadList.count();

    if (count < 2)
        return;

    mThreadModel->sort(column, order);
}

void ThreadManager::requestSetObservedThread(int index, InfoWidget *widget)
{
    if (!widget)
        return;

    if (index >= threadList.count() || index < 0)
    {
        widget->setObservedThread();
        return;
    }

    widget->setObservedThread( threadList.value(index) );
}

void ThreadManager::requestModifyRestart(const QList<int> &indexes,
                                         bool enabled, int interval)
{
    for (int i = 0; i < indexes.count(); ++i)
    {
        int index = indexes.at(i);

        if ( ( index < threadList.count() ) && (index > -1) )
        {
            ImageboardThread *thread = threadList.value(i);

            if (thread)
                thread->modifyRestart(enabled, interval);
        }
    }
}

void ThreadManager::requestRetranslate()
{
    mThreadModel->retranslate();
    mCategoryModel->retranslate();
}

void ThreadManager::requestWriteSettings()
{
    QSettings settings;
    settings.beginGroup(GROUP_THREADS);
      settings.remove(ARRAY_THREADS);
      settings.beginWriteArray(ARRAY_THREADS);

      for (int i = 0;i < threadList.count(); ++i)
      {
          settings.setArrayIndex(i);
          ImageboardThread::writeParameters( settings,
                                            threadList.at(i)->parameters() );
      }

      settings.endArray();
    settings.endGroup();
}

//

void ThreadManager::readSettings()
{
    QSettings settings;
    settings.beginGroup(GROUP_THREADS);
      settings.beginGroup(SUB_GROUP_DEFAULT);
      ImageboardThread::Parameters defParam =
              ImageboardThread::readParameters(settings);
      settings.endGroup();
      int count = settings.beginReadArray(ARRAY_THREADS);

      for (int i = 0; i < count; ++i)
      {
          settings.setArrayIndex(i);
          ImageboardThread::Parameters param =
                  ImageboardThread::readParameters(settings, defParam);
          requestAddThread(param, false); //to be improved
      }

      settings.endArray();
    settings.endGroup();
}

//

void ThreadManager::threadModelItemsRearranged(int prevIndex1, int prevIndex2)
{
    ImageboardThread *thread1 = threadList.value(prevIndex1);
    ImageboardThread *thread2 = threadList.value(prevIndex2);
    threadList.replace(prevIndex1, thread2);
    threadList.replace(prevIndex2, thread1);
}
