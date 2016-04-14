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

#ifndef LIBPTP_PTP_TIME_PROPERTIES_DS_H_
#define LIBPTP_PTP_TIME_PROPERTIES_DS_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_PrimitiveDataTypes.h"
#include "SubmoduleInitBase.h"

// ======================================================
// Types
// ======================================================

class   PTP_Stack;

// ======================================================
// Declarations
// ======================================================

// Time properties data set, see clause 8.2.4 in IEEE 1588-2008
class cTimePropertiesDS : public cSubmoduleInitBase
{
    private:

        // Resources

        // Static members
        // --

        // Dynamic members
        Integer16               currentUtcOffset;                               // [s]  Offset UTC <-> TAI, currently 33s
        bool                    currentUtcOffsetValid;                          //
        bool                    leap59;                                         //
        bool                    leap61;                                         //
        bool                    timeTraceable;                                  //
        bool                    frequencyTraceable;                             //
        bool                    ptpTimescale;                                   //
        timeSource_t            timeSource;                                     //

        // Configurable members
        // --

    protected:

    public:
        // Constructors/Destructor
        cTimePropertiesDS();

        // Setters
        void  SetCurrentUtcOffset       ( Integer16     currentUtcOffset        );
        void  SetCurrentUtcOffsetValid  ( bool          currentUtcOffsetValid   );
        void  SetLeap59                 ( bool          leap59                  );
        void  SetLeap61                 ( bool          leap61                  );
        void  SetTimeTraceable          ( bool          timeTraceable           );
        void  SetFrequencyTraceable     ( bool          frequencyTraceable      );
        void  SetPtpTimescale           ( bool          ptpTimescale            );
        void  SetTimeSource             ( timeSource_t  timeSource              );

        // Getters
        Integer16               GetCurrentUtcOffset();
        bool                    GetCurrentUtcOffsetValid();
        bool                    GetLeap59();
        bool                    GetLeap61();
        bool                    GetTimeTraceable();
        bool                    GetFrequencyTraceable();
        bool                    GetPtpTimescale();
        timeSource_t            GetTimeSource();

        // Operators
        bool                operator== (const cTimePropertiesDS& other);
        cTimePropertiesDS&  operator= (const cTimePropertiesDS& other);

        // Debug functions
        void    Print();
};


#endif
