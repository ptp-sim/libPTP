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
#include "AppAnnounce.h"
#include "PTP_ForeignClockDS.h"

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

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Private methods
// ------------------------------------------------------
PTPv2_AnnounceFrame *
cAppAnnounce::CreateAnnounceFrame()
{
    // Announce message specific stuff, 13.5.2

    PTPv2_AnnounceFrame *pAnn   = new PTPv2_AnnounceFrame;

    pAnn->setOriginTimestamp( cTimeStamp(0,0) );
    pAnn->setCurrentUtcOffset( pApp->timePropertiesDS.GetCurrentUtcOffset() );
    pAnn->setGrandmasterPriority1( pApp->parentDS.GetGrandmasterPriority1() );

    pAnn->setGrandmasterClockQuality( pApp->parentDS.GrandmasterClockQuality() );
    pAnn->setGrandmasterPriority2( pApp->parentDS.GetGrandmasterPriority2() );
    pAnn->setGrandmasterIdentity( pApp->parentDS.GetGrandmasterIdentity() );

    pAnn->setGrandMasterModuleID( pApp->parentDS.GetGrandMasterModuleID() );

    pAnn->setStepsRemoved( pApp->currentDS.GetStepsRemoved() );

    pAnn->setTimeSource( pApp->timePropertiesDS.GetTimeSource() );

    // Generic PTP header, 13.3.2
    pAnn->setTransportSpecific(0);
    pAnn->setMessageType(PTP_TYPE_ANNOUNCE);
    pAnn->setReserved_0(0);
    pAnn->setVersionPTP(pPort->PortDS().GetVersionNumber());
    pAnn->setMessageLength(PTP_MSG_ANNOUNCE_LEN);
    pAnn->setDomainNumber(pApp->defaultDS.GetDomainNumber());
    pAnn->setReserved_1(0);

    cPtpHeaderFlags&    FlagField   = pAnn->getFlagField();

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

    pAnn->setCorrectionField( cTimeInterval() );   // Table 21

    for( unsigned int i = 0; i < pAnn->getReserved_2ArraySize(); i++ )
    {
        pAnn->setReserved_2( i, 0);
    }

    pAnn->setSourcePortIdentity( pPort->PortDS().PortIdentity() );
    pAnn->setSequenceId( DrawSequId() );
    pAnn->setControlField( PTP_MSG_CTRL_OTHERS );
    pAnn->setLogMessageInterval( pPort->PortDS().GetLogAnnounceInterval() );

    // OMNeT specific stuff
    pAnn->setByteLength( PTP_MSG_ANNOUNCE_LEN );

    return  pAnn;
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
cAppAnnounce::RegisterSignals()
{
    cPortService::RegisterSignals();

    AnnounceRcvd_SigId      = pPort->RegisterDynamicSignal( "AnnounceRcvd" );
    ErbestModuleID_SigId    = pPort->RegisterDynamicSignal( "ErbestModuleID" );
}

void
cAppAnnounce::InitSignals()
{
    cPortService::InitSignals();
}

// ------------------------------------------------------
// Constructors
// ------------------------------------------------------
cAppAnnounce::cAppAnnounce()
    : cPortService()
{
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
cAppAnnounce::StartListening()
{
    StartTimeout();
}

void
cAppAnnounce::StopListening()
{
    StopTimeout();
}

void
cAppAnnounce::HandleIntervalEvent()
{
    PTPv2_AnnounceFrame *pAnn   = CreateAnnounceFrame();

    pPort->IssueFrame(pAnn);
}

void
cAppAnnounce::HandleTimeoutEvent()
{
    StopTimeout();

    pPort->HandleEvent( PORT_EVENT_ANNOUNCE_RCV_TIMEOUT );
}

void
cAppAnnounce::HandleMsg( PTPv2_Frame *pPtpFrame )
{
    portState_t             PortState;
    PTPv2_AnnounceFrame     *pAnn   = check_and_cast<PTPv2_AnnounceFrame *>(pPtpFrame);

    PortState   = pPort->PortDS().GetPortState();

    // Re-check message before sending
    assert( pAnn->getGrandMasterModuleID() >= 0 );

    pParentModule->emit( AnnounceRcvd_SigId, pAnn->getGrandMasterModuleID() );

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

    // Restart timeout if in a listening state
    switch( PortState )
    {
        case PORT_STATE_LISTENING:
        case PORT_STATE_PASSIVE:
        case PORT_STATE_UNCALIBRATED:
        case PORT_STATE_SLAVE:
        {
            UpdateTimeout();
            break;
        }
        default:
            break;
    }

    // if in SLAVE-state && sender is current master
    if
    (
        ( PortState == PORT_STATE_SLAVE ) &&
        ( pApp->parentDS.GetGrandmasterIdentity() == pAnn->getSourcePortIdentity().ClockIdentity() )
    )
    {
        // Update data sets as specified in Table 16
        pApp->currentDS.SetStepsRemoved                 ( pAnn->getStepsRemoved() + 1                );

        pApp->parentDS.ParentPortIdentity() =             pAnn->getSourcePortIdentity();
        pApp->parentDS.SetGrandmasterIdentity           ( pAnn->getGrandmasterIdentity()             );
        pApp->parentDS.GrandmasterClockQuality() =        pAnn->getGrandmasterClockQuality();
        pApp->parentDS.SetGrandmasterPriority1          ( pAnn->getGrandmasterPriority1()            );
        pApp->parentDS.SetGrandmasterPriority2          ( pAnn->getGrandmasterPriority2()            );

        pApp->timePropertiesDS.SetCurrentUtcOffset      ( pAnn->getCurrentUtcOffset()                );
        pApp->timePropertiesDS.SetCurrentUtcOffsetValid ( pAnn->getFlagField().currentUtcOffsetValid );
        pApp->timePropertiesDS.SetLeap59                ( pAnn->getFlagField().leap59                );
        pApp->timePropertiesDS.SetLeap61                ( pAnn->getFlagField().leap61                );
        pApp->timePropertiesDS.SetTimeTraceable         ( pAnn->getFlagField().timeTraceable         );
        pApp->timePropertiesDS.SetFrequencyTraceable    ( pAnn->getFlagField().frequencyTraceable    );
        pApp->timePropertiesDS.SetPtpTimescale          ( pAnn->getFlagField().ptpTimescale          );
        pApp->timePropertiesDS.SetTimeSource            ( (timeSource_t) pAnn->getTimeSource()       );

        // Remark: portDS.portState is calculated at next state decision interval

        // TODO: This is a workaround to avoid the foreignMaster dataset to dry out
        cForeignClockDS ForeignClockDS( pAnn );
        ForeignClockDS.ReceiverPortIdentity()   = pPort->PortDS().PortIdentity();

        pPort->ForeignMasterDS().PushForeignClock( ForeignClockDS, pAnn->getIngressTimeStamp().GetTime() );

        pParentModule->emit( ErbestModuleID_SigId, pPort->ForeignMasterDS().GetErbest().GetModuleID() );

    }
    else
    {
        // if known sender: increment foreignMasterDS
        // else (new sender): try to add to foreignMasterDS

        cForeignClockDS ForeignClockDS( pAnn );
        ForeignClockDS.ReceiverPortIdentity()   = pPort->PortDS().PortIdentity();

        pPort->ForeignMasterDS().PushForeignClock( ForeignClockDS, pAnn->getIngressTimeStamp().GetTime() );

        pParentModule->emit( ErbestModuleID_SigId, pPort->ForeignMasterDS().GetErbest().GetModuleID() );
    }
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
