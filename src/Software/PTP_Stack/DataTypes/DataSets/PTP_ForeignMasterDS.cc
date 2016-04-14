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

#include "PTP_ForeignMasterDS.h"
#include "PTP_ForeignClockDS.h"

#include "PTP_Port.h"

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
// Constructors/Destructor
// ------------------------------------------------------
cForeignMasterDS::cForeignMasterDS()
{
    this->pPort                     = nullptr;
    this->pClock                    = nullptr;
    this->ForeignMasterTimeWindow   = SIMTIME_ZERO;

    this->Erbest                    = cForeignClockMsg();
}

cForeignMasterDS::cForeignMasterDS( const cForeignMasterDS& other )
{
    this->pPort                     = other.pPort;
    this->pClock                    = other.pClock;
    this->ForeignMasterTimeWindow   = other.ForeignMasterTimeWindow;

    this->ForeignMasters            = other.ForeignMasters;
    this->Erbest                    = other.Erbest;
}

cForeignMasterDS::~cForeignMasterDS()
{
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------

// ------------------------------------------------------
// Init functions
// ------------------------------------------------------
void
cForeignMasterDS::SetHierarchy( cPTP_Port *pPort, cScheduleClock *pClock )
{
    this->pPort     = pPort;
    this->pClock    = pClock;
}

// ------------------------------------------------------
// Init API
// ------------------------------------------------------
void
cForeignMasterDS::RegisterSignals()
{
    ForeinMasterDS_Cnt_SigId = pPort->RegisterDynamicSignal( "ForeignMasterDS_Cnt" );
}

void
cForeignMasterDS::InitSignals()
{
    pParentModule->emit( ForeinMasterDS_Cnt_SigId, 0 );
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------

void
cForeignMasterDS::SetForeignMasterTimeWindow( simtime_t ForeignMasterTimeWindow )
{
    this->ForeignMasterTimeWindow   = ForeignMasterTimeWindow;

    for(std::vector<cForeignMasterDS_Entry>::iterator it = ForeignMasters.begin(); it != ForeignMasters.end(); ++it)
    {
        it->SetForeignMasterTimeWindow( ForeignMasterTimeWindow );
    }
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------

// ------------------------------------------------------
// API functions
// ------------------------------------------------------

void
cForeignMasterDS::CalculateErbest()
{
    Erbest.ForeignClockDS().Clear();

    // Loop over all remaining entries and try to find a new Erbest
    for(std::vector<cForeignMasterDS_Entry>::iterator it = ForeignMasters.begin(); it != ForeignMasters.end(); ++it)
    {
        if( it->GetNumEntries() >= PTP_FOREIGN_MASTER_THRESHOLD )
        {
            cForeignClockMsg    Candidate   = it->GetMostRecentEntry();
            ClockCompReturn_t   CompReturn  = cForeignClockDS::CompareClockDS( Erbest.GetForeignClockDS(), Candidate.GetForeignClockDS(), false );

            switch( CompReturn.Result )
            {
                case ClockCompResult::B_BETTER_A:
                case ClockCompResult::B_BETTER_A_BY_TOPO:

                    Erbest   = Candidate;
                    break;

                default:
                    break;
            }
        }
    }
}

bool
cForeignMasterDS::IsErbestValid()
{
    simtime_t   Now = pClock->GetScaledTime();

    if( Now <= this->ForeignMasterTimeWindow  )
        return true;

    if( Erbest.GetRxTime() >= (pClock->GetScaledTime() - this->ForeignMasterTimeWindow) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void
cForeignMasterDS::PushForeignClock( cForeignClockDS ForeignClockDS, simtime_t RxTime )
{
    // Get rid of old values
    std::vector<cForeignMasterDS_Entry>::iterator it = ForeignMasters.begin();
    while( it != ForeignMasters.end() )
    {
        if( it->IsEmpty() )
        {
            // erase returns the new iterator
            it = ForeignMasters.erase( it );
        }
        else
        {
            ++it;
        }
    }
    pParentModule->emit( ForeinMasterDS_Cnt_SigId, ForeignMasters.size() );

    // Try to find existing entry with that ID
    bool    NewDataFlag;

    for(std::vector<cForeignMasterDS_Entry>::iterator it = ForeignMasters.begin(); it != ForeignMasters.end(); ++it)
    {
        if( ForeignClockDS.SenderPortIdentity() == it->PortIdentity() )
        {
            it->PushForeignClock( ForeignClockDS, RxTime, NewDataFlag );

            // TODO: Check if Erbest is still valid

            if( Erbest.ForeignClockDS().SenderPortIdentity() == ForeignClockDS.SenderPortIdentity() )
            {
                if( NewDataFlag )
                {
                    CalculateErbest();
                }
                else
                {
                    Erbest.SetRxTime( RxTime );
                }
            }
            else if( NewDataFlag )
            {
                ClockCompReturn_t   CompReturn  = cForeignClockDS::CompareClockDS( Erbest.ForeignClockDS(), ForeignClockDS, false );

                switch( CompReturn.Result )
                {
                    case ClockCompResult::B_BETTER_A:
                    case ClockCompResult::B_BETTER_A_BY_TOPO:

                        Erbest  = cForeignClockMsg( ForeignClockDS, RxTime );
                        break;

                    default:
                        break;
                }
            }

            return;
        }
    }

    // Add a new entry
    cForeignMasterDS_Entry  NewEntry;

    // Initialize new entry
    NewEntry.SetClock( pClock );
    NewEntry.SetForeignMasterTimeWindow( ForeignMasterTimeWindow );
    NewEntry.PortIdentity() = ForeignClockDS.SenderPortIdentity();

    NewEntry.PushForeignClock( ForeignClockDS, RxTime, NewDataFlag );

    ForeignMasters.push_back( NewEntry );

    pParentModule->emit( ForeinMasterDS_Cnt_SigId, ForeignMasters.size() );
}

size_t
cForeignMasterDS::GetNumEntries( cPortIdentity foreignMasterPortIdentity )
{
    // Try to find existing entry
    for(std::vector<cForeignMasterDS_Entry>::iterator it = ForeignMasters.begin(); it != ForeignMasters.end(); ++it)
    {
        if( foreignMasterPortIdentity == it->PortIdentity() )
        {
            return  it->GetNumEntries();
        }
    }

    // If entry is not found, return 0
    return  0;
}

cForeignClockDS
cForeignMasterDS::GetErbest()
{
    if( !IsErbestValid() )
    {
        CalculateErbest();
    }

    return Erbest.ForeignClockDS();
}

void
cForeignMasterDS::AdjustTimestamps( simtime_t Delta )
{
    for(std::vector<cForeignMasterDS_Entry>::iterator it = ForeignMasters.begin(); it != ForeignMasters.end(); ++it)
    {
        it->AdjustTimestamps( Delta );
    }
}

// ------------------------------------------------------
// Operators
// ------------------------------------------------------
cForeignMasterDS&
cForeignMasterDS::operator=( const cForeignMasterDS& other )
{
    this->pPort                     = other.pPort;
    this->pClock                    = other.pClock;
    this->ForeignMasterTimeWindow   = other.ForeignMasterTimeWindow;

    this->ForeignMasters            = other.ForeignMasters;
    this->Erbest                    = other.Erbest;

    // By convention, always return *this
    return *this;
}
