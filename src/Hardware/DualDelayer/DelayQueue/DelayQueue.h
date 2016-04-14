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

#ifndef LIBPTP_DELAY_QUEUE_H_
#define LIBPTP_DELAY_QUEUE_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include <queue>

#include "ModuleInitBase.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class DelayQueue: public cModuleInitBase
{
    private:

        // Configuration
        size_t      MaxSize;

        // Resources
        std::queue<cMessage *>  MsgQueue;
        cMessage    *ReminderMsg;
        cPar        *delayPar;
        cGate       *pOutGate;

        // Private functions
        void    HandleReminder();
        void    ScheduleReminder();

    protected:

        // Init API
        void    AllocateResources();
        void    ParseParameters();

        // OMNeT API
        virtual void handleMessage(cMessage *pMsg);

    public:
        DelayQueue();
        ~DelayQueue();
};

#endif

