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

#include "TimeDiffObserver.h"

#include <cmath>

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

Define_Module(cTimeDiffObserver);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Internal functions
// ------------------------------------------------------
simtime_t
cTimeDiffObserver::RoundSimtime( simtime_t t, int Exponent )
{
    if( Exponent <= t.getScaleExp() )
    {
        return t;
    }

    if( t >= SIMTIME_ZERO )
    {
        t += simtime_t(5, Exponent-1);
    }
    else
    {
        t -= simtime_t(5, Exponent-1);
    }

    t = simtime_t( t.inUnit(Exponent), Exponent);

    return t;
}

std::string
cTimeDiffObserver::GetUnitString( int Exponent )
{
    std::string UnitString;

    switch( Exponent )
    {
        case 3:             UnitString = " ks";   break;
        case SIMTIME_S:     UnitString = " s";    break;
        case SIMTIME_MS:    UnitString = " ms";   break;
        case SIMTIME_US:    UnitString = " us";   break;
        case SIMTIME_NS:    UnitString = " ns";   break;
        case SIMTIME_PS:    UnitString = " ps";   break;
        case SIMTIME_FS:    UnitString = " fs";   break;

        default:
        {
            std::stringstream ss;
            ss << " *10^" << Exponent;
            UnitString = ss.str();
            break;
        }
    }

    return UnitString;
}

int SelectExponent( simtime_t t )
{
    if( t == SIMTIME_ZERO )
    {
        return -12;
    }

    if( t < SIMTIME_ZERO )
    {
        t = -t;
    }

    if( t >= simtime_t(1, SIMTIME_MS))
    {
        t = t;
    }

    int64_t t64 = t.inUnit( t.getScaleExp() );
    double  log = log10( t64 );
    int     ilog = log;

    ilog = ilog + t.getScaleExp();

    if( ilog < 0 )
        ilog -= 2;

    ilog /= 3;
    ilog *= 3;

    return ilog;
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
cTimeDiffObserver::ParseResourceParameters()
{
    ClockPath1 = par( "ClockPath1" ).stringValue();
    ClockPath2 = par( "ClockPath2" ).stringValue();
}

void
cTimeDiffObserver::AllocateResources()
{
    pClock1 = check_and_cast<cScheduleClock *>( getModuleByPath( ClockPath1.c_str() ) );
    pClock2 = check_and_cast<cScheduleClock *>( getModuleByPath( ClockPath2.c_str() ) );

    pTraceMsg = new cMessage("TimeDiffObserver: Trace TimeDiff" );
}

void
cTimeDiffObserver::ParseParameters()
{
    TraceInterval           = simtime_t( par("TraceInterval").doubleValue() );
    GuiExponent             = par( "GuiExponent" ).longValue();
    AutomaticGuiExponent    = par( "AutomaticGuiExponent").boolValue();
}

void
cTimeDiffObserver::RegisterSignals()
{
    // Register signals
    HwTimeDiff_SigId        = registerSignal( "HwTimeDiff" );
    ScaledTimeDiff_SigId    = registerSignal( "ScaledTimeDiff" );
}

void
cTimeDiffObserver::InitInternalState()
{
}

void
cTimeDiffObserver::InitSignals()
{
}

void
cTimeDiffObserver::FinishInit()
{
    if( TraceInterval != 0 )
    {
        scheduleAt( simulation.getWarmupPeriod() + TraceInterval, pTraceMsg );
    }
}

// ------------------------------------------------------
// Handle messages
// ------------------------------------------------------
void
cTimeDiffObserver::handleMessage(cMessage *pMsg)
{
    if( pMsg == pTraceMsg )
    {
        simtime_t   ScaledTimeDiff  = pClock1->GetScaledTime() - pClock2->GetScaledTime();

        emit( ScaledTimeDiff_SigId, ScaledTimeDiff );

        int Exponent = AutomaticGuiExponent ? SelectExponent( ScaledTimeDiff ) : GuiExponent;

        std::stringstream   ss;

        ss << "TimeDiff: " << RoundSimtime( ScaledTimeDiff, Exponent ).inUnit( Exponent ) << GetUnitString( Exponent );
        DisplayString   = ss.str();

        getDisplayString().setTagArg("t", 0, DisplayString.c_str());

        scheduleAt( simTime() + TraceInterval, pTraceMsg );
    }
}

// ------------------------------------------------------
// Finish
// ------------------------------------------------------
void
cTimeDiffObserver::finish()
{
}

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
cTimeDiffObserver::cTimeDiffObserver()
{
    DisplayString   = "TimeDiff: no data yet";
}

// ------------------------------------------------------
// Desctructor
// ------------------------------------------------------
cTimeDiffObserver::~cTimeDiffObserver()
{
    cancelAndDelete( pTraceMsg );
}

