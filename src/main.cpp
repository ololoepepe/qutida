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
#include "src/core/rmdirtask.h"
#include "src/core/imageboardthread.h"
#include "src/gui/infowidget.h"
#include "src/mv/categorymodel.h"

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

int main(int argc, char *argv[])
{
    const QString PREFIX = "-";
    const QString ARG_MULTIPLE = PREFIX + "multiple";
    const QString ARG_URLS = PREFIX + "urls";
    const QString ARG_DEFAULT = PREFIX + "default";
    const QString ARG_START = PREFIX + "start";

    QApplication app(argc, argv);
    QApplication::setOrganizationName(Common::ORG_NAME);
    QApplication::setOrganizationDomain(Common::ORG_DOMAIN);
    QApplication::setApplicationName(Common::APP_NAME);
    QApplication::setWindowIcon( QIcon(":/res/ico/anonymous.png") );
    QLocalServer localServer;
    bool serverStarted = localServer.listen(Common::APP_NAME);
    QStringList argList;
    QStringList urlList;
    bool fillingUrlList = false;

    for (int i = 0; i < argc; ++i)
    {
        QString arg(argv[i]);

        if ( arg.at(0) == PREFIX.at(0) )
        {
            argList << argv[i];

            if (fillingUrlList)
                fillingUrlList = false;
            else if (ARG_URLS == arg)
                fillingUrlList = true;
        }
        else if (fillingUrlList)
        {
            urlList << arg;
        }
    }

    if ( !serverStarted && !argList.contains(ARG_MULTIPLE) )
        return 0;

    qRegisterMetaType<ParceTask::Result>("ParceTask::Result");
    qRegisterMetaType<SaveTask::Result>("SaveTask::Result");
    qRegisterMetaType<RmdirTask::Result>("RmdirTask::Result");
    qRegisterMetaType<ImageboardThread*>("ImageboardThread*");
    qRegisterMetaType<InfoWidget*>("InfoWidget*");
    qRegisterMetaType<QModelIndex>("QModelIndex");
    QThreadPool::globalInstance()->setMaxThreadCount(10);
    ThreadManager threadManager(0);

    if ( argList.contains(ARG_DEFAULT) && !urlList.isEmpty() )
    {
        bool start = argList.contains(ARG_START);
        QSettings settings;
        settings.beginGroup(ParametersDialog::GROUP_PARAMETERS);
          settings.beginGroup(ThreadManager::SUB_GROUP_DEFAULT);
            ImageboardThread::Parameters param =
                    ImageboardThread::readParameters(settings);
          settings.endGroup();
        settings.endGroup();

        for (int i = 0; i < urlList.count(); ++i)
        {
            param.url = urlList.at(i);
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
                      SIGNAL( requestSortThreads(int, Qt::SortOrder) ),
                      &threadManager,
                      SLOT( requestSortThreads(int, Qt::SortOrder) ) );
    QObject::connect( mainWindow,
                      SIGNAL(requestSetObservedThread(int, InfoWidget*) ),
                      &threadManager,
                      SLOT( requestSetObservedThread(int, InfoWidget*) ) );
    QObject::connect( mainWindow,
                      SIGNAL( requestModifyRestart(QList<int>, bool, int) ),
                      &threadManager,
                      SLOT( requestModifyRestart(QList<int>, bool, int) ) );
    QObject::connect( mainWindow, SIGNAL( requestRetranslate() ),
                      &threadManager, SLOT( requestRetranslate() ) );
    QObject::connect( mainWindow, SIGNAL( requestWriteSettings() ),
                      &threadManager, SLOT( requestWriteSettings() ) );
    mainWindow->show();

    if ( !argList.contains(ARG_DEFAULT) && !urlList.isEmpty() )
    {
        mainWindow->callAddThreadDialog(urlList);
    }

    return app.exec();
}
