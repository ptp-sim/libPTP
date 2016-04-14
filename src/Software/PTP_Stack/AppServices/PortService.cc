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

#include "PortService.h"
#include "PTP_Port.h"

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
cPortService::VerifyCopyable()
{
    cBasicService::VerifyCopyable();

    if( TimeoutState != TimerState::STOPPED )
    {
        throw cRuntimeError( "It is not allowed to copy services with non-stopped timers." );
    }
}

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
cPortService::cPortService()
    : cBasicService()
{
    this->pPort             = nullptr;
    this->SequId            = 0;

    this->Timeout           = SIMTIME_ZERO;
    this->TimeoutState      = TimerState::STOPPED;
    this->TimeoutEvent      = cClockEvent( static_cast<int>(PortServiceEvent::TIMEOUT), 0, 0, 0, nullptr );
    this->ScheduledTimeout  = cScheduledClockEvent();
}

cPortService::cPortService( const cPortService& other )
    : cBasicService( other )
{
    VerifyCopyable();

    this->pPort             = other.pPort;
    this->SequId            = other.SequId;

    this->Timeout           = other.Timeout;
    this->TimeoutState      = other.TimeoutState;
    this->TimeoutEvent      = other.TimeoutEvent;
    this->ScheduledTimeout  = other.ScheduledTimeout;
}

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------
cPortService::~cPortService()
{
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cPortService::SetTimeout( simtime_t Timeout )
{
    this->Timeout   = Timeout;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------

// ------------------------------------------------------
// API functions
// ------------------------------------------------------
void
cPortService::SetHierarchy( PTP_Stack *pApp, cPTP_Port *pPort, cScheduleClock *pClock )
{
    cBasicService::SetHierarchy( pApp, pClock );

    this->pPort = pPort;
}

UInteger16
cPortService::DrawSequId()
{
    return  this->SequId++;
}

void
cPortService::StartTimeout()
{
    if( TimeoutState == TimerState::STOPPED )
    {
        ScheduledTimeout = pClock->ScheduleRelativeEvent( Timeout, this, TimeoutEvent );
    }

    TimeoutState = TimerState::ACTIVE;
}

void
cPortService::UpdateTimeout()
{
    if( TimeoutState == TimerState::ACTIVE )
    {
        if( Timeout == SIMTIME_ZERO )
        {
            throw cRuntimeError( "PortService: Can't update timeout service with a value of 0" );
        }

        pClock->CancelEvent( ScheduledTimeout );
        ScheduledTimeout = pClock->ScheduleRelativeEvent( Timeout, this, TimeoutEvent );
    }
}

void
cPortService::StopTimeout()
{
    if( TimeoutState == TimerState::ACTIVE )
    {
        pClock->CancelEvent( ScheduledTimeout );
    }

    TimeoutState = TimerState::STOPPED;
}

// ------------------------------------------------------
// Clock API
// ------------------------------------------------------
void
cPortService::HandleClockEvent( cClockEvent& ClockEvent )
{
    PortServiceEvent    Event = static_cast<PortServiceEvent>(ClockEvent.GetID1());

    switch( Event )
    {
        case PortServiceEvent::TIMEOUT:
        {
            HandleTimeoutEvent();
            break;
        }

        default:
        {
            cBasicService::HandleClockEvent( ClockEvent );
            break;
        }
    }
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
cPortService&
cPortService::operator=( const cPortService& other )
{
    VerifyCopyable();

    cBasicService::operator=( other );

    this->pPort             = other.pPort;
    this->SequId            = other.SequId;

    this->Timeout           = other.Timeout;
    this->TimeoutState      = other.TimeoutState;
    this->TimeoutEvent      = other.TimeoutEvent;
    this->ScheduledTimeout  = other.ScheduledTimeout;

    // By convention, always return *this
    return *this;
}
