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

#include "PTP_ForeignClockDS.h"

#include <limits>
#include <cstdint>

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
cForeignClockDS::cForeignClockDS()
{
    this->Empty     = true;

    this->ModuleID  = -1;

    this->Priority1         = 0;
    this->Priority2         = 0;
    this->StepsRemoved      = 0;

    this->flagField         = cPtpHeaderFlags();
    this->currentUtcOffset  = 0;
    this->timeSource        = TIME_SRC_OTHER;
}

cForeignClockDS::cForeignClockDS(const cDefaultDS& defaultDS)
{
    this->operator=( defaultDS );
}

cForeignClockDS::cForeignClockDS(const PTPv2_AnnounceFrame *pAnn)
{
    this->operator=( pAnn );
}

cForeignClockDS::cForeignClockDS(const cForeignClockDS& other)
{
    this->Empty             = other.Empty;

    this->Priority1         = other.Priority1;
    this->ClockId           = other.ClockId;
    this->ClockQual         = other.ClockQual;
    this->Priority2         = other.Priority2;
    this->StepsRemoved      = other.StepsRemoved;
    this->SenderPortId      = other.SenderPortId;
    this->ReceiverPortId    = other.ReceiverPortId;

    this->ModuleID          = other.ModuleID;

    this->flagField         = other.flagField;
    this->currentUtcOffset  = other.currentUtcOffset;
    this->timeSource        = other.timeSource;
}

cForeignClockDS::~cForeignClockDS()
{
}

// ------------------------------------------------------
// Instance methods
// ------------------------------------------------------
cClockIdentity&
cForeignClockDS::ClockIdentity()
{
    return  this->ClockId;
}

cClockQuality&
cForeignClockDS::ClockQuality()
{
    return  this->ClockQual;
}

cPortIdentity&
cForeignClockDS::SenderPortIdentity()
{
    return  this->SenderPortId;
}

cPortIdentity&
cForeignClockDS::ReceiverPortIdentity()
{
    return  this->ReceiverPortId;
}

cPtpHeaderFlags&
cForeignClockDS::FlagsField()
{
    return  this->flagField;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
void
cForeignClockDS::SetPriority1( UInteger8 Priority1 )
{
    this->Priority1 = Priority1;
}

void
cForeignClockDS::SetPriority2( UInteger8 Priority2 )
{
    this->Priority2 = Priority2;
}

void
cForeignClockDS::SetStepsRemoved( UInteger16 StepsRemoved )
{
    this->StepsRemoved  = StepsRemoved;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
UInteger8
cForeignClockDS::GetPriority1() const
{
    if( Empty )
    {
        return std::numeric_limits<std::uint8_t>::max();
    }

    return  this->Priority1;
}

UInteger8
cForeignClockDS::GetPriority2() const
{
    if( Empty )
    {
        return std::numeric_limits<std::uint8_t>::max();
    }

    return  this->Priority2;
}

UInteger16
cForeignClockDS::GetStepsRemoved() const
{
    if( Empty )
    {
        return std::numeric_limits<std::uint16_t>::max();
    }

    return  this->StepsRemoved;
}

timeSource_t
cForeignClockDS::GetTimeSource() const
{
    if( Empty )
    {
        return TIME_SRC_OTHER;
    }

    return  this->timeSource;
}

UInteger16
cForeignClockDS::GetCurrentUtcOffset() const
{
    if( Empty )
    {
        return std::numeric_limits<std::uint16_t>::max();
    }

    return  this->currentUtcOffset;
}

int
cForeignClockDS::GetModuleID() const
{
    if( Empty )
    {
        return -1;
    }

    return this->ModuleID;
}

// ------------------------------------------------------
// API functions
// ------------------------------------------------------
void
cForeignClockDS::Clear()
{
    this->Empty = true;
}

void
cForeignClockDS::Activate()
{
    this->Empty = false;
}

bool
cForeignClockDS::IsEmpty() const
{
    return  this->Empty;
}

ClockCompReturn_t
cForeignClockDS::CompareClockDS( const cForeignClockDS& A, const cForeignClockDS& B, bool EnableDebugOutput )
{
    ClockCompReturn_t   ret;

    if( EnableDebugOutput )
    {
        EV << "----------------------------------------------------------------" << endl;
        EV << "Clock DataSet comparison" << endl;
        EV << "----------------------------------------------------------------" << endl;
        EV << "A :" << endl;
        A.Print();
        EV << "----------------------------------------------------------------" << endl;
        EV << "B :" << endl;
        B.Print();
        EV << "----------------------------------------------------------------" << endl;
    }

    // Check for empty and equal sets
    if( B.IsEmpty() )
    {
        ret.Result  = ClockCompResult::A_BETTER_B;
        ret.Reason  = ClockCompReason::OTHER_EMPTY;
        return ret;
    }

    if( A.IsEmpty() )
    {
        ret.Result  = ClockCompResult::B_BETTER_A;
        ret.Reason  = ClockCompReason::OTHER_EMPTY;
        return ret;
    }

    // This function implements Figure 27 and Figure 28 of IEEE 1588-2008

    // GM Identity of A == GM Identity of B
    if( A.ClockId == B.ClockId )
    {
        // Figure 28

        // Compare StepsRemoved of A and B
        if( A.StepsRemoved > B.StepsRemoved + 1 )
        {
            ret.Result  = ClockCompResult::B_BETTER_A;
            ret.Reason  = ClockCompReason::STEPS_REMOVED;
            return ret;
        }

        if( A.StepsRemoved + 1 < B.StepsRemoved )
        {
            ret.Result  = ClockCompResult::A_BETTER_B;
            ret.Reason  = ClockCompReason::STEPS_REMOVED;
            return ret;
        }

        if( A.StepsRemoved > B.StepsRemoved )
        {
            // Compare Identities of Receiver of A and Sender of A
            if( A.ReceiverPortId < A.SenderPortId )
            {
                ret.Result  = ClockCompResult::B_BETTER_A;
                ret.Reason  = ClockCompReason::RCV_PORT_ID_SMALLER;
                return ret;
            }
            else if( A.ReceiverPortId > A.SenderPortId )
            {
                ret.Result  = ClockCompResult::B_BETTER_A_BY_TOPO;
                ret.Reason  = ClockCompReason::RCV_PORT_ID_GREATER;
                return ret;
            }

            ret.Result  = ClockCompResult::ERROR_1;
            ret.Reason  = ClockCompReason::NONE;
            return ret;
        }

        if( A.StepsRemoved < B.StepsRemoved )
        {
            // Compare Identities of Receiver of B and Sender of B
            if( B.ReceiverPortId < B.SenderPortId )
            {
                ret.Result  = ClockCompResult::A_BETTER_B;
                ret.Reason  = ClockCompReason::RCV_PORT_ID_SMALLER;
                return ret;
            }
            else if( B.ReceiverPortId > B.SenderPortId )
            {
                ret.Result  = ClockCompResult::A_BETTER_B_BY_TOPO;
                ret.Reason  = ClockCompReason::RCV_PORT_ID_GREATER;
                return ret;
            }

            ret.Result  = ClockCompResult::ERROR_1;
            ret.Reason  = ClockCompReason::NONE;
            return ret;
        }

        // Compare identities of senders of A and B
        if( A.SenderPortId > B.SenderPortId )
        {
            ret.Result  = ClockCompResult::B_BETTER_A_BY_TOPO;
            ret.Reason  = ClockCompReason::SND_PORT_ID;
            return ret;
        }
        else if( A.SenderPortId < B.SenderPortId )
        {
            ret.Result  = ClockCompResult::A_BETTER_B_BY_TOPO;
            ret.Reason  = ClockCompReason::SND_PORT_ID;
            return ret;
        }

        // Compare Port Numbers of receivers of A and B
        if( A.ReceiverPortId.GetPortNumber() > B.ReceiverPortId.GetPortNumber() )
        {
            ret.Result  = ClockCompResult::B_BETTER_A_BY_TOPO;
            ret.Reason  = ClockCompReason::RCV_PORT_NUMBER;
            return ret;
        }
        else if( A.ReceiverPortId.GetPortNumber() < B.ReceiverPortId.GetPortNumber() )
        {
            ret.Result  = ClockCompResult::A_BETTER_B_BY_TOPO;
            ret.Reason  = ClockCompReason::RCV_PORT_NUMBER;
            return ret;
        }

        ret.Result  = ClockCompResult::ERROR_2;
        ret.Reason  = ClockCompReason::NONE;
        return ret;
    }
    else
    {
        // Figure 27

        // Compare GM priority1 values of A and B
        if( A.Priority1 > B.Priority1 )
        {
            ret.Result  = ClockCompResult::B_BETTER_A;
            ret.Reason  = ClockCompReason::PRIORITY1;
            return ret;
        }
        else if( A.Priority1 < B.Priority1 )
        {
            ret.Result  = ClockCompResult::A_BETTER_B;
            ret.Reason  = ClockCompReason::PRIORITY1;
            return ret;
        }

        // Compare GM class values of A and B
        if( A.ClockQual.GetClockClass() > B.ClockQual.GetClockClass() )
        {
            ret.Result  = ClockCompResult::B_BETTER_A;
            ret.Reason  = ClockCompReason::CLOCK_CLASS;
            return ret;
        }
        else if( A.ClockQual.GetClockClass() < B.ClockQual.GetClockClass() )
        {
            ret.Result  = ClockCompResult::A_BETTER_B;
            ret.Reason  = ClockCompReason::CLOCK_CLASS;
            return ret;
        }

        // Compare GM accuracy values of A and B
        if( A.ClockQual.GetClockAccuracy() > B.ClockQual.GetClockAccuracy() )
        {
            ret.Result  = ClockCompResult::B_BETTER_A;
            ret.Reason  = ClockCompReason::CLOCK_ACCURACY;
            return ret;
        }
        else if( A.ClockQual.GetClockAccuracy() < B.ClockQual.GetClockAccuracy() )
        {
            ret.Result  = ClockCompResult::A_BETTER_B;
            ret.Reason  = ClockCompReason::CLOCK_ACCURACY;
            return ret;
        }

        // Compare GM offsetScaledLogVariance of A and B
        if( A.ClockQual.GetOffsetScaledLogVar() > B.ClockQual.GetOffsetScaledLogVar() )
        {
            ret.Result  = ClockCompResult::B_BETTER_A;
            ret.Reason  = ClockCompReason::OFFSET_LOG_VAR;
            return ret;
        }
        else if( A.ClockQual.GetOffsetScaledLogVar() < B.ClockQual.GetOffsetScaledLogVar() )
        {
            ret.Result  = ClockCompResult::A_BETTER_B;
            ret.Reason  = ClockCompReason::OFFSET_LOG_VAR;
            return ret;
        }

        // Compare GM priority2 values of A and B
        if( A.Priority2 > B.Priority2 )
        {
            ret.Result  = ClockCompResult::B_BETTER_A;
            ret.Reason  = ClockCompReason::PRIORITY2;
            return ret;
        }
        else if( A.Priority2 < B.Priority2 )
        {
            ret.Result  = ClockCompResult::A_BETTER_B;
            ret.Reason  = ClockCompReason::PRIORITY2;
            return ret;
        }

        // Compare GM identity valeus of A and B
        if( A.ClockId > B.ClockId )
        {
            ret.Result  = ClockCompResult::B_BETTER_A;
            ret.Reason  = ClockCompReason::CLOCK_ID;
            return ret;
        }
        else if( A.ClockId < B.ClockId )
        {
            ret.Result  = ClockCompResult::A_BETTER_B;
            ret.Reason  = ClockCompReason::CLOCK_ID;
            return ret;
        }
    }

    throw cRuntimeError("Data Set Comparison algorithm: Invalid comparison" );

    ret.Result  = ClockCompResult::ERROR_INVALID;
    ret.Reason  = ClockCompReason::NONE;
    return ret;
}

// ------------------------------------------------------
// Compare Operators
// ------------------------------------------------------
bool
cForeignClockDS::operator== (const cForeignClockDS& other) const
{
    if( this->Empty || other.Empty )
    {
        return false;
    }

    if
    (
        ( this->Priority1        == other.Priority1        ) &&
        ( this->ClockId          == other.ClockId          ) &&
        ( this->ClockQual        == other.ClockQual        ) &&
        ( this->Priority2        == other.Priority2        ) &&
        ( this->StepsRemoved     == other.StepsRemoved     ) &&
        ( this->SenderPortId     == other.SenderPortId     ) &&
        ( this->ReceiverPortId   == other.ReceiverPortId   ) &&
        ( this->ModuleID         == other.ModuleID         ) &&
        ( this->flagField        == other.flagField        ) &&
        ( this->currentUtcOffset == other.currentUtcOffset ) &&
        ( this->timeSource       == other.timeSource       )
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
cForeignClockDS::operator!= (const cForeignClockDS& other) const
{
    return !( this->operator ==(other) );
}

// ------------------------------------------------------
// Assignment Operators
// ------------------------------------------------------
cForeignClockDS&
cForeignClockDS::operator= (const cForeignClockDS& other)
{
    this->Empty             = other.Empty;

    this->Priority1         = other.Priority1;
    this->ClockId           = other.ClockId;
    this->ClockQual         = other.ClockQual;
    this->Priority2         = other.Priority2;
    this->StepsRemoved      = other.StepsRemoved;
    this->SenderPortId      = other.SenderPortId;
    this->ReceiverPortId    = other.ReceiverPortId;

    this->ModuleID          = other.ModuleID;

    this->flagField         = other.flagField;
    this->currentUtcOffset  = other.currentUtcOffset;
    this->timeSource        = other.timeSource;

    // By convention, always return *this
    return *this;
}

cForeignClockDS&
cForeignClockDS::operator= (const PTPv2_AnnounceFrame *pAnn)
{
    // Information source is used as specified in Table 12 of IEEE 1588-2008

    this->Priority1         = pAnn->getGrandmasterPriority1();
    this->ClockId           = pAnn->getGrandmasterIdentity();
    this->ClockQual         = pAnn->getGrandmasterClockQuality();
    this->Priority2         = pAnn->getGrandmasterPriority2();
    this->StepsRemoved      = pAnn->getStepsRemoved();
    this->SenderPortId      = pAnn->getSourcePortIdentity();
    this->ReceiverPortId    = cPortIdentity();

    this->ModuleID          = pAnn->getGrandMasterModuleID();

    // Save Announce message specific stuff
    this->flagField         = pAnn->getFlagField();
    this->currentUtcOffset  = pAnn->getCurrentUtcOffset();
    this->timeSource        = (timeSource_t) pAnn->getTimeSource();

    this->Empty = false;

    // By convention, always return *this
    return *this;
}

cForeignClockDS&
cForeignClockDS::operator= (const cDefaultDS& defaultDS)
{
    // Information source is used as specified in Table 12 of IEEE 1588-2008

    this->Priority1         = defaultDS.GetPriority1();
    this->ClockId           = defaultDS.GetClockIdentity();
    this->ClockQual         = defaultDS.GetClockQuality();
    this->Priority2         = defaultDS.GetPriority2();
    this->StepsRemoved      = 0;
    this->SenderPortId      = defaultDS.GetClockIdentity();
    this->ReceiverPortId    = defaultDS.GetClockIdentity();

    this->ModuleID          = defaultDS.GetModuleID();

    this->flagField         = cPtpHeaderFlags();
    this->currentUtcOffset  = 0;
    this->timeSource        = TIME_SRC_OTHER;

    this->Empty = false;

    // By convention, always return *this
    return *this;
}


// ------------------------------------------------------
// Debug functions
// ------------------------------------------------------
void
cForeignClockDS::Print() const
{
    if( Empty )
    {
        EV << "   Empty" << endl;
        return;
    }

    EV << "Priority1:        " << static_cast<int>(Priority1) << endl;
    EV << "ClockId:          " << ClockId                     << endl;
    EV << "ClockQual:        " << ClockQual                   << endl;
    EV << "Priority2:        " << static_cast<int>(Priority2) << endl;
    EV << "StepsRemoved:     " << StepsRemoved                << endl;
    EV << "SenderPortId:     " << SenderPortId                << endl;
    EV << "ReceiverPortId:   " << ReceiverPortId              << endl;
    EV << "ModuleID:         " << ModuleID                    << endl;
    EV << "flagField:        " << flagField                   << endl;
    EV << "currentUtcOffset: " << currentUtcOffset            << endl;
    EV << "timeSource:       " << timeSource                  << endl;

}

// ------------------------------------------------------
// Stream operators
// ------------------------------------------------------
std::ostream&
operator<<(std::ostream& os, const ClockCompResult& o)
{
    switch(o)
    {
        case ClockCompResult::A_BETTER_B:           os << "A better B";             break;
        case ClockCompResult::A_BETTER_B_BY_TOPO:   os << "A better B by topology"; break;
        case ClockCompResult::B_BETTER_A:           os << "B better A";             break;
        case ClockCompResult::B_BETTER_A_BY_TOPO:   os << "B better A by topology"; break;
        case ClockCompResult::ERROR_1:              os << "Error 1";                break;
        case ClockCompResult::ERROR_2:              os << "Error 2";                break;
        case ClockCompResult::ERROR_INVALID:        os << "Error (invalid)";        break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const ClockCompReason& o)
{
    switch(o)
    {
        case ClockCompReason::NONE:                 os << "No reason";                  break;
        case ClockCompReason::OTHER_EMPTY:          os << "The other one is empty";     break;
        case ClockCompReason::STEPS_REMOVED:        os << "Steps removed";              break;
        case ClockCompReason::RCV_PORT_ID_SMALLER:  os << "Receiver PortID 1";          break;
        case ClockCompReason::RCV_PORT_ID_GREATER:  os << "Receiver PortID 2";          break;
        case ClockCompReason::SND_PORT_ID:          os << "Sender Port ID";             break;
        case ClockCompReason::RCV_PORT_NUMBER:      os << "Receiver Port number";       break;
        case ClockCompReason::PRIORITY1:            os << "Priority 1";                 break;
        case ClockCompReason::CLOCK_CLASS:          os << "Clock class";                break;
        case ClockCompReason::CLOCK_ACCURACY:       os << "Clock accuracy";             break;
        case ClockCompReason::OFFSET_LOG_VAR:       os << "Offset log variance";        break;
        case ClockCompReason::PRIORITY2:            os << "Priority 2";                 break;
        case ClockCompReason::CLOCK_ID:             os << "ClockID";                    break;
    }

    return os;
}
