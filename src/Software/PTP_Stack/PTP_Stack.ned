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

package libptp.Software.PTP_Stack;

simple PTP_Stack
{
    parameters:
        @display("i=PTP/Components/PTP_Stack/PTP_Stack");

        // Simulation parameters
        bool        Enabled                         = default(true);
        int         NodeNumber                      = default(-1);
        double      StartupInterval @unit(s)        = default( uniform(10ms, 100ms) );

        // Parameters for internal state
        string      PTP_Profile                     = default("PTP_PROFILE_CUSTOM");
        string      PTP_ClockType                   = default("PTP_CLOCK_TYPE_ORDINARY");
        string      PTP_BestMasterClockAlgorithm    = default("BMCA_1588_2008_DEFAULT");
        string      PTP_ManagementProtocol          = default("MGMT_1588_2008_DEFAULT");
        bool        Active_E2E_TC                   = default(false);

        // Resource config
        bool        UseMacForClockID                = default(true);
        bool        AutoMAC                         = default(true);
        string      MAC_Address                     = default("C0:FF:EE:BA:D0:1D");
        string      ClockID                         = default("C0 FF EE 00 00 BA D0 1D");
        string      ClockPath                       = default("^.Clock");
        string      ClockServoPath                  = default("^.ClockServo");

        // Parameters for default data set
        bool        PTP_TwoStepFlag                 = default(false);
        bool        SlaveOnly                       = default(false);
        int         Priority1                       = default(128);
        int         Priority2                       = default(128);
        int         OffsetScaledLogVariance         = default(100);
        string      ClockClass                      = default("CLOCK_CLASS_SLAVE_ONLY");
        string      ClockAccuracy                   = default("CLOCK_ACCURACY_UNKNOWN");
        string      DomainNumber                    = default("DOMAIN_DEFAULT");

        // Synchronization parameters
        double      MaxOffsetFromMaster @unit(s) = default( 100us );

        // Filtering parameters
        string      offsetFromMasterFilter_Type             = default("MOVING_AVG_FILTER");
        int         offsetFromMasterFilter_Len              = default( 5 );
        bool        offsetFromMasterFilter_DiscardMinMax    = default(true);
        string      meanPathDelayFilter_Type                = default("MOVING_AVG_FILTER");
        int         meanPathDelayFilter_Len                 = default( 5 );
        bool        meanPathDelayFilter_DiscardMinMax       = default(true);
        string      meanPeerDelayFilter_Type                = default("MOVING_AVG_FILTER");
        int         meanPeerDelayFilter_Len                 = default( 5 );
        bool        meanPeerDelayFilter_DiscardMinMax       = default(true);

        // Fault simulation
        bool        SimulateFault               = default( false );
        double      FaultTime @unit(s)          = default( 0s );
        double      FaultDuration @unit(s)      = default( 1s );
        int         FaultPortID                 = default( 0 );

        // Profile specific parameters
        bool        PowerProfile_GrandmasterCapable     = default( true );
        bool        PowerProfile_PreferredGrandmaster   = default( true );

        // Debug
        bool        EnableDebugOutput                       = default( false );
        bool        StateDecision_EnableBriefDebugOutput    = default( false );
        bool        StateDecision_EnableDetailedDebugOutput = default( false );
        bool        Port_StateChange_EnableDebugOutput      = default( false );
        bool        Port_Sync_EnableDebugOutput             = default( false );
        bool        Port_Delay_EnableDebugOutput            = default( false );
        bool        Port_PDelay_EnableDebugOutput           = default( false );

        // Display options
        string      StateDec_TooltipPath    = default("^");

        // Port characteristics
        // Default values taken from J.3.2) PTP attribute values
        int         LogAnnounceInterval     = default(1);                     // Exponent in range -128 ... 127, [s]
        int         AnnounceReceiptTimeout  = default(3);                     // Number of missed Announce-intervals before ANNOUNCE_RECEIPT_TIMEOUT_EXPIRES is triggered
        int         LogSyncInterval         = default(0);                     // Exponent in range -128 ... 127, [s]
        int         LogMinDelayReqInterval  = default(0);                     // Exponent in range -128 ... 127, [s]
        int         LogMinPdelayReqInterval = default(0);
        string      PTP_DelayMechanism      = default("DELAY_MECH_E2E");

        double      Asymmetry@unit(s)       = default(-62ns);                 // See 1588 chapter 11.6
                                                                              // Rx delay 208ns
                                                                              // Tx delay 84ns
                                                                              // --> asymmetry -62ns

        // -----------------------------------------------------------------------
        // Signals
        // -----------------------------------------------------------------------

        // BMC
        @signal[EbestModuleID](type=long);

        // CurrentDS
        @signal[offsetFromMaster](type=simtime_t);
        @signal[offsetFromMaster_raw](type=simtime_t);
        @signal[stepsRemoved](type=unsigned long);
        @signal[meanPathDelay](type=simtime_t);
        @signal[meanPathDelay_raw](type=simtime_t);

        // ParentDS

        // Ports
        @signal[Port_*_peerMeanPathDelay](type=simtime_t);
        @signal[Port_*_peerMeanPathDelay_raw](type=simtime_t);
        @signal[Port_*_portState](type=long);

        @signal[Port_*_AnnounceRcvd](type=long);
        @signal[Port_*_SyncRcvd](type=long);
        @signal[Port_*_SyncFuRcvd](type=long);
        @signal[Port_*_DelReqRcvd](type=long);
        @signal[Port_*_DelRespRcvd](type=long);
        @signal[Port_*_PDelReqRcvd](type=long);
        @signal[Port_*_PDelRespRcvd](type=long);
        @signal[Port_*_PDelRespFuRcvd](type=long);

        @signal[Port_*_ErbestModuleID](type=long);
        @signal[Port_*_StateDecision](type=long);
        @signal[Port_*_ForgeinMasterDS_Cnt](type=long);

        // -----------------------------------------------------------------------
        // Statistics
        // -----------------------------------------------------------------------

        // BMC
        @statistic[EbestModuleID](record=vector?);

        // CurrentDS
        @statistic[offsetFromMaster](record=stats,vector?);
        @statistic[offsetFromMaster_raw](record=stats,vector?);
        @statistic[stepsRemoved](record=stats,vector?);
        @statistic[meanPathDelay](record=stats,vector?);
        @statistic[meanPathDelay_raw](record=stats,vector?);

        // ParentDS

        // Ports
        @statisticTemplate[peerMeanPathDelay](record=vector?);
        @statisticTemplate[peerMeanPathDelay_raw](record=vector?);
        @statisticTemplate[portState](enum="INITIALIZING=1,FAULTY=2,DISABLED=3,LISTENING=4,PRE_MASTER=5,MASTER=6,PASSIVE=7,UNCALIBRATED=8,SLAVE=9";record=vector?);

        @statisticTemplate[AnnounceRcvd](record=vector?);
        @statisticTemplate[SyncRcvd](record=vector?);
        @statisticTemplate[SyncFuRcvd](record=vector?);
        @statisticTemplate[DelReqRcvd](record=vector?);
        @statisticTemplate[DelRespRcvd](record=vector?);
        @statisticTemplate[PDelReqRcvd](record=vector?);
        @statisticTemplate[PDelRespRcvd](record=vector?);
        @statisticTemplate[PDelRespFuRcvd](record=vector?);

        @statisticTemplate[ErbestModuleID](record=vector?);
        @statisticTemplate[StateDecision](enum="PORT_SD_LIST=10,PORT_SD_M1=43,PORT_SD_M2=42,PORT_SD_M3=41,PORT_SD_P1=22,PORT_SD_P2=21,PORT_SD_S1=31";record=vector?);
        @statisticTemplate[ForeignMasterDS_Cnt](record=vector?);

    gates:

        input   PtpIn[];  // PTP traffic
        output  PtpOut[];

        output  PortConfig[];
        input   PortRequest[];
}
