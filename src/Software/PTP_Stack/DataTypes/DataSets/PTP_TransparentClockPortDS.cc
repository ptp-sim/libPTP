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

#include "PTP_TransparentClockPortDS.h"

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
cTransparentClockPortDS::cTransparentClockPortDS()
{
    this->portIdentity              = cPortIdentity();
    this->logMinPdelayReqInterval   = 0;
    this->faultyFlag                = false;
    this->peerMeanPathDelay         = cTimeInterval();
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------
cPortIdentity&
cTransparentClockPortDS::PortIdentity()
{
    return  this->portIdentity;
}

cTimeInterval&
cTransparentClockPortDS::PeerMeanPathDelay()
{
    return  this->peerMeanPathDelay;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cTransparentClockPortDS::SetLogMinPdelayReqInterval( int8_t logMinPdelayReqInterval )
{
    this->logMinPdelayReqInterval   = logMinPdelayReqInterval;
}

void
cTransparentClockPortDS::SetFaultyFlag( bool faultyFlag )
{
    this->faultyFlag    = faultyFlag;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
int8_t
cTransparentClockPortDS::GetLogMinPdelayReqInterval()
{
    return  this->logMinPdelayReqInterval;
}

bool
cTransparentClockPortDS::GetFaultyFlag()
{
    return  this->faultyFlag;
}

// ------------------------------------------------------
// Compare operator
// ------------------------------------------------------
bool
cTransparentClockPortDS::operator== (const cTransparentClockPortDS& other)
{
    if
    (
        ( this->portIdentity            == other.portIdentity           ) &&
        ( this->logMinPdelayReqInterval == other.logMinPdelayReqInterval) &&
        ( this->faultyFlag              == other.faultyFlag             ) &&
        ( this->peerMeanPathDelay       == other.peerMeanPathDelay      )
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
// Assign operator
// ------------------------------------------------------
cTransparentClockPortDS&
cTransparentClockPortDS::operator= (const cTransparentClockPortDS& other)
{
    this->portIdentity              = other.portIdentity;
    this->logMinPdelayReqInterval   = other.logMinPdelayReqInterval;
    this->faultyFlag                = other.faultyFlag;
    this->peerMeanPathDelay         = other.peerMeanPathDelay;

    // By convention, always return *this
    return *this;
}
