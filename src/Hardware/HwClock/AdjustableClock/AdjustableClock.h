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

#ifndef LIBPTP_ADJUSTABLE_CLOCK_H_
#define LIBPTP_ADJUSTABLE_CLOCK_H_

// ======================================================
// Includes
// ======================================================

#include "HwClock.h"
#include "LocalTimeStamp.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cAdjustableClock: public cHwClock
{
    private:

        // Types
        struct ScalePoint_t
        {
            simtime_t   HwTime;
            simtime_t   ScaledTime;
            int64_t     ScaleFactor_ppb;
            double      ScaleFactor;
        };

        // Resources
        uint64_t        EpochID;

        // Configuration
        int64_t         ScaleFactor_LowerBound_ppb;
        int64_t         ScaleFactor_UpperBound_ppb;

        // Internal housekeeping
        ScalePoint_t    ScalePoints[2];

        // Signal handling
        simsignal_t     ScaledTime_SigId;
        simsignal_t     ScaleFactor_ppb_SigId;

        // Watch variables
        simtime_t       LastScaledTime;

        // Debug functions

        // Internal functions
        void    SetScalePoint( simtime_t HwTime, simtime_t ScaledTime, int64_t ScaleFactor_ppb, bool Reset );

    protected:

        // Configuration
        bool            EnableAdjustments;

        // OMNeT API
        virtual void handleMessage(cMessage *msg);
        virtual void finish();

        // API for sub-classes
        simtime_t   GetScaledTimeAtHwTime( simtime_t HwTime     ) const;
        simtime_t   GetHwTimeAtScaledTime( simtime_t ScaledTime ) const;

        // Clock API
        simtime_t       GetScaledTime();
        virtual void    HandleTimeJump( simtime_t Delta );

        // Init API
        void    AllocateResources();
        void    ParseParameters();
        void    RegisterSignals();
        void    InitInternalState();
        void    InitSignals();
        void    FinishInit();

    public:

        // Constructors/Destructor
        cAdjustableClock();
        ~cAdjustableClock();

        // Time adjustment API
        void    IncScaledTime       ( const simtime_t   Delta           );
        void    SetScaledTime       ( const simtime_t   ScaledTime      );
        void    SetScaleFactor_ppb  ( const int64_t     ScaleFactor_ppb );

        // Clock API
        cLocalTimeStamp GetTimeStamp();

        // Getters
        int64_t     GetScaleFactor_ppb();
        int64_t     GetScaleFactor_LowerBound_ppb();
        int64_t     GetScaleFactor_UpperBound_ppb();
};

#endif

