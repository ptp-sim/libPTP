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

#include "PTP_DefaultProfileChecker.h"
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
PTP_DefaultProfileChecker::ParseParameters()
{
    PTP_ProfileChecker::ParseParameters();
}

// ------------------------------------------------------------
// Constructors/Destructor
// ------------------------------------------------------------
PTP_DefaultProfileChecker::PTP_DefaultProfileChecker()
    : PTP_ProfileChecker()
{
}

PTP_DefaultProfileChecker::~PTP_DefaultProfileChecker()
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
PTP_DefaultProfileChecker::CheckParametersFixed( PTP_Stack *pApp )
{
    if( pApp->BMCA != BMCA_1588_2008_DEFAULT )
    {
        HandleWrongInitValue( "Best Master Clock Algorithm" );
    }

    if( pApp->MgmtProtocol != MGMT_1588_2008_DEFAULT )
    {
        HandleWrongInitValue( "Management protocol" );
    }
}

void
PTP_DefaultProfileChecker::CheckParametersInit( PTP_Stack *pApp )
{
    if( pApp->defaultDS.GetDomainNumber() != 0 )
    {
        HandleWrongInitValue( "defaultDS.domainNumber" );
    }

    if( pApp->defaultDS.GetPriority1() != 128 )
    {
        HandleWrongInitValue( "defaultDS.priority1" );
    }

    if( pApp->defaultDS.GetPriority2() != 128 )
    {
        HandleWrongInitValue( "defaultDS.priority2" );
    }

    // Remark: The following parameters are not checked here on purpose,
    // as these checks would not fit well with the current implementation
    //
    // SlaveOnly flag
    // transparentClockDS.primaryDomain
    // Tau

    for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
    {
        cPortDS& portDS = pApp->pPorts[PortIdx].PortDS();

        if( portDS.GetLogAnnounceInterval() != 1 )
        {
            HandleWrongInitValue( "portDS.logAnnounceInterval" );
        }

        if( portDS.GetLogSyncInterval() != 0 )
        {
            HandleWrongInitValue( "portDS.logSyncInterval" );
        }

        if( portDS.GetAnnounceReceiptTimeout() != 3 )
        {
            HandleWrongInitValue( "portDS.announceReceiptTimeout" );
        }
    }
}

void
PTP_DefaultProfileChecker::CheckParametersRange( PTP_Stack *pApp )
{
    for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
    {
        cPortDS& portDS = pApp->pPorts[PortIdx].PortDS();

        if
        (
            ( portDS.GetLogAnnounceInterval() < 0 ) ||
            ( portDS.GetLogAnnounceInterval() > 4 )
        )
        {
            HandleRangeError( "portDS.logAnnounceInterval" );
        }

        if
        (
            ( portDS.GetLogSyncInterval() < -1 ) ||
            ( portDS.GetLogSyncInterval() > 1 )
        )
        {
            HandleRangeError( "portDS.logSyncInterval" );
        }

        if
        (
            ( portDS.GetAnnounceReceiptTimeout() < 2 ) ||
            ( portDS.GetAnnounceReceiptTimeout() > 10 )
        )
        {
            HandleRangeError( "portDS.announceReceiptTimeout" );
        }
    }
}
