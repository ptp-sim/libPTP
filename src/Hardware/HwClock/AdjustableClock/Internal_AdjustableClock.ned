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

package libptp.Hardware.HwClock.AdjustableClock;

import libptp.Hardware.HwClock.HwClock.Internal_HwClock;

simple Internal_AdjustableClock extends Internal_HwClock like IAdjustableClock
{
    parameters:

        @display("i=PTP/Components/InternalModule/InternalModule");
        @class("cAdjustableClock");

        // Clock configuration
        bool    EnableAdjustments               = default(true);
        int     ScaleFactor_LowerBound_ppb      = default(-32767999);   // Limit value taken from Intel i210 data sheet
        int     ScaleFactor_UpperBound_ppb      = default(32767999);
        double  BeginScaledTime@unit(s)         = default(uniform(0ms,5ms));

        // -----------------------------------------------------------------------
        // Signals
        // -----------------------------------------------------------------------
        @signal[ScaledTime](type=double);
        @signal[ScaleFactor_ppb](type=long);

        // -----------------------------------------------------------------------
        // Statistics
        // -----------------------------------------------------------------------
        @statistic[ScaledTime](record=vector?);
        @statistic[ScaleFactor_ppb](record=stats?,vector?);
}
