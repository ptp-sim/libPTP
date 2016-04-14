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

#ifndef LIBPTP_PTP_TIMEINTERVAL_H_
#define LIBPTP_PTP_TIMEINTERVAL_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include <PTP_PrimitiveDataTypes.h>

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

// Time interval, see clause 5.3.2 in IEEE 1588-2008
class cTimeInterval
{
    private:
        Integer64               scaledNanoseconds;              // [ns], scaled by 2^16

    protected:

    public:
        // Constructors
        cTimeInterval();
        cTimeInterval( simtime_t t );
        cTimeInterval( Integer64 scaledNanoseconds );
        cTimeInterval( const cTimeInterval& other );

        // Destructor
        ~cTimeInterval();

        // Setters

        // Getters
        simtime_t       GetSimTime();
        std::string     GetString() const;
        Integer64       GetScaledNanoseconds() const;

        // Operators
        bool            operator== (const cTimeInterval& other);
        bool            operator!= (const cTimeInterval& other);
        bool            operator== (const simtime_t& other);
        bool            operator!= (const simtime_t& other);
        cTimeInterval&  operator= (const cTimeInterval& other);
        cTimeInterval&  operator= (const Integer64 ScaledNanoSeconds);
        cTimeInterval&  operator= (const simtime_t& other);

        const cTimeInterval operator+(const cTimeInterval &other) const;
        const cTimeInterval operator-(const cTimeInterval &other) const;
        const cTimeInterval operator*(const double        d) const;
        const cTimeInterval operator/(const double        d) const;
        const cTimeInterval operator-() const;

        cTimeInterval &operator+=(const cTimeInterval &other);
        cTimeInterval &operator-=(const cTimeInterval &other);
        cTimeInterval &operator*=(const double        d);
        cTimeInterval &operator/=(const double        d);

        friend std::ostream& operator<<(std::ostream& os, const cTimeInterval& o);
};

#endif
