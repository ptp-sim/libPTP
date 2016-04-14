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

#include "PTP_EtherEncap.h"

#include "PTP_Ethernet.h"

#include "EtherFrame_m.h"
#include "PTPv2_m.h"
#include "PtpEthernet_m.h"

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

Define_Module( PTP_EtherEncap );

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Encapsulate PTP frames
// ------------------------------------------------------
void
PTP_EtherEncap::processPacketFromHigherLayer(cPacket *msg)
{
    PTPv2_Frame *pPtpFrame;

    pPtpFrame   = dynamic_cast<PTPv2_Frame *>(msg);

    if
    (
        ( PTP_Enable ) &&
        ( pPtpFrame != nullptr )
    )
    {
        EthernetIIFrame     *pEthFrame;
        tPtpMessageType     MsgType;

        MsgType = (tPtpMessageType) pPtpFrame->getMessageType();

        switch( MsgType )
        {
            case PTP_TYPE_SYNC:             pEthFrame   = new PtpEtherSync;
                                            break;

            case PTP_TYPE_DELAY_REQ:        pEthFrame   = new PtpEtherDelayReq;
                                            break;

            case PTP_TYPE_PDELAY_REQ:       pEthFrame   = new PtpEtherPDelayReq;
                                            break;

            case PTP_TYPE_PDELAY_RESP:      pEthFrame   = new PtpEtherPDelayResp;
                                            break;

            case PTP_TYPE_FOLLOW_UP:        pEthFrame   = new PtpEtherFollow;
                                            break;

            case PTP_TYPE_DELAY_RESP:       pEthFrame   = new PtpEtherDelayResp;
                                            break;

            case PTP_TYPE_PDELAY_RESP_FU:   pEthFrame   = new PtpEtherPDelayRespF;
                                            break;

            case PTP_TYPE_ANNOUNCE:         pEthFrame   = new PtpEtherAnnounce;
                                            break;

            case PTP_TYPE_SIGNALING:        pEthFrame   = new PtpEtherSignaling;
                                            break;

            case PTP_TYPE_MANAGEMENT:       pEthFrame   = new PtpEtherMgmt;
                                            break;

            default:                        pEthFrame   = new EthernetIIFrame;
                                            break;
        }

        Ieee802Ctrl *etherctrl = check_and_cast<Ieee802Ctrl*>(msg->removeControlInfo());

        // Set up Eth frame + encapsulate PTP frame
        pEthFrame->setSrc(etherctrl->getSrc());  // if blank, will be filled in by MAC
        pEthFrame->setDest(etherctrl->getDest());
        pEthFrame->setEtherType(etherctrl->getEtherType());
        pEthFrame->setByteLength(ETHER_MAC_FRAME_BYTES);

        pEthFrame->setControlInfo(etherctrl);

        pEthFrame->encapsulate( msg );

        if (pEthFrame->getByteLength() < MIN_ETHERNET_FRAME_BYTES)
        {
            pEthFrame->setByteLength(MIN_ETHERNET_FRAME_BYTES);
        }

        send( pEthFrame, "lowerLayerOut" );
    }
    else
    {
        EtherEncap::processPacketFromHigherLayer( msg );
    }
}

void
PTP_EtherEncap::processFrameFromMAC(EtherFrame *frame)
{
    EthernetIIFrame     *pEthFrame = dynamic_cast<EthernetIIFrame *>(frame);

    if( pEthFrame != nullptr )
    {
        if
        (
            ( PTP_Enable ) &&
            ( pEthFrame->getEtherType() == PTP_ETH_TYPE )
        )
        {
            // decapsulate and attach control info
            cPacket *higherlayermsg = frame->decapsulate();

            // add Ieee802Ctrl to packet
            Ieee802Ctrl *etherctrl = check_and_cast<Ieee802Ctrl *>(pEthFrame->removeControlInfo());
            etherctrl->setSrc(pEthFrame->getSrc());
            etherctrl->setDest(pEthFrame->getDest());
            etherctrl->setEtherType(pEthFrame->getEtherType());

            higherlayermsg->setControlInfo(etherctrl);

            // pass up to higher layers.
            send(higherlayermsg, "upperLayerOut");
            delete frame;
        }
        else
        {
            EtherEncap::processFrameFromMAC(frame);
        }
    }
    else
    {
        EtherEncap::processFrameFromMAC(frame);
    }
}

// ------------------------------------------------------
// Init API
// ------------------------------------------------------
void
PTP_EtherEncap::ParseParameters()
{
    PTP_Enable  = par( "PTP_Enable" ).boolValue();
}

// ------------------------------------------------------
// OMNeT API
// ------------------------------------------------------

int
PTP_EtherEncap::numInitStages() const
{
    return std::max( IInitBase::numInitStages(), EtherEncap::numInitStages() );
}

void
PTP_EtherEncap::initialize(int stage)
{
    // Forward call
    if( stage < EtherEncap::numInitStages() )
    {
        EtherEncap::initialize();
    }

    if( stage < IInitBase::numInitStages() )
    {
        IInitBase::initialize( stage );
    }
}

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
PTP_EtherEncap::PTP_EtherEncap()
    : EtherEncap(), PtpMcMAC(PTP_ETH_MC_DEFAULT_MAC), PtpMcPDelayMAC(PTP_ETH_MC_PDELAY_MAC)
{
}
