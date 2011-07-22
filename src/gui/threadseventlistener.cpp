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

#include "threadseventlistener.h"

#include <QObject>
#include <QEvent>
#include <QKeyEvent>

ThreadsEventListener::ThreadsEventListener(QObject *parent) :
    QObject(parent)
{
}

//

bool ThreadsEventListener::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->modifiers() == Qt::ControlModifier)
        {
            switch ( keyEvent->key() )
            {
            case Qt::Key_A:
                emit requestAdd();
                break;
            case Qt::Key_O:
                emit requestOpenDir();
                break;
            case Qt::Key_U:
                emit requestOpenUrl();
                break;
            case Qt::Key_S:
                emit requestStop();
                break;
            case Qt::Key_D:
                emit requestStart();
                break;
            case Qt::Key_P:
                emit requestThreadParameters();
                break;
            default:
                break;
            }
        }
        else if (keyEvent->modifiers() == Qt::NoModifier)
        {
            if (keyEvent->key() == Qt::Key_Delete)
                emit requestRemove();
        }

        return true;
    }
    else
    {
        return QObject::eventFilter(object, event);
    }
}
