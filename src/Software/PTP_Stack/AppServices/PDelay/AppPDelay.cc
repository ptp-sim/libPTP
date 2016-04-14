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

#include "PTP_Stack.h"
#include "AppPDelay.h"
#include "PTP_Port.h"

#include "PTPv2_m.h"

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

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Private methods
// ------------------------------------------------------

PTPv2_PDelay_ReqFrame *
cAppPDelay::CreatePDelayReqFrame()
{
    // Delay Req message specific stuff, 13.9.2
    PTPv2_PDelay_ReqFrame *pReq  = new PTPv2_PDelay_ReqFrame;

    // Precise timestamp will be filled in by MAC, thus it is set to 0 here
    pReq->setOriginTimestamp( cTimeStamp(0,0) );

    for( unsigned int i = 0; i < pReq->getReservedDelayReqArraySize(); i++ )
    {
        pReq->setReservedDelayReq( i, 0);
    }

    // Generic PTP header, 13.3.2
    pReq->setTransportSpecific(0);
    pReq->setMessageType(PTP_TYPE_PDELAY_REQ);
    pReq->setReserved_0(0);
    pReq->setVersionPTP(pPort->PortDS().GetVersionNumber());
    pReq->setMessageLength(PTP_MSG_PDEL_REQ_LEN);
    pReq->setDomainNumber(pApp->defaultDS.GetDomainNumber());
    pReq->setReserved_1(0);

    cPtpHeaderFlags&    FlagField   = pReq->getFlagField();

    FlagField.alternateMasterFlag   = false;    // Alternate master currently not supported
    FlagField.twoStepFlag           = false;
    FlagField.unicastFlag           = false;    // Unicast is currently not supported
    FlagField.ptpProfileSpecific_1  = false;
    FlagField.ptpProfileSpecific_1  = false;
    FlagField.reserved              = false;
    FlagField.leap59                = pApp->timePropertiesDS.GetLeap59();
    FlagField.leap61                = pApp->timePropertiesDS.GetLeap61();
    FlagField.currentUtcOffsetValid = pApp->timePropertiesDS.GetCurrentUtcOffsetValid();
    FlagField.ptpTimescale          = pApp->timePropertiesDS.GetPtpTimescale();
    FlagField.timeTraceable         = pApp->timePropertiesDS.GetTimeTraceable();
    FlagField.frequencyTraceable    = pApp->timePropertiesDS.GetFrequencyTraceable();

    pReq->setCorrectionField( cTimeInterval() );   // Table 21

    for( unsigned int i = 0; i < pReq->getReserved_2ArraySize(); i++ )
    {
        pReq->setReserved_2( i, 0);
    }

    pReq->setSourcePortIdentity( pPort->PortDS().PortIdentity() );
    pReq->setSequenceId( DrawSequId() );
    pReq->setControlField( PTP_MSG_CTRL_OTHERS );
    pReq->setLogMessageInterval( 0x7F );        // Table 24

    // OMNeT specific stuff
    pReq->setByteLength( PTP_MSG_PDEL_REQ_LEN );

    return  pReq;
}

PTPv2_PDelay_RespFrame *
cAppPDelay::CreatePDelayRespFrame( PTPv2_PDelay_ReqFrame *pReq )
{
    // Delay Resp message specific stuff, 13.10.2
    PTPv2_PDelay_RespFrame *pResp   = new PTPv2_PDelay_RespFrame;

    pResp->setRequestReceiptTimestamp( cTimeStamp(0,0) ); // This is also set to 0 in case of twoStepFlag == true (first implementation approach, see 11.4.3)
    pResp->setRequestingPortIdentity( pReq->getSourcePortIdentity() );

    // Remember timestamps of pReq for future calculations
    pResp->setReqEgressTime( SIMTIME_ZERO );   // Will be set by receiver's MAC on ingress

    // Generic PTP header, 13.3.2
    pResp->setTransportSpecific(0);
    pResp->setMessageType(PTP_TYPE_PDELAY_RESP);
    pResp->setReserved_0(0);
    pResp->setVersionPTP(pPort->PortDS().GetVersionNumber());
    pResp->setMessageLength(PTP_MSG_PDEL_RESP_LEN);
    pResp->setDomainNumber( pReq->getDomainNumber() );
    pResp->setReserved_1(0);

    cPtpHeaderFlags&    FlagField   = pResp->getFlagField();

    FlagField.alternateMasterFlag   = false;    // Alternate master currently not supported
    FlagField.twoStepFlag           = pApp->defaultDS.GetTwoStepFlag();
    FlagField.unicastFlag           = false;    // Unicast is currently not supported
    FlagField.ptpProfileSpecific_1  = false;
    FlagField.ptpProfileSpecific_1  = false;
    FlagField.reserved              = false;
    FlagField.leap59                = pApp->timePropertiesDS.GetLeap59();
    FlagField.leap61                = pApp->timePropertiesDS.GetLeap61();
    FlagField.currentUtcOffsetValid = pApp->timePropertiesDS.GetCurrentUtcOffsetValid();
    FlagField.ptpTimescale          = pApp->timePropertiesDS.GetPtpTimescale();
    FlagField.timeTraceable         = pApp->timePropertiesDS.GetTimeTraceable();
    FlagField.frequencyTraceable    = pApp->timePropertiesDS.GetFrequencyTraceable();

    if( pApp->defaultDS.GetTwoStepFlag() )
    {
        pResp->setCorrectionField( cTimeInterval() );
    }
    else
    {
        pResp->setCorrectionField( pReq->getCorrectionField() );   // 11.3.2
    }

    for( unsigned int i = 0; i < pResp->getReserved_2ArraySize(); i++ )
    {
        pResp->setReserved_2( i, 0);
    }

    pResp->setSourcePortIdentity( pPort->PortDS().PortIdentity() );
    pResp->setSequenceId( pReq->getSequenceId() );
    pResp->setControlField( PTP_MSG_CTRL_OTHERS );
    pResp->setLogMessageInterval( 0x7F );        // Table 24

    // OMNeT specific stuff
    pResp->setByteLength( PTP_MSG_PDEL_RESP_LEN );

    return  pResp;
}

PTPv2_PDelay_Resp_FU_Frame *
cAppPDelay::CreatePDelayRespFUFrame( PTPv2_PDelay_ReqFrame *pReq )
{
    // Delay Resp message specific stuff, 13.11.2
    PTPv2_PDelay_Resp_FU_Frame *pRespFU   = new PTPv2_PDelay_Resp_FU_Frame;

    pRespFU->setResponseOriginTimestamp( cTimeStamp(0,0) ); // This is always set to 0 (first implementation approach, see 11.4.3)
    pRespFU->setRequestingPortIdentity( pReq->getSourcePortIdentity() );

    // Generic PTP header, 13.3.2
    pRespFU->setTransportSpecific(0);
    pRespFU->setMessageType(PTP_TYPE_PDELAY_RESP_FU);
    pRespFU->setReserved_0(0);
    pRespFU->setVersionPTP(pPort->PortDS().GetVersionNumber());
    pRespFU->setMessageLength(PTP_MSG_PDEL_RESP_FU_LEN);
    pRespFU->setDomainNumber( pReq->getDomainNumber() );
    pRespFU->setReserved_1(0);

    cPtpHeaderFlags&    FlagField   = pRespFU->getFlagField();

    FlagField.alternateMasterFlag   = false;    // Alternate master currently not supported
    FlagField.twoStepFlag           = true;
    FlagField.unicastFlag           = false;    // Unicast is currently not supported
    FlagField.ptpProfileSpecific_1  = false;
    FlagField.ptpProfileSpecific_1  = false;
    FlagField.reserved              = false;
    FlagField.leap59                = pApp->timePropertiesDS.GetLeap59();
    FlagField.leap61                = pApp->timePropertiesDS.GetLeap61();
    FlagField.currentUtcOffsetValid = pApp->timePropertiesDS.GetCurrentUtcOffsetValid();
    FlagField.ptpTimescale          = pApp->timePropertiesDS.GetPtpTimescale();
    FlagField.timeTraceable         = pApp->timePropertiesDS.GetTimeTraceable();
    FlagField.frequencyTraceable    = pApp->timePropertiesDS.GetFrequencyTraceable();

    pRespFU->setCorrectionField( pReq->getCorrectionField() );   // 11.3.2

    for( unsigned int i = 0; i < pRespFU->getReserved_2ArraySize(); i++ )
    {
        pRespFU->setReserved_2( i, 0);
    }

    pRespFU->setSourcePortIdentity( pPort->PortDS().PortIdentity() );
    pRespFU->setSequenceId( pReq->getSequenceId() );
    pRespFU->setControlField( PTP_MSG_CTRL_OTHERS );
    pRespFU->setLogMessageInterval( 0x7F );        // Table 24

    // OMNeT specific stuff
    pRespFU->setByteLength( PTP_MSG_PDEL_RESP_FU_LEN );

    return  pRespFU;
}

PTPv2_PDelay_Resp_FU_Frame *
cAppPDelay::CreatePDelayRespFUFrame( PTPv2_PDelay_RespFrame *pResp )
{
    // Delay Resp FU creation, according to 11.5.4.3
    PTPv2_PDelay_Resp_FU_Frame *pRespFU   = new PTPv2_PDelay_Resp_FU_Frame;

    pRespFU->setResponseOriginTimestamp( cTimeStamp(0,0) ); // This is always set to 0 (first implementation approach, see 11.4.3)
    pRespFU->setRequestingPortIdentity( pResp->getSourcePortIdentity() );

    // Generic PTP header, 13.3.2
    pRespFU->setTransportSpecific(0);
    pRespFU->setMessageType(PTP_TYPE_PDELAY_RESP_FU);
    pRespFU->setReserved_0(0);
    pRespFU->setVersionPTP( pResp->getVersionPTP() );
    pRespFU->setMessageLength(PTP_MSG_PDEL_RESP_FU_LEN);
    pRespFU->setDomainNumber( pResp->getDomainNumber() );
    pRespFU->setReserved_1(0);
    pRespFU->setFlagField( pResp->getFlagField() );

    pRespFU->setCorrectionField( cTimeInterval(0) );

    for( unsigned int i = 0; i < pRespFU->getReserved_2ArraySize(); i++ )
    {
        pRespFU->setReserved_2( i, 0);
    }

    pRespFU->setSourcePortIdentity( pResp->getSourcePortIdentity() );
    pRespFU->setSequenceId( pResp->getSequenceId() );
    pRespFU->setControlField( PTP_MSG_CTRL_OTHERS );
    pRespFU->setLogMessageInterval( 0x7F );        // Table 24

    // OMNeT specific stuff
    pRespFU->setByteLength( PTP_MSG_PDEL_RESP_FU_LEN );

    return  pRespFU;
}

// Frame handling
void
cAppPDelay::HandlePDelayReq( PTPv2_PDelay_ReqFrame *pReq )
{
    // Verify we are in a state that is allowed to answer
    switch( pPort->PortDS().GetPortState() )
    {
        case PORT_STATE_LISTENING:
        case PORT_STATE_PRE_MASTER:
        case PORT_STATE_MASTER:
        case PORT_STATE_PASSIVE:
        case PORT_STATE_UNCALIBRATED:
        case PORT_STATE_SLAVE:

            break;

        default:
            return;
    }

    pParentModule->emit( PDelReqRcvd_SigId, pReq->getSequenceId() );

    PTPv2_PDelay_RespFrame   *pResp  = CreatePDelayRespFrame( pReq );

    pPort->IssueFrame(pResp);

    // Create + send follow up if necessary
    if( pApp->defaultDS.GetTwoStepFlag() )
    {
        PTPv2_PDelay_Resp_FU_Frame *pRespFU;

        pRespFU   = CreatePDelayRespFUFrame( pReq );

        // Issue frame
        pPort->IssueFrame(pRespFU);
    }
}

void
cAppPDelay::HandlePDelayResp( PTPv2_PDelay_RespFrame *pResp )
{
    // Check if we are waiting for this response
    if( pResp->getSequenceId() != RequSequId )
    {
        return;
    }

    // Response for our request?
    if( pPort->PortDS().PortIdentity() != pResp->getRequestingPortIdentity() )
    {
        return;
    }

    // Verify we are in suitable state
    switch( pPort->PortDS().GetPortState() )
    {
        case PORT_STATE_LISTENING:
        case PORT_STATE_PRE_MASTER:
        case PORT_STATE_MASTER:
        case PORT_STATE_PASSIVE:
        case PORT_STATE_UNCALIBRATED:
        case PORT_STATE_SLAVE:

            break;

        default:
            return;
    }

    pParentModule->emit( PDelRespRcvd_SigId, pResp->getSequenceId() );

    if( pResp->getFlagField().twoStepFlag )
    {
        ReqEgress           = pResp->getReqEgressTime();
        RequestReceipt      = pResp->getRequestReceiptTimestamp().GetSimTime();
        RespIngress         = pResp->getIngressTimeStamp().GetTime();
        RespCorr            = pResp->getCorrectionField().GetSimTime();
        RespSequID          = pResp->getSequenceId();
    }
    else
    {
        CalcMeanPathDelay(  pResp->getReqEgressTime(),
                            pResp->getIngressTimeStamp().GetTime(),
                            pResp->getRequestReceiptTimestamp().GetSimTime(),
                            SIMTIME_ZERO,
                            pResp->getCorrectionField().GetSimTime(),
                            SIMTIME_ZERO );
    }
}

void
cAppPDelay::HandlePDelayRespFU( PTPv2_PDelay_Resp_FU_Frame *pRespFU )
{
    // Check if we are waiting for this response
    if( pRespFU->getSequenceId() != RequSequId )
    {
        return;
    }

    // Response for our request?
    if( pPort->PortDS().PortIdentity() != pRespFU->getRequestingPortIdentity() )
    {
        return;
    }

    // Verify we are in suitable state
    switch( pPort->PortDS().GetPortState() )
    {
        case PORT_STATE_LISTENING:
        case PORT_STATE_PRE_MASTER:
        case PORT_STATE_MASTER:
        case PORT_STATE_PASSIVE:
        case PORT_STATE_UNCALIBRATED:
        case PORT_STATE_SLAVE:

            break;

        default:
            return;
    }

    pParentModule->emit( PDelRespFuRcvd_SigId, pRespFU->getSequenceId() );

    if( pRespFU->getSequenceId() == RespSequID )
    {
        CalcMeanPathDelay(  ReqEgress,
                            RespIngress,
                            RequestReceipt,
                            pRespFU->getResponseOriginTimestamp().GetSimTime(),
                            RespCorr,
                            pRespFU->getCorrectionField().GetSimTime() );
    }
}

void
cAppPDelay::CalcMeanPathDelay
(
    simtime_t   t1,
    simtime_t   t4,
    simtime_t   requestReceiptTimestamp,
    simtime_t   responseOriginTimestamp,
    simtime_t   CorrPDelResp,
    simtime_t   CorrPDelRespFU
)
{
    // 11.4.3
    // <meanPathDelay> = [(t4 − t1) −
    // (responseOriginTimestamp − requestReceiptTimestamp) −
    // correctionField of Pdelay_Resp − correctionField of Pdelay_Resp_Follow_Up]/2

    simtime_t   meanPathDelay;

    meanPathDelay   =   (t4-t1);
    meanPathDelay   -=  (responseOriginTimestamp-requestReceiptTimestamp);
    meanPathDelay   -=  CorrPDelResp;
    meanPathDelay   -=  CorrPDelRespFU;
    meanPathDelay   /=  2.0;

    pParentModule->emit( peerMeanPathDelay_raw_SigId, meanPathDelay );

    pFilter->push( meanPathDelay );
    pPort->PortDS().SetPeerMeanPathDelay( pFilter->pop() );
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------

void
cAppPDelay::ParseResourceParameters()
{
    cFilteredPortService::ParseResourceParameters();

    FilterType          = cSimTimeFilter_ParameterParser::ParseSimTimeFilterType( pParentModule->par( "meanPeerDelayFilter_Type" ) );
    FilterLen           = pParentModule->par( "meanPeerDelayFilter_Len" ).longValue();
    FilterDiscardMinMax = pParentModule->par( "meanPeerDelayFilter_DiscardMinMax" ).boolValue();
}

void
cAppPDelay::ParseParameters()
{
    EnableDebugOutput = pParentModule->par( "Port_PDelay_EnableDebugOutput" ).boolValue();
}

void
cAppPDelay::RegisterSignals()
{
    cFilteredPortService::RegisterSignals();

    PDelReqRcvd_SigId           = pPort->RegisterDynamicSignal( "PDelReqRcvd"    );
    PDelRespRcvd_SigId          = pPort->RegisterDynamicSignal( "PDelRespRcvd"   );
    PDelRespFuRcvd_SigId        = pPort->RegisterDynamicSignal( "PDelRespFuRcvd" );

    peerMeanPathDelay_raw_SigId = pPort->RegisterDynamicSignal( "peerMeanPathDelay_raw" );
}

void
cAppPDelay::InitSignals()
{
    cFilteredPortService::InitSignals();
}

// ------------------------------------------------------
// Constructors
// ------------------------------------------------------
cAppPDelay::cAppPDelay()
    : cFilteredPortService()
{
}

cAppPDelay::cAppPDelay( const cAppPDelay& other )
    : cFilteredPortService( other )
{
    // Internal housekeeping
    RequSequId  = other.RequSequId;
    RespSequID  = other.RespSequID;

    // TwoStepFlag stuff
    ReqEgress       = other.ReqEgress;
    RespIngress     = other.RespIngress;
    RespCorr        = other.RespCorr;
    RequestReceipt  = other.RequestReceipt;

    // Signals for statistics
    PDelReqRcvd_SigId       = other.PDelReqRcvd_SigId;
    PDelRespRcvd_SigId      = other.PDelRespRcvd_SigId;
    PDelRespFuRcvd_SigId    = other.PDelRespFuRcvd_SigId;
}

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------

// ------------------------------------------------------
// Setters
// ------------------------------------------------------

// ------------------------------------------------------
// Getters
// ------------------------------------------------------

// ------------------------------------------------------
// API functions
// ------------------------------------------------------

void
cAppPDelay::HandleIntervalEvent()
{
    PTPv2_PDelay_ReqFrame *pReq = CreatePDelayReqFrame();

    pPort->IssueFrame(pReq);

    // Remember SeqID
    RequSequId  = pReq->getSequenceId();
}

void
cAppPDelay::HandleTimeoutEvent()
{
    StopTimeout();

    // Nothing special to do here, timeouts are not used in AppPDelay
}

void
cAppPDelay::HandleTimeJump()
{
    cFilteredPortService::HandleTimeJump();

    // Set request sequence Id to some bogus value
    // This will cause received responses for ongoing requests
    // to be ignored
    RequSequId = DrawSequId();
}

void
cAppPDelay::HandleMsg( PTPv2_Frame *pPtpFrame )
{
    if( pApp->DelayMechanism != DELAY_MECH_P2P )
    {
        return;
    }

    switch( pPtpFrame->getMessageType() )
    {
        case PTP_TYPE_PDELAY_REQ:       HandlePDelayReq( check_and_cast<PTPv2_PDelay_ReqFrame *>(pPtpFrame) );
                                        break;

        case PTP_TYPE_PDELAY_RESP:      HandlePDelayResp( check_and_cast<PTPv2_PDelay_RespFrame *>(pPtpFrame) );
                                        break;

        case PTP_TYPE_PDELAY_RESP_FU:   HandlePDelayRespFU( check_and_cast<PTPv2_PDelay_Resp_FU_Frame *>(pPtpFrame) );
                                        break;

        default:                        // Ignore other frames
                                        break;
    }
}

void
cAppPDelay::TriggerFollowUp( PTPv2_PDelay_RespFrame *pResp )
{
    PTPv2_PDelay_Resp_FU_Frame    *pFollowUp;

    pFollowUp   = CreatePDelayRespFUFrame( pResp );

    pPort->IssueFrame( pFollowUp );
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
cAppPDelay&
cAppPDelay::operator=( const cAppPDelay& other )
{
    cFilteredPortService::operator=( other );

    // Internal housekeeping
    RequSequId  = other.RequSequId;
    RespSequID  = other.RespSequID;

    // TwoStepFlag stuff
    ReqEgress       = other.ReqEgress;
    RespIngress     = other.RespIngress;
    RespCorr        = other.RespCorr;
    RequestReceipt  = other.RequestReceipt;

    // Signals for statistics
    PDelReqRcvd_SigId       = other.PDelReqRcvd_SigId;
    PDelRespRcvd_SigId      = other.PDelRespRcvd_SigId;
    PDelRespFuRcvd_SigId    = other.PDelRespFuRcvd_SigId;

    // By convention, always return *this
    return *this;
}
