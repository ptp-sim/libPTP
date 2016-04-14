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

#ifndef LIBPTP_PTP_PORTDS_H_
#define LIBPTP_PTP_PORTDS_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "PTP_PrimitiveDataTypes.h"
#include "PTP_PortIdentity.h"
#include "PTP_TimeInterval.h"
#include "SubmoduleInitBase.h"

// ======================================================
// Types
// ======================================================

class   cPTP_Port;

// ======================================================
// Declarations
// ======================================================

// Port data set, see clause 8.2.5 in IEEE 1588-2008
class cPortDS : public cSubmoduleInitBase
{
    private:

        // ------------------------------------------------------------
        // Resources
        // ------------------------------------------------------------
        cPTP_Port               *pPort;

        // Signals for statistics
        simsignal_t             peerMeanPathDelay_SigId;
        simsignal_t             portState_SigId;

        // ------------------------------------------------------------
        // Static members
        // ------------------------------------------------------------
        cPortIdentity           portIdentity;                                   // ID of this port

        // ------------------------------------------------------------
        // Dynamic members
        // ------------------------------------------------------------
        portState_t             portState;                                      // Current port state
        int8_t                  logMinDelayReqInterval;                         // Exponent in range -128 ... 127, [s]
        cTimeInterval           peerMeanPathDelay;                              // Result of P2P measurement

        // ------------------------------------------------------------
        // Configurable members
        // ------------------------------------------------------------
        int8_t                  logAnnounceInterval;                            // Exponent in range -128 ... 127, [s]
        uint8_t                 announceReceiptTimeout;                         // Number of missed Announce-intervals before ANNOUNCE_RECEIPT_TIMEOUT_EXPIRES is triggered
        int8_t                  logSyncInterval;                                // Exponent in range -128 ... 127, [s]
        delayMechanism_t        delayMechanism;                                 // P2P or E2E
        int8_t                  logMinPdelayReqInterval;                        // Exponent in range -128 ... 127, [s]
        UInteger4               versionNumber;                                  // Has to be 2 for IEEE 1588-2008

        cTimeInterval           Asymmetry;                                      // Not in the standard, but added to portDS as it fits here

        // ------------------------------------------------------------
        // Internal functions
        // ------------------------------------------------------------
        void    RegisterSignals();
        void    InitSignals();

    protected:

    public:

        // Constructors/Destructor
        cPortDS();
        cPortDS( const cPortDS& other );
        ~cPortDS();

        // Initialization
        void    SetHierarchy( cPTP_Port *pPort );

        // Setters
        void    SetPeerMeanPathDelay        ( cTimeInterval     offsetFromMaster        );
        void    SetPortState                ( portState_t       portState               );
        void    SetLogMinDelayReqInterval   ( int8_t            logMinDelayReqInterval  );
        void    SetLogAnnounceInterval      ( int8_t            logAnnounceInterval     );
        void    SetAnnounceReceiptTimeout   ( uint8_t           announceReceiptTimeout  );
        void    SetLogSyncInterval          ( int8_t            logSyncInterval         );
        void    SetDelayMechanism           ( delayMechanism_t  delayMechanism          );
        void    SetLogMinPdelayReqInterval  ( int8_t            logMinPdelayReqInterval );
        void    SetVersionNumber            ( UInteger4         versionNumber           );
        void    SetAsymmetry                ( cTimeInterval     Asymmetry               );

        // Instance methods
        cPortIdentity&          PortIdentity();

        // Getters
        cTimeInterval           GetPeerMeanPathDelay();
        portState_t             GetPortState();
        int8_t                  GetLogMinDelayReqInterval();
        int8_t                  GetLogAnnounceInterval();
        uint8_t                 GetAnnounceReceiptTimeout();
        int8_t                  GetLogSyncInterval();
        delayMechanism_t        GetDelayMechanism();
        int8_t                  GetLogMinPdelayReqInterval();
        UInteger4               GetVersionNumber();
        cTimeInterval           GetAsymmetry();

        // Operators
        bool            operator== (const cPortDS& other);
        cPortDS&        operator= (const cPortDS& other);

        // Debug functions
        void    Print();
};

#endif
