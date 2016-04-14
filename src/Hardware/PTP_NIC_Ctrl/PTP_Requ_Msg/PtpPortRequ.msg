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
#include "PTPv2_m.h"
}};

// =================================================
// Class announcements
// =================================================

class noncobject PTPv2_SyncFrame;
class noncobject PTPv2_PDelay_RespFrame;

// =================================================
// Type definitions
// =================================================

enum tPtpPortRequ
{
    PTP_PORT_REQU_TYPE_TRIG_SFU     = 0;
    PTP_PORT_REQU_TYPE_TRIG_PDFU    = 1;
    PTP_PORT_REQU_TYPE_FAULT        = 2;
    PTP_PORT_REQU_TYPE_RECOVERY     = 3;
};

// =================================================
// Message definitions
// =================================================

message PtpPortRequ
{
    int     Type    @enum(tPtpPortRequ);
}

message PtpPortRequ_TrigSyncFU extends PtpPortRequ
{
    PTPv2_SyncFrame     SyncFrame;
}

message PtpPortRequ_TrigPDelayFU extends PtpPortRequ
{
    PTPv2_PDelay_RespFrame  PDelayFrame;
}
