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

#include "PtpFrameGen.h"

#include "PTP_Parser.h"

// ======================================================
// Definitions
// ======================================================

#define     MAX_PARSE_STRING_LEN    50

#define     ArrayLen(A)             (sizeof(A)/sizeof(A[0]))

// ======================================================
// Types
// ======================================================

typedef enum
{
    TEST_SEND_PTP_ANNOUNCE,
    TEST_SEND_PTP_SYNC,
    TEST_SEND_PTP_FOLLOW_UP,
    TEST_SEND_PTP_DELAY_REQ,
    TEST_SEND_PTP_PDELAY_REQ,
    TEST_SEND_ETH_TINY,
    TEST_SEND_ETH_SMALL,
    TEST_SEND_ETH_LARGE,
    TEST_SEND_ETH_HUGE,
}
tTestID;

struct tTestIdParse
{
    tTestID     Value;
    char        String[MAX_PARSE_STRING_LEN];
};

// ======================================================
// Constants
// ======================================================

const tTestIdParse  TestIdPareArray[]   =
{
    { TEST_SEND_PTP_ANNOUNCE,     "TEST_SEND_PTP_ANNOUNCE"    },
    { TEST_SEND_PTP_SYNC,         "TEST_SEND_PTP_SYNC"        },
    { TEST_SEND_PTP_FOLLOW_UP,    "TEST_SEND_PTP_FOLLOW_UP"   },
    { TEST_SEND_PTP_DELAY_REQ,    "TEST_SEND_PTP_DELAY_REQ"   },
    { TEST_SEND_PTP_PDELAY_REQ,   "TEST_SEND_PTP_PDELAY_REQ"  },
    { TEST_SEND_ETH_TINY,         "TEST_SEND_ETH_TINY"        },
    { TEST_SEND_ETH_SMALL,        "TEST_SEND_ETH_SMALL"       },
    { TEST_SEND_ETH_LARGE,        "TEST_SEND_ETH_LARGE"       },
    { TEST_SEND_ETH_HUGE,         "TEST_SEND_ETH_HUGE"        },
};

// ======================================================
// Variables
// ======================================================

// ======================================================
// Declarations
// ======================================================

Define_Module(PtpFrameGen);

// ======================================================
// Definitions
// ======================================================

// ------------------------------------------------------
// Template search function
// ------------------------------------------------------

tTestID ParseTestID( const tTestIdParse *pParse, const size_t size, const char *Str)
{
    for(size_t i = 0; i < size; i++)
    {
        if( 0 == strcmp(pParse[i].String, Str))
        {
            return  pParse[i].Value;
        }
    }

    // No match could be found --> Throw exception
    throw cRuntimeError("Parsing exception: Failed to parse value '%s'", Str );
}

// ------------------------------------------------------
// Fill in basic information
// ------------------------------------------------------
void
PtpFrameGen::FillInBasicInfo( PTPv2_Frame *pPtp )
{
    pPtp->setTransportSpecific(0);
    pPtp->setReserved_0(0);
    pPtp->setVersionPTP( PTP_VERSION_IEEE_1588_2008 );
    pPtp->setDomainNumber( PrimaryDomain );
    pPtp->setReserved_1(0);

    cPtpHeaderFlags&    FlagField   = pPtp->getFlagField();

    FlagField.alternateMasterFlag   = false;
    FlagField.unicastFlag           = false;
    FlagField.ptpProfileSpecific_1  = false;
    FlagField.ptpProfileSpecific_2  = false;
    FlagField.reserved              = false;
    FlagField.leap59                = false;
    FlagField.leap61                = false;
    FlagField.currentUtcOffsetValid = false;
    FlagField.ptpTimescale          = false;
    FlagField.timeTraceable         = true;
    FlagField.frequencyTraceable    = true;

    pPtp->setCorrectionField( cTimeInterval() );   // Table 21

    for( unsigned int i = 0; i < pPtp->getReserved_2ArraySize(); i++ )
    {
        pPtp->setReserved_2( i, 0);
    }

    pPtp->setSourcePortIdentity( PortIdentity );
}

// ------------------------------------------------------
// Fill in Announce specific information
// ------------------------------------------------------
void
PtpFrameGen::FillInAnnounceFrame( PTPv2_AnnounceFrame *pPtp )
{
    pPtp->setOriginTimestamp( cTimeStamp(0,0) );

    pPtp->setCurrentUtcOffset( 0 );
    pPtp->setGrandmasterPriority1( Priority1 );
    pPtp->setGrandmasterClockQuality( ClockQuality );
    pPtp->setGrandmasterPriority2( Priority2 );
    pPtp->setGrandmasterIdentity( PortIdentity.ClockIdentity() );
    pPtp->setStepsRemoved( 0 );
    pPtp->setTimeSource( TIME_SRC_ATOMIC_CLOCK );

    // Generic PTP header, 13.3.2
    pPtp->setMessageType(PTP_TYPE_ANNOUNCE);
    pPtp->setMessageLength(PTP_MSG_ANNOUNCE_LEN);
    pPtp->getFlagField().twoStepFlag = false;
    pPtp->setSequenceId( AnnSequId++ );
    pPtp->setControlField( PTP_MSG_CTRL_OTHERS );
    pPtp->setLogMessageInterval( 0 );
    pPtp->setByteLength( PTP_MSG_ANNOUNCE_LEN );
}

// ------------------------------------------------------
// Fill in Sync specific information
// ------------------------------------------------------
void
PtpFrameGen::FillInSyncFrame( PTPv2_SyncFrame *pPtp )
{
    if(PTP_TwoStepFlag)
    {
        pPtp->setOriginTimestamp( cTimeStamp(0,0) );
    }
    else
    {
        pPtp->setOriginTimestamp( simTime() );
    }

    pPtp->setMessageType(PTP_TYPE_SYNC);
    pPtp->setMessageLength(PTP_MSG_SYNC_LEN);
    pPtp->getFlagField().twoStepFlag = PTP_TwoStepFlag;
    pPtp->setSequenceId( SyncSequId++ );
    pPtp->setControlField( PTP_MSG_CTRL_SYNC );
    pPtp->setLogMessageInterval( 0 );
    pPtp->setByteLength( PTP_MSG_SYNC_LEN );
}

void
PtpFrameGen::FillInSyncFUFrame( PTPv2_Follow_UpFrame *pPtp )
{
    pPtp->setPreciseOriginTimestamp( cTimeStamp(0,0) );

    // Generic PTP header, 13.3.2
    pPtp->setMessageType(PTP_TYPE_FOLLOW_UP);
    pPtp->setMessageLength(PTP_MSG_FOLLOW_UP_LEN);

    pPtp->setFlagField( pSyncDup->getFlagField() );
    pPtp->getFlagField().twoStepFlag = false;

    pPtp->setSequenceId( pSyncDup->getSequenceId() );
    pPtp->setControlField( PTP_MSG_CTRL_FOLLOW_UP );
    pPtp->setLogMessageInterval( pSyncDup->getLogMessageInterval() );

    pPtp->setByteLength( PTP_MSG_FOLLOW_UP_LEN );
}

// ------------------------------------------------------
// Fill in DelayReq specific information
// ------------------------------------------------------
void
PtpFrameGen::FillInDelayReqFrame( PTPv2_Delay_ReqFrame *pPtp)
{
    pPtp->setOriginTimestamp( cTimeStamp(0,0) );

    pPtp->setMessageType(PTP_TYPE_DELAY_REQ);
    pPtp->setMessageLength(PTP_MSG_DEL_REQ_LEN);
    pPtp->getFlagField().twoStepFlag = PTP_TwoStepFlag;
    pPtp->setSequenceId( DelaySequId++ );
    pPtp->setControlField( PTP_MSG_CTRL_DEL_REQ );
    pPtp->setLogMessageInterval( 0x7F );
    pPtp->setByteLength( PTP_MSG_DEL_REQ_LEN );
}

void
PtpFrameGen::FillInPDelayReqFrame( PTPv2_PDelay_ReqFrame *pPtp)
{
    pPtp->setOriginTimestamp( cTimeStamp(0,0) );

    pPtp->setMessageType(PTP_TYPE_PDELAY_REQ);
    pPtp->setMessageLength(PTP_MSG_PDEL_REQ_LEN);

    pPtp->getFlagField().twoStepFlag = false;

    pPtp->setSequenceId( PDelaySequId++ );
    pPtp->setControlField( PTP_MSG_CTRL_OTHERS );
    pPtp->setLogMessageInterval( 0x7F );
    pPtp->setByteLength( PTP_MSG_PDEL_REQ_LEN );
}

// ------------------------------------------------------
// Create frames
// ------------------------------------------------------

PTPv2_AnnounceFrame *
PtpFrameGen::CreateAnnounceFrame()
{
    PTPv2_AnnounceFrame *pPtp   = new PTPv2_AnnounceFrame;

    FillInBasicInfo( pPtp );
    FillInAnnounceFrame( pPtp );

    return pPtp;
}

PTPv2_SyncFrame *
PtpFrameGen::CreateSyncFrame()
{
    PTPv2_SyncFrame *pPtp  = new PTPv2_SyncFrame;

    FillInBasicInfo( pPtp );
    FillInSyncFrame( pPtp );

    if( pSyncDup != nullptr )
    {
        delete pSyncDup;
    }
    pSyncDup    = pPtp->dup();

    return pPtp;
}

PTPv2_Follow_UpFrame *
PtpFrameGen::CreateFollowUpFrame()
{
    if( pSyncDup == nullptr )
    {
        throw cRuntimeError( "Can't create FollowUp frame before Sync frame" );
    }
    if( PTP_TwoStepFlag == false )
    {
        throw cRuntimeError( "Can't create FollowUp if TwoStepFlag is set to false" );
    }

    PTPv2_Follow_UpFrame *pPtp  = new PTPv2_Follow_UpFrame;

    FillInBasicInfo( pPtp );
    FillInSyncFUFrame( pPtp );

    return pPtp;
}

PTPv2_Delay_ReqFrame *
PtpFrameGen::CreateDelayReqFrame()
{
    PTPv2_Delay_ReqFrame *pPtp  = new PTPv2_Delay_ReqFrame;

    FillInBasicInfo( pPtp );
    FillInDelayReqFrame( pPtp );

    return pPtp;
}

PTPv2_PDelay_ReqFrame *
PtpFrameGen::CreatePDelayReqFrame()
{
    PTPv2_PDelay_ReqFrame *pPtp  = new PTPv2_PDelay_ReqFrame;

    FillInBasicInfo( pPtp );
    FillInPDelayReqFrame( pPtp );

    return pPtp;
}

PTPv2_SyncFrame *
PtpFrameGen::CreateEthFrame( size_t ByteLength )
{
    PTPv2_SyncFrame *pPtp  = new PTPv2_SyncFrame;

    FillInBasicInfo( pPtp );
    FillInSyncFrame( pPtp );

    pPtp->setByteLength( ByteLength );

    return pPtp;
}

// ------------------------------------------------------
// Parse parameters
// ------------------------------------------------------
void
PtpFrameGen::ParseParameters()
{
    PortIdentity.ClockIdentity()    = par("ClockID").stringValue();
    PortIdentity.SetPortNumber( par( "PortNumber" ).longValue() );

    PTP_TwoStepFlag         = par( "PTP_TwoStepFlag" ).boolValue();
    Priority1               = par( "Priority1" ).longValue();
    Priority2               = par( "Priority2" ).longValue();
    PrimaryDomain           = cPTP_Parser::ParseDomainNumber  ( par( "DomainNumber" ).stringValue() );

    ClockQuality.SetOffsetScaledLogVar( par( "OffsetScaledLogVariance" ).longValue() );
    ClockQuality.SetClockAccuracy( cPTP_Parser::ParseClockAccuracy ( par( "ClockAccuracy" ).stringValue() ) );
    ClockQuality.SetClockClass( cPTP_Parser::ParseClockClass( par( "ClockClass" ).stringValue() ) );

    const char *vstr = par("TestSchedule").stringValue(); // e.g. "aa bb cc";
    std::vector<std::string> v = cStringTokenizer(vstr, " ,\t" ).asVector();

    for( std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
    {
        simtime_t   t;
        tTestID     TestID;

        t   = simtime_t(::strtod(i->c_str(), nullptr ) );

        i++;

        TestID = ParseTestID( TestIdPareArray, ArrayLen(TestIdPareArray), i->c_str() );

        EV << "Scheduling Test with ID " << TestID << " at time " << t << endl;

        cMessage    *pMsg   = new cMessage;

        pMsg->setKind( TestID );

        scheduleAt( t, pMsg );
    }
}

// ------------------------------------------------------
// Initialize
// ------------------------------------------------------
void
PtpFrameGen::initialize()
{
    ParseParameters();

    // Get resources
    AnnSequId       = 0;
    SyncSequId      = 0;
    DelaySequId     = 0;
    PDelaySequId    = 0;

    pSyncDup        = nullptr;
}

// ------------------------------------------------------
// Handle message
// ------------------------------------------------------
void
PtpFrameGen::handleMessage(cMessage *msg)
{
    if( msg->isSelfMessage() )
    {
        EV << "Starting test of type " << msg->getKind() << endl;

        switch( msg->getKind() )
        {
            case TEST_SEND_PTP_ANNOUNCE:        send( CreateAnnounceFrame(),    "PtpOut"   );  break;
            case TEST_SEND_PTP_SYNC:            send( CreateSyncFrame(),        "PtpOut"   );  break;
            case TEST_SEND_PTP_FOLLOW_UP:       send( CreateFollowUpFrame(),    "PtpOut"   );  break;
            case TEST_SEND_PTP_DELAY_REQ:       send( CreateDelayReqFrame(),    "PtpOut"   );  break;
            case TEST_SEND_PTP_PDELAY_REQ:      send( CreatePDelayReqFrame(),   "PtpOut"   );  break;

            case TEST_SEND_ETH_TINY:            send( CreateEthFrame( 40   ),   "PtpOut"   );  break;
            case TEST_SEND_ETH_SMALL:           send( CreateEthFrame( 64   ),   "PtpOut"   );  break;
            case TEST_SEND_ETH_LARGE:           send( CreateEthFrame( 1200 ),   "PtpOut"   );  break;
            case TEST_SEND_ETH_HUGE:            send( CreateEthFrame( 2000 ),   "PtpOut"   );  break;
        }
    }

    delete msg;
}

// ------------------------------------------------------
// Constructor/Destructor
// ------------------------------------------------------
PtpFrameGen::~PtpFrameGen()
{
    if( pSyncDup != nullptr )
    {
        delete pSyncDup;
    }
}
