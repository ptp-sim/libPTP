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

#ifndef LIBPTP_PTP_PARENTDS_H_
#define LIBPTP_PTP_PARENTDS_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_PrimitiveDataTypes.h"
#include "PTP_PortIdentity.h"
#include "PTP_ClockIdentity.h"
#include "PTP_ClockQuality.h"
#include "SubmoduleInitBase.h"

// ======================================================
// Types
// ======================================================

class   PTP_Stack;

// ======================================================
// Declarations
// ======================================================

// Parent data set, see clause 8.2.3 in IEEE 1588-2008
class cParentDS : public cSubmoduleInitBase
{
    private:

        // Resources

        // Static members
        // --

        // Dynamic members
        cPortIdentity           parentPortIdentity;                             //
        bool                    parentStats;                                    //
        UInteger16              observedParentOffsetScaledLogVariance;          //
        Integer32               observedParentClockPhaseChangeRate;             //
        cClockIdentity          grandmasterIdentity;                            //
        cClockQuality           grandmasterClockQuality;                        //
        uint8_t                 grandmasterPriority1;                           //
        uint8_t                 grandmasterPriority2;                           //

        int                     GrandMasterModuleID;

        // Configurable members
        // --

    protected:

    public:

        // Constructors/Destructor
        cParentDS();

        // Instance methods
        cPortIdentity&      ParentPortIdentity();
        cClockQuality&      GrandmasterClockQuality();

        // Setters
        void    SetParentStats                              ( bool                    parentStats                          );
        void    SetObservedParentOffsetScaledLogVariance    ( UInteger16              observedParentOffsetScaledLogVariance);
        void    SetObservedParentClockPhaseChangeRate       ( Integer32               observedParentClockPhaseChangeRate   );
        void    SetGrandmasterIdentity                      ( cClockIdentity          grandmasterIdentity                  );
        void    SetGrandmasterPriority1                     ( uint8_t                 grandmasterPriority1                 );
        void    SetGrandmasterPriority2                     ( uint8_t                 grandmasterPriority2                 );
        void    SetGrandMasterModuleID                      ( int                     GrandMasterModuleID                  );

        // Getters
        bool                GetParentStats();
        UInteger16          GetObservedParentOffsetScaledLogVariance();
        Integer32           GetObservedParentClockPhaseChangeRate();
        cClockIdentity      GetGrandmasterIdentity();
        uint8_t             GetGrandmasterPriority1();
        uint8_t             GetGrandmasterPriority2();
        int                 GetGrandMasterModuleID();

        // Operators
        bool            operator== (const cParentDS& other);
        cParentDS&      operator= (const cParentDS& other);

        // Debugn functions
        void    Print();

        // Friends
        friend PTP_Stack;
};

#endif
