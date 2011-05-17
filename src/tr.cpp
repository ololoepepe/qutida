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

#include "src/tr.h"
#include "src/core/imageboardthread.h"
#include "src/common.h"

#include <QString>
#include <QObject>

namespace Tr
{
    //button texts
    QString buttonCancelText()
    {return QObject::tr("Cancel", "global scope: button text");}
    QString buttonOkText()
    {return QObject::tr("OK", "global scope: button text");}

    namespace IT
    {
        QString threadModelHeader(ImageboardThread::Info key)
        {
            switch (key)
            {
            case ImageboardThread::InfoThread:
                return QObject::tr("Thread",
                                   "imageboard thread: model header");
            case ImageboardThread::InfoBoard:
                return QObject::tr("Board",
                                   "imageboard thread: model header");
            case ImageboardThread::InfoHost:
                return QObject::tr("Host",
                                   "imageboard thread: model header");
            case ImageboardThread::InfoStateExtended:
                return QObject::tr("State",
                                   "imageboard thread: model header");
            case ImageboardThread::InfoProgress:
                return QObject::tr("Progress",
                                   "imageboard thread: model header");
            case ImageboardThread::InfoDir:
                return QObject::tr("Directory",
                                   "imageboard thread: model header");
            case ImageboardThread::InfoUrl:
                return QObject::tr("URL",
                                   "imageboard thread: model header");
            default:
                return QString();
            }
        }
        QString threadExtendedState(ImageboardThread::ExtendedState ext)
        {
            switch (ext)
            {
            case ImageboardThread::ExtReadyInitial:
                return QObject::tr("Ready",
                                   "imageboard thread: extended state");
            case ImageboardThread::ExtReadyCompleted:
                return QObject::tr("Completed",
                                   "imageboard thread: extended state");
            case ImageboardThread::ExtReadyNothing:
                return QObject::tr("Nothing to download",
                                   "imageboard thread: extended state");
            case ImageboardThread::ExtReadyFailed:
                return QObject::tr("Failed",
                                   "imageboard thread: extended state");
            case ImageboardThread::ExtReadyAborted:
                return QObject::tr("Aborted",
                                   "imageboard thread: extended state");
            case ImageboardThread::ExtActiveStarted:
                return QObject::tr("Started",
                                   "imageboard thread: extended state");
            case ImageboardThread::ExtActiveDownloading:
                return QObject::tr("Downloading",
                                   "imageboard thread: extended state");
            case ImageboardThread::ExtWaiting:
                return QObject::tr("Waiting",
                                   "imageboard thread: extended state");
            default:
                return QString();
            }
        }
    }

    namespace MW
    {
        //menu titles
        QString menuFileTitle()
        {return QObject::tr("File", "main window: menu title");}
        QString menuEditTitle()
        {return QObject::tr("Edit", "main window: menu title");}
        QString menuThreadTitle()
        {return QObject::tr("Thread", "main window: menu title");}
        QString menuViewTitle()
        {return QObject::tr("View", "main window: menu title");}
        QString menuColumnsTitle()
        {return QObject::tr("Columns", "main window: menu title");}
        QString menuHelpTitle()
        {return QObject::tr("Help", "main window: menu title");}
        //action titles
        QString actAddText()
        {return QObject::tr("Add thread", "main window: action text");}
        QString actBackupText()
        {return QObject::tr("Backup", "main window: action text");}
        QString actExitText()
        {return QObject::tr("Exit", "main window: action text");}
        QString actParametersText()
        {return QObject::tr("Parameters", "main window: action text");}
        QString actOpenDirText()
        {return QObject::tr("Open directory", "main window: action text");}
        QString actOpenUrlText()
        {return QObject::tr("Open URL", "main window: action text");}
        QString actStopText()
        {return QObject::tr("Stop downloading selected thread",
                            "main window: action text");}
        QString actStartText()
        {return QObject::tr("Start downloading selected thread",
                            "main window: action text");}
        QString actThreadParametersText()
        {return QObject::tr("Thread parameters", "main window: action text");}
        QString actRemoveText()
        {return QObject::tr("Remove selected threads",
                            "main window: action text");}
        QString actToolBarText()
        {return QObject::tr("Toolbar", "main window: action text");}
        QString actHomepageText()
        {return QObject::tr("Homepage", "main window: action text");}
        QString actAboutText()
        {return QObject::tr("About", "main window: action text");}
        QString actAboutQtText()
        {return QObject::tr("About Qt", "main window: action text");}
        QString actShowHideText(bool visible)
        {
            if (visible)
                return QObject::tr("Hide", "main window: action text");
            else
                return QObject::tr("Show", "main window: action text");
        }
        //message box titles
        QString msgAboutTitle()
        {return QObject::tr("About", "main window: message box title");}
        QString msgAboutQtTitle()
        {return QObject::tr("About Qt", "main window: message box title");}
        QString msgExitConfirmationTitle()
        {return QObject::tr("Exit", "main window: message box title");}
        //message box texts
        QString msgExitConfirmationText()
        {return QObject::tr("Do you really want to exit?",
                            "main window: message box text");}
        QString msgAboutText()
        {return Common::APP_NAME + " " + Common::APP_VERSION + "\n\n" +
                    + "Qt Imageboard Download Assistant" + "\n\n" +
                    QObject::tr("A piece of software for convenient\n"
                                "imageboard content download.",
                                "main window: message box text") + "\n\n" +
                    "Version: " + Common::APP_VERSION + "\n\n" +
                    "Copyright (c) " + Common::CR_YEARS + " " +
                    Common::ORG_NAME + "\n\n" + Common::ORG_DOMAIN;}
        //dialog captions
        QString dialogAddThreadCaption()
        {return QObject::tr("Add threads", "main window: dialog caption");}
        QString dialogParametersCaption()
        {return QObject::tr("Parameters", "main window: dialog caption");}
        QString dialogRemoveCaption()
        {return QObject::tr("Remove threads", "main window: dialog caption");}
        QString dialogThreadParametersCaption()
        {return QObject::tr("Thread parameters",
                            "main window: dialog caption");}
        QString dialogBackupCaption()
        {return QObject::tr("Backup", "main window: dialog caption");}
    }

    namespace AT
    {
        //text edit tooltip
        QString textEditUrlsTooltip()
        {return QObject::tr("Put thread URLs here, divided by "
                            "one or more space symbols "
                            "(whitespace, tabulation or endline)",
                            "add thread: text edit tooltip");}
        //label texts
        QString labelDirText()
        {return QObject::tr("Directory", "add thread: label text") + ":";}
        QString labelAttemptPageText()
        {return QObject::tr("Attempt download page",
                            "add thread: label text") + ":";}
        QString labelAttemptFileText()
        {return QObject::tr("Attempt download files",
                            "add thread: label text") + ":";}
        QString labelExtentionsText()
        {return QObject::tr("File extentions",
                            "add thread: label text") + ":";}
        //check box texts
        QString checkBoxExternalText()
        {return QObject::tr("Enable external links",
                            "add thread: check box text");}
        QString checkBoxReplaceText()
        {return QObject::tr("Replace existing files",
                            "add thread: check box text");}
        QString checkBoxRestartEnabledText()
        {return QObject::tr("Restart automatically (interval in minutes)",
                            "add thread: check box text") + ":";}
        QString checkBoxStartText()
        {return QObject::tr("Start new threads after confirmation",
                            "add thread: check box text");}
        //tool button tooltip
        QString tbuttonSearchDirTooltip()
        {return QObject::tr("Open the select directory dialog",
                            "add thread: tool button tooltip");}
        //check box tooltips
        QString checkBoxExternalTooltip()
        {return QObject::tr("If checked, links to another hosts, contained "
                            "in the thread, will be added to download queue",
                            "add thread: check box tooltip");}
        QString checkBoxReplaceTooltip()
        {return QObject::tr("If checked, existing files will be overwritten. "
                            "Othrewise existing files will not be "
                            "downloaded again",
                            "add thread: check box tooltip");}
        QString checkBoxRestartEnabledTooltip()
        {return QObject::tr("If checked, the thread will be redownloaded. "
                            "Redownloads will occure every time the "
                            "specified interval expires",
                            "add thread: check box tooltip");}
        //line edit tooltips
        QString lineEditExtentionsTooltip()
        {return QObject::tr("Put the desired file extentions here, "
                            "divided by one or more whitespace or "
                            "tabulation symbols",
                            "add thread: line edit tooltip");}
        //button texts
        QString buttonResetExtentionsText()
        {return QObject::tr("Default", "add thread: button text");}
        //button tooltips
        QString buttonResetExtentionsTooltip()
        {return QObject::tr("Reset file extentions to the default value of",
                            "add thread: button tooltip") + " \"" +
                    Common::strFromList(
                        ImageboardThread::DEF_EXTENTIONS_LIST, ", ") + "\"";}

        //dialog captions
        QString dialogGetExistingDirectoryCaption()
        {return QObject::tr("Select directory", "add thread: dialog caption");}
    }

    namespace PD
    {
        //label texts
        QString labelLanguageText()
        {return QObject::tr("Language",
                            "parameters dialog: label text") + ":";}
        //check box texts
        QString checkBoxExitConfirmationText()
        {return QObject::tr("Ask confirmation on exit",
                            "parameters dialog: check box text");}
        QString checkBoxMinimizeText()
        {return QObject::tr("Minimize to syztem tray on close",
                            "parameters dialog: check box text");}
    }

    namespace RD
    {
        //label texts
        QString labelWarningText()
        {return QObject::tr("Remove selected threads? "
                            "This action is irreversible.",
                            "remove dialog: label text");}
        //button texts
        QString buttonDeleteText()
        {return QObject::tr("Remove threads and files",
                            "remove dialog: button text");}
        QString buttonRemoveText()
        {return QObject::tr("Remove threads only",
                            "remove dialog: button text");}
    }

    namespace IW
    {
        //label texts
        QString labelThreadKeyText()
        {return IT::threadModelHeader(ImageboardThread::InfoThread) + ":";}
        QString labelBoardKeyText()
        {return IT::threadModelHeader(ImageboardThread::InfoBoard) + ":";}
        QString labelHostKeyText()
        {return IT::threadModelHeader(ImageboardThread::InfoHost) + ":";}
        QString labelDirKeyText()
        {return IT::threadModelHeader(ImageboardThread::InfoDir) + ":";}
        QString labelUrlKeyText()
        {return IT::threadModelHeader(ImageboardThread::InfoUrl) + ":";}
        QString labelExtStateKey()
        {return IT::threadModelHeader(ImageboardThread::InfoStateExtended) +
                    ":";}
        QString labelRestartKeyText()
        {return QObject::tr("Restart (minutes left)",
                            "info widget: label text") + ":";}
        QString labelRestartDataText(int rest, bool enabled)
        {
            if (enabled)
            {
                if (rest > 0)
                {
                    return QString::number(rest);
                }
                else
                {
                    return QObject::tr("Enabled", "info widget: label text");
                }
            }
            else
            {
                return QObject::tr("Disabled", "info widget: label text");
            }
        }
        QString labelFilesTotalKeyText()
        {return QObject::tr("Files total", "info widget: label text") + ":";}
        QString labelFilesSavedKeyText()
        {return QObject::tr("Files saved", "info widget: label text") + ":";}
        QString labelFilesFailedKeyText()
        {return QObject::tr("Files failed", "info widget: label text") + ":";}
        QString labelFilesDataText(int count)
        {
            if (count >= 0)
                return QString::number(count);
            else
                return QObject::tr("Unknown", "info widget: label text");
        }
    }

    namespace TP
    {
        //check box texts
        QString checkBoxRestartEnabledText()
        {return QObject::tr("Restart enabled",
                            "thread parameters: check box text");}
        //label texts
        QString labelRestartIntervalText()
        {return QObject::tr("Restart interval",
                            "thread parameters: label text");}
    }

    namespace CM
    {
        //categories
        QString category(Category key)
        {
            switch (key)
            {
            case CategState:
                return QObject::tr("State", "category model: category");
            case CategHost:
                return QObject::tr("Hosts", "category model: category");
            case CategBoard:
                return QObject::tr("Boards", "category model: category");
            default:
                return QString();
            }
        }
        //sub categories
        QString subCategory(SubCategory key, int count)
        {
            switch (key)
            {
            case SubAll:
                return QObject::tr("All", "category model: sub category") +
                        " (" + QString::number(count) + ")";;
            case SubActive:
                return QObject::tr("Active", "category model: sub category") +
                        " (" + QString::number(count) + ")";;
            case SubWaiting:
                return QObject::tr("Waiting", "category model: sub category") +
                        " (" + QString::number(count) + ")";;
            case SubReady:
                return QObject::tr("Ready", "category model: sub category") +
                        " (" + QString::number(count) + ")";;
            case SubError:
                return QObject::tr("Error", "category model: sub category") +
                        " (" + QString::number(count) + ")";;
            default:
                return QString();
            }
        }
    }
}
