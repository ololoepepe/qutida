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
#include <QFrame>
#include <QLineEdit>

const QString ParametersDialog::GROUP_PARAMETERS_DIALOG = "parameters_dialog";
const QString ParametersDialog::KEY_POSITION = "position";

const QString ParametersDialog::GROUP_PARAMETERS = "parameters";
const QString ParametersDialog::KEY_LANGUAGE = "language";
const QString ParametersDialog::KEY_MINIMIZE = "minimize";
const QString ParametersDialog::KEY_EXIT_CONFIRMATION = "exit_confirmation";

const QString ParametersDialog::GROUP_PROXY = "proxy";
  const QString ParametersDialog::KEY_ENABLED = "enabled";
  const QString ParametersDialog::KEY_HOST = "host";
  const QString ParametersDialog::KEY_PORT = "port";
  const QString ParametersDialog::KEY_USER = "user";
  const QString ParametersDialog::KEY_PASSWORD = "password";

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
      hLayoutWindow = new QHBoxLayout();
        checkBoxMinimise = new QCheckBox(Tr::PD::checkBoxMinimizeText(), this);
        hLayoutWindow->addWidget(checkBoxMinimise);
        //
        checkBoxExitConfirmation =
                new QCheckBox(Tr::PD::checkBoxExitConfirmationText(), this);
        hLayoutWindow->addWidget(checkBoxExitConfirmation);
      vLayout->addLayout(hLayoutWindow);
      //
      frameProxy = new QFrame(this);
      frameProxy->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        vLayoutProxy = new QVBoxLayout(frameProxy);
          checkBoxProxyEnabled = new QCheckBox(
                      Tr::PD::checkBoxProxyEnabledText(), frameProxy);
          vLayoutProxy->addWidget(checkBoxProxyEnabled);
          //
          hLayoutProxy1 = new QHBoxLayout();
            labelProxyHost = new QLabel(Tr::PD::labelHostText(), frameProxy);
            hLayoutProxy1->addWidget(labelProxyHost);
            //
            lineEditProxyHost = new QLineEdit(frameProxy);
            hLayoutProxy1->addWidget(lineEditProxyHost);
            //
            labelProxyPort = new QLabel(Tr::PD::labelPortText(), frameProxy);
            hLayoutProxy1->addWidget(labelProxyPort);
            //
            lineEditProxyPort = new QLineEdit(frameProxy);
            hLayoutProxy1->addWidget(lineEditProxyPort);
          vLayoutProxy->addLayout(hLayoutProxy1);
          //
          hLayoutProxy2 = new QHBoxLayout();
            labelProxyUser = new QLabel(Tr::PD::labelUserText(), frameProxy);
            hLayoutProxy2->addWidget(labelProxyUser);
            //
            lineEditProxyUser = new QLineEdit(frameProxy);
            hLayoutProxy2->addWidget(lineEditProxyUser);
            //
            labelProxyPassword = new QLabel(Tr::PD::labelPasswordText(),
                                            frameProxy);
            hLayoutProxy2->addWidget(labelProxyPassword);
            //
            lineEditProxyPassword = new QLineEdit(frameProxy);
            hLayoutProxy2->addWidget(lineEditProxyPassword);
          vLayoutProxy->addLayout(hLayoutProxy2);
      vLayout->addWidget(frameProxy);
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


ParametersDialog::CommonParameters ParametersDialog::commonParameters() const
{
    return commonParam;
}

ParametersDialog::ProxySettings ParametersDialog::proxy() const
{
    return proxySettings;
}

//

void ParametersDialog::readSettings()
{
    QSettings settings;
    settings.beginGroup(GROUP_PARAMETERS_DIALOG);
      QPoint point = settings.value(KEY_POSITION).toPoint();

      if (QPoint() != point)
          this->move(point);

    settings.endGroup();
    commonParam = readCommonParameters(settings);
    comboBoxLanguage->setCurrentIndex(
                comboBoxLanguage->findText(commonParam.language) );
    checkBoxMinimise->setChecked(commonParam.minimize);
    checkBoxExitConfirmation->setChecked(commonParam.exitConfirmation);
    proxySettings = readProxySettings(settings);
    checkBoxProxyEnabled->setChecked(proxySettings.enabled);
    lineEditProxyHost->setText(proxySettings.host);
    quint16 port = proxySettings.port;
    lineEditProxyPort->setText( (port > 1000) ? QString::number(port) : "" );
    lineEditProxyUser->setText(proxySettings.user);
    lineEditProxyPassword->setText(proxySettings.password);
}

void ParametersDialog::writeCommonSettings()
{
    QSettings settings;
    settings.remove(GROUP_PARAMETERS);
    settings.beginGroup(GROUP_PARAMETERS);
      settings.setValue(KEY_LANGUAGE, commonParam.language);
      settings.setValue(KEY_MINIMIZE, commonParam.minimize);
      settings.setValue(KEY_EXIT_CONFIRMATION, commonParam.exitConfirmation);
    settings.endGroup();
    writeProxySettings(settings, proxySettings);
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
    commonParam.language = comboBoxLanguage->currentText();
    commonParam.minimize = checkBoxMinimise->isChecked();
    commonParam.exitConfirmation = checkBoxExitConfirmation->isChecked();
    proxySettings.enabled = checkBoxProxyEnabled->isChecked();
    proxySettings.host = lineEditProxyHost->text();
    proxySettings.port = (quint16) lineEditProxyPort->text().toUInt();
    proxySettings.user = lineEditProxyUser->text();
    proxySettings.password = lineEditProxyPassword->text();
    writePrivateSettings();
    writeCommonSettings();
    accept();
}
