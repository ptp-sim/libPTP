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

#ifndef LIBPTP_PTP_ETHERNET_MAPPING_H_
#define LIBPTP_PTP_ETHERNET_MAPPING_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include <MACAddress.h>

#include "ModuleInitBase.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class PTP_EthernetMapping : public cModuleInitBase
{
    private:

        // Resources
        int     LowerLayerOutGateID;
        int     LowerLayerInGateID;
        int     UpperLayerOutGateID;
        int     UpperLayerInGateID;
        int     GateSize;

        const   MACAddress  PtpMcMAC;
        const   MACAddress  PtpMcPDelayMAC;

        // Config
        bool        Enable;

        // Init API
        void    AllocateResources();
        void    ParseParameters();

    protected:

        // OMNeT API
        virtual void handleMessage(cMessage *msg);

    public:

        // Constructor
        PTP_EthernetMapping();

        // Setters
        void    SetMACAddress( MACAddress MAC );
};

#endif
