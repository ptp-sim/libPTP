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

#ifndef LIBPTP_PTP_BYTE_BUFFERS_H_
#define LIBPTP_PTP_BYTE_BUFFERS_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_PrimitiveDataTypes.h"

// ======================================================
// Declarations
// ======================================================

// ======================================================
// Types
// ======================================================

struct __attribute__((packed)) BufPtpClockId_t
{
    Octet               data[8];
};

struct __attribute__((packed)) BufPtpPortId_t
{
    BufPtpClockId_t     clockIdentity;
    UInteger16          portNumber;
};

struct __attribute__((packed)) BufPtpTimeStamp_t
{
    Octet               seconds[6];
    UInteger32          nanoseconds;
};

struct __attribute__((packed)) BufPtpClockQual_t
{
    UInteger8           clockClass;
    Enumeration8        clockAccuracy;
    UInteger16          offsetScaledLogVariance;
};

struct __attribute__((packed)) BufPtpHeader_t
{
    UInteger8           transportSpecific_messageType;
    UInteger8           reserved_versionPTP;
    UInteger16          messageLength;
    UInteger8           domainNumber;
    Octet               reserved_1;
    Octet               flagField[2];
    Integer64           correctionField;
    Octet               reserved_2[4];
    BufPtpPortId_t      sourcePortIdentity;
    UInteger16          sequenceId;
    UInteger8           controlField;
    Integer8            logMessageInterval;
};

struct __attribute__((packed)) BufPtpAnnounce_t
{
    BufPtpTimeStamp_t   originTimestamp;
    Integer16           currentUtcOffset;
    Octet               reserved_0;
    UInteger8           grandmasterPriority1;
    BufPtpClockQual_t   clockQuality;
    UInteger8           grandmasterPriority2;
    BufPtpClockId_t     grandmasterIdentity;
    UInteger16          stepsRemoved;
    Enumeration8        timeSource;
};

struct __attribute__((packed)) BufPtpSync_t
{
    BufPtpTimeStamp_t   originTimestamp;
};

struct __attribute__((packed)) BufPtpFollowUp_t
{
    BufPtpTimeStamp_t   preciseOriginTimestamp;
};

struct __attribute__((packed)) BufPtpDelayReq_t
{
    BufPtpTimeStamp_t   originTimestamp;
};

struct __attribute__((packed)) BufPtpDelayResp_t
{
    BufPtpTimeStamp_t   receiveTimestamp;
    BufPtpPortId_t      requestingPortIdentity;
};

struct __attribute__((packed)) BufPtpPDelayReq_t
{
    BufPtpTimeStamp_t   originTimestamp;
    Octet               reserved[10];
}
;

struct __attribute__((packed)) BufPtpPDelayResp_t
{
    BufPtpTimeStamp_t   requestReceiptTimestamp;
    BufPtpPortId_t      requestingPortIdentity;
};

struct __attribute__((packed)) BufPtpPDelayRespFU_t
{
    BufPtpTimeStamp_t   responseOriginTimestamp;
    BufPtpPortId_t      requestingPortIdentity;
};

struct __attribute__((packed)) BufPtpData_t
{
    BufPtpHeader_t      Header;
    union
    {
        BufPtpAnnounce_t        Announce;
        BufPtpSync_t            Sync;
        BufPtpFollowUp_t        FollowUp;
        BufPtpDelayReq_t        DelayReq;
        BufPtpDelayResp_t       DelayResp;
        BufPtpPDelayReq_t       PDelayReq;
        BufPtpPDelayResp_t      PDelayResp;
        BufPtpPDelayRespFU_t    PDelayRespFU;
    }
    PtpFrame;
};

#endif
