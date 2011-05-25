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

#ifndef IMAGEBOARDTHREAD_H
#define IMAGEBOARDTHREAD_H

#include "src/common.h"
#include "src/core/download.h"
#include "src/core/parcetask.h"
#include "src/core/savetask.h"
#include "src/core/savepagetask.h"
#include "src/core/rmdirtask.h"
#include "src/core/threadinfo.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include <QTimer>
#include <QSignalMapper>
#include <QSettings>
#include <QDateTime>

class ImageboardThread : public QObject
{
    Q_OBJECT
public:
    struct Parameters
    {
        QString url;
        QString directory;
        QDateTime added;
        QStringList extentions;
        bool external;
        bool replace;
        int attemptPage;
        int attemptFile;
        bool restartEnabled;
        int restartInterval;
        bool savePage;
    };

    enum State
    {
        ReadyState,
        ActiveState,
        WaitingState
    };

    enum ExtendedState
    {
        ExtReadyInitial,
        ExtReadyCompleted,
        ExtReadyNothing,
        ExtReadyFailed,
        ExtReadyAborted,
        ExtActiveStarted,
        ExtActiveDownloading,
        ExtWaiting
    };

    static const int TIMER_INTERVAL;

    static const QString DEF_EXTENTIONS_PLAIN;
    static const QStringList DEF_EXTENTIONS_LIST;
    static const Parameters DEF_PARAMETERS;

    static const QString KEY_URL;
    static const QString KEY_DIRECTORY;
    static const QString KEY_ADDED;
    static const QString KEY_EXTENTIONS;
    static const QString KEY_EXTERNAL;
    static const QString KEY_REPLACE;
    static const QString KEY_ATTEMPT_PAGE;
    static const QString KEY_ATTEMPT_FILE;
    static const QString KEY_RESTART_ENABLED;
    static const QString KEY_RESTART_INTERVAL;
    static const QString KEY_SAVE_PAGE;

    static QMap<ThreadInfo::Enum, QVariant> infoFromParameters(
        const Parameters &param)
    {
        QMap<ThreadInfo::Enum, QVariant> map;
        map.insert( ThreadInfo::Thread, Common::getThread(param.url) );
        map.insert( ThreadInfo::Board, Common::getBoard(param.url) );
        map.insert( ThreadInfo::Host, Common::getHost(param.url) );
        map.insert(ThreadInfo::ExtendedState, ExtReadyInitial);
        map.insert(ThreadInfo::ExtendedStatePrev, ExtReadyInitial);
        map.insert(ThreadInfo::Progress, 0);
        map.insert(ThreadInfo::Dir, param.directory);
        map.insert(ThreadInfo::Url, param.url);
        map.insert(ThreadInfo::Added, param.added);
        map.insert(ThreadInfo::RestartEnabled, param.restartEnabled);
        map.insert(ThreadInfo::RestartInterval, param.restartInterval);
        map.insert(ThreadInfo::TimeRest, -1);
        map.insert(ThreadInfo::FilesTotal, -1);
        map.insert(ThreadInfo::FilesSaved, -1);
        map.insert(ThreadInfo::FilesFailed, -1);
        map.insert(ThreadInfo::SavePage, param.savePage);
        map.insert(ThreadInfo::FilesAuxTotal, -1);
        map.insert(ThreadInfo::FilesAuxSaved, -1);
        map.insert(ThreadInfo::FilesAuxFailed, -1);
        return map;
    }

    static Parameters readParameters(
        QSettings &settings, const Parameters &defParam = DEF_PARAMETERS)
    {
        Parameters param;
        param.url = settings.value(KEY_URL, defParam.url).toString();
        param.directory =
                settings.value(KEY_DIRECTORY, defParam.directory).toString();
        param.added = settings.value(
                    KEY_ADDED, QDateTime::currentDateTime() ).toDateTime();
        param.extentions = settings.value(KEY_EXTENTIONS,
                                          defParam.extentions).toStringList();
        param.external =
                settings.value(KEY_EXTERNAL, defParam.external).toBool();
        param.replace = settings.value(KEY_REPLACE, defParam.replace).toBool();
        param.attemptPage =
                settings.value(KEY_ATTEMPT_PAGE, defParam.attemptPage).toInt();
        param.attemptFile =
                settings.value(KEY_ATTEMPT_FILE, defParam.attemptFile).toInt();
        param.restartEnabled =
                settings.value(KEY_RESTART_ENABLED,
                               defParam.restartEnabled).toBool();
        param.restartInterval =
                settings.value(KEY_RESTART_INTERVAL,
                               defParam.restartInterval).toInt();
        param.savePage = settings.value(KEY_SAVE_PAGE,
                                        defParam.savePage).toBool();
        return param;
    }

    static void writeParameters(QSettings &settings,
                                      const Parameters &param)
    {
        settings.setValue(KEY_URL, param.url);
        settings.setValue(KEY_DIRECTORY, param.directory);
        settings.setValue(KEY_ADDED, param.added);
        settings.setValue(KEY_EXTENTIONS, param.extentions);
        settings.setValue(KEY_EXTERNAL, param.external);
        settings.setValue(KEY_REPLACE, param.replace);
        settings.setValue(KEY_ATTEMPT_PAGE, param.attemptPage);
        settings.setValue(KEY_ATTEMPT_FILE, param.attemptFile);
        settings.setValue(KEY_RESTART_ENABLED, param.restartEnabled);
        settings.setValue(KEY_RESTART_INTERVAL, param.restartInterval);
        settings.setValue(KEY_SAVE_PAGE, param.savePage);
    }

    ImageboardThread(const Parameters &param, QObject *parent = 0);
    ~ImageboardThread();

    const Parameters &parameters() const;
    QString url() const;
    QString dir() const;
    bool restartEnabled() const;
    int restartInterval() const;
    bool savePage() const;
    ExtendedState extendedState() const;
    QVariant dataForKey(ThreadInfo::Enum key) const;

public slots:
    void startDownload();
    void stopDownload();
    void deleteWithFiles();
    void modifyRestart(bool enabled, int interval);
    void modifySavePage(bool enabled);

signals:
    void infoChanged(ThreadInfo::Enum key, const QVariant &data);

private:
    Parameters threadParameters;
    QMap<ThreadInfo::Enum, QVariant> infoMap;
    State threadState;
    ExtendedState previousState;
    QTimer timer;
    int timePassed;
    int filesTotal;
    int filesSaved;
    int filesFailed;
    int filesAuxTotal;
    int filesAuxSaved;
    int filesAuxFailed;
    QMap<QString, Download*> downloadMap;
    QSignalMapper downloadMapper;

    int progress() const;
    void addDownload(const QString &url, int attempts, bool aux = false);
    void setUpRestart();
    void checkCompletion();
    void changeInfo(ThreadInfo::Enum key, const QVariant &data);

private slots:
    void timerTimeout();
    void downloadFinished(const QString &url);
    void parceFinished(ParceTask::Result result);
    void saveFinished(SaveTask::Result result);
    void savePageFinished(SavePageTask::Result result);
    void rmdirFinished(RmdirTask::Result result);

};

#endif // IMAGEBOARDTHREAD_H
