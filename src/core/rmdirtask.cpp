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
    result.success = rmdirRec(parameters.dir);
    emit finished(result);
}

//

bool RmdirTask::rmdirRec(const QString &dir)
{
    QDir d(dir);

    if ( !d.exists() )
        return true;

    QStringList fileList = d.entryList(QDir::Files | QDir::Hidden);
    QStringList directoryList = d.entryList(QDir::Dirs | QDir::Hidden);
    bool success = true;

    foreach (QString file, fileList)
        if ( !QFile::remove(d.absolutePath() + QDir::separator() + file) )
            success = false;

    foreach (QString directory, directoryList)
    {
        if ("." == directory || ".." == directory)
            continue;

        if ( !rmdirRec(d.absolutePath() + QDir::separator() + directory) )
            success = false;
    }

    if ( !d.rmdir(dir) )
        success = false;

    return success;
}
