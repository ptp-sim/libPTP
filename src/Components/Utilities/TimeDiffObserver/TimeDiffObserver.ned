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

package libptp.Components.Utilities.TimeDiffObserver;

simple TimeDiffObserver
{
    parameters:

        @display("i=PTP/Nodes/TimeDiffObserver/TimeDiffObserver");
        @class(cTimeDiffObserver);

        // Hardware configuration
        string      ClockPath1 = default( "^.Clock1" );
        string      ClockPath2 = default( "^.Clock2" );

        // Trace configuration
        double      TraceInterval @unit(s)  = default(0);       // 0 -> tracing disabled

        // GUI configuration
        int         GuiExponent = default(-6);
        bool        AutomaticGuiExponent = default(true);

        // -----------------------------------------------------------------------
        // Signals
        // -----------------------------------------------------------------------
        @signal[HwTimeDiff](type=simtime_t);
        @signal[ScaledTimeDiff](type=double);

        // -----------------------------------------------------------------------
        // Statistics
        // -----------------------------------------------------------------------
        @statistic[HwTimeDiff](record=stats?,vector?);
        @statistic[ScaledTimeDiff](record=stats?,vector?);
}
