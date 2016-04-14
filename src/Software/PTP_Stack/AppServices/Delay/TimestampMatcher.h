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

#ifndef LIBPTP_TIMESTAMP_MATCHER_H_
#define LIBPTP_TIMESTAMP_MATCHER_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

struct MatchedTime_t
{
    simtime_t   Time1;
    simtime_t   Time2;
    simtime_t   Corr1;
    simtime_t   Corr2;
    uint16_t    SequId;
};

class cTimestampMatcher
{
    private:

        uint16_t    Sequ1;
        uint16_t    Sequ2;
        simtime_t   Time1;
        simtime_t   Time2;
        simtime_t   Corr1;
        simtime_t   Corr2;

        MatchedTime_t   MostRecentMatch;

    protected:

    public:

        void    PushFirst( simtime_t Time, simtime_t Corr, uint16_t SequId );
        void    PushSecond( simtime_t Time, simtime_t Corr, uint16_t SequId );

        MatchedTime_t   GetMostRecent();
};

#endif
