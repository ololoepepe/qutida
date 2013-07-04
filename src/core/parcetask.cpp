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

#include "src/core/parcetask.h"
#include "src/common.h"
#include "src/core/savepagetask.h"

#include <QObject>
#include <QString>
#include <QTextCodec>
#include <QStringList>
#include <QRegExp>
#include <QUrl>
#include <QDir>

#include <QDebug>

const QString ParceTask::PARCE_PATTERN_BEG = "(href=\"(http://)?\\S+\\.(";
const QString ParceTask::PARCE_PATTERN_END = ")\")+";
const QString ParceTask::PARCE_PATTERN_AUX = "((http://)?\\S+/\\S+\\.\\w+(?=(\"|')))+";
const QString ParceTask::REMOVE_PATTERN = "href=|\"";

//

ParceTask::ParceTask(const Parameters &param) :
    QObject(0)
{
    parameters = param;
}

//

void ParceTask::run()
{
    Result result;
    result.download = parameters.download;

    if ( parameters.download->data().isEmpty() )
    {
        result.err = DataError;
        emit finished(result);
        return;
    }

    if ( parameters.extentions.isEmpty() )
    {
        result.err = ExtentionsError;
        emit finished(result);
        return;
    }

    QString content =
            QTextCodec::codecForHtml(
                parameters.download->data() )->toUnicode(
                parameters.download->data() );
    QStringList urlList;
    QRegExp parceMask;

    if ( parameters.extentions.contains("*") )
        parceMask.setPattern("(href=\"(https?://)?\\S+/\\d+\\.(\\S+)\")+");
    else
        parceMask.setPattern("(href=\"(https?://)?\\S+/\\d+\\.(" + Common::strFromList(parameters.extentions,
                                                                                  "|") + ")\")+");

    QRegExp removeMask("(href=|\"|src=)");
    int pos = 0;

    while ( ( pos = parceMask.indexIn(content, pos) ) != -1 )
    {
        urlList << parceMask.cap(1).remove(removeMask).replace("//", "/");
        QString &s = urlList.last();
        if (!s.contains("://"))
            s.replace(":/", "://");
        pos += parceMask.matchedLength();
    }

    urlList.removeDuplicates();

    if (parameters.download->url().contains("4chan.org"))
    {
        for (int i = 0; i < urlList.size(); ++i)
        {
            if (urlList.at(i).left(2) == "//")
                urlList[i].prepend("http:");
            else if (urlList.at(i).left(1) == "/")
                urlList[i].prepend("http:/");
        }
    }

    QStringList nameList;

    for (int i = 0; i < urlList.count(); ++i)
    {
        nameList << Common::getFileName( urlList.at(i) );
    }

    int i = nameList.count() - 1;

    while (i >= 0)
    {
        int j = i - 1;

        while (j >= 0)
        {
            if ( nameList.at(i) == nameList.at(j) )
            {
                urlList.removeAt(j);
                nameList.removeAt(j);
                --i;
            }

            --j;
        }

        --i;
    }

    QString scheme = QUrl( parameters.download->url() ).scheme();
    QString domain = Common::getHost( parameters.download->url() );

    for (int i = 0; i < urlList.count(); ++i)
    {
        if ( QUrl( urlList.at(i) ).scheme().isEmpty() )
        {
            urlList.replace( i, urlList.value(i).prepend(scheme +
                                                         QString("://") +
                                                         domain) );
        }
    }

    if (!parameters.external)
    {
        int i = urlList.count() - 1;

        while (i >= 0)
        {
            if (Common::getHost( urlList.at(i) ) != domain)
            {
                urlList.removeAt(i);
            }

            --i;
        }
    }
    if (!parameters.replace)
    {
        int i = urlList.count() - 1;
        while (i >= 0)
        {
            if ( QFileInfo( Common::getFileName(urlList.at(i),
                                                parameters.dir) ).exists() )
            {
                result.existingUrls << urlList.at(i);
                urlList.removeAt(i);
            }

            --i;
        }
    }

    if ( !urlList.isEmpty() )
    {
        QDir dir(parameters.dir);

        if ( !dir.exists() )
        {
            dir.mkpath(parameters.dir);
        }
    }

    if (parameters.savePage)
    {
        QStringList auxUrlList;
        QRegExp auxParceMask("((href|src)=\"(https?://)?\\S+\\.(\\S+)\")+");
        int pos = 0;

        while ( ( pos = auxParceMask.indexIn(content, pos) ) != -1 )
        {
            auxUrlList << auxParceMask.cap(1).remove(removeMask);;
            pos += auxParceMask.matchedLength();
        }

        auxUrlList.removeDuplicates();

        if (parameters.download->url().contains("4chan.org"))
        {
            for (int i = 0; i < auxUrlList.size(); ++i)
            {
                if (auxUrlList.at(i).left(2) == "//")
                    auxUrlList[i].prepend("http:");
                else if (auxUrlList.at(i).left(1) == "/")
                    auxUrlList[i].prepend("http:/");
            }
        }

        for (int i = 0; i < auxUrlList.count(); ++i)
        {
            if (auxUrlList.at(i).contains(".html") || auxUrlList.at(i).contains(".htm")
                    || auxUrlList.at(i).endsWith("/"))
                continue;
            QString d = Common::getHost( auxUrlList.at(i) );

            if (!urlList.contains(auxUrlList.at(i)) && (d == domain || QString() == d))
            {
                if ( QUrl( auxUrlList.at(i) ).scheme().isEmpty() )
                    auxUrlList.replace(i, auxUrlList.value(i).prepend(scheme + QString("://") + domain));
                result.auxUrls << auxUrlList.at(i);
            }
        }

        if ( !result.auxUrls.isEmpty() )
        {
            QDir dir(parameters.dir + QDir::separator() +
                     SavePageTask::AUX_FILES_DIR);

            if ( !dir.exists() )
            {
                dir.mkpath(parameters.dir + QDir::separator() +
                           SavePageTask::AUX_FILES_DIR);
            }
        }
    }

    result.newUrls << urlList;
    result.err = NoError;
    emit finished(result);
}
