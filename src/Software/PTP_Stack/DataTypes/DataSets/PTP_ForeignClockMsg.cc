// ============================================================================
//
// Copyright 2013-2015 Wolfgang Wallner (wolfgang-wallner AT gmx.at)
//
// This file is part of the LibPTP project.
//
// The LibPTP project is free software: you can redistribute it
// and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// The LibPTP project is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License.com for more details.
//
// You should have received a copy of the GNU General Public License.com
// along with The LibPTP project.
// If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================

// ======================================================
// Includes
// ======================================================

#include "PTP_ForeignClockMsg.h"

// ======================================================
// Definitions
// ======================================================

// ======================================================
// Types
// ======================================================

// ======================================================
// Variables
// ======================================================

// ======================================================
// Declarations
// ======================================================

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Constructors
// ------------------------------------------------------

cForeignClockMsg::cForeignClockMsg()
{
    foreignClockDS  = cForeignClockDS();
    RxTime          = SIMTIME_ZERO;
}

cForeignClockMsg::cForeignClockMsg( cForeignClockDS ForeignClockDS, simtime_t RxTime )
    : foreignClockDS( ForeignClockDS ), RxTime( RxTime )
{
}

cForeignClockMsg::cForeignClockMsg(const cForeignClockMsg& other)
{
    this->foreignClockDS = other.foreignClockDS;
    this->RxTime         = other.RxTime;
}

cForeignClockMsg::~cForeignClockMsg()
{
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------
cForeignClockDS&
cForeignClockMsg::ForeignClockDS()
{
    return  this->foreignClockDS;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cForeignClockMsg::SetRxTime( simtime_t RxTime )
{
    this->RxTime = RxTime;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
simtime_t
cForeignClockMsg::GetRxTime()
{
    return this->RxTime;
}

cForeignClockDS
cForeignClockMsg::GetForeignClockDS()
{
    return this->foreignClockDS;
}

// ------------------------------------------------------
// Compare Operators
// ------------------------------------------------------
bool
cForeignClockMsg::operator<(const cForeignClockMsg& rhs) const
{
    return this->RxTime < rhs.RxTime;
}

bool
cForeignClockMsg::operator>(const cForeignClockMsg& rhs) const
{
    return this->RxTime > rhs.RxTime;
}

cForeignClockMsg&
cForeignClockMsg::operator= (const cForeignClockMsg& other)
{
    this->foreignClockDS = other.foreignClockDS;
    this->RxTime         = other.RxTime;

    // By convention, always return *this
    return *this;
}
