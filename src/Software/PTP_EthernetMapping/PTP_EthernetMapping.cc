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

#include "PTP_EthernetMapping.h"

#include "PTP_Ethernet.h"

#include "PTP_Ctrl_m.h"
#include "PTPv2_m.h"

#include <cassert>

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

Define_Module(PTP_EthernetMapping);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------

void
PTP_EthernetMapping::AllocateResources()
{
    LowerLayerOutGateID = gate( "lowerLayerOut" )->getId();
    LowerLayerInGateID  = gate( "lowerLayerIn"  )->getId();
    UpperLayerOutGateID = gateBaseId( "upperLayerOut" );
    UpperLayerInGateID  = gateBaseId( "upperLayerIn" );

    GateSize = gateSize( "upperLayerIn" );

    if( gateSize( "upperLayerIn" ) != gateSize( "upperLayerOut" ) )
    {
        error( "Size of vector gates for upperLayerIn and upperLayerOut must match." );
    }
}

void
PTP_EthernetMapping::ParseParameters()
{
    Enable  = par( "Enable" ).boolValue();
}

// ------------------------------------------------------
// Handle messages
// ------------------------------------------------------
void
PTP_EthernetMapping::handleMessage(cMessage *msg)
{
    if( !Enable )
    {
        delete msg;
        return;
    }

    if( msg->getArrivalGateId() == LowerLayerInGateID )
    {
        PTP_Ctrl    *pCtrl;
        int         GateID;

        pCtrl   = check_and_cast<PTP_Ctrl *>(msg->removeControlInfo());
        GateID  = pCtrl->getRxPort();

        delete pCtrl;

        if
        (
            ( GateID >= 0 ) &&
            ( GateID < GateSize )
        )
        {
            send( msg, UpperLayerOutGateID + GateID );
        }
        else
        {
            error( "Received frame with invalid gate ID" );
        }
    }
    else
    {
        PTP_Ctrl            *pCtrl  = new PTP_Ctrl;
        PTPv2_Frame         *pPtp   = check_and_cast<PTPv2_Frame *>(msg);
        tPtpMessageType     MsgType = (tPtpMessageType) pPtp->getMessageType();
        int                 GateID;

        GateID = msg->getArrivalGateId() - UpperLayerInGateID;

        assert( GateID >= 0 );
        assert( GateID < GateSize );

        pCtrl->setEtherType( PTP_ETH_TYPE );
        pCtrl->setTxPort( GateID );

        switch( MsgType )
        {
            case PTP_TYPE_PDELAY_REQ:
            case PTP_TYPE_PDELAY_RESP:
            case PTP_TYPE_PDELAY_RESP_FU:
            {
                pCtrl->setDest( PtpMcPDelayMAC );
                break;
            }
            default:
            {
                pCtrl->setDest( PtpMcMAC );
                break;
            }
        }

        msg->setControlInfo( pCtrl );
        send( msg, LowerLayerOutGateID );
    }
}

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
PTP_EthernetMapping::PTP_EthernetMapping()
    : PtpMcMAC(PTP_ETH_MC_DEFAULT_MAC), PtpMcPDelayMAC(PTP_ETH_MC_PDELAY_MAC)
{
    Enable = true;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
PTP_EthernetMapping::SetMACAddress( MACAddress MAC )
{
    par( "MAC_Address" ).setStringValue( MAC.str() );
}
