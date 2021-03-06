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

#ifndef LIBPTP_PTP_FAULTRECORD_H_
#define LIBPTP_PTP_FAULTRECORD_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_TimeStamp.h"
#include "PTP_PTPText.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

// Fault record, see clause 5.3.10 in IEEE 1588-2008
class cFaultRecord
{
    private:
        UInteger16              faultRecordLength;              //
        cTimeStamp              faultTime;                      //
        SeverityCode_t          severityCode;                   //
        cPTPText                faultName;                      //
        cPTPText                faultValue;                     //
        cPTPText                faultDescription;               //

    protected:

    public:

        // Constructors
        cFaultRecord();
        cFaultRecord(   simtime_t       faultTime,
                        SeverityCode_t  severityCode,
                        const char*     faultName,
                        const char*     faultValue,
                        const char*     faultDescription
                    );
        ~cFaultRecord();

        // Setters/Getters

        // Operators
        bool            operator== (const cFaultRecord& other);
        cFaultRecord    &operator= (const cFaultRecord& other);

        // Debug functions
};


#endif
