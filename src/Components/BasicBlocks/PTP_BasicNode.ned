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

package libptp.Components.BasicBlocks;

// ======================================================
// Imports
// ======================================================

import libptp.Software.PTP_Stack.*;
import libptp.Software.ClockServo.IClockServo;
import libptp.Software.ClockServo.*;
import libptp.Software.PTP_EthernetMapping.*;

import libptp.Firmware.*.*;
import libptp.Hardware.*;

// ======================================================
// Channel declarations
// ======================================================

// ======================================================
// Network declarations
// ======================================================

module PTP_BasicNode like IPTP_EtherNode
{
    parameters:

        @display("i=PTP/Nodes/PTP_Nodes/Generic");

        string PTP_ClockType = default("PTP_CLOCK_TYPE_ORDINARY");
        bool PTP_TwoStepFlag = default(false);
        string PTP_DelayMechanism = default("DELAY_MECH_E2E");

        string ClockServoType = default("PI_ClockServo");

    gates:

        inout ethg[] @labels(EtherFrame-conn);

    submodules:

        PTP_Stack: PTP_Stack {
            parameters:

                @display("p=174,52");

                PTP_ClockType = PTP_ClockType;
                PTP_TwoStepFlag = PTP_TwoStepFlag;
                PTP_DelayMechanism = PTP_DelayMechanism;

                ClockPath = default( "^.NIC.Clock" );

            gates:
                PtpIn[sizeof(ethg)];
                PtpOut[sizeof(ethg)];
                PortConfig[sizeof(ethg)];
                PortRequest[sizeof(ethg)];
        }

        ClockServo: <ClockServoType> like IClockServo {
            parameters:
                @display("p=60,52");
        }

        PTP_EthMap: PTP_EthernetMapping {
            parameters:
                @display("p=233,171");

            gates:
                upperLayerIn[sizeof(ethg)];
                upperLayerOut[sizeof(ethg)];
        }

        LLC: EthernetII_LLC {
            parameters:
                @display("p=233,292");

                EtherTypeMapping = default("35063:0");
        }

        NIC: PTP_NIC {
            parameters:
                @display("p=174,406");

                PTP_ClockType = PTP_ClockType;
                PTP_DelayMechanism = PTP_DelayMechanism;
                PTP_TwoStepFlag = PTP_TwoStepFlag;

            gates:
                ethg[sizeof(ethg)];
                PortConfig[sizeof(ethg)];
                PortRequest[sizeof(ethg)];
        }

    connections allowunconnected:

        for i=0..sizeof(ethg)-1 {
            PTP_Stack.PtpOut[i] --> PTP_EthMap.upperLayerIn[i];
            PTP_Stack.PtpIn[i] <-- PTP_EthMap.upperLayerOut[i];
        }

        PTP_EthMap.lowerLayerOut --> LLC.upperLayerIn++;
        PTP_EthMap.lowerLayerIn  <-- LLC.upperLayerOut++;

        LLC.lowerLayerOut --> NIC.upperLayerIn;
        LLC.lowerLayerIn <-- NIC.upperLayerOut;

        for i=0..sizeof(ethg)-1 {
            NIC.ethg[i] <--> ethg[i];

            NIC.PortConfig[i] <-- PTP_Stack.PortConfig[i];
            NIC.PortRequest[i] --> PTP_Stack.PortRequest[i];
        }
}
