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

#ifndef LIBPTP_PTP_CLOCKQUALITY_H_
#define LIBPTP_PTP_CLOCKQUALITY_H_

// ======================================================
// Includes
// ======================================================

#include <string>

#include "PTP_PrimitiveDataTypes.h"
#include "PTP_ByteBuffers.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

// Clock quality, see clause 5.3.7 and 8.2.1.3.1 in IEEE 1588-2008
class cClockQuality
{
    private:

        ClockClass_t            clockClass;                     //
        clockAccuracy_t         clockAccuracy;                  //
        UInteger16              offsetScaledLogVariance;        //

    protected:

    public:

        // Constructors
        cClockQuality();
        cClockQuality(  ClockClass_t        clockClass,
                        clockAccuracy_t     clockAccuracy,
                        UInteger16          offsetScaledLogVariance);
        cClockQuality(  BufPtpClockQual_t   *pBuf );
        cClockQuality( const cClockQuality& other );
        ~cClockQuality();

        // Setters
        void    SetClockClass           ( ClockClass_t      clockClass              );
        void    SetClockAccuracy        ( clockAccuracy_t   clockAccuracy           );
        void    SetOffsetScaledLogVar   ( UInteger16        offsetScaledLogVariance );

        void    ReadFromBuffer( BufPtpClockQual_t *pBuf );

        // Getters
        ClockClass_t        GetClockClass()         const;
        clockAccuracy_t     GetClockAccuracy()      const;
        UInteger16          GetOffsetScaledLogVar() const;

        void                SaveToBuffer( BufPtpClockQual_t *pBuf ) const;

        // API functions
        uint64_t    GetUInt64();

        // Operators
        bool            operator== (const cClockQuality& other) const;
        cClockQuality   &operator= (const cClockQuality& other);

        friend std::ostream& operator<<(std::ostream& os, const cClockQuality& o);

        // Debug functions
        void            Print();
        std::string     GetString();
};

#endif
