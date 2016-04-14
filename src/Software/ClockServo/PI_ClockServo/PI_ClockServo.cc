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

#include <omnetpp.h>

#include "PI_ClockServo.h"
#include "PI_ClockServo_ParameterParser.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Constants
// ======================================================

// ======================================================
// Variables
// ======================================================

// ======================================================
// Declarations
// ======================================================

Define_Module(cPI_ClockServo);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Configuration
// ------------------------------------------------------

double
cPI_ClockServo::CalcK( simtime_t ControlInterval, KConfig_t Config )
{
    double  K;
    double  Limit;
    double  Interval    = ControlInterval.dbl();

    if( ControlInterval == SIMTIME_ZERO )
    {
        throw cRuntimeError( "ControlInterval can't be 0." );
    }

    switch( Config.ParameterMode )
    {
        case PI_PARAMETER_MODE_CONSTANT:
        {
            K = Config.Const;
            break;
        }

        case PI_PARAMETER_MODE_LINUX_PTP:
        {
            K           = Config.LinuxPTP_Config.Scale * pow( Interval, Config.LinuxPTP_Config.Exp );
            Limit       = Config.LinuxPTP_Config.MaxNorm / Interval;

            K = std::min( K, Limit ); // Enforce limit
            break;
        }

        case PI_PARAMETER_MODE_LIBPTP:
        {
            double  SyncExp = log2( Interval );

            K = Config.LibPTP_Config.Scale * pow( Config.LibPTP_Config.Base, SyncExp );

            K = std::max( K, Config.LibPTP_Config.Min );
            K = std::min( K, Config.LibPTP_Config.Max );
            break;
        }
    }

    return K;
}

// ------------------------------------------------------
// Print config
// ------------------------------------------------------
void
cPI_ClockServo::PrintConfig()
{
    EV << endl;
    EV << "---------------------------" << endl;
    EV << "ClockServo configuration:" << endl;
    EV << "---------------------------" << endl;
    EV << endl;

    EV << "Proportional config: " << endl;
    EV << "ParameterMode: " << KP_Config.ParameterMode << endl;
    EV << endl;
    EV << "Const:        " << KP_Config.Const        << endl;
    EV << endl;
    EV << "LinuxPTP Config:" << endl;
    EV << "Scale:        " << KP_Config.LinuxPTP_Config.Scale   << endl;
    EV << "Exp:          " << KP_Config.LinuxPTP_Config.Exp     << endl;
    EV << "MaxNorm:      " << KP_Config.LinuxPTP_Config.MaxNorm << endl;
    EV << endl;
    EV << "LibPTP Config:" << endl;
    EV << "Scale:        " << KP_Config.LibPTP_Config.Scale     << endl;
    EV << "Base:         " << KP_Config.LibPTP_Config.Base      << endl;
    EV << "Min:          " << KP_Config.LibPTP_Config.Min       << endl;
    EV << "Max:          " << KP_Config.LibPTP_Config.Max       << endl;

    EV << "Integral config: " << endl;
    EV << "ParameterMode: " << KI_Config.ParameterMode << endl;
    EV << endl;
    EV << "Const:        " << KI_Config.Const        << endl;
    EV << endl;
    EV << "LinuxPTP Config:" << endl;
    EV << "Scale:        " << KI_Config.LinuxPTP_Config.Scale   << endl;
    EV << "Exp:          " << KI_Config.LinuxPTP_Config.Exp     << endl;
    EV << "MaxNorm:      " << KI_Config.LinuxPTP_Config.MaxNorm << endl;
    EV << endl;
    EV << "Scale:        " << KI_Config.LibPTP_Config.Scale     << endl;
    EV << "Base:         " << KI_Config.LibPTP_Config.Base      << endl;
    EV << "Min:          " << KI_Config.LibPTP_Config.Min       << endl;
    EV << "Max:          " << KI_Config.LibPTP_Config.Max       << endl;

    EV << "ScaleFactor UpperBound [ppb]: " << ScaleFactor_UpperBound_ppb << endl;
    EV << "ScaleFactor LowerBound [ppb]: " << ScaleFactor_LowerBound_ppb << endl;
    EV << endl;
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
cPI_ClockServo::ParseParameters()
{
    IClockServo::ParseParameters();

    KP_Config.ParameterMode                 = cPI_ClockServo_ParameterParser::ParseParameterMode( par( "KP_ParameterMode" ).stringValue() );
    KP_Config.Const                         = par( "KP_Const"            ).doubleValue();

    KP_Config.LinuxPTP_Config.Scale         = par( "KP_LinuxPTP_Scale"   ).doubleValue();
    KP_Config.LinuxPTP_Config.Exp           = par( "KP_LinuxPTP_Exp"     ).doubleValue();
    KP_Config.LinuxPTP_Config.MaxNorm       = par( "KP_LinuxPTP_MaxNorm" ).doubleValue();

    KP_Config.LibPTP_Config.Scale           = par( "KP_LibPTP_Scale"     ).doubleValue();
    KP_Config.LibPTP_Config.Base            = par( "KP_LibPTP_Base"      ).doubleValue();
    KP_Config.LibPTP_Config.Min             = par( "KP_LibPTP_Min"       ).doubleValue();
    KP_Config.LibPTP_Config.Max             = par( "KP_LibPTP_Max"       ).doubleValue();

    KI_Config.ParameterMode                 = cPI_ClockServo_ParameterParser::ParseParameterMode( par( "KI_ParameterMode" ).stringValue() );
    KI_Config.Const                         = par( "KI_Const"            ).doubleValue();

    KI_Config.LinuxPTP_Config.Scale         = par( "KI_LinuxPTP_Scale"   ).doubleValue();
    KI_Config.LinuxPTP_Config.Exp           = par( "KI_LinuxPTP_Exp"     ).doubleValue();
    KI_Config.LinuxPTP_Config.MaxNorm       = par( "KI_LinuxPTP_MaxNorm" ).doubleValue();

    KI_Config.LibPTP_Config.Scale           = par( "KI_LibPTP_Scale"     ).doubleValue();
    KI_Config.LibPTP_Config.Base            = par( "KI_LibPTP_Base"      ).doubleValue();
    KI_Config.LibPTP_Config.Min             = par( "KI_LibPTP_Min"       ).doubleValue();
    KI_Config.LibPTP_Config.Max             = par( "KI_LibPTP_Max"       ).doubleValue();
}

void
cPI_ClockServo::RegisterSignals()
{
    IClockServo::RegisterSignals();

    P_SigId                 = registerSignal( "P" );
    I_SigId                 = registerSignal( "I" );
    Integral_SigId          = registerSignal( "Integral" );
    KP_SigId                = registerSignal( "KP" );
    KI_SigId                = registerSignal( "KI" );

    WATCH(KP);
    WATCH(KI);
    WATCH(Integral);
}

void
cPI_ClockServo::InitSignals()
{
    emit( KP_SigId, KP );
    emit( KI_SigId, KI );

    emit( Integral_SigId, Integral );
}

void
cPI_ClockServo::PrintDebugOutput()
{
    IClockServo::PrintDebugOutput();

    if( EnableDebugOutput )
    {
        PrintConfig();
    }
}


void
cPI_ClockServo::SetServoState( ClockServoState NewServoState )
{
    IClockServo::SetServoState( NewServoState );

    switch( NewServoState )
    {
        default:                            // Do nothing
                                            break;

        case ClockServoState::SCALE:        Integral = SampleDec.ScaleFactor_ppb;
                                            emit( Integral_SigId, Integral );
                                            break;
    }
}

// ------------------------------------------------------
// Handle messages
// ------------------------------------------------------
void
cPI_ClockServo::handleMessage(cMessage *pMsg)
{
}

// ------------------------------------------------------
// Finish
// ------------------------------------------------------
void
cPI_ClockServo::finish()
{
}

// ------------------------------------------------------
// Constructors
// ------------------------------------------------------
cPI_ClockServo::cPI_ClockServo()
{
    KP_Config.ParameterMode                 = PI_PARAMETER_MODE_CONSTANT;
    KP_Config.Const                         = 0.0;
    KP_Config.LinuxPTP_Config.Exp           = 0.0;
    KP_Config.LinuxPTP_Config.MaxNorm       = 0.0;
    KP_Config.LinuxPTP_Config.Scale         = 0.0;

    KI_Config.ParameterMode                 = PI_PARAMETER_MODE_CONSTANT;
    KI_Config.Const                         = 0.0;
    KI_Config.LinuxPTP_Config.Exp           = 0.0;
    KI_Config.LinuxPTP_Config.MaxNorm       = 0.0;
    KI_Config.LinuxPTP_Config.Scale         = 0.0;

    KP          = 0.0;
    KI          = 0.0;
    Integral    = 0.0;
}

// ------------------------------------------------------
// Desctructor
// ------------------------------------------------------
cPI_ClockServo::~cPI_ClockServo()
{
}

// ------------------------------------------------------
// Sample API
// ------------------------------------------------------
void
cPI_ClockServo::Reset()
{
    IClockServo::Reset();

    Integral = 0.0;
}

void
cPI_ClockServo::SetKP( double KP )
{
    this->KP = KP;
    emit( KP_SigId, KP );
}

void
cPI_ClockServo::SetKI( double KI )
{
    this->KI = KI;
    emit( KI_SigId, KI );
}

void
cPI_ClockServo::SetSyncInterval( simtime_t SyncInterval )
{
    if( this->SyncInterval != SyncInterval )
    {
        IClockServo::SetSyncInterval( SyncInterval );

        SetKP( CalcK( SyncInterval, KP_Config ) );
        SetKI( CalcK( SyncInterval, KI_Config ) );
    }
}

// ------------------------------------------------------
// Servo internal sampling function
// ------------------------------------------------------
void
cPI_ClockServo::SampleInternal( simtime_t offsetFromMaster, simtime_t Ingress )
{
    int64_t ScaleFactor_ppb = 0;
    int64_t Offset_ns   = offsetFromMaster.inUnit( SIMTIME_NS );

    double  P   = KP * Offset_ns;
    double  I   = KI * Offset_ns;

    ScaleFactor_ppb = P + I + Integral;

    if( ScaleFactor_ppb > ScaleFactor_UpperBound_ppb )
    {
        ScaleFactor_ppb = ScaleFactor_UpperBound_ppb;
    }
    else if( ScaleFactor_ppb < ScaleFactor_LowerBound_ppb )
    {
        ScaleFactor_ppb = ScaleFactor_LowerBound_ppb;
    }
    else
    {
        Integral += I;
    }

    SampleDec.EnableJump        = false;
    SampleDec.Delta             = SIMTIME_ZERO;
    SampleDec.EnableScale       = true;
    SampleDec.ScaleFactor_ppb   = ScaleFactor_ppb;

    emit( P_SigId, P );
    emit( I_SigId, I );
    emit( Integral_SigId, Integral );
}

// ------------------------------------------------------
// Stream operators
// ------------------------------------------------------
std::ostream&
operator<<(std::ostream& os, const cPI_ClockServo::ParameterMode_t& o )
{
    switch( o )
    {
        default:                                                            os << "Unknown parameter mode"; break;
        case cPI_ClockServo::ParameterMode_t::PI_PARAMETER_MODE_CONSTANT:   os << "Constant";               break;
        case cPI_ClockServo::ParameterMode_t::PI_PARAMETER_MODE_LINUX_PTP:  os << "LinuxPTP";               break;
        case cPI_ClockServo::ParameterMode_t::PI_PARAMETER_MODE_LIBPTP:     os << "LibPTP";                 break;
    }

    return os;
}
