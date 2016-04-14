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

#ifndef LIBPTP_PTP_PORTADDRESS_H_
#define LIBPTP_PTP_PORTADDRESS_H_

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

// Port address, see clause 5.3.6 in IEEE 1588-2008
class cPortAddress
{
    private:
        struct PortAddress_t
        {
            Enumeration16           networkProtocol;            // Protocol of physical layer
            UInteger16              addressLength;              // Length of pAddressField
            Octet                   *pAddressField;             // Size defined by addressLength
        };

        NetworkProtocol_t       networkProtocol;                // Protocol of physical layer
        std::string             address;

    protected:

    public:

        // Constructors
        cPortAddress();
        cPortAddress(   const NetworkProtocol_t     networkProtocol,
                        const char *                address);

        // Setters/Getters
        // TODO: Implement

        // Operators
        bool            operator== (const cPortAddress& other);
        cPortAddress&   operator= (const cPortAddress& other);
};

#endif
