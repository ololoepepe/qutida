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

#ifndef ADDTHREAD_H
#define ADDTHREAD_H

#include "src/core/imageboardthread.h"

#include <QDialog>
#include <QWidget>
#include <QString>
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
#include <QStringList>

class AddThread : public QDialog
{
    Q_OBJECT
public:
    static const QString GROUP_ADD_THREAD;
    static const QString KEY_GEOMETRY;
    static const QString KEY_START;

    explicit AddThread(QWidget *parent = 0);
    AddThread(const QStringList &urlList, QWidget *parent = 0);

    const QList<ImageboardThread::Parameters> &parameters() const;
    bool start() const;

private:
    QVBoxLayout *vLayout;
      QTextEdit *textEditUrls;
      QHBoxLayout *hLayoutDir;
        QLabel *labelDir;
        QLineEdit *lineEditDir;
        QToolButton *tbuttonSearchDir;
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
        QCheckBox *checkBoxStart;
        QPushButton *buttonCancel;
        QPushButton *buttonOk;

    ImageboardThread::Parameters defParam;
    QList<ImageboardThread::Parameters> paramList;

    void initialize();
    void readSettings();
    void writeSettings();
    void resetDefParam();
    QString constructSubPath(const QString &url);

private slots:
    void tbuttonSearchDirClicked();
    void buttonResetExtentionsClicked();
    void buttonCancelClicked();
    void buttonOkClicked();

};

#endif // ADDTHREAD_H
