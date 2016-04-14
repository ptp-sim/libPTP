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

#ifndef LIBPTP_APP_PDELAY_H_
#define LIBPTP_APP_PDELAY_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "PTP.h"
#include "FilteredPortService.h"

#include "PTPv2_m.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cAppPDelay  : public cFilteredPortService
{
    private:

    // Configuration

    // Resources

    // Internal housekeeping
    UInteger16      RequSequId;
    UInteger16      RespSequID;

    // TwoStepFlag stuff
    simtime_t       ReqEgress;
    simtime_t       RespIngress;
    simtime_t       RespCorr;
    simtime_t       RequestReceipt;

    // Signals for statistics
    simsignal_t     PDelReqRcvd_SigId;
    simsignal_t     PDelRespRcvd_SigId;
    simsignal_t     PDelRespFuRcvd_SigId;
    simsignal_t     peerMeanPathDelay_raw_SigId;

    // Debug config
    bool            EnableDebugOutput;

    // Frame creation
    PTPv2_PDelay_ReqFrame       *CreatePDelayReqFrame();
    PTPv2_PDelay_RespFrame      *CreatePDelayRespFrame( PTPv2_PDelay_ReqFrame *pReq );
    PTPv2_PDelay_Resp_FU_Frame  *CreatePDelayRespFUFrame( PTPv2_PDelay_ReqFrame *pReq );
    PTPv2_PDelay_Resp_FU_Frame  *CreatePDelayRespFUFrame( PTPv2_PDelay_RespFrame *pResp );

    // Frame handling
    void    HandlePDelayReq     ( PTPv2_PDelay_ReqFrame         *pReq       );
    void    HandlePDelayResp    ( PTPv2_PDelay_RespFrame        *pResp      );
    void    HandlePDelayRespFU  ( PTPv2_PDelay_Resp_FU_Frame    *pRespFU    );

    void    CalcMeanPathDelay   (   simtime_t t1,
                                    simtime_t t4,
                                    simtime_t requestReceiptTimestamp,
                                    simtime_t responseOriginTimestamp,
                                    simtime_t CorrPDelResp,
                                    simtime_t CorrPDelRespFU );

    // Init API
    void    ParseResourceParameters();
    void    ParseParameters();
    void    RegisterSignals();
    void    InitSignals();

    protected:

    public:

        // Constructors/Destructor
        cAppPDelay();
        cAppPDelay( const cAppPDelay& other );

        // Destructor

        // Setters

        // Getters

        // API functions
        void    HandleMsg( PTPv2_Frame  *pPtpFrame );

        void    HandleIntervalEvent();
        void    HandleTimeoutEvent();
        void    HandleTimeJump();

        void    TriggerFollowUp( PTPv2_PDelay_RespFrame *pResp );

        // Operators
        cAppPDelay& operator=( const cAppPDelay& other );
};

#endif
