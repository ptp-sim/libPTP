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

#ifndef LIBPTP_APP_STATE_DECISION_H_
#define LIBPTP_APP_STATE_DECISION_H_

// ======================================================
// Includes
// ======================================================

#include "AppService.h"

#include <string>

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cAppStateDecision  : public cAppService
{
    private:

        // Signals
        simsignal_t     EbestModuleID_SigId;

        // Internal functions
        void    HandleIntervalEvent();

    protected:

        // Resources
        std::string     Tooltip;
        cModule         *pTooltipModule;

        // Configuration
        std::string     TooltipPath;
        bool            EnableTooltip;

        // Debug configuration
        bool            EnableBriefDebugOutput;
        bool            EnableDetailedDebugOutput;

    public:

        // Constructors
        cAppStateDecision();

        // Destructor
        ~cAppStateDecision();

        // Setters

        // Getters

        // Initialization
        void    ParseResourceParameters();
        void    AllocateResources();
        void    ParseParameters();
        void    RegisterSignals();
        void    InitSignals();
};

#endif
