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

#include "TdGen_ParameterParser.h"
#include "ParameterParser.h"

// ======================================================
// Definitions
// ======================================================

// ======================================================
// Variables
// ======================================================

ParseType<LibPLN_Example_t>     LibPLN_ExampleParse[]    =
{
    { LIBPLN_EXAMPLE_AVG_OSC,       "AVG_OSC"      },
    { LIBPLN_EXAMPLE_WATCH_QUARTZ,  "WATCH_QUARTZ" },
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
LibPLN_Example_t
cTdGen_ParameterParser::ParseLibPLN_Example(const char *Str)
{
    return Parse<LibPLN_Example_t>( LibPLN_ExampleParse, ArrayLen(LibPLN_ExampleParse), Str );
}
