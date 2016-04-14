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

#ifndef LIBPTP_PTP_TLV_H_
#define LIBPTP_PTP_TLV_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_PrimitiveDataTypes.h"

#include <vector>

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

// Type Length Value, see clause 5.3.8 in IEEE 1588-2008
class cTLV
{
    private:
        struct TLV_t
        {
            Enumeration16           tlvType;                        //
            UInteger16              lengthField;                    // Length of lengthField
            Octet                   *pValueField;                   // Size defined by lengthField
        };

        TLV_Type_t          tlvType;
        std::vector<Octet>  Value;

    protected:

    public:

        // Constructors
        cTLV();
        cTLV(   TLV_Type_t      tlvType,
                UInteger16      lengthField,
                Octet           *pValueField);

        // Setters/Getters
        // TODO

        // Operators
        bool    operator== (const cTLV& other);
        cTLV&   operator= (const cTLV& other);
};

#endif
