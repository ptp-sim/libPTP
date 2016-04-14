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

#ifndef LIBPTP_PTP_STACK_H_
#define LIBPTP_PTP_STACK_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "MACAddress.h"

#include "PTP.h"
#include "PTP_Port.h"
#include "AppStateDecision.h"
#include "AppAnnounce.h"

#include "PTP_ProfileChecker.h"

#include "IClockEventSink.h"
#include "ScheduleClock.h"
#include "IClockServo.h"
#include "ModuleInitBase.h"

#include "PTPv2_m.h"
#include "PtpPortRequ_m.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class PTP_Stack: public cModuleInitBase, public IClockEventSink
{
    private:

        // ------------------------------------------------------------
        // Resources
        // ------------------------------------------------------------
        cScheduleClock                  *pClock;
        IClockServo                     *pClockServo;
        cPTP_Port                       *pPorts;
        PTP_ProfileChecker              *pProfileChecker;
        int                             PtpInGateID;
        int                             PtpOutGateID;
        int                             RequestGateID;
        int                             ConfigGateID;
        int                             PtpGateSize;
        int                             ModuleID;
        cClockEvent                     FinishBootupEvent;

        // ------------------------------------------------------------
        // Configuration
        // ------------------------------------------------------------
        bool                            Enabled;
        PTP_Profile_t                   PTP_Profile;
        PTP_ClockType_t                 PTP_ClockType;
        bool                            Active_E2E_TC;
        PTP_BMCA_t                      BMCA;
        PTP_MgmtProtocol_t              MgmtProtocol;
        delayMechanism_t                DelayMechanism;
        std::string                     ClockPath;
        std::string                     ClockServoPath;
        simtime_t                       Asymmetry;

        // Initial values for default data set
        bool                            TwoStepFlag;                //
        bool                            SlaveOnly;                  //
        uint8_t                         Priority1;                  //
        uint8_t                         Priority2;                  //
        clockAccuracy_t                 ClockAccuracy;              //
        ClockClass_t                    ClockClass;                 //
        UInteger16                      offsetScaledLogVariance;    //
        domainNumber_t                  PrimaryDomain;              // Primary domain number
        MACAddress                      MAC;
        cClockIdentity                  ClockIdentity;

        // Debug config
        bool                            EnableDebugOutput;

        // Port config
        int8_t                          LogMinDelayReqInterval;
        int8_t                          LogAnnounceInterval;
        int8_t                          LogSyncInterval;
        int8_t                          LogMinPdelayReqInterval;
        uint8_t                         AnnounceReceiptTimeout;

        // ------------------------------------------------------------
        // Synchronization
        // ------------------------------------------------------------
        simtime_t                       MaxOffsetFromMaster;

        // ------------------------------------------------------------
        // Data sets
        // ------------------------------------------------------------
        cDefaultDS                      defaultDS;                  // Default Data Set
        cCurrentDS                      currentDS;                  // Current Data Set
        cParentDS                       parentDS;                   // Parent Data Set
        cTimePropertiesDS               timePropertiesDS;           // Time Properties Data Set
        cTimePropertiesDS               timePropertiesDS_Backup;    // Backup copy of the Time Properties Data Set
        cForeignClockDS                 D0;                         // Default Data Set in comparable form

        // ------------------------------------------------------------
        // Services
        // ------------------------------------------------------------
        cAppStateDecision               AppStateDecision;
        simtime_t                       StateDecisionInterval;

        // ------------------------------------------------------------
        // Internal state handling
        // ------------------------------------------------------------
        cForeignClockDS                 Ebest;

        // Parent DS
        simsignal_t                     offsetFromMaster_raw_SigId;
        simsignal_t                     meanPathDelay_raw_SigId;

        // ------------------------------------------------------------
        // Watch variables
        // ------------------------------------------------------------
        simtime_t                       offsetFromMaster;
        simtime_t                       meanPathDelay;

        // ------------------------------------------------------------
        // Debug functions
        // ------------------------------------------------------------
        void    PrintInfo();

        // ------------------------------------------------------------
        // Internal utilities
        // ------------------------------------------------------------
        void    CheckPortIdx( int PortIdx );

        // ------------------------------------------------------------
        // Clock servo handling
        // ------------------------------------------------------------
        void    StartClockServo();
        void    StopClockServo();

        // ------------------------------------------------------------
        // Functions for initialization
        // ------------------------------------------------------------
        void    ParseStackParameters();
        void    ParsePortParameters();
        void    ParsePtpEvents();

        // ------------------------------------------------------------
        // State handling
        // ------------------------------------------------------------
        void    StartInitialize();
        void    StopInitialize();
        void    HandleEvent     ( portEvent_t   Event, int  PortID );
        void    HandlePtpFrame  ( PTPv2_Frame   *pPtpFrame, int PortIdx);
        void    HandlePortRequ  ( PtpPortRequ   *pRequ );

        void    BecomeSlave( cForeignClockDS   Ebest );
        void    BecomeGrandMaster();

    protected:

        // ------------------------------------------------------------
        // Init API
        // ------------------------------------------------------------
        void    ParseResourceParameters();
        void    AllocateResources();
        void    InitHierarchy();
        void    ParseParameters();
        void    RegisterSignals();
        void    InitInternalState();
        void    InitSignals();
        void    FinishInit();
        void    DebugOutput();
        void    PrintDebugOutput();

        void    ForwardInit( int stage );

        // ------------------------------------------------------------
        // OMNeT API
        // ------------------------------------------------------------
        void    handleMessage(cMessage *msg);
        void    finish();

        // ------------------------------------------------------------
        // Port service API
        // ------------------------------------------------------------
        void    HandleTimejump( simtime_t Delta );

        // ------------------------------------------------------------
        // Service API
        // ------------------------------------------------------------
        void    HandlePortStateDec( portStateDecision_t StateDecision );

    public:

        // ------------------------------------------------------------
        // Constructors/Destructor
        // ------------------------------------------------------------
        PTP_Stack();
        ~PTP_Stack();

        // ------------------------------------------------------------
        // Setters
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Getters
        // ------------------------------------------------------------
        int     GetModuleID();

        // ------------------------------------------------------------
        // API functions for services
        // ------------------------------------------------------------
        void    SetOffsetFromMaster( simtime_t offsetFromMaster );
        void    SetMeanPathDelay( simtime_t meanPathDelay );
        void    IssueFrame( PTPv2_Frame *pPTP, UInteger16 PortNumber );
        void    ConfigNIC_MAC();
        void    ConfigPortPathDelay( uint16_t PortID, simtime_t meanPathDelay );
        void    ConfigPortAsymmetry( uint16_t PortID, simtime_t Asymmetry );
        void    EmitSignal_offsetFromMaster_raw( simtime_t offsetFromMaster_raw );
        void    EmitSignal_meanPathDelay_raw( simtime_t meanPathDelay_raw );

        // ------------------------------------------------------------
        // Clock API
        // ------------------------------------------------------------
        void    HandleClockEvent( cClockEvent& ClockEvent );

        // ------------------------------------------------------------
        // API for other modules
        // ------------------------------------------------------------
        void    SetMACAddress( MACAddress MAC );
        void    SetClockIdentity( cClockIdentity ClockID );
        void    Disable();

        // ------------------------------------------------------------
        // Friends
        // ------------------------------------------------------------
        friend  class   cAppService;
        friend  class   cPTP_Port;
        friend  class   cAppAnnounce;
        friend  class   cAppSync;
        friend  class   cAppDelay;
        friend  class   cAppPDelay;
        friend  class   cAppStateDecision;
        friend  class   PTP_DefaultProfileChecker;
        friend  class   PTP_DefaultE2EProfileChecker;
        friend  class   PTP_DefaultP2PProfileChecker;
        friend  class   PTP_PowerProfileChecker;
};

#endif
