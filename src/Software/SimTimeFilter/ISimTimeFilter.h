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

#ifndef LIBPTP_ISIMTIME_FILTER_H_
#define LIBPTP_ISIMTIME_FILTER_H_

// ======================================================
// Includes
// ======================================================

#include <omnetpp.h>

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class ISimTimeFilter
{
    private:

    protected:

    public:

        // Constructors/Destructor
        ISimTimeFilter();
        ISimTimeFilter( const ISimTimeFilter& other );
        virtual ~ISimTimeFilter();

        virtual ISimTimeFilter* Clone() const = 0;  // Virtual constructor (copying)

        // Setters

        // Getters

        // API Functions
        virtual void        reset() = 0;
        virtual void        push( simtime_t v ) = 0;
        virtual simtime_t   pop() = 0;

        // Operators
        ISimTimeFilter&     operator= (const ISimTimeFilter& other);
};

#endif
