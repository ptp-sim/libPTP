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

package libptp.Hardware.PTP_MAC;

import inet.linklayer.ethernet.EtherMAC;

simple PTP_MAC extends EtherMAC like IPTP_MAC
{
    parameters:

        @display("i=PTP/Components/MAC/MAC");
        @class(PTP_MAC);

        // PTP configuration
        bool        PTP_Enable          = default(true);
        string      PTP_ClockType       = default("PTP_CLOCK_TYPE_ORDINARY");
        bool        PTP_TwoStepFlag     = default(false);
        string      PTP_DelayMechanism  = default("DELAY_MECH_E2E");

        // Hardware setup
        string      PTP_NIC_CtrlPath    = default("^.PTP_NIC_Ctrl");
        string      ClockPath           = default("^.Clock");

        // Debug configuration
        bool        EnableDebugOutput   = default( false );

        // Fault simulation
        bool        SimulateFault               = default( false );
        double      FaultTime @unit(s)          = default( 0s );
        double      FaultDuration @unit(s)      = default( 1s );

    gates:
}

