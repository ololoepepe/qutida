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

#include <QObject>
#include <QString>
#include <QTextCodec>
#include <QStringList>
#include <QRegExp>
#include <QUrl>
#include <QDir>

const QString ParceTask::PARCE_PATTERN_BEG = "(href=\"(http://)?\\S+\\.(";
const QString ParceTask::PARCE_PATTERN_END = ")\")+";
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

    if ( parameters.download->data().isEmpty() )
    {
        result.err = DataError;
        emit finished(result);
        Common::deleteObject(parameters.download);
        return;
    }

    if ( parameters.extentions.isEmpty() )
    {
        result.err = ExtentionsError;
        emit finished(result);
        Common::deleteObject(parameters.download);
        return;
    }

    QString content =
            QTextCodec::codecForHtml(
                parameters.download->data() )->toUnicode(
                parameters.download->data() );
    QStringList urlList;
    QRegExp parceMask(PARCE_PATTERN_BEG +
                      Common::strFromList(parameters.extentions, "|") +
                      PARCE_PATTERN_END);
    QRegExp removeMask(REMOVE_PATTERN);
    int pos = 0;

    while ( ( pos = parceMask.indexIn(content, pos) ) != -1 )
    {
        urlList << parceMask.cap(1).remove(removeMask);
        pos += parceMask.matchedLength();
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

    result.newUrls << urlList;
    result.err = NoError;
    Common::deleteObject(parameters.download);
    emit finished(result);
}
