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

#include "PTP_ForeignMasterDS_Entry.h"

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
cForeignMasterDS_Entry::cForeignMasterDS_Entry()
{
    this->pClock                    = nullptr;
    this->ForeignMasterTimeWindow   = SIMTIME_ZERO;
}

cForeignMasterDS_Entry::cForeignMasterDS_Entry( const cForeignMasterDS_Entry& other )
{
    this->pClock                    = other.pClock;
    this->ForeignMasterTimeWindow   = other.ForeignMasterTimeWindow;

    this->ForeignClockMsgs          = other.ForeignClockMsgs;
    this->foreignMasterPortIdentity = other.foreignMasterPortIdentity;
}

cForeignMasterDS_Entry::~cForeignMasterDS_Entry()
{
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------
cPortIdentity&
cForeignMasterDS_Entry::PortIdentity()
{
    return  this->foreignMasterPortIdentity;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cForeignMasterDS_Entry::SetClock( cScheduleClock *pClock )
{
    this->pClock    = pClock;
}

void
cForeignMasterDS_Entry::SetForeignMasterTimeWindow( simtime_t ForeignMasterTimeWindow )
{
    this->ForeignMasterTimeWindow   = ForeignMasterTimeWindow;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
simtime_t
cForeignMasterDS_Entry::GetForeignMasterTimeWindow()
{
    return  this->ForeignMasterTimeWindow;
}

// ------------------------------------------------------
// API functions
// ------------------------------------------------------
void
cForeignMasterDS_Entry::PushForeignClock( cForeignClockDS ForeignClockDS, simtime_t RxTime, bool &NewDataFlag )
{
    // Check if the newly added entry makes the already available information valid
    if( ForeignClockMsgs.size() + 1 == PTP_FOREIGN_MASTER_THRESHOLD )
    {
        NewDataFlag = true;
    }
    else if
    (
        ( ForeignClockMsgs.size() >= PTP_FOREIGN_MASTER_THRESHOLD )  &&
        ( ForeignClockMsgs.back().ForeignClockDS() != ForeignClockDS )
    )
    {
        NewDataFlag = true;
    }
    else
    {
        NewDataFlag = false;
    }

    // Add entry to queue
    ForeignClockMsgs.push_back( cForeignClockMsg( ForeignClockDS, RxTime ) );
}

cForeignClockMsg
cForeignMasterDS_Entry::GetMostRecentEntry()
{
    if( IsEmpty() )
    {
        return cForeignClockMsg();
    }
    else
    {
        return ForeignClockMsgs.back();
    }
}

size_t
cForeignMasterDS_Entry::GetNumEntries()
{
    CleanUp();

    return  ForeignClockMsgs.size();
}

bool
cForeignMasterDS_Entry::IsEmpty()
{
    CleanUp();

    return  ForeignClockMsgs.empty();
}

void
cForeignMasterDS_Entry::CleanUp()
{
    // Get minimal scaled time that is inside the window
    simtime_t MinScaledTime = pClock->GetScaledTime() - this->ForeignMasterTimeWindow;

    // Drop all timestamps that are below the minimal time
    while( !ForeignClockMsgs.empty() )
    {
        if( ForeignClockMsgs.front().GetRxTime() < MinScaledTime )
        {
            ForeignClockMsgs.pop_front();
        }
        else
        {
            break;
        }
    }
}

void
cForeignMasterDS_Entry::AdjustTimestamps( simtime_t Delta )
{
    for( std::list<cForeignClockMsg>::iterator it = ForeignClockMsgs.begin(); it != ForeignClockMsgs.end(); it ++ )
    {
        it->SetRxTime( it->GetRxTime() + Delta );
    }
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
cForeignMasterDS_Entry&
cForeignMasterDS_Entry::operator=( const cForeignMasterDS_Entry& other )
{
    this->pClock                    = other.pClock;
    this->ForeignMasterTimeWindow   = other.ForeignMasterTimeWindow;

    this->ForeignClockMsgs          = other.ForeignClockMsgs;
    this->foreignMasterPortIdentity = other.foreignMasterPortIdentity;

    // By convention, always return *this
    return *this;
}
