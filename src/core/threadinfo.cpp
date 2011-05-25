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

#include "threadinfo.h"

//constructors
ThreadInfo::ThreadInfo()
{
    mEnum = IteratorFirst;
}

ThreadInfo::ThreadInfo(Enum en)
{
    mEnum = en;
}

ThreadInfo::ThreadInfo(int v)
{
    if (v <= IteratorLast && v >= IteratorFirst)
        mEnum = static_cast<Enum>(v);
    else
        mEnum = InvalidValue;
}

//accessor functions
ThreadInfo::Enum ThreadInfo::toEnum() const
{
    return mEnum;
}

bool ThreadInfo::isValid() const
{
    return (InvalidValue != mEnum);
}

//assignment operators
ThreadInfo &ThreadInfo::operator=(const ThreadInfo &other)
{
    mEnum = other.toEnum();
    return *this;
}

ThreadInfo &ThreadInfo::operator=(Enum en)
{
    mEnum = en;
    return *this;
}

ThreadInfo &ThreadInfo::operator=(int v)
{
    if (v <= IteratorLast && v >= IteratorFirst)
        mEnum = static_cast<Enum>(v);
    else
        mEnum = InvalidValue;

    return *this;
}

//comparisson operators
bool ThreadInfo::operator<(const ThreadInfo &other) const
{
    return ( mEnum < other.toEnum() );
}

bool ThreadInfo::operator<(Enum en) const
{
    return (mEnum < en);
}

bool ThreadInfo::operator>(const ThreadInfo &other) const
{
    return ( mEnum > other.toEnum() );
}

bool ThreadInfo::operator>(Enum en) const
{
    return (mEnum > en);
}

bool ThreadInfo::operator<=(const ThreadInfo &other) const
{
    return ( mEnum <= other.toEnum() );
}

bool ThreadInfo::operator<=(Enum en) const
{
    return (mEnum <= en);
}

bool ThreadInfo::operator>=(const ThreadInfo &other) const
{
    return ( mEnum <= other.toEnum() );
}

bool ThreadInfo::operator>=(Enum en) const
{
    return (mEnum >= en);
}

bool ThreadInfo::operator==(const ThreadInfo &other) const
{
    return ( mEnum == other.toEnum() );
}

bool ThreadInfo::operator==(Enum en) const
{
    return (mEnum == en);
}

bool ThreadInfo::operator!=(const ThreadInfo &other) const
{
    return ( mEnum != other.toEnum() );
}

bool ThreadInfo::operator!=(Enum en) const
{
    return (mEnum != en);
}

//increment/decrement operators
ThreadInfo &ThreadInfo::operator++()
{
    if (mEnum < IteratorLast)
        mEnum = static_cast<Enum>(mEnum + 1);

    return *this;
}

ThreadInfo ThreadInfo::operator++(int)
{
    ThreadInfo previous(mEnum);

    if (mEnum < IteratorLast)
        mEnum = static_cast<Enum>(mEnum + 1);

    return previous;
}

ThreadInfo &ThreadInfo::operator--()
{
    if (mEnum > IteratorFirst)
        mEnum = static_cast<Enum>(mEnum - 1);

    return *this;
}

ThreadInfo ThreadInfo::operator--(int)
{
    ThreadInfo previous(mEnum);

    if (mEnum > IteratorFirst)
        mEnum = static_cast<Enum>(mEnum + 1);

    return previous;
}
