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

#include "PTP_Parser.h"
#include "ParameterParser.h"

// ======================================================
// Definitions
// ======================================================

// ======================================================
// Types
// ======================================================

// ======================================================
// Variables
// ======================================================

ParseType<PTP_ClockType_t>      PTP_ClockTypeParse[] =
{
    { PTP_CLOCK_TYPE_ORDINARY,                          "PTP_CLOCK_TYPE_ORDINARY"                       },
    { PTP_CLOCK_TYPE_TRANSPARENT,                       "PTP_CLOCK_TYPE_TRANSPARENT"                    },
};

ParseType<PTP_Profile_t>        PTP_ProfileParse[]   =
{
    { PTP_PROFILE_CUSTOM,                               "PTP_PROFILE_CUSTOM"                            },
    { PTP_PROFILE_DEFAULT_E2E,                          "PTP_PROFILE_DEFAULT_E2E"                       },
    { PTP_PROFILE_DEFAULT_P2P,                          "PTP_PROFILE_DEFAULT_P2P"                       },
    { PTP_PROFILE_POWER,                                "PTP_PROFILE_POWER"                             },
};

ParseType<PTP_BMCA_t>           BestMasterClockAlgorithmParse[] =
{
    { BMCA_1588_2008_DEFAULT,                           "BMCA_1588_2008_DEFAULT"                        },
};

ParseType<PTP_MgmtProtocol_t>   MgmtProtocolParse[] =
{
    { MGMT_1588_2008_DEFAULT,                           "MGMT_1588_2008_DEFAULT"                        },
    { MGMT_SNMP,                                        "MGMT_SNMP"                                     },
};

ParseType<ClockClass_t>         ClockClassParse[]    =
{
    { CLOCK_CLASS_PRIMARY,                              "CLOCK_CLASS_PRIMARY"                           },
    { CLOCK_CLASS_PRIMARY_HOLDOVER,                     "CLOCK_CLASS_PRIMARY_HOLDOVER"                  },
    { CLOCK_CLASS_APP_SPECIFIC,                         "CLOCK_CLASS_APP_SPECIFIC"                      },
    { CLOCK_CLASS_APP_SPECIFIC_HOLDOVER,                "CLOCK_CLASS_APP_SPECIFIC_HOLDOVER"             },
    { CLOCK_CLASS_PRIMARY_HOLDOVER_DEGRADE_A,           "CLOCK_CLASS_PRIMARY_HOLDOVER_DEGRADE_A"        },
    { CLOCK_CLASS_APP_SPECIFIC_HOLDOVER_DEGRADE_A,      "CLOCK_CLASS_APP_SPECIFIC_HOLDOVER_DEGRADE_A"   },
    { CLOCK_CLASS_ALTERNATE_PTP_1_1,                    "CLOCK_CLASS_ALTERNATE_PTP_1_1"                 },
    { CLOCK_CLASS_ALTERNATE_PTP_1_2,                    "CLOCK_CLASS_ALTERNATE_PTP_1_2"                 },
    { CLOCK_CLASS_ALTERNATE_PTP_1_3,                    "CLOCK_CLASS_ALTERNATE_PTP_1_3"                 },
    { CLOCK_CLASS_ALTERNATE_PTP_1_4,                    "CLOCK_CLASS_ALTERNATE_PTP_1_4"                 },
    { CLOCK_CLASS_ALTERNATE_PTP_2_1,                    "CLOCK_CLASS_ALTERNATE_PTP_2_1"                 },
    { CLOCK_CLASS_ALTERNATE_PTP_2_2,                    "CLOCK_CLASS_ALTERNATE_PTP_2_2"                 },
    { CLOCK_CLASS_ALTERNATE_PTP_2_3,                    "CLOCK_CLASS_ALTERNATE_PTP_2_3"                 },
    { CLOCK_CLASS_ALTERNATE_PTP_2_4,                    "CLOCK_CLASS_ALTERNATE_PTP_2_4"                 },
    { CLOCK_CLASS_PRIMARY_HOLDOVER_DEGRADE_B,           "CLOCK_CLASS_PRIMARY_HOLDOVER_DEGRADE_B"        },
    { CLOCK_CLASS_APP_SPECIFIC_HOLDOVER_DEGRADE_B,      "CLOCK_CLASS_APP_SPECIFIC_HOLDOVER_DEGRADE_B"   },
    { CLOCK_CLASS_ALTERNATE_PTP_3_1,                    "CLOCK_CLASS_ALTERNATE_PTP_3_1"                 },
    { CLOCK_CLASS_ALTERNATE_PTP_3_2,                    "CLOCK_CLASS_ALTERNATE_PTP_3_2"                 },
    { CLOCK_CLASS_ALTERNATE_PTP_3_3,                    "CLOCK_CLASS_ALTERNATE_PTP_3_3"                 },
    { CLOCK_CLASS_ALTERNATE_PTP_3_4,                    "CLOCK_CLASS_ALTERNATE_PTP_3_4"                 },
    { CLOCK_CLASS_DEFAULT,                              "CLOCK_CLASS_DEFAULT"                           },
    { CLOCK_CLASS_PTP_V1,                               "CLOCK_CLASS_PTP_V1"                            },
    { CLOCK_CLASS_SLAVE_ONLY,                           "CLOCK_CLASS_SLAVE_ONLY"                        },
};

ParseType<clockAccuracy_t>      ClockAccuracyParse[]    =
{
    { CLOCK_ACCURACY_25_NS,                             "CLOCK_ACCURACY_25_NS"                  },
    { CLOCK_ACCURACY_100_NS,                            "CLOCK_ACCURACY_100_NS"                 },
    { CLOCK_ACCURACY_250_NS,                            "CLOCK_ACCURACY_250_NS"                 },
    { CLOCK_ACCURACY_1_US,                              "CLOCK_ACCURACY_1_US"                   },
    { CLOCK_ACCURACY_2_5_US,                            "CLOCK_ACCURACY_2_5_US"                 },
    { CLOCK_ACCURACY_10_US,                             "CLOCK_ACCURACY_10_US"                  },
    { CLOCK_ACCURACY_25_US,                             "CLOCK_ACCURACY_25_US"                  },
    { CLOCK_ACCURACY_100_US,                            "CLOCK_ACCURACY_100_US"                 },
    { CLOCK_ACCURACY_250_US,                            "CLOCK_ACCURACY_250_US"                 },
    { CLOCK_ACCURACY_1_MS,                              "CLOCK_ACCURACY_1_MS"                   },
    { CLOCK_ACCURACY_2_5_MS,                            "CLOCK_ACCURACY_2_5_MS"                 },
    { CLOCK_ACCURACY_10_MS,                             "CLOCK_ACCURACY_10_MS"                  },
    { CLOCK_ACCURACY_25_MS,                             "CLOCK_ACCURACY_25_MS"                  },
    { CLOCK_ACCURACY_100_MS,                            "CLOCK_ACCURACY_100_MS"                 },
    { CLOCK_ACCURACY_250_MS,                            "CLOCK_ACCURACY_250_MS"                 },
    { CLOCK_ACCURACY_1_S,                               "CLOCK_ACCURACY_1_S"                    },
    { CLOCK_ACCURACY_10_S,                              "CLOCK_ACCURACY_10_S"                   },
    { CLOCK_ACCURACY_OVER_10_S,                         "CLOCK_ACCURACY_OVER_10_S"              },
    { CLOCK_ACCURACY_PROFILE_SPECIFIC_1,                "CLOCK_ACCURACY_PROFILE_SPECIFIC_1"     },
    { CLOCK_ACCURACY_PROFILE_SPECIFIC_2,                "CLOCK_ACCURACY_PROFILE_SPECIFIC_2"     },
    { CLOCK_ACCURACY_PROFILE_SPECIFIC_3,                "CLOCK_ACCURACY_PROFILE_SPECIFIC_3"     },
    { CLOCK_ACCURACY_PROFILE_SPECIFIC_4,                "CLOCK_ACCURACY_PROFILE_SPECIFIC_4"     },
    { CLOCK_ACCURACY_UNKNOWN,                           "CLOCK_ACCURACY_UNKNOWN"                },
};

ParseType<timeSource_t>         TimeSourceParse[]    =
{
    { TIME_SRC_ATOMIC_CLOCK,                             "TIME_SRC_ATOMIC_CLOCK"                },
    { TIME_SRC_GPS,                                      "TIME_SRC_GPS"                         },
    { TIME_SRC_TERRESTRIAL_RADIO,                        "TIME_SRC_TERRESTRIAL_RADIO"           },
    { TIME_SRC_PTP,                                      "TIME_SRC_PTP"                         },
    { TIME_SRC_NTP,                                      "TIME_SRC_NTP"                         },
    { TIME_SRC_HAND_SET,                                 "TIME_SRC_HAND_SET"                    },
    { TIME_SRC_OTHER,                                    "TIME_SRC_OTHER"                       },
    { TIME_SRC_INTERNAL_OSCILLATOR,                      "TIME_SRC_INTERNAL_OSCILLATOR"         },
    { TIME_SRC_PROFILE_SPECIFIC_1,                       "TIME_SRC_PROFILE_SPECIFIC_1"          },
    { TIME_SRC_PROFILE_SPECIFIC_2,                       "TIME_SRC_PROFILE_SPECIFIC_2"          },
    { TIME_SRC_PROFILE_SPECIFIC_3,                       "TIME_SRC_PROFILE_SPECIFIC_3"          },
    { TIME_SRC_PROFILE_SPECIFIC_4,                       "TIME_SRC_PROFILE_SPECIFIC_4"          },
};

ParseType<delayMechanism_t>     DelayMechParse[]    =
{
    { DELAY_MECH_E2E,                                    "DELAY_MECH_E2E"                       },
    { DELAY_MECH_P2P,                                    "DELAY_MECH_P2P"                       },
    { DELAY_MECH_DISABLED,                               "DELAY_MECH_DISABLED"                  },
};

ParseType<domainNumber_t>       DomainNumberParse[]    =
{
    { DOMAIN_DEFAULT,                                    "DOMAIN_DEFAULT"                       },
    { DOMAIN_ALTERNATE_1,                                "DOMAIN_ALTERNATE_1"                   },
    { DOMAIN_ALTERNATE_2,                                "DOMAIN_ALTERNATE_2"                   },
    { DOMAIN_ALTERNATE_3,                                "DOMAIN_ALTERNATE_3"                   },
    { DOMAIN_USER_1,                                     "DOMAIN_USER_1"                        },
    { DOMAIN_USER_2,                                     "DOMAIN_USER_2"                        },
    { DOMAIN_USER_3,                                     "DOMAIN_USER_3"                        },
    { DOMAIN_USER_4,                                     "DOMAIN_USER_4"                        },
    { DOMAIN_RESERVED,                                   "DOMAIN_RESERVED"                      },
};

// ======================================================
// Declarations
// ======================================================

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Clock type
// ------------------------------------------------------
PTP_ClockType_t
cPTP_Parser::ParsePtpClockType(const char *Str)
{
    return Parse<PTP_ClockType_t>( PTP_ClockTypeParse, ArrayLen(PTP_ClockTypeParse), Str );
}

// ------------------------------------------------------
// PTP Profile
// ------------------------------------------------------
PTP_Profile_t
cPTP_Parser::ParsePtpProfile(const char *Str)
{
    return Parse<PTP_Profile_t>( PTP_ProfileParse, ArrayLen(PTP_ProfileParse), Str );
}

// ------------------------------------------------------
// Best Master Clock Algorithm
// ------------------------------------------------------
PTP_BMCA_t
cPTP_Parser::ParseBestMasterClockAlgorithm(const char *Str)
{
    return Parse<PTP_BMCA_t>( BestMasterClockAlgorithmParse, ArrayLen(BestMasterClockAlgorithmParse), Str );
}

// ------------------------------------------------------
// Management protocol
// ------------------------------------------------------
PTP_MgmtProtocol_t
cPTP_Parser::ParseManagementProtocol(const char *Str)
{
    return Parse<PTP_MgmtProtocol_t>( MgmtProtocolParse, ArrayLen(MgmtProtocolParse), Str );
}

// ------------------------------------------------------
// Clock class
// ------------------------------------------------------
ClockClass_t
cPTP_Parser::ParseClockClass(const char *Str)
{
    return Parse<ClockClass_t>( ClockClassParse, ArrayLen(ClockClassParse), Str );
}

// ------------------------------------------------------
// Clock accuracy
// ------------------------------------------------------
clockAccuracy_t
cPTP_Parser::ParseClockAccuracy (const char *Str)
{
    return Parse<clockAccuracy_t>( ClockAccuracyParse, ArrayLen(ClockAccuracyParse), Str );
}

// ------------------------------------------------------
// Time source
// ------------------------------------------------------
timeSource_t
cPTP_Parser::ParseTimeSource(const char *Str)
{
    return Parse<timeSource_t>( TimeSourceParse, ArrayLen(TimeSourceParse), Str );
}

// ------------------------------------------------------
// Delay mechanism
// ------------------------------------------------------
delayMechanism_t
cPTP_Parser::ParseDelayMechanism(const char *Str)
{
    return Parse<delayMechanism_t>( DelayMechParse, ArrayLen(DelayMechParse), Str );
}

// ------------------------------------------------------
// Domain number
// ------------------------------------------------------
domainNumber_t
cPTP_Parser::ParseDomainNumber(const char *Str)
{
    return Parse<domainNumber_t>( DomainNumberParse, ArrayLen(DomainNumberParse), Str );
}

// ------------------------------------------------------
//
// ------------------------------------------------------
int8_t
cPTP_Parser::ParseParameterInt8( cComponent *pComp, const char *ParName)
{
    int x;

    x   = pComp->par( ParName ).longValue();

    if( x >= -128 && x <= 127 )
        return (int8_t) x;
    else
        throw cRuntimeError("Parsing exception: Value for '%' is out of range.", ParName );
}

// ------------------------------------------------------
//
// ------------------------------------------------------
uint8_t
cPTP_Parser::ParseParameterUInt8( cComponent *pComp, const char *ParName)
{
    int x;

    x   = pComp->par( ParName ).longValue();

    if( x >= 0 && x <= 255 )
        return (int8_t) x;
    else
        throw cRuntimeError("Parsing exception: Value for '%' is out of range.", ParName );
}

// ------------------------------------------------------
//
// ------------------------------------------------------
int
cPTP_Parser::ParsePortParameter(int PortNumber, cComponent *pComp, const char *ParName)
{
    std::stringstream   StringStream;
    std::string         FullName;

    int x;

    StringStream << "PORT_" << PortNumber << "_" << ParName;
    FullName    = StringStream.str();

    x   = pComp->par( FullName.data() ).longValue();

    return x;
}

// ------------------------------------------------------
//
// ------------------------------------------------------
int8_t
cPTP_Parser::ParsePortParameterInt8(int PortNumber, cComponent *pComp, const char *ParName)
{
    int x;

    x   = ParsePortParameter( PortNumber, pComp, ParName );

    if( x >= -128 && x <= 127 )
        return (int8_t) x;
    else
        throw cRuntimeError("Parsing exception: Value for '%' of Port %d is out of range.", ParName, PortNumber );
}

// ------------------------------------------------------
//
// ------------------------------------------------------
uint8_t
cPTP_Parser::ParsePortParameterUInt8(int PortNumber, cComponent *pComp, const char *ParName)
{
    int x;

    x   = ParsePortParameter( PortNumber, pComp, ParName );

    if( x >= 0 && x <= 255 )
        return (uint8_t) x;
    else
        throw cRuntimeError("Parsing exception: Value for '%' of Port %d is out of range.", ParName, PortNumber );
}

// ------------------------------------------------------
//
// ------------------------------------------------------
delayMechanism_t
cPTP_Parser::ParsePortParameterDelayMech(int PortNumber, cComponent *pComp )
{
    std::stringstream   StringStream;
    std::string         FullName;

    StringStream << "PORT_" << PortNumber << "_DelayMechanism";
    FullName    = StringStream.str();

    return ParseDelayMechanism( pComp->par( FullName.data() ).stringValue() );
}
