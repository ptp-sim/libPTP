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

#include "PTP_MAC.h"

#include "PTP_Parser.h"
#include "PTP_Ethernet.h"

#include "EtherFrame_m.h"
#include "PTPv2_m.h"
#include "PTP_Ctrl_m.h"

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

Define_Module(PTP_MAC);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Egress utilities
// ------------------------------------------------------

simtime_t
PTP_MAC::CalcResidenceTime( PTPv2_Frame *pPtpFrame, int inputPort )
{
    cLocalTimeStamp IngressTimeStamp    = pPtpFrame->getIngressTimeStamp();
    cLocalTimeStamp CurrentTimeStamp    = pClock->GetTimeStamp();
    simtime_t       ResidenceTime       = CurrentTimeStamp - IngressTimeStamp;
    simtime_t       PathDelay           = SIMTIME_ZERO;

    if( PTP_DelayMechanism == DELAY_MECH_P2P )
    {
        PathDelay = pNIC_Ctrl->GetMeanPathDelay( inputPort );
    }
    ResidenceTime += PathDelay;

    if( EnableDebugOutput )
    {
        EV << "Calculating ResidenceTime for " << pPtpFrame->getMessageType() << " message" << endl;
        EV << "Ingress:       " << IngressTimeStamp     << endl;
        EV << "Now:           " << CurrentTimeStamp     << endl;
        EV << "PathDelay:     " << PathDelay            << endl;
        EV << "ResidenceTime: " << ResidenceTime        << endl;
    }

    if( ResidenceTime < SIMTIME_ZERO )
    {
        throw cRuntimeError( "Frame has a negative residence time, there must something fundamentally wrong." );
    }

    return ResidenceTime;
}

void
PTP_MAC::AddCorrection( PTPv2_Frame *pPtpFrame, simtime_t Correction )
{
    pPtpFrame->setCorrectionField( pPtpFrame->getCorrectionField().GetSimTime() + Correction );
}

void
PTP_MAC::AddAsymmetry( PTPv2_Frame *pPtpFrame )
{
    AddAsymmetry( pPtpFrame, getIndex() );
}

void
PTP_MAC::AddAsymmetry( PTPv2_Frame *pPtpFrame, int inputPort )
{
    AddCorrection( pPtpFrame, pNIC_Ctrl->GetAsymmetry( inputPort ) );
}

void
PTP_MAC::SubstractAsymmetry( PTPv2_Frame *pPtpFrame )
{
    SubstractAsymmetry( pPtpFrame, getIndex() );
}

void
PTP_MAC::SubstractAsymmetry( PTPv2_Frame *pPtpFrame, int inputPort )
{
    AddCorrection( pPtpFrame, -pNIC_Ctrl->GetAsymmetry( inputPort ) );
}

// ------------------------------------------------------
// Egress of own frames
// ------------------------------------------------------

void
PTP_MAC::HandleEgressOwnPtp( PTPv2_Frame *pPtpFrame )
{
    switch( pPtpFrame->getMessageType() )
    {
        case PTP_TYPE_SYNC:             HandleEgressOwnSync( check_and_cast<PTPv2_SyncFrame *>(pPtpFrame) );
                                        break;

        case PTP_TYPE_FOLLOW_UP:        HandleEgressOwnFollowUp( check_and_cast<PTPv2_Follow_UpFrame *>(pPtpFrame) );
                                        break;

        case PTP_TYPE_DELAY_REQ:        HandleEgressOwnDelayReq( check_and_cast<PTPv2_Delay_ReqFrame *>(pPtpFrame) );
                                        break;

        case PTP_TYPE_DELAY_RESP:       HandleEgressOwnDelayResp(check_and_cast<PTPv2_Delay_RespFrame *>(pPtpFrame) );
                                        break;

        case PTP_TYPE_PDELAY_REQ:       HandleEgressOwnPDelayReq( check_and_cast<PTPv2_PDelay_ReqFrame *>(pPtpFrame) );
                                        break;

        case PTP_TYPE_PDELAY_RESP:      HandleEgressOwnPDelayResp( check_and_cast<PTPv2_PDelay_RespFrame *>(pPtpFrame) );
                                        break;


        case PTP_TYPE_PDELAY_RESP_FU:   HandleEgressOwnPDelayRespFU( check_and_cast<PTPv2_PDelay_Resp_FU_Frame*>(pPtpFrame) );
                                        break;

        default:
        case PTP_TYPE_ANNOUNCE:
        case PTP_TYPE_SIGNALING:
        case PTP_TYPE_MANAGEMENT:       // These frames don't need to be handled by the MAC at egress
                                        break;
    }
}

void
PTP_MAC::HandleEgressOwnSync( PTPv2_SyncFrame *pSync )
{
    if( PTP_TwoStepFlag != pSync->getFlagField().twoStepFlag )
    {
        HandleUnexpectedEgress( pSync, "TwoStepFlag mismatch" );
    }

    if( PTP_TwoStepFlag )
    {
        pNIC_Ctrl->SyncFuMatcher().push( pSync->getSourcePortIdentity(), pSync->getSequenceId(), pClock->GetScaledTime() );
    }
    else
    {
        pSync->setOriginTimestamp( pClock->GetScaledTime() );
    }
}

void
PTP_MAC::HandleEgressOwnFollowUp( PTPv2_Follow_UpFrame *pFollowUp )
{
    if( !PTP_TwoStepFlag )
    {
        HandleUnexpectedEgress( pFollowUp, "Egress of own follow up on 1-step clock" );
    }

    PTP_FrameMatch::MatchResult_t   res;

    res = pNIC_Ctrl->SyncFuMatcher().pop( pFollowUp->getSourcePortIdentity(), pFollowUp->getSequenceId() );

    if( res.valid )
    {
        simtime_t   SyncEgressTime = res.Time;

        pFollowUp->setPreciseOriginTimestamp( SyncEgressTime );
    }
}

void
PTP_MAC::HandleEgressOwnDelayReq( PTPv2_Delay_ReqFrame *pDelReq )
{
    if( PTP_DelayMechanism != DELAY_MECH_E2E )
    {
        HandleUnexpectedEgress( pDelReq, "Delay mechanism is not E2E" );
    }

    pNIC_Ctrl->DelayReqRespMatcher().push( PortIdentity, pDelReq->getSequenceId(), pClock->GetScaledTime() );

    // 1588 11.6.3b)
    SubstractAsymmetry(pDelReq);
}

void
PTP_MAC::HandleEgressOwnDelayResp( PTPv2_Delay_RespFrame *pDelResp )
{
    if( PTP_DelayMechanism != DELAY_MECH_E2E )
    {
        HandleUnexpectedEgress( pDelResp, "Delay mechanism is not E2E" );
    }
}

void
PTP_MAC::HandleEgressOwnPDelayReq( PTPv2_PDelay_ReqFrame *pPDelReq )
{
    if( PTP_DelayMechanism != DELAY_MECH_P2P )
    {
        HandleUnexpectedEgress( pPDelReq, "Delay mechanism is not P2P" );
    }

    // 1588 11.6.4b)
    SubstractAsymmetry(pPDelReq);

    // Remember PDelayReq Tx timestamp
    pNIC_Ctrl->PDelayReqRespMatcher().push( PortIdentity, pPDelReq->getSequenceId(), pClock->GetScaledTime() );
}

void
PTP_MAC::HandleEgressOwnPDelayResp( PTPv2_PDelay_RespFrame *pPDelResp )
{
    if( PTP_TwoStepFlag != pPDelResp->getFlagField().twoStepFlag )
    {
        HandleUnexpectedEgress( pPDelResp, "TwoStepFlag mismatch" );
    }

    // Try to get T2
    PTP_FrameMatch::MatchResult_t   res = pNIC_Ctrl->PDelayRespFuMatcher().pop( pPDelResp->getRequestingPortIdentity(),
                                                                                pPDelResp->getSequenceId() );

    if( res.valid )
    {
        // Generate T3, step b.7.ii)
        simtime_t   T3_T2   = pClock->GetScaledTime() - res.Time;

        if( PTP_TwoStepFlag )
        {
            pNIC_Ctrl->PDelayRespFuMatcher().push( pPDelResp->getRequestingPortIdentity(), pPDelResp->getSequenceId(), T3_T2 );
        }
        else
        {
            AddCorrection( pPDelResp, T3_T2 );
        }
    }
}

void
PTP_MAC::HandleEgressOwnPDelayRespFU( PTPv2_PDelay_Resp_FU_Frame *pPDelRespFU )
{
    if( !PTP_TwoStepFlag )
    {
        HandleUnexpectedEgress( pPDelRespFU, "Egress of own follow up on 1-step clock" );
    }

    pPDelRespFU->setResponseOriginTimestamp( cTimeStamp(0, 0) );

    PTP_FrameMatch::MatchResult_t   res = pNIC_Ctrl->PDelayRespFuMatcher().pop( pPDelRespFU->getRequestingPortIdentity(), pPDelRespFU->getSequenceId() );

    if( res.valid )
    {
        simtime_t   T3_T2 = res.Time;
        AddCorrection( pPDelRespFU, T3_T2 );
    }
}

// ------------------------------------------------------
// Egress of foreign frames
// ------------------------------------------------------

void
PTP_MAC::HandleEgressForeignPtp( PTPv2_Frame *pPtpFrame, int inputPort )
{
    switch( pPtpFrame->getMessageType() )
    {
        case PTP_TYPE_SYNC:             HandleEgressForeignSync( check_and_cast<PTPv2_SyncFrame *>(pPtpFrame), inputPort );
                                        break;

        case PTP_TYPE_FOLLOW_UP:        HandleEgressForeignFollowUp( check_and_cast<PTPv2_Follow_UpFrame *>(pPtpFrame), inputPort );
                                        break;

        case PTP_TYPE_DELAY_REQ:        HandleEgressForeignDelayReq( check_and_cast<PTPv2_Delay_ReqFrame *>(pPtpFrame), inputPort );
                                        break;

        case PTP_TYPE_DELAY_RESP:       HandleEgressForeignDelayResp(check_and_cast<PTPv2_Delay_RespFrame *>(pPtpFrame), inputPort );
                                        break;

        case PTP_TYPE_PDELAY_REQ:       HandleEgressForeignPDelayReq( check_and_cast<PTPv2_PDelay_ReqFrame *>(pPtpFrame), inputPort );
                                        break;

        case PTP_TYPE_PDELAY_RESP:      HandleEgressForeignPDelayResp( check_and_cast<PTPv2_PDelay_RespFrame *>(pPtpFrame), inputPort );
                                        break;


        case PTP_TYPE_PDELAY_RESP_FU:   HandleEgressForeignPDelayRespFU( check_and_cast<PTPv2_PDelay_Resp_FU_Frame*>(pPtpFrame), inputPort );
                                        break;

        default:
        case PTP_TYPE_ANNOUNCE:
        case PTP_TYPE_SIGNALING:
        case PTP_TYPE_MANAGEMENT:       // These frames don't need to be handled by the MAC at egress
                                        break;
    }
}

void
PTP_MAC::HandleEgressForeignSync( PTPv2_SyncFrame *pSync, int inputPort )
{
    if( PTP_ClockType == PTP_CLOCK_TYPE_ORDINARY )
    {
        HandleUnexpectedEgress( pSync, "Foreign sync exgress on ordinary/boundary clock" );
    }

    if( PTP_TwoStepFlag )
    {
        // Remember residence time
        simtime_t   Correction  = CalcResidenceTime( pSync, inputPort );

        if(PTP_ClockType == PTP_CLOCK_TYPE_TRANSPARENT)
        {
            // 1588 11.6.2c.1)
            // Add asymmetry of sync frame to follow up frame
            Correction += pNIC_Ctrl->GetAsymmetry( inputPort );
        }

        pNIC_Ctrl->SyncFuMatcher().push( pSync->getSourcePortIdentity(), pSync->getSequenceId(), Correction );

        // Request Follow Up if necessary
        if( pSync->getFlagField().twoStepFlag == false )
        {
            pSync->getFlagField().twoStepFlag = true;

            pNIC_Ctrl->RequestSyncFollowUp( getIndex(), pSync );
        }
    }
    else
    {
        AddCorrection( pSync, CalcResidenceTime( pSync, inputPort ) );
    }
}

void
PTP_MAC::HandleEgressForeignFollowUp( PTPv2_Follow_UpFrame *pFollowUp, int inputPort )
{
    if( PTP_ClockType == PTP_CLOCK_TYPE_ORDINARY )
    {
        HandleUnexpectedEgress( pFollowUp, "Foreign follow up exgress on ordinary/boundary clock" );
    }

    if( PTP_TwoStepFlag )
    {
        PTP_FrameMatch::MatchResult_t   res = pNIC_Ctrl->SyncFuMatcher().pop( pFollowUp->getSourcePortIdentity(), pFollowUp->getSequenceId() );

        if( res.valid )
        {
            AddCorrection( pFollowUp, res.Time );
        }
    }
}

void
PTP_MAC::HandleEgressForeignDelayReq( PTPv2_Delay_ReqFrame *pDelReq, int inputPort )
{
    if( PTP_ClockType == PTP_CLOCK_TYPE_ORDINARY )
    {
        HandleUnexpectedEgress( pDelReq, "Foreign E2E exgress on ordinary/boundary clock" );
    }

    if
    (
        ( PTP_ClockType == PTP_CLOCK_TYPE_TRANSPARENT ) &&
        ( PTP_DelayMechanism == DELAY_MECH_P2P )
    )
    {
        HandleUnexpectedEgress( pDelReq, "Foreign E2E egress on P2P transparent clock" );
    }

    simtime_t Correction;
    Correction =  CalcResidenceTime( pDelReq, inputPort );
    Correction -= pNIC_Ctrl->GetAsymmetry( getIndex() );    // 1588 11.6.3c.1) + 11.6.3c.2)

    if( PTP_TwoStepFlag )
    {
        pNIC_Ctrl->DelayReqRespMatcher().push( pDelReq->getSourcePortIdentity(), pDelReq->getSequenceId(), Correction );
    }
    else
    {
        AddCorrection( pDelReq, Correction );
    }
}

void
PTP_MAC::HandleEgressForeignDelayResp( PTPv2_Delay_RespFrame *pDelResp, int inputPort )
{
    if( PTP_ClockType == PTP_CLOCK_TYPE_ORDINARY )
    {
        HandleUnexpectedEgress( pDelResp, "Foreign E2E on ordinary/boundary clock" );
    }

    if
    (
        ( PTP_ClockType == PTP_CLOCK_TYPE_TRANSPARENT ) &&
        ( PTP_DelayMechanism == DELAY_MECH_P2P )
    )
    {
        HandleUnexpectedEgress( pDelResp, "Foreign E2E egress on P2P transparent clock" );
    }

    // Add correction of DelayReq's residence time
    if( PTP_TwoStepFlag )
    {
        PTP_FrameMatch::MatchResult_t   res = pNIC_Ctrl->DelayReqRespMatcher().pop( pDelResp->getRequestingPortIdentity(), pDelResp->getSequenceId() );

        if( res.valid )
        {
            AddCorrection( pDelResp, res.Time );
        }
    }
}

void
PTP_MAC::HandleEgressForeignPDelayReq( PTPv2_PDelay_ReqFrame *pPDelReq, int inputPort )
{
    if( PTP_ClockType == PTP_CLOCK_TYPE_ORDINARY )
    {
        HandleUnexpectedEgress( pPDelReq, "Foreign P2P exgress on ordinary/boundary clock" );
    }

    if
    (
        ( PTP_ClockType == PTP_CLOCK_TYPE_TRANSPARENT ) &&
        ( PTP_DelayMechanism == DELAY_MECH_P2P )
    )
    {
        HandleUnexpectedEgress( pPDelReq, "Foreign P2P egress on P2P transparent clock" );
    }

    // 1588 11.6.4b)
    SubstractAsymmetry(pPDelReq);

    simtime_t   ResidenceTime = CalcResidenceTime(pPDelReq, inputPort);
    if( PTP_TwoStepFlag )
    {
        pNIC_Ctrl->PDelayRespFuMatcher().push( pPDelReq->getSourcePortIdentity(), pPDelReq->getSequenceId(), ResidenceTime );
    }
    else
    {
        AddCorrection( pPDelReq, ResidenceTime );
    }
}

void
PTP_MAC::HandleEgressForeignPDelayResp( PTPv2_PDelay_RespFrame *pPDelResp, int inputPort )
{
    if( PTP_ClockType == PTP_CLOCK_TYPE_ORDINARY )
    {
        HandleUnexpectedEgress( pPDelResp, "Foreign P2P exgress on ordinary/boundary clock" );
    }

    if
    (
        ( PTP_ClockType == PTP_CLOCK_TYPE_TRANSPARENT ) &&
        ( PTP_DelayMechanism == DELAY_MECH_P2P )
    )
    {
        HandleUnexpectedEgress( pPDelResp, "Foreign P2P egress on P2P transparent clock" );
    }

    if( PTP_TwoStepFlag )
    {
        // Latch residence time of both Request + Response
        PTP_FrameMatch::MatchResult_t   res = pNIC_Ctrl->PDelayRespFuMatcher().pop( pPDelResp->getSourcePortIdentity(), pPDelResp->getSequenceId() );

        if( res.valid )
        {
            simtime_t   PDelayReqResTime = res.Time;

            pNIC_Ctrl->PDelayRespFuMatcher().push( pPDelResp->getSourcePortIdentity(), pPDelResp->getSequenceId(), PDelayReqResTime + CalcResidenceTime(pPDelResp, inputPort) );
        }

        // Request Follow Up if necessary
        if( pPDelResp->getFlagField().twoStepFlag == false )
        {
            pPDelResp->getFlagField().twoStepFlag = true;

            pNIC_Ctrl->RequestPDelayFollowUp( getIndex(), pPDelResp );
        }
    }
    else
    {
        AddCorrection( pPDelResp, CalcResidenceTime(pPDelResp, inputPort) );
    }
}

void
PTP_MAC::HandleEgressForeignPDelayRespFU( PTPv2_PDelay_Resp_FU_Frame *pPDelRespFU, int inputPort )
{
    if( PTP_ClockType == PTP_CLOCK_TYPE_ORDINARY )
    {
        HandleUnexpectedEgress( pPDelRespFU, "Foreign P2P exgress on ordinary/boundary clock" );
    }

    if
    (
        ( PTP_ClockType == PTP_CLOCK_TYPE_TRANSPARENT ) &&
        ( PTP_DelayMechanism == DELAY_MECH_P2P )
    )
    {
        HandleUnexpectedEgress( pPDelRespFU, "Foreign P2P egress on P2P transparent clock" );
    }

    // Correct residence time of Requ and Resp
    PTP_FrameMatch::MatchResult_t res = pNIC_Ctrl->PDelayRespFuMatcher().pop( pPDelRespFU->getSourcePortIdentity(), pPDelRespFU->getSequenceId() );

    if( res.valid )
    {
        AddCorrection( pPDelRespFU, res.Time );
    }
}

// -------------------------------------------------
// PTP specific extension
// Ingress handling
// -------------------------------------------------
void
PTP_MAC::TimestampOnIngress( PTPv2_Frame *pPtpFrame )
{
    pPtpFrame->setIngressTimeStamp( pClock->GetTimeStamp() );
}

void
PTP_MAC::HandleIngressAnnounce( PTPv2_AnnounceFrame *pAnn )
{
    TimestampOnIngress( pAnn );
}

void
PTP_MAC::HandleIngressSync( PTPv2_SyncFrame *pSync )
{
    // 1588 11.6.2b)
    if( PTP_ClockType == PTP_CLOCK_TYPE_ORDINARY )
    {
        AddAsymmetry( pSync );
    }

    // 1588 11.6.2c)
    if( PTP_ClockType == PTP_CLOCK_TYPE_TRANSPARENT )
    {
        // 1588 11.6.2c.1)
        if( PTP_TwoStepFlag == false )
        {
            AddAsymmetry( pSync );
        }
    }

    TimestampOnIngress( pSync );
}

void
PTP_MAC::HandleIngressDelayReq( PTPv2_Delay_ReqFrame *pDelReq )
{
    TimestampOnIngress( pDelReq );
}

void
PTP_MAC::HandleIngressDelayResp( PTPv2_Delay_RespFrame *pDelResp )
{
    if( PortIdentity == pDelResp->getRequestingPortIdentity() )
    {
        PTP_FrameMatch::MatchResult_t res = pNIC_Ctrl->DelayReqRespMatcher().pop( PortIdentity, pDelResp->getSequenceId() );

        if( res.valid )
        {
            simtime_t   DelayReqEgress = res.Time;

            pDelResp->setReqEgressTime( DelayReqEgress );
        }
    }
}

void
PTP_MAC::HandleIngressPDelayReq( PTPv2_PDelay_ReqFrame *pPDelReq )
{
    // Generate T2, step b.1)
    pNIC_Ctrl->PDelayRespFuMatcher().push( pPDelReq->getSourcePortIdentity(), pPDelReq->getSequenceId(), pClock->GetScaledTime() );
}

void
PTP_MAC::HandleIngressPDelayResp( PTPv2_PDelay_RespFrame *pPDelResp )
{
    // Generate T4, step d.1)
    TimestampOnIngress( pPDelResp );

    // 1588 11.6.5b)
    AddAsymmetry(pPDelResp);

    if( PortIdentity == pPDelResp->getRequestingPortIdentity() )
    {
        PTP_FrameMatch::MatchResult_t res = pNIC_Ctrl->PDelayReqRespMatcher().pop( PortIdentity, pPDelResp->getSequenceId() );

        if( res.valid )
        {
            simtime_t   PDelayReqEgress = res.Time;

            pPDelResp->setReqEgressTime( PDelayReqEgress );
        }
    }
}

void
PTP_MAC::HandleEgress()
{
    EthernetIIFrame *pEthFrame  = dynamic_cast<EthernetIIFrame *>(curTxFrame);

    if( pEthFrame != nullptr )
    {
        if( PTP_ETH_TYPE == pEthFrame->getEtherType() )
        {
            PTPv2_Frame *pPtp   = check_and_cast<PTPv2_Frame*>(pEthFrame->getEncapsulatedPacket());

            if( PortIdentity == pPtp->getSourcePortIdentity() )
            {
                HandleEgressOwnPtp( pPtp );
            }
            else
            {
                PTP_Ctrl *pCtrl = check_and_cast<PTP_Ctrl *>(pEthFrame->removeControlInfo());
                int inputPort   = pCtrl->getRxPort();

                delete pCtrl;

                HandleEgressForeignPtp( pPtp, inputPort );
            }
        }
    }
}

PTP_MAC::FrameHandling_t
PTP_MAC::HandleIngress( EtherFrame *pEthFrame )
{
    // Filter out our own frames
    // We could receive them in a ring without STP
    if( pEthFrame->getSrc() == address )
    {
        return DROP_FRAME;
    }

    PTPv2_Frame     *pPtpFrame;

    pPtpFrame = dynamic_cast<PTPv2_Frame*>(pEthFrame->getEncapsulatedPacket());

    if( pPtpFrame != nullptr )
    {
        tPtpMessageType MsgType  = (tPtpMessageType) pPtpFrame->getMessageType();

        // Dump frames of wrong delay mode
        if
        (
            (
                ( DELAY_MECH_P2P == PTP_DelayMechanism )    &&
                (
                    ( MsgType == PTP_TYPE_DELAY_REQ     ) ||
                    ( MsgType == PTP_TYPE_DELAY_RESP    )
                )
            ) ||
            (
                ( DELAY_MECH_E2E == PTP_DelayMechanism )    &&
                (
                    ( MsgType == PTP_TYPE_PDELAY_REQ        ) ||
                    ( MsgType == PTP_TYPE_PDELAY_RESP       ) ||
                    ( MsgType == PTP_TYPE_PDELAY_RESP_FU    )
                )
            )
        )
        {
            return DROP_FRAME;
        }

        assert( pEthFrame->getControlInfo() == nullptr );

        PTP_Ctrl *pCtrl = new PTP_Ctrl;

        pCtrl->setRxPort( this->getIndex() );
        pEthFrame->setControlInfo( pCtrl );

        // Handle ingress event
        switch( MsgType )
        {
            case PTP_TYPE_ANNOUNCE:     HandleIngressAnnounce( check_and_cast<PTPv2_AnnounceFrame *>(pPtpFrame) );
                                        break;

            case PTP_TYPE_SYNC:         HandleIngressSync( check_and_cast<PTPv2_SyncFrame *>(pPtpFrame) );
                                        break;

            case PTP_TYPE_DELAY_REQ:    HandleIngressDelayReq( check_and_cast<PTPv2_Delay_ReqFrame *>(pPtpFrame) );
                                        break;

            case PTP_TYPE_DELAY_RESP:   HandleIngressDelayResp( check_and_cast<PTPv2_Delay_RespFrame *>(pPtpFrame) );
                                        break;

            case PTP_TYPE_PDELAY_REQ:   HandleIngressPDelayReq( check_and_cast<PTPv2_PDelay_ReqFrame *>(pPtpFrame) );
                                        break;

            case PTP_TYPE_PDELAY_RESP:  HandleIngressPDelayResp( check_and_cast<PTPv2_PDelay_RespFrame *>(pPtpFrame) );
                                        break;

            default:
            case PTP_TYPE_FOLLOW_UP:

            case PTP_TYPE_PDELAY_RESP_FU:
            case PTP_TYPE_SIGNALING:
            case PTP_TYPE_MANAGEMENT:
            {
                // Ignore other PTP frames
                break;
            }
        }
    }

    return HANDLE_FRAME;
}

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
PTP_MAC::PTP_MAC()
    : EtherMAC(), IInitBase()
{
    PTP_Enable  = true;

    FaultMsg    = NULL;
    RecoveryMsg = NULL;

    EnableDebugOutput   = false;
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
PTP_MAC::ParseResourceParameters()
{
    PTP_Enable          = par( "PTP_Enable" ).boolValue();
    ClockPath           = par("ClockPath").stringValue();
    PTP_NIC_CtrlPath    = par("PTP_NIC_CtrlPath").stringValue();
}

void
PTP_MAC::AllocateResources()
{
    if( PTP_Enable )
    {
        pClock      = check_and_cast<cScheduleClock *>(getModuleByPath( ClockPath.c_str() ));
        pNIC_Ctrl   = check_and_cast<PTP_NIC_Ctrl *>(getModuleByPath( PTP_NIC_CtrlPath.c_str() ));
    }
}

void
PTP_MAC::ParseParameters()
{
    if( PTP_Enable )
    {
        PTP_ClockType       = cPTP_Parser::ParsePtpClockType( par( "PTP_ClockType" ).stringValue() );
        PTP_DelayMechanism  = cPTP_Parser::ParseDelayMechanism( par( "PTP_DelayMechanism" ).stringValue() );
        PTP_TwoStepFlag     = par( "PTP_TwoStepFlag" ).boolValue();

        if( par( "SimulateFault" ).boolValue() )
        {
            simtime_t   FaultTime       = simtime_t( par( "FaultTime" ).doubleValue() );
            simtime_t   FaultDuration   = simtime_t( par( "FaultDuration" ).doubleValue() );

            if( FaultTime < SIMTIME_ZERO )
            {
                error( "Real time value for the simulated fault occurrence must be strictly positive." );
            }

            if( FaultDuration < SIMTIME_ZERO )
            {
                error( "Real time value for the simulated fault duration must be strictly positive." );
            }

            FaultMsg       = new cMessage( "MAC Fault Event"    );
            RecoveryMsg    = new cMessage( "MAC Recovery Event" );

            scheduleAt( FaultTime, FaultMsg );
            scheduleAt( FaultTime + FaultDuration, RecoveryMsg );
        }

        EnableDebugOutput = par( "EnableDebugOutput" ).boolValue();
    }
}

void
PTP_MAC::InitInternalState()
{
    if( PTP_Enable )
    {
        PortIdentity.ClockIdentity() = address;
        PortIdentity.SetPortNumber( getIndex() + 1 );
    }
}

void
PTP_MAC::handleSelfMessage(cMessage *msg)
{
    if( msg == FaultMsg )
    {
        updateOperationalFlag(false);
        delete msg;
        FaultMsg = NULL;

        pNIC_Ctrl->ReportFault( getIndex() );
    }
    else
    {
        EtherMAC::handleSelfMessage( msg );
    }
}

void
PTP_MAC::handleMessageWhenDown(cMessage *msg)
{
    if( msg == RecoveryMsg )
    {
        updateOperationalFlag(true);
        RecoveryMsg = NULL;

        pNIC_Ctrl->ReportRecovery( getIndex() );
    }

    delete msg;
}

void
PTP_MAC::HandleUnexpectedEgress( PTPv2_Frame *pFrame, std::string Reason )
{
    std::stringstream ss;

    ss << "Error: Caught unexpected frame exgress!" << endl;
    ss << endl;
    ss << "Reason: " << Reason << endl;
    ss << endl;
    ss << "------------------------------------------" << endl;
    ss << "Debug info: " << endl;
    ss << "------------------------------------------" << endl;
    ss << "Frame type: ";
    switch( (tPtpMessageType) pFrame->getMessageType() )
    {
        case PTP_TYPE_SYNC:             ss << "Sync"                << endl; break;
        case PTP_TYPE_DELAY_REQ:        ss << "DelayReq"            << endl; break;
        case PTP_TYPE_PDELAY_REQ:       ss << "PDelayReq"           << endl; break;
        case PTP_TYPE_PDELAY_RESP:      ss << "PDeleayResp"         << endl; break;
        case PTP_TYPE_FOLLOW_UP:        ss << "FollowUp"            << endl; break;
        case PTP_TYPE_DELAY_RESP:       ss << "DelayResp"           << endl; break;
        case PTP_TYPE_PDELAY_RESP_FU:   ss << "PDelayRespFollowUp"  << endl; break;
        case PTP_TYPE_ANNOUNCE:         ss << "Announce"            << endl; break;
        case PTP_TYPE_SIGNALING:        ss << "Signaling"           << endl; break;
        case PTP_TYPE_MANAGEMENT:       ss << "Management"          << endl; break;
        case PTP_TYPE_INVALID:          ss << "Invalid"             << endl; break;
    }
    ss << "ClockType: "                     << PTP_ClockType << endl;
    ss << "Local twoStepFlag: "             << PTP_TwoStepFlag << endl;
    ss << "Frame's twoStepFlag: "           << pFrame->getFlagField().twoStepFlag << endl;
    ss << "DelayMechanism: "                << PTP_DelayMechanism << endl;
    ss << "Local port identity: "           << PortIdentity << endl;
    ss << "Frame's source port identity: "  << pFrame->getSourcePortIdentity() << endl;

    const std::string tmp = ss.str();

    throw cRuntimeError( tmp.c_str() );
}

int
PTP_MAC::numInitStages() const
{
    return std::max( IInitBase::numInitStages(), EtherMAC::numInitStages() );
}

void
PTP_MAC::initialize(int stage)
{
    // MACBase does not play well with InitBase, as it relies on the value of numInitStages
    // and has code that is executed when stage == numInitStage-1.
    // Thus we must forward as many stages as there are in IInitBase::numInitStages, not only
    // as many as EtherMAC::numInitStages() requests.
    if( stage < IInitBase::numInitStages() )
    {
        EtherMAC::initialize( stage );
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
PTP_MAC::SetMACAddress( MACAddress MAC )
{
    address                      = MAC;
    PortIdentity.ClockIdentity() = MAC;

    par( "address" ).setStringValue( MAC.str() );
}

// ------------------------------------------------------
// Derived functions of base class
// ------------------------------------------------------
void
PTP_MAC::startFrameTransmission()
{
    if( PTP_Enable )
    {
        HandleEgress();
    }

    EtherMAC::startFrameTransmission();
}

void
PTP_MAC::processMsgFromNetwork(EtherTraffic *msg)
{
    if( PTP_Enable )
    {
        EtherFrame *frame = dynamic_cast<EtherFrame *>(msg);

        if( frame != nullptr )
        {
            FrameHandling_t FrameHandling;

            FrameHandling = HandleIngress( frame );

            if( FrameHandling == DROP_FRAME )
            {
                delete frame;
                return;
            }
        }
    }

    EtherMAC::processMsgFromNetwork( msg );
}
