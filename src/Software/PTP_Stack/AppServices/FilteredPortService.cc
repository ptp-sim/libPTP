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

#include "FilteredPortService.h"

#include "MovingAvgSimTimeFilter.h"
#include "IdentitySimTimeFilter.h"

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

cFilteredPortService::cFilteredPortService()
    : cPortService()
{
    pFilter = nullptr;
}

cFilteredPortService::cFilteredPortService( const cFilteredPortService& other )
    : cPortService( other )
{
    // Configuration
    FilterType          = other.FilterType;
    FilterLen           = other.FilterLen;
    FilterDiscardMinMax = other.FilterDiscardMinMax;

    // Resources
    pFilter = other.pFilter->Clone();
}

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------
cFilteredPortService::~cFilteredPortService()
{
    delete pFilter;
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------

void
cFilteredPortService::AllocateResources()
{
    cPortService::AllocateResources();

    switch( FilterType )
    {
        default:
        {
            throw cRuntimeError( "Invalid filter configured" );
            break;
        }

        case MOVING_AVG_FILTER:
        {
            if( FilterLen == 0 )
            {
                throw cRuntimeError( "Invalid filter length configured" );
            }

            pFilter = new MovingAvgSimTimeFilter( FilterLen, FilterDiscardMinMax );
            break;
        }

        case NO_FILTER:
        {
            pFilter = new IdentitySimTimeFilter();
            break;
        }
    }
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------

// ------------------------------------------------------
// Getters
// ------------------------------------------------------

// ------------------------------------------------------
// API functions
// ------------------------------------------------------

void
cFilteredPortService::StartInterval()
{
    if( IntervalState == TimerState::STOPPED )
    {
        if( pFilter != nullptr )
        {
            pFilter->reset();
        }
    }

    cPortService::StartInterval();
}

void
cFilteredPortService::HandleTimeJump()
{
    if( pFilter != nullptr )
    {
        pFilter->reset();
    }
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
cFilteredPortService&
cFilteredPortService::operator=( const cFilteredPortService& other )
{
    cPortService::operator=( other );

    // Configuration
    FilterType          = other.FilterType;
    FilterLen           = other.FilterLen;
    FilterDiscardMinMax = other.FilterDiscardMinMax;

    // Resources
    pFilter = other.pFilter->Clone();

    // By convention, always return *this
    return *this;
}
