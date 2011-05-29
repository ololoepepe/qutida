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

#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class Authentication : public QDialog
{
    Q_OBJECT
public:
    explicit Authentication(QWidget *parent = 0);

    QString user() const;
    QString password() const;

private:
    QVBoxLayout *vLayout;
      QHBoxLayout *hLayoutUser;
        QLabel *labelUser;
        QLineEdit *lineEditUser;
      QHBoxLayout *hLayoutPassword;
        QLabel *labelPassword;
        QLineEdit *lineEditPassword;
      QHBoxLayout *hLayoutActions;
        //stretch
        QPushButton *buttonCancel;
        QPushButton *buttonOk;

};

#endif // AUTHENTICATION_H
