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

#include "PTP_ParentDS.h"
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
cParentDS::cParentDS()
    : cSubmoduleInitBase()
{
    // Data set specific variables
    this->parentPortIdentity                    = cPortIdentity();
    this->parentStats                           = false;
    this->observedParentOffsetScaledLogVariance = 0;
    this->observedParentClockPhaseChangeRate    = 0;
    this->grandmasterIdentity                   = cClockIdentity();
    this->grandmasterClockQuality               = cClockQuality();
    this->grandmasterPriority1                  = 0;
    this->grandmasterPriority2                  = 0;
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------
cPortIdentity&
cParentDS::ParentPortIdentity()
{
    return  this->parentPortIdentity;
}

cClockQuality&
cParentDS::GrandmasterClockQuality()
{
    return  this->grandmasterClockQuality;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cParentDS::SetParentStats( bool parentStats )
{
    this->parentStats   = parentStats;
}

void
cParentDS::SetObservedParentOffsetScaledLogVariance( UInteger16 observedParentOffsetScaledLogVariance )
{
    this->observedParentOffsetScaledLogVariance = observedParentOffsetScaledLogVariance;
}

void
cParentDS::SetObservedParentClockPhaseChangeRate( Integer32 observedParentClockPhaseChangeRate )
{
    this->observedParentClockPhaseChangeRate    = observedParentClockPhaseChangeRate;
}

void
cParentDS::SetGrandmasterIdentity( cClockIdentity grandmasterIdentity )
{
    this->grandmasterIdentity   = grandmasterIdentity;
}

void
cParentDS::SetGrandmasterPriority1( uint8_t grandmasterPriority1 )
{
    this->grandmasterPriority1  = grandmasterPriority1;
}

void
cParentDS::SetGrandmasterPriority2( uint8_t grandmasterPriority2 )
{
    this->grandmasterPriority2    = grandmasterPriority2;
}

void
cParentDS::SetGrandMasterModuleID( int GrandMasterModuleID )
{
    this->GrandMasterModuleID   = GrandMasterModuleID;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
bool
cParentDS::GetParentStats()
{
    return  this->parentStats;
}

UInteger16
cParentDS::GetObservedParentOffsetScaledLogVariance()
{
    return  this->observedParentOffsetScaledLogVariance;
}

Integer32
cParentDS::GetObservedParentClockPhaseChangeRate()
{
    return  this->observedParentClockPhaseChangeRate;
}

cClockIdentity
cParentDS::GetGrandmasterIdentity()
{
    return  this->grandmasterIdentity;
}

uint8_t
cParentDS::GetGrandmasterPriority1()
{
    return  this->grandmasterPriority1;
}

uint8_t
cParentDS::GetGrandmasterPriority2()
{
    return  this->grandmasterPriority2;
}

int
cParentDS::GetGrandMasterModuleID()
{
    return  this->GrandMasterModuleID;
}

// ------------------------------------------------------
// Compare operator
// ------------------------------------------------------
bool
cParentDS::operator== (const cParentDS& other)
{
    if
    (
        ( this->parentPortIdentity                      == other.parentPortIdentity                     ) &&
        ( this->parentStats                             == other.parentStats                            ) &&
        ( this->observedParentOffsetScaledLogVariance   == other.observedParentOffsetScaledLogVariance  ) &&
        ( this->observedParentClockPhaseChangeRate      == other.observedParentClockPhaseChangeRate     ) &&
        ( this->grandmasterIdentity                     == other.grandmasterIdentity                    ) &&
        ( this->grandmasterClockQuality                 == other.grandmasterClockQuality                ) &&
        ( this->grandmasterPriority1                    == other.grandmasterPriority1                   ) &&
        ( this->grandmasterPriority2                    == other.grandmasterPriority2                   )
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
cParentDS&
cParentDS::operator= (const cParentDS& other)
{
    this->parentPortIdentity                    = other.parentPortIdentity;
    this->parentStats                           = other.parentStats;
    this->observedParentOffsetScaledLogVariance = other.observedParentOffsetScaledLogVariance;
    this->observedParentClockPhaseChangeRate    = other.observedParentClockPhaseChangeRate;
    this->grandmasterIdentity                   = other.grandmasterIdentity;
    this->grandmasterClockQuality               = other.grandmasterClockQuality;
    this->grandmasterPriority1                  = other.grandmasterPriority1;
    this->grandmasterPriority2                  = other.grandmasterPriority2;

    // By convention, always return *this
    return *this;
}

// ------------------------------------------------------
// Debug functions
// ------------------------------------------------------
void
cParentDS::Print()
{
    EV << "===============================================================" << endl;
    EV << "      Parent Data Set"                                           << endl;
    EV << "===============================================================" << endl << endl;

    EV << "Parent PortIdentity:\t"                      << this->parentPortIdentity.GetString()         << endl;
    EV << "Parent Stats:\t"                             << this->parentStats                            << endl;
    EV << "Observed ParentOffsetScaledLogVariance :\t"  << this->observedParentOffsetScaledLogVariance  << endl;
    EV << "Observed ParentClockPhaseChangeRate:\t"      << this->observedParentClockPhaseChangeRate     << endl;
    EV << "Grandmaster Identity:\t"                     << this->grandmasterIdentity.GetString()        << endl;
    EV << "Grandmaster ClockQuality:\t"                 << this->grandmasterClockQuality.GetString()    << endl;
    EV << "Grandmaster Priority1:\t"                    << (int) this->grandmasterPriority1             << endl;
    EV << "Grandmaster Priority2:\t"                    << (int) this->grandmasterPriority2             << endl << endl;
}
