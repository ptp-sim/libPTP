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

package libptp.Components.Nodes;

// ======================================================
// Imports
// ======================================================

import libptp.Components.Nodes.InternalNodes.*;

// ======================================================
// Channel declarations
// ======================================================

// ======================================================
// Network declarations
// ======================================================

module PTP_EN_E2E_1S_SO extends PTP_Internal_Node_SO
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_E_1_SO");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = false;
        PTP_DelayMechanism  = "DELAY_MECH_E2E";

    gates:
        ethg[1];
}

module PTP_EN_E2E_1S_M1 extends PTP_Internal_Node_M1
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_E_1_M1");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = false;
        PTP_DelayMechanism  = "DELAY_MECH_E2E";

    gates:
        ethg[1];
}

module PTP_EN_E2E_1S_M2 extends PTP_Internal_Node_M2
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_E_1_M2");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = false;
        PTP_DelayMechanism  = "DELAY_MECH_E2E";

    gates:
        ethg[1];
}

module PTP_EN_E2E_1S_M3 extends PTP_Internal_Node_M3
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_E_1_M3");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = false;
        PTP_DelayMechanism  = "DELAY_MECH_E2E";

    gates:
        ethg[1];
}

module PTP_EN_E2E_2S_SO extends PTP_Internal_Node_SO
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_E_2_SO");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = true;
        PTP_DelayMechanism  = "DELAY_MECH_E2E";

    gates:
        ethg[1];
}

module PTP_EN_E2E_2S_M1 extends PTP_Internal_Node_M1
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_E_2_M1");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = true;
        PTP_DelayMechanism  = "DELAY_MECH_E2E";

    gates:
        ethg[1];
}

module PTP_EN_E2E_2S_M2 extends PTP_Internal_Node_M2
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_E_2_M2");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = true;
        PTP_DelayMechanism  = "DELAY_MECH_E2E";

    gates:
        ethg[1];
}

module PTP_EN_E2E_2S_M3 extends PTP_Internal_Node_M3
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_E_2_M3");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = true;
        PTP_DelayMechanism  = "DELAY_MECH_E2E";

    gates:
        ethg[1];
}

module PTP_EN_P2P_1S_SO extends PTP_Internal_Node_SO
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_P_1_SO");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = false;
        PTP_DelayMechanism  = "DELAY_MECH_P2P";

    gates:
        ethg[1];
}

module PTP_EN_P2P_1S_M1 extends PTP_Internal_Node_M1
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_P_1_M1");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = false;
        PTP_DelayMechanism  = "DELAY_MECH_P2P";

    gates:
        ethg[1];
}

module PTP_EN_P2P_1S_M2 extends PTP_Internal_Node_M2
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_P_1_M2");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = false;
        PTP_DelayMechanism  = "DELAY_MECH_P2P";

    gates:
        ethg[1];
}

module PTP_EN_P2P_1S_M3 extends PTP_Internal_Node_M3
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_P_1_M3");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = false;
        PTP_DelayMechanism  = "DELAY_MECH_P2P";

    gates:
        ethg[1];
}

module PTP_EN_P2P_2S_SO extends PTP_Internal_Node_SO
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_P_2_SO");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = true;
        PTP_DelayMechanism  = "DELAY_MECH_P2P";

    gates:
        ethg[1];
}

module PTP_EN_P2P_2S_M1 extends PTP_Internal_Node_M1
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_P_2_M1");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = true;
        PTP_DelayMechanism  = "DELAY_MECH_P2P";

    gates:
        ethg[1];
}

module PTP_EN_P2P_2S_M2 extends PTP_Internal_Node_M2
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_P_2_M2");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = true;
        PTP_DelayMechanism  = "DELAY_MECH_P2P";

    gates:
        ethg[1];
}

module PTP_EN_P2P_2S_M3 extends PTP_Internal_Node_M3
{
    parameters:
        @display("i=PTP/Nodes/PTP_Nodes/N_P_2_M3");

        PTP_ClockType       = "PTP_CLOCK_TYPE_ORDINARY";
        PTP_TwoStepFlag     = true;
        PTP_DelayMechanism  = "DELAY_MECH_P2P";

    gates:
        ethg[1];
}
