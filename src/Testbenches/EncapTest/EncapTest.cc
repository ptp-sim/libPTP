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

#include "EncapTest.h"

#include "PTP_Ethernet.h"

#include "IPProtocolId_m.h"
#include "IPv4Datagram.h"
#include "IPv4Address.h"
#include "Ieee802Ctrl_m.h"

// ======================================================
// Types
// ======================================================

typedef enum
{
    TEST_SEND_PTP_ANNOUNCE,
    TEST_SEND_PTP_SYNC,
    TEST_SEND_PTP_FOLLOW_UP,
    TEST_SEND_PTP_DELAY_REQ,
    TEST_SEND_PTP_DELAY_RESP,
    TEST_SEND_PTP_PDELAY_REQ,
    TEST_SEND_PTP_PDELAY_RESP,
    TEST_SEND_PTP_PDELAY_RESP_FU,
    TEST_SEND_PTP_SIGNALING,
    TEST_SEND_PTP_MANAGEMENT,
    TEST_SEND_IP,
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

static tTestSchedule   TestSchedule[]    =
{
    {   1,   TEST_SEND_PTP_ANNOUNCE        },
    {   2,   TEST_SEND_PTP_SYNC            },
    {   3,   TEST_SEND_PTP_FOLLOW_UP       },
    {   4,   TEST_SEND_PTP_DELAY_REQ       },
    {   5,   TEST_SEND_PTP_DELAY_RESP      },
    {   6,   TEST_SEND_PTP_PDELAY_REQ      },
    {   7,   TEST_SEND_PTP_PDELAY_RESP     },
    {   8,   TEST_SEND_PTP_PDELAY_RESP_FU  },
    {   9,   TEST_SEND_PTP_SIGNALING       },
    {   10,  TEST_SEND_PTP_MANAGEMENT      },
    {   11,  TEST_SEND_IP                  },
};

// ======================================================
// Declarations
// ======================================================

Define_Module(EncapTest);

// ======================================================
// Definitions
// ======================================================

void
EncapTest::SendPtpMsg( tPtpMessageType MsgType )
{
    Ieee802Ctrl         *pCtrl  = new Ieee802Ctrl;
    PTPv2_Frame         *pPtp;

    switch( MsgType )
    {
        case PTP_TYPE_ANNOUNCE:         pPtp = new PTPv2_AnnounceFrame;             break;
        case PTP_TYPE_SYNC:             pPtp = new PTPv2_SyncFrame;                 break;
        case PTP_TYPE_FOLLOW_UP:        pPtp = new PTPv2_Follow_UpFrame;            break;
        case PTP_TYPE_DELAY_REQ:        pPtp = new PTPv2_Delay_ReqFrame;            break;
        case PTP_TYPE_DELAY_RESP:       pPtp = new PTPv2_Delay_RespFrame;           break;
        case PTP_TYPE_PDELAY_REQ:       pPtp = new PTPv2_PDelay_ReqFrame;           break;
        case PTP_TYPE_PDELAY_RESP:      pPtp = new PTPv2_PDelay_RespFrame;          break;
        case PTP_TYPE_PDELAY_RESP_FU:   pPtp = new PTPv2_PDelay_Resp_FU_Frame;      break;
        case PTP_TYPE_SIGNALING:        pPtp = new PTPv2_SignalingFrame;            break;
        case PTP_TYPE_MANAGEMENT:       pPtp = new PTPv2_ManagementFrame;           break;

        default:
        case PTP_TYPE_INVALID:
        {
            return;
        }
    }

    pPtp->setMessageType( MsgType );

    pCtrl->setSrc( SrcMAC );
    pCtrl->setEtherType( PTP_ETH_TYPE );

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

    pPtp->setControlInfo( pCtrl );

    send( pPtp, "upperLayerOut" );
}

void
EncapTest::SendIpMsg()
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

    send( pIP, "upperLayerOut" );
}

void
EncapTest::initialize()
{
    // Get resources

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
}

void EncapTest::handleMessage(cMessage *msg)
{
    if( msg->isSelfMessage() )
    {
        EV << "Starting test " << msg->getKind() << endl;

        switch( msg->getKind() )
        {
            case TEST_SEND_PTP_SYNC:                SendPtpMsg( PTP_TYPE_SYNC           );      break;
            case TEST_SEND_PTP_DELAY_REQ:           SendPtpMsg( PTP_TYPE_DELAY_REQ      );      break;
            case TEST_SEND_PTP_PDELAY_REQ:          SendPtpMsg( PTP_TYPE_PDELAY_REQ     );      break;
            case TEST_SEND_PTP_PDELAY_RESP:         SendPtpMsg( PTP_TYPE_PDELAY_RESP    );      break;
            case TEST_SEND_PTP_FOLLOW_UP:           SendPtpMsg( PTP_TYPE_FOLLOW_UP      );      break;
            case TEST_SEND_PTP_DELAY_RESP:          SendPtpMsg( PTP_TYPE_DELAY_RESP     );      break;
            case TEST_SEND_PTP_PDELAY_RESP_FU:      SendPtpMsg( PTP_TYPE_PDELAY_RESP_FU );      break;
            case TEST_SEND_PTP_ANNOUNCE:            SendPtpMsg( PTP_TYPE_ANNOUNCE       );      break;
            case TEST_SEND_PTP_SIGNALING:           SendPtpMsg( PTP_TYPE_SIGNALING      );      break;
            case TEST_SEND_PTP_MANAGEMENT:          SendPtpMsg( PTP_TYPE_MANAGEMENT     );      break;
            case TEST_SEND_IP:                      SendIpMsg();                                break;
        }

        delete msg;
    }
    else
    {
        delete msg;
    }
}

EncapTest::EncapTest()
    : SrcMAC( "C0:FF:EE:C0:FF:EE" ), PtpMcMAC(PTP_ETH_MC_DEFAULT_MAC), PtpMcPDelayMAC(PTP_ETH_MC_PDELAY_MAC)
{
}
