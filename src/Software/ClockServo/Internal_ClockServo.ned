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

package libptp.Software.ClockServo;

simple Internal_ClockServo like IClockServo
{
    parameters:

        @display("i=PTP/Components/InternalModule/InternalModule");

        // Control configuration
        bool        EnableSyntonize                 = default( false );
        bool        EnableTimeJump                  = default( true  );
        bool        EnableSynchronize               = default( true  );
        int         FrequEstShift                   = default( 4     );
        double      MaxFrequEstInterval @unit(s)    = default( 10s   );
        double      OffsetThreshForReset @unit(s)   = default( 100us );

        // Debug
        bool        EnableDebugOutput               = default( false );

        // -----------------------------------------------------------------------
        // Signals
        // -----------------------------------------------------------------------
        @signal[ClockServoState]    (type=long);
        @signal[SyncInterval]       (type=simtime_t);
        @signal[OffsetFromMaster]   (type=double);

        @signal[Decision_EnableJump]        (type=bool);
        @signal[Decision_JumpDelta]         (type=simtime_t);
        @signal[Decision_EnableScale]       (type=bool);
        @signal[Decision_ScaleFactor_ppb]   (type=long);

        // -----------------------------------------------------------------------
        // Statistics
        // -----------------------------------------------------------------------
        @statistic[ClockServoState](enum="DISABLED=0,INIT=10,SYNTONIZE_INIT=20,SYNTONIZE_START=21,SYNTONIZE=22,JUMP=30,SCALE=40";record=vector?);
        @statistic[SyncInterval](record=vector?);
        @statistic[OffsetFromMaster](record=vector?);

        @statistic[Decision_EnableJump]     (record=vector?);
        @statistic[Decision_JumpDelta]      (record=vector?);
        @statistic[Decision_EnableScale]    (record=vector?);
        @statistic[Decision_ScaleFactor_ppb](record=vector?);
}

