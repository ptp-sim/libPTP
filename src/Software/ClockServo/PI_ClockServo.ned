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

package libptp.Software.ClockServo;

simple PI_ClockServo extends Internal_ClockServo
{
    parameters:

        @display("i=PTP/Components/ClockServo/PI_ClockServo");
        @class(cPI_ClockServo);

        // Control configuration
        string  KP_ParameterMode            = default( "LIBPTP" );
        double  KP_Const                    = default( 0.0  );
        double  KP_LinuxPTP_Scale           = default( 0.7  );
        double  KP_LinuxPTP_Exp             = default( -0.3 );
        double  KP_LinuxPTP_MaxNorm         = default( 0.7  );
        double  KP_LibPTP_Scale             = default( 0.4  );
        double  KP_LibPTP_Base              = default( 0.57 );
        double  KP_LibPTP_Min               = default( 0    );
        double  KP_LibPTP_Max               = default( 10   );

        string  KI_ParameterMode            = default( "LIBPTP" );
        double  KI_Const                    = default( 0.0  );
        double  KI_LinuxPTP_Scale           = default( 0.3  );
        double  KI_LinuxPTP_Exp             = default( 0.4  );
        double  KI_LinuxPTP_MaxNorm         = default( 0.3  );
        double  KI_LibPTP_Scale             = default( 0.0012 );
        double  KI_LibPTP_Base              = default( 1.7    );
        double  KI_LibPTP_Min               = default( 0.0005 );
        double  KI_LibPTP_Max               = default( 0.0070 );

        // -----------------------------------------------------------------------
        // Signals
        // -----------------------------------------------------------------------
        @signal[P](type=double);
        @signal[I](type=double);
        @signal[Integral](type=double);
        @signal[KP](type=double);
        @signal[KI](type=double);

        // -----------------------------------------------------------------------
        // Statistics
        // -----------------------------------------------------------------------
        @statistic[P](record=vector?);
        @statistic[I](record=vector?);
        @statistic[Integral](record=vector?);
        @statistic[KP](record=vector?);
        @statistic[KI](record=vector?);
}

