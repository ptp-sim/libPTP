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

#include "PTP_MACRelayUnit.h"

#include <EtherFrame.h>

#include "PTP_Ethernet.h"
#include "PTP_Parser.h"

#include "PTPv2_m.h"
#include "PTP_Ctrl_m.h"

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

Define_Module( PTP_MACRelayUnit );

// ======================================================
// Definitions
// ======================================================

void
PTP_MACRelayUnit::handleAndDispatchFrame_Dispatch(EtherFrame *frame, int inputport)
{
    EthernetIIFrame *pEthFrame  = dynamic_cast<EthernetIIFrame*>(frame);

    if( internalPort >= 0 )
    {
        if( pEthFrame->getDest() == MAC )
        {
            send(frame, "ifOut", internalPort);
            return;
        }
    }

    if
    (
        ( PTP_Enable ) &&
        ( pEthFrame != nullptr ) &&
        ( PTP_ETH_TYPE == pEthFrame->getEtherType() )
    )
    {
        PTPv2_Frame     *pPtpFrame  = check_and_cast<PTPv2_Frame *>( pEthFrame->getEncapsulatedPacket() );
        tPtpMessageType MsgType     = (tPtpMessageType) pPtpFrame->getMessageType();

        // Check if frame comes from local application
        if( inputport == internalPort )
        {
            PTP_Ctrl *pCtrl  = check_and_cast<PTP_Ctrl *>(pEthFrame->getControlInfo());

            int outputport = pCtrl->getTxPort();

            if( outputport >= internalPort )
            {
                outputport++;
            }

            send(frame, "ifOut", outputport );
        }
        else if // Filter out frames for local application
        (
            ( PTP_ClockType == PTP_CLOCK_TYPE_ORDINARY  ) ||
            ( MsgType == PTP_TYPE_PDELAY_REQ            ) ||
            ( MsgType == PTP_TYPE_PDELAY_RESP           ) ||
            ( MsgType == PTP_TYPE_PDELAY_RESP_FU        )
        )
        {
            if( internalPort >= 0 )
            {
                send(frame, "ifOut", internalPort);
            }
        }
        else
        {
            multicastFrame( frame, inputport );
        }
    }
    else
    {
        MACRelayUnitBase::handleAndDispatchFrame_Dispatch( frame, inputport );
    }
}

void
PTP_MACRelayUnit::broadcastFrame(EtherFrame *frame, int inputport)
{
    // Broadcast frame
    // Ensure that original frame gets forwarded to the internal port,
    // so that the attached control info reaches the application
    for (int i=0; i<numPorts; ++i)
    {
        if(i != inputport)
        {
            EtherFrame *d = (EtherFrame*)frame->dup();

            if( frame->getControlInfo() != nullptr )
            {
                d->setControlInfo(frame->getControlInfo()->dup());
            }

            send(d, "ifOut", i);
        }
    }

    delete frame;
}

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
PTP_MACRelayUnit::PTP_MACRelayUnit()
    : MACRelayUnitBase(), IInitBase()
{
    PTP_Enable = true;
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
PTP_MACRelayUnit::ParseParameters()
{
    PTP_Enable      = par( "PTP_Enable" ).boolValue();
    PTP_ClockType   = cPTP_Parser::ParsePtpClockType( par( "PTP_ClockType" ).stringValue() );
    internalPort    = par( "internalPort" ).longValue();

    MAC.setAddress( par( "MAC_Address" ).stringValue() );

    // Write back parsed values
    par( "MAC_Address" ).setStringValue( MAC.str() );
}

int
PTP_MACRelayUnit::numInitStages() const
{
    return std::max( IInitBase::numInitStages(), MACRelayUnitBase::numInitStages() );
}

void
PTP_MACRelayUnit::initialize(int stage)
{
    // Forward call
    if( stage < MACRelayUnitBase::numInitStages() )
    {
        MACRelayUnitBase::initialize(stage);
    }

    if( stage < IInitBase::numInitStages() )
    {
        IInitBase::initialize( stage );
    }
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
PTP_MACRelayUnit::SetMACAddress( MACAddress MAC )
{
    this->MAC = MAC;

    par( "MAC_Address" ).setStringValue( MAC.str() );
}
