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

package libptp.Firmware.EthernetII_LLC;

// ======================================================
// Imports
// ======================================================

// ======================================================
// Channel declarations
// ======================================================

// ======================================================
// Network declarations
// ======================================================

simple EthernetII_LLC
{
    parameters:
        @display("i=block/fork_l");

        string  EtherTypeMapping = default("");

    gates:

        input upperLayerIn[] @labels(Ieee802Ctrl/down);
        output upperLayerOut[] @labels(Ieee802Ctrl/up);

        input   lowerLayerIn @labels(Ieee802Ctrl/up);
        output  lowerLayerOut @labels(Ieee802Ctrl/down);
}
