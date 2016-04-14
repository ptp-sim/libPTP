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

#ifndef LIBPTP_SCHEDULE_CLOCK_H_
#define LIBPTP_SCHEDULE_CLOCK_H_

// ======================================================
// Includes
// ======================================================

#include "AdjustableClock.h"
#include "IClockEventSink.h"
#include "ScheduledClockEvent.h"

#include <queue>

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cScheduleClock: public cAdjustableClock
{
    private:

        // Types
        typedef std::priority_queue< cScheduledClockEvent, std::vector<cScheduledClockEvent> > ClockEventQueue_t;

        // Resources
        ClockEventQueue_t   EventQueue;
        cMessage            *pNextScheduledEvent;

        // Configuration
        simtime_t           EdgePrecision;

        // Internal housekeeping
        uint64_t            UniqueIdCnt;

        // Debug configuration
        bool                EnableScheduleDebugOutput;

        // Signal handling

        // Watch variables
        size_t  NumScheduledEvents;

        // Debug functions
        void    PrintEventQueue();

        // Event Scheduling
        void    ShiftQueuedEvents( simtime_t Delta );
        void    RemoveQueuedEvent( const cScheduledClockEvent &ScheduledEvent );

        void    UpdateSchedule();

    protected:

        // Configuration

        // OMNeT API
        virtual void handleMessage(cMessage *pMsg);
        virtual void finish();

        // Init API
        void    AllocateResources();
        void    ParseParameters();
        void    RegisterSignals();
        void    InitInternalState();

        // Time adjustment API
        virtual void    HandleTimeJump( simtime_t Delta );

    public:

        // Constructors/Destructor
        cScheduleClock();
        ~cScheduleClock();

        // Event scheduling API
        cScheduledClockEvent    ScheduleAbsoluteEvent( simtime_t ScaledTime, IClockEventSink *pSink, cClockEvent ClockEvent );
        cScheduledClockEvent    ScheduleRelativeEvent( simtime_t Delta,  IClockEventSink *pSink, cClockEvent ClockEvent );
        void    CancelEvent(const cScheduledClockEvent &ScheduledEvent);

        // Time adjustment API
        void    SetScaleFactor_ppb( const int64_t ScaleFactor_ppb );

        // Clock API
        simtime_t       GetScaledTime();
        cLocalTimeStamp GetTimeStamp();
};

#endif
