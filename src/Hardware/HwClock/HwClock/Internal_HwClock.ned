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

package libptp.Hardware.HwClock.HwClock;

simple Internal_HwClock like IHwClock
{
    parameters:

        @display("i=PTP/Components/InternalModule/InternalModule");
        @class("cHwClock");

        // Clock configuration
        double  TickLenNom @unit(s)             = default(50ns);
        double  InitialPhaseNom                 = default(uniform(0.0,1.0));        // Initial phase offset, 0.0-1.0

        // Time Deviation configuration
        string  TdGenType = default("TDGEN_TYPE_PERFECT");

        // Trace configuration
        double  TimeTraceInterval  @unit(s)        = default(0);

        // -----------------------------------------------------------------------
        // Signals
        // -----------------------------------------------------------------------
        @signal[TimeDeviation](type=simtime_t);
        @signal[HwTime](type=simtime_t);
        @signal[RequestedHwTime](type=simtime_t);

        // -----------------------------------------------------------------------
        // Statistics
        // -----------------------------------------------------------------------
        @statistic[TimeDeviation](record=stats,vector?);
        @statistic[HwTime](record=vector?);
        @statistic[RequestedHwTime](record=vector?);
}
