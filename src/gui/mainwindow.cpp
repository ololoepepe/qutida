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

#include "src/gui/mainwindow.h"
#include "src/core/imageboardthread.h"
#include "src/mv/threadmodel.h"
#include "src/core/threadmanager.h"
#include "src/tr.h"
#include "src/gui/addthread.h"
#include "src/gui/parametersdialog.h"
#include "src/gui/removedialog.h"
#include "src/gui/infowidget.h"
#include "src/gui/threadparameters.h"
#include "src/mv/progressbardelegate.h"
#include "src/mv/categorymodel.h"
#include "src/common.h"
#include "src/core/networkaccessmanager.h"
#include "src/gui/authentication.h"

#include <QSplitter>
#include <QTreeView>
#include <QAction>
#include <QIcon>
#include <QToolBar>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QRect>
#include <QByteArray>
#include <QTranslator>
#include <QApplication>
#include <QAbstractItemView>
#include <QItemSelection>
#include <QModelIndex>
#include <QModelIndexList>
#include <QWidget>
#include <QPoint>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QRegExp>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QDateTime>
#include <QDir>
#include <QNetworkProxy>
#include <QAuthenticator>

const QString MainWindow::GROUP_MAIN_WINDOW = "main_window";
  const QString MainWindow::KEY_GEOMETRY = "geometry";
  const QString MainWindow::KEY_VSPLITTER = "vsplitter";
  const QString MainWindow::KEY_HSPLITTER_TOP = "hsplitter_top";
  const QString MainWindow::SUB_GROUP_HEADER = "header";
    const QString MainWindow::KEY_SORT_ORDER = "sort_order";
    const QString MainWindow::KEY_SORT_SECTION = "sort_section";
    const QString MainWindow::ARRAY_SECTIONS = "sections";
      const QString MainWindow::KEY_WIDTH = "width";
      const QString MainWindow::KEY_ENABLED = "enabled";
      const QString MainWindow::KEY_POSITION = "position";

//

MainWindow::MainWindow(ThreadModel *threadModel, CategoryModel *categoryModel,
                       QWidget *parent) :
    QMainWindow(parent)
{
    vSplitter = new QSplitter(Qt::Vertical, this);
    setCentralWidget(vSplitter);
      hSplitterTop = new QSplitter(Qt::Horizontal, vSplitter);
        treeViewCategories = new QTreeView(hSplitterTop);
        treeViewCategories->setModel(categoryModel);
          selectionModelCategories = treeViewCategories->selectionModel();
          selectionModelCategories->setCurrentIndex(
                      categoryModel->index( 0, 0, categoryModel->index(0, 0) ),
                      QItemSelectionModel::SelectCurrent);
          connect( selectionModelCategories,
                   SIGNAL(selectionChanged( QItemSelection, QItemSelection) ),
                   this,
                   SLOT( categoriesSelectionChanged() ) );
        treeViewCategories->expandAll();
        //
        treeViewThreads = new QTreeView(hSplitterTop);
        treeViewThreads->setModel(threadModel);
          connect( threadModel,
                   SIGNAL( dataChanged(QModelIndex, QModelIndex) ),
                   this, SLOT( threadModelDataChanged(QModelIndex) ) );
        treeViewThreads->setItemDelegateForColumn(
                    ImageboardThread::InfoProgress,
                    new ProgressBarDelegate(threadModel) );
        treeViewThreads->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeViewThreads->setSelectionMode(
                    QAbstractItemView::ExtendedSelection);
        treeViewThreads->setContextMenuPolicy(Qt::CustomContextMenu);
        treeViewThreads->setAlternatingRowColors(true);
        treeViewThreads->setRootIsDecorated(false);
        treeViewThreads->setSortingEnabled(true);
        connect( treeViewThreads,
                 SIGNAL( customContextMenuRequested(QPoint) ),
                 this, SLOT( treeViewThreadsMenuRequested(QPoint) ) );
          headerThreads = treeViewThreads->header();
          headerThreads->setContextMenuPolicy(Qt::CustomContextMenu);
          connect( headerThreads,
                   SIGNAL( sortIndicatorChanged(int, Qt::SortOrder) ),
                   this,
                   SLOT( threadsSortIndicatorChanged(int, Qt::SortOrder) ) );
          connect( headerThreads, SIGNAL( customContextMenuRequested(QPoint) ),
                   this, SLOT( headerViewThreadsMenuRequested(QPoint) ) );
          //
          selectionModelThreads = treeViewThreads->selectionModel();
          connect( selectionModelThreads,
                   SIGNAL( selectionChanged(QItemSelection, QItemSelection) ),
                   this,
                   SLOT( threadsSelectionChanged(QItemSelection) ) );
      infoWidget = new InfoWidget(vSplitter);
    //menuBar
      menuBar()->setNativeMenuBar(Qt::AA_DontUseNativeMenuBar);
      menuFile = menuBar()->addMenu( Tr::MW::menuFileTitle() );
        actAdd = new QAction(QIcon(":/res/ico/add.png"),
                             Tr::MW::actAddText(), this);
        connect( actAdd, SIGNAL( triggered() ), this, SLOT( addRequested() ) );
        menuFile->addAction(actAdd);
        //
        menuFile->addSeparator();
        //
        actBackup = new QAction(QIcon(":/res/ico/database.png"),
                                Tr::MW::actBackupText(), this);
        connect( actBackup, SIGNAL( triggered() ),
                 this, SLOT( backupRequested() ) );
        menuFile->addAction(actBackup);
        //
        menuFile->addSeparator();
        //
        actExit = new QAction(Tr::MW::actExitText(), this);
        connect( actExit, SIGNAL( triggered() ),
                 this, SLOT( exitRequested() ) );
        menuFile->addAction(actExit);
      menuEdit = menuBar()->addMenu( Tr::MW::menuEditTitle() );
        actParameters = new QAction(QIcon(":/res/ico/gear.png"),
                                    Tr::MW::actParametersText(), this);
        connect( actParameters, SIGNAL( triggered() ),
                 this, SLOT( parametersRequested() ) );
        menuEdit->addAction(actParameters);
      menuThread = menuBar()->addMenu( Tr::MW::menuThreadTitle() );
      menuThread->setEnabled(false);
        actOpenDir = new QAction(Tr::MW::actOpenDirText(), this);
        connect( actOpenDir, SIGNAL( triggered() ),
                 this, SLOT( openDirRequested() ) );
        menuThread->addAction(actOpenDir);
        //
        actOpenUrl = new QAction(QIcon(":/res/ico/earth.png"),
                                 Tr::MW::actOpenUrlText(), this);
        connect( actOpenUrl, SIGNAL( triggered() ),
                 this, SLOT( openUrlRequested() ) );
        menuThread->addAction(actOpenUrl);
        //
        menuThread->addSeparator();
        //
        actStop = new QAction(QIcon(":/res/ico/stop.png"),
                          Tr::MW::actStopText(), this);

        actStop->setEnabled(false);
        connect( actStop, SIGNAL( triggered() ),
                 this, SLOT( stopRequested() ) );
        menuThread->addAction(actStop);
        //
        actStart = new QAction(QIcon(":/res/ico/arrow-down.png"),
                          Tr::MW::actStartText(), this);
        actStart->setEnabled(false);
        connect( actStart, SIGNAL( triggered() ),
                 this, SLOT( startRequested() ) );
        menuThread->addAction(actStart);
        //
        menuThread->addSeparator();
        //
        actThreadParameters = new QAction(QIcon(":res/ico/gear.png"),
                                          Tr::MW::actThreadParametersText(),
                                          this);
        connect( actThreadParameters, SIGNAL( triggered() ),
                 this, SLOT( threadParametersRequested() ) );
        menuThread->addAction(actThreadParameters);
        //
        menuThread->addSeparator();
        //
        actRemove = new QAction(QIcon(":/res/ico/remove.png"),
                          Tr::MW::actRemoveText(), this);
        actRemove->setEnabled(false);
        connect( actRemove, SIGNAL( triggered() ),
                 this, SLOT( removeRequested() ) );
        menuThread->addAction(actRemove);
      menuView = menuBar()->addMenu( Tr::MW::menuViewTitle() );
        actToolBar = new QAction(Tr::MW::actToolBarText(), this);
        actToolBar->setCheckable(true);
        actToolBar->setChecked(true);
        connect( actToolBar, SIGNAL( triggered(bool) ),
                 this, SLOT( toolBarRequested(bool) ) );
        menuView->addAction(actToolBar);
        //
        menuView->addSeparator();
        //
        menuColumns = new QMenu(Tr::MW::menuColumnsTitle(), this);
          for (int i = ImageboardThread::InfoIteratorFirst;
               i <= ImageboardThread::InfoIteratorLast; ++i)
          {
              QAction *actColumn = new QAction(
                          Tr::IT::threadModelHeader(
                              static_cast<ImageboardThread::Info>(i) ),
                          this);
              actColumn->setCheckable(true);
              actColumn->setChecked(true);
              actColumnList << actColumn;
              connect( actColumn, SIGNAL( triggered() ),
                       &columnMapper, SLOT( map() ) );
              columnMapper.setMapping(actColumn, i);
              menuColumns->addAction(actColumn);
          }

          connect( &columnMapper, SIGNAL( mapped(int) ),
                   this, SLOT( columnRequested(int) ) );
        menuView->addMenu(menuColumns);
        //
      menuHelp = menuBar()->addMenu( Tr::MW::menuHelpTitle() );
        actHomepage = new QAction(Tr::MW::actHomepageText(), this);
        connect( actHomepage, SIGNAL( triggered() ),
                 this, SLOT( homepageRequested() ) );
        menuHelp->addAction(actHomepage);
        //
        menuHelp->addSeparator();
        //
        actAbout = new QAction(QIcon(":/res/ico/anonymous.png"),
                               Tr::MW::actAboutText(), this);
        connect( actAbout, SIGNAL( triggered() ),
                 this, SLOT( aboutRequested() ) );
        menuHelp->addAction(actAbout);
        //
        actAboutQt = new QAction(QIcon(":/res/ico/qt-logo.png"),
                                 Tr::MW::actAboutQtText(), this);
        connect( actAboutQt, SIGNAL( triggered() ),
                 this, SLOT( aboutQtRequested() ) );
        menuHelp->addAction(actAboutQt);
    toolBar = new QToolBar( Tr::MW::actToolBarText() );
    toolBar->setMovable(false);
    connect( toolBar, SIGNAL( visibilityChanged(bool) ),
             this, SLOT (toolBarRequested(bool) ) );
    addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar->addAction(actAdd);
        toolBar->addAction(actRemove);
        toolBar->addSeparator();
        toolBar->addAction(actStop);
        toolBar->addAction(actStart);
        toolBar->addSeparator();
        toolBar->addAction(actParameters);
    trayIcon = new QSystemTrayIcon(QIcon(":/res/ico/anonymous.png"), this);
    trayIcon->setToolTip(Common::APP_NAME);
      connect( trayIcon,
               SIGNAL( activated(QSystemTrayIcon::ActivationReason) ),
               this,
               SLOT( trayIconActivated(QSystemTrayIcon::ActivationReason) ) );
      contextMenuTray = new QMenu(this);
        actShowHide = new QAction(Tr::MW::actShowHideText(true), this);
        connect( actShowHide, SIGNAL( triggered() ),
                 this, SLOT( showHideRequested() ) );
        contextMenuTray->addAction(actShowHide);
        //
        contextMenuTray->addSeparator();
        //
        contextMenuTray->addAction(actExit);
      trayIcon->setContextMenu(contextMenuTray);
    trayIcon->show();
    //
    readSettings();
    finalClose = false;
    connect( NetworkAccessManager::instance(),
             SIGNAL( proxyAuthenticationRequired(QNetworkProxy,
                                                 QAuthenticator*) ),
             this, SLOT( proxyAuthenticationRequired(QNetworkProxy,
                                                     QAuthenticator*) ) );
    addThreadDialog = 0;
}

//

void MainWindow::callAddThreadDialog(const QStringList &urlList)
{
    if ( !this->isVisible() )
        showHideRequested();

    if (!addThreadDialog)
    {
        addThreadDialog = new AddThread(urlList, this);
        addThreadDialog->setWindowTitle( Tr::MW::dialogAddThreadCaption() );
        addThreadDialog->exec();

        if (addThreadDialog->result() == QDialog::Accepted)
        {
            bool start = addThreadDialog->start();
            int count = addThreadDialog->parameters().count();

            if (count > 0)
                selectionModelThreads->clearSelection();

            for (int i = 0; i < count; ++i)
                emit requestAddThread(addThreadDialog->parameters().at(i), start);
        }

        addThreadDialog->deleteLater();
        addThreadDialog = 0;
        emit requestSortThreads( headerThreads->sortIndicatorSection(),
                                 headerThreads->sortIndicatorOrder() );
    }
    else
    {
        addThreadDialog->appendList(urlList);
        addThreadDialog->activateWindow();
    }
}

//

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (commonParam.minimize && !finalClose)
    {
        this->hide();
        e->ignore();
    }
    else
    {
        if (commonParam.exitConfirmation)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle( Tr::MW::msgExitConfirmationTitle() );
            msgBox.setText( Tr::MW::msgExitConfirmationText() );
            msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);

            if (msgBox.exec() != QMessageBox::Ok)
            {
                e->ignore();
                finalClose = false;
                return;
            }
        }

        writeSettings();
        trayIcon->hide();
        e->accept();
    }
}

//

void MainWindow::retranslate(bool initial)
{
    qApp->removeTranslator(&translator);
    translator.load(Common::APP_NAME + "_" +
                    ParametersDialog::suffixForLanguage(commonParam.language),
                    ":/res/loc");
    qApp->installTranslator(&translator);
    menuFile->setTitle( Tr::MW::menuFileTitle() );
    menuEdit->setTitle( Tr::MW::menuEditTitle() );
    menuThread->setTitle( Tr::MW::menuThreadTitle() );
    menuView->setTitle( Tr::MW::menuViewTitle() );
    menuColumns->setTitle( Tr::MW::menuColumnsTitle() );
    menuHelp->setTitle( Tr::MW::menuHelpTitle() );
    actAdd->setText( Tr::MW::actAddText() );
    actBackup->setText( Tr::MW::actBackupText() );
    actExit->setText( Tr::MW::actExitText() );
    actParameters->setText( Tr::MW::actParametersText() );
    actOpenDir->setText( Tr::MW::actOpenDirText() );
    actOpenUrl->setText( Tr::MW::actOpenUrlText() );
    actStop->setText( Tr::MW::actStopText() );
    actStart->setText( Tr::MW::actStartText() );
    actThreadParameters->setText( Tr::MW::actThreadParametersText() );
    actRemove->setText( Tr::MW::actRemoveText() );
    actToolBar->setText( Tr::MW::actToolBarText() );
    actHomepage->setText( Tr::MW::actHomepageText() );
    actAbout->setText( Tr::MW::actAboutText() );
    actAboutQt->setText( Tr::MW::actAboutQtText() );
    actShowHide->setText( Tr::MW::actShowHideText( this->isVisible() ) );

    for (int i = ImageboardThread::InfoIteratorFirst;
         i <= ImageboardThread::InfoIteratorLast; ++i)
        actColumnList.value(i)->setText(
                    Tr::IT::threadModelHeader(
                        static_cast<ImageboardThread::Info>(i) ) );

    toolBar->setWindowTitle( Tr::MW::actToolBarText() );
    actShowHide->setText( Tr::MW::actShowHideText(
                             initial ? true : this->isVisible() ) );
    emit requestRetranslate();
    infoWidget->retranslate();
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup(GROUP_MAIN_WINDOW);
      QRect geometry = settings.value(KEY_GEOMETRY).toRect();

      if (QRect() != geometry)
          this->setGeometry(geometry);

      QByteArray vsplitter = settings.value(KEY_VSPLITTER).toByteArray();

      if (QByteArray() != vsplitter)
          vSplitter->restoreState(vsplitter);

      QByteArray hsplitterTop =
              settings.value(KEY_HSPLITTER_TOP).toByteArray();

      if (QByteArray() != hsplitterTop)
          hSplitterTop->restoreState(hsplitterTop);

      settings.beginGroup(SUB_GROUP_HEADER);
        Qt::SortOrder order =
                ( settings.value(KEY_SORT_ORDER, true).toBool() ) ?
                    Qt::AscendingOrder : Qt::DescendingOrder;
        int index = settings.value(KEY_SORT_SECTION, 0).toInt();
        headerThreads->setSortIndicator(index, order);
        emit requestSortThreads(index, order); //maybe not nessesary
        int count = settings.beginReadArray(ARRAY_SECTIONS);

          for (int i = 0; i < count; ++i)
          {
              settings.setArrayIndex(i);
              int colWidth = settings.value(KEY_WIDTH, -1).toInt();

              if (colWidth > -1)
                  treeViewThreads->setColumnWidth(i, colWidth);

              actColumnList.value(i)->setChecked(
                          settings.value(KEY_ENABLED, true).toBool() );
              columnRequested(i);
              int position = settings.value(KEY_POSITION, -1).toInt();

              if (position > -1)
                  headerThreads->moveSection(headerThreads->visualIndex(i),
                                             position);
          }

        settings.endArray();
      settings.endGroup();
    settings.endGroup();
    commonParam = ParametersDialog::readCommonParameters(settings);
    this->retranslate(true);
    ParametersDialog::ProxySettings proxySettings =
            ParametersDialog::readProxySettings(settings);
    trySetProxy(proxySettings);
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.remove(GROUP_MAIN_WINDOW);
    settings.beginGroup(GROUP_MAIN_WINDOW);
      settings.setValue( KEY_GEOMETRY, this->geometry() );
      settings.setValue( KEY_VSPLITTER, vSplitter->saveState() );
      settings.setValue( KEY_HSPLITTER_TOP, hSplitterTop->saveState() );
      settings.beginGroup(SUB_GROUP_HEADER);
        settings.setValue( KEY_SORT_ORDER,
                           ( Qt::AscendingOrder ==
                             headerThreads->sortIndicatorOrder() ) );
        settings.setValue( KEY_SORT_SECTION,
                           headerThreads->sortIndicatorSection() );
        settings.beginWriteArray(ARRAY_SECTIONS);

          for (int i = 0; i < actColumnList.count(); ++i)
          {
              settings.setArrayIndex(i);
              settings.setValue( KEY_WIDTH, treeViewThreads->columnWidth(i) );
              settings.setValue( KEY_ENABLED,
                                 actColumnList.at(i)->isChecked() );
              settings.setValue( KEY_POSITION, headerThreads->visualIndex(i) );
          }

        settings.endArray();
      settings.endGroup();
    settings.endGroup();
    emit requestWriteSettings();
}

int MainWindow::getCurrentIndex()
{
    QModelIndex index = selectionModelThreads->currentIndex();

    if ( index.isValid() )
        return index.row();
    else
        return -1;
}

QList<int> MainWindow::getSelectedIndexes()
{
    QModelIndexList selectedList = selectionModelThreads->selectedIndexes();
    QList<int> selectedIndexes;

    for (int i = 0; i < selectedList.count(); ++i)
    {
        int row = selectedList.at(i).row();

        if ( !selectedIndexes.contains(row) )
            selectedIndexes << row;
    }

    return selectedIndexes;
}

void MainWindow::trySetProxy(
    const ParametersDialog::ProxySettings &proxySettings)
{
    QNetworkProxy proxy;

    if (ParametersDialog::proxyIsValid(proxySettings) && proxySettings.enabled)
    {
        proxy.setType(QNetworkProxy::HttpCachingProxy);
        proxy.setHostName(proxySettings.host);
        proxy.setPort(proxySettings.port);
        proxy.setUser(proxySettings.user);
        proxy.setPassword(proxySettings.password);
    }

    NetworkAccessManager::instance()->setProxy(proxy);
}

//

void MainWindow::addRequested()
{
    AddThread *dialog = new AddThread(this);
    dialog->setWindowTitle( Tr::MW::dialogAddThreadCaption() );
    dialog->exec();

    if (dialog->result() == QDialog::Accepted)
    {
        bool start = dialog->start();
        int count = dialog->parameters().count();

        if (count > 0)
            selectionModelThreads->clearSelection();

        for (int i = 0; i < count; ++i)
            emit requestAddThread(dialog->parameters().at(i), start);
    }

    dialog->deleteLater();
    emit requestSortThreads( headerThreads->sortIndicatorSection(),
                             headerThreads->sortIndicatorOrder() );
}

void MainWindow::backupRequested()
{
    if (treeViewThreads->model()->rowCount() < 1)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle( Tr::MW::msgNothingToBackupTitle() );
        msgBox.setText( Tr::MW::msgNothingToBackupText() );
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QString fileName =
            QFileDialog::getSaveFileName(
                this, Tr::MW::dialogBackupCaption(), QDir::currentPath() +
                QDir::separator() + Common::APP_NAME + "-backup-" +
                QDateTime::currentDateTime().toString("yyyy.MM.dd-hh.mm.ss") );

    if ( !fileName.isEmpty() )
        emit requestBackup(fileName);
}

void MainWindow::exitRequested()
{
    finalClose = true;
    this->close();
}

void MainWindow::parametersRequested()
{
    ParametersDialog *dialog = new ParametersDialog(this);
    dialog->setWindowTitle( Tr::MW::dialogParametersCaption() );
    dialog->exec();

    if (dialog->result() == QDialog::Accepted)
    {
        QString previousLanguage = commonParam.language;
        commonParam = dialog->commonParameters();

        if (previousLanguage != commonParam.language)
        {
            this->retranslate(false);
        }

        ParametersDialog::ProxySettings proxySettings = dialog->proxy();
        trySetProxy(proxySettings);
    }

    dialog->deleteLater();
}

void MainWindow::openDirRequested()
{
    int index = getCurrentIndex();

    if (index >= 0)
        emit requestOpenDir(index);
}

void MainWindow::openUrlRequested()
{
    int index = getCurrentIndex();

    if (index >= 0)
        emit requestOpenUrl(index);
}

void MainWindow::stopRequested()
{
    QList<int> selected = getSelectedIndexes();

    for (int i = 0; i < selected.count(); ++i)
        emit requestStopThread( selected.at(i) );
}

void MainWindow::startRequested()
{
    QList<int> selected = getSelectedIndexes();

    for (int i = 0; i < selected.count(); ++i)
        emit requestStartThread( selected.at(i) );
}

void MainWindow::threadParametersRequested()
{
    ImageboardThread *current = infoWidget->observed();

    if (!current)
        return;

    ThreadParameters *dialog =
            new ThreadParameters(current->restartEnabled(),
                                 current->restartInterval(), this);
    dialog->setWindowTitle( Tr::MW::dialogThreadParametersCaption() );
    dialog->exec();

    if ( QDialog::Accepted == dialog->result() )
    {
        emit requestModifyRestart( getSelectedIndexes(),
                                  dialog->restartEnabled(),
                                  dialog->restartInterval() );
    }

    dialog->deleteLater();
}

void MainWindow::removeRequested()
{
    QList<int> selected = getSelectedIndexes();

    if ( selected.isEmpty() )
        return;

    RemoveDialog *dialog = new RemoveDialog(this);
    dialog->setWindowTitle( Tr::MW::dialogRemoveCaption() );
    dialog->exec();
    RemoveDialog::Decision decision = dialog->decision();

    if (RemoveDialog::Cancel == decision)
        return;

    bool del = (RemoveDialog::Delete == decision);
    selectionModelThreads->clearSelection();

    for (int i = selected.count() - 1; i >= 0; --i)
        emit requestRemoveThread(selected.at(i), del);
}

void MainWindow::toolBarRequested(bool visible)
{
    if (toolBar->isVisible() != visible)
        toolBar->setVisible(visible);

    if (actToolBar->isChecked() != visible)
        actToolBar->setChecked(visible);
}

void MainWindow::columnRequested(int index)
{
    if ( index < 0 || index >= actColumnList.count() )
        return;

    treeViewThreads->setColumnHidden( index,
                                      !actColumnList.at(index)->isChecked() );
}

void MainWindow::homepageRequested()
{
    QDesktopServices::openUrl( QUrl(Common::ORG_DOMAIN) );
}

void MainWindow::aboutRequested()
{
    QMessageBox::about( this, Tr::MW::msgAboutTitle(),
                        Tr::MW::msgAboutText() );
}

void MainWindow::aboutQtRequested()
{
    QMessageBox::aboutQt( this, Tr::MW::msgAboutQtTitle() );
}

void MainWindow::showHideRequested()
{
    this->setVisible( !this->isVisible() );
    actShowHide->setText( Tr::MW::actShowHideText( this->isVisible() ) );
}

void MainWindow::proxyAuthenticationRequired(const QNetworkProxy &proxy,
                                             QAuthenticator *authenticator)
{
    Authentication *dialog = new Authentication(this);
    dialog->setWindowTitle( Tr::MW::dialogAuthenticationCaption() );
    dialog->exec();

    if (dialog->result() == QDialog::Accepted)
    {
        authenticator->setUser( dialog->user() );
        authenticator->setPassword( dialog->password() );
    }

    dialog->deleteLater();
}

void MainWindow::threadModelDataChanged(const QModelIndex &topLeft)
{
    if (topLeft.column() == ImageboardThread::InfoStateExtended)
        categoriesSelectionChanged();
}

void MainWindow::threadsSortIndicatorChanged(int column, Qt::SortOrder order)
{
    selectionModelThreads->clearSelection(); //to be improved
    emit requestSortThreads(column, order);
}

void MainWindow::threadsSelectionChanged(const QItemSelection &selected)
{
    if (!infoWidget)
        return;

    bool anySelection = !getSelectedIndexes().isEmpty();
    menuThread->setEnabled(anySelection);
    actRemove->setEnabled(anySelection);
    actStop->setEnabled(anySelection);
    actStart->setEnabled(anySelection);

    QModelIndexList indexes = selected.indexes();

    if ( !indexes.isEmpty() )
    {
        emit requestSetObservedThread(indexes.at(indexes.count() - 1).row(),
                                      infoWidget);
    }
    else
    {
        indexes = selectionModelThreads->selectedIndexes();

        if ( !indexes.isEmpty() )
        {
            emit requestSetObservedThread(indexes.at(0).row(), infoWidget);
        }
        else
        {
            emit requestSetObservedThread(-1, infoWidget);
        }
    }
}

void MainWindow::categoriesSelectionChanged()
{
    TreeModel *threadModel = dynamic_cast<TreeModel*>( treeViewThreads->model() );
    CategoryModel *categoryModel = dynamic_cast<CategoryModel*>( treeViewCategories->model() );

    if (!threadModel || !categoryModel)
        return;

    QModelIndex index = selectionModelCategories->currentIndex();

    if ( !index.isValid() )
        return;

    QModelIndex parentIndex = index.parent();

    if ( !parentIndex.isValid() )
        return;

    Tr::CM::Category category = categoryModel->categoryForIndex(parentIndex);

    if (Tr::CM::CategState == category)
    {
        Tr::CM::SubCategory subCategory = static_cast<Tr::CM::SubCategory>(
                    categoryModel->pureData(index).toInt() );
        QList<ImageboardThread::ExtendedState> stateList;

        if (Tr::CM::SubWaiting == subCategory)
        {
            stateList << ImageboardThread::ExtWaiting;
        }
        else if (Tr::CM::SubError == subCategory)
        {
            stateList << ImageboardThread::ExtReadyFailed;
        }
        else if (Tr::CM::SubActive == subCategory)
        {
            stateList << ImageboardThread::ExtActiveDownloading;
            stateList << ImageboardThread::ExtActiveStarted;
        }
        else if (Tr::CM::SubReady == subCategory)
        {
            stateList << ImageboardThread::ExtReadyAborted;
            stateList << ImageboardThread::ExtReadyCompleted;
            stateList << ImageboardThread::ExtReadyInitial;
            stateList << ImageboardThread::ExtReadyNothing;
        }
        else
        {
            for (int i = 0; i < threadModel->rowCount(); ++i)
            {
                treeViewThreads->setRowHidden(i, QModelIndex(), false);
            }

            return;
        }

        for (int i = 0; i < threadModel->rowCount(); ++i)
        {
            ImageboardThread::ExtendedState state =
              static_cast<ImageboardThread::ExtendedState>(
                threadModel->pureData(
                  threadModel->index(
                      i, ImageboardThread::InfoStateExtended) ).toInt() );
            bool hide = true;

            for (int j = 0; j < stateList.count(); ++j)
            {
                if (stateList.at(j) == state)
                {
                    hide = false;
                    break;
                }
            }

            treeViewThreads->setRowHidden(i, QModelIndex(), hide);

            if (hide)
                selectionModelThreads->select(threadModel->index(i, 0),
                                              QItemSelectionModel::Deselect |
                                              QItemSelectionModel::Rows);
        }
    }
    else if (Tr::CM::CategHost == category)
    {
        QString categoryValue =
                categoryModel->pureData(index).toString();

        for (int i = 0; i < threadModel->rowCount(); ++i)
        {
            if (Common::getHost(
                  threadModel->pureData(
                    threadModel->index(
                      i, ImageboardThread::InfoUrl) ).toString() ) ==
                        categoryValue)
            {
                treeViewThreads->setRowHidden(i, QModelIndex(), false);
            }
            else
            {
                treeViewThreads->setRowHidden(i, QModelIndex(), true);
                selectionModelThreads->select(threadModel->index(i, 0),
                                              QItemSelectionModel::Deselect |
                                              QItemSelectionModel::Rows);
            }
        }
    }
    else if (Tr::CM::CategBoard == category)
    {
        QString categoryValue =
                categoryModel->pureData(index).toString();

        for (int i = 0; i < threadModel->rowCount(); ++i)
        {
            if (Common::getBoard(
                  threadModel->pureData(
                    threadModel->index(
                      i, ImageboardThread::InfoUrl) ).toString() ) ==
                        categoryValue)
            {
                treeViewThreads->setRowHidden(i, QModelIndex(), false);
            }
            else
            {
                treeViewThreads->setRowHidden(i, QModelIndex(), true);
                selectionModelThreads->select(threadModel->index(i, 0),
                                              QItemSelectionModel::Deselect |
                                              QItemSelectionModel::Rows);
            }
        }
    }
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
#ifndef Q_OS_MAC
    if (QSystemTrayIcon::Trigger == reason)
        showHideRequested();
#endif
}

void MainWindow::treeViewThreadsMenuRequested(const QPoint &point)
{
    if ( !getSelectedIndexes().isEmpty() )
        menuThread->popup( QWidget::mapToGlobal(point) );
}

void MainWindow::headerViewThreadsMenuRequested(const QPoint &point)
{
    menuColumns->popup( QWidget::mapToGlobal(point) );
}
