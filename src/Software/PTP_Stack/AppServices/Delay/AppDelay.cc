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
#include <distrib.h>

#include "PTP_Stack.h"
#include "AppDelay.h"
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
PTPv2_Delay_ReqFrame *
cAppDelay::CreateDelayReqFrame()
{
    // Delay Req message specific stuff, 13.6.2

    PTPv2_Delay_ReqFrame *pReq  = new PTPv2_Delay_ReqFrame;

    // Precise timestamp will be filled in by MAC, thus it is set to 0 here
    pReq->setOriginTimestamp( cTimeStamp(0,0) );

    // Generic PTP header, 13.3.2
    pReq->setTransportSpecific(0);
    pReq->setMessageType(PTP_TYPE_DELAY_REQ);
    pReq->setReserved_0(0);
    pReq->setVersionPTP(pPort->PortDS().GetVersionNumber());
    pReq->setMessageLength(PTP_MSG_DEL_REQ_LEN);
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
    pReq->setControlField( PTP_MSG_CTRL_DEL_REQ );
    pReq->setLogMessageInterval( 0x7F );        // Table 24

    // OMNeT specific stuff
    pReq->setByteLength( PTP_MSG_DEL_REQ_LEN );

    return  pReq;
}

PTPv2_Delay_RespFrame *
cAppDelay::CreateDelayRespFrame( PTPv2_Delay_ReqFrame *pReq )
{
    // Delay Resp message specific stuff, 13.8.2

    PTPv2_Delay_RespFrame *pResp  = new PTPv2_Delay_RespFrame;

    pResp->setReceiveTimestamp( pReq->getIngressTimeStamp().GetTime() );
    pResp->setRequestingPortIdentity( pReq->getSourcePortIdentity() );
    pResp->setReqEgressTime( SIMTIME_ZERO );   // Will be set by receiver's MAC on ingress

    // Generic PTP header, 13.3.2
    pResp->setTransportSpecific(0);
    pResp->setMessageType(PTP_TYPE_DELAY_RESP);
    pResp->setReserved_0(0);
    pResp->setVersionPTP(pPort->PortDS().GetVersionNumber());
    pResp->setMessageLength(PTP_MSG_DEL_RESP_LEN);
    pResp->setDomainNumber(pApp->defaultDS.GetDomainNumber());
    pResp->setReserved_1(0);

    cPtpHeaderFlags&    FlagField   = pResp->getFlagField();

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

    pResp->setCorrectionField( pReq->getCorrectionField() );   // 11.3.2

    for( unsigned int i = 0; i < pResp->getReserved_2ArraySize(); i++ )
    {
        pResp->setReserved_2( i, 0);
    }

    pResp->setSourcePortIdentity( pPort->PortDS().PortIdentity() );
    pResp->setSequenceId( pReq->getSequenceId() );
    pResp->setControlField( PTP_MSG_CTRL_DEL_RESP );
    pResp->setLogMessageInterval( pPort->PortDS().GetLogMinDelayReqInterval() );        // Table 24

    // OMNeT specific stuff
    pResp->setByteLength( PTP_MSG_DEL_RESP_LEN );

    return  pResp;
}

void
cAppDelay::HandleSync( PTPv2_SyncFrame *pSync )
{
    TwoStepFlag = pSync->getFlagField().twoStepFlag;

    if( !TwoStepFlag )
    {
        TimestampMatcher.PushFirst( pSync->getOriginTimestamp().GetSimTime(), SIMTIME_ZERO, pSync->getSequenceId() );
    }

    TimestampMatcher.PushSecond( pSync->getIngressTimeStamp().GetTime(),
                                 pSync->getCorrectionField().GetSimTime(),
                                 pSync->getSequenceId() );

    if( TriggerStart == true )
    {
        TriggerStart = false;

        cFilteredPortService::StartInterval();
    }
}

void
cAppDelay::HandleFollowUp( PTPv2_Follow_UpFrame *pFollowUp )
{
    if( TwoStepFlag )
    {
        TimestampMatcher.PushFirst( pFollowUp->getPreciseOriginTimestamp().GetSimTime(), pFollowUp->getCorrectionField().GetSimTime(), pFollowUp->getSequenceId() );
    }
}

void
cAppDelay::HandleDelayReq  ( PTPv2_Delay_ReqFrame *pReq )
{
    portState_t             PortState;

    // This code implements Figure 33
    PortState   = pPort->PortDS().GetPortState();

    if
    (
        ( PortState == PORT_STATE_INITIALIZING  ) ||
        ( PortState == PORT_STATE_DISABLED      )
    )
    {
        return;
    }

    if( PortState == PORT_STATE_FAULTY )
    {
        // Optional implementation specific handling of announce message would be possible here
        return;
    }

    if( PortState != PORT_STATE_MASTER )
    {
        return;
    }

    pParentModule->emit( DelReqRcvd_SigId, pReq->getSequenceId() );

    PTPv2_Delay_RespFrame   *pResp  = CreateDelayRespFrame( pReq );

    pPort->IssueFrame(pResp);
}

void
cAppDelay::HandleDelayResp ( PTPv2_Delay_RespFrame *pResp )
{
    portState_t             PortState;

    // This code implements Figure 33
    PortState   = pPort->PortDS().GetPortState();

    if
    (
        ( PortState == PORT_STATE_INITIALIZING  ) ||
        ( PortState == PORT_STATE_DISABLED      )
    )
    {
        return;
    }

    if( PortState == PORT_STATE_FAULTY )
    {
        // Optional implementation specific handling of announce message would be possible here
        return;
    }

    if
    (
        ( PortState != PORT_STATE_SLAVE         ) &&
        ( PortState != PORT_STATE_UNCALIBRATED  )
    )
    {
        return;
    }

    // Ignore not associated responses
    if( pResp->getSequenceId() != RequSequId )
    {
        return;
    }

    // Message from current master?
    if( pApp->parentDS.ParentPortIdentity() != pResp->getSourcePortIdentity() )
    {
        return;
    }

    // Response for our request?
    if( pPort->PortDS().PortIdentity() != pResp->getRequestingPortIdentity() )
    {
        return;
    }

    pParentModule->emit( DelRespRcvd_SigId, pResp->getSequenceId() );

    // Execute DelayReq-Response mechanism (11.3.2)
    simtime_t   t1  = SyncTimes.Time1;
    simtime_t   t2  = SyncTimes.Time2;
    simtime_t   t3  = pResp->getReqEgressTime();
    simtime_t   t4  = pResp->getReceiveTimestamp().GetSimTime();

    simtime_t   c1  = SyncTimes.Corr1;
    simtime_t   c2  = SyncTimes.Corr2;
    simtime_t   c3  = pResp->getCorrectionField().GetSimTime();

    simtime_t   meanPathDelay   = ( (t2-t3) + (t4-t1) - (c1+c2+c3) ) / 2.0;

    pApp->EmitSignal_meanPathDelay_raw( meanPathDelay );

    pFilter->push( meanPathDelay );
    meanPathDelay = pFilter->pop();

    pApp->SetMeanPathDelay( meanPathDelay );

    // Update logMinDelayReqInterval
    pPort->PortDS().SetLogMinDelayReqInterval( pResp->getLogMessageInterval() );
    Interval    = pow( 2.0, pResp->getLogMessageInterval() );
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
cAppDelay::ParseResourceParameters()
{
    cFilteredPortService::ParseResourceParameters();

    FilterType          = cSimTimeFilter_ParameterParser::ParseSimTimeFilterType( pParentModule->par( "meanPathDelayFilter_Type" ) );
    FilterLen           = pParentModule->par( "meanPathDelayFilter_Len" ).longValue();
    FilterDiscardMinMax = pParentModule->par( "meanPathDelayFilter_DiscardMinMax" ).boolValue();
}

void
cAppDelay::ParseParameters()
{
    EnableDebugOutput = pParentModule->par( "Port_Delay_EnableDebugOutput" ).boolValue();
}

void
cAppDelay::RegisterSignals()
{
    cFilteredPortService::RegisterSignals();

    DelReqRcvd_SigId    = pPort->RegisterDynamicSignal( "DelReqRcvd"  );
    DelRespRcvd_SigId   = pPort->RegisterDynamicSignal( "DelRespRcvd" );
}

void
cAppDelay::InitSignals()
{
    cFilteredPortService::InitSignals();
}

// ------------------------------------------------------
// Constructors
// ------------------------------------------------------
cAppDelay::cAppDelay()
    : cFilteredPortService()
{
    TriggerStart    = false;
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
cAppDelay::StartInterval()
{
    TriggerStart    = true;
}

void
cAppDelay::StopInterval()
{
    TriggerStart = false;
    cFilteredPortService::StopInterval();
}

void
cAppDelay::HandleIntervalEvent()
{
    PTPv2_Delay_ReqFrame *pReq = CreateDelayReqFrame();

    // Remember SeqID
    RequSequId  = pReq->getSequenceId();

    // Remember Time of most recent Sync frame
    SyncTimes   = TimestampMatcher.GetMostRecent();

    pPort->IssueFrame(pReq);
}

void
cAppDelay::HandleTimeoutEvent()
{
    StopTimeout();

    // Nothing special to do here, timeouts are not used in AppDelay
}

void
cAppDelay::HandleTimeJump()
{
    cFilteredPortService::HandleTimeJump();

    // Set request sequence Id to some bogus value
    // This will cause received responses for ongoing requests
    // to be ignored
    RequSequId = DrawSequId();
}

void
cAppDelay::HandleMsg( PTPv2_Frame  *pPtpFrame )
{

    if( pApp->DelayMechanism != DELAY_MECH_E2E )
    {
        return;
    }

    switch( pPtpFrame->getMessageType() )
    {
        case PTP_TYPE_SYNC:         HandleSync( check_and_cast<PTPv2_SyncFrame *>(pPtpFrame) );
                                    break;

        case PTP_TYPE_FOLLOW_UP:    HandleFollowUp( check_and_cast<PTPv2_Follow_UpFrame *>(pPtpFrame) );
                                    break;

        case PTP_TYPE_DELAY_REQ:    HandleDelayReq( check_and_cast<PTPv2_Delay_ReqFrame *>(pPtpFrame) );
                                    break;

        case PTP_TYPE_DELAY_RESP:   HandleDelayResp ( check_and_cast<PTPv2_Delay_RespFrame *>(pPtpFrame) );
                                    break;

        default:                    // Ignore other frames
                                    break;
    }
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
