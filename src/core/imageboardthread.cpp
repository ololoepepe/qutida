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
#include "src/core/rmdirtask.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QThreadPool>
#include <QDir>

const int ImageboardThread::TIMER_INTERVAL = 60 * 1000; //1 minute

const QString ImageboardThread::DEF_EXTENTIONS_PLAIN = "jpg|jpeg|png|gif";
const QStringList ImageboardThread::DEF_EXTENTIONS_LIST =
ImageboardThread::DEF_EXTENTIONS_PLAIN.split("|", QString::SkipEmptyParts);
const ImageboardThread::Parameters ImageboardThread::DEF_PARAMETERS =
{QString(), QDir::homePath(), ImageboardThread::DEF_EXTENTIONS_LIST,
 false, false, 1, 1, false, 20, false};

const QString ImageboardThread::KEY_URL = "url";
const QString ImageboardThread::KEY_DIRECTORY = "directory";
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
                infoMap.value(InfoStateExtended).toInt() );
    connect( &downloadMapper, SIGNAL( mapped(QString) ),
             this, SLOT( downloadFinished(QString) ) );
    timer.setInterval(TIMER_INTERVAL);
    connect( &timer, SIGNAL( timeout() ), this, SLOT( timerTimeout() ) );
    timePassed = 0;
    filesTotal = -1;
    filesSaved = -1;
    filesFailed = -1;
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

ImageboardThread::ExtendedState ImageboardThread::extendedState() const
{
    return static_cast<ExtendedState>(
                infoMap.value(InfoStateExtended).toInt() );
}

QVariant ImageboardThread::dataForKey(Info key) const
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
    changeInfo(InfoStateExtended, ExtActiveStarted);
    filesTotal = 0;
    filesSaved = 0;
    filesFailed = 0;
    changeInfo(InfoFilesTotal, filesTotal);
    changeInfo(InfoFilesSaved, filesSaved);
    changeInfo(InfoFilesFailed, filesFailed);
    changeInfo( InfoProgress, progress() );
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
        changeInfo(InfoStateExtended, previousState);
        changeInfo(InfoTimeRest, -1);
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
        changeInfo(InfoStateExtended, ExtReadyAborted);
        changeInfo(InfoTimeRest, -1);
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
    changeInfo(InfoRestartEnabled, enabled);
    threadParameters.restartInterval = (interval > 0) ? interval : 1;
    changeInfo(InfoRestartInterval, threadParameters.restartInterval);

    if ( (WaitingState == threadState) && !enabled )
    {
        timer.stop();
        threadState = ReadyState;
        changeInfo(InfoStateExtended, previousState);
        changeInfo(InfoTimeRest, -1);
        changeInfo(InfoTimeRest,
                   (threadParameters.restartInterval - timePassed > 0) ?
                       (threadParameters.restartInterval - timePassed) : 1);
    }

    changeInfo(InfoTimeRest, -1);
}

//

int ImageboardThread::progress() const
{
    if (filesTotal > 0)
    {
        return 100 *
                ( (double) (filesSaved + filesFailed) / (double) filesTotal );
    }
    else
    {
        return 0;
    }
}

void ImageboardThread::addDownload(const QString &url, int attempts)
{
    Download *download = new Download(url, attempts, this);
    downloadMap.insert(url, download);
    connect( download, SIGNAL( finished() ), &downloadMapper, SLOT( map() ) );
    downloadMapper.setMapping(download, url);
    download->start();
}

void ImageboardThread::setUpRestart()
{
    threadState = WaitingState;
    changeInfo(InfoStateExtended, ExtWaiting);
    timePassed = 0;
    changeInfo(InfoTimeRest, threadParameters.restartInterval - timePassed);
    timer.start();
}

void ImageboardThread::checkCompletion()
{
    if (filesSaved + filesFailed >= filesTotal)
    {
        threadState = ReadyState;
        changeInfo(InfoStateExtended, ExtReadyCompleted);

        if (threadParameters.restartEnabled)
            setUpRestart();
    }
}

void ImageboardThread::changeInfo(Info key, const QVariant &data)
{
    if (InfoStateExtended == key)
    {
        previousState = static_cast<ExtendedState>(
                    infoMap.value(InfoStateExtended).toInt() );
        infoMap[InfoStateExtendedPrev] = previousState;
        emit infoChanged(InfoStateExtendedPrev, previousState);
    }

    infoMap[key] = data;
    emit infoChanged(key, data);
}

//

void ImageboardThread::timerTimeout()
{
    ++timePassed;
    changeInfo(InfoTimeRest, threadParameters.restartInterval - timePassed);

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
            changeInfo(InfoStateExtended, ExtReadyFailed);
        }
        else
        {
            ParceTask::Parameters param;
            param.download = download;
            param.dir = threadParameters.directory;
            param.extentions = threadParameters.extentions;
            param.external = threadParameters.external;
            param.replace = threadParameters.replace;
            ParceTask *parceTask = new ParceTask(param);
            connect( parceTask, SIGNAL( finished(ParceTask::Result) ),
                     this, SLOT( parceFinished(ParceTask::Result) ) );
            QThreadPool::globalInstance()->start(parceTask);
        }
    }
    else
    {
        if ( Download::CompletedState != download->state() )
        {
            download->deleteLater();
            ++filesFailed;
            changeInfo(InfoFilesFailed, filesFailed);
            changeInfo( InfoProgress, progress() );
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
}

void ImageboardThread::parceFinished(ParceTask::Result result)
{
    if (ParceTask::NoError != result.err)
    {
        threadState = ReadyState;
        changeInfo(InfoStateExtended, ExtReadyFailed);
    }
    else
    {
        filesSaved = result.existingUrls.count();
        filesTotal = filesSaved + result.newUrls.count();

        if (threadParameters.savePage)
        {
            ++filesTotal;
            SaveTask::Parameters param;
            param.download = result.download;
            param.dir = threadParameters.directory;
            param.replace = true;
            SaveTask *saveTask = new SaveTask(param);
            connect( saveTask, SIGNAL( finished(SaveTask::Result) ),
                     this, SLOT( saveFinished(SaveTask::Result) ) );
            QThreadPool::globalInstance()->start(saveTask);
        }
        else
        {
            delete result.download;
        }

        changeInfo(InfoFilesTotal, filesTotal);
        changeInfo(InfoFilesSaved, filesSaved);
        changeInfo( InfoProgress, progress() );

        if ( !result.newUrls.isEmpty() )
        {
            foreach (QString url, result.newUrls)
            {
                addDownload(url, threadParameters.attemptFile);
            }
        }

        if (filesTotal > filesSaved)
        {
            threadState = ActiveState;
            changeInfo(InfoStateExtended, ExtActiveDownloading);
        }
        else
        {
            threadState = ReadyState;
            changeInfo(InfoStateExtended, ExtReadyNothing);
            if (threadParameters.restartEnabled)
                setUpRestart();
        }
    }
}

void ImageboardThread::saveFinished(SaveTask::Result result)
{
    if (SaveTask::NoError != result.err)
    {
        ++filesFailed;
        changeInfo(InfoFilesFailed, filesFailed);
    }
    else
    {
        ++filesSaved;
        changeInfo(InfoFilesSaved, filesSaved);
    }

    delete result.download;
    changeInfo( InfoProgress, progress() );
    checkCompletion();
}

void ImageboardThread::rmdirFinished(RmdirTask::Result result)
{
    this->deleteLater();
}
