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

#ifndef TR_H
#define TR_H

#include "src/core/imageboardthread.h"

#include <QObject>
#include <QString>

namespace Tr
{
    //button texts
    QString buttonCancelText();
    QString buttonOkText();

    namespace IT
    {
        QString threadModelHeader(ImageboardThread::Info key);
        QString threadExtendedState(ImageboardThread::ExtendedState ext);
    }

    namespace MW
    {
        //menu titles
        QString menuFileTitle();
        QString menuEditTitle();
        QString menuThreadTitle();
        QString menuViewTitle();
        QString menuColumnsTitle();
        QString menuHelpTitle();
        //action titles
        QString actAddText();
        QString actBackupText();
        QString actExitText();
        QString actParametersText();
        QString actOpenDirText();
        QString actOpenUrlText();
        QString actStopText();
        QString actStartText();
        QString actThreadParametersText();
        QString actRemoveText();
        QString actToolBarText();
        QString actHomepageText();
        QString actAboutText();
        QString actAboutQtText();
        QString actShowHideText(bool visible);
        //message box titles
        QString msgAboutTitle();
        QString msgAboutQtTitle();
        QString msgExitConfirmationTitle();
        QString msgNothingToBackupTitle();
        //message box texts
        QString msgExitConfirmationText();
        QString msgAboutText();
        QString msgNothingToBackupText();
        //dialog captions
        QString dialogAddThreadCaption();
        QString dialogParametersCaption();
        QString dialogRemoveCaption();
        QString dialogThreadParametersCaption();
        QString dialogBackupCaption();
        QString dialogAuthenticationCaption();
    }

    namespace AT
    {
        //text edit tooltip
        QString textEditUrlsTooltip();
        //label texts
        QString labelDirText();
        QString labelAttemptPageText();
        QString labelAttemptFileText();
        QString labelExtentionsText();
        //tool button tooltip
        QString tbuttonSearchDirTooltip();
        //check box texts
        QString checkBoxSavePageText();
        QString checkBoxExternalText();
        QString checkBoxReplaceText();
        QString checkBoxRestartEnabledText();
        QString checkBoxStartText();
        //check box tooltips
        QString checkBoxSavePageTooltip();
        QString checkBoxExternalTooltip();
        QString checkBoxReplaceTooltip();
        QString checkBoxRestartEnabledTooltip();
        //line edit tooltips
        QString lineEditExtentionsTooltip();
        //button texts
        QString buttonResetExtentionsText();
        //button tooltips
        QString buttonResetExtentionsTooltip();
        //dialog captions
        QString dialogGetExistingDirectoryCaption();
    }

    namespace PD
    {
        //label texts
        QString labelLanguageText();
        QString labelHostText();
        QString labelPortText();
        QString labelUserText();
        QString labelPasswordText();
        //check box texts
        QString checkBoxExitConfirmationText();
        QString checkBoxMinimizeText();
        QString checkBoxProxyEnabledText();
    }

    namespace RD
    {
        //label texts
        QString labelWarningText();
        //button texts
        QString buttonDeleteText();
        QString buttonRemoveText();
    }

    namespace IW
    {
        //label texts
        QString labelThreadKeyText();
        QString labelBoardKeyText();
        QString labelHostKeyText();
        QString labelDirKeyText();
        QString labelUrlKeyText();
        QString labelExtStateKey();
        QString labelRestartKeyText();
        QString labelRestartDataText(int rest, bool enabled);
        QString labelFilesTotalKeyText();
        QString labelFilesSavedKeyText();
        QString labelFilesFailedKeyText();
        QString labelFilesDataText(int count);
    }

    namespace TP
    {
        //check box texts
        QString checkBoxRestartEnabledText();
        //label texts
        QString labelRestartIntervalText();
    }

    namespace CM
    {
        //keys
        enum Category
        {
            CategState = 0,
            CategHost,
            CategBoard
        };
        enum SubCategory
        {
            SubAll = 0,
            SubActive,
            SubWaiting,
            SubReady,
            SubError
        };
        //categories
        QString category(Category key);
        //sub categories
        QString subCategory(SubCategory key, int count);
    }
}

#endif // TR_H
