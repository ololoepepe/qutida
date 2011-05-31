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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/core/imageboardthread.h"
#include "src/mv/threadmodel.h"
#include "src/gui/infowidget.h"
#include "src/mv/categorymodel.h"
#include "src/gui/parametersdialog.h"
#include "src/gui/addthread.h"
#include "src/gui/threadparameters.h"

#include <QMainWindow>
#include <QString>
#include <QSplitter>
#include <QTreeView>
#include <QHeaderView>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QList>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QSignalMapper>
#include <QCloseEvent>
#include <QTranslator>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QPoint>
#include <QAuthenticator>
#include <QNetworkProxy>
#include <QStringList>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    static const QString GROUP_MAIN_WINDOW;
      static const QString KEY_GEOMETRY;
      static const QString KEY_VSPLITTER;
      static const QString KEY_HSPLITTER_TOP;
      static const QString SUB_GROUP_HEADER;
        static const QString KEY_SORT_ORDER;
        static const QString KEY_SORT_SECTION;
        static const QString ARRAY_SECTIONS;
          static const QString KEY_WIDTH;
          static const QString KEY_ENABLED;
          static const QString KEY_POSITION;


    explicit MainWindow(ThreadModel *threadModel, CategoryModel *categoryModel,
                        QWidget *parent = 0);

public slots:
    void callAddThreadDialog(const QStringList &urlList);
    void setVisibility(bool visible);

signals:
    void requestAddThread(const ImageboardThread::Parameters &param,
                          bool start);
    void requestBackup(const QString fileName);
    void requestRemoveThread(const QList<int> &indexes, bool);
    void requestStartThread(int index);
    void requestStopThread(int index);
    void requestOpenDir(int index);
    void requestOpenUrl(int index);
    void requestSetObservedThread(int index, InfoWidget *widget);
    void requestModifyParameters(const QList<int> &indexes,
                                 const ImageboardThread::Modifiable &modParam);
    void requestRetranslate();
    void requestWriteSettings();

protected:
    void closeEvent(QCloseEvent *e);

private:
    QSplitter *vSplitter;
      QSplitter *hSplitterTop;
        QTreeView *treeViewCategories;
          QItemSelectionModel *selectionModelCategories;
        QTreeView *treeViewThreads;
          QHeaderView *headerThreads;
          QItemSelectionModel *selectionModelThreads;
      InfoWidget *infoWidget;
    //menuBar()
      QMenu *menuFile;
        QAction *actAdd;
        //separator
        QAction *actBackup;
        //separator
        QAction *actExit;
      QMenu *menuEdit;
        QAction *actParameters;
      QMenu *menuThread;
        QAction *actOpenDir;
        QAction *actOpenUrl;
        //separator
        QAction *actStop;
        QAction *actStart;
        //separator
        QAction *actThreadParameters;
        //separator
        QAction *actRemove;
      QMenu *menuView;
        QAction *actToolBar;
        //separator
        QMenu *menuColumns;
          QList<QAction*> actColumnList;
          QSignalMapper columnMapper;
      QMenu *menuHelp;
        QAction *actHomepage;
        //separator
        QAction *actAbout;
        QAction *actAboutQt;
    QToolBar *toolBar;
      //actAdd
      //actRemove
      //separator
      //actStop
      //actStart
      //separator
      //actParameters
    QSystemTrayIcon *trayIcon;
      QMenu *contextMenuTray;
        QAction *actShowHide;
        //separator
        //actExit
    QTranslator translator;
    ParametersDialog::CommonParameters commonParam;
    bool finalClose;
    AddThread *addThreadDialog;
    QList<ImageboardThread*> selectedThreads;
    ImageboardThread *currentThread;
    bool lockInfoWidget;

    void retranslate(bool initial = false);
    void readSettings();
    void writeSettings();
    int getCurrentIndex();
    QList<int> getSelectedIndexes();
    void trySetProxy(const ParametersDialog::ProxySettings &proxySettings);

private slots:
    void setSelectedThreads();
    void restoreSelectedThreads();
    void addRequested();
    void backupRequested();
    void exitRequested();
    void parametersRequested();
    void openDirRequested();
    void openUrlRequested();
    void stopRequested();
    void startRequested();
    void threadParametersRequested();
    void removeRequested();
    void toolBarRequested(bool visible);
    void columnRequested(int index);
    void homepageRequested();
    void aboutRequested();
    void aboutQtRequested();
    void showHideRequested();
    void proxyAuthenticationRequired (const QNetworkProxy &proxy,
                                      QAuthenticator *authenticator);
    void threadModelDataChanged(const QModelIndex &topLeft);
    void threadsSelectionChanged(const QItemSelection &selected);
    void categoriesSelectionChanged();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void treeViewThreadsMenuRequested(const QPoint &point);
    void headerViewThreadsMenuRequested(const QPoint &point);

};

#endif // MAINWINDOW_H
