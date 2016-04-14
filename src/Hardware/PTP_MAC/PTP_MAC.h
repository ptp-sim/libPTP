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

#ifndef LIBPTP_PTP_MAC_H_
#define LIBPTP_PTP_MAC_H_

// ======================================================
// Includes
// ======================================================

#include "EtherMAC.h"

#include "PTP.h"
#include "ScheduleClock.h"
#include "PTP_NIC_Ctrl.h"
#include "IInitBase.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class PTP_MAC : public EtherMAC, public IInitBase
{

  private:

    // ------------------------------------------------------------
    // Types
    // ------------------------------------------------------------
    typedef enum
    {
        DROP_FRAME,
        HANDLE_FRAME,
    }
    FrameHandling_t;

    // ------------------------------------------------------------
    // Resources
    // ------------------------------------------------------------
    std::string             ClockPath;
    std::string             PTP_NIC_CtrlPath;

    cScheduleClock          *pClock;
    PTP_NIC_Ctrl            *pNIC_Ctrl;

    cMessage                *FaultMsg;
    cMessage                *RecoveryMsg;

    // ------------------------------------------------------------
    // Internal housekeeping
    // ------------------------------------------------------------
    cPortIdentity           PortIdentity;

    // ------------------------------------------------------------
    // Configuration
    // ------------------------------------------------------------
    bool                    PTP_Enable;
    PTP_ClockType_t         PTP_ClockType;
    delayMechanism_t        PTP_DelayMechanism;     // P2P or E2E
    bool                    PTP_TwoStepFlag;        // On-the-fly mode or FollowUp frames

    // ------------------------------------------------------------
    // Debug configuration
    // ------------------------------------------------------------
    bool                    EnableDebugOutput;

    // -------------------------------------------------
    // Internal functions
    // -------------------------------------------------

    // Egress handling
    simtime_t   CalcResidenceTime( PTPv2_Frame *pPtpFrame, int inputPort );
    void        AddCorrection( PTPv2_Frame *pPtpFrame, simtime_t Correction );
    void        AddAsymmetry( PTPv2_Frame *pPtpFrame );
    void        AddAsymmetry( PTPv2_Frame *pPtpFrame, int inputPort );
    void        SubstractAsymmetry( PTPv2_Frame *pPtpFrame );
    void        SubstractAsymmetry( PTPv2_Frame *pPtpFrame, int inputPort );

    void        HandleEgress();

    void        HandleEgressOwnPtp          ( PTPv2_Frame                   *pPtpFrame      );
    void        HandleEgressOwnSync         ( PTPv2_SyncFrame               *pSync          );
    void        HandleEgressOwnFollowUp     ( PTPv2_Follow_UpFrame          *pFollowUp      );
    void        HandleEgressOwnDelayReq     ( PTPv2_Delay_ReqFrame          *pDelReq        );
    void        HandleEgressOwnDelayResp    ( PTPv2_Delay_RespFrame         *pDelResp       );
    void        HandleEgressOwnPDelayReq    ( PTPv2_PDelay_ReqFrame         *pPDelReq       );
    void        HandleEgressOwnPDelayResp   ( PTPv2_PDelay_RespFrame        *pPDelResp      );
    void        HandleEgressOwnPDelayRespFU ( PTPv2_PDelay_Resp_FU_Frame    *pPDelRespFU    );

    void        HandleEgressForeignPtp          ( PTPv2_Frame                   *pPtpFrame,   int inputPort );
    void        HandleEgressForeignSync         ( PTPv2_SyncFrame               *pSync,       int inputPort );
    void        HandleEgressForeignFollowUp     ( PTPv2_Follow_UpFrame          *pFollowUp,   int inputPort );
    void        HandleEgressForeignDelayReq     ( PTPv2_Delay_ReqFrame          *pDelReq,     int inputPort );
    void        HandleEgressForeignDelayResp    ( PTPv2_Delay_RespFrame         *pDelResp,    int inputPort );
    void        HandleEgressForeignPDelayReq    ( PTPv2_PDelay_ReqFrame         *pPDelReq,    int inputPort );
    void        HandleEgressForeignPDelayResp   ( PTPv2_PDelay_RespFrame        *pPDelResp,   int inputPort );
    void        HandleEgressForeignPDelayRespFU ( PTPv2_PDelay_Resp_FU_Frame    *pPDelRespFU, int inputPort );

    // Ingress handling
    void        TimestampOnIngress( PTPv2_Frame *pPtpFrame );

    FrameHandling_t HandleIngress( EtherFrame *pEthFrame );
    void            HandleIngressAnnounce   ( PTPv2_AnnounceFrame       *pAnn       );
    void            HandleIngressSync       ( PTPv2_SyncFrame           *pSync      );
    void            HandleIngressDelayReq   ( PTPv2_Delay_ReqFrame      *pDelReq    );
    void            HandleIngressDelayResp  ( PTPv2_Delay_RespFrame     *pDelResp   );
    void            HandleIngressPDelayReq  ( PTPv2_PDelay_ReqFrame     *pPDelReq   );
    void            HandleIngressPDelayResp ( PTPv2_PDelay_RespFrame    *pPDelResp  );

    // Init API
    void    ParseResourceParameters();
    void    AllocateResources();
    void    ParseParameters();
    void    InitInternalState();

    // Error handling
    void    HandleUnexpectedEgress( PTPv2_Frame *pFrame, std::string Reason );

  protected:

    virtual void    handleSelfMessage(cMessage *msg);
    virtual void    handleMessageWhenDown(cMessage *msg);

  public:

    // Constructor
    PTP_MAC();

    // Setters
    void    SetMACAddress( MACAddress MAC );

  protected:

    // OMNeT API
    int     numInitStages() const;
    void    initialize(int stage);

    // -------------------------------------------------
    // Derived functions of base class
    // -------------------------------------------------
    void startFrameTransmission();
    void processMsgFromNetwork(EtherTraffic *msg);
};

#endif

