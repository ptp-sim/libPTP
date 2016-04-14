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

#include "BasicService.h"

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
// Internal functions
// ------------------------------------------------------
void
cBasicService::VerifyCopyable()
{
    if( IntervalState != TimerState::STOPPED )
    {
        throw cRuntimeError( "It is not allowed to copy services with non-stopped timers." );
    }
}

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
cBasicService::cBasicService()
    : cSubmoduleInitBase()
{
    this->pApp              = nullptr;
    this->pClock            = nullptr;

    this->Interval          = SIMTIME_ZERO;
    this->IntervalState     = TimerState::STOPPED;
    this->IntervalEvent     = cClockEvent( static_cast<int>(BasicServiceEvent::INTERVAL), 0, 0, 0, nullptr );
    this->ScheduledInterval = cScheduledClockEvent();
}

cBasicService::cBasicService( const cBasicService& other )
    : cSubmoduleInitBase( other )
{
    VerifyCopyable();

    this->pApp              = other.pApp;
    this->pClock            = other.pClock;

    this->Interval          = other.Interval;
    this->IntervalState     = other.IntervalState;
    this->IntervalEvent     = other.IntervalEvent;
    this->ScheduledInterval = other.ScheduledInterval;
}

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------
cBasicService::~cBasicService()
{
}

// ------------------------------------------------------
// Initialization
// ------------------------------------------------------
void
cBasicService::SetHierarchy( PTP_Stack *pApp, cScheduleClock *pClock )
{
    this->pApp      = pApp;
    this->pClock    = pClock;
}

// ------------------------------------------------------
// Clock API
// ------------------------------------------------------
void
cBasicService::HandleClockEvent( cClockEvent& ClockEvent )
{
    BasicServiceEvent   Event = static_cast<BasicServiceEvent>(ClockEvent.GetID1());

    switch( Event )
    {
        case BasicServiceEvent::INTERVAL:
        {
            HandleIntervalEvent();

            ScheduledInterval = pClock->ScheduleRelativeEvent( Interval, this, IntervalEvent );
            break;
        }

        default:
        {
            throw cRuntimeError( "Received unexpected clock event." );
            break;
        }
    }
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cBasicService::SetInterval( simtime_t Interval )
{
    if( Interval == SIMTIME_ZERO )
    {
        throw cRuntimeError( "PortService: Can't set service interval to 0" );
    }

    this->Interval  = Interval;
}

// ------------------------------------------------------
// API functions
// ------------------------------------------------------
void
cBasicService::StartInterval()
{
    if( IntervalState == TimerState::STOPPED )
    {
        if( Interval == SIMTIME_ZERO )
        {
            throw cRuntimeError( "PortService: Can't start interval service with a value of 0" );
        }

        ScheduledInterval = pClock->ScheduleRelativeEvent( SIMTIME_ZERO, this, IntervalEvent );
    }

    IntervalState = TimerState::ACTIVE;
}

void
cBasicService::StopInterval()
{
    if( IntervalState == TimerState::ACTIVE )
    {
        pClock->CancelEvent( ScheduledInterval );
    }

    IntervalState = TimerState::STOPPED;
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
cBasicService&
cBasicService::operator=( const cBasicService& other )
{
    VerifyCopyable();

    cSubmoduleInitBase::operator=( other );

    this->pApp              = other.pApp;
    this->pClock            = other.pClock;

    this->Interval          = other.Interval;
    this->IntervalState     = other.IntervalState;
    this->IntervalEvent     = other.IntervalEvent;
    this->ScheduledInterval = other.ScheduledInterval;

    // By convention, always return *this
    return *this;
}
