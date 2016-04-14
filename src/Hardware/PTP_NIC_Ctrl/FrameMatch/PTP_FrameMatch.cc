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

#include "PTP_FrameMatch.h"
#include "PTP_NIC_Ctrl.h"

// ======================================================
// Definitions
// ======================================================

// ======================================================
// Types
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
// Constructor
// ------------------------------------------------------
PTP_FrameMatch::PTP_FrameMatch( std::string InstanceName )
    :  cSubmoduleInitBase(), InstanceName( InstanceName )
{
    initialized = false;
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
PTP_FrameMatch::RegisterSignals()
{
    Push_SigId  = pParentModule->registerSignal( (std::string( "Push" ) + InstanceName).c_str() );
    Pop_SigId   = pParentModule->registerSignal( (std::string( "Pop"  ) + InstanceName).c_str() );
}

void
PTP_FrameMatch::InitSignals()
{
}

void
PTP_FrameMatch::FinishInit()
{
    initialized = true;
}

// ------------------------------------------------------
// API functions
// ------------------------------------------------------

void
PTP_FrameMatch::push( cPortIdentity sourcePortIdentity, uint16_t sequenceId, simtime_t time )
{
    if( !initialized )
        throw cRuntimeError( "FrameMatcher was not initialized before use" );

    pParentModule->emit( Push_SigId, sequenceId );

    FrameMatch_t    Match;

    Match.sourcePortIdentity    = sourcePortIdentity;
    Match.sequenceId            = sequenceId;
    Match.Time                  = time;

    Queue.push_back(Match);
}

PTP_FrameMatch::MatchResult_t
PTP_FrameMatch::pop( cPortIdentity sourcePortIdentity, uint16_t sequenceId )
{
    if( !initialized )
        throw cRuntimeError( "FrameMatcher was not initialized before use" );

    pParentModule->emit( Pop_SigId, sequenceId );

    MatchResult_t res;

    std::vector<FrameMatch_t>::iterator it = Queue.begin();
    while( it != Queue.end() )
    {
        if
        (
            ( it->sourcePortIdentity == sourcePortIdentity) &&
            ( it->sequenceId         <  sequenceId )
        )
        {
            it = Queue.erase(it);
        }
        else if
        (
            ( it->sourcePortIdentity == sourcePortIdentity) &&
            ( it->sequenceId         == sequenceId )
        )
        {
            res.valid   = true;
            res.Time    = it->Time;

            it = Queue.erase(it);

            return res;
        }
        else
        {
            ++it;
        }
    }

    EV << InstanceName << ": failed to match frames" << endl;

    res.valid   = false;
    res.Time    = SIMTIME_ZERO;

    return res;
}


