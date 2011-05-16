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

#include "src/gui/removedialog.h"
#include "src/tr.h"

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLayout>
#include <QFont>
#include <QSizePolicy>

RemoveDialog::RemoveDialog(QWidget *parent) :
    QDialog(parent)
{
    QFont labelFont("Arial", 12, QFont::Bold, false);
    vLayout = new QVBoxLayout(this);
      labelWarning = new QLabel(Tr::RD::labelWarningText(), this);
      labelWarning->setFont(labelFont);
      vLayout->addWidget(labelWarning);
      //
      hLayoutActions = new QHBoxLayout();
        hLayoutActions->addStretch();
        //
        buttonCancel = new QPushButton(Tr::buttonCancelText(), this);
        buttonCancel->setSizePolicy(QSizePolicy::Expanding,
                                    QSizePolicy::Expanding);
        connect( buttonCancel, SIGNAL( clicked() ),
                 this, SLOT( buttonCancelClicked() ) );
        hLayoutActions->addWidget(buttonCancel);
        //
        buttonDelete = new QPushButton(QIcon(":res/ico/delete.png"),
                                       Tr::RD::buttonDeleteText(), this);
        connect( buttonDelete, SIGNAL( clicked() ),
                 this, SLOT( buttonDeleteClicked() ) );
        hLayoutActions->addWidget(buttonDelete);
        //
        buttonRemove = new QPushButton(QIcon(":res/ico/remove.png"),
                                       Tr::RD::buttonRemoveText(), this);
        connect( buttonRemove, SIGNAL( clicked() ),
                 this, SLOT( buttonRemoveClicked() ) );
        hLayoutActions->addWidget(buttonRemove);
      vLayout->addLayout(hLayoutActions);
    vLayout->setSizeConstraint(QLayout::SetFixedSize);
}

//

RemoveDialog::Decision RemoveDialog::decision() const
{
    return userDecision;
}

//

void RemoveDialog::buttonCancelClicked()
{
    userDecision = Cancel;
    reject();
}

void RemoveDialog::buttonDeleteClicked()
{
    userDecision = Delete;
    accept();
}

void RemoveDialog::buttonRemoveClicked()
{
    userDecision = Remove;
    accept();
}
