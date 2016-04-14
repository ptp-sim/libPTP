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

#ifndef LIBPTP_PTP_NIC_CTRL_H_
#define LIBPTP_PTP_NIC_CTRL_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "PTP_FrameMatch.h"
#include "ModuleInitBase.h"

#include "PtpPortConfig_m.h"

// ======================================================
// Types
// ======================================================

struct Port_t
{
    simtime_t   peerMeanPathDelay;
    simtime_t   Asymmetry;
};

// ======================================================
// Declarations
// ======================================================

class PTP_NIC_Ctrl : public cModuleInitBase
{
    private:

        // Resources
        PTP_FrameMatch          SyncFU_Matcher;
        PTP_FrameMatch          DelayReqResp_Matcher;
        PTP_FrameMatch          PDelayReqResp_Matcher;
        PTP_FrameMatch          PDelayRespFu_Matcher;

        std::vector<Port_t>     Ports;

        int                     RequestGateID;
        int                     ConfigGateID;
        int                     NumPorts;

        // Config

        // -------------------------------------------------
        // Internal functions
        // -------------------------------------------------

        // Init API
        void    ParseResourceParameters();
        void    AllocateResources();
        void    InitHierarchy();
        void    InitInternalState();
        void    ForwardInit( int stage );

        void    HandlePtpConfig( PtpPortConfig *pConfig );
        void    HandleNicConfig( PtpNicConfig  *pConfig );

        void    CheckPortIdx( int PortIdx );

    protected:

        void handleMessage(cMessage *msg);

    public:

        // Constructors
        PTP_NIC_Ctrl();

        // Instance functions
        PTP_FrameMatch  &SyncFuMatcher();
        PTP_FrameMatch  &DelayReqRespMatcher();
        PTP_FrameMatch  &PDelayReqRespMatcher();
        PTP_FrameMatch  &PDelayRespFuMatcher();

        void        RequestSyncFollowUp( uint16_t PortIdx, PTPv2_SyncFrame *pSync );
        void        RequestPDelayFollowUp( uint16_t PortIdx, PTPv2_PDelay_RespFrame *pPDelResp );
        void        ReportFault( uint16_t PortIdx );
        void        ReportRecovery( uint16_t PortIdx );
        simtime_t   GetMeanPathDelay( uint16_t PortIdx );
        simtime_t   GetAsymmetry( uint16_t PortIdx );
};

#endif
