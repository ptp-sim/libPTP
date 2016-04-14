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

#include "ScheduledClockEvent.h"

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
// Constructor
// ------------------------------------------------------
cScheduledClockEvent::cScheduledClockEvent()
    : ClockEvent()
{
    this->pSink         = nullptr;
    this->ScaledTime    = SIMTIME_ZERO;
    this->UniqueId      = -1;
}

cScheduledClockEvent::cScheduledClockEvent( IClockEventSink *pSink, simtime_t ScaledTime, cClockEvent ClockEvent, uint64_t UniqueId )
    : ClockEvent( ClockEvent )
{
    this->pSink         = pSink;
    this->ScaledTime    = ScaledTime;
    this->UniqueId      = UniqueId;
}

cScheduledClockEvent::cScheduledClockEvent( const cScheduledClockEvent &other )
{
    this->pSink         = other.pSink;
    this->ScaledTime    = other.ScaledTime;
    this->ClockEvent    = other.ClockEvent;
    this->UniqueId      = other.UniqueId;
}

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------
cScheduledClockEvent::~cScheduledClockEvent()
{
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
cClockEvent
cScheduledClockEvent::GetClockEvent() const
{
    return this->ClockEvent;
}

simtime_t
cScheduledClockEvent::GetScaledTime() const
{
    return this->ScaledTime;
}

IClockEventSink *
cScheduledClockEvent::GetClockEventSink() const
{
    return this->pSink;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cScheduledClockEvent::SetScaledTime( simtime_t ScaledTime )
{
    this->ScaledTime = ScaledTime;
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
bool
cScheduledClockEvent::operator<(const cScheduledClockEvent& rhs) const
{
    /// Because we want to have the element with the *smallest*
    /// timestamp to be the one with the highest priority, we
    /// swap > and <
    return this->ScaledTime > rhs.ScaledTime;
}

bool
cScheduledClockEvent::operator>(const cScheduledClockEvent& rhs) const
{
    /// Because we want to have the element with the *smallest*
    /// timestamp to be the one with the highest priority, we
    /// swap > and <
    return this->ScaledTime < rhs.ScaledTime;
}

bool
cScheduledClockEvent::operator== (const cScheduledClockEvent& other) const
{
    // Time is not compared, as it might change for an already scheduled event
    // if the clock jumps

    if
    (
        ( other.pSink       == this->pSink      ) &&
        ( other.ClockEvent  == this->ClockEvent ) &&
        ( other.UniqueId    == this->UniqueId   )
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

cScheduledClockEvent&
cScheduledClockEvent::operator=( const cScheduledClockEvent& other )
{
    this->pSink         = other.pSink;
    this->ScaledTime    = other.ScaledTime;
    this->ClockEvent    = other.ClockEvent;
    this->UniqueId      = other.UniqueId;

    // By convention, always return *this
    return *this;
}

// ------------------------------------------------------
// API functions
// ------------------------------------------------------
void
cScheduledClockEvent::Execute()
{
    pSink->ClockEventCallback( ClockEvent );
}

// ------------------------------------------------------
// Debug functions
// ------------------------------------------------------
void
cScheduledClockEvent::Print()
{
    EV << "  ScaledTime: "  << ScaledTime << endl;
    EV << "  ID1: "         << ClockEvent.GetID1()  << endl;
    EV << "  ID2: "         << ClockEvent.GetID2()  << endl;
    EV << "  ID3: "         << ClockEvent.GetID3()  << endl;
    EV << "  ID4: "         << ClockEvent.GetID4()  << endl;
}
