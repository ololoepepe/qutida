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

#include "src/core/download.h"
#include "src/core/networkaccessmanager.h"

#include <QObject>
#include <QString>
#include <QNetworkRequest>
#include <QTimer>

const int Download::REPLY_TIMEOUT = 10 * 1000; //10 seconds
const int Download::RETRY_DELAY = 5 * 1000; //5 seconds

//

Download::Download(const QString &url, bool aux,
                   int attempts, QObject *parent) :
    QObject(parent)
{
    downloadUrl = url;
    isAux = aux;
    attemptsTotal = (attempts > 1) ? attempts : 1;
    attemptsUsed = 0;
    networkReply = 0;
    downloadState = !downloadUrl.isEmpty() ? InitialState : FailedState;
    timer.setInterval(REPLY_TIMEOUT);
    connect( &timer, SIGNAL( timeout() ), this, SLOT( timerTimeout() ) );
}

Download::~Download()
{
    if (networkReply)
    {
        networkReply->deleteLater();
    }
}

//

QString Download::url() const
{
    return downloadUrl;
}

bool Download::isAuxiliary() const
{
    return isAux;
}

const QByteArray &Download::data() const
{
    return downloadData;
}

Download::State Download::state() const
{
    return downloadState;
}

//

void Download::start()
{
    if (InitialState != downloadState)
    {
        return;
    }

    if ( downloadUrl.isEmpty() )
    {
        downloadState = FailedState;
        return;
    }

    downloadState = ActiveState;
    resetReply();
}

void Download::abort()
{
    if (ActiveState != downloadState)
    {
        return;
    }

    timer.stop();
    killReply();
    downloadState = FailedState;
}

//

void Download::killReply()
{
    if (networkReply)
    {
        networkReply->blockSignals(true);
        networkReply->abort();
        networkReply->deleteLater();
        networkReply = 0;
    }
}

//

void Download::timerTimeout()
{
    timer.stop();
    killReply();
    ++attemptsUsed;

    if (attemptsUsed < attemptsTotal)
    {
        QTimer::singleShot( RETRY_DELAY, this, SLOT( resetReply() ) );
    }
    else
    {
        downloadState = FailedState;
        emit finished();
    }
}

void Download::resetReply()
{
    QNetworkRequest request(downloadUrl);
    request.setOriginatingObject(this);
    networkReply = NetworkAccessManager::instance()->get(request);
    connect( networkReply, SIGNAL( finished() ),
             this, SLOT( replyFinished() ) );
    connect( networkReply, SIGNAL( downloadProgress(qint64, qint64) ),
             this, SLOT( downloadProgress(qint64, qint64) ) );
}

void Download::downloadProgress(qint64 bytesRecieved, qint64 bytesTotal)
{
    if ( (bytesRecieved == bytesTotal) && (bytesTotal == 0) )
    {
        timerTimeout();
    }
    else
    {
        timer.stop();
        timer.start();
    }
}

void Download::replyFinished()
{
    timer.stop();

    if ( networkReply->error() )
    {
        killReply();
        ++attemptsUsed;

        if (attemptsUsed < attemptsTotal)
        {
            QTimer::singleShot( RETRY_DELAY, this, SLOT( resetReply() ) );
        }
        else
        {
            downloadState = FailedState;
            emit finished();
        }
    }
    else
    {
        downloadData = networkReply->readAll();
        killReply();
        downloadState = CompletedState;
        emit finished();
    }
}
