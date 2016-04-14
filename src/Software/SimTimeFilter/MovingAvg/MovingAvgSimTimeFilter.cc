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

#include "MovingAvgSimTimeFilter.h"

#include <boost/circular_buffer.hpp>

// ======================================================
// Definitions
// ======================================================

// ======================================================
// Types
// ======================================================

class cSimTimeCircBuf : public boost::circular_buffer<simtime_t>
{
};

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
// Constructors/Destructor
// ------------------------------------------------------
MovingAvgSimTimeFilter::MovingAvgSimTimeFilter( size_t size, bool DiscardMinMax )
    : ISimTimeFilter()
{
    pBuf = new cSimTimeCircBuf;

    pBuf->set_capacity( size );

    this->DiscardMinMax = DiscardMinMax;
}

MovingAvgSimTimeFilter::MovingAvgSimTimeFilter( const MovingAvgSimTimeFilter& other )
    : ISimTimeFilter( other )
{
    pBuf = new cSimTimeCircBuf( *other.pBuf );

    this->DiscardMinMax = other.DiscardMinMax;
}

MovingAvgSimTimeFilter::~MovingAvgSimTimeFilter()
{
    delete pBuf;
}

MovingAvgSimTimeFilter*
MovingAvgSimTimeFilter::Clone() const
{
    return new MovingAvgSimTimeFilter(*this);
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------

// ------------------------------------------------------
// API functions
// ------------------------------------------------------
void
MovingAvgSimTimeFilter::reset()
{
    pBuf->clear();
}

void
MovingAvgSimTimeFilter::push( simtime_t v )
{
    pBuf->push_back( v );
}

simtime_t
MovingAvgSimTimeFilter::pop()
{
    if( pBuf->empty() )
    {
        throw cRuntimeError( "Calling pop() of an empty buffer." );
    }

    simtime_t sum;
    simtime_t avg;

    if( DiscardMinMax )
    {
        cSimTimeCircBuf::iterator itMin = std::min_element( pBuf->begin(), pBuf->end() );
        cSimTimeCircBuf::iterator itMax = std::max_element( pBuf->begin(), pBuf->end() );

        if( pBuf->size() > 3 )
        {
            sum = std::accumulate( pBuf->begin(), pBuf->end(), SIMTIME_ZERO );
            sum -= *itMin;
            sum -= *itMax;

            avg = sum / (pBuf->size()-2);
        }
        else
        {
            avg = SIMTIME_ZERO;
        }
    }
    else
    {
        sum = std::accumulate( pBuf->begin(), pBuf->end(), SIMTIME_ZERO );
        avg = sum / pBuf->size();
    }

    return avg;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------

// ------------------------------------------------------
// Getters
// ------------------------------------------------------

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
MovingAvgSimTimeFilter&
MovingAvgSimTimeFilter::operator= (const MovingAvgSimTimeFilter& other)
{
    this->pBuf = new cSimTimeCircBuf( *other.pBuf );

    this->DiscardMinMax = other.DiscardMinMax;

    // By convention, always return *this
    return *this;
}
