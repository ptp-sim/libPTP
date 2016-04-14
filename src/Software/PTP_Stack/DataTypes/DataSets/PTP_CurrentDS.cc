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

#include "PTP_CurrentDS.h"

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
cCurrentDS::cCurrentDS()
    : cSubmoduleInitBase()
{
    // Data set specific variables
    this->stepsRemoved      = 0;
    this->meanPathDelay     = 0;
    this->offsetFromMaster  = 0;
}

// ------------------------------------------------------
// Initialization
// ------------------------------------------------------
void
cCurrentDS::RegisterSignals()
{
    offsetFromMaster_SigId  = pParentModule->registerSignal("offsetFromMaster");
    stepsRemoved_SigId      = pParentModule->registerSignal("stepsRemoved");
    meanPathDelay_SigId     = pParentModule->registerSignal("meanPathDelay");
}

void
cCurrentDS::InitSignals()
{
    pParentModule->emit( stepsRemoved_SigId,      this->stepsRemoved );
    pParentModule->emit( offsetFromMaster_SigId,  this->offsetFromMaster.GetSimTime() );
    pParentModule->emit( meanPathDelay_SigId,     this->meanPathDelay.GetSimTime() );
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cCurrentDS::SetStepsRemoved( size_t stepsRemoved )
{
    this->stepsRemoved = stepsRemoved;

    pParentModule->emit( stepsRemoved_SigId, this->stepsRemoved );
}

void
cCurrentDS::SetOffsetFromMaster( cTimeInterval offsetFromMaster )
{
    this->offsetFromMaster = offsetFromMaster;

    pParentModule->emit( offsetFromMaster_SigId, this->offsetFromMaster.GetSimTime() );
}

void
cCurrentDS::SetMeanPathDelay( cTimeInterval meanPathDelay )
{
    if( this->meanPathDelay != meanPathDelay )
    {
        pParentModule->emit( meanPathDelay_SigId, meanPathDelay.GetSimTime() );
    }

    this->meanPathDelay = meanPathDelay;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
size_t
cCurrentDS::GetStepsRemoved()
{
    return  this->stepsRemoved;
}

cTimeInterval
cCurrentDS::GetOffsetFromMaster()
{
    return  this->offsetFromMaster;
}

cTimeInterval cCurrentDS::GetMeanPathDelay()
{
    return  this->meanPathDelay;
}

// ------------------------------------------------------
// Compare operator
// ------------------------------------------------------
bool
cCurrentDS::operator== (const cCurrentDS& other)
{
    if
    (
        ( this->stepsRemoved        == other.stepsRemoved       ) &&
        ( this->offsetFromMaster    == other.offsetFromMaster   ) &&
        ( this->meanPathDelay       == other.meanPathDelay      )
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
cCurrentDS&
cCurrentDS::operator= (const cCurrentDS& other)
{
    this->stepsRemoved     = other.stepsRemoved;
    this->offsetFromMaster = other.offsetFromMaster;
    this->meanPathDelay    = other.meanPathDelay;

    // By convention, always return *this
    return *this;
}

void
cCurrentDS::Print()
{
    EV << "===============================================================" << endl;
    EV << "      Current Data Set"                                          << endl;
    EV << "===============================================================" << endl << endl;

    EV << "StepsRemoved:\t"         << this->stepsRemoved                   << endl;
    EV << "OffsetFromMaster:\t"     << this->offsetFromMaster.GetString()   << endl;
    EV << "MeanPathDelay:\t"        << this->meanPathDelay.GetString()      << endl << endl;
}
