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

#include "PTP_HeaderFlags.h"

#include <omnetpp.h>

// ======================================================
// Definitions
// ======================================================

const Octet PTP_FLAG_ALT_MASTER_MASK        = (1 << 0);
const Octet PTP_FLAG_TWO_STEP_MASK_MASK     = (1 << 1);
const Octet PTP_FLAG_UNICAST_MASK_MASK      = (1 << 2);
const Octet PTP_FLAG_PROF_SPEC_1_MASK       = (1 << 5);
const Octet PTP_FLAG_PROF_SPEC_2_MASK       = (1 << 6);
const Octet PTP_FLAG_RESERVED_MASK          = (1 << 7);

const Octet PTP_FLAG_LEAP_61_MASK           = (1 << 0);
const Octet PTP_FLAG_LEAP_59_MASK           = (1 << 1);
const Octet PTP_FLAG_CURR_UTC_VALID_MASK    = (1 << 2);
const Octet PTP_FLAG_PTP_TIMESCALE_MASK     = (1 << 3);
const Octet PTP_FLAG_TIME_TRACABLE_MASK     = (1 << 4);
const Octet PTP_FLAG_FREQ_TRACABLE_MASK     = (1 << 5);

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
cPtpHeaderFlags::cPtpHeaderFlags()
{
    alternateMasterFlag     = false;
    twoStepFlag             = false;
    unicastFlag             = false;
    ptpProfileSpecific_1    = false;
    ptpProfileSpecific_2    = false;
    reserved                = false;
    leap61                  = false;
    leap59                  = false;
    currentUtcOffsetValid   = false;
    ptpTimescale            = false;
    timeTraceable           = false;
    frequencyTraceable      = false;
}

cPtpHeaderFlags::cPtpHeaderFlags( const Octet flagField[] )
{
    ReadFromBuffer( flagField );
}

cPtpHeaderFlags::cPtpHeaderFlags( const cPtpHeaderFlags& other )
{
    this->alternateMasterFlag       = other.alternateMasterFlag;
    this->twoStepFlag               = other.twoStepFlag;
    this->unicastFlag               = other.unicastFlag;
    this->ptpProfileSpecific_1      = other.ptpProfileSpecific_1;
    this->ptpProfileSpecific_2      = other.ptpProfileSpecific_2;
    this->reserved                  = other.reserved;
    this->leap61                    = other.leap61;
    this->leap59                    = other.leap59;
    this->currentUtcOffsetValid     = other.currentUtcOffsetValid;
    this->ptpTimescale              = other.ptpTimescale;
    this->timeTraceable             = other.timeTraceable;
    this->frequencyTraceable        = other.frequencyTraceable;
}

cPtpHeaderFlags::~cPtpHeaderFlags()
{
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------

void
cPtpHeaderFlags::ReadFromBuffer( const Octet flagField[] )
{
    alternateMasterFlag    = (flagField[0] & PTP_FLAG_ALT_MASTER_MASK     ) == PTP_FLAG_ALT_MASTER_MASK;
    twoStepFlag            = (flagField[0] & PTP_FLAG_TWO_STEP_MASK_MASK  ) == PTP_FLAG_TWO_STEP_MASK_MASK;
    unicastFlag            = (flagField[0] & PTP_FLAG_UNICAST_MASK_MASK   ) == PTP_FLAG_UNICAST_MASK_MASK;
    ptpProfileSpecific_1 = (flagField[0] & PTP_FLAG_PROF_SPEC_1_MASK    ) == PTP_FLAG_PROF_SPEC_1_MASK;
    ptpProfileSpecific_2 = (flagField[0] & PTP_FLAG_PROF_SPEC_2_MASK    ) == PTP_FLAG_PROF_SPEC_2_MASK;
    reserved               = (flagField[0] & PTP_FLAG_RESERVED_MASK       ) == PTP_FLAG_RESERVED_MASK;

    leap61                 = (flagField[1] & PTP_FLAG_LEAP_61_MASK        ) == PTP_FLAG_LEAP_61_MASK;
    leap59                 = (flagField[1] & PTP_FLAG_LEAP_59_MASK        ) == PTP_FLAG_LEAP_59_MASK;
    currentUtcOffsetValid  = (flagField[1] & PTP_FLAG_CURR_UTC_VALID_MASK ) == PTP_FLAG_CURR_UTC_VALID_MASK;
    ptpTimescale           = (flagField[1] & PTP_FLAG_PTP_TIMESCALE_MASK  ) == PTP_FLAG_PTP_TIMESCALE_MASK;
    timeTraceable          = (flagField[1] & PTP_FLAG_TIME_TRACABLE_MASK  ) == PTP_FLAG_TIME_TRACABLE_MASK;
    frequencyTraceable     = (flagField[1] & PTP_FLAG_FREQ_TRACABLE_MASK  ) == PTP_FLAG_FREQ_TRACABLE_MASK;
}

// ------------------------------------------------------
// Getters
// ------------------------------------------------------
void
cPtpHeaderFlags::SaveToBuffer( Octet flagField[] )
{
    flagField[ 0 ]  = ( alternateMasterFlag    ? PTP_FLAG_ALT_MASTER_MASK     : 0x00 ) |
                      ( twoStepFlag            ? PTP_FLAG_TWO_STEP_MASK_MASK  : 0x00 ) |
                      ( unicastFlag            ? PTP_FLAG_UNICAST_MASK_MASK   : 0x00 ) |
                      ( ptpProfileSpecific_1   ? PTP_FLAG_PROF_SPEC_1_MASK    : 0x00 ) |
                      ( ptpProfileSpecific_2   ? PTP_FLAG_PROF_SPEC_2_MASK    : 0x00 ) |
                      ( reserved               ? PTP_FLAG_RESERVED_MASK       : 0x00 );

    flagField[ 1 ]  = ( leap61                 ? PTP_FLAG_LEAP_61_MASK        : 0x00 ) |
                      ( leap59                 ? PTP_FLAG_LEAP_59_MASK        : 0x00 ) |
                      ( currentUtcOffsetValid  ? PTP_FLAG_CURR_UTC_VALID_MASK : 0x00 ) |
                      ( ptpTimescale           ? PTP_FLAG_PTP_TIMESCALE_MASK  : 0x00 ) |
                      ( timeTraceable          ? PTP_FLAG_TIME_TRACABLE_MASK  : 0x00 ) |
                      ( frequencyTraceable     ? PTP_FLAG_FREQ_TRACABLE_MASK  : 0x00 );
}

// ------------------------------------------------------
// Compare operators
// ------------------------------------------------------
bool
cPtpHeaderFlags::operator== (const cPtpHeaderFlags& other) const
{
    if
    (
        ( other.alternateMasterFlag     == this->alternateMasterFlag    ) &&
        ( other.twoStepFlag             == this->twoStepFlag            ) &&
        ( other.unicastFlag             == this->unicastFlag            ) &&
        ( other.ptpProfileSpecific_1    == this->ptpProfileSpecific_1   ) &&
        ( other.ptpProfileSpecific_2    == this->ptpProfileSpecific_2   ) &&
        ( other.reserved                == this->reserved               ) &&
        ( other.leap61                  == this->leap61                 ) &&
        ( other.leap59                  == this->leap59                 ) &&
        ( other.currentUtcOffsetValid   == this->currentUtcOffsetValid  ) &&
        ( other.ptpTimescale            == this->ptpTimescale           ) &&
        ( other.timeTraceable           == this->timeTraceable          ) &&
        ( other.frequencyTraceable      == this->frequencyTraceable     )
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
cPtpHeaderFlags::operator!= (const cPtpHeaderFlags& other) const
{
    return !(this->operator ==(other));
}

// ------------------------------------------------------
// Assign operator
// ------------------------------------------------------

cPtpHeaderFlags&
cPtpHeaderFlags::operator=(const cPtpHeaderFlags& other)
{
    this->alternateMasterFlag       = other.alternateMasterFlag;
    this->twoStepFlag               = other.twoStepFlag;
    this->unicastFlag               = other.unicastFlag;
    this->ptpProfileSpecific_1      = other.ptpProfileSpecific_1;
    this->ptpProfileSpecific_2      = other.ptpProfileSpecific_2;
    this->reserved                  = other.reserved;
    this->leap61                    = other.leap61;
    this->leap59                    = other.leap59;
    this->currentUtcOffsetValid     = other.currentUtcOffsetValid;
    this->ptpTimescale              = other.ptpTimescale;
    this->timeTraceable             = other.timeTraceable;
    this->frequencyTraceable        = other.frequencyTraceable;

    // By convention, always return *this
    return *this;
}

// ------------------------------------------------------
// Stream operators
// ------------------------------------------------------
std::ostream&
operator<<(std::ostream& os, const cPtpHeaderFlags& o)
{
    os << o.GetString(); // no endl!

    return os;
}

// ------------------------------------------------------
// Print
// ------------------------------------------------------
void
cPtpHeaderFlags::Print() const
{
    EV << "Header flags:" << endl;
    EV << endl;
    EV << "[" << (alternateMasterFlag    ? "x" : " ") << "]" << " Alternate master"           << endl;
    EV << "[" << (twoStepFlag            ? "x" : " ") << "]" << " Two step"                   << endl;
    EV << "[" << (unicastFlag            ? "x" : " ") << "]" << " Unicast"                    << endl;
    EV << "[" << (ptpProfileSpecific_1   ? "x" : " ") << "]" << " Profile specific 1"         << endl;
    EV << "[" << (ptpProfileSpecific_2   ? "x" : " ") << "]" << " Profile specific 2"         << endl;
    EV << "[" << (reserved               ? "x" : " ") << "]" << " Reserved"                   << endl;
    EV << "[" << (leap61                 ? "x" : " ") << "]" << " Leap61"                     << endl;
    EV << "[" << (leap59                 ? "x" : " ") << "]" << " Leap59"                     << endl;
    EV << "[" << (currentUtcOffsetValid  ? "x" : " ") << "]" << " Current UTC offset valid"   << endl;
    EV << "[" << (ptpTimescale           ? "x" : " ") << "]" << " PTP time scale"             << endl;
    EV << "[" << (timeTraceable          ? "x" : " ") << "]" << " Time traceable"             << endl;
    EV << "[" << (frequencyTraceable     ? "x" : " ") << "]" << " Frequency traceable"        << endl;
}

std::string
cPtpHeaderFlags::GetString() const
{
    std::stringstream   ss;

    ss << (alternateMasterFlag    ? " AM"  : " - ");
    ss << (twoStepFlag            ? " 2s"  : " - ");
    ss << (unicastFlag            ? "uni"  : " - ");
    ss << (ptpProfileSpecific_1   ? "ps1"  : " - ");
    ss << (ptpProfileSpecific_2   ? "ps2"  : " - ");
    ss << (reserved               ? "  r"  : " - ");
    ss << (leap61                 ? "l61"  : " - ");
    ss << (leap59                 ? "l51"  : " - ");
    ss << (currentUtcOffsetValid  ? "UTC"  : " - ");
    ss << (ptpTimescale           ? "PTP"  : " - ");
    ss << (timeTraceable          ? " TT"  : " - ");
    ss << (frequencyTraceable     ? " FT"  : " - ");

    return ss.str();
}
