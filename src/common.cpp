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

#include "src/common.h"

#include <QObject>
#include <QTimer>
#include <QString>
#include <QUrl>
#include <QStringList>
#include <QRegExp>
#include <QFileInfo>
#include <QDir>

#include <QDebug>

namespace Common
{
    void deleteObject(QObject *object, int msec)
    {
        QTimer::singleShot( msec, object, SLOT( deleteLater() ) );
    }

    QString getThread(const QString &url)
    {
        QUrl u(url);

        if ( !u.isValid() )
        {
            return QString();
        }

        QStringList parts = u.path().split("/");
        int count = parts.count();

        if (count < 3)
        {
            return QString();
        }

        QString thread = parts.at(count - 1);

        if ( '.' == thread.at(thread.count() - 1) )
        {
            return QString();
        }

        thread.remove( QRegExp("\\..+") );
        return thread;
    }

    QString getBoard(const QString &url)
    {
        QUrl u(url);

        if ( !u.isValid() )
        {
            return QString();
        }

        return u.path().section("/", 0, 0, QString::SectionSkipEmpty);
    }

    QString getHost(const QString &url)
    {
        QUrl u(url);

        if ( !u.isValid() )
        {
            return QString();
        }

        QStringList domains = u.host().split(".");
        int count = domains.count();

        if (count < 2)
        {
            return QString();
        }

        return strFromList(domains, ".", count - 2, count - 1);
    }

    QString strFromList(const QStringList &list, const QString &sep,
                        int begin, int end)
    {
        int size = list.size();

        if ( (0 == size) || (begin > size) || (end > size) )
        {
            return QString();
        }

        QString str;

        if ( (begin < 0) && (end < 0) )
        {
            begin = 0;
            end = size - 1;
        }
        else if (begin < 0)
        {
            begin = 0;
        }
        else
        {
            end = size - 1;
        }

        for (int i = begin; i <= end; ++i)
        {
            str += (i < list.size() - 1) ? (list.at(i) + sep) : list.at(i);
        }

        return str;
    }

    QString getFileName(const QString &url, const QString &dir)
    {
        QString fileName = QFileInfo(url).fileName();

        if (QString() == dir)
        {
            return fileName;
        }
        else
        {
            fileName.prepend( QDir::separator() );
            fileName.prepend(dir);
            return fileName;
        }
    }

    QString constructLink(const QString &url)
    {
        if ( QUrl(url).scheme().isEmpty() )
        {
            return "<a href =\"file://" + url + "\">" + url + "</a>";
        }
        else
        {
            return "<a href=\"" + url + "\">" + url + "</a>";
        }
    }

    bool fileExists(const QString &fileName)
    {
        return QFileInfo(fileName).exists();
    }

    bool isThreadUrlValid(const QString &url)
    {
        if ( getThread(url).isEmpty() ||
             getBoard(url).isEmpty() ||
             getHost(url).isEmpty() )
        {
            return false;
        }

        return true;
    }
}
