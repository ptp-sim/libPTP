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

#include "PTP_ProfileChecker.h"

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

// ------------------------------------------------------------
// Error handling
// ------------------------------------------------------------
void
PTP_ProfileChecker::HandleWrongInitValue( std::string ParameterName )
{
    std::stringstream ss;

    ss << "Wrong initialization value for PTP profile" << endl;
    ss << "PTP Profile: " << PTP_Profile << endl;
    ss << "Parameter: " << ParameterName << endl;

    throw cRuntimeError( ss.str().c_str() );
}

void
PTP_ProfileChecker::HandleRangeError( std::string ParameterName )
{
    std::stringstream ss;

    ss << "Parameter not in valid range for PTP profile" << endl;
    ss << "PTP Profile: " << PTP_Profile << endl;
    ss << "Parameter: " << ParameterName << endl;

    throw cRuntimeError( ss.str().c_str() );
}

// ------------------------------------------------------------
// Init API
// ------------------------------------------------------------
void
PTP_ProfileChecker::ParseParameters()
{
}

// ------------------------------------------------------------
// Constructors/Destructor
// ------------------------------------------------------------
PTP_ProfileChecker::PTP_ProfileChecker()
{
    PTP_Profile = PTP_PROFILE_CUSTOM;
}

PTP_ProfileChecker::~PTP_ProfileChecker()
{
}
