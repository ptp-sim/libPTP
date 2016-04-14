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

#ifndef LIBPTP_PTP_MAC_RELAY_UNIT_H_
#define LIBPTP_PTP_MAC_RELAY_UNIT_H_

// ======================================================
// Includes
// ======================================================

#include "MACRelayUnitBase.h"

#include "PTP.h"
#include "IInitBase.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================


class PTP_MACRelayUnit : public MACRelayUnitBase, public IInitBase
{
    private:

        // Configuration
        bool                PTP_Enable;
        PTP_ClockType_t     PTP_ClockType;
        MACAddress          MAC;

        // Init API
        void    ParseParameters();

    protected:

        // Configuration
        int                 internalPort;

        // Internal functions
        virtual void    handleAndDispatchFrame_Dispatch(EtherFrame *frame, int inputport);

        virtual void    broadcastFrame(EtherFrame *frame, int inputport);

    public:

        // Constructor
        PTP_MACRelayUnit();

        // OMNeT API
        int     numInitStages() const;
        void    initialize(int stage);

        // Setters
        void    SetMACAddress( MACAddress MAC );
};

#endif

