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
package libptp.Components.Cables;

// ======================================================
// Imports
// ======================================================

import inet.nodes.ethernet.Eth1G;

// ======================================================
// Channel declarations
// ======================================================

channel GigabitCable extends Eth1G
{
}

channel GigabitCable20cm extends Eth1G
{
    parameters:

        length  = 0.2m;
}

channel GigabitCable1m extends Eth1G
{
    parameters:

        length  = 1m;
}

channel GigabitCable10m extends Eth1G
{
    parameters:

        length  = 10m;
}

channel GigabitCable100m extends Eth1G
{
    parameters:

        length  = 100m;
}

channel GigabitCable1000m extends Eth1G
{
    parameters:

        length  = 1000m;
}

// ======================================================
// Network declarations
// ======================================================

