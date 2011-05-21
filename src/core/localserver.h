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

#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include "src/core/imageboardthread.h"

#include <QLocalServer>
#include <QObject>
#include <QStringList>
#include <QSignalMapper>
#include <QList>
#include <QLocalSocket>
#include <QString>

class LocalServer : public QLocalServer
{
    Q_OBJECT
public:
    static const QString PREFIX;
    static const QString ARG_MULTIPLE;
    static const QString ARG_URLS;
    static const QString ARG_DEFAULT;
    static const QString ARG_START;

    explicit LocalServer(QObject *parent = 0);

signals:
    void addThreads(const QStringList &urlList);
    void addThreadSilent(const ImageboardThread::Parameters &param,
                         bool start);

private:
    QSignalMapper socketMapper;

private slots:
    void newConnectionAvailable();
    void readChannelFinished(QObject *object);

};

#endif // LOCALSERVER_H
