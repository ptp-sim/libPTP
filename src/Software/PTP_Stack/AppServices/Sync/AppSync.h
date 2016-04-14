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

#ifndef LIBPTP_APP_SYNC_H_
#define LIBPTP_APP_SYNC_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "PTP.h"
#include "FilteredPortService.h"

#include "IClockServo.h"

#include "PTPv2_m.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cAppSync  : public cFilteredPortService
{
    private:

        // Configuration
        std::string         ClockServoPath;
        std::string         ClockPath;

        // Resources
        IClockServo         *pClockServo;
        cScheduleClock      *pClock;

        // TwoStepFlag stuff
        simtime_t           SyncIngress;
        simtime_t           SyncCorr;
        uint16_t            SyncSequID;

        // Syntonization support
        simtime_t           OldSyncIngress;
        simtime_t           OldMasterTime;

        // Signals for statistics
        simsignal_t         SyncRcvd_SigId;
        simsignal_t         SyncFuRcvd_SigId;

        // Debug config
        bool                EnableDebugOutput;

        // Frame creation
        PTPv2_SyncFrame         *CreateSyncFrame();
        PTPv2_Follow_UpFrame    *CreateFollowUpFrame( PTPv2_SyncFrame *pSync );

        void            HandleSync    ( PTPv2_SyncFrame         *pSync      );
        void            HandleFollowUp( PTPv2_Follow_UpFrame    *pFollowUp  );

        // Synchronization functions
        void SynchronizeClock(  simtime_t SyncIngress,
                                simtime_t originTimestamp,
                                simtime_t SyncCorr,
                                simtime_t FollowUpCorr );

        // Init API
        void    ParseResourceParameters();
        void    ParseParameters();
        void    RegisterSignals();
        void    InitInternalState();
        void    InitSignals();

    protected:

    public:

        // Constructors
        cAppSync();

        // Destructor

        // Setters

        // Getters

        // API functions
        void    StartListening();
        void    StopListening();
        void    HandleMsg( PTPv2_Frame  *pPtpFrame );
        void    TriggerFollowUp( PTPv2_SyncFrame *pSync );

        void    HandleIntervalEvent();
        void    HandleTimeoutEvent();

        // Init API
        void    SetHierarchy( PTP_Stack *pApp, cPTP_Port *pPort, cScheduleClock *pClock, IClockServo *pClockServo );
};

#endif
