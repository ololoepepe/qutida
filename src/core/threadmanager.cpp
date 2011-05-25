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
#include "threadinfo.h"

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
#include <QFile>
#include <QIODevice>
#include <QTextStream>

const QString ThreadManager::GROUP_THREADS = "threads";
const QString ThreadManager::SUB_GROUP_DEFAULT = "default";
const QString ThreadManager::ARRAY_THREADS = "threads";

//

ThreadManager::ThreadManager(QObject *parent) :
    QObject(parent)
{
    QList<QVariant> headerData;

    for (ThreadInfo i = ThreadInfo::IteratorFirst;
         i <= ThreadInfo::IteratorMiddle; ++i)
        headerData << i.toEnum();

    mThreadModel = new ThreadModel(headerData, this);
    mCategoryModel = new CategoryModel(this);
    sortingColumn = -1;
    sortingOrder = Qt::AscendingOrder;
    threadsSorted = false;
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
    if ( !Common::isThreadUrlValid(param.url) )
        return;

    foreach (ImageboardThread *thread, threadList)
        if (thread)
            if (thread->url() == param.url)
                return;

    ImageboardThread *thread = new ImageboardThread(param, this);
    threadList.append(thread);
    mThreadModel->addItem(thread);
    mCategoryModel->tryAddCategory(thread);
    threadsSorted = false;

    if (start)
        thread->startDownload();

    if (sortingColumn > -1)
        requestSortThreads(sortingColumn, sortingOrder);
}

void ThreadManager::requestBackup(const QString &fileName)
{
    if ( threadList.isEmpty() )
        return;

    QFile file(fileName);

    if ( file.exists() )
        if ( !file.remove() )
            return;

    if ( !file.open(QIODevice::WriteOnly) )
        return;

    QTextStream out(&file);

    for (int i = 0; i < threadList.count(); ++i)
        out << threadList.at(i)->url() << "\n";

    file.close();
}

void ThreadManager::requestRemoveThread(int index, bool del)
{
    if (index >= threadList.count() || index < 0)
        return;

    ImageboardThread *thread = mThreadModel->threadForRow(index);
    threadList.removeAll(thread);

    if (!thread)
        return;

    mCategoryModel->tryRemoveCategory(thread);
    mThreadModel->removeItem(thread);

    if (del)
    {
        thread->deleteWithFiles();
    }
    else
    {
        thread->deleteLater();
    }
}

void ThreadManager::requestStartThread(int index)
{
    if (index >= threadList.count() || index < 0)
        return;

    ImageboardThread *thread = mThreadModel->threadForRow(index);

    if (!thread)
        return;

    thread->startDownload();
}

void ThreadManager::requestStopThread(int index)
{
    if (index >= threadList.count() || index < 0)
        return;

    ImageboardThread *thread = mThreadModel->threadForRow(index);

    if (!thread)
        return;

    thread->stopDownload();
}

void ThreadManager::requestOpenDir(int index)
{
    if (index >= threadList.count() || index < 0)
        return;

    ImageboardThread *thread = mThreadModel->threadForRow(index);

    if (!thread)
        return;

    QDesktopServices::openUrl( QUrl::fromLocalFile( thread->dir() ) );
}

void ThreadManager::requestOpenUrl(int index)
{
    if (index >= threadList.count() || index < 0)
        return;

    ImageboardThread *thread = mThreadModel->threadForRow(index);

    if (!thread)
        return;

    QDesktopServices::openUrl( QUrl(thread->url(), QUrl::TolerantMode ) );
}

void ThreadManager::requestSortThreads(int column, Qt::SortOrder order)
{
    if (threadsSorted && sortingColumn == column && sortingOrder == order)
        return;

    int count = threadList.count();

    if (count < 2)
        return;

    mThreadModel->sort(column, order);
    sortingColumn = column;
    sortingOrder = order;
    threadsSorted = true;
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

    widget->setObservedThread( mThreadModel->threadForRow(index) );
}

void ThreadManager::requestModifyParameters(const QList<int> &indexes,
                                            ThreadParameters::Parameters param)
{
    for (int i = 0; i < indexes.count(); ++i)
    {
        int index = indexes.at(i);

        if ( ( index < threadList.count() ) && (index > -1) )
        {
            ImageboardThread *thread = mThreadModel->threadForRow(index);

            if (thread)
            {
                thread->modifyRestart(param.restartEnabled,
                                      param.restartInterval);
                thread->modifySavePage(param.savePage);
            }
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
