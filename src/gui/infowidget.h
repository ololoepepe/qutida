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

#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include "src/core/imageboardthread.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>

class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWidget(QWidget *parent = 0);

    ImageboardThread *observed();
    void setObservedThread(ImageboardThread *thread = 0);
    void retranslate();

private:
    QVBoxLayout *vLayout;
      QProgressBar *progressBar;
      QHBoxLayout *hLayoutBlocks;
        QVBoxLayout *vLayoutKey1;
          QLabel *labelThreadKey;
          QLabel *labelBoardKey;
          QLabel *labelHostKey;
          QLabel *labelDirKey;
          QLabel *labelUrlKey;
        QVBoxLayout *vLayoutData1;
          QLabel *labelThreadData;
          QLabel *labelBoardData;
          QLabel *labelHostData;
          QLabel *labelDirData;
          QLabel *labelUrlData;
        //stretch
        QVBoxLayout *vLayoutKey2;
          QLabel *labelExtStateKey;
          QLabel *labelRestartKey;
          QLabel *labelFilesTotalKey;
          QLabel *labelFilesSavedKey;
          QLabel *labelFilesFailedKey;
        QVBoxLayout *vLayoutData2;
          QLabel *labelExtStateData;
          QLabel *labelRestartData;
          QLabel *labelFilesTotalData;
          QLabel *labelFilesSavedData;
          QLabel *labelFilesFailedData;
        //stretch
      //stretch
    //
    ImageboardThread *observedThread;

    void getThreadData();

private slots:
    void threadInfoChanged(ImageboardThread::Info key, const QVariant &data);

};

#endif // INFOWIDGET_H
