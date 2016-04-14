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

#include "HwClock_ParameterParser.h"
#include "ParameterParser.h"

// ======================================================
// Definitions
// ======================================================

// ======================================================
// Variables
// ======================================================

ParseType<TdGenType_t>         TdGenTypeParse[]    =
{
    { TDGEN_TYPE_PERFECT,       "TDGEN_TYPE_PERFECT"     },
    { TDGEN_TYPE_CONST_DRIFT,   "TDGEN_TYPE_CONST_DRIFT" },
    { TDGEN_TYPE_SINE,          "TDGEN_TYPE_SINE"        },
    { TDGEN_TYPE_LIBPLN,        "TDGEN_TYPE_LIBPLN"      },
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
TdGenType_t
cHwClock_ParameterParser::ParseTdGenType(const char *Str)
{
    return Parse<TdGenType_t>( TdGenTypeParse, ArrayLen(TdGenTypeParse), Str );
}
