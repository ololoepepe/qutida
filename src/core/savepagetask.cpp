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

#include "src/core/savepagetask.h"
#include "src/common.h"

#include <QObject>
#include <QFile>
#include <QString>
#include <QTextCodec>
#include <QDir>

#include <QDebug>

const QString SavePageTask::AUX_FILES_DIR = "auxiliary";

SavePageTask::SavePageTask(const Parameters &param) :
    QObject(0)
{
    parameters = param;
}

//

void SavePageTask::run()
{
    Result result;
    result.download = parameters.download;

    if ( parameters.download->data().isEmpty() )
    {
        result.err = DataError;
        emit finished(result);
        return;
    }

    result.url = parameters.download->url();
    QFile file( Common::getFileName(result.url, parameters.dir) );

    if ( file.exists() )
    {
        if (parameters.replace)
        {
            if ( !file.remove() )
            {
                result.err = FileError;
                emit finished(result);
                return;
            }
        }
        else
        {
            result.err = NoError;
            emit finished(result);
            return;
        }
    }

    QTextCodec *codec = QTextCodec::codecForHtml(parameters.download->data(), QTextCodec::codecForName("UTF-8"));
    QString content = codec->toUnicode( parameters.download->data() );

    for (int i = 0; i < parameters.urls.count(); ++i)
    {
        if (QUrl(parameters.urls.at(i)).path().isEmpty() || QUrl(parameters.urls.at(i)).path().endsWith("/"))
            continue;
        if ( parameters.normalUrls.contains( parameters.urls.at(i) ) )
        {
            QString url = parameters.urls.at(i);
            QString path = QUrl(url).path();
            QString fn = Common::getFileName(url);
            content.replace(url, fn);
            url.remove(0, 5);
            content.replace(url, fn);
            url.remove(0, 1);
            content.replace(url, fn);
            content.replace(path, fn);
        }
        else
        {
            QString url = parameters.urls.at(i);
            QString path = QUrl(url).path();
            QString fn = Common::getFileName(url, AUX_FILES_DIR);
            content.replace(url, fn);
            url.remove(0, 5);
            content.replace(url, fn);
            url.remove(0, 1);
            content.replace(url, fn);
            content.replace(path, fn);
        }
    }

    if ( !file.open(QIODevice::WriteOnly) )
    {
        result.err = FileError;
        emit finished(result);
        return;
    }

    if (-1 == file.write(content.toUtf8()))
    {
        file.close();
        result.err = FileError;
        emit finished(result);
        return;
    }

    file.close();
    result.err = NoError;
    emit finished(result);
}
