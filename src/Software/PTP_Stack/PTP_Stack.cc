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

#include <new>
#include <cassert>

#include "MACAddress.h"

#include "PTP.h"
#include "PTP_Port.h"
#include "PTP_Stack.h"

#include "PTP_CustomProfileChecker.h"
#include "PTP_DefaultE2EProfileChecker.h"
#include "PTP_DefaultP2PProfileChecker.h"
#include "PTP_PowerProfileChecker.h"

#include "PTP_Parser.h"

#include "PtpPortConfig_m.h"
#include "PTP_EventMsg_m.h"

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

Define_Module(PTP_Stack);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Internal utilities
// ------------------------------------------------------
void
PTP_Stack::CheckPortIdx( int PortIdx )
{
    if
    (
        ( PortIdx < 0 ) ||
        ( PortIdx >= PtpGateSize )
    )
    {
        throw cRuntimeError( "Caught an invalid PortIdx: %d", PortIdx );
    }
}

// ------------------------------------------------------------
// Clock servo handling
// ------------------------------------------------------------
void
PTP_Stack::StartClockServo()
{
    if( !pClockServo->IsEnabled() )
    {
        pClockServo->SetScaleFactorBounds( pClock->GetScaleFactor_LowerBound_ppb(), pClock->GetScaleFactor_UpperBound_ppb() );
        pClockServo->SetScaleFactor_ppb( pClock->GetScaleFactor_ppb() );
        pClockServo->Enable();
    }
}

void
PTP_Stack::StopClockServo()
{
    pClockServo->Disable();
}

// ------------------------------------------------------
// Parse parameters
// ------------------------------------------------------
void
PTP_Stack::ParseStackParameters()
{
    // -------------------------------------------------------------------------
    // Parse parameters for Internal data
    // -------------------------------------------------------------------------
    ModuleID            = par( "NodeNumber" ).longValue();
    if( ModuleID < 0 )
    {
        ModuleID = getId();
        par( "NodeNumber" ).setLongValue( ModuleID );
    }
    PTP_ClockType       = cPTP_Parser::ParsePtpClockType( par( "PTP_ClockType" ).stringValue() );
    Active_E2E_TC       = par( "Active_E2E_TC" ).boolValue();
    MaxOffsetFromMaster = simtime_t( par( "MaxOffsetFromMaster" ).doubleValue() );

    // -------------------------------------------------------------------------
    // Parse parameters for default data set
    // -------------------------------------------------------------------------
    SlaveOnly               = par( "SlaveOnly" ).boolValue();
    TwoStepFlag             = par( "PTP_TwoStepFlag" ).boolValue();
    Priority1               = par( "Priority1" ).longValue();
    Priority2               = par( "Priority2" ).longValue();
    offsetScaledLogVariance = par( "OffsetScaledLogVariance" ).longValue();
    ClockAccuracy           = cPTP_Parser::ParseClockAccuracy ( par( "ClockAccuracy" ).stringValue() );
    ClockClass              = cPTP_Parser::ParseClockClass    ( par( "ClockClass" ).stringValue() );
    PrimaryDomain           = cPTP_Parser::ParseDomainNumber  ( par( "DomainNumber" ).stringValue() );

    if( par( "AutoMAC" ).boolValue() )
    {
        MACAddress MAC( "C0:FF:EE:00:00:00" );

        MAC.setAddressByte( 3, (ModuleID >> 16 & 0xFF) );
        MAC.setAddressByte( 4, (ModuleID >>  8 & 0xFF) );
        MAC.setAddressByte( 5, (ModuleID >>  0 & 0xFF) );

        par( "MAC_Address" ).setStringValue( MAC.str() );
    }

    MAC = MACAddress( par( "MAC_Address" ).stringValue() );

    if( par( "UseMacForClockID" ).boolValue() )
    {
        ClockIdentity   = MAC;
    }
    else
    {
        ClockIdentity   = par( "ClockID" ).stringValue();
    }

    // Write back parsed clock ID
    par( "ClockID" ).setStringValue( ClockIdentity.GetString() );

    // -------------------------------------------------------------------------
    // Parse parameters for current data set
    // -------------------------------------------------------------------------
    // Nothing needs to be done here

    // -------------------------------------------------------------------------
    // Parse parameters for parent data set
    // -------------------------------------------------------------------------
    // Nothing needs to be done here

    // -------------------------------------------------------------------------
    // Parse parameters for Time properties data set
    // -------------------------------------------------------------------------
    // Nothing needed for current data set

    // -------------------------------------------------------------------------
    // Parse PTP Event configuration
    // -------------------------------------------------------------------------
    ParsePtpEvents();

    // -------------------------------------------------------------------------
    // Debug configuration
    // -------------------------------------------------------------------------
    EnableDebugOutput = par( "EnableDebugOutput" ).boolValue();
}

void
PTP_Stack::ParsePortParameters()
{
    for( int PortIdx = 0; PortIdx < PtpGateSize; PortIdx++ )
    {
        // -------------------------------------------------------------------------
        // Parse parameters for Port Data Sets
        // -------------------------------------------------------------------------
        // Timing characteristics  + delay mechanism are currently taken from app
        LogSyncInterval         = cPTP_Parser::ParseParameterInt8   ( this, "LogSyncInterval"         );
        LogMinDelayReqInterval  = cPTP_Parser::ParseParameterInt8   ( this, "LogMinDelayReqInterval"  );
        LogMinPdelayReqInterval = cPTP_Parser::ParseParameterInt8   ( this, "LogMinPdelayReqInterval" );
        LogAnnounceInterval     = cPTP_Parser::ParseParameterInt8   ( this, "LogAnnounceInterval"     );
        AnnounceReceiptTimeout  = cPTP_Parser::ParseParameterUInt8  ( this, "AnnounceReceiptTimeout"  );
        Asymmetry               = simtime_t( par( "Asymmetry" ).doubleValue() );
    }
}

void
PTP_Stack::ParsePtpEvents()
{
    // Remark: currently only a simple parameter interface exists for fault simulation

    if( par( "SimulateFault" ).boolValue() )
    {
        simtime_t   FaultTime       = simtime_t( par( "FaultTime" ).doubleValue() );
        simtime_t   FaultDuration   = simtime_t( par( "FaultDuration" ).doubleValue() );

        int PortID = par( "FaultPortID" ).longValue();

        if( PortID != 0 )
        {
            CheckPortIdx( PortID - 1 );
        }

        if( FaultTime < SIMTIME_ZERO )
        {
            error( "Real time value for the simulated fault occurrence must be strictly positive." );
        }

        if( FaultDuration < SIMTIME_ZERO )
        {
            error( "Real time value for the simulated fault duration must be strictly positive." );
        }

        PTP_EventMsg    *FaultMsg       = new PTP_EventMsg( "PTP Fault Event"    );
        PTP_EventMsg    *RecoveryMsg    = new PTP_EventMsg( "PTP Recovery Event" );

        FaultMsg->setPortID( PortID );
        RecoveryMsg->setPortID( PortID );

        FaultMsg->setEvent( PORT_EVENT_FAULT_DETECTED );
        RecoveryMsg->setEvent( PORT_EVENT_FAULT_CLEARED );

        scheduleAt( FaultTime, FaultMsg );
        scheduleAt( FaultTime + FaultDuration, RecoveryMsg );
    }
}


// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
PTP_Stack::PTP_Stack()
    : AppStateDecision()
{
    pProfileChecker = nullptr;
}

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------
PTP_Stack::~PTP_Stack()
{
    delete[]    pPorts;
    delete      pProfileChecker;
}

// ------------------------------------------------------
// API functions for services
// ------------------------------------------------------

void
PTP_Stack::SetOffsetFromMaster( simtime_t offsetFromMaster )
{
    currentDS.SetOffsetFromMaster( offsetFromMaster );

    this->offsetFromMaster = offsetFromMaster;
}

void
PTP_Stack::SetMeanPathDelay( simtime_t meanPathDelay )
{
    currentDS.SetMeanPathDelay( meanPathDelay );

    this->meanPathDelay = meanPathDelay;
}

void
PTP_Stack::IssueFrame( PTPv2_Frame *pPTP, UInteger16 PortNumber )
{
    assert( PortNumber >= 1 );

    take(pPTP);
    send( pPTP, PtpOutGateID + PortNumber - 1 );
}

void
PTP_Stack::ConfigNIC_MAC()
{
    PtpNicConfig_MAC *pConfig  = new PtpNicConfig_MAC( "PTP NIC config: MAC" );

    pConfig->setType( PTP_NIC_CONF_TYPE_MAC );
    pConfig->setMAC( MAC );

    send( pConfig, ConfigGateID );
}

void
PTP_Stack::ConfigPortPathDelay( uint16_t PortID, simtime_t meanPathDelay )
{
    PtpPortConfig_PathDelay *pConfig  = new PtpPortConfig_PathDelay( "PTP port config: path delay" );

    pConfig->setType( PTP_PORT_CONF_TYPE_PATH_DELAY );
    pConfig->setMeanPathDelay( meanPathDelay );

    send( pConfig, ConfigGateID + PortID - 1 );
}

void
PTP_Stack::ConfigPortAsymmetry( uint16_t PortID, simtime_t Asymmetry )
{
    PtpPortConfig_Asymmetry *pConfig  = new PtpPortConfig_Asymmetry( "PTP port config: asymmetry" );

    pConfig->setType( PTP_PORT_CONF_TYPE_ASYMMETRY );
    pConfig->setAsymmetry( Asymmetry );

    send( pConfig, ConfigGateID + PortID - 1 );
}

void
PTP_Stack::EmitSignal_offsetFromMaster_raw( simtime_t offsetFromMaster_raw )
{
    emit( offsetFromMaster_raw_SigId, offsetFromMaster_raw );
}

void
PTP_Stack::EmitSignal_meanPathDelay_raw( simtime_t meanPathDelay_raw )
{
    emit( meanPathDelay_raw_SigId, meanPathDelay_raw );
}


// ------------------------------------------------------
// Debug functions
// ------------------------------------------------------
void
PTP_Stack::PrintInfo()
{
    EV << "================================================" << endl;
    EV << "PTP Clock information:" << endl;
    EV << "================================================" << endl;
    EV << endl;
    EV << "ModuleID: " << ModuleID << endl;
    EV << "Clock ModuleID " << pClock->getId() << endl;

    EV << "PTP clock type: " << PTP_ClockType << endl;
    EV << "Delay Mechanism: " << DelayMechanism << endl;
    EV << "Number of Ports: " << PtpGateSize << endl;

    defaultDS.ClockIdentity().Print();
    EV << "================================================" << endl;
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
PTP_Stack::ParseResourceParameters()
{
    Enabled             = par( "Enabled" ).boolValue();

    if( Enabled )
    {
        ClockPath       = par( "ClockPath" ).stringValue();
        ClockServoPath  = par( "ClockServoPath" ).stringValue();

        BMCA                = cPTP_Parser::ParseBestMasterClockAlgorithm( par( "PTP_BestMasterClockAlgorithm" ).stringValue() );
        MgmtProtocol        = cPTP_Parser::ParseManagementProtocol( par( "PTP_ManagementProtocol" ).stringValue() );
        PTP_Profile     = cPTP_Parser::ParsePtpProfile( par( "PTP_Profile" ).stringValue() );
    }
}

void
PTP_Stack::AllocateResources()
{
    if( !Enabled )
    {
        return;
    }

    PtpInGateID     = gateBaseId( "PtpIn" );
    PtpOutGateID    = gateBaseId( "PtpOut" );
    RequestGateID   = gateBaseId( "PortRequest" );
    ConfigGateID    = gateBaseId( "PortConfig" );
    PtpGateSize     = gateSize( "PtpOut" );

    if
    (
        ( gateSize( "PtpIn"       ) != PtpGateSize ) ||
        ( gateSize( "PtpOut"      ) != PtpGateSize ) ||
        ( gateSize( "PortRequest" ) != PtpGateSize ) ||
        ( gateSize( "PortConfig"  ) != PtpGateSize )
    )
    {
        error( "Size of vector gates must match." );
    }

    pPorts          = new cPTP_Port[ (size_t) PtpGateSize ];
    pClock          = check_and_cast<cScheduleClock *>( getModuleByPath( ClockPath.c_str() ) );
    pClockServo     = check_and_cast<IClockServo *>( getModuleByPath( ClockServoPath.c_str() ) );

    switch( PTP_Profile )
    {
        default:
        case PTP_PROFILE_CUSTOM:        pProfileChecker = new PTP_CustomProfileChecker();
                                        break;

        case PTP_PROFILE_DEFAULT_E2E:   pProfileChecker = new PTP_DefaultE2EProfileChecker();
                                        break;

        case PTP_PROFILE_DEFAULT_P2P:   pProfileChecker = new PTP_DefaultP2PProfileChecker();
                                        break;

        case PTP_PROFILE_POWER:         pProfileChecker = new PTP_PowerProfileChecker();
                                        break;
    }
}

void
PTP_Stack::InitHierarchy()
{
    if( !Enabled )
    {
        return;
    }

    currentDS.SetParentModule       ( this );
    parentDS.SetParentModule        ( this );
    timePropertiesDS.SetParentModule( this );

    defaultDS.SetHierarchy          ( this );

    AppStateDecision.SetParentModule( this );
    AppStateDecision.SetCallableParentModule( this );
    AppStateDecision.SetHierarchy   ( this, pClock );

    for( int PortIdx = 0; PortIdx < PtpGateSize; PortIdx ++ )
    {
        pPorts[PortIdx].SetParentModule( this );
        pPorts[PortIdx].SetHierarchy( this, pClock, pClockServo, PortIdx + 1 );
    }
}

void
PTP_Stack::ParseParameters()
{
    if( !Enabled )
    {
        return;
    }

    ParseStackParameters();
    ParsePortParameters();
}

void
PTP_Stack::RegisterSignals()
{
    if( !Enabled )
    {
        return;
    }

    offsetFromMaster_raw_SigId  = registerSignal("offsetFromMaster_raw");
    meanPathDelay_raw_SigId     = registerSignal("meanPathDelay_raw");

    // WATCH variables
    offsetFromMaster    = SIMTIME_ZERO;
    meanPathDelay       = SIMTIME_ZERO;

    WATCH(offsetFromMaster);
    WATCH(meanPathDelay);
    WATCH(parentDS.parentPortIdentity);
    WATCH(parentDS.grandmasterIdentity);
}

void
PTP_Stack::InitInternalState()
{
    if( !Enabled )
    {
        return;
    }

    // -------------------------------------------------------------------------
    // Populate default data set
    // -------------------------------------------------------------------------
    defaultDS.SetTwoStepFlag(TwoStepFlag);
    defaultDS.SetNumberPorts(PtpGateSize);

    defaultDS.ClockIdentity()   = ClockIdentity;

    defaultDS.SetSlaveOnly(SlaveOnly);                // Has to be initialized before clockClass

    defaultDS.ClockQuality().SetClockClass( ClockClass );
    defaultDS.ClockQuality().SetClockAccuracy( ClockAccuracy );
    defaultDS.ClockQuality().SetOffsetScaledLogVar( offsetScaledLogVariance );
    defaultDS.SetDomainNumber(PrimaryDomain);

    defaultDS.SetPriority1( Priority1 );
    defaultDS.SetPriority2( Priority2 );

    // Save contents of defaultDS in D0 for future comparison
    D0  = defaultDS;

    // -------------------------------------------------------------------------
    // Populate current data set
    //      This data set has to be maintained during runtime,
    //      thus it is only initialized to dummy values
    // -------------------------------------------------------------------------
    currentDS.SetMeanPathDelay( cTimeInterval() );
    currentDS.SetOffsetFromMaster( cTimeInterval() );
    currentDS.SetStepsRemoved(0);

    // -------------------------------------------------------------------------
    // Populate parent data set (Initialize mostly to defaultDS-values
    // -------------------------------------------------------------------------
    parentDS.ParentPortIdentity().ClockIdentity()   = defaultDS.ClockIdentity();        // 8.2.3.2
    parentDS.ParentPortIdentity().SetPortNumber(0);                                     // 8.2.3.2

    parentDS.SetParentStats(false);                                                     // 8.2.3.3 + 8.2.3.4
    parentDS.SetObservedParentOffsetScaledLogVariance(0);                               // 8.2.3.4
    parentDS.SetObservedParentClockPhaseChangeRate(PTP_DEFAULT_PHASE_CHANGE_RATE);      // 8.2.3.5

    parentDS.SetGrandmasterIdentity( defaultDS.ClockIdentity() );                       // 8.2.3.6
    parentDS.GrandmasterClockQuality()  = defaultDS.ClockQuality();                     // 8.2.3.7

    parentDS.SetGrandmasterPriority1( defaultDS.GetPriority1() );                       // 8.2.3.8
    parentDS.SetGrandmasterPriority2( defaultDS.GetPriority2() );                       // 8.2.3.8

    parentDS.SetGrandMasterModuleID( defaultDS.GetModuleID() );

    // -------------------------------------------------------------------------
    // Populate time properties data set
    // -------------------------------------------------------------------------

    // 8.2.4.8
    // 8.2.4.1: Needs to be initialized before the other variables
    // False implies ARB (arbitrary) time scale
    timePropertiesDS.SetPtpTimescale(false);

    // 8.2.4.2
    // 7.2.3, Note: UTC offset on 1 January 2006 00:00 was +33 seconds
    timePropertiesDS.SetCurrentUtcOffset( PTP_UTC_OFFSET_2006 );

    // 8.2.4.3
    timePropertiesDS.SetCurrentUtcOffsetValid(false);

    // 8.2.4.4
    timePropertiesDS.SetLeap59( 0 );

    // 8.2.4.5
    timePropertiesDS.SetLeap61( 0 );

    // 8.2.4.6
    timePropertiesDS.SetTimeTraceable(false);

    // 8.2.4.7
    timePropertiesDS.SetFrequencyTraceable(false);

    // 8.2.4.
    timePropertiesDS.SetTimeSource( TIME_SRC_INTERNAL_OSCILLATOR );

    // -------------------------------------------------------------------------
    // Calculate values for internal housekeeping
    // -------------------------------------------------------------------------
    StateDecisionInterval   = pow( 2.0, cPTP_Parser::ParseParameterInt8   ( this, "LogAnnounceInterval" ) );

    DelayMechanism  = cPTP_Parser::ParseDelayMechanism( par( "PTP_DelayMechanism" ).stringValue() );

    // -------------------------------------------------------------------------
    // Transparent Clock Data Sets
    // -------------------------------------------------------------------------
    //
    // transparentClockDefaultDS + transparentClockPortDS are currently not
    // implemented, as TC use the defaultDS and portDS of ordinary clocks
    //
    // -------------------------------------------------------------------------

    // TODO: care about transparent clock data sets

    // Init ports
    for( int PortIdx = 0; PortIdx < PtpGateSize; PortIdx ++ )
    {
        cPortDS &portDS = pPorts[PortIdx].PortDS();

        // Static members
        portDS.PortIdentity().ClockIdentity()    = defaultDS.ClockIdentity();
        portDS.PortIdentity().SetPortNumber( PortIdx+1 );

        // Dynamic members
        portDS.SetPortState( PORT_STATE_INITIALIZING );
        portDS.SetLogMinDelayReqInterval( LogMinDelayReqInterval );
        portDS.SetPeerMeanPathDelay( cTimeInterval() );                         // 8.2.5.3.3 Init value is 0

        // Configurable members
        portDS.SetLogAnnounceInterval       ( LogAnnounceInterval       );      //
        portDS.SetAnnounceReceiptTimeout    ( AnnounceReceiptTimeout    );      //
        portDS.SetLogSyncInterval           ( LogSyncInterval           );      //
        portDS.SetDelayMechanism            ( DelayMechanism            );      // E2E or P2P
        portDS.SetLogMinPdelayReqInterval   ( LogMinPdelayReqInterval   );      //
        portDS.SetVersionNumber             ( PTP_VERSION_IEEE_1588_2008);      // Has to be 2 for IEEE 1588-2008
        portDS.SetAsymmetry                 ( Asymmetry                 );

        // Init foreign master data set of port
        pPorts[PortIdx].ForeignMasterDS().SetForeignMasterTimeWindow( 4.0 * pow(2.0, LogAnnounceInterval) ); // 9.3.2.4.4: ForeignMasterTimeWindow = 4 * announceInterval
    }

    // Clock servo
    StopClockServo();

    // House keeping
    AppStateDecision.SetInterval( StateDecisionInterval );

    timePropertiesDS_Backup   = timePropertiesDS;

    // Init NIC Ctrl
    ConfigNIC_MAC();
}

void
PTP_Stack::InitSignals()
{
}

void
PTP_Stack::FinishInit()
{
    if( !Enabled )
    {
        return;
    }

    // Check profile parameters
    pProfileChecker->CheckParametersFixed(this);
    pProfileChecker->CheckParametersInit(this);

    // Simulate initial startup
    StartInitialize();
}

void
PTP_Stack::PrintDebugOutput()
{
    if( !Enabled )
    {
        return;
    }

    if( EnableDebugOutput )
    {
        PrintInfo();

        defaultDS.Print();
        currentDS.Print();
        parentDS.Print();
        timePropertiesDS.Print();

        for( int PortIdx = 0; PortIdx < PtpGateSize; PortIdx ++ )
        {
            pPorts[PortIdx].PortDS().Print();
        }
    }
}

void
PTP_Stack::ForwardInit( int stage )
{
    // Data sets
    currentDS.initialize( stage );
    parentDS.initialize( stage );
    timePropertiesDS.initialize( stage );

    // Services
    AppStateDecision.initialize( stage );

    // Profiles
    pProfileChecker->initialize(stage);

    // Ports
    for( int PortIdx = 0; PortIdx < PtpGateSize; PortIdx ++ )
    {
        pPorts[PortIdx].initialize( stage);
    }
}

// ------------------------------------------------------
// Handle Message
// ------------------------------------------------------
void PTP_Stack::handleMessage(cMessage *pMsg)
{
    if( Enabled )
    {
        if( pMsg->isSelfMessage() )
        {
            // ------------------------------------------------------
            // PTP Events
            // ------------------------------------------------------
            PTP_EventMsg    *pEventMsg = check_and_cast<PTP_EventMsg *>(pMsg);

            HandleEvent( pEventMsg->getEvent(), pEventMsg->getPortID() );
        }
        else
        {
            // ------------------------------------------------------
            // PTP Frames
            // ------------------------------------------------------
            if( pMsg->getArrivalGate()->getBaseId() == PtpInGateID )
            {
                int PortIdx  = pMsg->getArrivalGateId() - PtpInGateID;

                CheckPortIdx( PortIdx );

                HandlePtpFrame( check_and_cast<PTPv2_Frame *>(pMsg), PortIdx );
            }
            // ------------------------------------------------------
            // PTP Port Requests
            // ------------------------------------------------------
            else
            {
                HandlePortRequ( check_and_cast<PtpPortRequ *>(pMsg) );
            }
        }
    }

    delete pMsg;
}

// ------------------------------------------------------
// Finish
// ------------------------------------------------------
void PTP_Stack::finish()
{
}

void
PTP_Stack::HandleTimejump( simtime_t Delta )
{
    for( int PortIdx = 0; PortIdx < PtpGateSize; PortIdx ++ )
    {
        pPorts[PortIdx].HandleTimejump( Delta );
    }
}

void
PTP_Stack::HandlePortStateDec( portStateDecision_t StateDecision )
{
    // Update data sets
    switch( StateDecision )
    {
        // This instance is the grandmaster of the system
        case PORT_SD_M1:
        case PORT_SD_M2:
        {
            BecomeGrandMaster();
            break;
        }

        // At least one port is in slave state
        case PORT_SD_S1:
        {
            BecomeSlave( Ebest );
            break;
        }

        default:
        {
            // Don't update data sets in all other cases
            break;
        }
    }
}

// ------------------------------------------------------
// Clock API
// ------------------------------------------------------
void
PTP_Stack::HandleClockEvent( cClockEvent& ClockEvent )
{
    switch( ClockEvent.GetID1() )
    {
        default:
        {
            error( "Unexpected ClockEvent" );
            break;
        }
    }
}

// ------------------------------------------------------
// Protocol functions
// ------------------------------------------------------
void
PTP_Stack::HandleEvent( portEvent_t Event, int PortID )
{
    switch( Event )
    {
        case PORT_EVENT_INITIALIZE:             if( PortID == 0 )
                                                {
                                                    StartInitialize();
                                                }
                                                else
                                                {
                                                    error( "Initialize event can only happen on all ports at once." );
                                                }
                                                break;

        case PORT_EVENT_INITIALIZE_DONE:        if( PortID == 0 )
                                                {
                                                    StopInitialize();
                                                }
                                                else
                                                {
                                                    error( "InitializeDone event can only happen on all ports at once." );
                                                }
                                                break;

        case PORT_EVENT_FAULT_DETECTED:         // Fall-through
        case PORT_EVENT_FAULT_CLEARED:          if( PortID == 0 )
                                                {
                                                    for( int PortIdx = 0; PortIdx < PtpGateSize; PortIdx ++ )
                                                    {
                                                        pPorts[PortIdx].HandleEvent( Event );
                                                    }
                                                }
                                                else
                                                {
                                                    pPorts[PortID-1].HandleEvent( Event );
                                                }
                                                break;

        case PORT_EVENT_POWERUP:                // Fall-through
        case PORT_EVENT_ANNOUNCE_RCV_TIMEOUT:   // Fall-through
        case PORT_EVENT_QUALIFICATION_TIMEOUT:  // Fall-through
        case PORT_EVENT_MASTER_CLOCK_SELECTED:  // Fall-through
        case PORT_EVENT_SYNCHRONIZATION_FAULT:  // Fall-through

        default:                                // Ignore event
                                                break;
    }
}

void
PTP_Stack::HandlePtpFrame( PTPv2_Frame *pPtpFrame, int PortIdx )
{
    if( defaultDS.GetDomainNumber() != pPtpFrame->getDomainNumber() )
    {
        return;
    }

    pPorts[ PortIdx ].HandlePtpFrame( pPtpFrame );
}

void
PTP_Stack::HandlePortRequ( PtpPortRequ *pRequ )
{
    int PortIdx  = pRequ->getArrivalGateId() - RequestGateID;

    CheckPortIdx( PortIdx );

    switch( (tPtpPortRequ) pRequ->getType() )
    {
        case PTP_PORT_REQU_TYPE_TRIG_SFU:
        {
            pPorts[ PortIdx ].HandleSyncFURequ( check_and_cast<PtpPortRequ_TrigSyncFU *>(pRequ) );
            break;
        }
        case PTP_PORT_REQU_TYPE_TRIG_PDFU:
        {
            pPorts[ PortIdx ].HandlePDelayFURequ( check_and_cast<PtpPortRequ_TrigPDelayFU *>(pRequ) );
            break;
        }
        case PTP_PORT_REQU_TYPE_FAULT:
        {
            pPorts[ PortIdx ].HandleEvent( PORT_EVENT_FAULT_DETECTED );
            break;
        }

        case PTP_PORT_REQU_TYPE_RECOVERY:
        {
            pPorts[ PortIdx ].HandleEvent( PORT_EVENT_FAULT_CLEARED );
            break;
        }
    }
}

void
PTP_Stack::StartInitialize()
{
    for( int PortIdx = 0; PortIdx < PtpGateSize; PortIdx ++ )
    {
        pPorts[PortIdx].HandleEvent( PORT_EVENT_INITIALIZE );
    }

    AppStateDecision.StopInterval();

    // Schedule end-of-init

    PTP_EventMsg    *EndInitMsg       = new PTP_EventMsg( "PTP End Init Event"    );

    EndInitMsg->setPortID( 0 );
    EndInitMsg->setEvent( PORT_EVENT_INITIALIZE_DONE );

    scheduleAt( simTime() + simtime_t( par( "StartupInterval" ).doubleValue() ), EndInitMsg );
}

void
PTP_Stack::StopInitialize()
{
    // Start state machine by ending init phase
    for( int PortIdx = 0; PortIdx < PtpGateSize; PortIdx ++ )
    {
        pPorts[PortIdx].HandleEvent( PORT_EVENT_INITIALIZE_DONE );
    }

    // Start STATE_DECISION_EVENT interval
    if( Enabled )
    {
        AppStateDecision.StartInterval();
    }
}

void
PTP_Stack::BecomeSlave( cForeignClockDS Ebest )
{
    // Current DS
    currentDS.SetStepsRemoved( Ebest.GetStepsRemoved() + 1 );

    // Parent DS
    parentDS.ParentPortIdentity()         = Ebest.SenderPortIdentity();
    parentDS.GrandmasterClockQuality()    = Ebest.ClockQuality();

    parentDS.SetGrandmasterIdentity   ( Ebest.ClockIdentity() );
    parentDS.SetGrandmasterPriority1  ( Ebest.GetPriority1()  );
    parentDS.SetGrandmasterPriority2  ( Ebest.GetPriority2()  );
    parentDS.SetGrandMasterModuleID   ( Ebest.GetModuleID()   );

    // Time properties DS
    timePropertiesDS.SetCurrentUtcOffset      ( Ebest.GetCurrentUtcOffset()                   );
    timePropertiesDS.SetCurrentUtcOffsetValid ( Ebest.FlagsField().currentUtcOffsetValid      );
    timePropertiesDS.SetLeap59                ( Ebest.FlagsField().leap59                     );
    timePropertiesDS.SetLeap61                ( Ebest.FlagsField().leap61                     );
    timePropertiesDS.SetTimeTraceable         ( Ebest.FlagsField().timeTraceable              );
    timePropertiesDS.SetFrequencyTraceable    ( Ebest.FlagsField().frequencyTraceable         );
    timePropertiesDS.SetPtpTimescale          ( Ebest.FlagsField().ptpTimescale               );
    timePropertiesDS.SetTimeSource            ( Ebest.GetTimeSource()                         );

    // Activate clock servo
    StartClockServo();
}

void
PTP_Stack::BecomeGrandMaster()
{
    // Current DS
    currentDS.SetStepsRemoved( 0 );
    currentDS.SetOffsetFromMaster( 0 );
    currentDS.SetMeanPathDelay( 0 );

    // Parent DS
    parentDS.ParentPortIdentity().ClockIdentity() = defaultDS.ClockIdentity();
    parentDS.ParentPortIdentity().SetPortNumber( 0 );

    parentDS.GrandmasterClockQuality()  = defaultDS.ClockQuality();

    parentDS.SetGrandmasterIdentity(  defaultDS.ClockIdentity());
    parentDS.SetGrandmasterPriority1( defaultDS.GetPriority1() );
    parentDS.SetGrandmasterPriority2( defaultDS.GetPriority2() );
    parentDS.SetGrandMasterModuleID(  defaultDS.GetModuleID()  );

    // Time properties DS
    timePropertiesDS  = timePropertiesDS_Backup;

    // Deactivate clock servo
    StopClockServo();
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
PTP_Stack::SetMACAddress( MACAddress MAC )
{
    EnterModuleSilent();

    assert( simTime() == SIMTIME_ZERO );

    this->MAC = MAC;

    // Forward new MAC
    if( par( "UseMacForClockID" ).boolValue() )
    {
        SetClockIdentity( cClockIdentity( MAC ) );
    }
    par( "MAC_Address" ).setStringValue( MAC.str() );
    ConfigNIC_MAC();

    LeaveModule();
}

void
PTP_Stack::SetClockIdentity( cClockIdentity ClockID )
{
    EnterModuleSilent();

    assert( simTime() == SIMTIME_ZERO );

    defaultDS.ClockIdentity()                       = ClockID;
    D0.ClockIdentity()                              = ClockID;
    parentDS.ParentPortIdentity().ClockIdentity()   = ClockID;

    for( int PortIdx = 0; PortIdx < PtpGateSize; PortIdx ++ )
    {
        pPorts[PortIdx].PortDS().PortIdentity().ClockIdentity() = ClockID;
    }

    par( "ClockID" ).setStringValue( ClockID.GetString() );

    LeaveModule();
}

void
PTP_Stack::Disable()
{
    EnterModuleSilent();

    AppStateDecision.StopInterval();

    for( int PortIdx = 0; PortIdx < PtpGateSize; PortIdx ++ )
    {
        pPorts[PortIdx].ChangeState( PORT_STATE_DISABLED );
    }

    LeaveModule();
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
int
PTP_Stack::GetModuleID()
{
    return ModuleID;
}
