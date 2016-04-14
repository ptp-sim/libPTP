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

#include "ClockTest.h"

// ======================================================
// Types
// ======================================================

typedef enum
{
    TEST_SCHEDULE_EVENT,
    TEST_SET_TIME,
    TEST_INC_TIME,
    TEST_SET_FACTOR_1,
    TEST_SET_FACTOR_2,
    TEST_DEC_TIME,
}
tTestID;

struct tTestSchedule
{
    double      TestTime;
    tTestID     TestID;
};

// ======================================================
// Constants
// ======================================================

// ======================================================
// Variables
// ======================================================

static  tTestSchedule   TestSchedule[]    =
{
/*
    {   0.1,    TEST_SET_TIME       },
    {   0.1,    TEST_SCHEDULE_EVENT },
    {   0.2,    TEST_SCHEDULE_EVENT },
    {   0.3,    TEST_SCHEDULE_EVENT },
    {   0.4,    TEST_SCHEDULE_EVENT },
    {   1.0,    TEST_INC_TIME       },
    {   1.2,    TEST_DEC_TIME       },
    {   1.3,    TEST_SET_FACTOR_1   },
    {   1.4,    TEST_SET_FACTOR_2   },
*/
    {   1.5,    TEST_SET_FACTOR_1   },
    {   2.5,    TEST_SET_FACTOR_2   },
};


// ======================================================
// Declarations
// ======================================================

Define_Module(ClockTest);

// ======================================================
// Definitions
// ======================================================

ClockTest::ClockTest()
{
}

void ClockTest::initialize()
{
    // Get resources
    pClock  = check_and_cast<cScheduleClock *>( getModuleByPath( par("ClockPath").stringValue() ) );

    // Set up schedule
    for( size_t i = 0; i < sizeof(TestSchedule)/sizeof(TestSchedule[0]); i++ )
    {
        cMessage    *pMsg   = new cMessage;
        simtime_t   Time    = simtime_t(TestSchedule[i].TestTime);
        int         ID      = TestSchedule[i].TestID;

        EV << "Scheduling test " << ID << " at time " << Time << endl;

        pMsg->setKind( ID );

        scheduleAt( Time, pMsg );
    }

    pClock->ScheduleRelativeEvent( simtime_t(0.6), this, cClockEvent( 1, 1, 1, 1, nullptr ) );
    pClock->ScheduleRelativeEvent( simtime_t(0.4), this, cClockEvent( 2, 1, 1, 1, nullptr ) );
    pClock->ScheduleRelativeEvent( simtime_t(0.2), this, cClockEvent( 3, 1, 1, 1, nullptr ) );
    pClock->ScheduleRelativeEvent( simtime_t(0.1), this, cClockEvent( 4, 1, 1, 1, nullptr ) );
    pClock->ScheduleRelativeEvent( simtime_t(0.3), this, cClockEvent( 5, 1, 1, 1, nullptr ) );
    pClock->ScheduleRelativeEvent( simtime_t(0.5), this, cClockEvent( 6, 1, 1, 1, nullptr ) );
}

void
ClockTest::handleMessage(cMessage *msg)
{
    if( msg->isSelfMessage() )
    {
        EV << "Starting test " << msg->getKind() << endl;

        switch( msg->getKind() )
        {
            case TEST_SCHEDULE_EVENT:
            {
                pClock->ScheduleRelativeEvent( simtime_t(1.0), this, cClockEvent( 5, 4, 3, 2, nullptr ) );
                break;
            }

            case TEST_SET_TIME:
            {
                pClock->SetScaledTime( SIMTIME_ZERO );
                break;
            }

            case TEST_INC_TIME:
            {
                pClock->IncScaledTime( 1.0 );
                break;
            }

            case TEST_SET_FACTOR_1:
            {
                pClock->SetScaleFactor_ppb( -500000);
                break;
            }
            case TEST_SET_FACTOR_2:
            {
                pClock->SetScaleFactor_ppb( 500000 );
                break;
            }
            case TEST_DEC_TIME:
            {
                pClock->IncScaledTime( -0.5 );
                break;
            }
        }
    }
}

void
ClockTest::HandleClockEvent( cClockEvent& ClockEvent )
{
    EV << "ClockTest (" << getName() << "): received values " << ClockEvent.GetID1() << " " << ClockEvent.GetID2() << " " << ClockEvent.GetID3() << " " << ClockEvent.GetID4() << endl;
}
