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

#include "AdjustableClock.h"

#include <cassert>

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

Define_Module(cAdjustableClock);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Internal functions
// ------------------------------------------------------

void
cAdjustableClock::SetScalePoint( simtime_t HwTime, simtime_t ScaledTime, int64_t ScaleFactor_ppb, bool Reset )
{
    if( ScaledTime < SIMTIME_ZERO )
    {
        throw cRuntimeError( "Can't set ScaledTime to a negative value." );
    }

    // Limit ScaleFactor
    int64_t NewScaleFactor_ppb = ScaleFactor_ppb;
    double  ScaleFactor;

    if( NewScaleFactor_ppb < ScaleFactor_LowerBound_ppb )
        NewScaleFactor_ppb = ScaleFactor_LowerBound_ppb;
    if( NewScaleFactor_ppb > ScaleFactor_UpperBound_ppb )
        NewScaleFactor_ppb = ScaleFactor_UpperBound_ppb;

    ScaleFactor = (1E9L+NewScaleFactor_ppb) / 1E9L;

    assert( ScaleFactor != 0.0L );

    ScalePoints[0]                  = ScalePoints[1];
    ScalePoints[1].HwTime           = RoundDownToTickLen( HwTime );;
    ScalePoints[1].ScaledTime       = ScaledTime;
    ScalePoints[1].ScaleFactor_ppb  = NewScaleFactor_ppb;
    ScalePoints[1].ScaleFactor      = ScaleFactor;

    if( Reset )
    {
        ScalePoints[0].HwTime           = SIMTIME_ZERO;
        ScalePoints[0].ScaledTime       = SIMTIME_ZERO;
        ScalePoints[0].ScaleFactor      = ScalePoints[1].ScaledTime / ScalePoints[1].HwTime;
        ScalePoints[0].ScaleFactor_ppb  = (ScalePoints[0].ScaleFactor * 1E9L) - 1E9L;
    }

    // Emit statistics signal
    emit( ScaleFactor_ppb_SigId, (long) NewScaleFactor_ppb );
}

// ------------------------------------------------------
// OMNeT API
// ------------------------------------------------------

void
cAdjustableClock::handleMessage(cMessage *pMsg)
{
    // Handle self-message (can only be schedule App msgs)
    if( pMsg->isSelfMessage() )
    {
        if( pTimeTraceMsg == pMsg )
        {
            emit( ScaledTime_SigId, GetScaledTime() );

            cHwClock::handleMessage( pMsg );
        }
    }
    else
    {
        throw cRuntimeError( "HwClock: Received unexpected message" );
        delete pMsg;
    }
}

void
cAdjustableClock::finish()
{
}

// ------------------------------------------------------
// Time conversion
// ------------------------------------------------------
simtime_t
cAdjustableClock::GetScaledTimeAtHwTime( simtime_t HwTime ) const
{
    if( HwTime < ScalePoints[0].HwTime )
    {
        throw cRuntimeError( "Clock: Can't calculate ScaledTime based on old HwTime." );
    }

    simtime_t   BaseScaledTime;
    simtime_t   BaseHwTime;
    double      ScaleFactor;

    if( HwTime >= ScalePoints[1].HwTime )
    {
        BaseScaledTime  = ScalePoints[1].ScaledTime;
        BaseHwTime      = ScalePoints[1].HwTime;
        ScaleFactor     = ScalePoints[1].ScaleFactor;
    }
    else
    {
        BaseScaledTime  = ScalePoints[0].ScaledTime;
        BaseHwTime      = ScalePoints[0].HwTime;
        ScaleFactor     = ScalePoints[0].ScaleFactor;
    }

    simtime_t   HwDiff      = RoundDownToTickLen(HwTime) - BaseHwTime;
    simtime_t   ScaledDiff  = HwDiff * ScaleFactor;

    return BaseScaledTime + ScaledDiff;
}

simtime_t
cAdjustableClock::GetHwTimeAtScaledTime( simtime_t ScaledTime ) const
{
    if( ScaledTime == ScalePoints[1].ScaledTime )
    {
        return ScalePoints[1].HwTime;
    }

    if( ScaledTime < ScalePoints[0].ScaledTime )
    {
        std::stringstream ss;

        ss << "Clock: Can't calculate HwTime based on old ScaledTime." << endl;
        ss << "Requested ScaledTime: " << ScaledTime << endl;
        ss << "ScaledTime 0: " << ScalePoints[0].ScaledTime << endl;
        ss << "ScaledTime 1: " << ScalePoints[1].ScaledTime << endl;

        throw cRuntimeError( ss.str().c_str() );
    }

    simtime_t   BaseScaledTime;
    simtime_t   BaseHwTime;
    double      ScaleFactor;

    if( ScaledTime >= ScalePoints[1].ScaledTime)
    {
        BaseScaledTime  = ScalePoints[1].ScaledTime;
        BaseHwTime      = ScalePoints[1].HwTime;
        ScaleFactor     = ScalePoints[1].ScaleFactor;
    }
    else
    {
        BaseScaledTime  = ScalePoints[0].ScaledTime;
        BaseHwTime      = ScalePoints[0].HwTime;
        ScaleFactor     = ScalePoints[0].ScaleFactor;
    }

    simtime_t   ScaledDiff  = (ScaledTime - BaseScaledTime);
    simtime_t   HwDiff      = ScaledDiff / ScaleFactor;
    simtime_t   HwTime      = RoundDownToTickLen( BaseHwTime + HwDiff );

    while( GetScaledTimeAtHwTime(HwTime) < ScaledTime )
    {
        HwTime  += TickLenNom;
    }

    if( HwTime >= BaseHwTime + TickLenNom )
    {
        assert( GetScaledTimeAtHwTime(HwTime-TickLenNom) < ScaledTime);
    }
    assert( GetScaledTimeAtHwTime(HwTime) >= ScaledTime );

    return HwTime;
}

// ------------------------------------------------------
// Init API
// ------------------------------------------------------

void
cAdjustableClock::AllocateResources()
{
    cHwClock::AllocateResources();
}

void
cAdjustableClock::ParseParameters()
{
    cHwClock::ParseParameters();

    EnableAdjustments       = par("EnableAdjustments").boolValue();
    ScaleFactor_LowerBound_ppb  = par("ScaleFactor_LowerBound_ppb").longValue();
    ScaleFactor_UpperBound_ppb  = par("ScaleFactor_UpperBound_ppb").longValue();

    if( ScaleFactor_UpperBound_ppb < 0 )
    {
        throw cRuntimeError( "Clock: Can't have a negative upper bound for scale factor: %d", ScaleFactor_UpperBound_ppb );
    }
    if( ScaleFactor_LowerBound_ppb > 0 )
    {
        throw cRuntimeError( "Clock: Can't have a positive lower bound for scale factor: %d", ScaleFactor_LowerBound_ppb );
    }

    ScalePoints[1].ScaledTime   = simtime_t( par("BeginScaledTime").doubleValue() );
}

void
cAdjustableClock::RegisterSignals()
{
    cHwClock::RegisterSignals();

    ScaledTime_SigId        = registerSignal( "ScaledTime"      );
    ScaleFactor_ppb_SigId   = registerSignal( "ScaleFactor_ppb" );

    WATCH(ScalePoints[1].ScaleFactor_ppb);
    WATCH(LastScaledTime);
}

void
cAdjustableClock::InitInternalState()
{
    cHwClock::InitInternalState();

    SetScalePoint( SIMTIME_ZERO, ScalePoints[1].ScaledTime, 0, true );
}

void
cAdjustableClock::InitSignals()
{
    cHwClock::InitSignals();

    emit( ScaledTime_SigId, GetScaledTime() );
}

void
cAdjustableClock::FinishInit()
{
    cHwClock::FinishInit();
}

// ------------------------------------------------------
// Constructors/Destructor
// ------------------------------------------------------

cAdjustableClock::cAdjustableClock()
    : cHwClock()
{
    EpochID = 0;

    LastScaledTime              = SIMTIME_ZERO;
    ScalePoints[0].HwTime       = SIMTIME_ZERO;
    ScalePoints[0].ScaledTime   = SIMTIME_ZERO;
    ScalePoints[1].HwTime       = SIMTIME_ZERO;
    ScalePoints[1].ScaledTime   = SIMTIME_ZERO;
}

cAdjustableClock::~cAdjustableClock()
{
}

// ------------------------------------------------------
// Time adjustment API
// ------------------------------------------------------

void
cAdjustableClock::HandleTimeJump( simtime_t Delta )
{
    EpochID ++;
}

void
cAdjustableClock::IncScaledTime( const simtime_t Delta )
{
    EnterModuleSilent();

    if( EnableAdjustments )
    {
        SetScaledTime( GetScaledTime() + Delta );
    }

    LeaveModule();
}

void
cAdjustableClock::SetScaledTime( const simtime_t ScaledTime )
{
    EnterModuleSilent();

    if( EnableAdjustments )
    {
        simtime_t   Delta = ScaledTime - GetScaledTime();

        SetScalePoint( GetHwTime(), ScaledTime, GetScaleFactor_ppb(), true );
        HandleTimeJump( Delta );
    }

    LeaveModule();
}

void
cAdjustableClock::SetScaleFactor_ppb( const int64_t ScaleFactor_ppb )
{
    EnterModuleSilent();

    if( EnableAdjustments )
    {
        SetScalePoint( GetHwTime(), GetScaledTime(), ScaleFactor_ppb, false );
    }

    LeaveModule();
}

// ------------------------------------------------------
// Clock API
// ------------------------------------------------------
simtime_t
cAdjustableClock::GetScaledTime()
{
    LastScaledTime = GetScaledTimeAtHwTime( GetHwTime() );

    return LastScaledTime;
}

cLocalTimeStamp
cAdjustableClock::GetTimeStamp()
{
    return cLocalTimeStamp( EpochID, GetScaledTime() );
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
int64_t
cAdjustableClock::GetScaleFactor_ppb()
{
    // Remark: EnterModule omitted as function only return a simple value

    return ScalePoints[1].ScaleFactor_ppb;
}

int64_t
cAdjustableClock::GetScaleFactor_LowerBound_ppb()
{
    // Remark: EnterModule omitted as function only return a simple value

    return ScaleFactor_LowerBound_ppb;
}

int64_t
cAdjustableClock::GetScaleFactor_UpperBound_ppb()
{
    // Remark: EnterModule omitted as function only return a simple value

    return ScaleFactor_UpperBound_ppb;
}
