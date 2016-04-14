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

#ifndef LIBPTP_PTP_DEFAULTDS_H_
#define LIBPTP_PTP_DEFAULTDS_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

#include "PTP_PrimitiveDataTypes.h"
#include "PTP_ClockIdentity.h"
#include "PTP_ClockQuality.h"

// ======================================================
// Types
// ======================================================

class   PTP_Stack;

// ======================================================
// Declarations
// ======================================================

// Default data set, see 8.2.1
class cDefaultDS
{
    private:

        // Resources
        PTP_Stack               *pApp;

        // Static members
        bool                    twoStepFlag;                // On-the-fly mode or FollowUp frames
        cClockIdentity          clockIdentity;              //
        size_t                  numberPorts;                //

        // Dynamic members
        cClockQuality           clockQuality;               //

        // Configurable members
        uint8_t                 priority1;                  //
        uint8_t                 priority2;                  //
        domainNumber_t          domainNumber;               //
        bool                    slaveOnly;                  //

    protected:

    public:
        // Constructors/Destructor
        cDefaultDS();

        // Init
        void SetHierarchy( PTP_Stack *pApp );

        // Instance methods
        cClockIdentity  &ClockIdentity();
        cClockQuality   &ClockQuality();

        // Setters
        void    SetTwoStepFlag  ( bool              twoStepFlag );
        void    SetNumberPorts  ( size_t            numberPorts);
        void    SetPriority1    ( uint8_t           priority1);
        void    SetPriority2    ( uint8_t           priority2);
        void    SetDomainNumber ( domainNumber_t    domainNumber);
        void    SetSlaveOnly    ( bool              slaveOnly);

        // Getters
        bool                    GetTwoStepFlag()    const;
        size_t                  GetNumberPorts()    const;
        uint8_t                 GetPriority1()      const;
        uint8_t                 GetPriority2()      const;
        domainNumber_t          GetDomainNumber()   const;
        bool                    GetSlaveOnly()      const;
        cClockIdentity          GetClockIdentity()  const;
        cClockQuality           GetClockQuality()   const;
        int                     GetModuleID()       const;

        // Debug functions
        void    Print();
};

#endif
