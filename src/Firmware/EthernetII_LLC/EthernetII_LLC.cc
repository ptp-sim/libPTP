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

#include "EthernetII_LLC.h"

#include <climits>
#include <cerrno>

#include "Ieee802Ctrl_m.h"

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

Define_Module(EthernetII_LLC);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
EthernetII_LLC::AllocateResources()
{
    upperLayerOutBaseID = gate("upperLayerOut",0)->getBaseId();
    lowerLayerInID      = gate("lowerLayerIn")->getId();
    lowerLayerOutID     = gate("lowerLayerOut")->getId();

    // Sanity checks
    if( gate("upperLayerOut",0)->getVectorSize() != gate("upperLayerIn",0)->getVectorSize() )
    {
        throw cRuntimeError( "Size of upper layer gate vectors for input and output direction does not match." );
    }
}

void
EthernetII_LLC::ParseParameters()
{
    mapping.parseProtocolMapping( par("EtherTypeMapping").stringValue() );
}

// ------------------------------------------------------
// Handle messages
// ------------------------------------------------------
void
EthernetII_LLC::handleMessage(cMessage *msg)
{
    if( msg->getArrivalGate()->getId() == lowerLayerInID )
    {
        Ieee802Ctrl     *pCtrl  = check_and_cast<Ieee802Ctrl*>(msg->getControlInfo());
        int             EthType = pCtrl->getEtherType();
        int             GateID  = mapping.findOutputGateForProtocol( EthType );

        if( GateID == -2 )
        {
            EV_WARN << "Dropping frame with unregistered Ethertype (" << EthType << ")" << endl;

            delete msg;
        }
        else
        {
            send( msg, upperLayerOutBaseID + GateID );
        }
    }
    else
    {
        send( msg, lowerLayerOutID );
    }
}
