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

#include "PI_ClockServo_ParameterParser.h"
#include "ParameterParser.h"

// ======================================================
// Definitions
// ======================================================

// ======================================================
// Variables
// ======================================================

static
ParseType<cPI_ClockServo::ParameterMode_t> ParameterModeParse[] =
{
    { cPI_ClockServo::ParameterMode_t::PI_PARAMETER_MODE_CONSTANT,      "CONSTANT"  },
    { cPI_ClockServo::ParameterMode_t::PI_PARAMETER_MODE_LINUX_PTP,     "LINUX_PTP" },
    { cPI_ClockServo::ParameterMode_t::PI_PARAMETER_MODE_LIBPTP,        "LIBPTP"    },
};

// ======================================================
// Declarations
// ======================================================

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// TdGen type
// ------------------------------------------------------

cPI_ClockServo::ParameterMode_t
cPI_ClockServo_ParameterParser::ParseParameterMode(const char *Str)
{
    return Parse<cPI_ClockServo::ParameterMode_t>( ParameterModeParse, ArrayLen(ParameterModeParse), Str );
}
