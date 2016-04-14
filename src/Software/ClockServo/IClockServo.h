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

#ifndef LIBPTP_ICLOCK_SERVO_H_
#define LIBPTP_ICLOCK_SERVO_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "ScheduleClock.h"
#include "ModuleInitBase.h"

// ======================================================
// Types
// ======================================================

// ------------------------------------------------------------
// Types
// ------------------------------------------------------------

struct SampleDecision_t
{
    bool        EnableJump;
    simtime_t   Delta;
    bool        EnableScale;
    int64_t     ScaleFactor_ppb;
};

// ======================================================
// Declarations
// ======================================================

class IClockServo: public cModuleInitBase
{
    public:

        // Type definitions
        enum class ClockServoState
        {
            DISABLED        = 0,
            INIT            = 10,
            SYNTONIZE_INIT  = 20,
            SYNTONIZE_START = 21,
            SYNTONIZE       = 22,
            JUMP            = 30,
            SCALE           = 40,
        };

    private:

        // Internal functions
        void    CalcMaxFrequEstCnt();

    protected:

        // Resources

        // Configuration
        int64_t         ScaleFactor_LowerBound_ppb;
        int64_t         ScaleFactor_UpperBound_ppb;
        int             FrequEstShift;
        bool            EnableSyntonize;
        bool            EnableTimeJump;
        bool            EnableSynchronize;
        simtime_t       MaxFrequEstInterval;
        simtime_t       OffsetThreshForReset;

        // Debug config
        bool            EnableDebugOutput;

        // Servo state handling
        int                 MaxFreqEstCnt;
        int                 FreqEstCnt;
        ClockServoState     ServoState;
        simtime_t           Remote[2];
        simtime_t           Local[2];
        simtime_t           SyncInterval;
        SampleDecision_t    SampleDec;

        // Signals
        simsignal_t     ClockServoState_SigId;
        simsignal_t     SyncInterval_SigId;
        simsignal_t     OffsetFromMaster_SigId;

        simsignal_t     Decision_EnableJump_SigId;
        simsignal_t     Decision_JumpDelta_SigId;
        simsignal_t     Decision_EnableScale_SigId;
        simsignal_t     Decision_ScaleFactor_ppb_SigId;

        // API for sub-classes
        virtual void    SetServoState( ClockServoState NewServoState );
        virtual void    SampleInternal( simtime_t offsetFromMaster, simtime_t Ingress ) = 0;
        virtual void    Reset();

        // OMNeT API
        virtual void    handleMessage(cMessage *msg);
        virtual void    finish();

        // Init API
        void    ParseParameters();
        void    RegisterSignals();
        void    InitInternalState();
        void    InitSignals();

    public:

        // Constructor
        IClockServo();

        // Destructor
        ~IClockServo();

        // Basic clock servo API
        void                Enable();
        void                Disable();
        bool                IsEnabled();
        SampleDecision_t    Sample( simtime_t offsetFromMaster, simtime_t Ingress );
        virtual void        SetSyncInterval( simtime_t SyncInterval );
        virtual void        SetScaleFactor_ppb( int64_t ScaleFactor_ppb );
        virtual void        SetScaleFactorBounds( int64_t ScaleFactor_LowerBound_ppb, int64_t ScaleFactor_UpperBound_ppb );
};

#endif
