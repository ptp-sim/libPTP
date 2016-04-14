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

#ifndef LIBPTP_TIME_DIFF_OBSERVER_H_
#define LIBPTP_TIME_DIFF_OBSERVER_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "ModuleInitBase.h"
#include "ScheduleClock.h"

#include <string>

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cTimeDiffObserver: public cModuleInitBase
{
    private:

        // Resources
        cMessage            *pTraceMsg;
        cScheduleClock      *pClock1;
        cScheduleClock      *pClock2;

        std::string         DisplayString;

        // Configuration
        std::string         ClockPath1;
        std::string         ClockPath2;
        simtime_t           TraceInterval;

        // Gui configuration
        int                 GuiExponent;
        bool                AutomaticGuiExponent;

        // Internal housekeeping

        // Signal handling
        simsignal_t         HwTimeDiff_SigId;
        simsignal_t         ScaledTimeDiff_SigId;

        // Internal functions
        static simtime_t   RoundSimtime( simtime_t t, int Exponent  );
        static std::string GetUnitString( int Exponent );

        // Initialization
        void    ParseResourceParameters();
        void    AllocateResources();
        void    ParseParameters();
        void    RegisterSignals();
        void    InitInternalState();
        void    InitSignals();
        void    FinishInit();

    protected:

        // OMNeT API
        void handleMessage(cMessage *msg);
        void finish();

    public:

        cTimeDiffObserver();
        ~cTimeDiffObserver();
};

#endif
