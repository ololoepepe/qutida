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

#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class NetworkAccessManager : public QNetworkAccessManager
{
public:
    static NetworkAccessManager *instance()
    {
        if (!managerInstance)
        {
            managerInstance = new NetworkAccessManager();
        }

        return managerInstance;
    }

private:
    explicit NetworkAccessManager() : QNetworkAccessManager(0) {}

    NetworkAccessManager(const NetworkAccessManager &other);
    NetworkAccessManager &operator=(const NetworkAccessManager &other);

    static NetworkAccessManager *managerInstance;

};

#endif // NETWORKACCESSMANAGER_H
