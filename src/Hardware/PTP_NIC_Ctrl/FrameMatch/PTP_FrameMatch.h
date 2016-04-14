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

#ifndef LIBPTP_PTP_FRAME_MATCH_H_
#define LIBPTP_PTP_FRAME_MATCH_H_

// ======================================================
// Includes
// ======================================================

#include "PTP.h"
#include "SubmoduleInitBase.h"

// ======================================================
// Types
// ======================================================

class PTP_NIC_Ctrl;

// ======================================================
// Declarations
// ======================================================

class PTP_FrameMatch : public cSubmoduleInitBase
{
    private:

        struct FrameMatch_t
        {
            cPortIdentity   sourcePortIdentity;
            uint16_t        sequenceId;
            simtime_t       Time;
        };

    // ------------------------------------------------------------
    // Resources
    // ------------------------------------------------------------
    std::vector<FrameMatch_t>   Queue;

    bool                        initialized;

    simsignal_t                 Push_SigId;
    simsignal_t                 Pop_SigId;

    // ------------------------------------------------------------
    // Internal housekeeping
    // ------------------------------------------------------------

    // ------------------------------------------------------------
    // Configuration
    // ------------------------------------------------------------
    std::string                 InstanceName;

    // ------------------------------------------------------------
    // Internal functions
    // ------------------------------------------------------------

    // Init API
    void    RegisterSignals();
    void    InitSignals();
    void    FinishInit();

    protected:

    public:

        struct MatchResult_t
        {
            bool        valid;
            simtime_t   Time;
        };

        // Constructor
        PTP_FrameMatch( std::string InstanceName );

        // API
        void            push( cPortIdentity sourcePortIdentity, uint16_t sequenceId, simtime_t time );
        MatchResult_t   pop( cPortIdentity sourcePortIdentity, uint16_t sequenceId );
};

#endif

