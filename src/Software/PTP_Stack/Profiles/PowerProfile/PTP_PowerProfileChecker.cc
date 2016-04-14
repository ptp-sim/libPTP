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

#include "PTP_PowerProfileChecker.h"
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

// ------------------------------------------------------------
// Init API
// ------------------------------------------------------------
void
PTP_PowerProfileChecker::ParseParameters()
{
    PTP_ProfileChecker::ParseParameters();

    GrandmasterCapable      = pParentModule->par( "PowerProfile_GrandmasterCapable" );
    PreferredGrandmaster    = pParentModule->par( "PowerProfile_PreferredGrandmaster" );
}

void
PTP_PowerProfileChecker::InitInternalState()
{
    AnnounceReceiptTimeout  = PreferredGrandmaster ? 3 : 2;

    Priority1   = GrandmasterCapable ? 128 : 255;
    Priority2   = GrandmasterCapable ? 128 : 255;
    SlaveOnly   = GrandmasterCapable ? true : false;
}

// ------------------------------------------------------------
// Constructors/Destructor
// ------------------------------------------------------------
PTP_PowerProfileChecker::PTP_PowerProfileChecker()
    : PTP_ProfileChecker()
{
    PTP_Profile             = PTP_PROFILE_POWER;
    GrandmasterCapable      = false;
    PreferredGrandmaster    = false;
}

PTP_PowerProfileChecker::~PTP_PowerProfileChecker()
{
}

// ------------------------------------------------------------
// Setters
// ------------------------------------------------------------

// ------------------------------------------------------------
// Getters
// ------------------------------------------------------------

// ------------------------------------------------------------
// PTP Profile handling
// ------------------------------------------------------------
void
PTP_PowerProfileChecker::CheckParametersFixed( PTP_Stack *pApp )
{
    if( pApp->defaultDS.GetDomainNumber() != 0 )
    {
        HandleWrongInitValue( "defaultDS.domainNumber" );
    }

    for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
    {
        cPortDS& portDS = pApp->pPorts[PortIdx].PortDS();

        if( portDS.GetDelayMechanism() != DELAY_MECH_P2P )
        {
            HandleWrongInitValue( "portDS.delayMechanism" );
        }

        if( portDS.GetLogAnnounceInterval() != 0 )
        {
            HandleWrongInitValue( "portDS.logAnnounceInterval" );
        }

        if( portDS.GetLogSyncInterval() != 0 )
        {
            HandleWrongInitValue( "portDS.logSyncInterval" );
        }

        if( portDS.GetLogMinPdelayReqInterval() != 0 )
        {
            HandleWrongInitValue( "portDS.logMinPdelayReqInterval" );
        }

        if( portDS.GetAnnounceReceiptTimeout() != AnnounceReceiptTimeout )
        {
            HandleWrongInitValue( "portDS.announceReceiptTimeout" );
        }
    }

    if( pApp->defaultDS.GetPriority1() != Priority1 )
    {
        HandleWrongInitValue( "defaultDS.priority1" );
    }

    if( pApp->defaultDS.GetPriority2() != Priority2 )
    {
        HandleWrongInitValue( "defaultDS.priority2" );
    }

    if( pApp->defaultDS.GetSlaveOnly() != SlaveOnly )
    {
        HandleWrongInitValue( "defaultDS.priority2" );
    }
}

void
PTP_PowerProfileChecker::CheckParametersInit( PTP_Stack *pApp )
{
}

void
PTP_PowerProfileChecker::CheckParametersRange( PTP_Stack *pApp )
{
}
