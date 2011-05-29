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

#include "src/gui/infowidget.h"
#include "src/core/threadinfo.h"
#include "src/common.h"
#include "src/tr.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QFont>

InfoWidget::InfoWidget(QWidget *parent) :
    QWidget(parent)
{
    QFont keyFont("Arial", 10, QFont::Bold, false);
    vLayout = new QVBoxLayout(this);
      progressBar = new QProgressBar(this);
      progressBar->setMinimum(0);
      progressBar->setMaximum(100);
      vLayout->addWidget(progressBar);
      //
      hLayoutBlocks = new QHBoxLayout();
        vLayoutKey1 = new QVBoxLayout();
          labelThreadKey = new QLabel(Tr::IW::labelThreadKeyText(), this);
          labelThreadKey->setFont(keyFont);
          vLayoutKey1->addWidget(labelThreadKey);
          //
          labelBoardKey = new QLabel(Tr::IW::labelBoardKeyText(), this);
          labelBoardKey->setFont(keyFont);
          vLayoutKey1->addWidget(labelBoardKey);
          //
          labelHostKey = new QLabel(Tr::IW::labelHostKeyText(), this);
          labelHostKey->setFont(keyFont);
          vLayoutKey1->addWidget(labelHostKey);
          //
          labelDirKey = new QLabel(Tr::IW::labelDirKeyText(), this);
          labelDirKey->setFont(keyFont);
          vLayoutKey1->addWidget(labelDirKey);
          //
          labelUrlKey = new QLabel(Tr::IW::labelUrlKeyText(), this);
          labelUrlKey->setFont(keyFont);
          vLayoutKey1->addWidget(labelUrlKey);
        hLayoutBlocks->addLayout(vLayoutKey1);
        //
        vLayoutData1 = new QVBoxLayout();
          labelThreadData = new QLabel(this);
          vLayoutData1->addWidget(labelThreadData);
          //
          labelBoardData = new QLabel(this);
          vLayoutData1->addWidget(labelBoardData);
          //
          labelHostData = new QLabel(this);
          vLayoutData1->addWidget(labelHostData);
          //
          labelDirData = new QLabel(this);
          labelDirData->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                                Qt::TextSelectableByKeyboard);
          vLayoutData1->addWidget(labelDirData);
          //
          labelUrlData = new QLabel(this);
          labelUrlData->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                                Qt::TextSelectableByKeyboard);
          vLayoutData1->addWidget(labelUrlData);
        hLayoutBlocks->addLayout(vLayoutData1);
        //
        hLayoutBlocks->addStretch();
        //
        vLayoutKey2 = new QVBoxLayout();
          labelExtStateKey = new QLabel(Tr::IW::labelExtStateKey(), this);
          labelExtStateKey->setFont(keyFont);
          vLayoutKey2->addWidget(labelExtStateKey);
          //
          labelRestartKey =
                  new QLabel(Tr::IW::labelRestartKeyText(), this);
          labelRestartKey->setFont(keyFont);
          vLayoutKey2->addWidget(labelRestartKey);
          //
          labelFilesTotalKey =
                  new QLabel(Tr::IW::labelFilesTotalKeyText(), this);
          labelFilesTotalKey->setFont(keyFont);
          vLayoutKey2->addWidget(labelFilesTotalKey);
          //
          labelFilesSavedKey =
                  new QLabel(Tr::IW::labelFilesSavedKeyText(), this);
          labelFilesSavedKey->setFont(keyFont);
          vLayoutKey2->addWidget(labelFilesSavedKey);
          //
          labelFilesFailedKey =
                  new QLabel(Tr::IW::labelFilesFailedKeyText(), this);
          labelFilesFailedKey->setFont(keyFont);
          vLayoutKey2->addWidget(labelFilesFailedKey);
        hLayoutBlocks->addLayout(vLayoutKey2);
        //
        vLayoutData2 = new QVBoxLayout();
          labelExtStateData = new QLabel(this);
          vLayoutData2->addWidget(labelExtStateData);
          //
          labelRestartData = new QLabel(this);
          vLayoutData2->addWidget(labelRestartData);
          //
          labelFilesTotalData = new QLabel(this);
          vLayoutData2->addWidget(labelFilesTotalData);
          //
          labelFilesSavedData = new QLabel(this);
          vLayoutData2->addWidget(labelFilesSavedData);
          //
          labelFilesFailedData = new QLabel(this);
          vLayoutData2->addWidget(labelFilesFailedData);
        hLayoutBlocks->addLayout(vLayoutData2);
        //
        hLayoutBlocks->addStretch();
        //
        vLayoutKey3 = new QVBoxLayout();
          labelSavePageKey = new QLabel(Tr::IW::labelSavePageKeyText(), this);
          labelSavePageKey->setFont(keyFont);
          vLayoutKey3->addWidget(labelSavePageKey);
          //
          labelFilesAuxTotalKey = new QLabel(
                      Tr::IW::labelFilesAuxTotalKeyText(), this);
          labelFilesAuxTotalKey->setFont(keyFont);
          vLayoutKey3->addWidget(labelFilesAuxTotalKey);
          //
          labelFilesAuxSavedKey = new QLabel(
                      Tr::IW::labelFilesAuxSavedKeyText(), this);
          labelFilesAuxSavedKey->setFont(keyFont);
          vLayoutKey3->addWidget(labelFilesAuxSavedKey);
          //
          labelFilesAuxFailedKey = new QLabel(
                      Tr::IW::labelFilesAuxFailedKeyText(), this);
          labelFilesAuxFailedKey->setFont(keyFont);
          vLayoutKey3->addWidget(labelFilesAuxFailedKey);
          //
          labelExtentionsKey = new QLabel(
                      Tr::AT::labelExtentionsText(), this);
          labelExtentionsKey->setFont(keyFont);
          vLayoutKey3->addWidget(labelExtentionsKey);
        hLayoutBlocks->addLayout(vLayoutKey3);
        //
        vLayoutData3 = new QVBoxLayout();
          labelSavePageData = new QLabel(this);
          vLayoutData3->addWidget(labelSavePageData);
          //
          labelFilesAuxTotalData = new QLabel(this);
          vLayoutData3->addWidget(labelFilesAuxTotalData);
          //
          labelFilesAuxSavedData = new QLabel(this);
          vLayoutData3->addWidget(labelFilesAuxSavedData);
          //
          labelFilesAuxFailedData = new QLabel(this);
          vLayoutData3->addWidget(labelFilesAuxFailedData);
          //
          labelExtentionsData = new QLabel(this);
          vLayoutData3->addWidget(labelExtentionsData);
        hLayoutBlocks->addLayout(vLayoutData3);
        //
        hLayoutBlocks->addStretch();
      vLayout->addLayout(hLayoutBlocks);
      //
      vLayout->addStretch();
    //
    observedThread = 0;
    getThreadData();
}

//

ImageboardThread *InfoWidget::observed()
{
    return observedThread;
}

void InfoWidget::setObservedThread(ImageboardThread *thread)
{
    if (observedThread)
        disconnect(observedThread,
                    SIGNAL( infoChanged(ThreadInfo::Enum, QVariant) ),
                    this,
                    SLOT( threadInfoChanged(ThreadInfo::Enum, QVariant) ) );

    observedThread = thread;

    if (observedThread)
    {
        connect( observedThread,
                 SIGNAL( infoChanged(ThreadInfo::Enum, QVariant) ),
                 this,
                 SLOT( threadInfoChanged(ThreadInfo::Enum, QVariant) ) );
        connect( observedThread, SIGNAL( destroyed() ),
                 this, SLOT( observedThreadDestroyed() ) );
    }

    getThreadData();
}

void InfoWidget::retranslate()
{
    labelThreadKey->setText( Tr::IW::labelThreadKeyText() );
    labelBoardKey->setText( Tr::IW::labelBoardKeyText() );
    labelHostKey->setText( Tr::IW::labelHostKeyText() );
    labelDirKey->setText( Tr::IW::labelDirKeyText() );
    labelUrlKey->setText( Tr::IW::labelUrlKeyText() );
    labelExtStateKey->setText( Tr::IW::labelExtStateKey() );
    labelRestartKey->setText( Tr::IW::labelRestartKeyText() );
    labelFilesTotalKey->setText( Tr::IW::labelFilesTotalKeyText() );
    labelFilesSavedKey->setText( Tr::IW::labelFilesSavedKeyText() );
    labelFilesFailedKey->setText( Tr::IW::labelFilesFailedKeyText() );
    labelSavePageKey->setText( Tr::IW::labelSavePageKeyText() );
    labelFilesAuxTotalKey->setText( Tr::IW::labelFilesAuxTotalKeyText() );
    labelFilesAuxSavedKey->setText( Tr::IW::labelFilesAuxSavedKeyText() );
    labelFilesAuxFailedKey->setText( Tr::IW::labelFilesAuxFailedKeyText() );
    labelExtentionsKey->setText( Tr::AT::labelExtentionsText() );

    if (observedThread)
    {
        labelFilesTotalData->setText(
                    Tr::IW::labelFilesDataText(
                        observedThread->dataForKey(
                            ThreadInfo::FilesTotal).toInt() ) );
        labelFilesSavedData->setText(
                    Tr::IW::labelFilesDataText(
                        observedThread->dataForKey(
                            ThreadInfo::FilesSaved).toInt() ) );
        labelFilesFailedData->setText(
                    Tr::IW::labelFilesDataText(
                        observedThread->dataForKey(
                            ThreadInfo::FilesFailed).toInt() ) );
        labelSavePageData->setText(
                    Tr::IW::labelSavePageDataText(
                        observedThread->dataForKey(
                            ThreadInfo::SavePage).toBool() ) );
        labelFilesAuxTotalData->setText(
                    Tr::IW::labelFilesDataText(
                        observedThread->dataForKey(
                            ThreadInfo::FilesAuxTotal).toInt() ) );
        labelFilesAuxSavedData->setText(
                    Tr::IW::labelFilesDataText(
                        observedThread->dataForKey(
                            ThreadInfo::FilesAuxSaved).toInt() ) );
        labelFilesAuxFailedData->setText(
                    Tr::IW::labelFilesDataText(
                        observedThread->dataForKey(
                            ThreadInfo::FilesAuxFailed).toInt() ) );
    }

    getThreadData();
}

//

void InfoWidget::getThreadData()
{
    if (observedThread)
    {
        threadInfoChanged( ThreadInfo::Progress,
                           observedThread->dataForKey(
                              ThreadInfo::Progress) );
        threadInfoChanged( ThreadInfo::Thread,
                           observedThread->dataForKey(
                              ThreadInfo::Thread) );
        threadInfoChanged( ThreadInfo::Board,
                           observedThread->dataForKey(
                              ThreadInfo::Board) );
        threadInfoChanged( ThreadInfo::Host,
                           observedThread->dataForKey(
                              ThreadInfo::Host) );
        threadInfoChanged( ThreadInfo::Dir,
                           observedThread->dataForKey(
                              ThreadInfo::Dir) );
        threadInfoChanged( ThreadInfo::Url,
                           observedThread->dataForKey(
                              ThreadInfo::Url) );
        threadInfoChanged( ThreadInfo::ExtendedState,
                           observedThread->dataForKey(
                              ThreadInfo::ExtendedState) );
        threadInfoChanged( ThreadInfo::TimeRest,
                           observedThread->dataForKey(
                              ThreadInfo::TimeRest) );
        threadInfoChanged( ThreadInfo::FilesTotal,
                           observedThread->dataForKey(
                              ThreadInfo::FilesTotal) );
        threadInfoChanged( ThreadInfo::FilesSaved,
                           observedThread->dataForKey(
                              ThreadInfo::FilesSaved) );
        threadInfoChanged( ThreadInfo::FilesFailed,
                           observedThread->dataForKey(
                              ThreadInfo::FilesFailed) );
        threadInfoChanged( ThreadInfo::SavePage,
                           observedThread->dataForKey(
                              ThreadInfo::SavePage) );
        threadInfoChanged( ThreadInfo::FilesAuxTotal,
                           observedThread->dataForKey(
                              ThreadInfo::FilesAuxTotal) );
        threadInfoChanged( ThreadInfo::FilesAuxSaved,
                           observedThread->dataForKey(
                              ThreadInfo::FilesAuxSaved) );
        threadInfoChanged( ThreadInfo::FilesAuxFailed,
                           observedThread->dataForKey(
                              ThreadInfo::FilesAuxFailed) );
        threadInfoChanged( ThreadInfo::Extentions,
                           observedThread->dataForKey(
                              ThreadInfo::Extentions) );
    }
    else
    {
        progressBar->setValue(0);
        labelThreadData->clear();
        labelBoardData->clear();
        labelHostData->clear();
        labelDirData->clear();
        labelUrlData->clear();
        labelExtStateData->clear();
        labelRestartData->clear();
        labelFilesTotalData->clear();
        labelFilesSavedData->clear();
        labelFilesFailedData->clear();
        labelSavePageData->clear();
        labelFilesAuxTotalData->clear();
        labelFilesAuxSavedData->clear();
        labelFilesAuxFailedData->clear();
        labelExtentionsData->clear();
    }
}

//

void InfoWidget::threadInfoChanged(ThreadInfo::Enum key, const QVariant &data)
{
    switch (key)
    {
    case ThreadInfo::Progress:
        progressBar->setValue( data.toInt() );
        break;
    case ThreadInfo::Thread:
        labelThreadData->setText( data.toString() );
        break;
    case ThreadInfo::Board:
        labelBoardData->setText( data.toString() );
        break;
    case ThreadInfo::Host:
        labelHostData->setText( data.toString() );
        break;
    case ThreadInfo::Dir:
        labelDirData->setText( data.toString() );
        break;
    case ThreadInfo::Url:
        labelUrlData->setText( data.toString() );
        break;
    case ThreadInfo::ExtendedState:
        labelExtStateData->setText(
                    Tr::IT::threadExtendedState(
                        static_cast<ImageboardThread::ExtendedState>(
                            data.toInt() ) ) );
        break;
    case ThreadInfo::TimeRest:
    {
        int rest = observedThread->dataForKey(
                    ThreadInfo::TimeRest).toInt();
        bool enabled = observedThread->dataForKey(
                    ThreadInfo::RestartEnabled).toBool();
        labelRestartData->setText(
                    Tr::IW::labelRestartDataText(rest, enabled) );
        break;
    }
    case ThreadInfo::FilesTotal:
        labelFilesTotalData->setText(
                    Tr::IW::labelFilesDataText( data.toInt() ) );
        break;
    case ThreadInfo::FilesSaved:
        labelFilesSavedData->setText(
                    Tr::IW::labelFilesDataText( data.toInt() ) );
        break;
    case ThreadInfo::FilesFailed:
        labelFilesFailedData->setText(
                    Tr::IW::labelFilesDataText( data.toInt() ) );
        break;
    case ThreadInfo::SavePage:
        labelSavePageData->setText(
                    Tr::IW::labelSavePageDataText( data.toBool() ) );
        break;
    case ThreadInfo::FilesAuxTotal:
        labelFilesAuxTotalData->setText(
                    Tr::IW::labelFilesDataText( data.toInt() ) );
        break;
    case ThreadInfo::FilesAuxSaved:
        labelFilesAuxSavedData->setText(
                    Tr::IW::labelFilesDataText( data.toInt() ) );
        break;
    case ThreadInfo::FilesAuxFailed:
        labelFilesAuxFailedData->setText(
                    Tr::IW::labelFilesDataText( data.toInt() ) );
        break;
    case ThreadInfo::Extentions:
        labelExtentionsData->setText(
                    Common::strFromList(data.toStringList(), " ") );
    default:
        break;
    }
}

void InfoWidget::observedThreadDestroyed()
{
    observedThread = 0;
    getThreadData();
}
