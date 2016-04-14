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

#include "PTP_PortIdentity.h"
#include "ByteOrder.h"

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
// Constructors
// ------------------------------------------------------
cPortIdentity::cPortIdentity()
{
    this->clockIdentity = "";
    this->portNumber    = 0;
}

cPortIdentity::cPortIdentity( const char *ClockStr, UInteger16 portNumber )
{
    this->clockIdentity = ClockStr;
    this->portNumber    = portNumber;
}

cPortIdentity::cPortIdentity( const cClockIdentity clockIdentity, const UInteger16 portNumber )
{
    this->clockIdentity = clockIdentity;
    this->portNumber    = portNumber;
}

cPortIdentity::cPortIdentity( const BufPtpPortId_t *pBuf )
{
    ReadFromBuffer( pBuf );
}

cPortIdentity::cPortIdentity( const cPortIdentity& other )
{
    this->clockIdentity = other.clockIdentity;
    this->portNumber    = other.portNumber;
}

cPortIdentity::~cPortIdentity()
{
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------
cClockIdentity&
cPortIdentity::ClockIdentity()
{
    return  this->clockIdentity;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cPortIdentity::SetPortNumber( UInteger16 portNumber )
{
    this->portNumber = portNumber;
}

void
cPortIdentity::ReadFromBuffer( const BufPtpPortId_t *pBuf )
{
    this->clockIdentity.ReadFromBuffer( &pBuf->clockIdentity );
    this->portNumber = NetToHostUI16( pBuf->portNumber );
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
UInteger16
cPortIdentity::GetPortNumber() const
{
    return  this->portNumber;
}

std::string
cPortIdentity::GetString() const
{
    std::stringstream   ss;

    ss << clockIdentity << ", Port " << portNumber; // no endl!

    return  ss.str();
}

Octet
cPortIdentity::GetByteAt( size_t idx )
{
    Octet   ret = 0;

    if( idx <= 7 )
    {
        ret = this->clockIdentity.GetByteAt( idx );
    }
    else if( idx == 8 )
    {
        ret = (Octet) (this->portNumber >> 8);
    }
    else if( idx == 9 )
    {
        ret = (Octet) this->portNumber;
    }

    return  ret;
}

void
cPortIdentity::SaveToBuffer( BufPtpPortId_t *pBuf )
{
    clockIdentity.SaveToBuffer( &pBuf->clockIdentity );

    pBuf->portNumber    = HostToNetUI16( portNumber );
}

// ------------------------------------------------------
// Compare operators
// ------------------------------------------------------
bool
cPortIdentity::operator== (const cPortIdentity& other) const
{
    if
    (
        ( this->clockIdentity   == other.clockIdentity  ) &&
        ( this->portNumber      == other.portNumber     )
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
cPortIdentity::operator!= (const cPortIdentity& other) const
{
    return !(this->operator ==(other));
}

bool
operator<(cPortIdentity const& lhs, cPortIdentity const& rhs)
{
    if( lhs.clockIdentity < rhs.clockIdentity)
        return true;

    if( lhs.clockIdentity == rhs.clockIdentity )
        return lhs.portNumber < rhs.portNumber;

    return false;
}

bool
operator>(cPortIdentity const& lhs, cPortIdentity const& rhs)
{
    if( lhs.clockIdentity > rhs.clockIdentity)
        return true;

    if( lhs.clockIdentity == rhs.clockIdentity )
        return lhs.portNumber > rhs.portNumber;

    return false;
}

// ------------------------------------------------------
// Assign operator
// ------------------------------------------------------
cPortIdentity&
cPortIdentity::operator= (const cPortIdentity& other)
{
    this->clockIdentity = other.clockIdentity;
    this->portNumber    = other.portNumber;

    // By convention, always return *this
    return *this;
}

cPortIdentity&
cPortIdentity::operator= (const cClockIdentity& Clock)
{
    this->clockIdentity = Clock;
    this->portNumber    = 0;

    // By convention, always return *this
    return *this;
}

// ------------------------------------------------------
// Stream operators
// ------------------------------------------------------
std::ostream&
operator<<(std::ostream& os, const cPortIdentity& o)
{
    os << o.GetString(); // no endl!

    return os;
}

// ------------------------------------------------------
// Debug functions
// ------------------------------------------------------
void
cPortIdentity::Print()
{
    EV << "PortIdentity: " << GetString() << endl;
}
