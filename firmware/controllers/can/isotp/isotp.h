#pragma once

#include "fifo_buffer.h"
#include "can_msg_tx.h"

#define CAN_FLOW_STATUS_OK 0
#define CAN_FLOW_STATUS_WAIT_MORE 1
#define CAN_FLOW_STATUS_ABORT 2

enum IsoTpFrameType {
	ISO_TP_FRAME_SINGLE = 0,
	ISO_TP_FRAME_FIRST = 1,
	ISO_TP_FRAME_CONSECUTIVE = 2,
	ISO_TP_FRAME_FLOW_CONTROL = 3,
};

class IsoTpFrameHeader {
public:
	IsoTpFrameType frameType;

	// used for 'single' or 'first' frames
	int numBytes;
	// used for 'consecutive' frames
	int index;
	// used for 'flow control' frames
	int fcFlag;
	int blockSize;
	int separationTime;
};
