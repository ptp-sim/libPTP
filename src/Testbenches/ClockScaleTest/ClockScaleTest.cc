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

#include "ClockScaleTest.h"

Define_Module(ClockScaleTest);

void ClockScaleTest::initialize()
{
    bool    Enabled = par("Enabled").boolValue();

    if( Enabled )
    {
        std::string ClockPath;

        ClockPath   = par("ClockPath").stringValue();

        pClock      = check_and_cast<cAdjustableClock *>(getModuleByPath( ClockPath.c_str() ));

        pTestMsg1 = new cMessage( "Clock Scale Test 1: SetScaleFactor_ppb( 500000 )" );
        pTestMsg2 = new cMessage( "Clock Scale Test 2: SetScaleFactor_ppb( -500000 )" );
        pTestMsg3 = new cMessage( "Clock Scale Test 3: IncScaledTime( 3.5 )" );
        pTestMsg4 = new cMessage( "Clock Scale Test 4: SetScaleFactor_ppb( 20000 )" );
        pTestMsg5 = new cMessage( "Clock Scale Test 5: IncScaledTime( -1.0 )" );
        pTestMsg6 = new cMessage( "Clock Scale Test 6: Long distance query" );

        scheduleAt( 0.2, pTestMsg1 );
        scheduleAt( 0.6, pTestMsg2 );
        scheduleAt( 1.0, pTestMsg3 );
        scheduleAt( 1.3, pTestMsg4 );
        scheduleAt( 1.7, pTestMsg5 );
        scheduleAt( 1000000.0, pTestMsg6 );
    }
}

void ClockScaleTest::handleMessage(cMessage *pMsg)
{
    if( pMsg == pTestMsg1 )
    {
        pClock->SetScaleFactor_ppb( 500000 );
    }
    else if( pMsg == pTestMsg2 )
    {
        pClock->SetScaleFactor_ppb( -500000 );
    }
    else if( pMsg == pTestMsg3 )
    {
        pClock->IncScaledTime( 3.5 );
    }
    else if( pMsg == pTestMsg4 )
    {
        pClock->SetScaleFactor_ppb( 200000 );
    }
    else if( pMsg == pTestMsg5 )
    {
        pClock->IncScaledTime( -1.0 );
    }
    else if( pMsg == pTestMsg6 )
    {
        EV << "ScaledTime at " << simTime() << ": " << pClock->GetTimeStamp();
    }

    delete pMsg;
}
