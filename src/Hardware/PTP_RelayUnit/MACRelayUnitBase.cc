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

#include "MACRelayUnitBase.h"

#include <EtherFrame.h>

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

void
MACRelayUnitBase::handleAndDispatchFrame_Prefix(EtherFrame *frame, int &inputport)
{
    inputport = frame->getArrivalGate()->getIndex();

    numProcessedFrames++;

    // update address table
    addressTable->updateTableWithAddress(inputport, frame->getSrc());
}

void
MACRelayUnitBase::handleAndDispatchFrame_Dispatch(EtherFrame *frame, int inputport)
{
    if (frame->getDest().isBroadcast())
    {
        broadcastFrame(frame, inputport);
    }
    else if (frame->getDest().isMulticast())
    {
        multicastFrame(frame, inputport);
    }
    else
    {
        unicastFrame( frame, inputport );
    }
}

void
MACRelayUnitBase::handleAndDispatchFrame(EtherFrame *frame)
{
    int inputport;

    handleAndDispatchFrame_Prefix  ( frame, inputport );
    handleAndDispatchFrame_Dispatch( frame, inputport );
}

void
MACRelayUnitBase::unicastFrame(EtherFrame *frame, int inputport)
{
    // Finds output port of destination address and sends to output port
    // if not found then broadcasts to all other ports instead
    int outputport = addressTable->getPortForAddress(frame->getDest());
    // should not send out the same frame on the same ethernet port
    // (although wireless ports are ok to receive the same message)
    if (inputport == outputport)
    {
        EV << "Output port is same as input port, " << frame->getFullName() <<
              " dest " << frame->getDest() << ", discarding frame\n";
        numDiscardedFrames++;
        delete frame;
        return;
    }

    if (outputport >= 0)
    {
        send(frame, "ifOut", outputport);
    }
    else
    {
        broadcastFrame(frame, inputport);
    }
}

void
MACRelayUnitBase::multicastFrame(EtherFrame *frame, int inputport)
{
    // Emulating multicast via broadcast
    broadcastFrame( frame, inputport );
}

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
MACRelayUnitBase::MACRelayUnitBase()
    : MACRelayUnit()
{
}
