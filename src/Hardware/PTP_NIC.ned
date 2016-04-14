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

package libptp.Hardware;

// ======================================================
// Imports
// ======================================================

import libptp.Hardware.HwClock.ScheduleClock.IScheduleClock;
import libptp.Hardware.PTP_RelayUnit.IPTP_MACRelayUnit;
import libptp.Hardware.PTP_RelayUnit.PTP_MACRelayUnit;
import libptp.Hardware.PTP_MAC.PTP_MAC;
import libptp.Hardware.PTP_MAC.IPTP_MAC;
import libptp.Hardware.PTP_NIC_Ctrl.*;
import libptp.Hardware.PTP_EtherEncap.IPTP_EtherEncap;
import libptp.Hardware.PTP_EtherEncap.PTP_EtherEncap;
import libptp.Hardware.PTP_EtherEncap.*;
import libptp.Hardware.DualDelayer.IDualDelayer;
import libptp.Hardware.DualDelayer.DualDelayer;
import libptp.Hardware.EtherPhy.IEtherPhy;
import libptp.Hardware.EtherPhy.EtherPhy;

import inet.nodes.ethernet.Eth1G;

import inet.networklayer.common.InterfaceTable;
import inet.base.NotificationBoard;
import inet.linklayer.IMACAddressTable;

// ======================================================
// Channel declarations
// ======================================================

// ======================================================
// Network declarations
// ======================================================

module PTP_NIC
{
    parameters:
        @display("i=PTP/Components/NIC/NIC");
        @labels(node,ethernet-node);
        @node();

        string PTP_ClockType = default("PTP_CLOCK_TYPE_ORDINARY");
        bool PTP_TwoStepFlag = default(false);
        string PTP_DelayMechanism = default("DELAY_MECH_E2E");

        string EncapType = default("PTP_EtherEncap");
        string RelayUnitType = default("PTP_MACRelayUnit");
        string DelayerType = default("DualDelayer");
        string MACType = default("PTP_MAC");
        string EtherPhyType = default("EtherPhy");
        string ClockType = default("PerfectScheduleClock");
        string MacTableType = default("MACAddressTable");

    gates:
        input upperLayerIn @labels(Ieee802Ctrl/down);
        output upperLayerOut @labels(Ieee802Ctrl/up);

        input PortConfig[];
        output PortRequest[];

        inout ethg[] @labels(EtherFrame-conn);

    submodules:

        PTP_NIC_Ctrl: PTP_NIC_Ctrl {

            parameters:
                @display("p=100,70");
        }

        Clock: <ClockType> like IScheduleClock {
            parameters:
                @display("p=100,180");
        }

        interfaceTable: InterfaceTable {
            parameters:
                @display("p=100,350;is=s");
        }

        notificationBoard: NotificationBoard {
            parameters:
                @display("p=100,400;is=s");
        }

        macTable: <MacTableType> like IMACAddressTable {
            parameters:
                @display("p=100,450;is=s");
        }

        Encap: <EncapType> like IPTP_EtherEncap {
            parameters:
                @display("p=250,70");
        }

        RelayUnit: <RelayUnitType> like IPTP_MACRelayUnit {

            parameters:
                @display("p=250,180");

                internalPort = 0;

                PTP_ClockType = PTP_ClockType;

            gates:
                ifIn[sizeof(ethg)+1];       // Add 1 additional gate for App
                ifOut[sizeof(ethg)+1];      // Add 1 additional gate for App
        }

        Delayer[sizeof(ethg)]: <DelayerType> like IDualDelayer {
            parameters:
                @display("p=250,290,r,110");

                UpDelay     = default(uniform(1us, 10us));
                DownDelay   = default(uniform(1us, 10us));

                EnableUpDelay   = default(true);
                EnableDownDelay = default(true);
        }

        MAC[sizeof(ethg)]: <MACType> like IPTP_MAC {

            parameters:

                @display("p=250,400,r,110");

                promiscuous = true;
                queueModule = "";

                PTP_ClockType = PTP_ClockType;
                PTP_TwoStepFlag = PTP_TwoStepFlag;
                PTP_DelayMechanism = PTP_DelayMechanism;
        }

        PHY[sizeof(ethg)]: <EtherPhyType> like IEtherPhy {
            parameters:
                @display("p=250,510,r,110");
        }

    connections allowunconnected:

        upperLayerIn --> Encap.upperLayerIn;
        upperLayerOut <-- Encap.upperLayerOut;

        Encap.lowerLayerIn <-- RelayUnit.ifOut[0];
        Encap.lowerLayerOut --> RelayUnit.ifIn[0];

        for i=0..sizeof(ethg)-1 {
            RelayUnit.ifOut[i+1] --> Delayer[i].upperLayerIn;
            Delayer[i].lowerLayerOut --> MAC[i].upperLayerIn;

            MAC[i].upperLayerOut --> Delayer[i].lowerLayerIn;
            Delayer[i].upperLayerOut --> RelayUnit.ifIn[i+1];

            MAC[i].phys <--> PHY[i].mii;
            PHY[i].phy <--> ethg[i];

            PortConfig[i] --> PTP_NIC_Ctrl.PortConfig++;
            PortRequest[i] <-- PTP_NIC_Ctrl.PortRequest++;
        }
}
