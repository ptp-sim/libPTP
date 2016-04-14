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

#include "PTP_TransparentClockDefaultDS.h"

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
cTransparentClockDefaultDS::cTransparentClockDefaultDS()
{
    this->clockIdentity     = cClockIdentity();
    this->numberPorts       = 0;
    this->delayMechanism    = DELAY_MECH_DISABLED;
    this->primaryDomain     = DOMAIN_DEFAULT;
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------
cClockIdentity& cTransparentClockDefaultDS::ClockIdentity()
{
    return  this->clockIdentity;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cTransparentClockDefaultDS::SetNumberPorts( size_t numberPorts )
{
    this->numberPorts       = numberPorts;
}

void
cTransparentClockDefaultDS::SetDelayMechanism( delayMechanism_t delayMechanism )
{
    this->delayMechanism    = delayMechanism;
}

void
cTransparentClockDefaultDS::SetPrimaryDomain( domainNumber_t primaryDomain )
{
    this->primaryDomain     = primaryDomain;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
size_t
cTransparentClockDefaultDS::GetNumberPorts()
{
    return  this->numberPorts;
}

delayMechanism_t
cTransparentClockDefaultDS::GetDelayMechanism()
{
    return  this->delayMechanism;
}

domainNumber_t
cTransparentClockDefaultDS::GetPrimaryDomain()
{
    return  this->primaryDomain;
}

// ------------------------------------------------------
// Compare operator
// ------------------------------------------------------
bool
cTransparentClockDefaultDS::operator== (const cTransparentClockDefaultDS& other)
{
    if
    (
        ( this->clockIdentity   == other.clockIdentity  ) &&
        ( this->numberPorts     == other.numberPorts    ) &&
        ( this->delayMechanism  == other.delayMechanism ) &&
        ( this->primaryDomain   == other.primaryDomain  )
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
cTransparentClockDefaultDS&
cTransparentClockDefaultDS::operator= (const cTransparentClockDefaultDS& other)
{
    this->clockIdentity     = other.clockIdentity;
    this->numberPorts       = other.numberPorts;
    this->delayMechanism    = other.delayMechanism;
    this->primaryDomain     = other.primaryDomain;

    // By convention, always return *this
    return *this;
}
