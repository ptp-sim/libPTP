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
#include "PTP_HeaderFlags.h"
#include "PTP_PortIdentity.h"
#include "PTP_ClockIdentity.h"
#include "PTP_ClockQuality.h"
#include "PTP_TimeInterval.h"
#include "PTP_TimeStamp.h"
#include "PTP_PrimitiveDataTypes.h"
#include "LocalTimeStamp.h"
#include <simtime.h>
}};

// =================================================
// Class announcements
// =================================================

class noncobject Boolean;
class noncobject Enumeration4;
class noncobject Enumeration8;
class noncobject Enumeration16;
class noncobject Integer16;
class noncobject UInteger16;
class noncobject Integer32;
class noncobject UInteger32;
class noncobject Integer64;
class noncobject Octet;

class noncobject cPtpHeaderFlags;
class noncobject cPortIdentity;
class noncobject cClockIdentity;
class noncobject cClockQuality;
class noncobject cTimeInterval;
class noncobject cTimeStamp;
class noncobject tPtpMessageType;
class noncobject cLocalTimeStamp;

class noncobject simtime_t;

// =================================================
// Type definitions
// =================================================

// =================================================
// Message definitions
// =================================================

// ----------------------------------------------
// Basis PTP frame
// ----------------------------------------------
packet PTPv2_Frame
{
    // Internal house keeping stuff
    cLocalTimeStamp     IngressTimeStamp;

    // Generic Header
    uint8_t             transportSpecific;
    tPtpMessageType     MessageType;
    uint8_t             reserved_0;
    uint8_t             versionPTP;
    UInteger16          messageLength;
    uint8_t             domainNumber;
    uint8_t             reserved_1;
    cPtpHeaderFlags     flagField;
    cTimeInterval       correctionField;
    uint8_t             reserved_2 [4];
    cPortIdentity       sourcePortIdentity;
    UInteger16          sequenceId;
    uint8_t             controlField;
    int8_t              logMessageInterval;
}

// -------------------------------------------------
// Announce
// -------------------------------------------------
packet PTPv2_AnnounceFrame extends PTPv2_Frame
{
    string displayString = "i=PTP/Messages/Announce";

    cTimeStamp          originTimestamp;
    Integer16           currentUtcOffset;
    uint8_t             reserved_0;
    uint8_t             grandmasterPriority1;
    cClockQuality       grandmasterClockQuality;
    uint8_t             grandmasterPriority2;
    cClockIdentity      grandmasterIdentity;
    UInteger16          stepsRemoved;
    uint8_t             timeSource;

    // OMNeT only
    int                 GrandMasterModuleID;
}

// -------------------------------------------------
// Sync
// -------------------------------------------------
packet PTPv2_SyncFrame extends PTPv2_Frame
{
    string displayString = "i=PTP/Messages/Sync";

    cTimeStamp          originTimestamp;
}

// -------------------------------------------------
// Follow_Up
// -------------------------------------------------
packet PTPv2_Follow_UpFrame extends PTPv2_Frame
{
    string displayString = "i=PTP/Messages/SyncFU";

    cTimeStamp          preciseOriginTimestamp;
}

// -------------------------------------------------
// Delay_Req
// -------------------------------------------------
packet PTPv2_Delay_ReqFrame extends PTPv2_Frame
{
    string displayString = "i=PTP/Messages/DelayReq";

    cTimeStamp          originTimestamp;
}

// -------------------------------------------------
// Delay_Resp
// -------------------------------------------------
packet PTPv2_Delay_RespFrame extends PTPv2_Frame
{
    string displayString = "i=PTP/Messages/DelayResp";

    cTimeStamp          receiveTimestamp;
    cPortIdentity       requestingPortIdentity;

    // ------------------------------------
    simtime_t           ReqEgressTime; // Piggy back own Tx time back to app
}

// -------------------------------------------------
// PDelay_Req
// -------------------------------------------------
packet PTPv2_PDelay_ReqFrame extends PTPv2_Frame
{
    string displayString = "i=PTP/Messages/PDelayReq";

    cTimeStamp          originTimestamp;
    uint8_t             reservedDelayReq    [10];
}

// -------------------------------------------------
// PDelay_Resp
// -------------------------------------------------
packet PTPv2_PDelay_RespFrame extends PTPv2_Frame
{
    string displayString = "i=PTP/Messages/PDelayResp";

    cTimeStamp          requestReceiptTimestamp;
    cPortIdentity       requestingPortIdentity;

    // ------------------------------------
    simtime_t           ReqEgressTime;    // Piggy back own Tx time back to app
}

// -------------------------------------------------
// Pdelay_Resp_Follow_Up
// -------------------------------------------------
packet PTPv2_PDelay_Resp_FU_Frame extends PTPv2_Frame
{
    string displayString = "i=PTP/Messages/PDelayRespFU";

    cTimeStamp          responseOriginTimestamp;
    cPortIdentity       requestingPortIdentity;
}

// -------------------------------------------------
// Signaling
// -------------------------------------------------
packet PTPv2_SignalingFrame extends PTPv2_Frame
{
    string displayString = "i=PTP/Messages/Signaling";

    cPortIdentity       targetPortIdentity;
}

// -------------------------------------------------
// Management
// -------------------------------------------------
packet PTPv2_ManagementFrame extends PTPv2_Frame
{
    string displayString = "i=PTP/Messages/Management";

    cPortIdentity       targetPortIdentity;
}
