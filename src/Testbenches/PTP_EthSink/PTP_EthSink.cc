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

#include "PTP_EthSink.h"

#include "PTP_Ethernet.h"

#include "EtherFrame_m.h"
#include "PTPv2_m.h"

// ======================================================
// Definitions
// ======================================================

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

Define_Module(PTP_EthSink);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
PTP_EthSink::initialize()
{
}

// ------------------------------------------------------
// Handle message
// ------------------------------------------------------
void
PTP_EthSink::handleMessage(cMessage *msg)
{
    EthernetIIFrame *pEth   = check_and_cast<EthernetIIFrame *>(msg);

    if( pEth->getEtherType() != PTP_ETH_TYPE)
    {
        throw cRuntimeError("PTP EthSink: Received message with wrong EthType: %d", pEth->getEtherType() );
    }

    PTPv2_Frame *pPtp   = check_and_cast<PTPv2_Frame *>(pEth->decapsulate());

    switch( pPtp->getMessageType() )
    {
        case PTP_TYPE_ANNOUNCE:         EV << "Announce"        << endl;    break;
        case PTP_TYPE_SYNC:             EV << "Sync"            << endl;    break;
        case PTP_TYPE_FOLLOW_UP:        EV << "Follow Up"       << endl;    break;
        case PTP_TYPE_DELAY_REQ:        EV << "Delay Requ"      << endl;    break;
        case PTP_TYPE_DELAY_RESP:       EV << "Delay Resp"      << endl;    break;
        case PTP_TYPE_PDELAY_REQ:       EV << "PDelay Req"      << endl;    break;
        case PTP_TYPE_PDELAY_RESP:      EV << "PDelay Resp"     << endl;    break;
        case PTP_TYPE_PDELAY_RESP_FU:   EV << "PDelay Resp FU"  << endl;    break;
        case PTP_TYPE_SIGNALING:        EV << "Signaling"       << endl;    break;
        case PTP_TYPE_MANAGEMENT:       EV << "Management"      << endl;    break;
        case PTP_TYPE_INVALID:          EV << "Invalid"         << endl;    break;
    }

    EV << "" << endl;
    EV << "General PTP information" << endl;
    EV << "Domain Number: " << (int) pPtp->getDomainNumber() << endl;
    EV << "Ingress: "       << pPtp->getIngressTimeStamp() << endl;

    switch( pPtp->getMessageType() )
    {
        case PTP_TYPE_ANNOUNCE:
        {
            PTPv2_AnnounceFrame *pAnn = check_and_cast<PTPv2_AnnounceFrame *>(pPtp);

            EV << "" << endl;
            EV << "Announce specific information" << endl;
            EV << "GrandmasterIdentity: " << pAnn->getGrandmasterIdentity().GetString() << endl;
            EV << "GrandmasterPriority1: " << (int) pAnn->getGrandmasterPriority1() << endl;
            EV << "GrandmasterPriority2: " << (int) pAnn->getGrandmasterPriority2() << endl;
            EV << "StepsRemoved: " << pAnn->getStepsRemoved() << endl;
            EV << "" << endl;

            break;
        }
        case PTP_TYPE_SYNC:
        {
            PTPv2_SyncFrame     *pSync = check_and_cast<PTPv2_SyncFrame *>(pPtp);

            EV << "" << endl;
            EV << "Sync specific information" << endl;
            EV << "SequID: " << pSync->getSequenceId() << endl;
            EV << "OriginTimestamp: " << pSync->getOriginTimestamp().GetSimTime() << endl;
            EV << "CorrectionField: " << pSync->getCorrectionField().GetSimTime() << endl;
            EV << "" << endl;

            break;
        }
        case PTP_TYPE_FOLLOW_UP:
        {
            PTPv2_Follow_UpFrame    *pFU = check_and_cast<PTPv2_Follow_UpFrame *>(pPtp);

            EV << "" << endl;
            EV << "Follow Up specific information" << endl;
            EV << "SequID: " << pFU->getSequenceId() << endl;
            EV << "preciseOriginTimestamp: " << pFU->getPreciseOriginTimestamp().GetSimTime() << endl;
            EV << "CorrectionField: " << pFU->getCorrectionField().GetSimTime() << endl;
            EV << "" << endl;

            break;
        }
        case PTP_TYPE_DELAY_REQ:
        {
            PTPv2_Delay_ReqFrame    *pDelReq = check_and_cast<PTPv2_Delay_ReqFrame *>(pPtp);

            EV << "" << endl;
            EV << "Delay Request specific information" << endl;
            EV << "SequID: " << pDelReq->getSequenceId() << endl;
            EV << "originTimestamp: " << pDelReq->getOriginTimestamp().GetSimTime() << endl;
            EV << "" << endl;

            break;
        }
        case PTP_TYPE_DELAY_RESP:
        {
            PTPv2_Delay_RespFrame   *pDelResp = check_and_cast<PTPv2_Delay_RespFrame *>(pPtp);

            EV << "" << endl;
            EV << "Delay Response specific information" << endl;
            EV << "SequID: " << pDelResp->getSequenceId() << endl;
            EV << "ReceiveTimestamp: " << pDelResp->getReceiveTimestamp().GetSimTime() << endl;
            EV << "RequestingPortIdentity: " << pDelResp->getRequestingPortIdentity().GetString() << endl;
            EV << "DelayReq EgressTime: " << pDelResp->getReqEgressTime() << endl;
            EV << "" << endl;

            break;
        }
        case PTP_TYPE_PDELAY_REQ:
        {
            PTPv2_PDelay_ReqFrame   *pPDelReq = check_and_cast<PTPv2_PDelay_ReqFrame *>(pPtp);

            EV << "" << endl;
            EV << "PDelay Request specific information" << endl;
            EV << "SequID: " << pPDelReq->getSequenceId() << endl;
            EV << "originTimestamp: " << pPDelReq->getOriginTimestamp().GetSimTime() << endl;
            EV << "" << endl;

            break;
        }
        case PTP_TYPE_PDELAY_RESP:
        {
            PTPv2_PDelay_RespFrame  *pPDelResp = check_and_cast<PTPv2_PDelay_RespFrame *>(pPtp);

            EV << "" << endl;
            EV << "PDelay Response specific information" << endl;
            EV << "SequID: " << pPDelResp->getSequenceId() << endl;
            EV << "RequestReceiptTimestamp: " << pPDelResp->getRequestReceiptTimestamp().GetSimTime() << endl;
            EV << "RequestingPortIdentity: " << pPDelResp->getRequestingPortIdentity().GetString() << endl;
            EV << "PDelayReq EgressTime: " << pPDelResp->getReqEgressTime() << endl;
            EV << "" << endl;

            break;
        }
        case PTP_TYPE_PDELAY_RESP_FU:
        {
            PTPv2_PDelay_Resp_FU_Frame  *pPDelRespFU = check_and_cast<PTPv2_PDelay_Resp_FU_Frame *>(pPtp);

            EV << "" << endl;
            EV << "PDelay Response Follow Up specific information" << endl;
            EV << "SequID: " << pPDelRespFU->getSequenceId() << endl;
            EV << "ResponseOriginTimestamp: " << pPDelRespFU->getResponseOriginTimestamp().GetSimTime() << endl;
            EV << "RequestingPortIdentity: " << pPDelRespFU->getRequestingPortIdentity().GetString() << endl;
            EV << "" << endl;

            break;
        }
        case PTP_TYPE_SIGNALING:
        {
            PTPv2_SignalingFrame  *pSig = check_and_cast<PTPv2_SignalingFrame *>(pPtp);

            EV << "" << endl;
            EV << "Signaling specific information" << endl;
            EV << "TargetPortIdentity: " << pSig->getTargetPortIdentity().GetString() << endl;
            EV << "" << endl;

            break;
        }
        case PTP_TYPE_MANAGEMENT:
        case PTP_TYPE_INVALID:
        {
            break;
        }
    }

    delete pEth;
    delete pPtp;
}
