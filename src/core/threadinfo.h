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

#ifndef THREADINFO_H
#define THREADINFO_H

class ThreadInfo
{
public:
    enum Enum
    {
        IteratorFirst = 0,
        Thread = IteratorFirst,
        Board,
        Host,
        ExtendedState,
        Progress,
        Dir,
        Url,
        Added,
        IteratorMiddle = Added,
        ExtendedStatePrev,
        RestartEnabled,
        RestartInterval,
        TimeRest,
        FilesTotal,
        FilesSaved,
        FilesFailed,
        SavePage,
        FilesAuxTotal,
        FilesAuxSaved,
        FilesAuxFailed,
        Extentions,
        IteratorLast,
        InvalidValue = IteratorLast
    };

    //constructors
    ThreadInfo();
    ThreadInfo(Enum en);
    ThreadInfo(int v);

    //accessor functions
    Enum toEnum() const;
    bool isValid() const;

    //assignment operators
    ThreadInfo &operator=(const ThreadInfo &other);
    ThreadInfo &operator=(Enum en);
    ThreadInfo &operator=(int v);

    //comparisson operators
    bool operator<(const ThreadInfo &other) const;
    bool operator<(Enum en) const;
    bool operator>(const ThreadInfo &other) const;
    bool operator>(Enum en) const;
    bool operator<=(const ThreadInfo &other) const;
    bool operator<=(Enum en) const;
    bool operator>=(const ThreadInfo &other) const;
    bool operator>=(Enum en) const;
    bool operator==(const ThreadInfo &other) const;
    bool operator==(Enum en) const;
    bool operator!=(const ThreadInfo &other) const;
    bool operator!=(Enum en) const;

    //increment/decrement operators
    ThreadInfo &operator++();
    ThreadInfo operator++(int);
    ThreadInfo &operator--();
    ThreadInfo operator--(int);

private:
    Enum mEnum;

};

#endif // THREADINFO_H
