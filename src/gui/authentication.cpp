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

#include "authentication.h"
#include "src/tr.h"

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

Authentication::Authentication(QWidget *parent) :
    QDialog(parent)
{
    vLayout = new QVBoxLayout(this);
      hLayoutUser = new QHBoxLayout();
        labelUser = new QLabel(Tr::PD::labelUserText(), this);
        hLayoutUser->addWidget(labelUser);
        //
        lineEditUser = new QLineEdit(this);
        hLayoutUser->addWidget(lineEditUser);
      vLayout->addLayout(hLayoutUser);
      //
      hLayoutPassword = new QHBoxLayout();
        labelPassword = new QLabel(Tr::PD::labelPasswordText(), this);
        hLayoutPassword->addWidget(labelPassword);
        //
        lineEditPassword = new QLineEdit(this);
        hLayoutPassword->addWidget(lineEditPassword);
      vLayout->addLayout(hLayoutPassword);
      //
      hLayoutActions = new QHBoxLayout();
        buttonCancel = new QPushButton(Tr::buttonCancelText(), this);
        connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( accept() ) );
        hLayoutActions->addWidget(buttonCancel);
        //
        buttonOk = new QPushButton(Tr::buttonOkText(), this);
        connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
        hLayoutActions->addWidget(buttonOk);
      vLayout->addLayout(hLayoutActions);
}

//

QString Authentication::user() const
{
    return lineEditUser->text();
}

QString Authentication::password() const
{
    return lineEditPassword->text();
}
