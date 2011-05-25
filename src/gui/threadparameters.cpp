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

ThreadParameters::ThreadParameters(Parameters param, QWidget *parent) :
    QDialog(parent)
{
    vLayout = new QVBoxLayout(this);
      hLayoutRestartEnabled = new QHBoxLayout();
        hLayoutRestartEnabled->addStretch();
        //
        checkBoxRestartEnabled =
                new QCheckBox(Tr::TP::checkBoxRestartEnabledText(), this);
        checkBoxRestartEnabled->setChecked(param.restartEnabled);
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
        spinBoxRestartInterval->setValue(param.restartInterval);
        hLayoutRestartInterval->addWidget(spinBoxRestartInterval);
      vLayout->addLayout(hLayoutRestartInterval);
      //
      hLayoutSavePage = new QHBoxLayout();
        hLayoutSavePage->addStretch();
        //
        checkBoxSavePage = new QCheckBox(Tr::AT::checkBoxSavePageText(), this);
        checkBoxRestartEnabled->setChecked(param.savePage);
        hLayoutSavePage->addWidget(checkBoxSavePage);
      vLayout->addLayout(hLayoutSavePage);
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

ThreadParameters::Parameters ThreadParameters::parameters() const
{
    Parameters param;
    param.restartEnabled = checkBoxRestartEnabled->isChecked();
    param.restartInterval = spinBoxRestartInterval->value();
    param.savePage = checkBoxSavePage->isChecked();
    return param;
}
