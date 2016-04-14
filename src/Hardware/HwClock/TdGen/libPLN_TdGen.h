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

#ifndef LIBPTP_LIBPLN_TD_GEN_H_
#define LIBPTP_LIBPLN_TD_GEN_H_

#ifdef HAS_LIBPLN

// ======================================================
// Includes
// ======================================================

#include "ITdGen.h"

#include "libPLN.hpp"

#include "TdGen_ParameterParser.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cLibPLN_TdGen: public ITdGen
{
    private:

        // Resources
        TdOracle            *pTdGen;

        // Config
        unsigned    int     Seed;
        bool                AllowSkipping;
        LibPLN_Example_t    ExampleType;

    protected:

        // Init API
        void    ParseResourceParameters();
        void    AllocateResources();
        void    ParseParameters();

    public:

        cLibPLN_TdGen();
        ~cLibPLN_TdGen();

        // Time readout
        simtime_t   GetTD( simtime_t RealTime );
};

#endif

#endif

