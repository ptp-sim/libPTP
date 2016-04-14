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

package libptp.Hardware.EtherPhy;

import libptp.Hardware.EtherPhy.IEtherPhy;

import ned.DelayChannel;
import utils.Channels.VolatileDelayChannel.VolatileDelayChannel;

module EtherPhy like IEtherPhy
{
    parameters:
        // The Rx delay is added to incoming frames (phy -> mii).
        volatile double rxDelay @unit(s) = default(176ns + intuniform(0,4) * 8ns);

        // The Tx delay is added to outgoing frames (mii -> phy).
        volatile double txDelay @unit(s) = default(76ns + intuniform(0,1) * 8ns);

        @display("i=PTP/Components/PHY/PHY");

    gates:
        inout mii @labels(EtherFrame); // Media Independent Interface to MAC
        inout phy @labels(EtherFrame); // Phy interface to network

    connections:

        mii$i --> VolatileDelayChannel{ delay = txDelay; } --> phy$o;
        mii$o <-- VolatileDelayChannel{ delay = rxDelay; } <-- phy$i;
}
