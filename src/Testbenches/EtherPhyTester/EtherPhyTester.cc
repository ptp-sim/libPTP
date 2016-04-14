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

#include "EtherPhyTester.h"

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

Define_Module(EtherPhyTester);

// ======================================================
// Definitions
// ======================================================

void
EtherPhyTester::SendMsg()
{
    T_tx = simTime();

    EthernetIIFrame     *pEthFrame;

    pEthFrame = new EthernetIIFrame;

    send( pEthFrame, "TestGate$o" );
}

void
EtherPhyTester::HandleRx( EthernetIIFrame *pEthFrame )
{
    simtime_t   T_rx = simTime();
    simtime_t   T_diff = T_rx - T_tx;

    EV << "T_diff: " << T_diff << endl;

    emit( Delay_SigId, T_diff );

    delete pEthFrame;
}

void
EtherPhyTester::initialize()
{
    TestInterval    = simtime_t( 1, SIMTIME_S );
    IntervalTimer   = new cMessage( "Next Test" );

    Delay_SigId = registerSignal( "Delay" );

    scheduleAt( simTime() + TestInterval, IntervalTimer );
}

void
EtherPhyTester::handleMessage(cMessage *pMsg)
{
    if( pMsg->isSelfMessage() )
    {
        SendMsg();

        scheduleAt( simTime() + TestInterval, IntervalTimer );
    }
    else
    {
        HandleRx( check_and_cast<EthernetIIFrame *>(pMsg) );
    }
}
