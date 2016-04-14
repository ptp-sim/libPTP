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

#include "PTP_CustomProfileChecker.h"
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

// ------------------------------------------------------------
// Init API
// ------------------------------------------------------------
void
PTP_CustomProfileChecker::ParseParameters()
{
    PTP_ProfileChecker::ParseParameters();
}

// ------------------------------------------------------------
// Constructors/Destructor
// ------------------------------------------------------------
PTP_CustomProfileChecker::PTP_CustomProfileChecker()
    : PTP_ProfileChecker()
{
    PTP_Profile = PTP_PROFILE_CUSTOM;
}

PTP_CustomProfileChecker::~PTP_CustomProfileChecker()
{
}

// ------------------------------------------------------------
// Setters
// ------------------------------------------------------------

// ------------------------------------------------------------
// Getters
// ------------------------------------------------------------

// ------------------------------------------------------------
// PTP Profile handling
// ------------------------------------------------------------
void
PTP_CustomProfileChecker::CheckParametersFixed( PTP_Stack *pApp )
{
    // Nothing to do here
}

void
PTP_CustomProfileChecker::CheckParametersInit( PTP_Stack *pApp )
{
    // Nothing to do here
}

void
PTP_CustomProfileChecker::CheckParametersRange( PTP_Stack *pApp )
{
    // Nothing to do here
}
