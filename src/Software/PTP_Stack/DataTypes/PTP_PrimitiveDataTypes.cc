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

#include "PTP_PrimitiveDataTypes.h"

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
// Stream operators
// ------------------------------------------------------

std::ostream&
operator<<(std::ostream& os, const tPtpMessageType& o )
{
    switch( o )
    {
        // Event
        case PTP_TYPE_SYNC:             os << "Sync";                           break;
        case PTP_TYPE_DELAY_REQ:        os << "Delay Request";                  break;
        case PTP_TYPE_PDELAY_REQ:       os << "Peer Delay Request";             break;
        case PTP_TYPE_PDELAY_RESP:      os << "Peer Delay Response";            break;

        // General
        case PTP_TYPE_FOLLOW_UP:        os << "Sync Follow Up";                 break;
        case PTP_TYPE_DELAY_RESP:       os << "Delay Response";                 break;
        case PTP_TYPE_PDELAY_RESP_FU:   os << "Peer Delay Response Follow Up";  break;
        case PTP_TYPE_ANNOUNCE:         os << "Announce";                       break;
        case PTP_TYPE_SIGNALING:        os << "Signaling";                      break;
        case PTP_TYPE_MANAGEMENT:       os << "Management";                     break;

        default:
        case PTP_TYPE_INVALID:          os << "Invalid";                        break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const PTP_Profile_t& o )
{
    switch( o )
    {
        case PTP_PROFILE_CUSTOM:        os << "Custom profile";             break;
        case PTP_PROFILE_DEFAULT_E2E:   os << "Default profile, E2E";       break;
        case PTP_PROFILE_DEFAULT_P2P:   os << "Default profile, P2P";       break;
        case PTP_PROFILE_POWER:         os << "Power profile, C37.238";     break;

        default:                        os << "Unknown profile";            break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const PTP_ClockType_t& o)
{
    switch( o )
    {
        case PTP_CLOCK_TYPE_ORDINARY:       os << "ordinary/boundary clock";    break;
        case PTP_CLOCK_TYPE_TRANSPARENT:    os << "transparent clock";          break;
        default:                            os << "unsupported clock type";     break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const PTP_BMCA_t& o )
{
    switch( o )
    {
        case BMCA_1588_2008_DEFAULT:        os << "Default 1588-2008 Best Master Clock Algorithm";  break;
        default:                            os << "Unknown Best Mast Clock Algorithm";              break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const PTP_MgmtProtocol_t&o )
{
    switch( o )
    {
        case MGMT_1588_2008_DEFAULT:        os << "Default 1588-2008 management protocol";          break;
        case MGMT_SNMP:                     os << "Simple Network Management Protocol (SNMP)";      break;
        default:                            os << "Unknown management protocol";                    break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const NetworkProtocol_t& o)
{
    switch( o )
    {
        case PROTOCOL_UDP_IPv4:     os << "UDP/IPv4";           break;
        case PROTOCOL_UDP_IPv6:     os << "UDP/IPv6";           break;
        case PROTOCOL_IEEE_802_3:   os << "IEEE 802.3";         break;
        case PROTOCOL_DEVICE_NET:   os << "DeviceNet";          break;
        case PROTOCOL_CONTROL_NET:  os << "ControlNet";         break;
        case PROTOCOL_PROFINET:     os << "ProfiNet";           break;
        case PROTOCOL_UNKNOWN:      os << "unknown protol";     break;
        default:                    if(( 0x007 <= o ) && (o <= 0xEFFF))
                                    {
                                        os << "reserved protocol";
                                    }
                                    else if(( 0x007 <= o ) && (o <= 0xEFFF))
                                    {
                                        os << "reserved profile protocol";
                                    }
                                    else
                                    {
                                        os << "unknown protocol";
                                    }
                                    break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const ClockClass_t& o)
{
    switch( o )
    {
        case CLOCK_CLASS_PRIMARY:                           os << "primary";                                break;
        case CLOCK_CLASS_PRIMARY_HOLDOVER:                  os << "primary (holdover)";                     break;
        case CLOCK_CLASS_APP_SPECIFIC:                      os << "app specific";                           break;
        case CLOCK_CLASS_APP_SPECIFIC_HOLDOVER:             os << "app specific (holdover)";                break;
        case CLOCK_CLASS_PRIMARY_HOLDOVER_DEGRADE_A:        os << "primary (holdover, degrade A)";          break;
        case CLOCK_CLASS_APP_SPECIFIC_HOLDOVER_DEGRADE_A:   os << "app specific (holdover, degrade A)";     break;
        case CLOCK_CLASS_ALTERNATE_PTP_1_1:                 os << "alternate PTP profile 1.1";              break;
        case CLOCK_CLASS_ALTERNATE_PTP_1_2:                 os << "alternate PTP profile 1.2";              break;
        case CLOCK_CLASS_ALTERNATE_PTP_1_3:                 os << "alternate PTP profile 1.3";              break;
        case CLOCK_CLASS_ALTERNATE_PTP_1_4:                 os << "alternate PTP profile 1.4";              break;
        case CLOCK_CLASS_ALTERNATE_PTP_2_1:                 os << "alternate PTP profile 2.1";              break;
        case CLOCK_CLASS_ALTERNATE_PTP_2_2:                 os << "alternate PTP profile 2.2";              break;
        case CLOCK_CLASS_ALTERNATE_PTP_2_3:                 os << "alternate PTP profile 2.3";              break;
        case CLOCK_CLASS_ALTERNATE_PTP_2_4:                 os << "alternate PTP profile 2.4";              break;
        case CLOCK_CLASS_PRIMARY_HOLDOVER_DEGRADE_B:        os << "primary (holdover, degrade B)";          break;
        case CLOCK_CLASS_APP_SPECIFIC_HOLDOVER_DEGRADE_B:   os << "app specific (holdover, degrade B)";     break;
        case CLOCK_CLASS_ALTERNATE_PTP_3_1:                 os << "alternate PTP profile 3.1";              break;
        case CLOCK_CLASS_ALTERNATE_PTP_3_2:                 os << "alternate PTP profile 3.2";              break;
        case CLOCK_CLASS_ALTERNATE_PTP_3_3:                 os << "alternate PTP profile 3.3";              break;
        case CLOCK_CLASS_ALTERNATE_PTP_3_4:                 os << "alternate PTP profile 3.4";              break;
        case CLOCK_CLASS_DEFAULT:                           os << "default";                                break;
        case CLOCK_CLASS_PTP_V1:                            os << "PTPv1";                                  break;
        case CLOCK_CLASS_SLAVE_ONLY:                        os << "slave only";                             break;

        default:                                            if
                                                            (
                                                                ( 0 == o ) ||
                                                                (( 9 <= o ) && ( o <= 10 ))
                                                            )
                                                            {
                                                                os << "reserved for future PTP versions";
                                                            }
                                                            else if
                                                            (
                                                                ((   1 <= o ) && ( o <=   5 )) ||
                                                                                 ( o ==   8 )  ||
                                                                ((  11 <= o ) && ( o <=  12 )) ||
                                                                ((  15 <= o ) && ( o <=  51 )) ||
                                                                ((  53 <= o ) && ( o <=  57 )) ||
                                                                ((  59 <= o ) && ( o <=  67 )) ||
                                                                (( 123 <= o ) && ( o <= 127 )) ||
                                                                (( 128 <= o ) && ( o <= 132 )) ||
                                                                (( 188 <= o ) && ( o <= 192 )) ||
                                                                (( 194 <= o ) && ( o <= 215 )) ||
                                                                (( 233 <= o ) && ( o <= 247 )) ||
                                                                (( 249 <= o ) && ( o <= 250 )) ||
                                                                (( 252 <= o ) && ( o <= 254 ))
                                                            )
                                                            {
                                                                os << "reserved";
                                                            }
                                                            else
                                                            {
                                                                os << "unknown";
                                                            }
                                                            break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const clockAccuracy_t& o)
{
    switch( o )
    {
        case CLOCK_ACCURACY_25_NS:                  os << "<25 ns";                 break;
        case CLOCK_ACCURACY_100_NS:                 os << "<100 ns";                break;
        case CLOCK_ACCURACY_250_NS:                 os << "<250 ns";                break;
        case CLOCK_ACCURACY_1_US:                   os << "<1 us";                  break;
        case CLOCK_ACCURACY_2_5_US:                 os << "<2.5 us";                break;
        case CLOCK_ACCURACY_10_US:                  os << "<10 us";                 break;
        case CLOCK_ACCURACY_25_US:                  os << "<25 us";                 break;
        case CLOCK_ACCURACY_100_US:                 os << "<100 us";                break;
        case CLOCK_ACCURACY_250_US:                 os << "<250 us";                break;
        case CLOCK_ACCURACY_1_MS:                   os << "<1 ms";                  break;
        case CLOCK_ACCURACY_2_5_MS:                 os << "<2.5 ms";                break;
        case CLOCK_ACCURACY_10_MS:                  os << "<10 ms";                 break;
        case CLOCK_ACCURACY_25_MS:                  os << "<25 ms";                 break;
        case CLOCK_ACCURACY_100_MS:                 os << "<100 ms";                break;
        case CLOCK_ACCURACY_250_MS:                 os << "<250 ms";                break;
        case CLOCK_ACCURACY_1_S:                    os << "<1 s";                   break;
        case CLOCK_ACCURACY_10_S:                   os << "<10 s";                  break;
        case CLOCK_ACCURACY_OVER_10_S:              os << ">10 s";                  break;
        case CLOCK_ACCURACY_PROFILE_SPECIFIC_1:     os << "profile specific 1";     break;
        case CLOCK_ACCURACY_PROFILE_SPECIFIC_2:     os << "profile specific 2";     break;
        case CLOCK_ACCURACY_PROFILE_SPECIFIC_3:     os << "profile specific 3";     break;
        case CLOCK_ACCURACY_PROFILE_SPECIFIC_4:     os << "profile specific 4";     break;
        case CLOCK_ACCURACY_UNKNOWN:                os << "unknown";                break;

        default:                                    if
                                                    (
                                                        (( 0x01 <= o ) && ( o <= 0x1F )) ||
                                                        (( 0x32 <= o ) && ( o <= 0x79 )) ||
                                                                          ( o == 0xFF )
                                                    )
                                                    {
                                                        os << "reserved";
                                                    }
                                                    else if(( 0x84 <= o ) && ( o <= 0xFD ))
                                                    {
                                                        os << "profile specific";
                                                    }
                                                    else
                                                    {
                                                        os << "unknown";
                                                    }
                                                    break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const timeSource_t& o)
{
    switch( o )
    {
        case TIME_SRC_ATOMIC_CLOCK:             os << "atomic clock";                               break;
        case TIME_SRC_GPS:                      os << "satellite based";                            break;
        case TIME_SRC_TERRESTRIAL_RADIO:        os << "radio distributed time";                     break;
        case TIME_SRC_PTP:                      os << "PTP-based source external to the domain";    break;
        case TIME_SRC_NTP:                      os << "NTP or SNTP";                                break;
        case TIME_SRC_HAND_SET:                 os << "hand set";                                   break;
        case TIME_SRC_OTHER:                    os << "other";                                      break;
        case TIME_SRC_INTERNAL_OSCILLATOR:      os << "internal oscillator";                        break;
        case TIME_SRC_PROFILE_SPECIFIC_1:       os << "profile specific 1";                         break;
        case TIME_SRC_PROFILE_SPECIFIC_2:       os << "profile specific 2";                         break;
        case TIME_SRC_PROFILE_SPECIFIC_3:       os << "profile specific 3";                         break;
        case TIME_SRC_PROFILE_SPECIFIC_4:       os << "profile specific 4";                         break;

        default:                                if(( 0xF4 <= o ) && ( o <= 0xFE ))
                                                {
                                                    os << "profile specific";
                                                }
                                                else if( o == 0xFF )
                                                {
                                                    os << "reserved";
                                                }
                                                else
                                                {
                                                    os << "unknown";
                                                }
                                                break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const portState_t& o)
{
    switch( o )
    {
        case PORT_STATE_INITIALIZING:       os << "Initializing";       break;
        case PORT_STATE_FAULTY:             os << "Faulty";             break;
        case PORT_STATE_DISABLED:           os << "Disabled";           break;
        case PORT_STATE_LISTENING:          os << "Listening";          break;
        case PORT_STATE_PRE_MASTER:         os << "Pre-Master";         break;
        case PORT_STATE_MASTER:             os << "Master";             break;
        case PORT_STATE_PASSIVE:            os << "Passive";            break;
        case PORT_STATE_UNCALIBRATED:       os << "Uncalibrated";       break;
        case PORT_STATE_SLAVE:              os << "Slave";              break;

        default:                            os << "unknown";            break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const portStateDecision_t& o)
{
    switch( o )
    {
        case PORT_SD_LIST:      os << "Stay in listening";              break;
        case PORT_SD_M1:        os << "Master M1 (Grandmaster)";        break;
        case PORT_SD_M2:        os << "Master M2 (Grandmaster)";        break;
        case PORT_SD_M3:        os << "Master M3";                      break;
        case PORT_SD_P1:        os << "Passive P1";                     break;
        case PORT_SD_P2:        os << "Passive P2";                     break;
        case PORT_SD_S1:        os << "Slave";                          break;

        default:                os << "unknown";                        break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const portEvent_t& o)
{
    switch( o )
    {
        case PORT_EVENT_POWERUP:                os << "power up";                   break;
        case PORT_EVENT_INITIALIZE:             os << "initialize";                 break;
        case PORT_EVENT_FAULT_DETECTED:         os << "fault detected";             break;
        case PORT_EVENT_FAULT_CLEARED:          os << "fault cleared";              break;
        case PORT_EVENT_STATE_DECISION:         os << "state decision";             break;
        case PORT_EVENT_ANNOUNCE_RCV_TIMEOUT:   os << "announce receive timeout";   break;
        case PORT_EVENT_QUALIFICATION_TIMEOUT:  os << "qualification timeout";      break;
        case PORT_EVENT_MASTER_CLOCK_SELECTED:  os << "master clock selected";      break;
        case PORT_EVENT_SYNCHRONIZATION_FAULT:  os << "synchronization fault";      break;
        case PORT_EVENT_DESIGNATED_ENABLE:      os << "designated enable";          break;
        case PORT_EVENT_DESIGNATED_DISABLE:     os << "designated disable";         break;

        default:                                os << "unknown";                    break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const delayMechanism_t& o)
{
    switch( o )
    {
        case DELAY_MECH_E2E:        os << "E2E";        break;
        case DELAY_MECH_P2P:        os << "P2P";        break;
        case DELAY_MECH_DISABLED:   os << "disabled";   break;

        default:                    os << "unknown";    break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const domainNumber_t& o)
{
    switch( o )
    {
        case DOMAIN_DEFAULT:        os << "default";                    break;
        case DOMAIN_ALTERNATE_1:    os << "alternate domain 1";         break;
        case DOMAIN_ALTERNATE_2:    os << "alternate domain 2";         break;
        case DOMAIN_ALTERNATE_3:    os << "alternate domain 3";         break;
        case DOMAIN_USER_1:         os << "user defined domain 1";      break;
        case DOMAIN_USER_2:         os << "user defined domain 2";      break;
        case DOMAIN_USER_3:         os << "user defined domain 3";      break;
        case DOMAIN_USER_4:         os << "user defined domain 4";      break;

        default:                    if(( 4 <= o ) && ( o <= 127 ))
                                    {
                                        os << "user defined domain";
                                    }
                                    else if(( 128 <= o ) && ( o <= 255 ))
                                    {
                                        os << "reserved";    break;
                                    }
                                    else
                                    {
                                        os << "unknown";    break;
                                    }
                                    break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const SeverityCode_t& o)
{
    switch( o )
    {
        case SERVIRITY_EMERGENCY:       os << "emergency";          break;
        case SERVIRITY_ALERT:           os << "alert";              break;
        case SERVIRITY_CRITICAL:        os << "critical";           break;
        case SERVIRITY_ERROR:           os << "error";              break;
        case SERVIRITY_WARNING:         os << "warning";            break;
        case SERVIRITY_NOTICE:          os << "notice";             break;
        case SERVIRITY_INFORMATIONAL:   os << "informational";      break;
        case SERVIRITY_DEBUG:           os << "debug";              break;

        default:                        os << "unknown";            break;
    }

    return os;
}

std::ostream&
operator<<(std::ostream& os, const TLV_Type_t& o)
{
    switch( o )
    {
        case TLV_INVALID:                                   os << "invalid";                                    break;
        case TLV_MANAGEMENT:                                os << "management";                                 break;
        case TLV_MANAGEMENT_ERROR_STATUS:                   os << "management error status";                    break;
        case TLV_ORGANIZATION_EXTENSION:                    os << "organization extension";                     break;
        case TLV_REQUEST_UNICAST_TRANSMISSION:              os << "request unicast transmission";               break;
        case TLV_GRANT_UNICAST_TRANSMISSION:                os << "grand unicast transmission";                 break;
        case TLV_CANCEL_UNICAST_TRANSMISSION:               os << "cancel unicast transmission";                break;
        case TLV_ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION:   os << "acknowledge  cancel unicast transmission";   break;
        case TLV_PATH_TRACE:                                os << "path trace";                                 break;
        case TLV_ALTERNATE_TIME_OFFSET_INDICATOR:           os << "alternate time offset indicator";            break;
        case TLV_AUTHENTICATION:                            os << "authentication";                             break;
        case TLV_AUTHENTICATION_CHALLENGE:                  os << "authentication challenge";                   break;
        case TLV_SECURITY_ASSOCIATION_UPDATE:               os << "security association update";                break;
        case TLV_CUM_FREQ_SCALE_FACTOR_OFFSET:              os << "cumulative frequency scale factor offset";   break;

        default:                                            if(( 0x2004 <= o ) && ( o <= 0x3FFF ))
                                                            {
                                                                os << "reserved for Experimental TLVs";
                                                            }
                                                            else if(( 0x4000 <= o ) && ( o <= 0xFFFF ))
                                                            {
                                                                os << "reserved";
                                                            }
                                                            else
                                                            {
                                                                os << "unknown";
                                                            }
                                                            break;
    }

    return os;
}
