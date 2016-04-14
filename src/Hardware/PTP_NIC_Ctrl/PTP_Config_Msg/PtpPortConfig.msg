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

// =================================================
// Make needed C++ stuff available
// =================================================

cplusplus {{
#include <simtime.h>
#include "MACAddress.h"
}};

// =================================================
// Class announcements
// =================================================

class noncobject simtime_t;
class noncobject MACAddress;

// =================================================
// Type definitions
// =================================================

enum tPtpNicConf
{
    PTP_NIC_CONF_TYPE_MAC           = 0;
};

enum tPtpPortConf
{
    PTP_PORT_CONF_TYPE_PATH_DELAY   = 0;
    PTP_PORT_CONF_TYPE_ASYMMETRY    = 1;
};

// =================================================
// Message definitions
// =================================================

message PtpNicConfig
{
    int     Type    @enum(tPtpNicConf);
}

message PtpNicConfig_MAC extends PtpNicConfig
{
    MACAddress  MAC;
}

message PtpPortConfig
{
    int     Type    @enum(tPtpPortConf);
}

message PtpPortConfig_PathDelay extends PtpPortConfig
{
    simtime_t   MeanPathDelay;
}

message PtpPortConfig_Asymmetry extends PtpPortConfig
{
    simtime_t   Asymmetry;
}
