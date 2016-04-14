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

#include "DelayQueue.h"

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

Define_Module( DelayQueue );

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Private functions
// ------------------------------------------------------
void
DelayQueue::HandleReminder()
{
    cMessage *pNext = MsgQueue.front();

    MsgQueue.pop();
    send( pNext, pOutGate );
    ScheduleReminder();
}

void
DelayQueue::ScheduleReminder()
{
    if
    (
        ( !MsgQueue.empty() ) &&
        ( !ReminderMsg->isScheduled() )
    )
    {
        simtime_t   delay = simtime_t( delayPar->doubleValue() );

        scheduleAt( simTime() + delay, ReminderMsg );
    }
}

// ------------------------------------------------------
// Init API
// ------------------------------------------------------
void
DelayQueue::AllocateResources()
{
    ReminderMsg = new cMessage( "DelayQueue Reminder" );
    pOutGate    = gate( "out" );
}

void
DelayQueue::ParseParameters()
{
    delayPar    = &par("Delay");
    MaxSize     = par( "Size" ).longValue();

    if( MaxSize <= 0 )
    {
        throw cRuntimeError( "Delay queue size must be strictly positive." );
    }
}

// ------------------------------------------------------
// OMNeT API
// ------------------------------------------------------
void
DelayQueue::handleMessage(cMessage *pMsg)
{
    if( pMsg->isSelfMessage() )
    {
        HandleReminder();
    }
    else
    {
        if( MsgQueue.size() < MaxSize )
        {
            MsgQueue.push( pMsg );
            ScheduleReminder();
        }
        else
        {
            delete pMsg;
        }
    }
}

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
DelayQueue::DelayQueue()
{
    MaxSize     = 1;
    ReminderMsg = NULL;
    pOutGate    = NULL;
}

DelayQueue::~DelayQueue()
{
    while( !MsgQueue.empty() )
    {
        delete MsgQueue.front();
        MsgQueue.pop();
    }

    cancelAndDelete( ReminderMsg );
}
