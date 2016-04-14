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

#ifndef LIBPTP_PTP_PORTIDENTITY_H_
#define LIBPTP_PTP_PORTIDENTITY_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_ClockIdentity.h"
#include "PTP_PrimitiveDataTypes.h"
#include "PTP_ByteBuffers.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

// Port identity, see clause 5.3.5 and 7.5.2 in IEEE 1588-2008
class cPortIdentity
{
    private:
        cClockIdentity          clockIdentity;                  //
        UInteger16              portNumber;                     // 1-N, where N is the number of ports

    protected:

    public:

        // Constructors
        cPortIdentity();
        cPortIdentity( const char *ClockStr, UInteger16 portNumber );
        cPortIdentity( const cClockIdentity clockIdentity, const UInteger16 portNumber );
        cPortIdentity( const BufPtpPortId_t *pBuf );
        cPortIdentity( const cPortIdentity& other );
        ~cPortIdentity();


        // Instance methods
        cClockIdentity&     ClockIdentity();

        // Setters
        void            SetPortNumber( UInteger16   portNumber );
        void            ReadFromBuffer( const BufPtpPortId_t *pBuf );

        //Getters
        UInteger16      GetPortNumber() const;
        std::string     GetString()     const;
        Octet           GetByteAt( size_t idx );

        void            SaveToBuffer( BufPtpPortId_t *pBuf );

        // Operators
        bool            operator== (const cPortIdentity& other) const;
        bool            operator!= (const cPortIdentity& other) const;
        friend bool     operator<(cPortIdentity const& lhs, cPortIdentity const& rhs);
        friend bool     operator>(cPortIdentity const& lhs, cPortIdentity const& rhs);

        cPortIdentity&  operator= (const cPortIdentity& other);
        cPortIdentity&  operator= (const cClockIdentity& Clock);

        friend std::ostream& operator<<(std::ostream& os, const cPortIdentity& o);

        // Debug functions
        void    Print();
};

#endif
