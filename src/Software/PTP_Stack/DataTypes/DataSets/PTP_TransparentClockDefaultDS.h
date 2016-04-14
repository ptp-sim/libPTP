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

#ifndef LIBPTP_PTP_TRANSPARENT_CLOCK_DEFAULT_DS_H_
#define LIBPTP_PTP_TRANSPARENT_CLOCK_DEFAULT_DS_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "PTP_PrimitiveDataTypes.h"
#include "PTP_ClockIdentity.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

// Transparent Clock default data set, see clause 8.3.2 in IEEE 1588-2008
class cTransparentClockDefaultDS
{
    private:
        // Static members
        cClockIdentity          clockIdentity;                                  // ID of this clock
        size_t                  numberPorts;                                    // Number of ports

        // Configurable members
        delayMechanism_t        delayMechanism;                                 // P2P or E2E
        domainNumber_t          primaryDomain;                                  // Primary clock domain

    protected:

    public:

        // Constructors
        cTransparentClockDefaultDS();

        // Instance methods
        cClockIdentity&         ClockIdentity();

        // Setters
        void    SetNumberPorts      ( size_t            numberPorts   );
        void    SetDelayMechanism   ( delayMechanism_t  delayMechanism);
        void    SetPrimaryDomain    ( domainNumber_t    primaryDomain );

        // Getters
        size_t                  GetNumberPorts();
        delayMechanism_t        GetDelayMechanism();
        domainNumber_t          GetPrimaryDomain();

        // Operators
        bool                        operator== (const cTransparentClockDefaultDS& other);
        cTransparentClockDefaultDS& operator= (const cTransparentClockDefaultDS& other);
};

#endif
