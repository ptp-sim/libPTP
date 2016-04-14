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

#ifndef LIBPTP_CLOCK_EVENT_H_
#define LIBPTP_CLOCK_EVENT_H_

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

class cClockEvent
{
    private:

        // Resources
        int         ID1;
        int         ID2;
        int         ID3;
        int         ID4;
        void        *ContextPtr;

    public:

        // Constructor
        cClockEvent();
        cClockEvent( const int ID1, const int ID2, const int ID3, const int ID4, void *ContextPtr );
        cClockEvent( const cClockEvent &other );
        ~cClockEvent();

        // Getters
        const int     GetID1();
        const int     GetID2();
        const int     GetID3();
        const int     GetID4();
              void    *GetContextPtr();

        // Setters
        void    SetID1( const int ID );
        void    SetID2( const int ID );
        void    SetID3( const int ID );
        void    SetID4( const int ID );
        void    SetContextPtr( void *ContextPtr );

        // Operators
        bool    operator== (const cClockEvent& other) const;
        cClockEvent& operator=( const cClockEvent& other );
};

#endif
