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

#include "src/core/rmdirtask.h"

#include <QObject>
#include <QDir>
#include <QString>
#include <QStringList>

RmdirTask::RmdirTask(const Parameters &param) :
    QObject(0)
{
    parameters = param;
}

//

void RmdirTask::run()
{
    Result result;
    result.dir = parameters.dir;
    QDir d(parameters.dir);

    if ( !d.exists() )
    {
        result.success = false;
        emit finished(result);
        return;
    }

    QStringList entryList = d.entryList(QDir::Files);

    foreach (QString entry, entryList)
    {
        QString entryAbsPath = d.absolutePath() + QDir::separator() + entry;

        if ( !QFile::remove(entryAbsPath) )
        {
            result.success = false;
            emit finished(result);
            return;
        }
    }

    result.success = d.rmdir(parameters.dir);
    emit finished(result);
}
