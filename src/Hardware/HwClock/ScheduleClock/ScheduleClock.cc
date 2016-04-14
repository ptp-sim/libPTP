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

#include "ScheduleClock.h"

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

Define_Module(cScheduleClock);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Debug functions
// ------------------------------------------------------
void
cScheduleClock::PrintEventQueue()
{
    ClockEventQueue_t   Queue   = EventQueue;
    size_t              cnt     = 1;

    EV << endl;
    EV << "Iterating over EventQueue, size = " << Queue.size() << endl;
    while( !Queue.empty() )
    {
        cScheduledClockEvent Event   = Queue.top();
        Queue.pop();

        EV << cnt << ". element: " << endl;
        Event.Print();

        cnt ++;
    }
    EV << endl;
}

// ------------------------------------------------------
// Internal functions
// ------------------------------------------------------

void
cScheduleClock::ShiftQueuedEvents( simtime_t Delta )
{
    ClockEventQueue_t  NewQueue;

    while( !EventQueue.empty() )
    {
        cScheduledClockEvent    ClockEvent( EventQueue.top() );

        ClockEvent.SetScaledTime( ClockEvent.GetScaledTime() + Delta );

        NewQueue.push( ClockEvent );

        EventQueue.pop();
    }

    EventQueue    = NewQueue;

    UpdateSchedule();
}

void
cScheduleClock::RemoveQueuedEvent( const cScheduledClockEvent &ScheduledEvent )
{
    // If there are no messages, there is nothing to cancel
    if( EventQueue.empty() )
    {
        return;
    }

    // Get first message
    cScheduledClockEvent TopEvent = EventQueue.top();
    EventQueue.pop();

    // If it is the event we were looking for, we are done
    if( TopEvent == ScheduledEvent )
    {
        return;
    }
    // Otherwise, we continue to search recursively
    else
    {
        RemoveQueuedEvent( ScheduledEvent );

        EventQueue.push( TopEvent );
    }
}

void
cScheduleClock::UpdateSchedule()
{
    if( EnableScheduleDebugOutput )
    {
        EV << "Updating event schedule" << endl;
    }

    cancelEvent(pNextScheduledEvent);

    if( !EventQueue.empty() )
    {
        simtime_t   NowScaledTime  = cAdjustableClock::GetScaledTime();
        simtime_t   NowHwTime      = cAdjustableClock::GetHwTime();
        simtime_t   NowRealTime    = simTime();

        simtime_t   RequScaledTime  = EventQueue.top().GetScaledTime();
        simtime_t   RequHwTime      = GetHwTimeAtScaledTime( RequScaledTime );
        simtime_t   RequRealTime    = GetRealTimeBeforeHwTime( RequHwTime );

        if( EnableScheduleDebugOutput )
        {
            EV << "Scheduling next event:" << endl;
            EV << "  Current time: " << endl;
            EV << "     ScaledTime: " << NowScaledTime << endl;
            EV << "     HwTime:     " << NowHwTime << endl;
            EV << "     RealTime:   " << NowRealTime << endl;
            EV << "  Next event: " << endl;
            EV << "    ScaledTime:    " << RequScaledTime << endl;
            EV << "    HwTime:        " << RequHwTime << endl;
            EV << "    Next RealTime: " << RequRealTime << endl;
        }

        if
        (
            ( NowRealTime + EdgePrecision >= RequRealTime ) &&
            ( NowScaledTime < RequScaledTime )
        )
        {
            RequRealTime = NowRealTime + EdgePrecision;
        }

        if( EnableScheduleDebugOutput )
        {
            EV << "Scheduling at " << RequRealTime << endl;
        }

        scheduleAt( RequRealTime, pNextScheduledEvent );
    }

    NumScheduledEvents = EventQueue.size();
}

// ------------------------------------------------------
// OMNeT API
// ------------------------------------------------------

void
cScheduleClock::handleMessage(cMessage *pMsg)
{
    // Handle self-message (can only be schedule App msgs)
    if( pMsg->isSelfMessage() )
    {
        if( pMsg == pNextScheduledEvent )
        {
            if( EventQueue.empty() )
            {
                throw cRuntimeError( "Clock: Found an empty EventQueue" );
            }

            // Remove current top entry from queue
            cScheduledClockEvent Event = EventQueue.top();

            simtime_t   Now = cAdjustableClock::GetScaledTime();

            if( Now > Event.GetScaledTime() + 2*TickLenNom )
            {
                std::stringstream ss;

                ss << "Oh noes!!11 We missed a tick! This should not happen." << endl;
                ss << "Now: " << Now << endl;
                ss << "Requested: " << Event.GetScaledTime() << endl;
                ss << "TickLen: " << TickLenNom << endl;

                throw cRuntimeError( ss.str().c_str() );
            }
            else if( Now >= Event.GetScaledTime() )
            {
                if( EnableScheduleDebugOutput )
                {
                    EV << "Executing event!" << endl;
                    EV << "   Scheduled ScaledTime: " << Event.GetScaledTime() << endl;
                    EV << "   Actual ScaledTime: " << Now << endl;
                }

                EventQueue.pop();
                Event.Execute();
            }

            UpdateSchedule();
        }
        else
        {
            cAdjustableClock::handleMessage( pMsg );
        }
    }
    else
    {
        throw cRuntimeError( "HwClock: Received unexpected message" );
        delete pMsg;
    }
}

void
cScheduleClock::finish()
{
}

// ------------------------------------------------------
// Init API
// ------------------------------------------------------
void
cScheduleClock::AllocateResources()
{
    cAdjustableClock::AllocateResources();

    pNextScheduledEvent = new cMessage( "Scheduled Clock Event" );
}

void
cScheduleClock::ParseParameters()
{
    cAdjustableClock::ParseParameters();

    EdgePrecision   = simtime_t( par("ScheduleEdgePrecision").doubleValue() );

    if( EdgePrecision > TickLenNom )
    {
        throw cRuntimeError( "Configured edge precision for scheduling is larger than the tick length, that does not make sense." );
    }

    EnableScheduleDebugOutput   = par( "EnableScheduleDebugOutput" ).boolValue();
}

void
cScheduleClock::RegisterSignals()
{
    cAdjustableClock::RegisterSignals();

    WATCH(NumScheduledEvents);
}

void
cScheduleClock::InitInternalState()
{
    cAdjustableClock::InitInternalState();

    UniqueIdCnt = 0;
}

// ------------------------------------------------------
// Constructors/Destructor
// ------------------------------------------------------

cScheduleClock::cScheduleClock()
    : cAdjustableClock()
{
    pNextScheduledEvent = nullptr;
    NumScheduledEvents  = 0;
}

cScheduleClock::~cScheduleClock()
{
    while( !EventQueue.empty() )
    {
        EventQueue.pop();
    }

    cancelAndDelete( pNextScheduledEvent );
}

// ------------------------------------------------------
// Event scheduling API
// ------------------------------------------------------

cScheduledClockEvent
cScheduleClock::ScheduleAbsoluteEvent( simtime_t ScaledTime, IClockEventSink *pSink, cClockEvent ClockEvent )
{
    EnterModuleSilent();

    cScheduledClockEvent NewEvent = cScheduledClockEvent( pSink, ScaledTime, ClockEvent, UniqueIdCnt++ );

    EventQueue.push( NewEvent );

    UpdateSchedule();

    LeaveModule();

    return NewEvent;
}

cScheduledClockEvent
cScheduleClock::ScheduleRelativeEvent( simtime_t Delta, IClockEventSink *pSink, cClockEvent ClockEvent )
{
    EnterModuleSilent();

    cScheduledClockEvent ScheduledEvent = ScheduleAbsoluteEvent( cAdjustableClock::GetScaledTime() + Delta, pSink, ClockEvent );

    LeaveModule();

    return ScheduledEvent;
}

void
cScheduleClock::CancelEvent( const cScheduledClockEvent &ScheduledEvent )
{
    EnterModuleSilent();

    RemoveQueuedEvent( ScheduledEvent );
    UpdateSchedule();

    LeaveModule();
}

// ------------------------------------------------------
// Time adjustment API
// ------------------------------------------------------
void
cScheduleClock::HandleTimeJump( simtime_t Delta )
{
    cAdjustableClock::HandleTimeJump( Delta );

    ShiftQueuedEvents( Delta );
}

void
cScheduleClock::SetScaleFactor_ppb( const int64_t ScaleFactor_ppb )
{
    EnterModuleSilent();

    if( EnableAdjustments )
    {
        cAdjustableClock::SetScaleFactor_ppb( ScaleFactor_ppb );
        UpdateSchedule();
    }

    LeaveModule();
}

// ------------------------------------------------------
// Clock API
// ------------------------------------------------------
simtime_t
cScheduleClock::GetScaledTime()
{
    EnterModuleSilent();

    simtime_t   ScaledTime =  cAdjustableClock::GetScaledTime();
    UpdateSchedule();

    LeaveModule();

    return ScaledTime;
}

cLocalTimeStamp
cScheduleClock::GetTimeStamp()
{
    EnterModuleSilent();

    cLocalTimeStamp TimeStamp = cAdjustableClock::GetTimeStamp();
    UpdateSchedule();

    LeaveModule();

    return TimeStamp;
}
