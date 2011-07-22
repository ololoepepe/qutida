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
    if (event->type() != QEvent::KeyPress)
        return QObject::eventFilter(object, event);

    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

    if (keyEvent->modifiers() == Qt::ControlModifier)
    {
        switch ( keyEvent->key() )
        {
        case Qt::Key_O:
            emit requestOpenDir();
            return true;
        case Qt::Key_U:
            emit requestOpenUrl();
            return true;
        case Qt::Key_S:
            emit requestStop();
            return true;
        case Qt::Key_D:
            emit requestStart();
            return true;
        case Qt::Key_P:
            emit requestThreadParameters();
            return true;
        default:
            return QObject::eventFilter(object, event);
        }
    }
    else if (keyEvent->modifiers() == Qt::NoModifier)
    {
        if (keyEvent->key() != Qt::Key_Delete)
            return  QObject::eventFilter(object, event);

        emit requestRemove();
        return true;
    }
    else
    {
        return QObject::eventFilter(object, event);
    }
}
