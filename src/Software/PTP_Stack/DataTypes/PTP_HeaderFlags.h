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

#ifndef LIBPTP_PTP_HEADER_FLAGS_H_
#define LIBPTP_PTP_HEADER_FLAGS_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_PrimitiveDataTypes.h"
#include "PTP_Constants.h"
#include "PTP_ByteBuffers.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cPtpHeaderFlags
{
    public:

        Boolean alternateMasterFlag;
        Boolean twoStepFlag;
        Boolean unicastFlag;
        Boolean ptpProfileSpecific_1;
        Boolean ptpProfileSpecific_2;
        Boolean reserved;
        Boolean leap61;
        Boolean leap59;
        Boolean currentUtcOffsetValid;
        Boolean ptpTimescale;
        Boolean timeTraceable;
        Boolean frequencyTraceable;

        // Constructors/Destructor
        cPtpHeaderFlags();
        cPtpHeaderFlags( const Octet flagField[] );
        cPtpHeaderFlags( const cPtpHeaderFlags& other );
        ~cPtpHeaderFlags();

        // Setters
        void    ReadFromBuffer( const Octet flagField[] );

        // Getters
        void    SaveToBuffer( Octet flagField[] );

        // Operators
        bool                operator== (const cPtpHeaderFlags& other)    const;
        bool                operator!= (const cPtpHeaderFlags& other)    const;
        cPtpHeaderFlags&    operator=  (const cPtpHeaderFlags& other);

        friend std::ostream& operator<<(std::ostream& os, const cPtpHeaderFlags& o);

        // Debug functions
        void            Print()     const;
        std::string     GetString() const;
};

#endif
