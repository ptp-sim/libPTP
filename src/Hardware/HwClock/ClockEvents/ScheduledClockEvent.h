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

#ifndef LIBPTP_SCHEDULED_CLOCK_EVENT_H_
#define LIBPTP_SCHEDULED_CLOCK_EVENT_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "IClockEventSink.h"
#include "ClockEvent.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cScheduledClockEvent
{
    private:

        // Resources
        IClockEventSink     *pSink;
        simtime_t           ScaledTime;
        cClockEvent         ClockEvent;
        uint64_t            UniqueId;

    public:

        // Constructor
        cScheduledClockEvent();
        cScheduledClockEvent( IClockEventSink *pSink, simtime_t ScaledTime, cClockEvent ClockEvent, uint64_t UniqueId );
        cScheduledClockEvent( const cScheduledClockEvent &other );
        ~cScheduledClockEvent();

        // Getters
        cClockEvent         GetClockEvent() const;
        simtime_t           GetScaledTime() const;
        IClockEventSink     *GetClockEventSink() const;

        // Setters
        void                SetScaledTime( simtime_t ScaledTime );

        // Operators
        bool operator<(const cScheduledClockEvent& rhs) const;
        bool operator>(const cScheduledClockEvent& rhs) const;
        bool operator==(const cScheduledClockEvent& other) const;
        cScheduledClockEvent& operator=( const cScheduledClockEvent& other );

        // API functions
        void Execute();

        // Debug functions
        void Print();
};

#endif
