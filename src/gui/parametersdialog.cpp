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
#include <QRegExp>
#include <QRegExpValidator>

const QString ParametersDialog::GROUP_PARAMETERS_DIALOG = "parameters_dialog";
const QString ParametersDialog::KEY_POSITION = "position";

const QString ParametersDialog::GROUP_PARAMETERS = "parameters";
  const QString ParametersDialog::KEY_START_ON_LOAD = "start_on_load";
  const QString ParametersDialog::KEY_LANGUAGE = "language";
  const QString ParametersDialog::KEY_MINIMIZE = "minimize";
  const QString ParametersDialog::KEY_START_MINIMIZED = "start_minimized";
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
    QRegExp rxPort("([1-5][0-9]{4,4}|6[1-4][0-9]{1,3}|65[1-4][0-9]{1,2}|"
                   "655[1-2][0-9]{1,1}|6553[0-5]|[1-9]{1,1}[0-9]{3,3})");
    vLayout = new QVBoxLayout(this);
      hLayoutLanguage = new QHBoxLayout();
        labelLanguage = new QLabel(Tr::PD::labelLanguageText(), this);
        hLayoutLanguage->addWidget(labelLanguage);
        //
        comboBoxLanguage = new QComboBox(this);
        comboBoxLanguage->addItem(LANGUAGE_ENGLISH);
        comboBoxLanguage->addItem(LANGUAGE_RUSSIAN);
        hLayoutLanguage->addWidget(comboBoxLanguage);
        //
        hLayoutLanguage->addStretch();
      vLayout->addLayout(hLayoutLanguage);
      //
      checkBoxStartOnLoad =
              new QCheckBox(Tr::PD::checkBoxStartOnLoadText(), this);
      vLayout->addWidget(checkBoxStartOnLoad);
      //
      checkBoxMinimise = new QCheckBox(Tr::PD::checkBoxMinimizeText(), this);
      vLayout->addWidget(checkBoxMinimise);
      //
      checkBoxStartMinimized =
              new QCheckBox(Tr::PD::checkBoxStartMinimizedText(), this);
      vLayout->addWidget(checkBoxStartMinimized);
      //
      checkBoxExitConfirmation =
              new QCheckBox(Tr::PD::checkBoxExitConfirmationText(), this);
      vLayout->addWidget(checkBoxExitConfirmation);
      //
      checkBoxProxyEnabled =
              new QCheckBox(Tr::PD::checkBoxProxyEnabledText(), this);
      vLayout->addWidget(checkBoxProxyEnabled);
      //
      hLayoutProxyHostPort = new QHBoxLayout();
        labelProxyHost = new QLabel(Tr::PD::labelHostText(), this);
        hLayoutProxyHostPort->addWidget(labelProxyHost);
        //
        lineEditProxyHost = new QLineEdit(this);
        hLayoutProxyHostPort->addWidget(lineEditProxyHost);
        //
        labelProxyPort = new QLabel(Tr::PD::labelPortText(), this);
        hLayoutProxyHostPort->addWidget(labelProxyPort);
        //
        lineEditProxyPort = new QLineEdit(this);
        lineEditProxyPort->setValidator(
                      new QRegExpValidator(rxPort, lineEditProxyPort) );
        lineEditProxyPort->setFixedWidth(50);
        hLayoutProxyHostPort->addWidget(lineEditProxyPort);
      vLayout->addLayout(hLayoutProxyHostPort);
      //
      hLayoutProxyUser = new QHBoxLayout();
        labelProxyUser = new QLabel(Tr::PD::labelUserText(), this);
        hLayoutProxyUser->addWidget(labelProxyUser);
        //
        lineEditProxyUser = new QLineEdit(this);
        hLayoutProxyUser->addWidget(lineEditProxyUser);
      vLayout->addLayout(hLayoutProxyUser);
      //
      hLayoutProxyPassword = new QHBoxLayout();
        labelProxyPassword = new QLabel(Tr::PD::labelPasswordText(), this);
        hLayoutProxyPassword->addWidget(labelProxyPassword);
        //
        lineEditProxyPassword = new QLineEdit(this);
        hLayoutProxyPassword->addWidget(lineEditProxyPassword);
      vLayout->addLayout(hLayoutProxyPassword);
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
    checkBoxStartOnLoad->setChecked(commonParam.startOnLoad);
    checkBoxMinimise->setChecked(commonParam.minimize);
    checkBoxStartMinimized->setChecked(commonParam.startMinimized);
    checkBoxExitConfirmation->setChecked(commonParam.exitConfirmation);
    proxySettings = readProxySettings(settings);
    checkBoxProxyEnabled->setChecked(proxySettings.enabled);
    lineEditProxyHost->setText(proxySettings.host);
    quint16 port = proxySettings.port;
    lineEditProxyPort->setText( (port > 0) ? QString::number(port) : "" );
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
      settings.setValue(KEY_START_MINIMIZED, commonParam.startMinimized);
      settings.setValue(KEY_EXIT_CONFIRMATION, commonParam.exitConfirmation);
      settings.setValue(KEY_START_ON_LOAD, commonParam.startOnLoad);
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
    commonParam.startOnLoad = checkBoxStartOnLoad->isChecked();
    commonParam.minimize = checkBoxMinimise->isChecked();
    commonParam.startMinimized = checkBoxStartMinimized->isChecked();
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
