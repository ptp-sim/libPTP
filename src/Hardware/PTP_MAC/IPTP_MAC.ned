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

import inet.linklayer.IEtherMAC;

moduleinterface IPTP_MAC extends IEtherMAC
{
    parameters:

        @display("i=PTP/Components/MAC/MAC");
        @class(PTP_MAC);

        // PTP configuration
        bool        PTP_Enable;
        string      PTP_ClockType;
        bool        PTP_TwoStepFlag;
        string      PTP_DelayMechanism;

        // Hardware setup
        string      PTP_NIC_CtrlPath;
        string      ClockPath;

        // Debug configuration
        bool        EnableDebugOutput;

        // Fault simulation
        bool        SimulateFault;
        double      FaultTime @unit(s);
        double      FaultDuration @unit(s);

    gates:
}

