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

#ifdef HAS_LIBPLN

#include "libPLN_TdGen.h"

#include "Examples/AverageOscillator_20MHz/AverageOscillator_20MHz.hpp"
#include "Examples/WatchQuartz_20MHz/WatchQuartz_20MHz.hpp"

using namespace LibPLN_Examples;

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
cLibPLN_TdGen::ParseResourceParameters()
{
    Seed            = pParentModule->par( "libPLN_TdGen_Seed" ).longValue();
    AllowSkipping   = pParentModule->par( "libPLN_TdGen_AllowSkipping" ).boolValue();

    if( Seed == 0 )
    {
        Seed = pParentModule->getId() * 100;
        pParentModule->par( "libPLN_TdGen_Seed" ).setLongValue( Seed );
    }

    ExampleType = cTdGen_ParameterParser::ParseLibPLN_Example( pParentModule->par( "libPLN_TdGen_Example" ).stringValue() );
}

void
cLibPLN_TdGen::AllocateResources()
{
    switch( ExampleType )
    {
    case LIBPLN_EXAMPLE_AVG_OSC:        pTdGen = new cAvgOsc20MHz( Seed, AllowSkipping );
                                        break;

    case LIBPLN_EXAMPLE_WATCH_QUARTZ:   pTdGen = new cWatchQuartz_20MHz( Seed, AllowSkipping );
                                        break;
    }

}

void
cLibPLN_TdGen::ParseParameters()
{
}

// ------------------------------------------------------
// Constructors/Destructor
// ------------------------------------------------------

cLibPLN_TdGen::cLibPLN_TdGen()
    : ITdGen( "libPLN_TdGen" )
{
    Seed    = 0;
    pTdGen  = nullptr;
}

cLibPLN_TdGen::~cLibPLN_TdGen()
{
    delete pTdGen;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------

simtime_t
cLibPLN_TdGen::GetTD( simtime_t RealTime )
{
    return pTdGen->EstimateTD( simTime().dbl(), RealTime.dbl() );
}

#endif
