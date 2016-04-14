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

#include "PTP_Stack.h"

#include "AppStateDecision.h"

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
cAppStateDecision::cAppStateDecision()
    : cAppService()
{
}

// ------------------------------------------------------
// Destructor
// ------------------------------------------------------
cAppStateDecision::~cAppStateDecision()
{
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------

// ------------------------------------------------------
// Getters
// ------------------------------------------------------

// ------------------------------------------------------
// API Functions
// ------------------------------------------------------

// This functions implements the procedure described in 9.3.2.2
// Steps a) and b) are handled inside the foreignMasterDS of each port
// Step c) Calculate E_best
void
cAppStateDecision::HandleIntervalEvent()
{
    UInteger16          EbestPortID     = 0;

    if( EnableBriefDebugOutput )
    {
        EV << "========================================================================" << endl;
        EV << "           State Decision           " << endl;
        EV << "========================================================================" << endl;
        EV << endl;
        EV << "Step 1: Calculating current Ebest" << endl;
    }

    pApp->Ebest.Clear();
    for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
    {
        cForeignClockDS     Erbest  = pApp->pPorts[PortIdx].GetErbest();

        if( EnableBriefDebugOutput )
        {
            EV << "Checking Erbest of Port " << PortIdx + 1 << endl;
        }
        if( !Erbest.IsEmpty() )
        {
            ClockCompReturn_t   CompReturn  = cForeignClockDS::CompareClockDS( pApp->Ebest, Erbest, EnableDetailedDebugOutput );

            if( EnableBriefDebugOutput )
            {
                EV << "   CompReturn.Result: " << CompReturn.Result << endl;
                EV << "   CompReturn.Reason: " << CompReturn.Reason << endl;
            }

            switch( CompReturn.Result )
            {
                case ClockCompResult::B_BETTER_A:
                case ClockCompResult::B_BETTER_A_BY_TOPO:

                    pApp->Ebest = Erbest;
                    EbestPortID = PortIdx + 1;
                    break;

                default:
                    break;
            }
        }
        else
        {
            if( EnableBriefDebugOutput )
            {
                EV << "   Erbest of Port " << PortIdx + 1 << " is empty" << endl;
            }
        }
    }

    pParentModule->emit( EbestModuleID_SigId, pApp->Ebest.GetModuleID() );

    if( EnableBriefDebugOutput )
    {
        EV << "Current Ebest: ";
        if( pApp->Ebest.IsEmpty() )
        {
            EV << "empty" << endl;
        }
        else
        {
            EV << pApp->Ebest.ClockIdentity().GetString() << endl;
        }
        EV << endl;
        EV << "Step 2: Calculating StateDecision for all ports" << endl;
        EV << endl;
    }

    bool NewMasterFlag  = pApp->Ebest.SenderPortIdentity() != pApp->parentDS.ParentPortIdentity();

    // Step d) Apply state decision algorithm for each port
    std::vector<portStateDecision_t>    StateDecisions;

    for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
    {
        cPTP_Port               *pPort          = &pApp->pPorts[PortIdx];
        portStateDecision_t     StateDecision   = pPort->GetStateDecision( pApp->Ebest, EbestPortID, EnableDetailedDebugOutput );

        StateDecisions.push_back( StateDecision );

        if( EnableBriefDebugOutput )
        {
            EV << "State Decision for Port " << PortIdx + 1 << ": " << StateDecision << endl;
        }
    }

    for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
    {
        pApp->HandlePortStateDec( StateDecisions[PortIdx] );
    }

    for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
    {
        pApp->pPorts[PortIdx].HandleStateDecision( StateDecisions[PortIdx], NewMasterFlag );

        if( EnableBriefDebugOutput )
        {
            EV << endl;
        }
    }
    if( EnableBriefDebugOutput )
    {
        EV << "========================================================================" << endl;
    }

    // Update tool tip
    if( EnableTooltip )
    {
        std::stringstream   ss;
        for( int PortIdx = 0; PortIdx < pApp->PtpGateSize; PortIdx ++ )
        {
            cPTP_Port           *pPort  = &pApp->pPorts[PortIdx];

            ss << "Port " << PortIdx + 1 << ": " << pPort->PortDS().GetPortState();

            if( PortIdx < pApp->PtpGateSize-1 )
            {
                ss << endl;
            }
        }

        Tooltip = ss.str();

        pTooltipModule->getDisplayString().setTagArg("tt", 0, Tooltip.c_str());
    }
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------

void
cAppStateDecision::ParseResourceParameters()
{
    cAppService::ParseResourceParameters();

    TooltipPath     = pParentModule->par( "StateDec_TooltipPath" ).stringValue();
    EnableTooltip   = TooltipPath.empty() ? false : true;
}

void
cAppStateDecision::AllocateResources()
{
    cAppService::AllocateResources();

    pTooltipModule  = EnableTooltip ? pParentModule->getModuleByPath( TooltipPath.c_str() ) : nullptr;
}

void
cAppStateDecision::ParseParameters()
{
    cAppService::ParseParameters();

    EnableBriefDebugOutput      = pParentModule->par( "StateDecision_EnableBriefDebugOutput" ).boolValue();
    EnableDetailedDebugOutput   = pParentModule->par( "StateDecision_EnableDetailedDebugOutput" ).boolValue();
}

void
cAppStateDecision::RegisterSignals()
{
    cAppService::RegisterSignals();

    EbestModuleID_SigId = pParentModule->registerSignal("EbestModuleID");
}
void
cAppStateDecision::InitSignals()
{
    cAppService::InitSignals();
}
