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
// Constructors/Destructor
// ------------------------------------------------------
IdentitySimTimeFilter::IdentitySimTimeFilter()
    : ISimTimeFilter()
{
    reset();
}

IdentitySimTimeFilter::IdentitySimTimeFilter( const IdentitySimTimeFilter& other )
    : ISimTimeFilter( other )
{
    buf = other.buf;
}

IdentitySimTimeFilter::~IdentitySimTimeFilter()
{
}

IdentitySimTimeFilter*
IdentitySimTimeFilter::Clone() const
{
    return new IdentitySimTimeFilter(*this);
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
IdentitySimTimeFilter::reset()
{
    buf = SIMTIME_ZERO;
}

void
IdentitySimTimeFilter::push( simtime_t v )
{
    buf = v;
}

simtime_t
IdentitySimTimeFilter::pop()
{
    return buf;
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
IdentitySimTimeFilter&
IdentitySimTimeFilter::operator= (const IdentitySimTimeFilter& other)
{
    this->buf = other.buf;

    // By convention, always return *this
    return *this;
}
