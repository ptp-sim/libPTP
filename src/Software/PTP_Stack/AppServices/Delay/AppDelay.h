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

#ifndef LIBPTP_APP_DELAY_H_
#define LIBPTP_APP_DELAY_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "PTP.h"
#include "FilteredPortService.h"
#include "TimestampMatcher.h"

#include "PTPv2_m.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cAppDelay  : public cFilteredPortService
{
    private:

    // Resources
    cTimestampMatcher   TimestampMatcher;

    // Configuration
    bool                TwoStepFlag;

    // Internal housekeeping
    UInteger16          RequSequId;
    bool                TriggerStart;
    MatchedTime_t       SyncTimes;

    // Signals for statistics
    simsignal_t         DelReqRcvd_SigId;
    simsignal_t         DelRespRcvd_SigId;

    // Debug config
    bool                EnableDebugOutput;

    // Frame creation
    PTPv2_Delay_ReqFrame    *CreateDelayReqFrame();
    PTPv2_Delay_RespFrame   *CreateDelayRespFrame( PTPv2_Delay_ReqFrame *pReq );

    // Frame handling
    void    HandleSync      ( PTPv2_SyncFrame         *pSync        );
    void    HandleFollowUp  ( PTPv2_Follow_UpFrame    *pFollowUp    );
    void    HandleDelayReq  ( PTPv2_Delay_ReqFrame    *pReq         );
    void    HandleDelayResp ( PTPv2_Delay_RespFrame   *pResp        );

    // Init API
    void    ParseResourceParameters();
    void    ParseParameters();
    void    RegisterSignals();
    void    InitSignals();

    protected:

    public:

        // Constructors
        cAppDelay();

        // Destructor

        // Setters

        // Getters

        // API functions
        void    StartInterval();
        void    StopInterval();

        void    HandleMsg( PTPv2_Frame  *pPtpFrame );

        void    HandleIntervalEvent();
        void    HandleTimeoutEvent();
        void    HandleTimeJump();
};

#endif
