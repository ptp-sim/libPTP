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

#ifndef LIBPTP_PTP_FOREIGN_MASTER_DS_H_
#define LIBPTP_PTP_FOREIGN_MASTER_DS_H_

// ======================================================
// Includes
// ======================================================

#include "ScheduleClock.h"

#include "PTP_PortIdentity.h"
#include "PTP_ForeignMasterDS_Entry.h"
#include "PTP_ForeignClockDS.h"
#include "PTP_ForeignClockMsg.h"

#include "SubmoduleInitBase.h"

// ======================================================
// Types
// ======================================================

class   cPTP_Port;

// ======================================================
// Declarations
// ======================================================

// Foreign Master data set, see clause 9.3.2.4
class cForeignMasterDS : public cSubmoduleInitBase
{
    private:

        // Resources
        cPTP_Port                               *pPort;
        cScheduleClock                          *pClock;
        std::vector<cForeignMasterDS_Entry>     ForeignMasters;

        // Configuration
        simtime_t                               ForeignMasterTimeWindow;

        cForeignClockMsg                        Erbest;

        // Signals for statistics
        simsignal_t                             ForeinMasterDS_Cnt_SigId;

        // Internal functions
        void    CalculateErbest();
        bool    IsErbestValid();

    protected:

    public:

        // Constructors/Destructor
        cForeignMasterDS();
        cForeignMasterDS( const cForeignMasterDS& other );
       ~cForeignMasterDS();

        // Instance methods

        // Initialize
        void    SetHierarchy( cPTP_Port *pPort, cScheduleClock *pClock );

        // Init API
        void    RegisterSignals();
        void    InitSignals();

        // Setters
        void    SetForeignMasterTimeWindow( simtime_t ForeignMasterTimeWindow );

        // Getters

        // API functions
        void                PushForeignClock( cForeignClockDS ForeignClockDS, simtime_t RxTime );
        size_t              GetNumEntries( cPortIdentity foreignMasterPortIdentity );
        cForeignClockDS     GetErbest();
        void                AdjustTimestamps( simtime_t Delta );

        // Operators
        cForeignMasterDS&   operator=( const cForeignMasterDS& other );

        // Debug functions
};

#endif
