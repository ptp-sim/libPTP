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

package libptp.Testbenches.PtpMacSource;

import libptp.Testbenches.PtpFrameGen.PtpFrameGen;
import inet.networklayer.common.InterfaceTable;
import inet.base.NotificationBoard;

import libptp.Hardware.*.*;
import libptp.Software.*.*;

import libptp.Hardware.HwClock.ScheduleClock.IScheduleClock;

module PtpMacSource
{
    parameters:

        string ClockType = default("ConstantDriftScheduleClock");
        string MAC_Address = default("C0:FF:EE:BA:D0:1D");

        string      PTP_ClockType       = default("PTP_CLOCK_TYPE_ORDINARY");
        bool        PTP_TwoStepFlag     = default(false);
        string      PTP_DelayMechanism  = default("DELAY_MECH_E2E");

        @display("i=block/source");
        @node;

    gates:

        inout ethg[] @labels(EtherFrame-conn);

    submodules:

        interfaceTable: InterfaceTable {
            @display("p=191,410;is=s");
        }

        notificationBoard: NotificationBoard {
            parameters:
                @display("p=298,410;is=s");
        }

        Clock: <ClockType> like IScheduleClock {
            parameters:
                @display("p=191,333");
        }

        PTP_NIC_Ctrl: PTP_NIC_Ctrl {
            @display("p=191,231");
        }

        FrameSource: PtpFrameGen {

            parameters:

                @display("p=84,36");

                PTP_TwoStepFlag = PTP_TwoStepFlag;
        }

        EthMap: PTP_EthernetMapping {

            parameters:

                @display("p=84,127");

            gates:
                upperLayerIn[1];
                upperLayerOut[1];
        }

        Encap: PTP_EtherEncap {
            @display("p=84,231");
        }

        MAC: PTP_MAC {

            parameters:
                @display("p=84,333");

                address = MAC_Address;
                PTP_ClockType = PTP_ClockType;
                PTP_TwoStepFlag = PTP_TwoStepFlag;
                PTP_DelayMechanism = PTP_DelayMechanism;
        }

    connections allowunconnected:

        FrameSource.PtpOut --> EthMap.upperLayerIn[0];
        EthMap.lowerLayerOut --> Encap.upperLayerIn;
        Encap.lowerLayerOut --> MAC.upperLayerIn;
        ethg[0] <--> MAC.phys;

}
