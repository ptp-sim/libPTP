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

#include "PTP_DefaultDS.h"
#include "PTP_Stack.h"

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
cDefaultDS::cDefaultDS()
{
    this->twoStepFlag   = true;
    this->clockIdentity = "";

    this->numberPorts   = 0;
    this->clockQuality.SetClockClass(CLOCK_CLASS_DEFAULT);
    this->clockQuality.SetClockAccuracy(CLOCK_ACCURACY_UNKNOWN);
    this->clockQuality.SetOffsetScaledLogVar(0);

    this->priority1     = 0;
    this->priority2     = 0;
    this->domainNumber  = DOMAIN_DEFAULT;
    this->slaveOnly     = false;

    this->pApp          = nullptr;
}

// ------------------------------------------------------
// Initialization
// ------------------------------------------------------
void
cDefaultDS::SetHierarchy( PTP_Stack *pApp )
{
    this->pApp  = pApp;
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------
cClockIdentity&
cDefaultDS::ClockIdentity()
{
    return  this->clockIdentity;
}

cClockQuality&
cDefaultDS::ClockQuality()
{
    return  this->clockQuality;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cDefaultDS::SetTwoStepFlag( bool twoStepFlag )
{
    this->twoStepFlag   = twoStepFlag;
}

void
cDefaultDS::SetNumberPorts( size_t numberPorts )
{
    this->numberPorts   = numberPorts;
}

void
cDefaultDS::SetPriority1( uint8_t priority1 )
{
    this->priority1 = priority1;
}

void
cDefaultDS::SetPriority2( uint8_t priority2)
{
    this->priority2 = priority2;
}

void
cDefaultDS::SetDomainNumber( domainNumber_t domainNumber)
{
    this->domainNumber  = domainNumber;
}

void
cDefaultDS::SetSlaveOnly( bool slaveOnly)
{
    this->slaveOnly = slaveOnly;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
bool
cDefaultDS::GetTwoStepFlag() const
{
    return  this->twoStepFlag;
}

size_t
cDefaultDS::GetNumberPorts() const
{
    return  this->numberPorts;
}

uint8_t
cDefaultDS::GetPriority1() const
{
    return  this->priority1;
}

uint8_t
cDefaultDS::GetPriority2() const
{
    return  this->priority2;
}

domainNumber_t
cDefaultDS::GetDomainNumber() const
{
    return  this->domainNumber;
}

bool
cDefaultDS::GetSlaveOnly() const
{
    return  this->slaveOnly;
}

cClockIdentity
cDefaultDS::GetClockIdentity()  const
{
    return  this->clockIdentity;
}

cClockQuality
cDefaultDS::GetClockQuality()   const
{
    return  this->clockQuality;
}

int
cDefaultDS::GetModuleID() const
{
    return  pApp->GetModuleID();
}

// ------------------------------------------------------
// Debug functions
// ------------------------------------------------------
void
cDefaultDS::Print()
{
    EV << "===============================================================" << endl;
    EV << "      Default Data Set"                                          << endl;
    EV << "===============================================================" << endl << endl;

    EV << "TwoStepFlag:\t"      << this->twoStepFlag                << endl;
    EV << "ClockIdentity:\t"    << this->clockIdentity.GetString()  << endl;
    EV << "Number of Ports:\t"  << this->numberPorts                << endl;
    EV << "ClockQuality:\t"     << this->clockQuality.GetString()   << endl;

    EV << "Priority1:\t"        << (int) this->priority1            << endl;
    EV << "Priority2:\t"        << (int) this->priority2            << endl;
    EV << "DomainNumber:\t"     << this->domainNumber               << endl;
    EV << "SlaveOnly:\t"        << this->slaveOnly                  << endl << endl;
}
