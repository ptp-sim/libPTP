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

#include "PTP_ForeignClockDS.h"

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

static void CompareClockDS( cForeignClockDS &A, cForeignClockDS &B, ClockCompResult Result1, ClockCompResult Result2, ClockCompReason Reason )
{
    ClockCompReturn_t   Res;

    Res = cForeignClockDS::CompareClockDS( A, B, false );
    assert( Res.Result == Result1 );
    assert( Res.Reason == Reason );

    Res = cForeignClockDS::CompareClockDS( B, A, false );
    assert( Res.Result == Result2 );
    assert( Res.Reason == Reason );
}

// ------------------------------------------------------
// Internal functions
// ------------------------------------------------------
void
PtpStackTest::TestForeignClockDS()
{
    EV << __func__ << endl;

    cForeignClockDS     A;
    cForeignClockDS     B;
    cForeignClockDS     A2;


    // Initialize A
    A.ClockIdentity() = "AA-AA-AA-FF-FE-00-00-01";
    A.ClockQuality().SetClockAccuracy( CLOCK_ACCURACY_1_S );
    A.ClockQuality().SetClockClass( CLOCK_CLASS_PRIMARY );
    A.ClockQuality().SetOffsetScaledLogVar( 100 );

    A.SenderPortIdentity() = cPortIdentity( "BB-BB-BB-FF-FE-00-00-01", 5 );
    A.ReceiverPortIdentity()= cPortIdentity( "CC-CC-CC-FF-FE-00-00-01", 4 );
    A.FlagsField() = cPtpHeaderFlags();

    A.SetPriority1( 100 );
    A.SetPriority2( 100 );
    A.SetStepsRemoved( 5 );

    A.Activate();

    A2  = A;

    A2.ClockIdentity() = "DD-DD-DD-FF-FE-00-00-01";

    // ------------------------------------------------------------------
    // Equal
    // ------------------------------------------------------------------
    B = A;
    CompareClockDS( A, B, ClockCompResult::ERROR_2, ClockCompResult::ERROR_2, ClockCompReason::NONE );

    // ------------------------------------------------------------------
    // One side empty
    // ------------------------------------------------------------------
    B.Clear();
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B, ClockCompResult::B_BETTER_A, ClockCompReason::OTHER_EMPTY );

    // ------------------------------------------------------------------
    // Priority 1
    // ------------------------------------------------------------------
    B = A2;
    B.SetPriority1(200);
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B, ClockCompResult::B_BETTER_A, ClockCompReason::PRIORITY1 );

    // ------------------------------------------------------------------
    // GM Class
    // ------------------------------------------------------------------
    B = A2;
    B.ClockQuality().SetClockClass(CLOCK_CLASS_SLAVE_ONLY);
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B, ClockCompResult::B_BETTER_A, ClockCompReason::CLOCK_CLASS );

    // ------------------------------------------------------------------
    // GM Accuracy
    // ------------------------------------------------------------------
    B = A2;
    B.ClockQuality().SetClockAccuracy(CLOCK_ACCURACY_OVER_10_S);
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B, ClockCompResult::B_BETTER_A, ClockCompReason::CLOCK_ACCURACY );

    // ------------------------------------------------------------------
    // GM offsetScaledLogVariance
    // ------------------------------------------------------------------
    B = A2;
    B.ClockQuality().SetOffsetScaledLogVar(200);
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B, ClockCompResult::B_BETTER_A, ClockCompReason::OFFSET_LOG_VAR );

    // ------------------------------------------------------------------
    // Priority 2
    // ------------------------------------------------------------------
    B = A2;
    B.SetPriority2(200);
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B, ClockCompResult::B_BETTER_A, ClockCompReason::PRIORITY2 );

    // ------------------------------------------------------------------
    // GM Identity
    // ------------------------------------------------------------------
    B = A2;
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B, ClockCompResult::B_BETTER_A, ClockCompReason::CLOCK_ID );

    // ------------------------------------------------------------------
    // StepsRemoved (not within 1 step)
    // ------------------------------------------------------------------
    B = A;
    B.SetStepsRemoved( A.GetStepsRemoved() + 2 );
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B, ClockCompResult::B_BETTER_A, ClockCompReason::STEPS_REMOVED );

    // ------------------------------------------------------------------
    // StepsRemoved 2a (within 1 step, receiver smaller)
    // ------------------------------------------------------------------
    B = A;
    B.SetStepsRemoved( A.GetStepsRemoved() + 1 );
    B.SenderPortIdentity()      = cPortIdentity( "BB-BB-BB-FF-FE-00-00-01", 5 );
    B.ReceiverPortIdentity()    = cPortIdentity( "BB-BB-BB-FF-FE-00-00-01", 4 );
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B, ClockCompResult::B_BETTER_A, ClockCompReason::RCV_PORT_ID_SMALLER );

    // ------------------------------------------------------------------
    // StepsRemoved 2b (within 1 step, receiver greater)
    // ------------------------------------------------------------------
    B = A;
    B.SetStepsRemoved( A.GetStepsRemoved() + 1 );
    B.SenderPortIdentity()      = cPortIdentity( "BB-BB-BB-FF-FE-00-00-01", 5 );
    B.ReceiverPortIdentity()    = cPortIdentity( "BB-BB-BB-FF-FE-00-00-01", 6 );
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B_BY_TOPO, ClockCompResult::B_BETTER_A_BY_TOPO, ClockCompReason::RCV_PORT_ID_GREATER );

    // ------------------------------------------------------------------
    // StepsRemoved 2c (within 1 step, receiver equals sender)
    // ------------------------------------------------------------------
    B = A;
    B.SetStepsRemoved( A.GetStepsRemoved() + 1 );
    B.SenderPortIdentity() = B.ReceiverPortIdentity();
    CompareClockDS( A, B, ClockCompResult::ERROR_1, ClockCompResult::ERROR_1, ClockCompReason::NONE );

    // ------------------------------------------------------------------
    // Sender identity
    // ------------------------------------------------------------------
    B = A;
    B.SenderPortIdentity().SetPortNumber( A.SenderPortIdentity().GetPortNumber() + 1 );
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B_BY_TOPO, ClockCompResult::B_BETTER_A_BY_TOPO, ClockCompReason::SND_PORT_ID );

    // ------------------------------------------------------------------
    // Port number
    // ------------------------------------------------------------------
    B = A;
    B.ReceiverPortIdentity().SetPortNumber( A.ReceiverPortIdentity().GetPortNumber() + 1 );
    CompareClockDS( A, B, ClockCompResult::A_BETTER_B_BY_TOPO, ClockCompResult::B_BETTER_A_BY_TOPO, ClockCompReason::RCV_PORT_NUMBER );

}
