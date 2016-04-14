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

#ifndef LIBPTP_PI_CLOCK_SERVO_H_
#define LIBPTP_PI_CLOCK_SERVO_H_

// ======================================================
// Includes
// ======================================================

#include "IClockServo.h"
#include "IClockEventSink.h"

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

class cPI_ClockServo: public IClockServo
{
    public:
        // Type definitions
        enum ParameterMode_t
        {
            PI_PARAMETER_MODE_CONSTANT,
            PI_PARAMETER_MODE_LINUX_PTP,
            PI_PARAMETER_MODE_LIBPTP,
        };

    private:

        // Type definitions
        struct LinuxPTP_Config_t
        {
            double  Scale;
            double  Exp;
            double  MaxNorm;
        };

        struct LibPTP_Config_t
        {
            double  Scale;
            double  Base;
            double  Min;
            double  Max;
        };

        struct KConfig_t
        {
            ParameterMode_t     ParameterMode;
            double              Const;
            LinuxPTP_Config_t   LinuxPTP_Config;
            LibPTP_Config_t     LibPTP_Config;
        };

        // Resources

        // Configuration
        KConfig_t       KP_Config;
        KConfig_t       KI_Config;
        double          KP;
        double          KI;
        double          Integral;

        // Signals
        simsignal_t     P_SigId;
        simsignal_t     I_SigId;
        simsignal_t     Integral_SigId;
        simsignal_t     KP_SigId;
        simsignal_t     KI_SigId;

        // ------------------------------------------------------------
        // Private functions
        // ------------------------------------------------------------

        // Configuration
        double  CalcK( simtime_t ControlInterval, KConfig_t Config );

        // Sample API
        void    PrintConfig();
        void    Reset();
        void    SetKP( double KP );
        void    SetKI( double KI );
        void    SetSyncInterval( simtime_t SyncInterval );
        void    SampleInternal( simtime_t offsetFromMaster, simtime_t Ingress );

        // Init API
        void    ParseParameters();
        void    RegisterSignals();
        void    InitSignals();
        void    PrintDebugOutput();

    protected:

        // State handling
        virtual void    SetServoState( ClockServoState NewServoState );

        // OMNeT API
        virtual void    handleMessage(cMessage *msg);
        virtual void    finish();

    public:

        // Constructor/Desctructor
        cPI_ClockServo();
        ~cPI_ClockServo();
};

// ------------------------------------------------------
// Stream operators
// ------------------------------------------------------
std::ostream&   operator<<(std::ostream& os, const cPI_ClockServo::ParameterMode_t& o );

#endif
