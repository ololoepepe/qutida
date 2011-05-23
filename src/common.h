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

#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QString>

namespace Common
{
    const QString APP_NAME = "qutida";
    const QString APP_VERSION = "0.1.8";
    const QString ORG_NAME = "Andrew Bogdanov";
    const QString ORG_DOMAIN = "https://github.com/the-dark-angel";
    const QString CR_YEARS = "2011";

    void deleteObject(QObject *object, int msec = 0);
    QString getThread(const QString &url);
    QString getBoard(const QString &url);
    QString getHost(const QString &url);
    QString strFromList(const QStringList &list,
                        const QString &sep = QString(),
                        int begin = -1, int end = -1);
    QString getFileName( const QString &url, const QString &dir = QString() );
    QString constructLink(const QString &url);
    bool fileExists(const QString &fileName);
    bool isThreadUrlValid(const QString &url);
}

#endif // COMMON_H
