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

#ifndef LIBPTP_BYTE_ORDER_H_
#define LIBPTP_BYTE_ORDER_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_PrimitiveDataTypes.h"

#include <cstddef>

// ======================================================
// Definitions
// ======================================================

// ======================================================
// Types
// ======================================================

// ======================================================
// Declarations
// ======================================================

UInteger16  NetToHostUI16( UInteger16 UInt16 );
UInteger32  NetToHostUI32( UInteger32 UInt32 );
Integer16   NetToHostI16 ( Integer16  Int16  );
Integer32   NetToHostI32 ( Integer32  Int32  );
Integer64   NetToHostI64 ( Integer64  Int64  );

UInteger48  BufToHostUI48( Octet Buf[], size_t BufSize );

UInteger16  HostToNetUI16( UInteger16 UInt16 );
UInteger32  HostToNetUI32( UInteger32 UInt32 );
Integer16   HostToNetI16 ( Integer16  Int16  );
Integer32   HostToNetI32 ( Integer32  Int32  );
Integer64   HostToNetI64 ( Integer64  Int64  );

void        HostToBufUI48( UInteger48 UInt48, Octet Buf[], size_t BufSize );

#endif
