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

package libptp.Hardware.HwClock.ScheduleClock;

import libptp.Hardware.HwClock.AdjustableClock.IPerfectAdjustableClock;
import libptp.Hardware.HwClock.AdjustableClock.IConstantDriftAdjustableClock;
import libptp.Hardware.HwClock.AdjustableClock.ISineAdjustableClock;
import libptp.Hardware.HwClock.AdjustableClock.ILibPLN_AdjustableClock;

simple PerfectScheduleClock extends Internal_ScheduleClock like IScheduleClock, IPerfectAdjustableClock
{
    parameters:

        @display("i=PTP/Components/Clock/PerfectClock");

        // Time Deviation configuration
        TdGenType = "TDGEN_TYPE_PERFECT";

        // HwClockConfig
        InitialPhaseNom = default(0.0);
}

simple ConstantDriftScheduleClock extends Internal_ScheduleClock like IScheduleClock, IConstantDriftAdjustableClock
{
    parameters:

        @display("i=PTP/Components/Clock/ConstantDriftClock");

        // Time Deviation configuration
        TdGenType = "TDGEN_TYPE_CONST_DRIFT";

        double ConstDriftTdGen_k = default(0.5);
}

simple SineScheduleClock extends Internal_ScheduleClock like IScheduleClock, ISineAdjustableClock
{
    parameters:

        @display("i=PTP/Components/Clock/SineClock");

        // Time Deviation configuration
        TdGenType = "TDGEN_TYPE_SINE";

        double SineTdGen_f = default(0.01);
        double SineTdGen_A = default(-1.0);
        double SineTdGen_phi = default(0.0);
}

simple LibPLN_ScheduleClock extends Internal_ScheduleClock like IScheduleClock, ILibPLN_AdjustableClock
{
    parameters:

        @display("i=PTP/Components/Clock/RealClock");

        // Time Deviation configuration
        TdGenType = "TDGEN_TYPE_LIBPLN";

        int     libPLN_TdGen_Seed           = default(0);
        bool    libPLN_TdGen_AllowSkipping  = default(true);
        string  libPLN_TdGen_Example        = default("AVG_OSC");
}
