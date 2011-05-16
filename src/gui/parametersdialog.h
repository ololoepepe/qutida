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

#ifndef PARAMETERSDIALOG_H
#define PARAMETERSDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QStringList>

class ParametersDialog : public QDialog
{
    Q_OBJECT
public:
    static const QString GROUP_PARAMETERS_DIALOG;
    static const QString KEY_POSITION;

    static const QString GROUP_PARAMETERS;
    static const QString KEY_LANGUAGE;
    static const QString KEY_MINIMIZE;
    static const QString KEY_EXIT_CONFIRMATION;

    static const QString LANGUAGE_ENGLISH;
    static const QString LANGUAGE_RUSSIAN;

    static QString suffixForLanguage(const QString &language)
    {
        if (LANGUAGE_ENGLISH == language)
            return "en";
        else if (LANGUAGE_RUSSIAN == language)
            return "ru";
        else
            return "";
    }

    explicit ParametersDialog(QWidget *parent = 0);

    QString language() const;
    bool minimize() const;
    bool exitConfirmation() const;

private:
    QVBoxLayout *vLayout;
      QHBoxLayout *hLayoutLanguage;
        QLabel *labelLanguage;
        QComboBox *comboBoxLanguage;
      QCheckBox *checkBoxMinimise;
      QCheckBox *checkBoxExitConfirmation;
      QHBoxLayout *hLayoutActions;
        //stretch
        QPushButton *buttonCancel;
        QPushButton *buttonOk;
    //
    QString currentLanguage;

    void readSettings();
    void writeCommonSettings();
    void writePrivateSettings();

private slots:
    void buttonCancelClicked();
    void buttonOkClicked();

};

#endif // PARAMETERSDIALOG_H
