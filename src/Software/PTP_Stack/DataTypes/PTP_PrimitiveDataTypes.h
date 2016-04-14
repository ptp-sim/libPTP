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

#ifndef LIBPTP_PTP_PRIMITIVE_DATATYPES_H_
#define LIBPTP_PTP_PRIMITIVE_DATATYPES_H_

// ======================================================
// Includes
// ======================================================

#include <stdint.h>
#include <sstream>

// ======================================================
// Types
// ======================================================

// ------------------------------------------------------
// Primitive data types
// ------------------------------------------------------
typedef bool            Boolean;
typedef uint8_t         Enumeration4;
typedef uint8_t         Enumeration8;
typedef uint16_t        Enumeration16;
typedef uint8_t         UInteger4;
typedef int8_t          Integer8;
typedef uint8_t         UInteger8;
typedef int16_t         Integer16;
typedef uint16_t        UInteger16;
typedef int32_t         Integer32;
typedef uint32_t        UInteger32;
typedef uint64_t        UInteger48;
typedef int64_t         Integer64;
typedef uint8_t         Nibble;
typedef uint8_t         Octet;

// ------------------------------------------------------
// Enumerations
// ------------------------------------------------------

enum tPtpMessageType
{
    // Event
    PTP_TYPE_SYNC            = 0x00,
    PTP_TYPE_DELAY_REQ       = 0x01,
    PTP_TYPE_PDELAY_REQ      = 0x02,
    PTP_TYPE_PDELAY_RESP     = 0x03,

    // General
    PTP_TYPE_FOLLOW_UP       = 0x08,
    PTP_TYPE_DELAY_RESP      = 0x09,
    PTP_TYPE_PDELAY_RESP_FU  = 0x0A,
    PTP_TYPE_ANNOUNCE        = 0x0B,
    PTP_TYPE_SIGNALING       = 0x0C,
    PTP_TYPE_MANAGEMENT      = 0x0D,

    PTP_TYPE_INVALID         = 0xFF,
};

// PTP Profile type
typedef enum
{
    PTP_PROFILE_CUSTOM,
    PTP_PROFILE_DEFAULT_E2E,
    PTP_PROFILE_DEFAULT_P2P,
    PTP_PROFILE_POWER,
}
PTP_Profile_t;

// Delay mechanism, see table 9 in IEEE 1588-2008
typedef enum
{
    PTP_CLOCK_TYPE_ORDINARY,                // includes boundary clocks
    PTP_CLOCK_TYPE_TRANSPARENT,             //
}
PTP_ClockType_t;

// Best master clock algorithm
typedef enum
{
    BMCA_1588_2008_DEFAULT,                 // Default BMC algorithm as specified in IEEE1588-2008
}
PTP_BMCA_t;

// Management protocol
typedef enum
{
    MGMT_1588_2008_DEFAULT,                 // Default management protocol as specified in IEEE1588-2008
    MGMT_SNMP,                              // Simple network management protocol
}
PTP_MgmtProtocol_t;

// Network protocol, see clause 7.4.1 and table 3 in IEEE 1588-2008
typedef enum
{
                                            // 0x0000           Reserved
    PROTOCOL_UDP_IPv4       = 0x0001,       //                  Annex D
    PROTOCOL_UDP_IPv6       = 0x0002,       //                  Annex E
    PROTOCOL_IEEE_802_3     = 0x0003,       //                  Annex F
    PROTOCOL_DEVICE_NET     = 0x0004,       //                  Annex G
    PROTOCOL_CONTROL_NET    = 0x0005,       //                  Annex H
    PROTOCOL_PROFINET       = 0x0006,       //                  Annex I
                                            // 0x0007 - 0xEFFF  Reserved for assignment by the Precise Networked Clock Working Group
                                            //                  of the IM/ST Committee
                                            // 0xF000 - 0xFFFD  Reserved for assignment in a PTP profile
    PROTOCOL_UNKNOWN        = 0xFFFE,       //
                                            // 0xFFFF           Reserved
}
NetworkProtocol_t;

// Clock class, see clause 7.6.2.4 and table 5 in IEEE 1588-2008
typedef enum
{
                                                            // 0        Reserved to enable compatibility with future versions
                                                            // 1-5      Reserved
    CLOCK_CLASS_PRIMARY                             = 6,    // 6        Synchronized to primary reference time source
    CLOCK_CLASS_PRIMARY_HOLDOVER                    = 7,    // 7        Previously class 6, currently in holdover
                                                            // 8        Reserved
                                                            // 9-10     Reserved to enable compatibility with future versions
                                                            // 11-12    Reserved
    CLOCK_CLASS_APP_SPECIFIC                        = 13,   // 13       Synchronized to application specific reference time source
    CLOCK_CLASS_APP_SPECIFIC_HOLDOVER               = 14,   // 14       Previously class 13, currently in holdover
                                                            // 15-51    Reserved
    CLOCK_CLASS_PRIMARY_HOLDOVER_DEGRADE_A          = 52,   // 52       Degradation A for class 7
                                                            // 53-57    Reserved
    CLOCK_CLASS_APP_SPECIFIC_HOLDOVER_DEGRADE_A     = 58,   // 58       Degradation A for class 14
                                                            // 59-67    Reserved
    CLOCK_CLASS_ALTERNATE_PTP_1_1                   = 68,   // 68-122   For use by alternate PTP profiles
    CLOCK_CLASS_ALTERNATE_PTP_1_2                   = 69,   //
    CLOCK_CLASS_ALTERNATE_PTP_1_3                   = 70,   //
    CLOCK_CLASS_ALTERNATE_PTP_1_4                   = 71,   //
                                                            // 123-127  Reserved
                                                            // 128-132  Reserved
    CLOCK_CLASS_ALTERNATE_PTP_2_1                   = 133,  // 133-170   For use by alternate PTP profiles
    CLOCK_CLASS_ALTERNATE_PTP_2_2                   = 134,  //
    CLOCK_CLASS_ALTERNATE_PTP_2_3                   = 135,  //
    CLOCK_CLASS_ALTERNATE_PTP_2_4                   = 136,  //
                                                            // 171-186  Reserved
    CLOCK_CLASS_PRIMARY_HOLDOVER_DEGRADE_B          = 187,  // 187      Degradation B for class 7
                                                            // 188-192  Reserved
    CLOCK_CLASS_APP_SPECIFIC_HOLDOVER_DEGRADE_B     = 193,  // 193      Degradation B for class 14
                                                            // 194-215  Reserved
    CLOCK_CLASS_ALTERNATE_PTP_3_1                   = 216,  // 216-219  For use by alternate PTP profiles
    CLOCK_CLASS_ALTERNATE_PTP_3_2                   = 217,  //
    CLOCK_CLASS_ALTERNATE_PTP_3_3                   = 218,  //
    CLOCK_CLASS_ALTERNATE_PTP_3_4                   = 219,  //
                                                            // 233-247  Reserved
    CLOCK_CLASS_DEFAULT                             = 248,  // 248      Default clock class, used if no other applies
                                                            // 249-250  Reserved
    CLOCK_CLASS_PTP_V1                              = 251,  // 251      Reserved for version 1 compatibility
                                                            // 252-254  Reserved
    CLOCK_CLASS_SLAVE_ONLY                          = 255,  // 255      For slave-only clocks
}
ClockClass_t;

// Clock accuracy, see table 6 in IEEE 1588-2008
typedef enum
{
                                                            // 1-1F     Reserved
    CLOCK_ACCURACY_25_NS                = 0x20,             //          < 25ns
    CLOCK_ACCURACY_100_NS               = 0x21,             //          < 100ns
    CLOCK_ACCURACY_250_NS               = 0x22,             //          < 250ns
    CLOCK_ACCURACY_1_US                 = 0x23,             //          < 1us
    CLOCK_ACCURACY_2_5_US               = 0x24,             //          < 2.5us
    CLOCK_ACCURACY_10_US                = 0x25,             //          < 10us
    CLOCK_ACCURACY_25_US                = 0x26,             //          < 25us
    CLOCK_ACCURACY_100_US               = 0x27,             //          < 100us
    CLOCK_ACCURACY_250_US               = 0x28,             //          < 250us
    CLOCK_ACCURACY_1_MS                 = 0x29,             //          < 1ms
    CLOCK_ACCURACY_2_5_MS               = 0x2A,             //          < 2.5ms
    CLOCK_ACCURACY_10_MS                = 0x2B,             //          < 10ms
    CLOCK_ACCURACY_25_MS                = 0x2C,             //          < 25ms
    CLOCK_ACCURACY_100_MS               = 0x2D,             //          < 100ms
    CLOCK_ACCURACY_250_MS               = 0x2E,             //          < 250ms
    CLOCK_ACCURACY_1_S                  = 0x2F,             //          < 1s
    CLOCK_ACCURACY_10_S                 = 0x30,             //          < 10s
    CLOCK_ACCURACY_OVER_10_S            = 0x31,             //          > 10s
                                                            // 32-79    Reserved
    CLOCK_ACCURACY_PROFILE_SPECIFIC_1   = 0x80,             //          Alternate PTP profile
    CLOCK_ACCURACY_PROFILE_SPECIFIC_2   = 0x81,             //          Alternate PTP profile
    CLOCK_ACCURACY_PROFILE_SPECIFIC_3   = 0x82,             //          Alternate PTP profile
    CLOCK_ACCURACY_PROFILE_SPECIFIC_4   = 0x83,             //          Alternate PTP profile
                                                            // 84-FD    Alternate PTP profile
    CLOCK_ACCURACY_UNKNOWN              = 0xFE,             //          Accuracy unknown
                                                            // FF       Reserved
}
clockAccuracy_t;

// Time source, see table 7 in IEEE 1588-2008
typedef enum
{
    TIME_SRC_ATOMIC_CLOCK               = 0x10,             //          Atomic clocks
    TIME_SRC_GPS                        = 0x20,             //          Any satellite based time source
    TIME_SRC_TERRESTRIAL_RADIO          = 0x30,             //          Radio distributed time
    TIME_SRC_PTP                        = 0x40,             //          Any PTP source outside the domain
    TIME_SRC_NTP                        = 0x50,             //          NTP or SNTP
    TIME_SRC_HAND_SET                   = 0x60,             //          Set by human interaction
    TIME_SRC_OTHER                      = 0x90,             //          Other time source
    TIME_SRC_INTERNAL_OSCILLATOR        = 0xA0,             //          Based on free-running oscillator
    TIME_SRC_PROFILE_SPECIFIC_1         = 0xF0,             //          Alternate PTP profile specific value
    TIME_SRC_PROFILE_SPECIFIC_2         = 0xF1,             //          Alternate PTP profile specific value
    TIME_SRC_PROFILE_SPECIFIC_3         = 0xF2,             //          Alternate PTP profile specific value
    TIME_SRC_PROFILE_SPECIFIC_4         = 0xF3,             //          Alternate PTP profile specific value
                                                            // F4-FE    Alternate PTP profile specific value
                                                            // FF       Reserved
}
timeSource_t;

// Port state, see table 8 in IEEE 1588-2008
typedef enum
{
    PORT_STATE_INITIALIZING     = 0x01,         //
    PORT_STATE_FAULTY           = 0x02,         //
    PORT_STATE_DISABLED         = 0x03,         //
    PORT_STATE_LISTENING        = 0x04,         //
    PORT_STATE_PRE_MASTER       = 0x05,         //
    PORT_STATE_MASTER           = 0x06,         //
    PORT_STATE_PASSIVE          = 0x07,         //
    PORT_STATE_UNCALIBRATED     = 0x08,         //
    PORT_STATE_SLAVE            = 0x09,         //
}
portState_t;

// Port state decisions as returned by state decision algorithm (Figure 26)
typedef enum
{
    PORT_SD_LIST  = 10,     //
    PORT_SD_M1    = 43,     // Numerical values for state decisions were chosen arbitrary
    PORT_SD_M2    = 42,     //
    PORT_SD_M3    = 41,     // Higher value implies state is more 'active'
    PORT_SD_P1    = 22,     //
    PORT_SD_P2    = 21,     //
    PORT_SD_S1    = 31,     //
}
portStateDecision_t;

// Port events, see table 11 in IEEE 1588-2008
typedef enum
{
    PORT_EVENT_POWERUP,                         // All ports
    PORT_EVENT_INITIALIZE,                      // All ports
    PORT_EVENT_INITIALIZE_DONE,                 // All ports, non-PTP-standard event
    PORT_EVENT_FAULT_DETECTED,                  // All affected ports
    PORT_EVENT_FAULT_CLEARED,                   // All affected ports
    PORT_EVENT_STATE_DECISION,                  // All ports
    PORT_EVENT_ANNOUNCE_RCV_TIMEOUT,            // Single port
    PORT_EVENT_QUALIFICATION_TIMEOUT,           // Single port
    PORT_EVENT_MASTER_CLOCK_SELECTED,           // Single port
    PORT_EVENT_SYNCHRONIZATION_FAULT,           // Single port, trigger is implementation specific
    PORT_EVENT_DESIGNATED_ENABLE,               // Single port
    PORT_EVENT_DESIGNATED_DISABLE,              // Single port
}
portEvent_t;

// Delay mechanism, see table 9 in IEEE 1588-2008
typedef enum
{
    DELAY_MECH_E2E          = 0x01,             //
    DELAY_MECH_P2P          = 0x02,             //
    DELAY_MECH_DISABLED     = 0xFE,             //
}
delayMechanism_t;

// Domain number, see table 2 in IEEE 1588-2008
typedef enum
{
    DOMAIN_DEFAULT          = 0,                // 0        Default domain
    DOMAIN_ALTERNATE_1      = 1,                // 1-3      Alternate domains
    DOMAIN_ALTERNATE_2      = 2,                //
    DOMAIN_ALTERNATE_3      = 3,                //
    DOMAIN_USER_1           = 4,                // 4-127    User-defined domains
    DOMAIN_USER_2           = 5,                //
    DOMAIN_USER_3           = 6,                //
    DOMAIN_USER_4           = 7,                //
    DOMAIN_RESERVED         = 128,              // 128-255  Reserved
}
domainNumber_t;

// Severity Code, table 46 in IEEE 1588-2008
typedef enum
{
   SERVIRITY_EMERGENCY          = 0x00,            //               System is unusable
   SERVIRITY_ALERT              = 0x01,            //               Immediate action needed
   SERVIRITY_CRITICAL           = 0x02,            //               Critical conditions
   SERVIRITY_ERROR              = 0x03,            //               Error conditions
   SERVIRITY_WARNING            = 0x04,            //               Warning conditions
   SERVIRITY_NOTICE             = 0x05,            //               Normal but significant condition
   SERVIRITY_INFORMATIONAL      = 0x06,            //               Informational messages
   SERVIRITY_DEBUG              = 0x07,            //               Debug-level messages
                                                   // 0x08-0xFF     Reserved
}
SeverityCode_t;

// TLV Type, table 34 in IEEE 1588-2008
typedef enum
{
    // Not defined in standard
    TLV_INVALID                                     = 0x0000,   // 0x0000   Reserved

    // Standard TLVs
    TLV_MANAGEMENT                                  = 0x0001,   //
    TLV_MANAGEMENT_ERROR_STATUS                     = 0x0002,   //
    TLV_ORGANIZATION_EXTENSION                      = 0x0003,   //

    // Optional unicast message negotiation TLVs
    TLV_REQUEST_UNICAST_TRANSMISSION                = 0x0004,   //
    TLV_GRANT_UNICAST_TRANSMISSION                  = 0x0005,   //
    TLV_CANCEL_UNICAST_TRANSMISSION                 = 0x0006,   //
    TLV_ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION     = 0x0007,   //

    // Optional path trace mechanism TLV
    TLV_PATH_TRACE                                  = 0x0008,   //

    // Optional alternate timescale TLV
    TLV_ALTERNATE_TIME_OFFSET_INDICATOR             = 0x0009,   //
                                                                // 0x000A - 0x1FFF  Reserved for standard TLVs
    // Experimental TLVs
    // -

    // Security TLVs
    TLV_AUTHENTICATION                              = 0x2000,   //
    TLV_AUTHENTICATION_CHALLENGE                    = 0x2001,   //
    TLV_SECURITY_ASSOCIATION_UPDATE                 = 0x2002,   //

    // Cumulative frequency scale factor offset
    TLV_CUM_FREQ_SCALE_FACTOR_OFFSET                = 0x2003,   //
                                                                // 0x2004 - 0x3FFF  Reserved for Experimental TLVs
                                                                // 0x4000 - 0xFFFF  Reserved
}
TLV_Type_t;

// ======================================================
// Declarations
// ======================================================

// ------------------------------------------------------
// Stream operators
// ------------------------------------------------------
std::ostream&   operator<<(std::ostream& os, const tPtpMessageType&     o );
std::ostream&   operator<<(std::ostream& os, const PTP_Profile_t&       o );
std::ostream&   operator<<(std::ostream& os, const PTP_ClockType_t&     o );
std::ostream&   operator<<(std::ostream& os, const PTP_BMCA_t&          o );
std::ostream&   operator<<(std::ostream& os, const PTP_MgmtProtocol_t&  o );
std::ostream&   operator<<(std::ostream& os, const NetworkProtocol_t&   o );
std::ostream&   operator<<(std::ostream& os, const ClockClass_t&        o );
std::ostream&   operator<<(std::ostream& os, const clockAccuracy_t&     o );
std::ostream&   operator<<(std::ostream& os, const timeSource_t&        o );
std::ostream&   operator<<(std::ostream& os, const portState_t&         o );
std::ostream&   operator<<(std::ostream& os, const portStateDecision_t& o );
std::ostream&   operator<<(std::ostream& os, const portEvent_t&         o );
std::ostream&   operator<<(std::ostream& os, const delayMechanism_t&    o );
std::ostream&   operator<<(std::ostream& os, const domainNumber_t&      o );
std::ostream&   operator<<(std::ostream& os, const SeverityCode_t&      o );
std::ostream&   operator<<(std::ostream& os, const TLV_Type_t&          o );

#endif
