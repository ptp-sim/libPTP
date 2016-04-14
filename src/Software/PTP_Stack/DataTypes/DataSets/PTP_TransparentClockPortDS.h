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

#ifndef LIBPTP_PTP_TRANSPARENT_CLOCK_PORT_DS_H_
#define LIBPTP_PTP_TRANSPARENT_CLOCK_PORT_DS_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_PrimitiveDataTypes.h"
#include "PTP_PortIdentity.h"
#include "PTP_TimeInterval.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

// Transparent Clock port data set, see clause 8.3.3 in IEEE 1588-2008
class cTransparentClockPortDS
{
    private:
        // Static members
        cPortIdentity       portIdentity;                                   // Address of this port

        // Configurable members
        int8_t              logMinPdelayReqInterval;                        // Exponent in range -128 ... 127, [s]
        bool                faultyFlag;                                     // Indicates errors on this port
        cTimeInterval       peerMeanPathDelay;                              // Result of P2P measurement

    protected:

    public:

        // Constructors
        cTransparentClockPortDS();

        // Instance methods
        cPortIdentity&      PortIdentity();
        cTimeInterval&      PeerMeanPathDelay();

        // Setters
        void    SetLogMinPdelayReqInterval( int8_t logMinPdelayReqInterval );
        void    SetFaultyFlag( bool faultyFlag );

        // Getters
        int8_t              GetLogMinPdelayReqInterval();
        bool                GetFaultyFlag();

        // Operators
        bool                        operator== (const cTransparentClockPortDS& other);
        cTransparentClockPortDS&    operator= (const cTransparentClockPortDS& other);
};

#endif
