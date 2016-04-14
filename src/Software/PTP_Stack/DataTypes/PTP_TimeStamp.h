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

#ifndef LIBPTP_PTP_TIMESTAMP_H_
#define LIBPTP_PTP_TIMESTAMP_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "PTP_PrimitiveDataTypes.h"
#include "PTP_ByteBuffers.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

// Time stamp, see clause 5.3.3 in IEEE 1588-2008
class cTimeStamp
{
    private:
        UInteger48              secondsField;                   // [s]
        UInteger32              nanosecondsField;               // [ns], always < 10^9

    protected:

    public:

        // Constructors
        cTimeStamp();
        cTimeStamp( UInteger48  secondsField,
                    UInteger32  nanosecondsField );
        cTimeStamp( simtime_t   timestamp        );
        cTimeStamp( BufPtpTimeStamp_t *pBuf );

        // Setters
        void    Set(UInteger48  secondsField,
                    UInteger32  nanosecondsField );

        void        ReadFromBuffer( BufPtpTimeStamp_t *pBuf );

        // Getters
        simtime_t   GetSimTime();
        UInteger48  GetSeconds() const;
        UInteger32  GetNanoSeconds() const;
        std::string GetString() const;

        void        SaveToBuffer( BufPtpTimeStamp_t *pBuf );

        // Operators
        bool            operator== (const cTimeStamp& other);
        bool            operator== (const simtime_t& other);
        cTimeStamp&     operator= (const cTimeStamp& other);
        cTimeStamp&     operator= (const simtime_t& other);
        cTimeStamp&     operator= (const int64_t& other_ps);

        friend std::ostream& operator<<(std::ostream& os, const cTimeStamp& o);

        // Debug functions
};

#endif
