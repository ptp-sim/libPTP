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

// ======================================================
// Package description
// ======================================================

package libptp.Hardware.PTP_NIC_Ctrl;

// ======================================================
// Imports
// ======================================================

// ======================================================
// Channel declarations
// ======================================================

// ======================================================
// Network declarations
// ======================================================

simple PTP_NIC_Ctrl
{
    parameters:

        @display("i=PTP/Components/NIC_Ctrl/NIC_Ctrl");

        // -----------------------------------------------------------------------
        // Signals
        // -----------------------------------------------------------------------
        @signal[PushSync](type=unsigned long);
        @signal[PopSync](type=unsigned long);

        @signal[PushDelayReqResp](type=unsigned long);
        @signal[PopDelayReqResp](type=unsigned long);

        @signal[PushPDelayReqResp](type=unsigned long);
        @signal[PopPDelayReqResp](type=unsigned long);

        @signal[PushPDelayRespFu](type=unsigned long);
        @signal[PopPDelayRespFu](type=unsigned long);

        // -----------------------------------------------------------------------
        // Statistics
        // -----------------------------------------------------------------------
        @statistic[PushSync](record=vector?);
        @statistic[PopSync](record=vector?);

        @statistic[PushDelayReqResp](record=vector?);
        @statistic[PopDelayReqResp](record=vector?);

        @statistic[PushPDelayReqResp](record=vector?);
        @statistic[PopPDelayReqResp](record=vector?);

        @statistic[PushPDelayRespFu](record=vector?);
        @statistic[PopPDelayRespFu](record=vector?);

    gates:

        input PortConfig[];
        output PortRequest[];
}
