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

#include "PTP_PTPText.h"

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
cPTPText::cPTPText()
{
    s.assign("");
}

cPTPText::cPTPText( const char *Str )
{
    s.max_size();
    s.assign(Str);
}

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------
cPTPText::~cPTPText()
{
}

// ------------------------------------------------------
// Setters/Getters
// ------------------------------------------------------
UInteger8 cPTPText::GetLengthField()
{
    if( s.length() > 255 )
        return (UInteger8) 255;
    else
        return (UInteger8) s.length();
}

// ------------------------------------------------------
// Compare operator
// ------------------------------------------------------
bool cPTPText::operator== (const cPTPText& other)
{
    return  (other.s == this->s);
}

// ------------------------------------------------------
// Assign operator
// ------------------------------------------------------
cPTPText& cPTPText::operator= (const cPTPText& other)
{
    this->s = other.s;

    // By convention, always return *this
    return *this;
}

// ------------------------------------------------------
// Assign operator (basic string)
// ------------------------------------------------------
cPTPText& cPTPText::operator= (const char *Str)
{
    this->s.assign( Str );

    // By convention, always return *this
    return *this;
}
