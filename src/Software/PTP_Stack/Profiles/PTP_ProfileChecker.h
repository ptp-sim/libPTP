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

#ifndef LIBPTP_PTP_PROFILE_CHECKER_H_
#define LIBPTP_PTP_PROFILE_CHECKER_H_

// ======================================================
// Includes
// ======================================================

#include "PTP.h"
#include "SubmoduleInitBase.h"

// ======================================================
// Types
// ======================================================

class PTP_Stack;

// ======================================================
// Declarations
// ======================================================

class PTP_ProfileChecker: public cSubmoduleInitBase
{
    private:

    protected:

        // ------------------------------------------------------------
        // Resources
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Internal state handling
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Internal utilities
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Configuration
        // ------------------------------------------------------------
        PTP_Profile_t   PTP_Profile;

        // ------------------------------------------------------------
        // Error handling
        // ------------------------------------------------------------
        void    HandleWrongInitValue( std::string ParameterName );
        void    HandleRangeError( std::string ParameterName );

        // ------------------------------------------------------------
        // Init API
        // ------------------------------------------------------------
        void    ParseParameters();

    public:

        // ------------------------------------------------------------
        // Constructors/Destructor
        // ------------------------------------------------------------
        PTP_ProfileChecker();
        virtual ~PTP_ProfileChecker();

        // ------------------------------------------------------------
        // Setters
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Getters
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // PTP Profile handling
        // ------------------------------------------------------------
        virtual void    CheckParametersFixed( PTP_Stack *pApp ) = 0;
        virtual void    CheckParametersInit( PTP_Stack *pApp ) = 0;
        virtual void    CheckParametersRange( PTP_Stack *pApp ) = 0;
};

#endif
