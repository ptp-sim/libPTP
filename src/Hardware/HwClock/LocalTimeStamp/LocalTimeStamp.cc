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

#include "LocalTimeStamp.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Constants
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
cLocalTimeStamp::cLocalTimeStamp()
{
    this->EpochID   = 0;
    this->Time      = SIMTIME_ZERO;
}

cLocalTimeStamp::cLocalTimeStamp(uint64_t EpochID, simtime_t Time)
{
    this->EpochID   = EpochID;
    this->Time      = Time;
}

cLocalTimeStamp::cLocalTimeStamp( const cLocalTimeStamp &other )
{
    this->EpochID   = other.EpochID;
    this->Time      = other.Time;
}

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------
cLocalTimeStamp::~cLocalTimeStamp()
{
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
uint64_t
cLocalTimeStamp::GetEpochID() const
{
    return this->EpochID;
}

simtime_t
cLocalTimeStamp::GetTime() const
{
    return this->Time;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
bool
cLocalTimeStamp::operator== (const cLocalTimeStamp& other) const
{
    if
    (
        ( this->EpochID == other.EpochID ) &&
        ( this->Time    == other.Time    )
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

cLocalTimeStamp&
cLocalTimeStamp::operator=( const cLocalTimeStamp& other )
{
    this->EpochID   = other.EpochID;
    this->Time      = other.Time;

    // By convention, always return *this
    return *this;
}

simtime_t
cLocalTimeStamp::operator-( const cLocalTimeStamp& other )
{
    if( this->EpochID == other.EpochID )
    {
        return this->Time - other.Time;
    }
    else
    {
        return SIMTIME_ZERO;
    }
}

std::ostream&
operator<<(std::ostream& os, const cLocalTimeStamp& o )
{
    os << "Epoch: " << o.GetEpochID() << ", Time: " << o.GetTime();

    return os;
}
