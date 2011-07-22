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

#include "globaleventlistener.h"

#include <QObject>
#include <QEvent>
#include <QKeyEvent>

GlobalEventListener::GlobalEventListener(QObject *parent) :
    QObject(parent)
{
}

//

bool GlobalEventListener::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() != QEvent::KeyPress)
        return QObject::eventFilter(object, event);

    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

    if (keyEvent->modifiers() != Qt::ControlModifier ||
            keyEvent->key() != Qt::Key_N)
        return QObject::eventFilter(object, event);

    emit requestAdd();
    return true;
}
