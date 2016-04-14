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

package libptp.Components.Nodes.InternalNodes;

// ======================================================
// Imports
// ======================================================

import libptp.Components.BasicBlocks.PTP_BasicNode;

// ======================================================
// Channel declarations
// ======================================================

// ======================================================
// Network declarations
// ======================================================

// ------------------------------------------------------
// Base class for internal nodes
// ------------------------------------------------------

module PTP_Internal_Node extends PTP_BasicNode
{
    parameters:

        @display("i=PTP/Components/InternalModule/InternalModule");

    submodules:

    connections:
}

// ------------------------------------------------------
// Slave/Master capabilities
// ------------------------------------------------------

module PTP_Internal_Node_SO extends PTP_Internal_Node
{
    parameters:

        PTP_Stack.SlaveOnly     = true;

        PTP_Stack.Priority1     = default(128);
        PTP_Stack.ClockClass    = default("CLOCK_CLASS_SLAVE_ONLY");
        PTP_Stack.ClockAccuracy = default("CLOCK_ACCURACY_UNKNOWN");
}

module PTP_Internal_Node_M3 extends PTP_Internal_Node
{
    parameters:

        PTP_Stack.SlaveOnly     = false;

        PTP_Stack.ClockClass    = default("CLOCK_CLASS_DEFAULT");
        PTP_Stack.ClockAccuracy = default("CLOCK_ACCURACY_1_S");
}

module PTP_Internal_Node_M2 extends PTP_Internal_Node
{
    parameters:

        PTP_Stack.SlaveOnly     = false;

        PTP_Stack.ClockClass    = default("CLOCK_CLASS_DEFAULT");
        PTP_Stack.ClockAccuracy = default("CLOCK_ACCURACY_1_US");
}

module PTP_Internal_Node_M1 extends PTP_Internal_Node
{
    parameters:

        PTP_Stack.SlaveOnly     = false;

        PTP_Stack.ClockClass    = default("CLOCK_CLASS_PRIMARY");
        PTP_Stack.ClockAccuracy = default("CLOCK_ACCURACY_25_NS");
}
