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

#include "src/core/localserver.h"
#include "src/core/imageboardthread.h"
#include "src/gui/parametersdialog.h"
#include "src/core/threadmanager.h"

#include <QLocalServer>
#include <QObject>
#include <QLocalSocket>
#include <QSignalMapper>
#include <QByteArray>
#include <QList>
#include <QStringList>
#include <QString>
#include <QSettings>
#include <QUrl>
#include <QDateTime>

const QString LocalServer::PREFIX = "-";
const QString LocalServer::ARG_MULTIPLE = PREFIX + "multiple";
const QString LocalServer::ARG_URLS = PREFIX + "urls";
const QString LocalServer::ARG_DEFAULT = PREFIX + "default";
const QString LocalServer::ARG_START = PREFIX + "start";

//

LocalServer::LocalServer(QObject *parent) :
    QLocalServer(parent)
{
    connect( this, SIGNAL( newConnection() ),
             this, SLOT( newConnectionAvailable() ) );
    connect( &socketMapper, SIGNAL( mapped(QObject*) ),
             this, SLOT( readChannelFinished(QObject*) ) );
}

//

void LocalServer::newConnectionAvailable()
{
    QLocalSocket *socket = this->nextPendingConnection();
    connect( socket, SIGNAL( readyRead() ),
             &socketMapper, SLOT( map() ) );
    socketMapper.setMapping(socket, socket);
}

void LocalServer::readChannelFinished(QObject *object)
{
    QLocalSocket *socket = dynamic_cast<QLocalSocket*>(object);

    if (!socket)
        return;

    if (socket->bytesAvailable() < 1)
        return;

    QList<QByteArray> rawList = socket->readAll().split('\0');
    socket->deleteLater();
    QStringList argList;
    QStringList urlList;
    bool fillingUrlList = false;

    for (int i = 0; i < rawList.count(); ++i)
    {
        QString arg( rawList.at(i) );

        if ( arg.at(0) == PREFIX.at(0) )
        {
            argList << arg;

            if (fillingUrlList)
                fillingUrlList = false;
            else if (ARG_URLS == arg)
                fillingUrlList = true;
        }
        else if (fillingUrlList)
        {
            urlList << QUrl::fromUserInput(arg).toString();
        }
    }

    if ( urlList.isEmpty() )
        return;

    if ( argList.contains(ARG_DEFAULT) )
    {
        bool start = argList.contains(ARG_START);
        QSettings settings;
        settings.beginGroup(ParametersDialog::GROUP_PARAMETERS);
          settings.beginGroup(ThreadManager::SUB_GROUP_DEFAULT);
            ImageboardThread::Parameters param =
                    ImageboardThread::readParameters(settings);
          settings.endGroup();
        settings.endGroup();

        for (int i = 0; i < urlList.count(); ++i)
        {
            param.url = urlList.at(i);
            param.added = QDateTime::currentDateTime();
            emit addThreadSilent(param, start);
        }
    }
    else
    {
        emit addThreads(urlList);
    }
}
