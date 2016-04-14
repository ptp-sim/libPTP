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
#include "AppSync.h"
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
PTPv2_SyncFrame *
cAppSync::CreateSyncFrame()
{
    // Sync message specific stuff, 13.6.2

    PTPv2_SyncFrame *pSync  = new PTPv2_SyncFrame;

    // Precise timestamp will be filled in by MAC, thus it is set to 0 here
    pSync->setOriginTimestamp( cTimeStamp(0,0) );

    // Generic PTP header, 13.3.2
    pSync->setTransportSpecific(0);
    pSync->setMessageType(PTP_TYPE_SYNC);
    pSync->setReserved_0(0);
    pSync->setVersionPTP(pPort->PortDS().GetVersionNumber());
    pSync->setMessageLength(PTP_MSG_SYNC_LEN);
    pSync->setDomainNumber(pApp->defaultDS.GetDomainNumber());
    pSync->setReserved_1(0);

    cPtpHeaderFlags&    FlagField   = pSync->getFlagField();

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

    pSync->setCorrectionField( cTimeInterval() );   // Table 21

    for( unsigned int i = 0; i < pSync->getReserved_2ArraySize(); i++ )
    {
        pSync->setReserved_2( i, 0);
    }

    pSync->setSourcePortIdentity( pPort->PortDS().PortIdentity() );
    pSync->setSequenceId( DrawSequId() );
    pSync->setControlField( PTP_MSG_CTRL_SYNC );
    pSync->setLogMessageInterval( pPort->PortDS().GetLogSyncInterval() );

    // OMNeT specific stuff
    pSync->setByteLength( PTP_MSG_SYNC_LEN );

    return  pSync;
}

PTPv2_Follow_UpFrame *
cAppSync::CreateFollowUpFrame( PTPv2_SyncFrame *pSync )
{
    // Follow up message specific stuff, 13.7.2

    PTPv2_Follow_UpFrame *pFollowUp  = new PTPv2_Follow_UpFrame;

    pFollowUp->setPreciseOriginTimestamp( pSync->getOriginTimestamp() );

    // Generic PTP header, 13.3.2
    pFollowUp->setTransportSpecific(0);
    pFollowUp->setMessageType(PTP_TYPE_FOLLOW_UP);
    pFollowUp->setReserved_0(0);
    pFollowUp->setVersionPTP( pSync->getVersionPTP() );
    pFollowUp->setMessageLength(PTP_MSG_FOLLOW_UP_LEN);
    pFollowUp->setDomainNumber( pSync->getDomainNumber());
    pFollowUp->setReserved_1(0);

    pFollowUp->setFlagField( pSync->getFlagField() );
    pFollowUp->getFlagField().twoStepFlag = false;

    pFollowUp->setCorrectionField( cTimeInterval() );

    for( unsigned int i = 0; i < pFollowUp->getReserved_2ArraySize(); i++ )
    {
        pFollowUp->setReserved_2( i, 0);
    }

    pFollowUp->setSourcePortIdentity( pSync->getSourcePortIdentity() );
    pFollowUp->setSequenceId( pSync->getSequenceId() );
    pFollowUp->setControlField( PTP_MSG_CTRL_FOLLOW_UP );
    pFollowUp->setLogMessageInterval( pSync->getLogMessageInterval() );        // Table 24

    // OMNeT specific stuff
    pFollowUp->setByteLength( PTP_MSG_FOLLOW_UP_LEN );

    return  pFollowUp;
}

// ------------------------------------------------------
// Synchronization functions
// ------------------------------------------------------
void
cAppSync::SynchronizeClock
(
    simtime_t   SyncIngress,
    simtime_t   originTimestamp,
    simtime_t   SyncCorr,
    simtime_t   FollowUpCorr
)
{
    // Secion 11.2
    // <offsetFromMaster> = <syncEventIngressTimestamp> - <originTimestamp>        - <meanPathDelay> - correctionField of Sync message
    // <offsetFromMaster> = <syncEventIngressTimestamp> - <preciseOriginTimestamp> - <meanPathDelay> - correctionField of Sync message - correctionField of Follow_Up message.

    simtime_t   offsetFromMaster_raw;
    simtime_t   offsetFromMaster;
    simtime_t   meanPathDelay;

    if( pPort->PortDS().GetDelayMechanism() == DELAY_MECH_E2E )
    {
        meanPathDelay   = pApp->currentDS.GetMeanPathDelay().GetSimTime();
    }
    else if( pPort->PortDS().GetDelayMechanism() == DELAY_MECH_P2P )
    {
        meanPathDelay   = pPort->PortDS().GetPeerMeanPathDelay().GetSimTime();
    }
    else
    {
        meanPathDelay   = SIMTIME_ZERO;
    }

    offsetFromMaster_raw    = SyncIngress - originTimestamp - meanPathDelay - SyncCorr - FollowUpCorr;

    pApp->EmitSignal_offsetFromMaster_raw( offsetFromMaster_raw );

    pFilter->push( offsetFromMaster_raw );
    offsetFromMaster = pFilter->pop();

    pApp->SetOffsetFromMaster( offsetFromMaster );

    if( EnableDebugOutput )
    {
        EV << "Synchronizing clock" << endl;
        EV << "   SyncIngress:                 " << SyncIngress             << endl;
        EV << "   originTimestamp:             " << originTimestamp         << endl;
        EV << "   SyncCorr:                    " << SyncCorr                << endl;
        EV << "   FollowUpCorr:                " << FollowUpCorr            << endl;
        EV << "   meanPathDelay:               " << meanPathDelay           << endl;
        EV << "   offsetFromMaster (raw):      " << offsetFromMaster_raw    << endl;
        EV << "   offsetFromMaster (filtered): " << offsetFromMaster        << endl;
    }

    SampleDecision_t SampleDec;

    SampleDec = pClockServo->Sample( offsetFromMaster, SyncIngress );

    if( SampleDec.EnableJump )
    {
        pClock->IncScaledTime( SampleDec.Delta );
        pApp->HandleTimejump( SampleDec.Delta );
    }
    if( SampleDec.EnableScale )
    {
        pClock->SetScaleFactor_ppb( SampleDec.ScaleFactor_ppb );
    }

    // Check if offset is inside allowed window
    if( offsetFromMaster < SIMTIME_ZERO )
    {
        offsetFromMaster = -offsetFromMaster;
    }

    if( offsetFromMaster <= pApp->MaxOffsetFromMaster )
    {
        pPort->HandleEvent( PORT_EVENT_MASTER_CLOCK_SELECTED );
    }
    else
    {
        pPort->HandleEvent( PORT_EVENT_SYNCHRONIZATION_FAULT );
    }
}

void
cAppSync::HandleSync( PTPv2_SyncFrame *pSync )
{
    pParentModule->emit( SyncRcvd_SigId, pSync->getSequenceId() );

    // State machine in Figure 30

    portState_t PortState   = pPort->PortDS().GetPortState();

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
        // Optional implementation specific handling of sync message would be possible here
        return;
    }

    // Message from current master?
    if( pApp->parentDS.ParentPortIdentity() != pSync->getSourcePortIdentity() )
    {
        return;
    }

    if
    (
        ( PortState == PORT_STATE_SLAVE         ) ||
        ( PortState == PORT_STATE_UNCALIBRATED  )
    )
    {
        // Accept frame --> use frame info
        pPort->PortDS().SetLogSyncInterval( pSync->getLogMessageInterval() );
        pClockServo->SetSyncInterval( pow(2.0, pSync->getLogMessageInterval()) );

        if( pSync->getFlagField().twoStepFlag )
        {
            // Save timestamps + SequId
            SyncIngress = pSync->getIngressTimeStamp().GetTime();
            SyncCorr    = pSync->getCorrectionField().GetSimTime();
            SyncSequID  = pSync->getSequenceId();
        }
        else
        {
            SynchronizeClock(   pSync->getIngressTimeStamp().GetTime(),
                                pSync->getOriginTimestamp().GetSimTime(),
                                pSync->getCorrectionField().GetSimTime(),
                                SIMTIME_ZERO );
        }
    }
}

void
cAppSync::HandleFollowUp( PTPv2_Follow_UpFrame *pFollowUp )
{
    pParentModule->emit( SyncFuRcvd_SigId, pFollowUp->getSequenceId() );

    // State machine in Figure 31

    portState_t PortState   = pPort->PortDS().GetPortState();

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

    // Message from current master?
    if( pApp->parentDS.ParentPortIdentity() != pFollowUp->getSourcePortIdentity() )
    {
        return;
    }

    // Only accept associated follow ups
    if( pFollowUp->getSequenceId() != SyncSequID )
    {
        return;
    }

    if
    (
       ( PortState == PORT_STATE_SLAVE         ) ||
       ( PortState == PORT_STATE_UNCALIBRATED  )
    )
    {
        SynchronizeClock(   SyncIngress,
                            pFollowUp->getPreciseOriginTimestamp().GetSimTime(),
                            SyncCorr,
                            pFollowUp->getCorrectionField().GetSimTime() );
    }
}

// ------------------------------------------------------
// Constructors
// ------------------------------------------------------
cAppSync::cAppSync()
    : cFilteredPortService()
{
}

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------

void
cAppSync::ParseResourceParameters()
{
    cPortService::ParseResourceParameters();

    FilterType          = cSimTimeFilter_ParameterParser::ParseSimTimeFilterType( pParentModule->par( "offsetFromMasterFilter_Type" ) );
    FilterLen           = pParentModule->par( "offsetFromMasterFilter_Len" ).longValue();
    FilterDiscardMinMax = pParentModule->par( "offsetFromMasterFilter_DiscardMinMax" ).boolValue();
}

void
cAppSync::ParseParameters()
{
    cPortService::ParseParameters();

    EnableDebugOutput = pParentModule->par( "Port_Sync_EnableDebugOutput" ).boolValue();
}

void
cAppSync::RegisterSignals()
{
    cPortService::RegisterSignals();

    SyncRcvd_SigId      = pPort->RegisterDynamicSignal( "SyncRcvd"   );
    SyncFuRcvd_SigId    = pPort->RegisterDynamicSignal( "SyncFuRcvd" );
}

void
cAppSync::InitInternalState()
{
    cPortService::InitInternalState();
}

void
cAppSync::InitSignals()
{
    cPortService::InitSignals();
}

void
cAppSync::SetHierarchy( PTP_Stack *pApp, cPTP_Port *pPort, cScheduleClock *pClock, IClockServo *pClockServo )
{
    cPortService::SetHierarchy( pApp, pPort, pClock );

    this->pClock        = pClock;
    this->pClockServo   = pClockServo;
}

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
cAppSync::StartListening()
{
}
void
cAppSync::StopListening()
{
}

void
cAppSync::HandleIntervalEvent()
{
    PTPv2_SyncFrame *pSync = CreateSyncFrame();

    pPort->IssueFrame(pSync);

    if( pApp->defaultDS.GetTwoStepFlag() )
    {
        pPort->IssueFrame( CreateFollowUpFrame( pSync ) );
    }
}

void
cAppSync::HandleTimeoutEvent()
{
    StopTimeout();

    pPort->HandleEvent( PORT_EVENT_QUALIFICATION_TIMEOUT );
}

void
cAppSync::HandleMsg( PTPv2_Frame  *pPtpFrame )
{
    switch( pPtpFrame->getMessageType() )
    {
        case PTP_TYPE_SYNC:         HandleSync( check_and_cast<PTPv2_SyncFrame *>(pPtpFrame) );
                                    break;

        case PTP_TYPE_FOLLOW_UP:    HandleFollowUp( check_and_cast<PTPv2_Follow_UpFrame *>(pPtpFrame) );
                                    break;

        default:
            break;
    }
}

void
cAppSync::TriggerFollowUp( PTPv2_SyncFrame *pSync )
{
    PTPv2_Follow_UpFrame    *pFollowUp;

    pFollowUp   = CreateFollowUpFrame( pSync );

    pPort->IssueFrame( pFollowUp );
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------

