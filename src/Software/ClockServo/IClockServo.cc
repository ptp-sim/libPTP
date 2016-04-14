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

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "IClockServo.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Constants
// ======================================================

// ======================================================
// Variables
// ======================================================

// ======================================================
// Declarations
// ======================================================

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Stream operators
// ------------------------------------------------------
std::ostream&
operator<<(std::ostream& os, const IClockServo::ClockServoState& o)
{
    switch(o)
    {
        case IClockServo::ClockServoState::DISABLED:            os << "Disabled";           break;
        case IClockServo::ClockServoState::INIT:                os << "Init";               break;
        case IClockServo::ClockServoState::SYNTONIZE_INIT:      os << "Syntonize (Init)";   break;
        case IClockServo::ClockServoState::SYNTONIZE_START:     os << "Syntonize (Start)";  break;
        case IClockServo::ClockServoState::SYNTONIZE:           os << "Syntonize";          break;
        case IClockServo::ClockServoState::JUMP:                os << "Jump";               break;
        case IClockServo::ClockServoState::SCALE:               os << "Scaling";            break;

        default:                                                os << "Unknown";            break;
    }

    return os;
}


// ------------------------------------------------------
// Internal functions
// ------------------------------------------------------
void
IClockServo::CalcMaxFrequEstCnt()
{
    if( SyncInterval == SIMTIME_ZERO )
    {
        throw cRuntimeError( "Can't calculate maximum frequency estimation count if SyncInterval is not set." );
    }

    MaxFreqEstCnt   = std::min( (1 << FrequEstShift)-1, static_cast<int>(MaxFrequEstInterval/SyncInterval) );
    MaxFreqEstCnt   = std::max( MaxFreqEstCnt, 1 );

    if( EnableDebugOutput )
    {
        EV << "MaxFreqEstCnt: " << MaxFreqEstCnt << endl;
    }
}


// ------------------------------------------------------
// Set servo state
// ------------------------------------------------------
void
IClockServo::SetServoState( ClockServoState NewServoState )
{
    if( ServoState != NewServoState )
    {
        emit( ClockServoState_SigId, static_cast<int>(NewServoState) );
    }

    ServoState  = NewServoState;
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------

void
IClockServo::ParseParameters()
{
    EnableSyntonize         = par( "EnableSyntonize" ).boolValue();
    EnableTimeJump          = par( "EnableTimeJump" ).boolValue();
    EnableSynchronize       = par( "EnableSynchronize" ).boolValue();
    FrequEstShift           = par( "FrequEstShift" ).longValue();
    MaxFrequEstInterval     = par( "MaxFrequEstInterval" ).doubleValue();
    OffsetThreshForReset    = par( "OffsetThreshForReset" ).doubleValue();
    EnableDebugOutput       = par( "EnableDebugOutput" ).boolValue();
}

void
IClockServo::RegisterSignals()
{
    ClockServoState_SigId   = registerSignal( "ClockServoState" );
    SyncInterval_SigId      = registerSignal( "SyncInterval" );
    OffsetFromMaster_SigId  = registerSignal( "OffsetFromMaster" );

    Decision_EnableJump_SigId       = registerSignal( "Decision_EnableJump"      );
    Decision_JumpDelta_SigId        = registerSignal( "Decision_JumpDelta"       );
    Decision_EnableScale_SigId      = registerSignal( "Decision_EnableScale"     );
    Decision_ScaleFactor_ppb_SigId  = registerSignal( "Decision_ScaleFactor_ppb" );

    WATCH(ScaleFactor_LowerBound_ppb);
    WATCH(ScaleFactor_UpperBound_ppb);
    WATCH(ServoState);
    WATCH(SyncInterval);
    WATCH(SampleDec.EnableJump);
    WATCH(SampleDec.Delta);
    WATCH(SampleDec.EnableScale);
    WATCH(SampleDec.ScaleFactor_ppb);
}

void
IClockServo::InitInternalState()
{
    Disable();
}

void
IClockServo::InitSignals()
{
    emit( ClockServoState_SigId, static_cast<int>(ClockServoState::DISABLED) );
}

// ------------------------------------------------------
// Handle messages
// ------------------------------------------------------
void
IClockServo::handleMessage(cMessage *pMsg)
{
}

// ------------------------------------------------------
// Finish
// ------------------------------------------------------
void
IClockServo::finish()
{
}

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
IClockServo::IClockServo()
{
    ScaleFactor_LowerBound_ppb  = 0;
    ScaleFactor_UpperBound_ppb  = 0;
    FrequEstShift               = 0;
    EnableSyntonize             = false;
    EnableTimeJump              = false;
    EnableSynchronize           = false;
    OffsetThreshForReset        = SIMTIME_ZERO;
    SyncInterval                = SIMTIME_ZERO;
    ServoState                  = ClockServoState::DISABLED;

    Reset();
}

// ------------------------------------------------------
// Desctructor
// ------------------------------------------------------
IClockServo::~IClockServo()
{
}

// ------------------------------------------------------
// Sample API
// ------------------------------------------------------
void
IClockServo::Reset()
{
    FreqEstCnt          = 0;
    Remote[0]           = SIMTIME_ZERO;
    Remote[1]           = SIMTIME_ZERO;
    Local [0]           = SIMTIME_ZERO;
    Local [1]           = SIMTIME_ZERO;

    SampleDec.EnableJump        = false;
    SampleDec.Delta             = SIMTIME_ZERO;
    SampleDec.EnableScale       = false;
    SampleDec.ScaleFactor_ppb   = 0;
}

void
IClockServo::SetSyncInterval( simtime_t SyncInterval )
{
    EnterModuleSilent();

    if( this->SyncInterval != SyncInterval )
    {
        this->SyncInterval = SyncInterval;

        emit( SyncInterval_SigId, this->SyncInterval );
    }

    LeaveModule();
}

void
IClockServo::SetScaleFactor_ppb( int64_t ScaleFactor_ppb )
{
    EnterModuleSilent();

    if( ServoState != ClockServoState::DISABLED )
    {
        throw cRuntimeError( "Can't configure already enabled clock servo." );
    }

    SampleDec.ScaleFactor_ppb = ScaleFactor_ppb;

    LeaveModule();
}

void
IClockServo::SetScaleFactorBounds( int64_t ScaleFactor_LowerBound_ppb, int64_t ScaleFactor_UpperBound_ppb )
{
    EnterModuleSilent();

    if( ServoState != ClockServoState::DISABLED )
    {
        throw cRuntimeError( "Can't configure already enabled clock servo." );
    }

    this->ScaleFactor_LowerBound_ppb    = ScaleFactor_LowerBound_ppb;
    this->ScaleFactor_UpperBound_ppb    = ScaleFactor_UpperBound_ppb;

    LeaveModule();
}

void
IClockServo::Enable()
{
    EnterModuleSilent();

    if( ServoState == ClockServoState::DISABLED )
    {
        SetServoState( ClockServoState::INIT );
    }

    LeaveModule();
}

void
IClockServo::Disable()
{
    EnterModuleSilent();

    SetServoState( ClockServoState::DISABLED );
    Reset();

    LeaveModule();
}

bool
IClockServo::IsEnabled()
{
    EnterModuleSilent();

    if( ServoState == ClockServoState::DISABLED )
    {
        return false;
    }
    else
    {
        return true;
    }

    LeaveModule();
}

SampleDecision_t
IClockServo::Sample( simtime_t offsetFromMaster, simtime_t Ingress )
{
    EnterModuleSilent();

    emit( OffsetFromMaster_SigId, offsetFromMaster );

    if( EnableDebugOutput )
    {
        EV << "Sampling, current state: " << ServoState << endl;
    }

    switch( ServoState )
    {
        case ClockServoState::DISABLED:
        {
            // Nothing to do while disabled
            break;
        }

        case ClockServoState::INIT:
        {
            if( EnableSyntonize )
            {
                SetServoState( ClockServoState::SYNTONIZE_INIT );
            }
            else if( EnableTimeJump )
            {
                SetServoState( ClockServoState::JUMP );
            }
            else if( EnableSynchronize )
            {
                SetServoState( ClockServoState::SCALE );
            }
            break;
        }

        case ClockServoState::SYNTONIZE_INIT:
        {
            SampleDec.EnableJump        = false;
            SampleDec.Delta             = 0;
            SampleDec.EnableScale       = true;
            SampleDec.ScaleFactor_ppb   = 0;

            FreqEstCnt  = 0;

            CalcMaxFrequEstCnt();

            SetServoState( ClockServoState::SYNTONIZE_START );
            break;
        }

        case ClockServoState::SYNTONIZE_START:
        {
            if( FreqEstCnt >= MaxFreqEstCnt )
            {
                Remote[0]   = offsetFromMaster + Ingress;
                Local [0]   = Ingress;
                FreqEstCnt  = 1;

                if( EnableDebugOutput )
                {
                    EV << "Starting syntonizing" << endl;
                    EV << "Estimated remote time:    " << Remote[0] << endl;
                    EV << "Corresponding local time: " << Local [0] << endl;
                }

                SetServoState( ClockServoState::SYNTONIZE );
            }
            else
            {
                FreqEstCnt ++;
            }
            break;
        }

        case ClockServoState::SYNTONIZE:
        {
            Remote[1]  = offsetFromMaster + Ingress;
            Local [1]  = Ingress;

            if( FreqEstCnt >= MaxFreqEstCnt )
            {
                simtime_t   LocalDiff   = Local [1] - Local [0];
                simtime_t   RemoteDiff  = Remote[1] - Remote[0];

                int64_t     ScaleFactor_ppb = (( (1E9L+SampleDec.ScaleFactor_ppb) * LocalDiff.dbl()) / RemoteDiff.dbl() ) - 1E9L;

                if( EnableDebugOutput )
                {
                    EV << "Finshed syntonizing" << endl;
                    EV << "Estimated remote time:    " << Remote[1] << endl;
                    EV << "Corresponding local time: " << Local [1] << endl;
                    EV << endl;
                    EV << "LocalDiff:  " << LocalDiff << endl;
                    EV << "RemoteDiff: " << RemoteDiff << endl;
                    EV << "";
                    EV << "ScaleFactor_ppb: " << ScaleFactor_ppb << endl;
                }

                if( ScaleFactor_ppb > ScaleFactor_UpperBound_ppb )
                    ScaleFactor_ppb = ScaleFactor_UpperBound_ppb;
                if( ScaleFactor_ppb < ScaleFactor_LowerBound_ppb )
                    ScaleFactor_ppb = ScaleFactor_LowerBound_ppb;

                SampleDec.EnableJump        = false;
                SampleDec.Delta             = 0;
                SampleDec.EnableScale       = true;
                SampleDec.ScaleFactor_ppb   = ScaleFactor_ppb;

                if( EnableSynchronize )
                {
                    if( EnableTimeJump )
                    {
                        SetServoState( ClockServoState::JUMP );
                    }
                    else
                    {
                        SetServoState( ClockServoState::SCALE );
                    }
                }
                else
                {
                    SetServoState( ClockServoState::SYNTONIZE_START );
                }
            }
            else
            {
                FreqEstCnt ++;
            }
            break;
        }

        case ClockServoState::JUMP:
        {
            SampleDec.EnableScale       = false;
            SampleDec.EnableJump        = true;
            SampleDec.Delta             = -offsetFromMaster;

            if( EnableSynchronize )
            {
                SetServoState( ClockServoState::SCALE );
            }
            break;
        }

        case ClockServoState::SCALE:
        {
            // Check if we are close enough to the master for scaling the clock
            // Reset state machine otherwise
            if(( fabs(offsetFromMaster) <= OffsetThreshForReset ) || (!EnableTimeJump))
            {
                SampleInternal( -offsetFromMaster, Ingress );
            }
            else
            {
                Reset();
                SetServoState( ClockServoState::INIT );
            }

            break;
        }
    }

    emit( Decision_EnableJump_SigId,  SampleDec.EnableJump  );
    emit( Decision_JumpDelta_SigId,   SampleDec.Delta       );
    emit( Decision_EnableScale_SigId, SampleDec.EnableScale );
    emit( Decision_ScaleFactor_ppb_SigId, (long int)SampleDec.ScaleFactor_ppb );

    LeaveModule();

    return SampleDec;
}
