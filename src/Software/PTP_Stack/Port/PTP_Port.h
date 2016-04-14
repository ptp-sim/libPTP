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

#ifndef LIBPTP_PTP_PORT_H_
#define LIBPTP_PTP_PORT_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "PTP.h"
#include "AppAnnounce.h"
#include "AppSync.h"
#include "AppDelay.h"
#include "AppPDelay.h"
#include "SubmoduleInitBase.h"

#include "ScheduleClock.h"

#include "PTPv2_m.h"
#include "PtpPortRequ_m.h"

// ======================================================
// Types
// ======================================================

class   PTP_Stack;

// ======================================================
// Declarations
// ======================================================

class cPTP_Port : public cSubmoduleInitBase
{
    private:

        // ------------------------------------------------------------
        // Resources
        // ------------------------------------------------------------
        PTP_Stack           *pApp;
        cScheduleClock      *pClock;
        IClockServo         *pClockServo;

        // ------------------------------------------------------------
        // Configuration
        // ------------------------------------------------------------
        UInteger16          portNumber;

        // ------------------------------------------------------------
        // Debug config
        // ------------------------------------------------------------
        bool                StateChange_EnableDebugOutput;

        // ------------------------------------------------------------
        // Statistics
        // ------------------------------------------------------------
        simsignal_t         StateDecision_SigId;

        // ------------------------------------------------------------
        // Data sets
        // ------------------------------------------------------------
        cPortDS             portDS;
        cForeignMasterDS    foreignMasterDS;

        // ------------------------------------------------------------
        // Services
        // ------------------------------------------------------------
        cAppAnnounce        appAnnounce;
        cAppSync            appSync;
        cAppDelay           appDelay;
        cAppPDelay          appPDelay;

        // ------------------------------------------------------------
        // Internal functions
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Init API
        // ------------------------------------------------------------
        void    ParseParameters();
        void    InitHierarchy();
        void    RegisterSignals();
        void    InitInternalState();
        void    InitSignals();
        void    ForwardInit( int stage );

    protected:

    public:

        // ------------------------------------------------------------
        // Constructors
        // ------------------------------------------------------------
        cPTP_Port();

        // ------------------------------------------------------------
        // Destructor
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Initialization
        // ------------------------------------------------------------
        void    SetHierarchy( PTP_Stack *pApp, cScheduleClock *pClock, IClockServo *pClockServo, UInteger16 portNumber );

        // ------------------------------------------------------------
        // Instance methods
        // ------------------------------------------------------------
        cPortDS&            PortDS();
        cForeignMasterDS&   ForeignMasterDS();
        cAppAnnounce&       AppAnnounce();
        cAppSync&           AppSync();

        // ------------------------------------------------------------
        // API for PTP stack
        // ------------------------------------------------------------
        void                ChangeState( portState_t NewState );
        void                FinishInitPhase();
        void                HandleEvent( portEvent_t Event );
        void                HandleStateDecision( portStateDecision_t StateDecision, bool NewMaster );
        void                HandlePtpFrame( PTPv2_Frame  *pPtpFrame );
        void                HandleSyncFURequ( PtpPortRequ_TrigSyncFU *pRequ );
        void                HandlePDelayFURequ( PtpPortRequ_TrigPDelayFU *pRequ );

        cForeignClockDS     GetErbest();
        portStateDecision_t GetStateDecision( cForeignClockDS Ebest, UInteger16 EbestPortID, bool EnableDebugOutput );

        // ------------------------------------------------------------
        // API for services and data sets
        // ------------------------------------------------------------
        void                IssueFrame( PTPv2_Frame *pPTP );
        void                ConfigPortPathDelay( simtime_t meanPathDelay );
        void                ConfigPortAsymmetry( simtime_t Asymmetry );
        simsignal_t         RegisterDynamicSignal( const char *pSigName );

        // ------------------------------------------------------------
        // PTP stack API
        // ------------------------------------------------------------
        void                HandleTimejump( simtime_t Delta );

        // ------------------------------------------------------------
        // Setters
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Getters
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Instance functions
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Friends
        // ------------------------------------------------------------
        friend  class   cPortService;
};

#endif
