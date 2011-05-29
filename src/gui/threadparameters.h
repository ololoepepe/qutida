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

#ifndef THREADPARAMETERS_H
#define THREADPARAMETERS_H

#include "src/core/imageboardthread.h"

#include <QDialog>
#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

class ThreadParameters : public QDialog
{
    Q_OBJECT
public:
    explicit ThreadParameters(const ImageboardThread::Modifiable &modParam,
                              QWidget *parent = 0);

    ImageboardThread::Modifiable modParameters() const;

private:
    QVBoxLayout *vLayout;
      QHBoxLayout *hLayoutMode;
        //stretch
        QCheckBox *checkBoxSavePage;
        QCheckBox *checkBoxExternal;
        QCheckBox *checkBoxReplace;
      QHBoxLayout *hLayoutAttempt;
        //stretch
        QLabel *labelAttemptPage;
        QSpinBox *spinBoxAttemptPage;
        QLabel *labelAttemptFile;
        QSpinBox *spinBoxAttemptFile;
      QHBoxLayout *hLayoutExtentions;
        QLabel *labelExtentions;
        QLineEdit *lineEditExtentions;
        QPushButton *buttonResetExtentions;
      QHBoxLayout *hLayoutRestart;
        //stretch
        QCheckBox *checkBoxRestartEnabled;
        QSpinBox *spinBoxRestartInterval;
      QHBoxLayout *hLayoutActions;
        //stretch
        QPushButton *buttonCancel;
        QPushButton *buttonOk;

private slots:
    void buttonResetExtentionsClicked();

};

#endif // THREADPARAMETERS_H
