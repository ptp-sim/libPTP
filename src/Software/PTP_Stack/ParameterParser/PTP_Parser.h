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

#ifndef LIBPTP_PTP_PARSER_H_
#define LIBPTP_PTP_PARSER_H_

// ======================================================
// Includes
// ======================================================

#include "PTP.h"

// ======================================================
// Types
// ======================================================

class cPTP_Parser
{
    public:

        static      PTP_ClockType_t     ParsePtpClockType               (const char *Str);
        static      PTP_Profile_t       ParsePtpProfile                 (const char *Str);
        static      PTP_BMCA_t          ParseBestMasterClockAlgorithm   (const char *Str);
        static      PTP_MgmtProtocol_t  ParseManagementProtocol         (const char *Str);

        static      ClockClass_t        ParseClockClass                 (const char *Str);
        static      clockAccuracy_t     ParseClockAccuracy              (const char *Str);
        static      timeSource_t        ParseTimeSource                 (const char *Str);
        static      delayMechanism_t    ParseDelayMechanism             (const char *Str);
        static      domainNumber_t      ParseDomainNumber               (const char *Str);

        static      int8_t              ParseParameterInt8 ( cComponent *pComp, const char *ParName );
        static      uint8_t             ParseParameterUInt8( cComponent *pComp, const char *ParName );

        // Port specific parameters
        static      int                 ParsePortParameter          (int PortNumber, cComponent *pComp, const char *ParName);
        static      int8_t              ParsePortParameterInt8      (int PortNumber, cComponent *pComp, const char *ParName);
        static      uint8_t             ParsePortParameterUInt8     (int PortNumber, cComponent *pComp, const char *ParName);
        static      delayMechanism_t    ParsePortParameterDelayMech (int PortNumber, cComponent *pComp );
};

// ======================================================
// Declarations
// ======================================================

#endif

