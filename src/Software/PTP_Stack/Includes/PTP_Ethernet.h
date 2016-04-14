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

#ifndef LIBPTP_PTP_ETHERNET_H_
#define LIBPTP_PTP_ETHERNET_H_

// ======================================================
// Includes
// ======================================================

#include "PTP_PrimitiveDataTypes.h"
#include "PTP_ByteBuffers.h"

// ======================================================
// Declarations
// ======================================================

//extern  const   int     PTP_ETH_TYPE;

#define PTP_ETH_TYPE        0x88F7      //

#define PTP_ETH_MC_DEFAULT_MAC              "01:1B:19:00:00:00"         // Table F.1
#define PTP_ETH_MC_PDELAY_MAC               "01:80:C2:00:00:0E"         //

#define ETH_HEADER_LEN                  14
#define PTP_HEADER_LEN                  34
#define ETH_PTP_FRAME_OFFSET            ( ETH_HEADER_LEN + PTP_HEADER_LEN           )
#define ETH_PTP_MSG_ANNOUNCE_LEN        ( ETH_HEADER_LEN + PTP_MSG_ANNOUNCE_LEN     )
#define ETH_PTP_MSG_SYNC_LEN            ( ETH_HEADER_LEN + PTP_MSG_SYNC_LEN         )
#define ETH_PTP_MSG_FOLLOW_UP_LEN       ( ETH_HEADER_LEN + PTP_MSG_FOLLOW_UP_LEN    )
#define ETH_PTP_MSG_DEL_REQ_LEN         ( ETH_HEADER_LEN + PTP_MSG_DEL_REQ_LEN      )
#define ETH_PTP_MSG_DEL_RESP_LEN        ( ETH_HEADER_LEN + PTP_MSG_DEL_RESP_LEN     )
#define ETH_PTP_MSG_PDEL_REQ_LEN        ( ETH_HEADER_LEN + PTP_MSG_PDEL_REQ_LEN     )
#define ETH_PTP_MSG_PDEL_RESP_LEN       ( ETH_HEADER_LEN + PTP_MSG_PDEL_RESP_LEN    )
#define ETH_PTP_MSG_PDEL_RESP_FU_LEN    ( ETH_HEADER_LEN + PTP_MSG_PDEL_RESP_FU_LEN )

// ======================================================
// Types
// ======================================================

#endif






