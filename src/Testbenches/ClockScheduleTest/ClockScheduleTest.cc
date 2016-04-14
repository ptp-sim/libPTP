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

#include "ClockScheduleTest.h"

Define_Module(ClockScheduleTest);

void ClockScheduleTest::initialize()
{
    Enabled = par( "Enabled" );

    if( Enabled )
    {
        // Get resources
        pClock  = check_and_cast<cScheduleClock *>( getModuleByPath( par("ClockPath").stringValue() ) );

        pTestMsg1 = new cMessage( "Schedule Test 1: ScheduleRelativeEvent(1.0)" );
        pTestMsg2 = new cMessage( "Schedule Test 2: ScheduleAbsoluteEvent(15.0)" );
        pTestMsg3 = new cMessage( "Schedule Test 3: ScheduleAbsoluteEvent(16.0)" );
        pTestMsg4 = new cMessage( "Schedule Test 4: CancelEvent( ScheduledEvent2 )" );
        pTestMsg5 = new cMessage( "Schedule Test 5: ScheduleRelativeEvent(1.0)" );

        scheduleAt( 1.0, pTestMsg1 );
        scheduleAt( 3.0, pTestMsg2 );
        scheduleAt( 4.0, pTestMsg3 );
        scheduleAt( 5.0, pTestMsg4 );
        scheduleAt( 6.0, pTestMsg5 );
    }
}

void ClockScheduleTest::handleMessage(cMessage *pMsg)
{
    if( pMsg == pTestMsg1 )
    {
        ScheduledEvent1 = pClock->ScheduleRelativeEvent( 1.0, this, Event1 );
    }
    else if( pMsg == pTestMsg2 )
    {
        ScheduledEvent2 = pClock->ScheduleAbsoluteEvent( 15.0, this, Event2 );
    }
    else if( pMsg == pTestMsg3 )
    {
        ScheduledEvent3 = pClock->ScheduleAbsoluteEvent( 16.000000051, this, Event3 );
    }
    else if( pMsg == pTestMsg4 )
    {
        pClock->CancelEvent( ScheduledEvent2 );
    }
    else if( pMsg == pTestMsg5 )
    {
        ScheduledEvent4 = pClock->ScheduleRelativeEvent( 1.0, this, Event4 );
    }

    delete pMsg;
}

ClockScheduleTest::ClockScheduleTest()
{
    Event1 = cClockEvent( 1, 0, 0, 0, nullptr );
    Event2 = cClockEvent( 2, 0, 0, 0, nullptr );
    Event3 = cClockEvent( 3, 0, 0, 0, nullptr );
    Event4 = cClockEvent( 2, 0, 0, 0, nullptr );
}

void
ClockScheduleTest::HandleClockEvent( cClockEvent& ClockEvent )
{
    EV << "ClockTest (" << getName() << "): received values " << ClockEvent.GetID1() << " " << ClockEvent.GetID2() << " " << ClockEvent.GetID3() << " " << ClockEvent.GetID4() << endl;
    EV << "ScaledTime: " << pClock->GetScaledTime() << endl;
    EV << "RealTime:   " << simTime() << endl;
}
