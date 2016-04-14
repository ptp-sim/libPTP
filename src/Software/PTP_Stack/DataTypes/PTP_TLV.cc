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

#include "PTP_TLV.h"

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
cTLV::cTLV()
{
    this->tlvType   = TLV_INVALID;
}

cTLV::cTLV
(
    TLV_Type_t      tlvType,
    UInteger16      lengthField,
    Octet           *pValueField
)
{
    this->tlvType   = tlvType;

    Value.assign( pValueField, pValueField + lengthField);
}

// ------------------------------------------------------
// Setters/Getters
// ------------------------------------------------------
// TODO

// ------------------------------------------------------
// Compare operator
// ------------------------------------------------------
bool
cTLV::operator== (const cTLV& other)
{
    if
    (
        ( other.tlvType   == this->tlvType  ) &&
        ( other.Value     == this->Value    )
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
cTLV&
cTLV::operator= (const cTLV& other)
{
    this->tlvType   = other.tlvType;
    this->Value     = other.Value;

    // By convention, always return *this
    return *this;
}
