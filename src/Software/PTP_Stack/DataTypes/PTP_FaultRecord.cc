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

#include "PTP_FaultRecord.h"
#include "PTP_PrimitiveDataTypes.h"

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
// Constructors
// ------------------------------------------------------
cFaultRecord::cFaultRecord()
{
    this->faultTime         = 0;
    this->severityCode      = SERVIRITY_DEBUG;
    this->faultName         = "";
    this->faultValue        = "";
    this->faultDescription  = "";

    this->faultRecordLength = 0;
}

cFaultRecord::cFaultRecord
(
    simtime_t       faultTime,
    SeverityCode_t  severityCode,
    const char*     faultName,
    const char*     faultValue,
    const char*     faultDescription
)
{
    this->faultTime         = faultTime;
    this->severityCode      = severityCode;
    this->faultName         = faultName;
    this->faultValue        = faultValue;
    this->faultDescription  = faultDescription;

    // 5.3.10
    // The faultRecordLength member shall indicate the number of octets in the FaultRecord not including the 2
    // octets of the faultRecordLength member.
    this->faultRecordLength =   10 +                                        // faultTime
                                1  +                                        // severityCode
                                this->faultName.GetLengthField()    +
                                this->faultValue.GetLengthField()   +
                                this->faultDescription.GetLengthField();
}

cFaultRecord::~cFaultRecord()
{

}

// ------------------------------------------------------
// Setters/Getters
// ------------------------------------------------------
// TODO: Implement

// ------------------------------------------------------
// Compare operator
// ------------------------------------------------------
bool cFaultRecord::operator== (const cFaultRecord& other)
{
    if
    (
        ( this->faultTime            == other.faultTime         ) &&
        ( this->severityCode         == other.severityCode      ) &&
        ( this->faultName            == other.faultName         ) &&
        ( this->faultValue           == other.faultValue        ) &&
        ( this->faultDescription     == other.faultDescription  ) &&
        ( this->faultRecordLength    == other.faultRecordLength )
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ------------------------------------------------------
// Assign operator
// ------------------------------------------------------
cFaultRecord&   cFaultRecord::operator= (const cFaultRecord& other)
{
    this->faultTime         = other.faultTime;
    this->severityCode      = other.severityCode;
    this->faultName         = other.faultName;
    this->faultValue        = other.faultValue;
    this->faultDescription  = other.faultDescription;

    this->faultRecordLength = other.faultRecordLength;

    // By convention, always return *this
    return *this;
}
