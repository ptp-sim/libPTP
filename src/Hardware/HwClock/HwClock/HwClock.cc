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

#include "HwClock.h"

#include "PerfectTdGen.h"
#include "ConstDriftTdGen.h"
#include "SineTdGen.h"
#include "libPLN_TdGen.h"

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

Define_Module(cHwClock);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// OMNeT API
// ------------------------------------------------------
void
cHwClock::handleMessage(cMessage *pMsg)
{
    // Handle self-message (can only be schedule App msgs)
    if( pMsg->isSelfMessage() )
    {
        if( pTimeTraceMsg == pMsg )
        {
            emit( TimeDeviation_SigId, pTdGen->GetTD( simTime() ) );
            emit( HwTime_SigId, GetHwTime() );

            scheduleAt( simTime() + TimeTraceInterval, pTimeTraceMsg );
        }
        else
        {
            throw cRuntimeError( "HwClock: Received unexpected message" );
            delete pMsg;
        }
    }
    else
    {
        throw cRuntimeError( "HwClock: Received unexpected message" );
        delete pMsg;
    }
}

void
cHwClock::finish()
{
}

// ------------------------------------------------------
// Time conversion
// ------------------------------------------------------
simtime_t
cHwClock::GetRealTimeBeforeHwTime( const simtime_t HwTimeRequ )
{
    simtime_t   NowHwTime = GetHwTime();
    simtime_t   HwDiff = HwTimeRequ - NowHwTime;

    assert( HwDiff >= SIMTIME_ZERO );

    simtime_t   RealDiff    = HwDiff / 2;
    simtime_t   RealTime    = simTime() + RealDiff;
    simtime_t   HwTime      = GetHwTimeAtRealTime( RealTime );

    while( HwTime > HwTimeRequ )
    {
        RealDiff    /= 2;
        RealTime    = simTime() + RealDiff;
        HwTime      = GetHwTimeAtRealTime( simTime() + RealDiff );
    }

    return RealTime;
}

simtime_t
cHwClock::GetHwTimeAtRealTime( const simtime_t RealTime )
{
    uint64_t    Cnt = (RealTime + T0 + pTdGen->GetTD( RealTime )) / TickLenNom;

    return Cnt * TickLenNom;
}

simtime_t
cHwClock::RoundDownToTickLen( const simtime_t HwTime ) const
{
    // TODO: Use standard function 'remainder(a,b)' instead

    // Round to multiple of nominal TickLen
    int64_t     HwTime_ps   = HwTime.inUnit( SIMTIME_PS );
    int64_t     Fraction_ps = HwTime_ps % TickLenNom_ps;

    simtime_t   HwTimeRounded   = simtime_t( (HwTime_ps - Fraction_ps), SIMTIME_PS );

    assert( HwTimeRounded <= HwTime );
    assert( HwTimeRounded.inUnit(SIMTIME_PS) % TickLenNom_ps == 0 );

    return  HwTimeRounded;
}

simtime_t
cHwClock::RoundUpToTickLen( const simtime_t HwTime ) const
{
    simtime_t   HwTimeRounded   = RoundDownToTickLen( HwTime );

    if( HwTime != HwTimeRounded )
    {
        HwTimeRounded += TickLenNom;
    }

    assert( HwTimeRounded >= HwTime );
    assert( HwTimeRounded.inUnit(SIMTIME_PS) % TickLenNom_ps == 0 );

    return  HwTimeRounded;
}

// ------------------------------------------------------
// Init API
// ------------------------------------------------------
void
cHwClock::ParseResourceParameters()
{
    TdGenType           = cHwClock_ParameterParser::ParseTdGenType( par("TdGenType").stringValue() );
    TimeTraceInterval   = simtime_t( par("TimeTraceInterval").doubleValue() );

    if( TimeTraceInterval != SIMTIME_ZERO )
    {
        EnableTimeTracing   = true;
    }
}

void
cHwClock::AllocateResources()
{
    switch( TdGenType )
    {
        case TDGEN_TYPE_PERFECT:        pTdGen  = new cPerfectTdGen();
                                        break;

        case TDGEN_TYPE_CONST_DRIFT:    pTdGen  = new cConstDriftTdGen();
                                        break;

        case TDGEN_TYPE_SINE:           pTdGen  = new cSineTdGen();
                                        break;

        case TDGEN_TYPE_LIBPLN:         // Check if we were configured to use libPLN
                                        #ifdef HAS_LIBPLN
                                            pTdGen  = new cLibPLN_TdGen();
                                        #else
                                            EV << "===================================================================================================" << endl;
                                            EV << "Warning: TdGen was configured to use libPLN, but it is not available. Using PerfectTdGen instead."   << endl;
                                            EV << "===================================================================================================" << endl;
                                            pTdGen  = new cPerfectTdGen();
                                        #endif

                                        break;

        default:                        throw cRuntimeError( "Unsupported TdGenType" );
                                        break;
    }

    if( EnableTimeTracing )
    {
        pTimeTraceMsg       = new cMessage("HwClock: Trace time values" );
    }
}

void
cHwClock::InitHierarchy()
{
    pTdGen->SetParentModule( this );
}

void
cHwClock::ParseParameters()
{
    TickLenNom      = simtime_t( par("TickLenNom").doubleValue() );
    TickLenNom_ps   = TickLenNom.inUnit( SIMTIME_PS );

    if( TickLenNom_ps == 0 )
    {
        throw cRuntimeError( "Invalid value for parameter TickLenNom" );
    }

    T0 = TickLenNom * par("InitialPhaseNom").doubleValue();
}

void
cHwClock::RegisterSignals()
{
    TimeDeviation_SigId = registerSignal( "TimeDeviation"   );
    HwTime_SigId        = registerSignal( "HwTime"          );
    RequHwTime_SigId    = registerSignal( "RequestedHwTime" );
}

void
cHwClock::InitInternalState()
{
    if( EnableTimeTracing )
    {
        scheduleAt( simulation.getWarmupPeriod() + TimeTraceInterval, pTimeTraceMsg );
    }
}

void
cHwClock::InitSignals()
{
    emit( HwTime_SigId, GetHwTime() );
    emit( TimeDeviation_SigId, pTdGen->GetTD( simTime() ) );
}

void
cHwClock::FinishInit()
{
}

void
cHwClock::ForwardInit( int stage )
{
    pTdGen->initialize( stage );
}

// ------------------------------------------------------
// Debug API
// ------------------------------------------------------
simtime_t
cHwClock::GetTD( simtime_t RealTime )
{
    return pTdGen->GetTD( RealTime );
}

// ------------------------------------------------------
// Constructors/Destructor
// ------------------------------------------------------

cHwClock::cHwClock()
{
    pTdGen              = nullptr;
    pTimeTraceMsg       = nullptr;
    EnableTimeTracing   = false;
}

cHwClock::~cHwClock()
{
    delete pTdGen;

    if( EnableTimeTracing )
    {
        cancelAndDelete( pTimeTraceMsg );
    }
}

// ------------------------------------------------------
// HwClock API
// ------------------------------------------------------
simtime_t
cHwClock::GetHwTime()
{
    return GetHwTimeAtRealTime( simTime() );
}
