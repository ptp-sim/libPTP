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

#include "BabblingIP.h"

#include "distrib.h"

#include "IPProtocolId_m.h"
#include "IPv4Datagram.h"
#include "IPv4Address.h"
#include "Ieee802Ctrl_m.h"

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

Define_Module(BabblingIP);

// ======================================================
// Definitions
// ======================================================

void
BabblingIP::SendIpMsg()
{
    Ieee802Ctrl     *pCtrl  = new Ieee802Ctrl;
    IPv4Datagram    *pIP    = new IPv4Datagram;

    pIP->setSrcAddress( IPv4Address(192,168,0,1) );
    pIP->setDestAddress( IPv4Address(192,168,0,5) );
    pIP->setTransportProtocol( IP_PROT_TCP );

    pCtrl->setSrc( SrcMAC );
    pCtrl->setDest(MACAddress::BROADCAST_ADDRESS);
    pCtrl->setEtherType(ETHERTYPE_IPv4);

    pIP->setControlInfo(pCtrl);

    send( pIP, "IpOut" );
}

void
BabblingIP::ScheduleNext()
{
    simtime_t   NextInterval;

    NextInterval = simtime_t( intervalPar->doubleValue() );

    scheduleAt( simTime() + NextInterval, pScheduleTrigger );
}

void
BabblingIP::initialize()
{
    SrcMAC  = MACAddress::generateAutoAddress();

    pScheduleTrigger    = new cMessage("Schedule Trigger" );
    intervalPar         = &par( "Interval" );

    ScheduleNext();
}

void BabblingIP::handleMessage(cMessage *msg)
{
    SendIpMsg();
    ScheduleNext();
}
