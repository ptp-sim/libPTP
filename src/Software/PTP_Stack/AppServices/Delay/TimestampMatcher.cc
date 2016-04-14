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

#include <omnetpp.h>

#include "TimestampMatcher.h"

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
// Private methods
// ------------------------------------------------------

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------

// ------------------------------------------------------
// Constructors
// ------------------------------------------------------

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------

// ------------------------------------------------------
// Setters
// ------------------------------------------------------

// ------------------------------------------------------
// Getters
// ------------------------------------------------------

// ------------------------------------------------------
// API functions
// ------------------------------------------------------

void
cTimestampMatcher::PushFirst( simtime_t Time, simtime_t Corr, uint16_t SequId )
{
    this->Sequ1 = SequId;
    this->Time1 = Time;
    this->Corr1 = Corr;

    if( this->Sequ2 == SequId )
    {
        // New match
        MostRecentMatch.SequId  = SequId;
        MostRecentMatch.Time1   = this->Time1;
        MostRecentMatch.Time2   = this->Time2;
        MostRecentMatch.Corr1   = this->Corr1;
        MostRecentMatch.Corr2   = this->Corr2;
    }
}

void
cTimestampMatcher::PushSecond( simtime_t Time, simtime_t Corr, uint16_t SequId )
{
    this->Sequ2 = SequId;
    this->Time2 = Time;
    this->Corr2 = Corr;

    if( this->Sequ1 == SequId )
    {
        // New match
        MostRecentMatch.SequId  = SequId;
        MostRecentMatch.Time1   = this->Time1;
        MostRecentMatch.Time2   = this->Time2;
        MostRecentMatch.Corr1   = this->Corr1;
        MostRecentMatch.Corr2   = this->Corr2;
    }
}

MatchedTime_t
cTimestampMatcher::GetMostRecent()
{
    return  this->MostRecentMatch;
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
