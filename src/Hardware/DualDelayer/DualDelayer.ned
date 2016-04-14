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

package libptp.Hardware.DualDelayer;

import libptp.Hardware.DualDelayer.DelayQueue.*;

module DualDelayer like IDualDelayer
{
    parameters:

        @display("i=PTP/Components/Delayer/Delayer");

        volatile double UpDelay @unit(s) = default(0s);
        volatile double DownDelay @unit(s) = default(0s);

        bool EnableUpDelay = default(false);
        bool EnableDownDelay = default(false);

    gates:

        input upperLayerIn;
        output upperLayerOut;
        input lowerLayerIn;
        output lowerLayerOut;

    submodules:

        UpDelayer: DelayQueue if EnableUpDelay == true {
            parameters:
                Delay = UpDelay;
                @display("p=62,30");
        }

        DownDelayer: DelayQueue if EnableDownDelay == true {
            parameters:
                Delay = DownDelay;
                @display("p=62,76");
        }

    connections:

        if EnableDownDelay == true {
            upperLayerIn --> DownDelayer.in;
            DownDelayer.out --> lowerLayerOut;
        }
        if EnableDownDelay == false {
            upperLayerIn --> lowerLayerOut;
        }

        if EnableUpDelay == true {
            lowerLayerIn --> UpDelayer.in;
            UpDelayer.out --> upperLayerOut;
        }
        if EnableUpDelay == false {
            lowerLayerIn --> upperLayerOut;
        }
}
