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

#include "PTP_PortDS.h"

#include "PTP_Port.h"
#include "PTP_Stack.h"

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
// Constructors
// ------------------------------------------------------
cPortDS::cPortDS()
    : cSubmoduleInitBase()
{
    this->portIdentity              = cPortIdentity();
    this->portState                 = PORT_STATE_INITIALIZING;
    this->logMinDelayReqInterval    = 0;
    this->peerMeanPathDelay         = 0;
    this->logAnnounceInterval       = 0;
    this->announceReceiptTimeout    = 0;
    this->logSyncInterval           = 0;
    this->delayMechanism            = DELAY_MECH_DISABLED;
    this->logMinPdelayReqInterval   = 0;
    this->versionNumber             = PTP_VERSION_IEEE_1588_2008;

    // Hierarchy specific variables, will be set later
    this->pPort     = nullptr;
}

cPortDS::cPortDS( const cPortDS& other )
    : cSubmoduleInitBase( other )
{
    // Resources
    this->pPort                     = other.pPort;

    // Data set
    this->portIdentity              = other.portIdentity;
    this->portState                 = other.portState;
    this->logMinDelayReqInterval    = other.logMinDelayReqInterval;
    this->peerMeanPathDelay         = other.peerMeanPathDelay;
    this->logAnnounceInterval       = other.logAnnounceInterval;
    this->announceReceiptTimeout    = other.announceReceiptTimeout;
    this->logSyncInterval           = other.logSyncInterval;
    this->delayMechanism            = other.delayMechanism;
    this->logMinPdelayReqInterval   = other.logMinPdelayReqInterval;
    this->versionNumber             = other.versionNumber;
    this->Asymmetry                 = other.Asymmetry;

    // Signals
    this->peerMeanPathDelay_SigId   = other.peerMeanPathDelay_SigId;
    this->portState_SigId           = other.portState_SigId;
}

cPortDS::~cPortDS()
{
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
cPortDS::RegisterSignals()
{
    peerMeanPathDelay_SigId = pPort->RegisterDynamicSignal( "peerMeanPathDelay" );
    portState_SigId         = pPort->RegisterDynamicSignal( "portState" );
}

void
cPortDS::InitSignals()
{
    pParentModule->emit( peerMeanPathDelay_SigId, peerMeanPathDelay.GetSimTime() );
    pParentModule->emit( portState_SigId,         portState );
}

void
cPortDS::SetHierarchy( cPTP_Port *pPort )
{
    this->pPort = pPort;
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------
cPortIdentity&
cPortDS::PortIdentity()
{
    return  this->portIdentity;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cPortDS::SetPeerMeanPathDelay( cTimeInterval meanPathDelay )
{
    this->peerMeanPathDelay     = meanPathDelay;

    pParentModule->emit( peerMeanPathDelay_SigId, peerMeanPathDelay.GetSimTime() );
    pPort->ConfigPortPathDelay( peerMeanPathDelay.GetSimTime() );
}

void
cPortDS::SetPortState( portState_t portState )
{
    this->portState = portState;

    pParentModule->emit( portState_SigId, portState );
}

void
cPortDS::SetLogMinDelayReqInterval( int8_t logMinDelayReqInterval )
{
    this->logMinDelayReqInterval    = logMinDelayReqInterval;
}

void
cPortDS::SetLogAnnounceInterval( int8_t logAnnounceInterval )
{
    this->logAnnounceInterval       = logAnnounceInterval;
}

void
cPortDS::SetAnnounceReceiptTimeout( uint8_t announceReceiptTimeout )
{
    this->announceReceiptTimeout    = announceReceiptTimeout;
}

void
cPortDS::SetLogSyncInterval( int8_t logSyncInterval )
{
    this->logSyncInterval   = logSyncInterval;
}

void
cPortDS::SetDelayMechanism( delayMechanism_t delayMechanism )
{
    this->delayMechanism    = delayMechanism;
}

void
cPortDS::SetLogMinPdelayReqInterval( int8_t logMinPdelayReqInterval )
{
    this->logMinPdelayReqInterval   = logMinPdelayReqInterval;
}

void
cPortDS::SetVersionNumber( UInteger4 versionNumber )
{
    this->versionNumber = versionNumber;
}

void
cPortDS::SetAsymmetry( cTimeInterval Asymmetry )
{
    this->Asymmetry = Asymmetry;

    pPort->ConfigPortAsymmetry( Asymmetry.GetSimTime() );
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
cTimeInterval
cPortDS::GetPeerMeanPathDelay()
{
    return  this->peerMeanPathDelay;
}

portState_t
cPortDS::GetPortState()
{
    return  this->portState;
}

int8_t
cPortDS::GetLogMinDelayReqInterval()
{
    return  this->logMinDelayReqInterval;
}

int8_t
cPortDS::GetLogAnnounceInterval()
{
    return  this->logAnnounceInterval;
}

uint8_t
cPortDS::GetAnnounceReceiptTimeout()
{
    return  this->announceReceiptTimeout;
}

int8_t
cPortDS::GetLogSyncInterval()
{
    return  this->logSyncInterval;
}

delayMechanism_t
cPortDS::GetDelayMechanism()
{
    return  this->delayMechanism;
}

int8_t
cPortDS::GetLogMinPdelayReqInterval()
{
    return  this->logMinPdelayReqInterval;
}

UInteger4
cPortDS::GetVersionNumber()
{
    return  this->versionNumber;
}

cTimeInterval
cPortDS::GetAsymmetry()
{
    return  this->Asymmetry;
}

// ------------------------------------------------------
// Compare operator
// ------------------------------------------------------
bool
cPortDS::operator== (const cPortDS& other)
{
    if
    (
        ( this->portIdentity            == other.portIdentity           ) &&
        ( this->portState               == other.portState              ) &&
        ( this->logMinDelayReqInterval  == other.logMinDelayReqInterval ) &&
        ( this->peerMeanPathDelay       == other.peerMeanPathDelay      ) &&
        ( this->logAnnounceInterval     == other.logAnnounceInterval    ) &&
        ( this->announceReceiptTimeout  == other.announceReceiptTimeout ) &&
        ( this->logSyncInterval         == other.logSyncInterval        ) &&
        ( this->delayMechanism          == other.delayMechanism         ) &&
        ( this->logMinPdelayReqInterval == other.logMinPdelayReqInterval) &&
        ( this->versionNumber           == other.versionNumber          )
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ------------------------------------------------------
// Assign operator
// ------------------------------------------------------
cPortDS&
cPortDS::operator= (const cPortDS& other)
{
    cSubmoduleInitBase::operator=( other );

    // Resources
    this->pPort                     = other.pPort;

    // Data set
    this->portIdentity              = other.portIdentity;
    this->portState                 = other.portState;
    this->logMinDelayReqInterval    = other.logMinDelayReqInterval;
    this->peerMeanPathDelay         = other.peerMeanPathDelay;
    this->logAnnounceInterval       = other.logAnnounceInterval;
    this->announceReceiptTimeout    = other.announceReceiptTimeout;
    this->logSyncInterval           = other.logSyncInterval;
    this->delayMechanism            = other.delayMechanism;
    this->logMinPdelayReqInterval   = other.logMinPdelayReqInterval;
    this->versionNumber             = other.versionNumber;
    this->Asymmetry                 = other.Asymmetry;

    // Signals
    this->peerMeanPathDelay_SigId   = other.peerMeanPathDelay_SigId;
    this->portState_SigId           = other.portState_SigId;

    // By convention, always return *this
    return *this;
}

// ------------------------------------------------------
// Debug functions
// ------------------------------------------------------
void
cPortDS::Print()
{
    EV << "===============================================================" << endl;
    EV << "      Port Data Set " << portIdentity.GetPortNumber()            << endl;
    EV << "===============================================================" << endl << endl;

    EV << "PortIdentity:\t"             << this->portIdentity.GetString()       << endl;
    EV << "PortState:\t"                << this->portState                      << endl;
    EV << "LogMinDelayReqInterval:\t"   << (int) this->logMinDelayReqInterval   << endl;
    EV << "PeerMeanPathDelay:\t"        << this->peerMeanPathDelay.GetString()  << endl;
    EV << "LogAnnounceInterval:\t"      << (int) this->logAnnounceInterval      << endl;
    EV << "AnnounceReceiptTimeout:\t"   << (int) this->announceReceiptTimeout   << endl;
    EV << "LogSyncInterval"             << (int) this->logSyncInterval          << endl;
    EV << "DelayMechanism:\t"           << this->delayMechanism                 << endl;
    EV << "LogMinPdelayReqInterval:\t"  << (int) this->logMinPdelayReqInterval  << endl;
    EV << "VersionNumber:\t"            << (int) this->versionNumber            << endl;
    EV << "Asymmetry:\t"                << this->Asymmetry.GetSimTime()         << endl;
    EV << endl;
}
