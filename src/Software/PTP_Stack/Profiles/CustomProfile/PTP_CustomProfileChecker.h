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

#ifndef LIBPTP_PTP_CUSTOM_PROFILE_CHECKER_H_
#define LIBPTP_PTP_CUSTOM_PROFILE_CHECKER_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_ProfileChecker.h"

// ======================================================
// Types
// ======================================================

class PTP_CustomProfileChecker: public PTP_ProfileChecker
{
    private:

        // ------------------------------------------------------------
        // Resources
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Configuration
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Internal state handling
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Internal utilities
        // ------------------------------------------------------------

    protected:

        // ------------------------------------------------------------
        // Init API
        // ------------------------------------------------------------
        void    ParseParameters();

    public:

        // ------------------------------------------------------------
        // Constructors/Destructor
        // ------------------------------------------------------------
        PTP_CustomProfileChecker();
        ~PTP_CustomProfileChecker();

        // ------------------------------------------------------------
        // Setters
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Getters
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // PTP Profile handling
        // ------------------------------------------------------------
        void    CheckParametersFixed( PTP_Stack *pApp );
        void    CheckParametersInit( PTP_Stack *pApp );
        void    CheckParametersRange( PTP_Stack *pApp );
};

// ======================================================
// Declarations
// ======================================================

#endif
