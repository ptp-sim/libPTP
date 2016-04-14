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

#include "PTP_NIC_Ctrl.h"

#include "PTP_MAC.h"
#include "PTP_MACRelayUnit.h"

#include "PtpPortRequ_m.h"

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

Define_Module(PTP_NIC_Ctrl);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Handle config messages
// ------------------------------------------------------
void
PTP_NIC_Ctrl::HandlePtpConfig( PtpPortConfig *pConfig )
{
    tPtpPortConf    Type    = (tPtpPortConf) pConfig->getType();
    int             PortIdx = pConfig->getArrivalGateId() - ConfigGateID;

    CheckPortIdx( PortIdx );

    switch( Type )
    {
        case PTP_PORT_CONF_TYPE_PATH_DELAY:
        {
            PtpPortConfig_PathDelay *pPD = check_and_cast<PtpPortConfig_PathDelay *>(pConfig);
            Ports[ PortIdx ].peerMeanPathDelay = pPD->getMeanPathDelay();
            break;
        }

        case PTP_PORT_CONF_TYPE_ASYMMETRY:
        {
            PtpPortConfig_Asymmetry *pA = check_and_cast<PtpPortConfig_Asymmetry *>(pConfig);
            Ports[ PortIdx ].Asymmetry = pA->getAsymmetry();
            break;
        }

        default:
        {
            break;
        }
    }
}

void
PTP_NIC_Ctrl::HandleNicConfig( PtpNicConfig  *pConfig )
{
    tPtpNicConf Type = (tPtpNicConf) pConfig->getType();

    switch( Type )
    {
        case PTP_NIC_CONF_TYPE_MAC:
        {
            PtpNicConfig_MAC *pMAC_Config = check_and_cast<PtpNicConfig_MAC *>(pConfig);

            for( cModule::SubmoduleIterator it(getParentModule()); !it.end(); it++ )
            {
                if( strcmp( it()->getName(), "MAC" ) == 0 )
                {
                    PTP_MAC *pMod = check_and_cast<PTP_MAC *>(it());

                    pMod->SetMACAddress( pMAC_Config->getMAC() );
                }

                if( strcmp( it()->getName(), "RelayUnit" ) == 0 )
                {
                    PTP_MACRelayUnit    *pMod = check_and_cast<PTP_MACRelayUnit *>(it());

                    pMod->SetMACAddress( pMAC_Config->getMAC() );
                }
            }

            break;
        }

        default:
        {
            break;
        }
    }
}

// ------------------------------------------------------
// Internal functions
// ------------------------------------------------------
void
PTP_NIC_Ctrl::CheckPortIdx( int PortIdx )
{
    if( PortIdx < 0 )
    {
        error( "Port index must be positive" );
    }

    if( PortIdx >= NumPorts )
    {
        error( "Port index exceeds allowed range" );
    }
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
PTP_NIC_Ctrl::ParseResourceParameters()
{
    RequestGateID   = gateBaseId( "PortRequest" );
    ConfigGateID    = gateBaseId( "PortConfig" );
    NumPorts        = gateSize( "PortRequest" );

    if( gateSize( "PortRequest" ) != gateSize( "PortConfig" ) )
    {
        error( "Size of vector gates must match." );
    }
}

void
PTP_NIC_Ctrl::AllocateResources()
{
    Ports.reserve( NumPorts );
}

void
PTP_NIC_Ctrl::InitHierarchy()
{
    SyncFU_Matcher.SetParentModule       ( this );
    DelayReqResp_Matcher.SetParentModule ( this );
    PDelayReqResp_Matcher.SetParentModule( this );
    PDelayRespFu_Matcher.SetParentModule ( this );
}

void
PTP_NIC_Ctrl::InitInternalState()
{
    for( int PortIdx = 0; PortIdx < NumPorts; PortIdx ++ )
    {
        Ports[ PortIdx ].peerMeanPathDelay = SIMTIME_ZERO;
    }
}

void
PTP_NIC_Ctrl::ForwardInit( int stage )
{
    SyncFU_Matcher.initialize       ( stage );
    DelayReqResp_Matcher.initialize ( stage );
    PDelayReqResp_Matcher.initialize( stage );
    PDelayRespFu_Matcher.initialize ( stage );
}

// ------------------------------------------------------
// Handle messages
// ------------------------------------------------------
void
PTP_NIC_Ctrl::handleMessage(cMessage *pMsg)
{
    PtpPortConfig   *pPortConfig    = dynamic_cast<PtpPortConfig *>(pMsg);
    PtpNicConfig    *pNicConfig     = dynamic_cast<PtpNicConfig *>(pMsg);

    if( pPortConfig != NULL )
    {
        HandlePtpConfig( pPortConfig );
    }
    else if( pNicConfig != NULL )
    {
        HandleNicConfig( pNicConfig );
    }

    delete pMsg;
}

// ------------------------------------------------------
// Public API
// ------------------------------------------------------
PTP_NIC_Ctrl::PTP_NIC_Ctrl()
    : SyncFU_Matcher       ( "SyncFu"        ),
      DelayReqResp_Matcher ( "DelayReqResp"  ),
      PDelayReqResp_Matcher( "PDelayReqResp" ),
      PDelayRespFu_Matcher ( "PDelayRespFu"  )
{
}

// ------------------------------------------------------
// Public API
// ------------------------------------------------------

PTP_FrameMatch  &
PTP_NIC_Ctrl::SyncFuMatcher()
{
    return this->SyncFU_Matcher;
}

PTP_FrameMatch  &
PTP_NIC_Ctrl::DelayReqRespMatcher()
{
    return this->DelayReqResp_Matcher;
}

PTP_FrameMatch  &
PTP_NIC_Ctrl::PDelayReqRespMatcher()
{
    return this->PDelayReqResp_Matcher;
}

PTP_FrameMatch  &
PTP_NIC_Ctrl::PDelayRespFuMatcher()
{
    return this->PDelayRespFu_Matcher;
}

void
PTP_NIC_Ctrl::RequestSyncFollowUp( uint16_t PortIdx, PTPv2_SyncFrame *pSync )
{
    EnterModuleSilent();

    CheckPortIdx( PortIdx );

    PtpPortRequ_TrigSyncFU  *pRequ  = new PtpPortRequ_TrigSyncFU( "PTP: request Sync Follow Up" );

    pRequ->setType( PTP_PORT_REQU_TYPE_TRIG_SFU );
    pRequ->setSyncFrame( *pSync );

    send( pRequ, RequestGateID + PortIdx );

    LeaveModule();
}

void
PTP_NIC_Ctrl::RequestPDelayFollowUp( uint16_t PortIdx, PTPv2_PDelay_RespFrame *pPDelResp )
{
    EnterModuleSilent();

    CheckPortIdx( PortIdx );

    PtpPortRequ_TrigPDelayFU  *pRequ  = new PtpPortRequ_TrigPDelayFU( "PTP: request PDelay Resp Follow Up");

    pRequ->setType( PTP_PORT_REQU_TYPE_TRIG_PDFU );
    pRequ->setPDelayFrame( *pPDelResp );

    send( pRequ, RequestGateID + PortIdx );

    LeaveModule();
}

void
PTP_NIC_Ctrl::ReportFault( uint16_t PortIdx )
{
    EnterModuleSilent();

    CheckPortIdx( PortIdx );

    PtpPortRequ  *pRequ  = new PtpPortRequ( "PTP: Fault at MAC");
    pRequ->setType( PTP_PORT_REQU_TYPE_FAULT );

    send( pRequ, RequestGateID + PortIdx );

    LeaveModule();
}

void
PTP_NIC_Ctrl::ReportRecovery( uint16_t PortIdx )
{
    EnterModuleSilent();

    CheckPortIdx( PortIdx );

    PtpPortRequ  *pRequ  = new PtpPortRequ( "PTP: Fault at MAC");
    pRequ->setType( PTP_PORT_REQU_TYPE_RECOVERY );

    send( pRequ, RequestGateID + PortIdx );

    LeaveModule();
}

simtime_t
PTP_NIC_Ctrl::GetMeanPathDelay( uint16_t PortIdx )
{
    CheckPortIdx( PortIdx );

    return  Ports[PortIdx].peerMeanPathDelay;
}

simtime_t
PTP_NIC_Ctrl::GetAsymmetry( uint16_t PortIdx )
{
    CheckPortIdx( PortIdx );

    return  Ports[PortIdx].Asymmetry;
}
