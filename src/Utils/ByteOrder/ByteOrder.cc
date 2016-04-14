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

#include "ByteOrder.h"

#include <platdep/sockets.h>    // Get htonl etc. for Windows and Linux

#include <omnetpp.h>

// ======================================================
// Remarks
// ======================================================

// Boost/Endianess
//
// The Boost library recently got an Endianess module
// (starting with v1.58). This might be a better option
// than the self developed code here.
//
// http://www.boost.org/doc/libs/1_58_0/libs/endian/doc/index.html

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

UInteger16
NetToHostUI16( UInteger16 UInt16 )
{
    return (UInteger16) ntohs( UInt16 );
}

UInteger32
NetToHostUI32( UInteger32 UInt32 )
{
    return (UInteger32) ntohl( UInt32 );
}

Integer16
NetToHostI16( Integer16  Int16  )
{
    return (Integer16) ntohs( Int16 );
}

Integer32
NetToHostI32( Integer32  Int32  )
{
    return (Integer32) ntohl( Int32 );
}

Integer64
NetToHostI64( Integer64  Int64  )
{
#ifdef _WIN32
    // Code-snippet from http://www.codeproject.com/Articles/4804/Basic-concepts-on-Endianness
    return (((Integer64)(ntohl((int)((Int64 << 32) >> 32))) << 32) | (unsigned int)ntohl(((int)(Int64 >> 32))));
#else
    return (Integer64) be64toh( Int64 );
#endif
}

UInteger48
BufToHostUI48( Octet Buf[], size_t BufSize )
{
    UInteger48 UInt48;

    if( BufSize != 6 )
        throw cRuntimeError( "Unexpected BufSize" );

    UInt48  = ((((UInteger48) Buf[0]) & 0xFF) << 40) |
              ((((UInteger48) Buf[1]) & 0xFF) << 32) |
              ((((UInteger48) Buf[2]) & 0xFF) << 24) |
              ((((UInteger48) Buf[3]) & 0xFF) << 16) |
              ((((UInteger48) Buf[4]) & 0xFF) <<  8) |
              ((((UInteger48) Buf[5]) & 0xFF) <<  0);

    return UInt48;
}

UInteger16
HostToNetUI16( UInteger16 UInt16 )
{
    return (UInteger16) htons( UInt16 );
}

UInteger32
HostToNetUI32( UInteger32 UInt32 )
{
    return (UInteger32) htonl( UInt32 );
}

Integer16
HostToNetI16( Integer16  Int16  )
{
    return (Integer16) htons( Int16 );
}

Integer32
HostToNetI32( Integer32  Int32  )
{
    return (Integer32) htonl( Int32 );
}

Integer64
HostToNetI64( Integer64  Int64  )
{
#ifdef _WIN32
    return NetToHostI64(Int64);
#else
    return (Integer64) htobe64( Int64 );
#endif
}

void
HostToBufUI48( UInteger48 UInt48, Octet Buf[], size_t BufSize )
{
    if( BufSize != 6 )
        throw cRuntimeError( "Unexpected BufSize" );

    Buf[0] = (Octet) ((UInt48 >> 40) & 0xFF);
    Buf[1] = (Octet) ((UInt48 >> 32) & 0xFF);
    Buf[2] = (Octet) ((UInt48 >> 24) & 0xFF);
    Buf[3] = (Octet) ((UInt48 >> 16) & 0xFF);
    Buf[4] = (Octet) ((UInt48 >>  8) & 0xFF);
    Buf[5] = (Octet) ((UInt48 >>  0) & 0xFF);
}

