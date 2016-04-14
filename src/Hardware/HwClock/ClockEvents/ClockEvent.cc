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

#include "ClockEvent.h"

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
// Constructor
// ------------------------------------------------------
cClockEvent::cClockEvent()
{
    this->ID1           = 0;
    this->ID2           = 0;
    this->ID3           = 0;
    this->ID4           = 0;
    this->ContextPtr    = nullptr;
}

cClockEvent::cClockEvent( const int ID1, const int ID2, const int ID3, const int ID4, void *ContextPtr )
{
    this->ID1           = ID1;
    this->ID2           = ID2;
    this->ID3           = ID3;
    this->ID4           = ID4;
    this->ContextPtr    = ContextPtr;
}

cClockEvent::cClockEvent( const cClockEvent &other )
{
    this->ID1           = other.ID1;
    this->ID2           = other.ID2;
    this->ID3           = other.ID3;
    this->ID4           = other.ID4;
    this->ContextPtr    = other.ContextPtr;
}

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------
cClockEvent::~cClockEvent()
{
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
const int
cClockEvent::GetID1()
{
    return this->ID1;
}

const int
cClockEvent::GetID2()
{
    return this->ID2;
}

const int
cClockEvent::GetID3()
{
    return this->ID3;
}

const int
cClockEvent::GetID4()
{
    return this->ID4;
}

void *
cClockEvent::GetContextPtr()
{
    return this->ContextPtr;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cClockEvent::SetID1( int ID )
{
    this->ID1 = ID;
}

void
cClockEvent::SetID2( int ID )
{
    this->ID2 = ID;
}

void
cClockEvent::SetID3( int ID )
{
    this->ID3 = ID;
}

void
cClockEvent::SetID4( int ID )
{
    this->ID4 = ID;
}

void
cClockEvent::SetContextPtr( void *ContextPtr )
{
    this->ContextPtr = ContextPtr;
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------

bool
cClockEvent::operator== (const cClockEvent& other) const
{
    if
    (
        ( other.ID1         == this->ID1        ) &&
        ( other.ID2         == this->ID2        ) &&
        ( other.ID3         == this->ID3        ) &&
        ( other.ID4         == this->ID4        ) &&
        ( other.ContextPtr  == this->ContextPtr )
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

cClockEvent&
cClockEvent::operator=( const cClockEvent& other )
{
    this->ID1           = other.ID1;
    this->ID2           = other.ID2;
    this->ID3           = other.ID3;
    this->ID4           = other.ID4;
    this->ContextPtr    = other.ContextPtr;

    // By convention, always return *this
    return *this;
}
