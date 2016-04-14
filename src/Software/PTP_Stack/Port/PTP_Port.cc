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

#include "PTP.h"
#include "PTP_Stack.h"
#include "PTP_Port.h"

#include "DynamicSignals.h"

#include "PTPv2_m.h"
#include "PtpPortConfig_m.h"

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
// Constructor
// ------------------------------------------------------
cPTP_Port::cPTP_Port()
: cSubmoduleInitBase(),
  appAnnounce   (),
  appSync       (),
  appDelay      (),
  appPDelay     ()
{
    this->pApp  = nullptr;
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------
cPortDS&
cPTP_Port::PortDS()
{
    return  this->portDS;
}

cForeignMasterDS&
cPTP_Port::ForeignMasterDS()
{
    return  this->foreignMasterDS;
}

cAppAnnounce&
cPTP_Port::AppAnnounce()
{
    return  this->appAnnounce;
}

cAppSync&
cPTP_Port::AppSync()
{
    return  this->appSync;
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
cPTP_Port::ParseParameters()
{
    StateChange_EnableDebugOutput = pParentModule->par( "Port_StateChange_EnableDebugOutput" ).boolValue();
}

void
cPTP_Port::InitHierarchy()
{
    // Data sets
    portDS.SetParentModule( pParentModule );
    portDS.SetHierarchy( this );
    foreignMasterDS.SetParentModule( pParentModule );
    foreignMasterDS.SetHierarchy( this, pClock );

    // Services
    appAnnounce.SetParentModule ( pParentModule );
    appSync.SetParentModule     ( pParentModule );
    appDelay.SetParentModule    ( pParentModule );
    appPDelay.SetParentModule   ( pParentModule );

    appAnnounce.SetCallableParentModule ( pApp );
    appSync.SetCallableParentModule     ( pApp );
    appDelay.SetCallableParentModule    ( pApp );
    appPDelay.SetCallableParentModule   ( pApp );

    appAnnounce.SetHierarchy    ( pApp, this, pClock );
    appSync.SetHierarchy        ( pApp, this, pClock, pClockServo );
    appDelay.SetHierarchy       ( pApp, this, pClock );
    appPDelay.SetHierarchy      ( pApp, this, pClock );
}

void
cPTP_Port::RegisterSignals()
{
    StateDecision_SigId      = RegisterDynamicSignal( "StateDecision" );
}

void
cPTP_Port::InitInternalState()
{
    // Set up timeouts
    appSync.SetInterval     ( pow( 2.0, portDS.GetLogSyncInterval()         ) );
    appDelay.SetInterval    ( pow( 2.0, portDS.GetLogMinDelayReqInterval()  ) );
    appPDelay.SetInterval   ( pow( 2.0, portDS.GetLogMinPdelayReqInterval() ) );
    appAnnounce.SetInterval ( pow( 2.0, portDS.GetLogAnnounceInterval()     ) );
    appAnnounce.SetTimeout( portDS.GetAnnounceReceiptTimeout() * pow( 2.0, portDS.GetLogAnnounceInterval() ) );
}

void
cPTP_Port::InitSignals()
{
}

void
cPTP_Port::ForwardInit( int stage )
{
    // Data sets
    portDS.initialize           ( stage );
    foreignMasterDS.initialize  ( stage );

    // Services
    appAnnounce.initialize      ( stage );
    appSync.initialize          ( stage );
    appDelay.initialize         ( stage );
    appPDelay.initialize        ( stage );
}

void
cPTP_Port::SetHierarchy( PTP_Stack *pApp, cScheduleClock *pClock, IClockServo *pClockServo, UInteger16 portNumber )
{
    this->pApp          = pApp;
    this->pClock        = pClock;
    this->pClockServo   = pClockServo;
    this->portNumber    = portNumber;
}

// ------------------------------------------------------
// API functions
// ------------------------------------------------------
void
cPTP_Port::ChangeState( portState_t NewState )
{
    if( portDS.GetPortState() == NewState )
    {
        return;
    }

    if( StateChange_EnableDebugOutput )
    {
        EV << "------------------------------------------------------------------------------------------------------------" << endl;
        EV << "Port " << portDS.PortIdentity().GetPortNumber() << ": ";
        EV << "Changing port state from " <<  portDS.GetPortState() << " to " << NewState << endl;
        EV << "------------------------------------------------------------------------------------------------------------" << endl;
    }

    // Change service activity according to new port  state
    switch( NewState )
    {
        case PORT_STATE_INITIALIZING:
        case PORT_STATE_FAULTY:
        case PORT_STATE_DISABLED:
        {
            appSync.StopInterval();
            appSync.StopTimeout();
            appSync.StopListening();
            appAnnounce.StopListening();
            appAnnounce.StopInterval();
            appDelay.StopInterval();
            appPDelay.StopInterval();
            break;
        }

        case PORT_STATE_LISTENING:
        {
            appSync.StopInterval();
            appSync.StopTimeout();
            appSync.StopListening();
            appAnnounce.StopInterval();
            appDelay.StopInterval();

            appAnnounce.StartListening();

            switch( portDS.GetDelayMechanism() )
            {
                case DELAY_MECH_P2P:        appPDelay.StartInterval();
                                            break;

                default:
                case DELAY_MECH_E2E:
                case DELAY_MECH_DISABLED:   appPDelay.StopInterval();
                                            break;
            }
            break;
        }

        case PORT_STATE_PRE_MASTER:
        {
            appSync.StopInterval();
            appSync.StopListening();
            appAnnounce.StopListening();
            appAnnounce.StopInterval();
            appDelay.StopInterval();

            appSync.StartTimeout();

            switch( portDS.GetDelayMechanism() )
            {
                case DELAY_MECH_P2P:        appPDelay.StartInterval();
                                            break;

                default:
                case DELAY_MECH_E2E:
                case DELAY_MECH_DISABLED:   appPDelay.StopInterval();
                                            break;
            }
            break;
        }

        case PORT_STATE_MASTER:
        {
            appSync.StopTimeout();
            appSync.StopListening();
            appAnnounce.StopListening();
            appDelay.StopInterval();

            appSync.StartInterval();
            appAnnounce.StartInterval();

            switch( portDS.GetDelayMechanism() )
            {
                case DELAY_MECH_P2P:        appPDelay.StartInterval();
                                            break;

                default:
                case DELAY_MECH_E2E:
                case DELAY_MECH_DISABLED:   appPDelay.StopInterval();
                                            break;
            }
            break;
        }

        case PORT_STATE_PASSIVE:
        {
            appSync.StopInterval();
            appSync.StopTimeout();
            appSync.StopListening();
            appAnnounce.StopInterval();
            appDelay.StopInterval();

            appAnnounce.StartListening();

            switch( portDS.GetDelayMechanism() )
            {
                case DELAY_MECH_P2P:        appPDelay.StartInterval();
                                            break;

                default:
                case DELAY_MECH_E2E:
                case DELAY_MECH_DISABLED:   appPDelay.StopInterval();
                                            break;
            }
            break;
        }

        case PORT_STATE_UNCALIBRATED:
        case PORT_STATE_SLAVE:
        {
            appSync.StopInterval();
            appSync.StopTimeout();
            appAnnounce.StopInterval();

            appSync.StartListening();
            appAnnounce.StartListening();

            switch( portDS.GetDelayMechanism() )
            {
                case DELAY_MECH_P2P:        appDelay.StopInterval();
                                            appPDelay.StartInterval();
                                            break;

                case DELAY_MECH_E2E:        if
                                            (
                                                ( pApp->PTP_ClockType == PTP_CLOCK_TYPE_ORDINARY ) ||
                                                (
                                                    ( pApp->PTP_ClockType == PTP_CLOCK_TYPE_TRANSPARENT ) &&
                                                    ( pApp->Active_E2E_TC )
                                                )
                                            )
                                            {
                                                appDelay.StartInterval();
                                            }
                                            else
                                            {
                                                appDelay.StopInterval();
                                            }
                                            appPDelay.StopInterval();
                                            break;

                default:
                case DELAY_MECH_DISABLED:   appPDelay.StopInterval();
                                            appDelay.StopInterval();
                                            break;
            }
            break;
        }
    }

    // Set new state
    portDS.SetPortState( NewState );
}

void
cPTP_Port::FinishInitPhase()
{
    if( portDS.GetPortState() == PORT_STATE_INITIALIZING )
    {
        // Start state machine
        if( pApp->Enabled )
        {
            ChangeState( PORT_STATE_LISTENING );
        }
        else
        {
            ChangeState( PORT_STATE_DISABLED );
        }
    }
    else
    {
        throw cRuntimeError("Port: Was told to finish init state when not in init state.");
    }
}

void
cPTP_Port::HandleEvent( portEvent_t Event )
{
    switch( Event )
    {
        case PORT_EVENT_POWERUP:
        {
            ChangeState( PORT_STATE_INITIALIZING );
            break;
        }

        case PORT_EVENT_INITIALIZE:
        {
            ChangeState( PORT_STATE_INITIALIZING );
            break;
        }

        case PORT_EVENT_INITIALIZE_DONE:
        {
            if( portDS.GetPortState() != PORT_STATE_INITIALIZING )
            {
                throw cRuntimeError( "PTP Port: Received INITIALIZE_DONE event in a non-Initializing state" );
            }

            if( pApp->Enabled )
            {
                ChangeState( PORT_STATE_LISTENING );
            }
            else
            {
                ChangeState( PORT_STATE_DISABLED );
            }
            break;
        }

        case PORT_EVENT_FAULT_DETECTED:
        {
            switch( portDS.GetPortState() )
            {
                case PORT_STATE_LISTENING:
                case PORT_STATE_UNCALIBRATED:
                case PORT_STATE_SLAVE:
                case PORT_STATE_PRE_MASTER:
                case PORT_STATE_MASTER:
                case PORT_STATE_PASSIVE:
                case PORT_STATE_FAULTY:
                {
                    ChangeState( PORT_STATE_FAULTY );
                    break;
                }

                default:
                {
                    // Ignore this event
                    break;
                }
            }
            break;
        }

        case PORT_EVENT_FAULT_CLEARED:
        {
            if( portDS.GetPortState() == PORT_STATE_FAULTY )
            {
                pApp->StartInitialize();
            }
            break;
        }

        case PORT_EVENT_STATE_DECISION:
        {
            // This event is handled in the application for all ports
            break;
        }

        case PORT_EVENT_ANNOUNCE_RCV_TIMEOUT:
        {
            switch( portDS.GetPortState() )
            {
                case PORT_STATE_LISTENING:
                case PORT_STATE_UNCALIBRATED:
                case PORT_STATE_SLAVE:
                {
                    if( pApp->defaultDS.GetSlaveOnly() )
                    {
                        ChangeState( PORT_STATE_LISTENING );
                    }
                    else if
                    (
                        ( pApp->PTP_ClockType == PTP_CLOCK_TYPE_TRANSPARENT ) &&
                        ( pApp->parentDS.GetGrandmasterIdentity() != pApp->defaultDS.ClockIdentity() )
                    )
                    {
                        ChangeState( PORT_STATE_LISTENING );
                    }
                    else
                    {
                        ChangeState( PORT_STATE_MASTER );
                    }
                    break;
                }

                case PORT_STATE_PASSIVE:
                {
                    ChangeState( PORT_STATE_MASTER );
                    break;
                }

                default:
                {
                    // Ignore this event
                    break;
                }
            }
            break;
        }

        case PORT_EVENT_QUALIFICATION_TIMEOUT:
        {
            if( portDS.GetPortState() == PORT_STATE_PRE_MASTER )
            {
                ChangeState( PORT_STATE_MASTER );
            }
        }

        case PORT_EVENT_MASTER_CLOCK_SELECTED:
        {
            if( portDS.GetPortState() == PORT_STATE_UNCALIBRATED )
            {
                ChangeState( PORT_STATE_SLAVE );
            }
            break;
        }

        case PORT_EVENT_SYNCHRONIZATION_FAULT:
        {
            if( portDS.GetPortState() == PORT_STATE_SLAVE )
            {
                ChangeState( PORT_STATE_UNCALIBRATED );
            }
            break;
        }

        case PORT_EVENT_DESIGNATED_ENABLE:
        {
            if( portDS.GetPortState() == PORT_STATE_DISABLED )
            {
                ChangeState( PORT_STATE_INITIALIZING );
            }
        }

        case PORT_EVENT_DESIGNATED_DISABLE:
        {
            switch( portDS.GetPortState() )
            {
                case PORT_STATE_LISTENING:
                case PORT_STATE_UNCALIBRATED:
                case PORT_STATE_SLAVE:
                case PORT_STATE_PRE_MASTER:
                case PORT_STATE_MASTER:
                case PORT_STATE_PASSIVE:
                case PORT_STATE_FAULTY:
                {
                    ChangeState( PORT_STATE_DISABLED );
                    break;
                }

                default:
                {
                    // Ignore this event
                    break;
                }
            }
            break;
        }
    }
}

void
cPTP_Port::HandleStateDecision( portStateDecision_t StateDecision, bool NewMaster )
{
    portState_t     CurState    = portDS.GetPortState();
    portState_t     NextState;

    switch( CurState )
    {
        case PORT_STATE_INITIALIZING:
        case PORT_STATE_FAULTY:
        case PORT_STATE_DISABLED:
        {
            // These states do not react on State decisions
            return;
        }

        default:
            break;
    }

    // The qualificationTimeoutInterval shall be N multiplied by the announceInterval (see 7.7.2.2), in seconds, where:
    // a) If the recommended state = MASTER event was based on decision points M1 or M2 of Figure 26, N shall be 0
    // b) If the recommended state = MASTER event was based on decision point M3 of Figure 26, N shall be the value incremented by 1 (one) of the currentDS.stepsRemoved field.
    if
    (
        ( StateDecision == PORT_SD_M1 ) ||
        ( StateDecision == PORT_SD_M2 )
    )
    {
        appSync.SetTimeout( SIMTIME_ZERO );
    }
    else if( StateDecision == PORT_SD_M3 )
    {
        size_t      N                   = pApp->currentDS.GetStepsRemoved() + 1;
        simtime_t   announceInterval    = pow( 2.0, portDS.GetLogAnnounceInterval() );

        appSync.SetTimeout( announceInterval * N );
    }

    switch( StateDecision )
    {
        case PORT_SD_LIST:
        {
            if( CurState == PORT_STATE_LISTENING )
            {
                return;
            }
            else
            {
                throw cRuntimeError("REMAIN_LISTENING called in state other than Listening");
            }
            break;
        }

        case PORT_SD_M1:
        case PORT_SD_M2:
        case PORT_SD_M3:
        {
            switch( CurState )
            {
                default:
                    return;

                case PORT_STATE_LISTENING:
                case PORT_STATE_UNCALIBRATED:
                case PORT_STATE_SLAVE:
                case PORT_STATE_PASSIVE:
                case PORT_STATE_PRE_MASTER:

                    if
                    (
                        ( pApp->defaultDS.GetSlaveOnly() ) ||
                        (
                            ( pApp->PTP_ClockType == PTP_CLOCK_TYPE_TRANSPARENT ) &&
                            ( StateDecision == PORT_SD_M3 )
                        )
                    )
                    {
                        NextState   = PORT_STATE_LISTENING;
                    }
                    else
                    {
                        NextState   = PORT_STATE_PRE_MASTER;
                    }
                    break;

                case PORT_STATE_MASTER:

                    if
                    (
                        ( pApp->defaultDS.GetSlaveOnly() ) ||
                        (
                            ( pApp->PTP_ClockType == PTP_CLOCK_TYPE_TRANSPARENT ) &&
                            ( StateDecision == PORT_SD_M3 )
                        )
                    )
                    {
                        NextState   = PORT_STATE_PASSIVE;
                    }
                    else
                    {
                        NextState   = PORT_STATE_MASTER;
                    }
                    break;
            }
            break;
        }

        // Recommended state == passive
        case PORT_SD_P1:
        case PORT_SD_P2:
        {
            switch( CurState )
            {
                default:
                    return;

                case PORT_STATE_LISTENING:
                case PORT_STATE_UNCALIBRATED:
                case PORT_STATE_SLAVE:
                case PORT_STATE_PRE_MASTER:
                case PORT_STATE_MASTER:
                case PORT_STATE_PASSIVE:

                    NextState   = PORT_STATE_PASSIVE;
                    break;
            }
            break;
        }

        // Recommended state == slave
        case PORT_SD_S1:
        {
            switch( CurState )
            {
                default:
                    return;

                case PORT_STATE_LISTENING:
                case PORT_STATE_UNCALIBRATED:
                case PORT_STATE_PRE_MASTER:
                case PORT_STATE_MASTER:
                case PORT_STATE_PASSIVE:

                    NextState   = PORT_STATE_UNCALIBRATED;
                    break;

                case PORT_STATE_SLAVE:

                    if( NewMaster )
                    {
                        NextState   = PORT_STATE_UNCALIBRATED;
                    }
                    else
                    {
                        return;
                    }
                    break;
            }
            break;
        }
    }

    ChangeState( NextState );
}

void
cPTP_Port::HandlePtpFrame( PTPv2_Frame  *pPtpFrame )
{
    switch( pPtpFrame->getMessageType() )
    {
        case PTP_TYPE_SYNC:
        case PTP_TYPE_FOLLOW_UP:
        {
            appSync.HandleMsg( pPtpFrame );
            appDelay.HandleMsg( pPtpFrame );
            break;
        }

        case PTP_TYPE_DELAY_REQ:
        case PTP_TYPE_DELAY_RESP:
        {
            appDelay.HandleMsg( pPtpFrame );
            break;
        }

        case PTP_TYPE_PDELAY_REQ:
        case PTP_TYPE_PDELAY_RESP:
        case PTP_TYPE_PDELAY_RESP_FU:
        {
            appPDelay.HandleMsg( pPtpFrame );
            break;
        }

        case PTP_TYPE_ANNOUNCE:
        {
            this->appAnnounce.HandleMsg(pPtpFrame);
            break;
        }

        case PTP_TYPE_SIGNALING:
        case PTP_TYPE_MANAGEMENT:
        {
            // Not implemented
            break;
        }

        default:
        {
            break;
        }
    }
}

void
cPTP_Port::HandleSyncFURequ( PtpPortRequ_TrigSyncFU *pRequ )
{
    appSync.TriggerFollowUp( &pRequ->getSyncFrame() );
}

void
cPTP_Port::HandlePDelayFURequ( PtpPortRequ_TrigPDelayFU *pRequ )
{
    appPDelay.TriggerFollowUp( &pRequ->getPDelayFrame() );
}

cForeignClockDS
cPTP_Port::GetErbest()
{
    return foreignMasterDS.GetErbest();
}

// ------------------------------------------------------
// State Decision Algorithm
// Specified in Chapter 9.3.3 and Figure 26
// ------------------------------------------------------

portStateDecision_t
cPTP_Port::GetStateDecision( cForeignClockDS Ebest, UInteger16 EbestPortID, bool EnableDebugOutput )
{
    ClockClass_t                D0_ClockClass;
    ClockCompReturn_t           CompReturn;
    portStateDecision_t         StateDecision   = PORT_SD_LIST;

    if( EnableDebugOutput )
    {
        EV << "Calculating state decision for Port " << portDS.PortIdentity().GetPortNumber() << endl;
    }

    if
    (
        ( foreignMasterDS.GetErbest().IsEmpty()          )  &&
        ( portDS.GetPortState() == PORT_STATE_LISTENING  )
    )
    {
        if( EnableDebugOutput )
        {
            EV << "--> state decision: stay in listening" << endl;
        }

        StateDecision = PORT_SD_LIST;
    }
    else
    {
        if( EnableDebugOutput )
        {
            EV << "... don't stay in listening" << endl;
        }

        D0_ClockClass   = pApp->D0.ClockQuality().GetClockClass();

        if
        (
            ( 1     <= D0_ClockClass )  &&
            ( 127   >= D0_ClockClass )
        )
        {
            if( EnableDebugOutput )
            {
                EV << "... good clock class" << endl;
                EV << "... comparing D0, Erbest" << endl;
            }

            CompReturn  = cForeignClockDS::CompareClockDS( pApp->D0, GetErbest(), EnableDebugOutput );

            if
            (
                ( CompReturn.Result == ClockCompResult::A_BETTER_B          )  ||
                ( CompReturn.Result == ClockCompResult::A_BETTER_B_BY_TOPO  )
            )
            {
                if( EnableDebugOutput )
                {
                    EV << "... A better B" << endl;
                    EV << "...... Details:" << endl;
                    EV << "...... Result: " << CompReturn.Result << endl;
                    EV << "...... Reason: " << CompReturn.Reason << endl;
                    EV << "--> state decision: M1" << endl;
                }

                StateDecision = PORT_SD_M1;
            }
            else
            {
                if( EnableDebugOutput )
                {
                    EV << "... A not better B" << endl;
                    EV << "...... Details:" << endl;
                    EV << "...... Result: " << CompReturn.Result << endl;
                    EV << "...... Reason: " << CompReturn.Reason << endl;
                    EV << "--> state decision: P1" << endl;
                }

                StateDecision = PORT_SD_P1;
            }
        }
        else
        {
            if( EnableDebugOutput )
            {
                EV << "... bad clock class" << endl;
                EV << "... comparing D0, Ebest" << endl;
            }

            CompReturn  = cForeignClockDS::CompareClockDS( pApp->D0, Ebest, EnableDebugOutput );

            if
            (
                ( CompReturn.Result == ClockCompResult::A_BETTER_B          )  ||
                ( CompReturn.Result == ClockCompResult::A_BETTER_B_BY_TOPO  )
            )
            {
                if( EnableDebugOutput )
                {
                    EV << "... A better B" << endl;
                    EV << "...... Details:" << endl;
                    EV << "...... Result: " << CompReturn.Result << endl;
                    EV << "...... Reason: " << CompReturn.Reason << endl;
                    EV << "--> state decision: M2" << endl;
                }

                StateDecision = PORT_SD_M2;
            }
            else
            {
                if( EnableDebugOutput )
                {
                    EV << "... A is not better" << endl;
                    EV << "...... Details:" << endl;
                    EV << "...... Result: " << CompReturn.Result << endl;
                    EV << "...... Reason: " << CompReturn.Reason << endl;
                    EV << "... checking if this the Ebest-port" << endl;
                }

                if( portDS.PortIdentity().GetPortNumber() == EbestPortID )
                {
                    if( EnableDebugOutput )
                    {
                        EV << "...... this is the Ebest port" << endl;
                        EV << "--> state decision: S1" << endl;
                    }

                    StateDecision = PORT_SD_S1;
                }
                else
                {
                    if( EnableDebugOutput )
                    {
                        EV << "...... this is not the Ebest port" << endl;
                        EV << "... comparing Ebest, Erbest" << endl;
                    }

                    CompReturn  = cForeignClockDS::CompareClockDS( Ebest, GetErbest(), EnableDebugOutput );

                    assert( CompReturn.Result != ClockCompResult::B_BETTER_A );
                    assert( CompReturn.Result != ClockCompResult::B_BETTER_A_BY_TOPO );

                    if( CompReturn.Result == ClockCompResult::A_BETTER_B_BY_TOPO )
                    {
                        if( EnableDebugOutput )
                        {
                            EV << "... A better B by topo" << endl;
                            EV << "...... Details:" << endl;
                            EV << "...... Result: " << CompReturn.Result << endl;
                            EV << "...... Reason: " << CompReturn.Reason << endl;
                            EV << "--> state decision: P2" << endl;
                        }

                        StateDecision = PORT_SD_P2;
                    }
                    else
                    {
                        if( EnableDebugOutput )
                        {
                            EV << "... A is not better B by topo" << endl;
                            EV << "...... Details:" << endl;
                            EV << "...... Result: " << CompReturn.Result << endl;
                            EV << "...... Reason: " << CompReturn.Reason << endl;
                            EV << "--> state decision: M3" << endl;
                        }

                        StateDecision = PORT_SD_M3;
                    }
                }
            }
        }
    }

    pParentModule->emit( StateDecision_SigId, StateDecision );

    return StateDecision;
}

// ------------------------------------------------------
// API for services
// ------------------------------------------------------
void
cPTP_Port::IssueFrame( PTPv2_Frame *pPTP )
{
    pApp->IssueFrame( pPTP, portDS.PortIdentity().GetPortNumber() );
}

simsignal_t
cPTP_Port::RegisterDynamicSignal( const char *pSigName )
{
    return  DynamicSignals::RegisterDynamicSignal( pParentModule, "Port", portNumber, pSigName, pSigName );
}

void
cPTP_Port::ConfigPortPathDelay( simtime_t meanPathDelay )
{
    pApp->ConfigPortPathDelay( portDS.PortIdentity().GetPortNumber(), meanPathDelay );
}

void
cPTP_Port::ConfigPortAsymmetry( simtime_t Asymmetry )
{
    pApp->ConfigPortAsymmetry( portDS.PortIdentity().GetPortNumber(), Asymmetry );
}

// ------------------------------------------------------
// API for stack
// ------------------------------------------------------
void
cPTP_Port::HandleTimejump( simtime_t Delta )
{
    foreignMasterDS.AdjustTimestamps( Delta );

    appSync.HandleTimeJump();
    appDelay.HandleTimeJump();
    appPDelay.HandleTimeJump();
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------

// ------------------------------------------------------
// Getters
// ------------------------------------------------------

// ------------------------------------------------------
// Instance functions
// ------------------------------------------------------

