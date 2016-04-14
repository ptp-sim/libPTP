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

#include "PTP_DefaultP2PProfileChecker.h"
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
PTP_DefaultP2PProfileChecker::ParseParameters()
{
    PTP_DefaultProfileChecker::ParseParameters();
}

// ------------------------------------------------------------
// Constructors/Destructor
// ------------------------------------------------------------
PTP_DefaultP2PProfileChecker::PTP_DefaultP2PProfileChecker()
    : PTP_DefaultProfileChecker()
{
    PTP_Profile = PTP_PROFILE_DEFAULT_P2P;
}

PTP_DefaultP2PProfileChecker::~PTP_DefaultP2PProfileChecker()
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
PTP_DefaultP2PProfileChecker::CheckParametersFixed( PTP_Stack *pApp )
{
    PTP_DefaultProfileChecker::CheckParametersFixed( pApp );
}

void
PTP_DefaultP2PProfileChecker::CheckParametersInit( PTP_Stack *pApp )
{
    PTP_DefaultProfileChecker::CheckParametersInit( pApp );

    for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
    {
        cPortDS& portDS = pApp->pPorts[PortIdx].PortDS();

        if( portDS.GetDelayMechanism() != DELAY_MECH_E2E )
        {
            HandleWrongInitValue( "portDS.delayMechanism" );
        }

        if( portDS.GetLogMinPdelayReqInterval() != 0 )
        {
            HandleWrongInitValue( "portDS.logMinPdelayReqInterval" );
        }
    }
}

void
PTP_DefaultP2PProfileChecker::CheckParametersRange( PTP_Stack *pApp )
{
    PTP_DefaultProfileChecker::CheckParametersRange( pApp );

    for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
    {
        cPortDS& portDS = pApp->pPorts[PortIdx].PortDS();

        if
        (
            ( portDS.GetLogMinPdelayReqInterval() < 0 ) ||
            ( portDS.GetLogMinPdelayReqInterval() > 5 )
        )
        {
            HandleRangeError( "portDS.logMinPdelayReqInterval" );
        }
    }
}
