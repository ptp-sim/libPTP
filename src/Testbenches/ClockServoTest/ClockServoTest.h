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

#ifndef LIBPTP_CLOCKSERVO_TEST_H_
#define LIBPTP_CLOCKSERVO_TEST_H_

#include <omnetpp.h>

#include "IClockServo.h"

#include "ScheduleClock.h"

class ClockServoTest : public cSimpleModule
{
    private:

        // Types
        typedef enum
        {
            INIT,
            CONFIG,
            ENABLE,
            RUN,
            INVALID,
        }
        TestState_t;

        // Resources
        IClockServo     *pClockServo;
        cScheduleClock  *pMasterClock;
        cScheduleClock  *pSlaveClock;
        cMessage        *pTimerMsg;

        // Configuration
        simtime_t   Interval;

        // House keeping
        TestState_t TestState;

        void    ParseParameters();

    protected:

        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:

        ClockServoTest();
};

#endif
