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

#ifndef PARCETASK_H
#define PARCETASK_H

#include "src/core/download.h"

#include <QObject>
#include <QRunnable>
#include <QString>
#include <QStringList>

class ParceTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    struct Parameters
    {
        Download *download;
        QString dir;
        QStringList extentions;
        bool external;
        bool replace;
        bool savePage;
    };

    enum Error
    {
        NoError = 0,
        DataError,
        ExtentionsError
    };

    struct Result
    {
        Error err;
        QStringList existingUrls;
        QStringList newUrls;
        QStringList auxUrls;
        Download *download;
    };

    static const QString PARCE_PATTERN_BEG;
    static const QString PARCE_PATTERN_END;
    static const QString PARCE_PATTERN_AUX;
    static const QString REMOVE_PATTERN;

    explicit ParceTask(const Parameters &param);

    void run();

signals:
    void finished(ParceTask::Result result);

private:
    Parameters parameters;

};

#endif // PARCETASK_H
