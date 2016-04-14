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

package libptp.Testbenches.PtpMacSink;

import libptp.Testbenches.PTP_EthSink.PTP_EthSink;
import inet.networklayer.common.InterfaceTable;
import inet.base.NotificationBoard;

import libptp.Hardware.*.*;
import libptp.Hardware.HwClock.ScheduleClock.IScheduleClock;

module PtpMacSink
{
    parameters:

        string ClockType = default("ConstantDriftScheduleClock");
        string MAC_Address  = default( "C0:FF:EE:BA:D0:1D" );

        string      PTP_ClockType       = default("PTP_CLOCK_TYPE_ORDINARY");
        bool        PTP_TwoStepFlag     = default(false);
        string      PTP_DelayMechanism  = default("DELAY_MECH_E2E");

        @display("i=block/sink");
        @node;

    gates:

        inout ethg[] @labels(EtherFrame-conn);

    submodules:

        interfaceTable: InterfaceTable {
            @display("p=175,312;is=s");
        }

        notificationBoard: NotificationBoard {
            parameters:
                @display("p=284,312;is=s");
        }

        Clock: <ClockType> like IScheduleClock {
            parameters:
                @display("p=175,235");
        }

        PTP_NIC_Ctrl: PTP_NIC_Ctrl {
            @display("p=175,133");
        }

        MAC: PTP_MAC {

            parameters:
                @display("p=84,235");

                address = MAC_Address;
                PTP_ClockType = PTP_ClockType;
                PTP_TwoStepFlag = PTP_TwoStepFlag;
                PTP_DelayMechanism = PTP_DelayMechanism;
        }

        Sink: PTP_EthSink {
            @display("p=84,133");
        }

    connections allowunconnected:

        ethg[0] <--> MAC.phys;
        MAC.upperLayerOut --> Sink.lowerLayerIn;
}
