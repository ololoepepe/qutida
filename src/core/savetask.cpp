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

#include "src/core/savetask.h"
#include "src/common.h"

#include <QObject>
#include <QFile>

SaveTask::SaveTask(const Parameters &param) :
    QObject(0)
{
    parameters = param;
}

//

void SaveTask::run()
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

    if ( !file.open(QIODevice::WriteOnly) )
    {
        result.err = FileError;
        emit finished(result);
        return;
    }

    if ( -1 == file.write( parameters.download->data() ) )
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
