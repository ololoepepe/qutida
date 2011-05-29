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
#include "src/core/imageboardthread.h"
#include "src/gui/addthread.h"
#include "src/tr.h"

#include <QDialog>
#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLayout>
#include <QLineEdit>

ThreadParameters::ThreadParameters(
    const ImageboardThread::Modifiable &modParam, QWidget *parent) :
    QDialog(parent)
{
    vLayout = new QVBoxLayout(this);
      hLayoutMode = new QHBoxLayout();
        hLayoutMode->addStretch();
        //
        checkBoxSavePage = new QCheckBox(Tr::AT::checkBoxSavePageText(), this);
        checkBoxSavePage->setToolTip( Tr::AT::checkBoxSavePageTooltip() );
        checkBoxSavePage->setChecked(modParam.savePage);
        hLayoutMode->addWidget(checkBoxSavePage);
        //
        checkBoxExternal = new QCheckBox(Tr::AT::checkBoxExternalText(), this);
        checkBoxExternal->setToolTip( Tr::AT::checkBoxExternalTooltip() );
        checkBoxExternal->setChecked(modParam.external);
        hLayoutMode->addWidget(checkBoxExternal);
        //
        checkBoxReplace = new QCheckBox(Tr::AT::checkBoxReplaceText(), this);
        checkBoxReplace->setToolTip( Tr::AT::checkBoxReplaceTooltip() );
        checkBoxReplace->setChecked(modParam.replace);
        hLayoutMode->addWidget(checkBoxReplace);
      vLayout->addLayout(hLayoutMode);
      //
      hLayoutAttempt = new QHBoxLayout();
        hLayoutAttempt->addStretch();
        //
        labelAttemptPage = new QLabel(Tr::AT::labelAttemptPageText(), this);
        hLayoutAttempt->addWidget(labelAttemptPage);
        //
        spinBoxAttemptPage = new QSpinBox(this);
        spinBoxAttemptPage->setMinimum(AddThread::ATTEMPT_MIN);
        spinBoxAttemptPage->setMaximum(AddThread::ATTEMPT_MAX);
        spinBoxAttemptPage->setValue(modParam.attemptPage);
        hLayoutAttempt->addWidget(spinBoxAttemptPage);
        //
        labelAttemptFile = new QLabel(Tr::AT::labelAttemptFileText(), this);
        hLayoutAttempt->addWidget(labelAttemptFile);
        //
        spinBoxAttemptFile = new QSpinBox(this);
        spinBoxAttemptFile->setMinimum(AddThread::ATTEMPT_MIN);
        spinBoxAttemptFile->setMaximum(AddThread::ATTEMPT_MAX);
        spinBoxAttemptFile->setValue(modParam.attemptFile);
        hLayoutAttempt->addWidget(spinBoxAttemptFile);
      vLayout->addLayout(hLayoutAttempt);
      //
      hLayoutExtentions = new QHBoxLayout();
        labelExtentions = new QLabel(Tr::AT::labelExtentionsText(), this);
        hLayoutExtentions->addWidget(labelExtentions);
        //
        lineEditExtentions = new QLineEdit(this);
        lineEditExtentions->setToolTip( Tr::AT::lineEditExtentionsTooltip() );
        lineEditExtentions->setText(
                    Common::strFromList(modParam.extentions, " ") );
        hLayoutExtentions->addWidget(lineEditExtentions);
        //
        buttonResetExtentions =
                new QPushButton(Tr::AT::buttonResetExtentionsText(), this);
        buttonResetExtentions->setToolTip(
                    Tr::AT::buttonResetExtentionsTooltip() );
        connect( buttonResetExtentions, SIGNAL( clicked() ),
                 this, SLOT( buttonResetExtentionsClicked() ) );
        hLayoutExtentions->addWidget(buttonResetExtentions);
      vLayout->addLayout(hLayoutExtentions);
      //
      hLayoutRestart = new QHBoxLayout();
        hLayoutRestart->addStretch();
        //
        checkBoxRestartEnabled =
                new QCheckBox(Tr::AT::checkBoxRestartEnabledText(), this);
        checkBoxRestartEnabled->setToolTip(
                    Tr::AT::checkBoxRestartEnabledTooltip() );
        checkBoxRestartEnabled->setChecked(modParam.restartEnabled);
        hLayoutRestart->addWidget(checkBoxRestartEnabled);
        //
        spinBoxRestartInterval = new QSpinBox(this);
        spinBoxRestartInterval->setMinimum(AddThread::RESTART_MIN);
        spinBoxRestartInterval->setMaximum(AddThread::RESTART_MAX);
        spinBoxRestartInterval->setValue(modParam.restartInterval);
        hLayoutRestart->addWidget(spinBoxRestartInterval);
      vLayout->addLayout(hLayoutRestart);
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

ImageboardThread::Modifiable ThreadParameters::modParameters() const
{
    ImageboardThread::Modifiable modParam;
    modParam.extentions = lineEditExtentions->text().split(
                QRegExp("\\s+"), QString::SkipEmptyParts);
    modParam.external = checkBoxExternal->isChecked();
    modParam.replace = checkBoxReplace->isChecked();
    modParam.attemptPage = spinBoxAttemptPage->value();
    modParam.attemptFile = spinBoxAttemptFile->value();
    modParam.restartEnabled = checkBoxRestartEnabled->isChecked();
    modParam.restartInterval = spinBoxRestartInterval->value();
    modParam.savePage = checkBoxSavePage->isChecked();
    return modParam;
}

//

void ThreadParameters::buttonResetExtentionsClicked()
{
    lineEditExtentions->setText(
                Common::strFromList(
                    ImageboardThread::DEF_EXTENTIONS_LIST, " ") );
}
