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

#include "ClockServoTest.h"

Define_Module(ClockServoTest);

void
ClockServoTest::ParseParameters()
{
    pClockServo     = check_and_cast<IClockServo *>   ( getModuleByPath( par("ClockServoPath").stringValue()  ) );
    pMasterClock    = check_and_cast<cScheduleClock *>( getModuleByPath( par("MasterClockPath").stringValue() ) );
    pSlaveClock     = check_and_cast<cScheduleClock *>( getModuleByPath( par("SlaveClockPath").stringValue()  ) );

    Interval            = simtime_t( par("SampleInterval").doubleValue() );
}

ClockServoTest::ClockServoTest()
{
    pClockServo         = nullptr;
    pMasterClock        = nullptr;
    pSlaveClock         = nullptr;
}

void
ClockServoTest::initialize()
{
    ParseParameters();

    pTimerMsg   = new cMessage( "ClockServoTimer" );

    TestState   = INIT;

    scheduleAt( simTime() + Interval, pTimerMsg );
}

void
ClockServoTest::handleMessage(cMessage *msg)
{
    simtime_t   MasterTime;
    simtime_t   SlaveTime;
    simtime_t   Offset;

    switch( TestState )
    {
        case INIT:      pClockServo->Disable();
                        TestState   = CONFIG;
                        break;

        case CONFIG:    pClockServo->SetSyncInterval( Interval );
                        TestState   = ENABLE;
                        break;

        case ENABLE:    pClockServo->Enable();
                        TestState   = RUN;
                        break;

        case RUN:       MasterTime  = pMasterClock->GetScaledTime();
                        SlaveTime   = pSlaveClock->GetScaledTime();
                        Offset      = SlaveTime - MasterTime;

                        EV << "Offset: " << Offset << endl;

                        pClockServo->Sample( Offset, pSlaveClock->GetScaledTime() );
                        break;

        case INVALID:   // Do nothing
                        break;
    }

    scheduleAt( simTime() + Interval, pTimerMsg );
}
