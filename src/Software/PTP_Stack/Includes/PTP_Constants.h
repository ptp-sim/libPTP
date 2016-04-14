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

#ifndef LIBPTP_PTP_CONSTANTS_H_
#define LIBPTP_PTP_CONSTANTS_H_

// ======================================================
// Includes
// ======================================================

// ======================================================
// Declarations
// ======================================================

#define PTP_CLOCK_IDENTITY_SIZE             8                           // Length of Clock Identity array
#define PTP_DEFAULT_PHASE_CHANGE_RATE       0x7FFFFFFF                  //
#define PTP_UTC_OFFSET_2006                 33                          // [s]
#define PTP_VERSION_IEEE_1588_2008          2                           // Version number of IEEE 1588-2008

#define PTP_MSG_ANNOUNCE_LEN                64                          // Message lengths as given in 13
#define PTP_MSG_SYNC_LEN                    44                          //
#define PTP_MSG_FOLLOW_UP_LEN               44                          //
#define PTP_MSG_DEL_REQ_LEN                 44                          //
#define PTP_MSG_DEL_RESP_LEN                54                          //
#define PTP_MSG_PDEL_REQ_LEN                54                          //
#define PTP_MSG_PDEL_RESP_LEN               54                          //
#define PTP_MSG_PDEL_RESP_FU_LEN            54                          //


#define PTP_FOREIGN_MASTER_TIME_WINDOW      4                           // [announceInterval], 9.3.2.4.4
#define PTP_FOREIGN_MASTER_THRESHOLD        2                           // [announce messages], 9.3.2.4.4

#define PTP_LOG_MSG_INTVL_DELAY_REQ         0x7F                        // Table 24
#define PTP_LOG_MSG_INTVL_SIGNALING         0x7F                        //
#define PTP_LOG_MSG_INTVL_MANAGEMENT        0x7F                        //
#define PTP_LOG_MSG_INTVL_PDELAY_REQ        0x7F                        //
#define PTP_LOG_MSG_INTVL_PDELAY_RESP       0x7F                        //
#define PTP_LOG_MSG_INTVL_PDELAY_RESP_FU    0x7F                        //

#define PTP_MSG_CTRL_SYNC                   0x00                        // Table 23
#define PTP_MSG_CTRL_DEL_REQ                0x01                        //
#define PTP_MSG_CTRL_FOLLOW_UP              0x02                        //
#define PTP_MSG_CTRL_DEL_RESP               0x03                        //
#define PTP_MSG_CTRL_MGMT                   0x04                        //
#define PTP_MSG_CTRL_OTHERS                 0x05                        //

// ======================================================
// Types
// ======================================================

#endif /* PTP_CONSTANTS_H_ */
