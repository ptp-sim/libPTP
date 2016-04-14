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

#include "PTP_TimeInterval.h"

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
// Constructors / Destructor
// ------------------------------------------------------
cTimeInterval::cTimeInterval()
{
    this->scaledNanoseconds   = 0;
}

cTimeInterval::cTimeInterval( simtime_t t )
{
    // This conversion looses the fractional part of nanoseconds
    this->scaledNanoseconds     = t.inUnit( SIMTIME_NS ) << 16;
}

cTimeInterval::cTimeInterval( Integer64 scaledNanoseconds )
{
    this->scaledNanoseconds     = scaledNanoseconds;
}

cTimeInterval::cTimeInterval( const cTimeInterval& other )
{
    this->scaledNanoseconds     = other.scaledNanoseconds;
}

cTimeInterval::~cTimeInterval()
{
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
simtime_t
cTimeInterval::GetSimTime()
{
    Integer64   PicoSecs;
    Integer64   NanoSecs;

    NanoSecs    =   this->scaledNanoseconds >> 16;
    PicoSecs    =   (this->scaledNanoseconds - (NanoSecs << 16));
    PicoSecs    *=  1000;
    PicoSecs    >>= 16;

    PicoSecs    += NanoSecs * 1000;

    // This conversion looses the fractional part of picoseconds
    simtime_t t( PicoSecs, SIMTIME_PS );

    return t;
}

std::string
cTimeInterval::GetString() const
{
    std::stringstream   ss;

    ss << scaledNanoseconds << " [2^-16 ns]";

    return  ss.str();
}

Integer64
cTimeInterval::GetScaledNanoseconds() const
{
    return  scaledNanoseconds;
}

// ------------------------------------------------------
// Compare operators
// ------------------------------------------------------
bool
cTimeInterval::operator== (const cTimeInterval& other)
{
    return other.scaledNanoseconds == this->scaledNanoseconds;
}

bool
cTimeInterval::operator!= (const cTimeInterval& other)
{
    return !operator==(other);
}

bool
cTimeInterval::operator== (const simtime_t& other)
{
    cTimeInterval ti( other );

    return ti.scaledNanoseconds == this->scaledNanoseconds;
}

bool
cTimeInterval::operator!= (const simtime_t& other)
{
    return !operator==(other);
}

// ------------------------------------------------------
// Assign operators
// ------------------------------------------------------
cTimeInterval&
cTimeInterval::operator= (const cTimeInterval& other)
{
    this->scaledNanoseconds = other.scaledNanoseconds;

    // By convention, always return *this
    return *this;
}

cTimeInterval&
cTimeInterval::operator= (const Integer64 ScaledNanoSeconds)
{
    this->scaledNanoseconds = ScaledNanoSeconds;

    // By convention, always return *this
    return *this;
}

cTimeInterval&
cTimeInterval::operator= (const simtime_t& other)
{
    Integer64   PicoSecs;

    this->scaledNanoseconds     = other.inUnit( SIMTIME_NS ) << 16;     // Nanoseconds

    PicoSecs  = other.remainderForUnit( SIMTIME_NS ).inUnit( SIMTIME_PS );
    PicoSecs    <<= 16;
    PicoSecs    /=  1000;

    this->scaledNanoseconds += PicoSecs;

    // By convention, always return *this
    return *this;
}

// ------------------------------------------------------
// Numerical operators
// ------------------------------------------------------

const cTimeInterval
cTimeInterval::operator+(const cTimeInterval &other) const
{
    cTimeInterval tmp( *this );

    return (tmp += other);
}

const cTimeInterval
cTimeInterval::operator-(const cTimeInterval &other) const
{
    cTimeInterval tmp( *this );

    return (tmp -= other);
}

const cTimeInterval
cTimeInterval::operator*(const double d) const
{
    cTimeInterval tmp( *this );

    return (tmp *= d);
}

const cTimeInterval
cTimeInterval::operator/(const double d) const
{
    cTimeInterval tmp( *this );

    return (tmp /= d);
}

cTimeInterval &
cTimeInterval::operator+=(const cTimeInterval &other)
{
    this->scaledNanoseconds += other.scaledNanoseconds;

    // By convention, always return *this
    return *this;
}

cTimeInterval &
cTimeInterval::operator-=(const cTimeInterval &other)
{
    this->scaledNanoseconds -= other.scaledNanoseconds;

    // By convention, always return *this
    return *this;
}

cTimeInterval &
cTimeInterval::operator*=(const double d)
{
    this->scaledNanoseconds *= d;

    // By convention, always return *this
    return *this;
}

cTimeInterval &
cTimeInterval::operator/=(const double d)
{
    this->scaledNanoseconds /= d;

    // By convention, always return *this
    return *this;
}

const cTimeInterval
cTimeInterval::operator-() const
{
    return cTimeInterval( -this->scaledNanoseconds );
}

// ------------------------------------------------------
// Stream operators
// ------------------------------------------------------
std::ostream&
operator<<(std::ostream& os, const cTimeInterval& o)
{
    os << o.GetString(); // no endl!

    return os;
}
