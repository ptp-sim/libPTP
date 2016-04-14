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

#ifndef LIBPTP_PTP_TESTBENCH_PTP_FRAME_GEN_TEST_H_
#define LIBPTP_PTP_TESTBENCH_PTP_FRAME_GEN_TEST_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "PTPv2_m.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class PtpFrameGen : public cSimpleModule
{
    private:

        void    FillInBasicInfo( PTPv2_Frame *pPtp );
        void    FillInAnnounceFrame( PTPv2_AnnounceFrame *pPtp );
        void    FillInSyncFrame( PTPv2_SyncFrame *pPtp );
        void    FillInSyncFUFrame( PTPv2_Follow_UpFrame *pPtp );
        void    FillInDelayReqFrame( PTPv2_Delay_ReqFrame *pPtp);
        void    FillInPDelayReqFrame( PTPv2_PDelay_ReqFrame *pPtp);

        PTPv2_AnnounceFrame     *CreateAnnounceFrame();
        PTPv2_SyncFrame         *CreateSyncFrame();
        PTPv2_Follow_UpFrame    *CreateFollowUpFrame();
        PTPv2_Delay_ReqFrame    *CreateDelayReqFrame();
        PTPv2_PDelay_ReqFrame   *CreatePDelayReqFrame();

        PTPv2_SyncFrame         *CreateEthFrame( size_t ByteLength );

        void    ParseParameters();

    protected:

        // Resources
        UInteger16          AnnSequId;
        UInteger16          SyncSequId;
        UInteger16          DelaySequId;
        UInteger16          PDelaySequId;
        PTPv2_SyncFrame     *pSyncDup;

        // Configuration
        cPortIdentity       PortIdentity;
        bool                PTP_TwoStepFlag;
        uint8_t             Priority1;
        uint8_t             Priority2;

        cClockQuality       ClockQuality;
        domainNumber_t      PrimaryDomain;

        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        ~PtpFrameGen();
};

#endif
