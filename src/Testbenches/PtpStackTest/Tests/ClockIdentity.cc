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

#include "PtpStackTest.h"

#include "PTP_ClockIdentity.h"

#include <cassert>

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
// Internal functions
// ------------------------------------------------------
void
PtpStackTest::TestClockIdentity()
{
    EV << __func__ << endl;

    cClockIdentity  A( "C0-FF-EE-FE-EE-00-00-02" );
    cClockIdentity  B( "C0-FF-EE-FE-EE-00-00-05" );
    cClockIdentity  C( A );
    cClockIdentity  D;

    D = B;

    // Equal
    assert( (A == A) == true  );
    assert( (A == B) == false );
    assert( (A == C) == true  );
    assert( (A == D) == false );
    assert( (B == D) == true  );

    // Not equal
    assert( (A != A) == false );
    assert( (A != B) == true  );

    // Smaller
    assert( (A < B) == true  );
    assert( (A < A) == false );
    assert( (B < A) == false );

    // Greater
    assert( (B > A) == true  );
    assert( (B > B) == false );
    assert( (A > B) == false );
}
