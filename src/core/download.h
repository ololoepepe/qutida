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

#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QNetworkReply>
#include <QTimer>

class Download : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        InitialState,
        ActiveState,
        CompletedState,
        FailedState
    };

    static const int REPLY_TIMEOUT;
    static const int RETRY_DELAY;

    Download(const QString &url, bool aux = false,
             int attempts = 1, QObject *parent = 0);
    ~Download();

    QString url() const;
    bool isAuxiliary() const;
    const QByteArray &data() const;
    State state() const;

public slots:
    void start();
    void abort();

signals:
    void finished();

private:
    QString downloadUrl;
    bool isAux;
    QByteArray downloadData;
    QNetworkReply *networkReply;
    State downloadState;
    QTimer timer;
    int attemptsTotal;
    int attemptsUsed;

    void killReply();

private slots:
    void timerTimeout();
    void resetReply();
    void downloadProgress(qint64 bytesRecieved, qint64 bytesTotal);
    void replyFinished();

};

#endif // DOWNLOAD_H
