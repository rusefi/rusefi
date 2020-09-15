/**
 * @file	serial_can.h
 *
 * @date Aug 1, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "fifo_buffer.h"

#define CAN_TX_ID 0x102

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

class CanStreamerState {
public:
	fifo_buffer<uint8_t, 64> rxFifoBuf;
	fifo_buffer<uint8_t, 64> txFifoBuf;

#if defined(TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME)
	// used to restore the original packet with CRC
    uint8_t tmpRxBuf[13];
#endif

	// used for multi-frame ISO-TP packets
	int waitingForNumBytes = 0;
	int waitingForFrameIndex = 0;

	event_listener_t el;
	
public:
	int sendFrame(CANDriver *canp, const IsoTpFrameHeader & header, const uint8_t *data, int num);
	int receiveFrame(CANDriver *canp, CANRxFrame *rxmsg, uint8_t *buf, int num);
	int getDataFromFifo(uint8_t *rxbuf, size_t &numBytes);
	// returns the number of bytes sent
	int sendDataTimeout(CANDriver *canp, const uint8_t *txbuf, int numBytes, sysinterval_t timeout);
};

void canInit(CANDriver *canp);

// we don't have canStreamSendTimeout() because we need to "bufferize" the stream and send it in fixed-length packets
msg_t canAddToTxStreamTimeout(CANDriver *canp, size_t *np, const uint8_t *txbuf, sysinterval_t timeout);
msg_t canFlushTxStream(CANDriver *canp, sysinterval_t timeout);

msg_t canStreamReceiveTimeout(CANDriver *canp, size_t *np, uint8_t *rxbuf, sysinterval_t timeout);

