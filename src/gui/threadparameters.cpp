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

#include "src/gui/threadparameters.h"
#include "src/tr.h"

#include <QDialog>
#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLayout>

ThreadParameters::ThreadParameters(bool enabled, int interval,
                                   QWidget *parent) :
    QDialog(parent)
{
    vLayout = new QVBoxLayout(this);
      hLayoutRestartEnabled = new QHBoxLayout();
        hLayoutRestartEnabled->addStretch();
        //
        checkBoxRestartEnabled =
                new QCheckBox(Tr::TP::checkBoxRestartEnabledText(), this);
        checkBoxRestartEnabled->setChecked(enabled);
        hLayoutRestartEnabled->addWidget(checkBoxRestartEnabled);
      vLayout->addLayout(hLayoutRestartEnabled);
      //
      hLayoutRestartInterval = new QHBoxLayout();
        hLayoutRestartInterval->addStretch();
        //
        labelRestartInterval =
                new QLabel(Tr::TP::labelRestartIntervalText(), this);
        hLayoutRestartInterval->addWidget(labelRestartInterval);
        //
        spinBoxRestartInterval = new QSpinBox(this);
        spinBoxRestartInterval->setMinimum(1);
        spinBoxRestartInterval->setMaximum(360);
        spinBoxRestartInterval->setValue(interval);
        hLayoutRestartInterval->addWidget(spinBoxRestartInterval);
      vLayout->addLayout(hLayoutRestartInterval);
      //
      hLayoutActions = new QHBoxLayout();
        hLayoutActions->addStretch();
        //
        buttonCancel = new QPushButton(Tr::buttonCancelText(), this);
        connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
        hLayoutActions->addWidget(buttonCancel);
        //
        buttonOk = new QPushButton(Tr::buttonOkText(), this);
        connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
        hLayoutActions->addWidget(buttonOk);
      vLayout->addLayout(hLayoutActions);
    vLayout->setSizeConstraint(QLayout::SetFixedSize);
}

//

bool ThreadParameters::restartEnabled() const
{
    return checkBoxRestartEnabled->isChecked();
}

int ThreadParameters::restartInterval() const
{
    return spinBoxRestartInterval->value();
}
