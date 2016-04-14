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

#include "PTP_DefaultE2EProfileChecker.h"
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
PTP_DefaultE2EProfileChecker::ParseParameters()
{
    PTP_DefaultProfileChecker::ParseParameters();
}

// ------------------------------------------------------------
// Constructors/Destructor
// ------------------------------------------------------------
PTP_DefaultE2EProfileChecker::PTP_DefaultE2EProfileChecker()
    : PTP_DefaultProfileChecker()
{
    PTP_Profile = PTP_PROFILE_DEFAULT_E2E;
}

PTP_DefaultE2EProfileChecker::~PTP_DefaultE2EProfileChecker()
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
PTP_DefaultE2EProfileChecker::CheckParametersFixed( PTP_Stack *pApp )
{
    PTP_DefaultProfileChecker::CheckParametersFixed( pApp );
}

void
PTP_DefaultE2EProfileChecker::CheckParametersInit( PTP_Stack *pApp )
{
    PTP_DefaultProfileChecker::CheckParametersInit( pApp );

    for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
    {
        cPortDS& portDS = pApp->pPorts[PortIdx].PortDS();

        if( portDS.GetDelayMechanism() != DELAY_MECH_E2E )
        {
            HandleWrongInitValue( "portDS.delayMechanism" );
        }

        if( portDS.GetLogMinDelayReqInterval() != 0 )
        {
            HandleWrongInitValue( "portDS.logMinDelayReqInterval" );
        }
    }
}

void
PTP_DefaultE2EProfileChecker::CheckParametersRange( PTP_Stack *pApp )
{
    PTP_DefaultProfileChecker::CheckParametersRange( pApp );

    for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
    {
        cPortDS& portDS = pApp->pPorts[PortIdx].PortDS();

        if
        (
            ( portDS.GetLogMinDelayReqInterval() < 0 ) ||
            ( portDS.GetLogMinDelayReqInterval() > 5 )
        )
        {
            HandleRangeError( "portDS.logMinDelayReqInterval" );
        }
    }
}
