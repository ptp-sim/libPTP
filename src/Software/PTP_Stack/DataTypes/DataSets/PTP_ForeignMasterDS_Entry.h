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

#ifndef LIBPTP_PTP_FOREIGNMASTERDS_ENTRY_H_
#define LIBPTP_PTP_FOREIGNMASTERDS_ENTRY_H_

// ======================================================
// Includes
// ======================================================

#include "ScheduleClock.h"

#include "PTP_PortIdentity.h"
#include "PTP_ForeignClockDS.h"
#include "PTP_ForeignClockMsg.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cForeignMasterDS_Entry
{
    private:

        cPortIdentity                       foreignMasterPortIdentity;
        std::list<cForeignClockMsg>         ForeignClockMsgs;

        // Internal resources
        cScheduleClock                      *pClock;
        simtime_t                           ForeignMasterTimeWindow;

    protected:

    public:

        // Constructors/Destructor
        cForeignMasterDS_Entry();
        cForeignMasterDS_Entry( const cForeignMasterDS_Entry& other );
        ~cForeignMasterDS_Entry();

        // Instance methods
        cPortIdentity&      PortIdentity();

        // Setters
        void                SetClock( cScheduleClock *pClock );
        void                SetForeignMasterTimeWindow( simtime_t ForeignMasterTimeWindow );

        // Getters
        simtime_t           GetForeignMasterTimeWindow();

        // API functions
        void                PushForeignClock( cForeignClockDS ForeignClockDS, simtime_t RxTime, bool &NewDataFlag );
        cForeignClockMsg    GetMostRecentEntry();
        size_t              GetNumEntries();
        bool                IsEmpty();
        void                CleanUp();
        void                AdjustTimestamps( simtime_t Delta );

        // Operators
        cForeignMasterDS_Entry&  operator=( const cForeignMasterDS_Entry& other );
};

#endif
