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

#ifndef LIBPTP_BASIC_SERVICE_H_
#define LIBPTP_BASIC_SERVICE_H_

// ======================================================
// Includes
// ======================================================

#include "SubmoduleInitBase.h"
#include "ScheduleClock.h"
#include "IClockEventSink.h"
#include "CallableSubmodule.h"

// ======================================================
// Types
// ======================================================

class   PTP_Stack;

// ======================================================
// Declarations
// ======================================================

class cBasicService : public cSubmoduleInitBase, public IClockEventSink, public cCallableSubmodule
{
    private:

    protected:

        // Types
        enum class BasicServiceEvent
        {
            INTERVAL = 0,
            NEXT_FREE_ENTRY,
        };

        enum class TimerState
        {
            STOPPED,
            ACTIVE
        };

        // Resources
        PTP_Stack               *pApp;
        cScheduleClock          *pClock;
        cClockEvent             IntervalEvent;
        cScheduledClockEvent    ScheduledInterval;

        // Configuration
        simtime_t       Interval;

        // Internal housekeeping
        TimerState      IntervalState;

        // Internal functions
        virtual void    VerifyCopyable();

    public:

        // Constructors/Destructor
        cBasicService();
        cBasicService( const cBasicService& other );
        virtual ~cBasicService();

        // Setters
        void    SetInterval( simtime_t Interval );

        // Getters

        // Initialization
        void    SetHierarchy( PTP_Stack *pApp, cScheduleClock *pClock );

        // API functions
        void    StartInterval();
        void    StopInterval();

        // Clock API
        void    HandleClockEvent( cClockEvent& ClockEvent );

        // Operators
        cBasicService& operator=( const cBasicService& other );

        // Interval handling
        virtual void HandleIntervalEvent()  = 0;
};

#endif
