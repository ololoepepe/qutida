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

#ifndef SAVETASK_H
#define SAVETASK_H

#include "src/core/download.h"

#include <QObject>
#include <QRunnable>
#include <QString>

class SaveTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    struct Parameters
    {
        Download *download;
        QString dir;
        bool replace;
    };

    enum Error
    {
        NoError = 0,
        DataError,
        FileError
    };

    struct Result
    {
        QString url;
        Error err;
        Download *download;
    };

    explicit SaveTask(const Parameters &param);

    void run();

signals:
    void finished(SaveTask::Result result);

private:
    Parameters parameters;

};

#endif // SAVETASK_H
