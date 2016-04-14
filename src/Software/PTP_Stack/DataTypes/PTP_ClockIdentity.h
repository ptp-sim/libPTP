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

#ifndef LIBPTP_PTP_CLOCK_IDENTITY_H_
#define LIBPTP_PTP_CLOCK_IDENTITY_H_

// ======================================================
// Includes
// ======================================================

#include "MACAddress.h"

#include "PTP_PrimitiveDataTypes.h"
#include "PTP_Constants.h"
#include "PTP_ByteBuffers.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

// Clock identity, see clause 5.3.4 and 7.5.2.2 in IEEE 1588-2008
class cClockIdentity
{
    private:
        Octet   Bytes[ PTP_CLOCK_IDENTITY_SIZE ];

    protected:

    public:
        // Constructors/Destructor
        cClockIdentity();
        cClockIdentity( const char *Str );
        cClockIdentity( const MACAddress MAC );
        cClockIdentity( const BufPtpClockId_t *pBuf );
        cClockIdentity( const cClockIdentity& other );
        ~cClockIdentity();

        // Setters
        void    ReadFromBuffer( const BufPtpClockId_t *pBuf );

        // Getters
        Octet   GetByteAt( size_t idx );
        void    SaveToBuffer( BufPtpClockId_t *pBuf );

        // Operators
        bool            operator== (const cClockIdentity& other)    const;
        bool            operator!= (const cClockIdentity& other)    const;
        friend bool     operator<(cClockIdentity const& lhs, cClockIdentity const& rhs);
        friend bool     operator>(cClockIdentity const& lhs, cClockIdentity const& rhs);
        cClockIdentity& operator= (const cClockIdentity& other);
        cClockIdentity& operator= (const char *Str);
        cClockIdentity& operator= (const MACAddress MAC);

        friend std::ostream& operator<<(std::ostream& os, const cClockIdentity& o);

        // Debug functions
        void            Print()     const;
        std::string     GetString() const;
};

#endif
