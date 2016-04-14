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

#ifndef LIBPTP_PTP_CURRENTDS_H_
#define LIBPTP_PTP_CURRENTDS_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "PTP_TimeInterval.h"
#include "SubmoduleInitBase.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

// Current data set, see clause 8.2.2 in IEEE 1588-2008
class cCurrentDS: public cSubmoduleInitBase
{
    private:

        // Resources

        // Static members
        // --

        // Dynamic members
        size_t                  stepsRemoved;                   //
        cTimeInterval           offsetFromMaster;               //
        cTimeInterval           meanPathDelay;                  //

        // Configurable members
        // --

        // ------------------------------------------------------------
        // Signals for statistics
        // ------------------------------------------------------------

        // Current DS
        simsignal_t             stepsRemoved_SigId;
        simsignal_t             offsetFromMaster_SigId;
        simsignal_t             meanPathDelay_SigId;

    protected:

    public:
        // Constructors/Destructor
        cCurrentDS();

        // Init API
        void    RegisterSignals();
        void    InitSignals();

        // Instance methods

        // Setters
        void    SetStepsRemoved     ( size_t            stepsRemoved        );
        void    SetOffsetFromMaster ( cTimeInterval     offsetFromMaster    );
        void    SetMeanPathDelay    ( cTimeInterval     meanPathDelay       );

        // Getters
        size_t          GetStepsRemoved();
        cTimeInterval   GetOffsetFromMaster();
        cTimeInterval   GetMeanPathDelay();

        // Operators
        bool            operator== (const cCurrentDS& other);
        cCurrentDS&     operator= (const cCurrentDS& other);

        // Debug functions
        void    Print();
};

#endif
