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

#ifndef LIBPTP_HW_CLOCK_H_
#define LIBPTP_HW_CLOCK_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "ModuleInitBase.h"
#include "ITdGen.h"
#include "HwClock_ParameterParser.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cHwClock: public cModuleInitBase
{
    private:

        // Resources
        ITdGen          *pTdGen;

        // Configuration
        TdGenType_t     TdGenType;
        simtime_t       TimeTraceInterval;
        bool            EnableTimeTracing;

        // Internal housekeeping

        // Signal handling
        simsignal_t     TimeDeviation_SigId;
        simsignal_t     HwTime_SigId;
        simsignal_t     RequHwTime_SigId;

        // Debug functions

    protected:

        // Resources
        cMessage        *pTimeTraceMsg;

        // Configuration
        uint64_t        TickLenNom_ps;
        simtime_t       TickLenNom;
        simtime_t       T0;                 // Initial phase offset

        // OMNeT API
        virtual void handleMessage(cMessage *pMsg);
        virtual void finish();

        // API for sub-classes
        simtime_t   GetRealTimeBeforeHwTime( const simtime_t HwTime );
        simtime_t   GetHwTimeAtRealTime( const simtime_t RealTime );
        simtime_t   RoundDownToTickLen( const simtime_t HwTime ) const;
        simtime_t   RoundUpToTickLen( const simtime_t HwTime ) const;

        // Init API
        void    ParseResourceParameters();
        void    AllocateResources();
        void    InitHierarchy();
        void    ParseParameters();
        void    RegisterSignals();
        void    InitInternalState();
        void    InitSignals();
        void    FinishInit();
        void    ForwardInit( int stage );

        // Debug API
        simtime_t   GetTD( simtime_t RealTime );

        // Time readout
        simtime_t   GetHwTime();

    public:

        cHwClock();
        ~cHwClock();

};

#endif

