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

#include "src/gui/addthread.h"
#include "src/core/imageboardthread.h"
#include "src/core/threadmanager.h"
#include "src/tr.h"
#include "src/common.h"

#include <QString>
#include <QDialog>
#include <QWidget>
#include <QSettings>
#include <QRect>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QList>
#include <QRegExp>
#include <QFileDialog>
#include <QStringList>
#include <QDir>
#include <QUrl>
#include <QDateTime>

const QString AddThread::GROUP_ADD_THREAD = "add_thread";
const QString AddThread::KEY_GEOMETRY = "geometry";
const QString AddThread::KEY_START = "start";

const int AddThread::ATTEMPT_MIN = 1;
const int AddThread::ATTEMPT_MAX = 10;
const int AddThread::RESTART_MIN = 1;
const int AddThread::RESTART_MAX = 360;

//

AddThread::AddThread(QWidget *parent) :
    QDialog(parent)
{
    initialize();
}

AddThread::AddThread(const QStringList &urlList, QWidget *parent) :
    QDialog(parent)
{
    initialize();

    for (int i = 0; i < urlList.count(); ++i)
        textEditUrls->append(
                    QUrl::fromUserInput( urlList.at(i) ).toString() );
}

//

void AddThread::appendList(const QStringList &urlList)
{
    QStringList urls = textEditUrls->toPlainText().split(QRegExp("\\s+"),
                                                         QString::SkipEmptyParts);

    for (int i = 0; i < urlList.count(); ++i)
        urls.append( QUrl::fromUserInput( urlList.at(i) ).toString() );

    urls.removeDuplicates();

    textEditUrls->clear();

    for (int i = 0; i < urls.count(); ++i)
        textEditUrls->append( QUrl::fromUserInput( urls.at(i) ).toString() );
}

const QList<ImageboardThread::Parameters> &AddThread::parameters() const
{
    return paramList;
}

bool AddThread::start() const
{
    return checkBoxStart->isChecked();
}

//

void AddThread::initialize()
{
    vLayout = new QVBoxLayout(this);
      textEditUrls = new QTextEdit(this);
      textEditUrls->setToolTip( Tr::AT::textEditUrlsTooltip() );
      vLayout->addWidget(textEditUrls);
      //
      hLayoutDir = new QHBoxLayout();
        labelDir = new QLabel(Tr::AT::labelDirText(), this);
        hLayoutDir->addWidget(labelDir);
        //
        lineEditDir = new QLineEdit(this);
        hLayoutDir->addWidget(lineEditDir);
        //
        tbuttonSearchDir = new QToolButton(this);
        tbuttonSearchDir->setText("...");
        tbuttonSearchDir->setToolTip( Tr::AT::tbuttonSearchDirTooltip() );
        connect( tbuttonSearchDir, SIGNAL( clicked() ),
                 this, SLOT( tbuttonSearchDirClicked() ) );
        hLayoutDir->addWidget(tbuttonSearchDir);
      vLayout->addLayout(hLayoutDir);
      //
      hLayoutMode = new QHBoxLayout();
        hLayoutMode->addStretch();
        //
        checkBoxSavePage = new QCheckBox(Tr::AT::checkBoxSavePageText(), this);
        checkBoxSavePage->setToolTip( Tr::AT::checkBoxSavePageTooltip() );
        hLayoutMode->addWidget(checkBoxSavePage);
        //
        checkBoxExternal = new QCheckBox(Tr::AT::checkBoxExternalText(), this);
        checkBoxExternal->setToolTip( Tr::AT::checkBoxExternalTooltip() );
        hLayoutMode->addWidget(checkBoxExternal);
        //
        checkBoxReplace = new QCheckBox(Tr::AT::checkBoxReplaceText(), this);
        checkBoxReplace->setToolTip( Tr::AT::checkBoxReplaceTooltip() );
        hLayoutMode->addWidget(checkBoxReplace);
      vLayout->addLayout(hLayoutMode);
      //
      hLayoutAttempt = new QHBoxLayout();
        hLayoutAttempt->addStretch();
        //
        labelAttemptPage = new QLabel(Tr::AT::labelAttemptPageText(), this);
        labelAttemptPage->setToolTip( Tr::AT::labelAttemptTooltip() );
        hLayoutAttempt->addWidget(labelAttemptPage);
        //
        spinBoxAttemptPage = new QSpinBox(this);
        spinBoxAttemptPage->setMinimum(ATTEMPT_MIN);
        spinBoxAttemptPage->setMaximum(ATTEMPT_MAX);
        hLayoutAttempt->addWidget(spinBoxAttemptPage);
        //
        labelAttemptFile = new QLabel(Tr::AT::labelAttemptFileText(), this);
        labelAttemptFile->setToolTip( Tr::AT::labelAttemptTooltip() );
        hLayoutAttempt->addWidget(labelAttemptFile);
        //
        spinBoxAttemptFile = new QSpinBox(this);
        spinBoxAttemptFile->setMinimum(ATTEMPT_MIN);
        spinBoxAttemptFile->setMaximum(ATTEMPT_MAX);
        hLayoutAttempt->addWidget(spinBoxAttemptFile);
      vLayout->addLayout(hLayoutAttempt);
      //
      hLayoutExtentions = new QHBoxLayout();
        labelExtentions = new QLabel(Tr::AT::labelExtentionsText(), this);
        hLayoutExtentions->addWidget(labelExtentions);
        //
        lineEditExtentions = new QLineEdit(this);
        lineEditExtentions->setToolTip( Tr::AT::lineEditExtentionsTooltip() );
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
        hLayoutRestart->addWidget(checkBoxRestartEnabled);
        //
        spinBoxRestartInterval = new QSpinBox(this);
        spinBoxRestartInterval->setMinimum(RESTART_MIN);
        spinBoxRestartInterval->setMaximum(RESTART_MAX);
        hLayoutRestart->addWidget(spinBoxRestartInterval);
      vLayout->addLayout(hLayoutRestart);
      //
      hLayoutActions = new QHBoxLayout();
        hLayoutActions->addStretch();
        //
        checkBoxStart = new QCheckBox(Tr::AT::checkBoxStartText(), this);
        hLayoutActions->addWidget(checkBoxStart);
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
    //
    readSettings();
}

void AddThread::readSettings()
{
    QSettings settings;
    settings.beginGroup(GROUP_ADD_THREAD);
      QRect geometry = settings.value(KEY_GEOMETRY).toRect();

      if (QRect() != geometry)
          this->setGeometry(geometry);

      checkBoxStart->setChecked( settings.value(KEY_START, false).toBool() );
    settings.endGroup();
    settings.beginGroup(ThreadManager::GROUP_THREADS);
      settings.beginGroup(ThreadManager::SUB_GROUP_DEFAULT);
      defParam = ImageboardThread::readParameters(settings);
      lineEditDir->setText(defParam.directory);
      checkBoxExternal->setChecked(defParam.external);
      checkBoxReplace->setChecked(defParam.replace);
      spinBoxAttemptPage->setValue(defParam.attemptPage);
      spinBoxAttemptFile->setValue(defParam.attemptFile);
      lineEditExtentions->setText(
                  Common::strFromList(defParam.extentions, " ") );
      checkBoxRestartEnabled->setChecked(defParam.restartEnabled);
      spinBoxRestartInterval->setValue(defParam.restartInterval);
      checkBoxSavePage->setChecked(defParam.savePage);
      settings.endGroup();
    settings.endGroup();
}

void AddThread::writeSettings()
{
    QSettings settings;
    settings.remove(GROUP_ADD_THREAD);
    settings.beginGroup(GROUP_ADD_THREAD);
      settings.setValue( KEY_GEOMETRY, this->geometry() );
      settings.setValue( KEY_START, checkBoxStart->isChecked() );
    settings.endGroup();
    settings.beginGroup(ThreadManager::GROUP_THREADS);
      settings.remove(ThreadManager::SUB_GROUP_DEFAULT);
      settings.beginGroup(ThreadManager::SUB_GROUP_DEFAULT);
        ImageboardThread::writeParameters(settings, defParam);
      settings.endGroup();
    settings.endGroup();
}

void AddThread::resetDefParam()
{
    defParam.url = "";
    defParam.directory = lineEditDir->text();
    defParam.external = checkBoxExternal->isChecked();
    defParam.replace = checkBoxReplace->isChecked();
    defParam.attemptPage = spinBoxAttemptPage->value();
    defParam.attemptFile = spinBoxAttemptFile->value();
    defParam.extentions = lineEditExtentions->text().split(
                QRegExp("\\s+"), QString::SkipEmptyParts);
    defParam.restartEnabled = checkBoxRestartEnabled->isChecked();
    defParam.restartInterval = spinBoxRestartInterval->value();
    defParam.savePage = checkBoxSavePage->isChecked();
}

QString AddThread::constructSubPath(const QString &url)
{
    return QDir::separator() + Common::getHost(url) + QDir::separator() +
            Common::getBoard(url) + QDir::separator() + Common::getThread(url);
}

//

void AddThread::tbuttonSearchDirClicked()
{
    QString path = QFileDialog::getExistingDirectory(
                this, Tr::AT::dialogGetExistingDirectoryCaption(),
                lineEditDir->text(), QFileDialog::ShowDirsOnly);

    if ( !path.isEmpty() )
        lineEditDir->setText(path);
}

void AddThread::buttonResetExtentionsClicked()
{
    lineEditExtentions->setText(
                Common::strFromList(
                    ImageboardThread::DEF_EXTENTIONS_LIST, " ") );
}

void AddThread::buttonCancelClicked()
{
    resetDefParam();
    writeSettings();
    reject();
}

void AddThread::buttonOkClicked()
{
    resetDefParam();
    writeSettings();
    QStringList urls = textEditUrls->toPlainText().split(QRegExp("\\s+"),
                                                         QString::SkipEmptyParts);
    urls.removeDuplicates();
    int count = urls.count();

    for (int i = 0; i < count; ++i)
    {
        QString url = QUrl::fromUserInput( urls.at(i) ).toString();

        if ( Common::isThreadUrlValid(url) )
        {
            ImageboardThread::Parameters param = defParam;
            param.url = url;
            param.directory += constructSubPath(param.url);
            param.added = QDateTime::currentDateTime();
            paramList << param;
        }
    }

    accept();
}
