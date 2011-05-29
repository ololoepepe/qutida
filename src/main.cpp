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

#include "src/core/threadmanager.h"
#include "src/gui/mainwindow.h"
#include "src/gui/parametersdialog.h"
#include "src/common.h"
#include "src/core/parcetask.h"
#include "src/core/savetask.h"
#include "src/core/savepagetask.h"
#include "src/core/rmdirtask.h"
#include "src/core/imageboardthread.h"
#include "src/gui/infowidget.h"
#include "src/mv/categorymodel.h"
#include "src/core/localserver.h"
#include "src/core/networkaccessmanager.h"
#include "src/gui/threadparameters.h"

#include <QApplication>
#include <QObject>
#include <QString>
#include <QMetaType>
#include <QList>
#include <QWidget>
#include <QThreadPool>
#include <QModelIndex>
#include <QLocalServer>
#include <QStringList>
#include <QSettings>
#include <QLocalSocket>
#include <QIODevice>
#include <QUrl>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName(Common::ORG_NAME);
    QApplication::setOrganizationDomain(Common::ORG_DOMAIN);
    QApplication::setApplicationName(Common::APP_NAME);
    QApplication::setWindowIcon( QIcon(":/res/ico/anonymous.png") );
    QSettings settings;
    NetworkAccessManager::instance()->setCache(0);
    LocalServer localServer;
    //LocalServer::removeServer(Common::APP_NAME);
    bool serverStarted = localServer.listen(Common::APP_NAME);
    QStringList argList;
    QStringList urlList;
    bool fillingUrlList = false;

    for (int i = 0; i < argc; ++i)
    {
        QString arg(argv[i]);

        if ( arg.at(0) == LocalServer::PREFIX.at(0) )
        {
            argList << arg;

            if (fillingUrlList)
                fillingUrlList = false;
            else if (LocalServer::ARG_URLS == arg)
                fillingUrlList = true;
        }
        else if (fillingUrlList)
        {
            urlList << QUrl::fromUserInput(arg).toString();
        }
    }

    if ( !serverStarted && !argList.contains(LocalServer::ARG_MULTIPLE) )
    {
        if ( argList.contains(LocalServer::ARG_URLS) )
        {
            QLocalSocket socket;
            socket.connectToServer(Common::APP_NAME, QIODevice::WriteOnly);

            if ( !socket.waitForConnected() )
                return 1;

            QByteArray data;

            for (int i = 0; i < argc; ++i)
            {
                data.append(argv[i]).append('\0');
            }

            if ( -1 == socket.write(data) )
                return 2;

            if ( !socket.waitForBytesWritten() )
                return 3;

            socket.disconnectFromServer();
        }

        return 0;
    }

    qRegisterMetaType<ParceTask::Result>("ParceTask::Result");
    qRegisterMetaType<SaveTask::Result>("SaveTask::Result");
    qRegisterMetaType<SavePageTask::Result>("SavePageTask::Result");
    qRegisterMetaType<RmdirTask::Result>("RmdirTask::Result");
    qRegisterMetaType<ImageboardThread*>("ImageboardThread*");
    qRegisterMetaType<InfoWidget*>("InfoWidget*");
    qRegisterMetaType<QModelIndex>("QModelIndex");
    qRegisterMetaType<ImageboardThread::Modifiable>("ImageboardThread::Modifiable");
    QThreadPool::globalInstance()->setMaxThreadCount(10);
    ThreadManager threadManager(0);
    QObject::connect( &localServer,
             SIGNAL( addThreadSilent(ImageboardThread::Parameters, bool) ),
             &threadManager,
             SLOT( requestAddThread(ImageboardThread::Parameters, bool) ) );

    if ( argList.contains(LocalServer::ARG_DEFAULT) && !urlList.isEmpty() )
    {
        bool start = argList.contains(LocalServer::ARG_START);
        settings.beginGroup(ParametersDialog::GROUP_PARAMETERS);
          settings.beginGroup(ThreadManager::SUB_GROUP_DEFAULT);
            ImageboardThread::Parameters param =
                    ImageboardThread::readParameters(settings);
          settings.endGroup();
        settings.endGroup();

        for (int i = 0; i < urlList.count(); ++i)
        {
            param.url = urlList.at(i);
            param.added = QDateTime::currentDateTime();
            threadManager.requestAddThread(param, start);
        }
    }

    MainWindow *mainWindow = new MainWindow(threadManager.threadModel(),
                                            threadManager.categoryModel(), 0);
    QObject::connect(
                mainWindow,
                SIGNAL( requestAddThread(ImageboardThread::Parameters, bool) ),
                &threadManager,
                SLOT( requestAddThread(ImageboardThread::Parameters, bool) ) );
    QObject::connect( mainWindow, SIGNAL( requestBackup(QString) ),
                      &threadManager, SLOT( requestBackup(QString) ) );
    QObject::connect( mainWindow, SIGNAL( requestRemoveThread(int, bool) ),
                      &threadManager, SLOT( requestRemoveThread(int, bool) ) );
    QObject::connect( mainWindow, SIGNAL( requestStartThread(int) ),
                      &threadManager, SLOT( requestStartThread(int) ) );
    QObject::connect( mainWindow, SIGNAL( requestStopThread(int) ),
                      &threadManager, SLOT( requestStopThread(int) ) );
    QObject::connect( mainWindow, SIGNAL( requestOpenDir(int) ),
                      &threadManager, SLOT( requestOpenDir(int) ) );
    QObject::connect( mainWindow, SIGNAL( requestOpenUrl(int) ),
                      &threadManager, SLOT( requestOpenUrl(int) ) );
    QObject::connect( mainWindow,
                      SIGNAL(requestSetObservedThread(int, InfoWidget*) ),
                      &threadManager,
                      SLOT( requestSetObservedThread(int, InfoWidget*) ) );
    QObject::connect( mainWindow,
                      SIGNAL( requestModifyParameters(
                                 QList<int>, ImageboardThread::Modifiable) ),
                      &threadManager,
                      SLOT( requestModifyParameters(
                               QList<int>, ImageboardThread::Modifiable) ) );
    QObject::connect( mainWindow, SIGNAL( requestRetranslate() ),
                      &threadManager, SLOT( requestRetranslate() ) );
    QObject::connect( mainWindow, SIGNAL( requestWriteSettings() ),
                      &threadManager, SLOT( requestWriteSettings() ) );
    QObject::connect( &localServer, SIGNAL( addThreads(QStringList) ),
                      mainWindow, SLOT( callAddThreadDialog(QStringList) ) );
    ParametersDialog::CommonParameters commonParam =
            ParametersDialog::readCommonParameters(settings);
    mainWindow->setVisibility(!commonParam.startMinimized);

    if ( !argList.contains(LocalServer::ARG_DEFAULT) && !urlList.isEmpty() )
        mainWindow->callAddThreadDialog(urlList);

    int err = app.exec();
    //LocalServer::removeServer(Common::APP_NAME);
    localServer.close();
    return err;
}
