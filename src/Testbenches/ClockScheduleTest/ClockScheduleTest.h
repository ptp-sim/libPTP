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

#ifndef LIBPTP_CLOCK_SCHEDULE_TEST_H_
#define LIBPTP_CLOCK_SCHEDULE_TEST_H_

#include <omnetpp.h>

#include "CallableModule.h"
#include "IClockEventSink.h"
#include "ScheduleClock.h"

class ClockScheduleTest : public cCallableModule, public IClockEventSink
{
  private:

    // Resources
    cScheduleClock  *pClock;

    cMessage    *pTestMsg1;
    cMessage    *pTestMsg2;
    cMessage    *pTestMsg3;
    cMessage    *pTestMsg4;
    cMessage    *pTestMsg5;

    cClockEvent Event1;
    cClockEvent Event2;
    cClockEvent Event3;
    cClockEvent Event4;

    cScheduledClockEvent    ScheduledEvent1;
    cScheduledClockEvent    ScheduledEvent2;
    cScheduledClockEvent    ScheduledEvent3;
    cScheduledClockEvent    ScheduledEvent4;

    // Configuration
    bool    Enabled;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *pMsg);

  public:
    ClockScheduleTest();

    void    HandleClockEvent( cClockEvent& ClockEvent );
};

#endif
