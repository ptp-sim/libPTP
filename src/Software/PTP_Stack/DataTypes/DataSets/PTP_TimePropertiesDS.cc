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

#include <omnetpp.h>

#include "PTP_TimePropertiesDS.h"
#include "PTP_Constants.h"

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
cTimePropertiesDS::cTimePropertiesDS()
    : cSubmoduleInitBase()
{
    this->currentUtcOffset      = PTP_UTC_OFFSET_2006;
    this->currentUtcOffsetValid = false;
    this->leap59                = false;
    this->leap61                = false;
    this->timeTraceable         = false;
    this->frequencyTraceable    = false;
    this->ptpTimescale          = false;
    this->timeSource            = TIME_SRC_INTERNAL_OSCILLATOR;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cTimePropertiesDS::SetCurrentUtcOffset( Integer16 currentUtcOffset )
{
    this->currentUtcOffset  = currentUtcOffset;
}

void
cTimePropertiesDS::SetCurrentUtcOffsetValid( bool currentUtcOffsetValid )
{
    this->currentUtcOffsetValid = currentUtcOffsetValid;
}

void
cTimePropertiesDS::SetLeap59( bool leap59 )
{
    this->leap59    = leap59;
}

void
cTimePropertiesDS::SetLeap61( bool leap61 )
{
    this->leap61    = leap61;
}

void
cTimePropertiesDS::SetTimeTraceable( bool timeTraceable )
{
    this->timeTraceable = timeTraceable;
}

void
cTimePropertiesDS::SetFrequencyTraceable( bool frequencyTraceable )
{
    this->frequencyTraceable    = frequencyTraceable;
}

void
cTimePropertiesDS::SetPtpTimescale( bool ptpTimescale )
{
    this->ptpTimescale  =   ptpTimescale;
}

void
cTimePropertiesDS::SetTimeSource( timeSource_t timeSource )
{
    this->timeSource    = timeSource;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
Integer16
cTimePropertiesDS::GetCurrentUtcOffset()
{
    return  this->currentUtcOffset;
}

bool
cTimePropertiesDS::GetCurrentUtcOffsetValid()
{
    return  this->currentUtcOffsetValid;
}

bool
cTimePropertiesDS::GetLeap59()
{
    return  this->leap59;
}

bool
cTimePropertiesDS::GetLeap61()
{
    return  this->leap61;
}

bool
cTimePropertiesDS::GetTimeTraceable()
{
    return  this->timeTraceable;
}

bool
cTimePropertiesDS::GetFrequencyTraceable()
{
    return  this->frequencyTraceable;
}

bool
cTimePropertiesDS::GetPtpTimescale()
{
    return  this->ptpTimescale;
}

timeSource_t
cTimePropertiesDS::GetTimeSource()
{
    return  this->timeSource;
}

// ------------------------------------------------------
// Compare operator
// ------------------------------------------------------
bool
cTimePropertiesDS::operator== (const cTimePropertiesDS& other)
{
    if
    (
        ( other.currentUtcOffset        == this->currentUtcOffset       ) &&
        ( other.currentUtcOffsetValid   == this->currentUtcOffsetValid  ) &&
        ( other.leap59                  == this->leap59                 ) &&
        ( other.leap61                  == this->leap61                 ) &&
        ( other.timeTraceable           == this->timeTraceable          ) &&
        ( other.frequencyTraceable      == this->frequencyTraceable     ) &&
        ( other.ptpTimescale            == this->ptpTimescale           ) &&
        ( other.timeSource              == this->timeSource             )
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
cTimePropertiesDS&
cTimePropertiesDS::operator= (const cTimePropertiesDS& other)
{
    this->currentUtcOffset      = other.currentUtcOffset;
    this->currentUtcOffsetValid = other.currentUtcOffsetValid;
    this->leap59                = other.leap59;
    this->leap61                = other.leap61;
    this->timeTraceable         = other.timeTraceable;
    this->frequencyTraceable    = other.frequencyTraceable;
    this->ptpTimescale          = other.ptpTimescale;
    this->timeSource            = other.timeSource;

    // By convention, always return *this
    return *this;
}

// ------------------------------------------------------
// Debug functions
// ------------------------------------------------------
void
cTimePropertiesDS::Print()
{
    EV << "===============================================================" << endl;
    EV << "      Timeproperties Data Set"                                   << endl;
    EV << "===============================================================" << endl << endl;

    EV << "Current UtcOffset:\t"        << this->currentUtcOffset           << endl;
    EV << "Current UtcOffset Valid:\t"  << this->currentUtcOffsetValid      << endl;
    EV << "Leap59:\t"                   << this->leap59                     << endl;
    EV << "Leap59:\t"                   << this->leap61                     << endl;
    EV << "Time Traceable:\t"           << this->timeTraceable              << endl;
    EV << "Frequency Traceable:\t"      << this->frequencyTraceable         << endl;
    EV << "PtpTimescale:\t"             << this->ptpTimescale               << endl;
    EV << "TimeSource:\t"               << this->timeSource                 << endl << endl;
}
