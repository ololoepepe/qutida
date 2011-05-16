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

#include "src/gui/parametersdialog.h"
#include "src/tr.h"

#include <QString>
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLayout>
#include <QSettings>
#include <QPoint>

const QString ParametersDialog::GROUP_PARAMETERS_DIALOG = "parameters_dialog";
const QString ParametersDialog::KEY_POSITION = "position";

const QString ParametersDialog::GROUP_PARAMETERS = "parameters";
const QString ParametersDialog::KEY_LANGUAGE = "language";
const QString ParametersDialog::KEY_MINIMIZE = "minimize";
const QString ParametersDialog::KEY_EXIT_CONFIRMATION = "exit_confirmation";

const QString ParametersDialog::LANGUAGE_ENGLISH = "English";
const QString ParametersDialog::LANGUAGE_RUSSIAN = "Russian";

//

ParametersDialog::ParametersDialog(QWidget *parent) :
    QDialog(parent)
{
    vLayout = new QVBoxLayout(this);
      hLayoutLanguage = new QHBoxLayout();
        labelLanguage = new QLabel(Tr::PD::labelLanguageText(), this);
        hLayoutLanguage->addWidget(labelLanguage);
        //
        comboBoxLanguage = new QComboBox(this);
        comboBoxLanguage->addItem(LANGUAGE_ENGLISH);
        comboBoxLanguage->addItem(LANGUAGE_RUSSIAN);
        hLayoutLanguage->addWidget(comboBoxLanguage);
      vLayout->addLayout(hLayoutLanguage);
      //
      checkBoxMinimise = new QCheckBox(Tr::PD::checkBoxMinimizeText(), this);
      vLayout->addWidget(checkBoxMinimise);
      //
      checkBoxExitConfirmation =
              new QCheckBox(Tr::PD::checkBoxExitConfirmationText(), this);
      vLayout->addWidget(checkBoxExitConfirmation);
      //
      hLayoutActions = new QHBoxLayout();
        hLayoutActions->addStretch();
        //
        buttonCancel = new QPushButton(Tr::buttonCancelText(), this);
        connect( buttonCancel, SIGNAL( clicked() ),
                 this, SLOT( buttonCancelClicked() ) );
        hLayoutActions->addWidget(buttonCancel);
        //
        buttonOk = new QPushButton(Tr::buttonOkText(), this);
        connect( buttonOk, SIGNAL( clicked() ),
                 this, SLOT( buttonOkClicked() ) );
        hLayoutActions->addWidget(buttonOk);
      vLayout->addLayout(hLayoutActions);
    vLayout->setSizeConstraint(QLayout::SetFixedSize);
    //
    readSettings();
}

//

QString ParametersDialog::language() const
{
    return currentLanguage;
}

bool ParametersDialog::minimize() const
{
    return checkBoxMinimise->isChecked();
}

bool ParametersDialog::exitConfirmation() const
{
    return checkBoxExitConfirmation->isChecked();
}

//

void ParametersDialog::readSettings()
{
    QSettings settings;
    settings.beginGroup(GROUP_PARAMETERS_DIALOG);
    QPoint point = settings.value(KEY_POSITION).toPoint();
    settings.endGroup();
    settings.beginGroup(GROUP_PARAMETERS);
    QString language = settings.value(KEY_LANGUAGE).toString();
    checkBoxMinimise->setChecked(
                settings.value(KEY_MINIMIZE, true).toBool() );
    checkBoxExitConfirmation->setChecked(
                settings.value(KEY_EXIT_CONFIRMATION, false).toBool() );
    settings.endGroup();

    if (QPoint() != point)
        this->move(point);

    if ( !language.isEmpty() )
        currentLanguage = language;
    else
        currentLanguage = LANGUAGE_ENGLISH;

    comboBoxLanguage->setCurrentIndex(
                comboBoxLanguage->findText(currentLanguage) );
}

void ParametersDialog::writeCommonSettings()
{
    QSettings settings;
    settings.remove(GROUP_PARAMETERS);
    settings.beginGroup(GROUP_PARAMETERS);
      settings.setValue(KEY_LANGUAGE, currentLanguage);
      settings.setValue( KEY_MINIMIZE, checkBoxMinimise->isChecked() );
      settings.setValue( KEY_EXIT_CONFIRMATION,
                         checkBoxExitConfirmation->isChecked() );
    settings.endGroup();
}

void ParametersDialog::writePrivateSettings()
{
    QSettings settings;
    settings.remove(GROUP_PARAMETERS_DIALOG);
    settings.beginGroup(GROUP_PARAMETERS_DIALOG);
      settings.setValue( KEY_POSITION, this->pos() );
    settings.endGroup();
}

//

void ParametersDialog::buttonCancelClicked()
{
    writePrivateSettings();
    reject();
}

void ParametersDialog::buttonOkClicked()
{
    currentLanguage = comboBoxLanguage->currentText();
    writePrivateSettings();
    writeCommonSettings();
    accept();
}
