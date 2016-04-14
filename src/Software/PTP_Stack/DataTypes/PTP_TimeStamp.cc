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

#include "PTP_TimeStamp.h"

#include "ByteOrder.h"
#include "PhysicalConstants.h"

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
cTimeStamp::cTimeStamp()
{
    secondsField        = 0;
    nanosecondsField    = 0;
}

cTimeStamp::cTimeStamp( UInteger48  secondsField,
                        UInteger32  nanosecondsField )
:   secondsField(secondsField),
    nanosecondsField(nanosecondsField)
{
}

cTimeStamp::cTimeStamp( simtime_t timestamp )
{
    this->operator=(timestamp);
}

cTimeStamp::cTimeStamp( BufPtpTimeStamp_t *pBuf )
{
    ReadFromBuffer( pBuf );
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cTimeStamp::Set(    UInteger48  secondsField,
                    UInteger32  nanosecondsField )
{
    this->secondsField      = secondsField;
    this->nanosecondsField  = nanosecondsField;
}

void
cTimeStamp::ReadFromBuffer( BufPtpTimeStamp_t *pBuf )
{
    secondsField        = BufToHostUI48( pBuf->seconds, sizeof(pBuf->seconds) );
    nanosecondsField    = NetToHostUI32( pBuf->nanoseconds );
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------

simtime_t
cTimeStamp::GetSimTime()
{
    int64_t     NanoSecs    = secondsField * NANOSECS_PER_S + nanosecondsField;

    return simtime_t(NanoSecs, SIMTIME_NS);
}

UInteger48
cTimeStamp::GetSeconds() const
{
    return secondsField;
}

UInteger32
cTimeStamp::GetNanoSeconds() const
{
    return nanosecondsField;
}

std::string
cTimeStamp::GetString() const
{
    std::stringstream   ss;

    ss << secondsField * NANOSECS_PER_S + nanosecondsField << " [ns]";

    return  ss.str();
}

void
cTimeStamp::SaveToBuffer( BufPtpTimeStamp_t *pBuf )
{
    HostToBufUI48( secondsField, pBuf->seconds, sizeof(pBuf->seconds) );
    pBuf->nanoseconds = HostToNetUI32( nanosecondsField );
}

// ------------------------------------------------------
// Compare operator (TimeStamp)
// ------------------------------------------------------
bool
cTimeStamp::operator== (const cTimeStamp& other)
{
    if
    (
        ( other.secondsField        == this->secondsField       ) &&
        ( other.nanosecondsField    == this->nanosecondsField   )
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ------------------------------------------------------
// Compare operator (simtime_t)
// ------------------------------------------------------
bool
cTimeStamp::operator== (const simtime_t& other)
{
    return other == GetSimTime();
}

// ------------------------------------------------------
// Assignment operator (TimeStamp)
// ------------------------------------------------------
cTimeStamp &
cTimeStamp::operator= (const cTimeStamp& other)
{
    this->secondsField      = other.secondsField;
    this->nanosecondsField  = other.nanosecondsField;

    // By convention, always return *this
    return *this;
}

// ------------------------------------------------------
// Assignment operator (simtime_t)
// ------------------------------------------------------
cTimeStamp &
cTimeStamp::operator= (const simtime_t& other)
{
    this->secondsField      = other.inUnit( SIMTIME_S );
    this->nanosecondsField  = other.remainderForUnit( SIMTIME_S ).inUnit( SIMTIME_NS );

    // By convention, always return *this
    return *this;
}

// ------------------------------------------------------
// Assignment operator (int64_t)
// ------------------------------------------------------
cTimeStamp &
cTimeStamp::operator= (const int64_t& other_ps)
{
    int64_t tmp;
    int64_t NanoSecs;
    int64_t Secs;

    tmp         = (other_ps / 1000);
    NanoSecs    = tmp % NANOSECS_PER_S;
    Secs        = tmp / NANOSECS_PER_S;

    this->secondsField      = Secs;
    this->nanosecondsField  = NanoSecs;

    // By convention, always return *this
    return *this;
}

// ------------------------------------------------------
// Ouput operators
// ------------------------------------------------------
std::ostream&
operator<<(std::ostream& os, const cTimeStamp& o)
{
    os << o.GetString(); // no endl!

    return os;
}
