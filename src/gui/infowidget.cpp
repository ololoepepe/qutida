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
                    SIGNAL( infoChanged(ImageboardThread::Info, QVariant) ),
                    this,
                    SLOT( threadInfoChanged(ImageboardThread::Info,
                                            QVariant) ) );

    observedThread = thread;

    if (observedThread)
        connect( observedThread,
                 SIGNAL( infoChanged(ImageboardThread::Info,QVariant) ),
                 this,
                 SLOT( threadInfoChanged(ImageboardThread::Info, QVariant) ) );

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
    getThreadData();
}

//

void InfoWidget::getThreadData()
{
    if (observedThread)
    {
        threadInfoChanged(
                    ImageboardThread::InfoProgress,
                    observedThread->dataForKey(
                        ImageboardThread::InfoProgress) );
        threadInfoChanged(
                    ImageboardThread::InfoThread,
                    observedThread->dataForKey(
                        ImageboardThread::InfoThread) );
        threadInfoChanged(
                    ImageboardThread::InfoBoard,
                    observedThread->dataForKey(
                        ImageboardThread::InfoBoard) );
        threadInfoChanged(
                    ImageboardThread::InfoHost,
                    observedThread->dataForKey(
                        ImageboardThread::InfoHost) );
        threadInfoChanged(
                    ImageboardThread::InfoDir,
                    observedThread->dataForKey(
                        ImageboardThread::InfoDir) );
        threadInfoChanged(
                    ImageboardThread::InfoUrl,
                    observedThread->dataForKey(
                        ImageboardThread::InfoUrl) );
        threadInfoChanged(
                    ImageboardThread::InfoStateExtended,
                    observedThread->dataForKey(
                        ImageboardThread::InfoStateExtended) );
        threadInfoChanged(
                    ImageboardThread::InfoTimeRest,
                    observedThread->dataForKey(
                        ImageboardThread::InfoTimeRest) );
        threadInfoChanged(
                    ImageboardThread::InfoFilesTotal,
                    observedThread->dataForKey(
                        ImageboardThread::InfoFilesTotal) );
        threadInfoChanged(
                    ImageboardThread::InfoFilesSaved,
                    observedThread->dataForKey(
                        ImageboardThread::InfoFilesSaved) );
        threadInfoChanged(
                    ImageboardThread::InfoFilesFailed,
                    observedThread->dataForKey(
                        ImageboardThread::InfoFilesFailed) );
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
    }
}

//

void InfoWidget::threadInfoChanged(ImageboardThread::Info key,
                                   const QVariant &data)
{
    switch (key)
    {
    case ImageboardThread::InfoProgress:
        progressBar->setValue( data.toInt() );
        break;
    case ImageboardThread::InfoThread:
        labelThreadData->setText( data.toString() );
        break;
    case ImageboardThread::InfoBoard:
        labelBoardData->setText( data.toString() );
        break;
    case ImageboardThread::InfoHost:
        labelHostData->setText( data.toString() );
        break;
    case ImageboardThread::InfoDir:
        labelDirData->setText( data.toString() );
        break;
    case ImageboardThread::InfoUrl:
        labelUrlData->setText( data.toString() );
        break;
    case ImageboardThread::InfoStateExtended:
        labelExtStateData->setText(
                    Tr::IT::threadExtendedState(
                        static_cast<ImageboardThread::ExtendedState>(
                            data.toInt() ) ) );
        break;
    case ImageboardThread::InfoTimeRest:
    {
        int rest = observedThread->dataForKey(
                    ImageboardThread::InfoTimeRest).toInt();
        bool enabled = observedThread->dataForKey(
                    ImageboardThread::InfoRestartEnabled).toBool();
        labelRestartData->setText(
                    Tr::IW::labelRestartDataText(rest, enabled) );
        break;
    }
    case ImageboardThread::InfoFilesTotal:
        labelFilesTotalData->setText(
                    Tr::IW::labelFilesDataText( data.toInt() ) );
        break;
    case ImageboardThread::InfoFilesSaved:
        labelFilesSavedData->setText(
                    Tr::IW::labelFilesDataText( data.toInt() ) );
        break;
    case ImageboardThread::InfoFilesFailed:
        labelFilesFailedData->setText(
                    Tr::IW::labelFilesDataText( data.toInt() ) );
        break;
    default:
        break;
    }
}
