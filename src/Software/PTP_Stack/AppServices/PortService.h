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

#ifndef LIBPTP_PORT_SERVICE_H_
#define LIBPTP_PORT_SERVICE_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "BasicService.h"

#include "PTP_PrimitiveDataTypes.h"

// ======================================================
// Types
// ======================================================

class   cPTP_Port;

// ======================================================
// Declarations
// ======================================================

class cPortService : public cBasicService
{
    private:

    protected:

        // Types
        enum class  PortServiceEvent
        {
            TIMEOUT = BasicServiceEvent::NEXT_FREE_ENTRY,
            NEXT_FREE_ENTRY,
        };

        // Internal housekeeping
        UInteger16      SequId;
        TimerState      TimeoutState;

        // Configuration
        simtime_t       Timeout;

        // Resources
        cPTP_Port               *pPort;
        cClockEvent             TimeoutEvent;
        cScheduledClockEvent    ScheduledTimeout;

        // Internal functions
        virtual void    VerifyCopyable();

    public:

        // Constructors/Destructor
        cPortService();
        cPortService( const cPortService& other );
        virtual ~cPortService();

        // Setters
        void        SetTimeout( simtime_t Timeout );

        // Getters

        // Initialization
        void        SetHierarchy( PTP_Stack *pApp, cPTP_Port *pPort, cScheduleClock *pClock );

        // API functions
        UInteger16  DrawSequId();

        void        StartTimeout();
        void        UpdateTimeout();
        void        StopTimeout();

        // Clock API
        void        HandleClockEvent( cClockEvent& ClockEvent );

        // Operators
        cPortService& operator=( const cPortService& other );

        // Timeout handling
        virtual void HandleTimeoutEvent()   = 0;
};

#endif
