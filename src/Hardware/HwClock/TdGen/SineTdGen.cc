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

#include "SineTdGen.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Constants
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
// Init API
// ------------------------------------------------------

void
cSineTdGen::ParseParameters()
{
    f   = pParentModule->par( "SineTdGen_f"   ).doubleValue();
    A   = pParentModule->par( "SineTdGen_A"   ).doubleValue();
    phi = pParentModule->par( "SineTdGen_phi" ).doubleValue();

    if( A * f > 1.0 )
    {
        throw cRuntimeError( "Invalid parameters" );
    }
}

void
cSineTdGen::RegisterSignals()
{
}

// ------------------------------------------------------
// Constructors/Destructor
// ------------------------------------------------------

cSineTdGen::cSineTdGen()
    : ITdGen( "SineTdGen" )
{
}

cSineTdGen::~cSineTdGen()
{
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------

simtime_t
cSineTdGen::GetTD( simtime_t RealTime )
{
    return simtime_t( A * sin(RealTime.dbl() * f + phi) );
}
