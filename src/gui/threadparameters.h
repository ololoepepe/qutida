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

#ifndef THREADPARAMETERS_H
#define THREADPARAMETERS_H

#include <QDialog>
#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class ThreadParameters : public QDialog
{
    Q_OBJECT
public:
    explicit ThreadParameters(bool enabled, int interval, QWidget *parent = 0);

    bool restartEnabled() const;
    int restartInterval() const;

private:
    QVBoxLayout *vLayout;
      QHBoxLayout *hLayoutRestartEnabled;
        //stretch
        QCheckBox *checkBoxRestartEnabled;
      QHBoxLayout *hLayoutRestartInterval;
        //stretch
        QLabel *labelRestartInterval;
        QSpinBox *spinBoxRestartInterval;
      QHBoxLayout *hLayoutActions;
        //stretch
        QPushButton *buttonCancel;
        QPushButton *buttonOk;

};

#endif // THREADPARAMETERS_H
