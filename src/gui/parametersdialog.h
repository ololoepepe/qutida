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
#include <QFrame>
#include <QLineEdit>
#include <QSettings>

class ParametersDialog : public QDialog
{
    Q_OBJECT
public:
    struct CommonParameters
    {
        QString language;
        bool minimize;
        bool exitConfirmation;
    };

    struct ProxySettings
    {
        bool enabled;
        QString host;
        quint16 port;
        QString user;
        QString password;
    };

    static const QString GROUP_PARAMETERS_DIALOG;
    static const QString KEY_POSITION;

    static const QString GROUP_PARAMETERS;
    static const QString KEY_LANGUAGE;
    static const QString KEY_MINIMIZE;
    static const QString KEY_EXIT_CONFIRMATION;

    static const QString GROUP_PROXY;
      static const QString KEY_ENABLED;
      static const QString KEY_HOST;
      static const QString KEY_PORT;
      static const QString KEY_USER;
      static const QString KEY_PASSWORD;

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

    static CommonParameters readCommonParameters(QSettings &settings)
    {
        CommonParameters param;
        settings.beginGroup(GROUP_PARAMETERS);
          param.language = settings.value(KEY_LANGUAGE).toString();

          if ( param.language.isEmpty() )
              param.language = LANGUAGE_ENGLISH;

          param.minimize = settings.value(KEY_MINIMIZE, true).toBool();
          param.exitConfirmation = settings.value(KEY_EXIT_CONFIRMATION,
                                                  false).toBool();
        settings.endGroup();
        return param;
    }

    static ProxySettings readProxySettings(QSettings &settings)
    {
        ProxySettings proxy;
        settings.beginGroup(GROUP_PROXY);
          proxy.enabled = settings.value(KEY_ENABLED, false).toBool();
          proxy.host = settings.value(KEY_HOST).toString();
          proxy.port = (quint16) settings.value(KEY_PORT, 0).toUInt();
          proxy.user = settings.value(KEY_USER).toString();
          proxy.password = settings.value(KEY_PASSWORD).toString();
        settings.endGroup();
        return proxy;
    }

    static void writeProxySettings(QSettings &settings,
                                   const ProxySettings &proxy)
    {
        settings.remove(GROUP_PROXY);
        settings.beginGroup(GROUP_PROXY);
          settings.setValue(KEY_ENABLED, proxy.enabled);
          settings.setValue(KEY_HOST, proxy.host);
          settings.setValue(KEY_PORT, proxy.port);
          settings.setValue(KEY_USER, proxy.user);
          settings.setValue(KEY_PASSWORD, proxy.password);
        settings.endGroup();
    }

    static bool proxyIsValid(const ProxySettings &proxy)
    {
        if (proxy.host.isEmpty() || proxy.port < 1000)
            return false;

        return true;
    }

    explicit ParametersDialog(QWidget *parent = 0);

    CommonParameters commonParameters() const;
    ProxySettings proxy() const;

private:
    QVBoxLayout *vLayout;
      QHBoxLayout *hLayoutLanguage;
        QLabel *labelLanguage;
        QComboBox *comboBoxLanguage;
      QHBoxLayout *hLayoutWindow;
        QCheckBox *checkBoxMinimise;
        QCheckBox *checkBoxExitConfirmation;
      QFrame *frameProxy;
        QVBoxLayout *vLayoutProxy;
          QCheckBox *checkBoxProxyEnabled;
          QHBoxLayout *hLayoutProxy1;
            QLabel *labelProxyHost;
            QLineEdit *lineEditProxyHost;
            QLabel *labelProxyPort;
            QLineEdit *lineEditProxyPort;
          QHBoxLayout *hLayoutProxy2;
            QLabel *labelProxyUser;
            QLineEdit *lineEditProxyUser;
            QLabel *labelProxyPassword;
            QLineEdit *lineEditProxyPassword;
      QHBoxLayout *hLayoutActions;
        //stretch
        QPushButton *buttonCancel;
        QPushButton *buttonOk;
    //
    CommonParameters commonParam;
    ProxySettings proxySettings;

    void readSettings();
    void writeCommonSettings();
    void writePrivateSettings();

private slots:
    void buttonCancelClicked();
    void buttonOkClicked();

};

#endif // PARAMETERSDIALOG_H
