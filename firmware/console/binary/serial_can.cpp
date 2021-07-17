/**
 * @file	serial_can.cpp
 *
 * This code is a bridge between a serial streaming used by TS and a packet-frame CAN-bus, using the ISO-TP protocol.
 * ISO 15765-2, or ISO-TP (Transport Layer), which is an international standard for sending data packets over a CAN-Bus.
 * https://en.wikipedia.org/wiki/ISO_15765-2
 *
 * @date Aug 1, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "os_access.h"

#include "serial_can.h"


static CanStreamerState state;

int CanStreamerState::sendFrame(CANDriver *canp, const IsoTpFrameHeader & header, const uint8_t *data, int num) {
	CANTxFrame txmsg;
	memset(&txmsg, 0, sizeof(txmsg));
	txmsg.IDE = CAN_IDE_STD;
	txmsg.EID = CAN_TX_ID;
	txmsg.RTR = CAN_RTR_DATA;
	txmsg.DLC = 8;	// 8 bytes
	
	// fill the frame data according to the CAN-TP protocol (ISO 15765-2)
	txmsg.data8[0] = (uint8_t)((header.frameType & 0xf) << 4);
	int offset, maxNumBytes;
	switch (header.frameType) {
	case ISO_TP_FRAME_SINGLE:
		offset = 1;
		maxNumBytes = minI(header.numBytes, txmsg.DLC - offset);
		txmsg.data8[0] |= maxNumBytes;
		break;
	case ISO_TP_FRAME_FIRST:
		txmsg.data8[0] |= (header.numBytes >> 8) & 0xf;
		txmsg.data8[1] = (uint8_t)(header.numBytes & 0xff);
		offset = 2;
		maxNumBytes = minI(header.numBytes, txmsg.DLC - offset);
		break;
	case ISO_TP_FRAME_CONSECUTIVE:
		txmsg.data8[0] |= header.index & 0xf;
		offset = 1;
		maxNumBytes = txmsg.DLC - offset;
		break;
	case ISO_TP_FRAME_FLOW_CONTROL:
		txmsg.data8[0] |= header.fcFlag & 0xf;
		txmsg.data8[1] = (uint8_t)(header.blockSize);
		txmsg.data8[2] = (uint8_t)(header.separationTime);
		offset = 3;
		maxNumBytes = 0;	// no data is sent with 'flow control' frame
		break;
	}
	
	int numBytes = minI(maxNumBytes, num);
	// copy the contents
	if (data != nullptr) {
		for (int i = 0; i < numBytes; i++) {
			txmsg.data8[i + offset] = data[i];
		}
	}
	
	// send the frame!
	if (canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, TIME_MS2I(100)) == MSG_OK)
		return numBytes;
	return 0;
}

// returns the number of copied bytes
int CanStreamerState::receiveFrame(CANDriver *canp, CANRxFrame *rxmsg, uint8_t *buf, int num) {
	if (rxmsg == nullptr || rxmsg->DLC < 1)
		return 0;
	int frameType = (rxmsg->data8[0] >> 4) & 0xf;
	int numBytesAvailable, frameIdx;
	uint8_t *srcBuf = rxmsg->data8;
	switch (frameType) {
	case ISO_TP_FRAME_SINGLE:
		numBytesAvailable = rxmsg->data8[0] & 0xf;
		srcBuf = rxmsg->data8 + 1;
		this->waitingForNumBytes = -1;
		break;
	case ISO_TP_FRAME_FIRST:
		this->waitingForNumBytes = ((rxmsg->data8[0] & 0xf) << 8) | rxmsg->data8[1];
		this->waitingForFrameIndex = 1;
		numBytesAvailable = minI(this->waitingForNumBytes, 6);
		srcBuf = rxmsg->data8 + 2;
		break;
	case ISO_TP_FRAME_CONSECUTIVE:
		frameIdx = rxmsg->data8[0] & 0xf;
		if (this->waitingForNumBytes < 0 || this->waitingForFrameIndex != frameIdx) {
			// todo: that's an abnormal situation, and we probably should react?
			return 0;
		}
		numBytesAvailable = minI(this->waitingForNumBytes, 7);
		srcBuf = rxmsg->data8 + 1;
		this->waitingForFrameIndex = (this->waitingForFrameIndex + 1) & 0xf;
		break;
	case ISO_TP_FRAME_FLOW_CONTROL:
		// todo: currently we just ignore the FC frame
		return 0;
	}

#if defined(TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME)
	if (frameType == ISO_TP_FRAME_SINGLE) {
		srcBuf = state.tmpRxBuf;
		// restore the CRC on the whole packet
		uint32_t crc = crc32((void *) (rxmsg->data + 1), numBytesAvailable);
		// we need a separate buffer for crc because srcBuf may not be word-aligned for direct copy
		uint8_t crcBuffer[sizeof(uint32_t)];
		*(uint32_t *) (crcBuffer) = SWAP_UINT32(crc);

		// now set the packet size (including the command byte)
		*(uint16_t *) srcBuf = SWAP_UINT16(numBytesAvailable);
		// copy the data
		if (numBytesAvailable > 0)
			memcpy(srcBuf + 2, rxmsg->data8 + 1, numBytesAvailable);
		// copy the crc
		memcpy(srcBuf + 2 + numBytesAvailable, crcBuffer, sizeof(crcBuffer));
		numBytesAvailable += 1 + sizeof(crcBuffer);	// added command & crc bytes
	}
#endif /* TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME */

	int numBytesToCopy = minI(num, numBytesAvailable);
	if (buf != nullptr) {
		memcpy(buf, srcBuf, numBytesToCopy);
	}
	srcBuf += numBytesToCopy;
	numBytesAvailable -= numBytesToCopy;
	waitingForNumBytes -= numBytesToCopy;
	// if there are some more bytes left, we save them for the next time
	for (int i = 0; i < numBytesAvailable; i++) {
		rxFifoBuf.put(srcBuf[i]);
	}

	// according to the specs, we need to acknowledge the received multi-frame start frame
	if (frameType == ISO_TP_FRAME_FIRST) {
		IsoTpFrameHeader header;
		header.frameType = ISO_TP_FRAME_FLOW_CONTROL;
		header.fcFlag = 0;			// = "continue to send"
		header.blockSize = 0;		// = the remaining "frames" to be sent without flow control or delay
		header.separationTime = 0;	// = wait 0 milliseconds, send immediately
		sendFrame(canp, header, nullptr, 0);
	}

	return numBytesToCopy;
}

int CanStreamerState::sendDataTimeout(CANDriver *canp, const uint8_t *txbuf, int numBytes, sysinterval_t timeout) {

	int offset = 0;
	msg_t ret;
	// 1 frame
	if (numBytes <= 7) {
		IsoTpFrameHeader header;
		header.frameType = ISO_TP_FRAME_SINGLE;
		header.numBytes = numBytes;
		return state.sendFrame(canp, header, txbuf, numBytes);
	}

	// multiple frames

	// send the first header frame
	IsoTpFrameHeader header;
	header.frameType = ISO_TP_FRAME_FIRST;
	header.numBytes = numBytes;
	int numSent = state.sendFrame(canp, header, txbuf + offset, numBytes);
	offset += numSent;
	numBytes -= numSent;
	int totalNumSent = numSent;

	// get a flow control frame
	CANRxFrame rxmsg;
	if (canReceive(&CAND1, CAN_ANY_MAILBOX, &rxmsg, timeout) == MSG_OK) {
		state.receiveFrame(canp, &rxmsg, nullptr, 0);
	}

	// send the rest of the data
	int idx = 1;
	while (numBytes > 0) {
		int len = minI(numBytes, 7);
		// send the consecutive frames
		IsoTpFrameHeader header;
		header.frameType = ISO_TP_FRAME_CONSECUTIVE;
		header.index = ((idx++) & 0x0f);
		header.numBytes = numBytes;
		int numSent = state.sendFrame(canp, header, txbuf + offset, numBytes);
		if (numSent < 1)
			break;
		totalNumSent += numSent;
		offset += numSent;
		numBytes -= numSent;
	}
	return totalNumSent;
}

int CanStreamerState::getDataFromFifo(uint8_t *rxbuf, size_t &numBytes) {
	if (rxFifoBuf.isEmpty())
		return 0;
	int numReadFromFifo = minI(numBytes, rxFifoBuf.getCount());
	// move bytes from the FIFO buffer
	int i;
	for (i = 0; !rxFifoBuf.isEmpty() && i < numReadFromFifo; i++) {
		rxbuf[i] = rxFifoBuf.get();
		numBytes--;
	}
	return i;
}

void canInit(CANDriver *canp) {
	chEvtRegister(&CAND1.rxfull_event, &state.el, 0);
}

msg_t canAddToTxStreamTimeout(CANDriver *canp, size_t *np,
                        const uint8_t *txbuf, sysinterval_t timeout) {
	int numBytes = *np;
	int offset = 0;
	int minNumBytesRequiredToSend = 7 - state.txFifoBuf.getCount();
	while (numBytes >= minNumBytesRequiredToSend) {
		state.txFifoBuf.put(txbuf + offset, minNumBytesRequiredToSend);
		int numSent = state.sendDataTimeout(canp, (const uint8_t *)state.txFifoBuf.elements, state.txFifoBuf.getCount(), timeout);
		if (numSent < 1)
			break;
		state.txFifoBuf.clear();
		offset += numSent;
		numBytes -= numSent;
		minNumBytesRequiredToSend = 7;
	}
	
	// now we put the rest on hold
	state.txFifoBuf.put(txbuf + offset, numBytes);

	return MSG_OK;
}

msg_t canFlushTxStream(CANDriver *canp, sysinterval_t timeout) {
	int numSent = state.sendDataTimeout(canp, (const uint8_t *)state.txFifoBuf.elements, state.txFifoBuf.getCount(), timeout);
	state.txFifoBuf.clear();
	
	return MSG_OK;
}

msg_t canStreamReceiveTimeout(CANDriver *canp, size_t *np,
                           uint8_t *rxbuf, sysinterval_t timeout) {
	int i = 0;
	size_t numBytes = *np;
	
	// first, fill the data from the stored buffer (saved from the previous CAN frame)
	i = state.getDataFromFifo(rxbuf, numBytes);

	// if even more data is needed, then we receive more CAN frames
	while (numBytes > 0) {
		if (chEvtWaitAnyTimeout(ALL_EVENTS, timeout) == 0)
			return MSG_TIMEOUT;
		CANRxFrame rxmsg;
		if (canReceive(&CAND1, CAN_ANY_MAILBOX, &rxmsg, TIME_IMMEDIATE) == MSG_OK) {
			int numReceived = state.receiveFrame(canp, &rxmsg, rxbuf + i, numBytes);
			if (numReceived < 1)
				break;
			numBytes -= numReceived;
		}
	}
	//*np -= numBytes;
	return MSG_OK;
}

