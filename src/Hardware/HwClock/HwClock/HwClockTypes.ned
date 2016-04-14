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

package libptp.Hardware.HwClock.HwClock;

moduleinterface IPerfectHwClock extends IHwClock
{
    parameters:
        @display("i=PTP/Components/Clock/PerfectClock");
}

moduleinterface IConstantDriftHwClock extends IHwClock
{
    parameters:

        @display("i=PTP/Components/Clock/ConstantDriftClock");

        double  ConstDriftTdGen_k;
}

moduleinterface ISineHwClock extends IHwClock
{
    parameters:

        @display("i=PTP/Components/Clock/SineClock");

        double  SineTdGen_f;
        double  SineTdGen_A;
        double  SineTdGen_phi;
}

moduleinterface ILibPLN_HwClock extends IHwClock
{
    parameters:

        @display("i=PTP/Components/Clock/RealClock");

        int     libPLN_TdGen_Seed;
        bool    libPLN_TdGen_AllowSkipping;
}
