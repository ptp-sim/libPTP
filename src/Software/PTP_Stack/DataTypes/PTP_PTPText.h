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

#ifndef LIBPTP_PTP_PTPTEXT_H_
#define LIBPTP_PTP_PTPTEXT_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_PrimitiveDataTypes.h"

#include <string>

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

// PTP text, see clause 5.3.9 in IEEE 1588-2008
class cPTPText
{
    private:

        // Types
        struct PTPText_t
        {
            UInteger8           lengthField;                    // Length of pTextField
            Octet               *pTextField;                    // Size defined by lengthField
        };

        // Resources
        std::string             s;

    protected:

    public:

        // Constructors
        cPTPText();
        cPTPText( const char *Str );
        ~cPTPText();

        // Setters/Getters
        UInteger8       GetLengthField();

        // Operators
        bool            operator== (const cPTPText& other);
        cPTPText        &operator= (const cPTPText& other);
        cPTPText        &operator= (const char *Str);

        // Debug functions
};


#endif
