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

#include "src/core/imageboardthread.h"
#include "src/core/download.h"
#include "src/core/parcetask.h"
#include "src/core/savetask.h"
#include "src/core/savepagetask.h"
#include "src/core/rmdirtask.h"
#include "src/core/threadinfo.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QThreadPool>
#include <QDir>
#include <QDateTime>

const int ImageboardThread::TIMER_INTERVAL = 60 * 1000; //1 minute

const QString ImageboardThread::DEF_EXTENTIONS_PLAIN = "jpg|jpeg|png|gif";
const QStringList ImageboardThread::DEF_EXTENTIONS_LIST =
ImageboardThread::DEF_EXTENTIONS_PLAIN.split("|", QString::SkipEmptyParts);
const ImageboardThread::Parameters ImageboardThread::DEF_PARAMETERS =
{QString(), QDir::homePath(), QDateTime::currentDateTime(),
 ImageboardThread::DEF_EXTENTIONS_LIST, false, false, 1, 1, false, 20, false};

const QString ImageboardThread::KEY_URL = "url";
const QString ImageboardThread::KEY_DIRECTORY = "directory";
const QString ImageboardThread::KEY_ADDED = "added";
const QString ImageboardThread::KEY_EXTENTIONS = "extentions";
const QString ImageboardThread::KEY_EXTERNAL = "external";
const QString ImageboardThread::KEY_REPLACE = "replace";
const QString ImageboardThread::KEY_ATTEMPT_PAGE = "attempt_page";
const QString ImageboardThread::KEY_ATTEMPT_FILE = "attempt_file";
const QString ImageboardThread::KEY_RESTART_ENABLED = "restart_enabled";
const QString ImageboardThread::KEY_RESTART_INTERVAL = "restart_interval";
const QString ImageboardThread::KEY_SAVE_PAGE = "save_page";

//

ImageboardThread::ImageboardThread(const Parameters &param, QObject *parent) :
    QObject(parent)
{
    threadParameters = param;
    infoMap = infoFromParameters(threadParameters);
    previousState = static_cast<ExtendedState>(
                infoMap.value(ThreadInfo::ExtendedState).toInt() );
    connect( &downloadMapper, SIGNAL( mapped(QString) ),
             this, SLOT( downloadFinished(QString) ) );
    timer.setInterval(TIMER_INTERVAL);
    connect( &timer, SIGNAL( timeout() ), this, SLOT( timerTimeout() ) );
    timePassed = 0;
    filesTotal = -1;
    filesSaved = -1;
    filesFailed = -1;
    filesAuxTotal = -1;
    filesAuxSaved = -1;
    filesAuxFailed = -1;
    threadState = ReadyState;
}

ImageboardThread::~ImageboardThread()
{
    if ( downloadMap.isEmpty() )
        return;

    downloadMapper.blockSignals(true);

    foreach ( Download *download, downloadMap.values() )
    {
        download->abort();
        download->deleteLater();
    }
}

//

const ImageboardThread::Parameters &ImageboardThread::parameters() const
{
    return threadParameters;
}

QString ImageboardThread::url() const
{
    return threadParameters.url;
}

QString ImageboardThread::dir() const
{
    return threadParameters.directory;
}

bool ImageboardThread::restartEnabled() const
{
    return threadParameters.restartEnabled;
}

int ImageboardThread::restartInterval() const
{
    return threadParameters.restartInterval;
}

bool ImageboardThread::savePage() const
{
    return threadParameters.savePage;
}

ImageboardThread::ExtendedState ImageboardThread::extendedState() const
{
    return static_cast<ExtendedState>(
                infoMap.value(ThreadInfo::ExtendedState).toInt() );
}

QVariant ImageboardThread::dataForKey(ThreadInfo::Enum key) const
{
    return infoMap.value(key);
}

//

void ImageboardThread::startDownload()
{
    if (ActiveState == threadState)
    {
        return;
    }

    if (WaitingState == threadState)
    {
        timer.stop();
    }

    threadState = ActiveState;
    changeInfo(ThreadInfo::ExtendedState, ExtActiveStarted);
    filesTotal = -1;
    filesSaved = -1;
    filesFailed = -1;
    filesAuxTotal = -1;
    filesAuxSaved = -1;
    filesAuxFailed = -1;
    changeInfo(ThreadInfo::FilesTotal, filesTotal);
    changeInfo(ThreadInfo::FilesSaved, filesSaved);
    changeInfo(ThreadInfo::FilesFailed, filesFailed);
    changeInfo(ThreadInfo::FilesAuxTotal, filesAuxTotal);
    changeInfo(ThreadInfo::FilesAuxSaved, filesAuxSaved);
    changeInfo(ThreadInfo::FilesAuxFailed, filesAuxFailed);
    changeInfo( ThreadInfo::Progress, progress() );
    addDownload(threadParameters.url, threadParameters.attemptPage);
}

void ImageboardThread::stopDownload()
{
    if (ReadyState == threadState)
    {
        return;
    }
    else if (WaitingState == threadState)
    {
        timer.stop();
        threadState = ReadyState;
        changeInfo(ThreadInfo::ExtendedState, previousState);
        changeInfo(ThreadInfo::TimeRest, -1);
    }
    else
    {
        downloadMapper.blockSignals(true);

        foreach ( Download *download, downloadMap.values() )
        {
            download->abort();
            download->deleteLater();
        }

        downloadMap.clear();
        downloadMapper.blockSignals(false);
        threadState = ReadyState;
        changeInfo(ThreadInfo::ExtendedState, ExtReadyAborted);
        changeInfo(ThreadInfo::TimeRest, -1);
    }
}

void ImageboardThread::deleteWithFiles()
{
    stopDownload();
    RmdirTask::Parameters param;
    param.dir = threadParameters.directory;
    RmdirTask *rmdirTask = new RmdirTask(param);
    connect( rmdirTask, SIGNAL( finished(RmdirTask::Result) ),
             this, SLOT( rmdirFinished(RmdirTask::Result) ) );
    QThreadPool::globalInstance()->start(rmdirTask);
}

void ImageboardThread::modifyRestart(bool enabled, int interval)
{
    threadParameters.restartEnabled = enabled;
    changeInfo(ThreadInfo::RestartEnabled, enabled);
    threadParameters.restartInterval = (interval > 0) ? interval : 1;
    changeInfo(ThreadInfo::RestartInterval, threadParameters.restartInterval);

    if ( (WaitingState == threadState) && !enabled )
    {
        timer.stop();
        threadState = ReadyState;
        changeInfo(ThreadInfo::ExtendedState, previousState);
        changeInfo(ThreadInfo::TimeRest, -1);
    }
    else if (WaitingState == threadState)
    {
        changeInfo(ThreadInfo::TimeRest,
                   (threadParameters.restartInterval - timePassed > 0) ?
                       (threadParameters.restartInterval - timePassed) : 1);
    }
    else
    {
        changeInfo(ThreadInfo::TimeRest, -1);
    }
}

void ImageboardThread::modifySavePage(bool enabled)
{
    threadParameters.savePage = enabled;
    changeInfo(ThreadInfo::SavePage, enabled);
}

//

int ImageboardThread::progress() const
{
    int total = 0;
    total += (filesTotal > 0) ? filesTotal : 0;
    total += (filesAuxTotal > 0) ? filesAuxTotal : 0;
    int ready = 0;
    ready += (filesSaved > 0) ? filesSaved : 0;
    ready += (filesFailed > 0) ? filesFailed : 0;
    ready += (filesAuxSaved > 0) ? filesAuxSaved : 0;
    ready += (filesAuxFailed > 0) ? filesAuxFailed : 0;

    if (0 == ready)
        return 0;
    else
        return 100 * ( (double) ready / (double) total );
}

void ImageboardThread::addDownload(const QString &url, int attempts, bool aux)
{
    Download *download = new Download(url, aux, attempts, this);
    downloadMap.insert(url, download);
    connect( download, SIGNAL( finished() ), &downloadMapper, SLOT( map() ) );
    downloadMapper.setMapping(download, url);
    download->start();
}

void ImageboardThread::setUpRestart()
{
    threadState = WaitingState;
    changeInfo(ThreadInfo::ExtendedState, ExtWaiting);
    timePassed = 0;
    changeInfo(ThreadInfo::TimeRest,
               threadParameters.restartInterval - timePassed);
    timer.start();
}

void ImageboardThread::checkCompletion()
{
    if ( (filesSaved + filesFailed >= filesTotal) &&
            (filesAuxSaved + filesAuxFailed >= filesAuxTotal) )
    {
        threadState = ReadyState;
        changeInfo(ThreadInfo::ExtendedState, ExtReadyCompleted);

        if (threadParameters.restartEnabled)
            setUpRestart();
    }
}

void ImageboardThread::changeInfo(ThreadInfo::Enum key, const QVariant &data)
{
    if (ThreadInfo::ExtendedState == key)
    {
        previousState = static_cast<ExtendedState>(
                    infoMap.value(ThreadInfo::ExtendedState).toInt() );
        infoMap[ThreadInfo::ExtendedStatePrev] = previousState;
        emit infoChanged(ThreadInfo::ExtendedStatePrev, previousState);
    }

    infoMap[key] = data;
    emit infoChanged(key, data);
}

//

void ImageboardThread::timerTimeout()
{
    ++timePassed;
    changeInfo(ThreadInfo::TimeRest,
               threadParameters.restartInterval - timePassed);

    if (timePassed >= threadParameters.restartInterval)
    {
        startDownload();
    }
}

void ImageboardThread::downloadFinished(const QString &url)
{
    Download *download = downloadMap.take(url);

    if (!download)
    {
        return;
    }

    if (url == threadParameters.url)
    {
        if ( Download::CompletedState != download->state() )
        {
            download->deleteLater();
            threadState = ReadyState;
            changeInfo(ThreadInfo::ExtendedState, ExtReadyFailed);
        }
        else
        {
            ParceTask::Parameters param;
            param.download = download;
            param.dir = threadParameters.directory;
            param.extentions = threadParameters.extentions;
            param.external = threadParameters.external;
            param.replace = threadParameters.replace;
            param.savePage = threadParameters.savePage;
            ParceTask *parceTask = new ParceTask(param);
            connect( parceTask, SIGNAL( finished(ParceTask::Result) ),
                     this, SLOT( parceFinished(ParceTask::Result) ) );
            QThreadPool::globalInstance()->start(parceTask);
        }
    }
    else if ( !download->isAuxiliary() )
    {
        if ( Download::CompletedState != download->state() )
        {
            download->deleteLater();
            ++filesFailed;
            changeInfo(ThreadInfo::FilesFailed, filesFailed);
            changeInfo( ThreadInfo::Progress, progress() );
            checkCompletion();
        }
        else
        {
            SaveTask::Parameters param;
            param.download = download;
            param.dir = threadParameters.directory;
            param.replace = threadParameters.replace;
            SaveTask *saveTask = new SaveTask(param);
            connect( saveTask, SIGNAL( finished(SaveTask::Result) ),
                     this, SLOT( saveFinished(SaveTask::Result) ) );
            QThreadPool::globalInstance()->start(saveTask);
        }
    }
    else
    {
        if ( Download::CompletedState != download->state() )
        {
            download->deleteLater();
            ++filesAuxFailed;
            changeInfo(ThreadInfo::FilesAuxFailed, filesAuxFailed);
            changeInfo( ThreadInfo::Progress, progress() );
            checkCompletion();
        }
        else
        {
            SaveTask::Parameters param;
            param.download = download;
            param.dir = threadParameters.directory + QDir::separator() +
                    SavePageTask::AUX_FILES_DIR;
            param.replace = true;
            SaveTask *saveTask = new SaveTask(param);
            connect( saveTask, SIGNAL( finished(SaveTask::Result) ),
                     this, SLOT( saveFinished(SaveTask::Result) ) );
            QThreadPool::globalInstance()->start(saveTask);
        }
    }
}

void ImageboardThread::parceFinished(ParceTask::Result result)
{
    if (ParceTask::NoError != result.err)
    {
        threadState = ReadyState;
        changeInfo(ThreadInfo::ExtendedState, ExtReadyFailed);
    }
    else
    {
        filesSaved = result.existingUrls.count();
        filesFailed = 0;
        filesTotal = filesSaved + result.newUrls.count();

        if (threadParameters.savePage)
        {
            SavePageTask::Parameters param;
            param.download = result.download;
            param.dir = threadParameters.directory;
            param.replace = true;
            param.urls = result.auxUrls;
            param.normalUrls = result.newUrls;
            param.normalUrls += result.existingUrls;
            SavePageTask *savePageTask = new SavePageTask(param);
            connect( savePageTask, SIGNAL(finished (SavePageTask::Result) ),
                     this, SLOT( savePageFinished(SavePageTask::Result) ) );
            QThreadPool::globalInstance()->start(savePageTask);

            if ( !result.auxUrls.isEmpty() )
            {
                filesAuxSaved = 0;
                filesAuxFailed = 0;
                filesAuxTotal = 1;

                foreach (QString url, result.auxUrls)
                {
                    if ( !result.existingUrls.contains(url) &&
                            !result.newUrls.contains(url) )
                    {
                        addDownload(url, threadParameters.attemptFile, true);
                        ++filesAuxTotal;
                    }
                }

                changeInfo(ThreadInfo::FilesAuxTotal, filesAuxTotal);
                changeInfo(ThreadInfo::FilesAuxSaved, filesAuxSaved);
                changeInfo(ThreadInfo::FilesAuxFailed, filesAuxFailed);
            }
        }
        else
        {
            delete result.download;
        }

        changeInfo(ThreadInfo::FilesTotal, filesTotal);
        changeInfo(ThreadInfo::FilesSaved, filesSaved);
        changeInfo(ThreadInfo::FilesFailed, filesFailed);
        changeInfo( ThreadInfo::Progress, progress() );

        if ( !result.newUrls.isEmpty() )
        {
            foreach (QString url, result.newUrls)
            {
                addDownload(url, threadParameters.attemptFile);
            }
        }

        if (filesTotal > filesSaved || filesAuxTotal > 0)
        {
            threadState = ActiveState;
            changeInfo(ThreadInfo::ExtendedState, ExtActiveDownloading);
        }
        else
        {
            threadState = ReadyState;
            changeInfo(ThreadInfo::ExtendedState, ExtReadyNothing);
            if (threadParameters.restartEnabled)
                setUpRestart();
        }
    }
}

void ImageboardThread::saveFinished(SaveTask::Result result)
{
    if ( !result.download->isAuxiliary() )
    {
        if (SaveTask::NoError != result.err)
        {
            ++filesFailed;
            changeInfo(ThreadInfo::FilesFailed, filesFailed);
        }
        else
        {
            ++filesSaved;
            changeInfo(ThreadInfo::FilesSaved, filesSaved);
        }
    }
    else
    {
        if (SaveTask::NoError != result.err)
        {
            ++filesAuxFailed;
            changeInfo(ThreadInfo::FilesAuxFailed, filesAuxFailed);
        }
        else
        {
            ++filesAuxSaved;
            changeInfo(ThreadInfo::FilesAuxSaved, filesAuxSaved);
        }
    }

    delete result.download;
    changeInfo( ThreadInfo::Progress, progress() );
    checkCompletion();
}

void ImageboardThread::savePageFinished(SavePageTask::Result result)
{
    if (SaveTask::NoError != result.err)
    {
        ++filesAuxFailed;
        changeInfo(ThreadInfo::FilesAuxFailed, filesAuxFailed);
    }
    else
    {
        ++filesAuxSaved;
        changeInfo(ThreadInfo::FilesAuxSaved, filesAuxSaved);
    }

    delete result.download;
    changeInfo( ThreadInfo::Progress, progress() );
    checkCompletion();
}

void ImageboardThread::rmdirFinished(RmdirTask::Result result)
{
    this->deleteLater();
}
