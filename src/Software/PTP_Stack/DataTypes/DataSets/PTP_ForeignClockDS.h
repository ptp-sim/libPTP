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

#ifndef LIBPTP_PTP_FOREIGN_CLOCK_DS_H_
#define LIBPTP_PTP_FOREIGN_CLOCK_DS_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_PrimitiveDataTypes.h"
#include "PTP_ClockIdentity.h"
#include "PTP_ClockQuality.h"
#include "PTP_PortIdentity.h"
#include "PTP_DefaultDS.h"

#include "PTPv2_m.h"

// ======================================================
// Types
// ======================================================

enum class ClockCompResult
{
    A_BETTER_B,
    A_BETTER_B_BY_TOPO,
    B_BETTER_A,
    B_BETTER_A_BY_TOPO,
    ERROR_1,
    ERROR_2,
    ERROR_INVALID,
};

enum class ClockCompReason
{
    NONE,
    OTHER_EMPTY,
    STEPS_REMOVED,
    RCV_PORT_ID_SMALLER,
    RCV_PORT_ID_GREATER,
    SND_PORT_ID,
    RCV_PORT_NUMBER,
    PRIORITY1,
    CLOCK_CLASS,
    CLOCK_ACCURACY,
    OFFSET_LOG_VAR,
    PRIORITY2,
    CLOCK_ID,
};

struct ClockCompReturn_t
{
    ClockCompResult     Result;
    ClockCompReason     Reason;
};

// ======================================================
// Declarations
// ======================================================

std::ostream&   operator<<(std::ostream& os, const ClockCompResult& o);
std::ostream&   operator<<(std::ostream& os, const ClockCompReason& o);

class cForeignClockDS
{
    private:
        bool                Empty;

        UInteger8           Priority1;
        cClockIdentity      ClockId;
        cClockQuality       ClockQual;
        UInteger8           Priority2;
        UInteger16          StepsRemoved;
        cPortIdentity       SenderPortId;
        cPortIdentity       ReceiverPortId;

        int                 ModuleID;

        cPtpHeaderFlags     flagField;
        uint16_t            currentUtcOffset;
        timeSource_t        timeSource;

    protected:

    public:

        // Constructors/Destructor
        cForeignClockDS();
        cForeignClockDS(const cDefaultDS& defaultDS);
        cForeignClockDS(const PTPv2_AnnounceFrame *pAnn);
        cForeignClockDS(const cForeignClockDS& other);
        ~cForeignClockDS();

        // Instance methods
        cClockIdentity&             ClockIdentity();
        cClockQuality&              ClockQuality();
        cPortIdentity&              SenderPortIdentity();
        cPortIdentity&              ReceiverPortIdentity();
        cPtpHeaderFlags&            FlagsField();

        // Setters
        void                        SetPriority1( UInteger8 Priority1 );
        void                        SetPriority2( UInteger8 Priority2 );
        void                        SetStepsRemoved( UInteger16 StepsRemoved );

        // Getters
        UInteger8                   GetPriority1()          const;
        UInteger8                   GetPriority2()          const;
        UInteger16                  GetStepsRemoved()       const;
        timeSource_t                GetTimeSource()         const;
        UInteger16                  GetCurrentUtcOffset()   const;
        int                         GetModuleID()           const;

        // API functions
        void                        Clear();
        void                        Activate();
        bool                        IsEmpty()   const;
        static ClockCompReturn_t    CompareClockDS(const cForeignClockDS& A, const cForeignClockDS& B, bool EnableDebugOutput);

        // Operators
        bool                        operator== (const cForeignClockDS& other) const;
        bool                        operator!= (const cForeignClockDS& other) const;
        cForeignClockDS&            operator= (const cForeignClockDS& other);
        cForeignClockDS&            operator= (const PTPv2_AnnounceFrame *pAnn);
        cForeignClockDS&            operator= (const cDefaultDS& defaultDS);

        // Debug functions
        void                        Print() const;
};

#endif

