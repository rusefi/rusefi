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

#include "pch.h"
#include "os_access.h"

#if EFI_UNIT_TEST
#define PRINT printf
#define PRINT_EOL "\n"
#else
#define PRINT efiPrintf
#define PRINT_EOL ""
#endif

// todo: this file is asking to improve conditional compilation. unit_tests and cypress/kinetis are both special cases
#if HAL_USE_CAN || EFI_UNIT_TEST
#include "serial_can.h"
#include "can.h"
#include "can_msg_tx.h"
#endif // HAL_USE_CAN || EFI_UNIT_TEST


#if HAL_USE_CAN
static CanStreamer streamer;
static CanStreamerState state(&streamer);
static CanTsListener listener;
#endif // HAL_USE_CAN

#if HAL_USE_CAN || EFI_UNIT_TEST

int CanStreamerState::sendFrame(const IsoTpFrameHeader & header, const uint8_t *data, int num, can_sysinterval_t timeout) {
	int dlc = 8; // standard 8 bytes
	CanTxMessage txmsg(CanCategory::SERIAL, CAN_ECU_SERIAL_TX_ID, dlc, false);
	
	// fill the frame data according to the CAN-TP protocol (ISO 15765-2)
	txmsg[0] = (uint8_t)((header.frameType & 0xf) << 4);
	int offset, maxNumBytes;
	switch (header.frameType) {
	case ISO_TP_FRAME_SINGLE:
		offset = 1;
		maxNumBytes = minI(header.numBytes, dlc - offset);
		txmsg[0] |= maxNumBytes;
		break;
	case ISO_TP_FRAME_FIRST:
		txmsg[0] |= (header.numBytes >> 8) & 0xf;
		txmsg[1] = (uint8_t)(header.numBytes & 0xff);
		offset = 2;
		maxNumBytes = minI(header.numBytes, dlc - offset);
		break;
	case ISO_TP_FRAME_CONSECUTIVE:
		txmsg[0] |= header.index & 0xf;
		offset = 1;
		// todo: is it correct?
		maxNumBytes = dlc - offset;
		break;
	case ISO_TP_FRAME_FLOW_CONTROL:
		txmsg[0] |= header.fcFlag & 0xf;
		txmsg[1] = (uint8_t)(header.blockSize);
		txmsg[2] = (uint8_t)(header.separationTime);
		offset = 3;
		maxNumBytes = 0;	// no data is sent with 'flow control' frame
		break;
	default:
		// bad frame type
		return 0;
	}
	
	int numBytes = minI(maxNumBytes, num);
	// copy the contents
	if (data != nullptr) {
		for (int i = 0; i < numBytes; i++) {
			txmsg[i + offset] = data[i];
		}
	}

	// send the frame!
	if (streamer->transmit(CAN_ANY_MAILBOX, &txmsg, timeout) == CAN_MSG_OK)
		return numBytes;
	return 0;
}

// returns the number of copied bytes
int CanStreamerState::receiveFrame(CANRxFrame *rxmsg, uint8_t *buf, int num, can_sysinterval_t timeout) {
	if (rxmsg == nullptr || rxmsg->DLC < 1)
		return 0;
	engine->pauseCANdueToSerial = true;
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
	default:
		// bad frame type
		return 0;
	}

#if defined(TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME)
	if (frameType == ISO_TP_FRAME_SINGLE) {
		// restore the CRC on the whole packet
		uint32_t crc = crc32((void *) srcBuf, numBytesAvailable);
		// we need a separate buffer for crc because srcBuf may not be word-aligned for direct copy
		uint8_t crcBuffer[sizeof(uint32_t)];
		*(uint32_t *) (crcBuffer) = SWAP_UINT32(crc);

		// now set the packet size
		*(uint16_t *) tmpRxBuf = SWAP_UINT16(numBytesAvailable);
		// copy the data
		if (numBytesAvailable > 0)
			memcpy(tmpRxBuf + sizeof(uint16_t), srcBuf, numBytesAvailable);
		// copy the crc to the end
		memcpy(tmpRxBuf + sizeof(uint16_t) + numBytesAvailable, crcBuffer, sizeof(crcBuffer));
		
		// use the reconstructed tmp buffer as a source buffer
		srcBuf = tmpRxBuf;
		// we added the 16-bit size & 32-bit crc bytes
		numBytesAvailable += sizeof(uint16_t) + sizeof(crcBuffer);
	}
#endif /* TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME */

	int numBytesToCopy = minI(num, numBytesAvailable);
	if (buf != nullptr) {
		memcpy(buf, srcBuf, numBytesToCopy);
	}
	srcBuf += numBytesToCopy;
	waitingForNumBytes -= numBytesAvailable;
	numBytesAvailable -= numBytesToCopy;
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
		sendFrame(header, nullptr, 0, timeout);
	}

	return numBytesToCopy;
}

int CanStreamerState::sendDataTimeout(const uint8_t *txbuf, int numBytes, can_sysinterval_t timeout) {
	int offset = 0;

	if (engineConfiguration->verboseIsoTp) {
		PRINT("*** INFO: sendDataTimeout %d" PRINT_EOL, numBytes);
	}

	if (numBytes < 1)
		return 0;

	// 1 frame
	if (numBytes <= 7) {
		IsoTpFrameHeader header;
		header.frameType = ISO_TP_FRAME_SINGLE;
		header.numBytes = numBytes;
		return sendFrame(header, txbuf, numBytes, timeout);
	}

	// multiple frames

	// send the first header frame (FF)
	IsoTpFrameHeader header;
	header.frameType = ISO_TP_FRAME_FIRST;
	header.numBytes = numBytes;
	int numSent = sendFrame(header, txbuf + offset, numBytes, timeout);
	offset += numSent;
	numBytes -= numSent;
	int totalNumSent = numSent;

	// get a flow control (FC) frame
#if !EFI_UNIT_TEST // todo: add FC to unit-tests?
	CANRxFrame rxmsg;
	for (int numFcReceived = 0; ; numFcReceived++) {
		if (streamer->receive(CAN_ANY_MAILBOX, &rxmsg, timeout) != CAN_MSG_OK) {
#ifdef SERIAL_CAN_DEBUG
			PRINT("*** ERROR: CAN Flow Control frame not received" PRINT_EOL);
#endif /* SERIAL_CAN_DEBUG */
			//warning(CUSTOM_ERR_CAN_COMMUNICATION, "CAN Flow Control frame not received");
			return 0;
		}
		receiveFrame(&rxmsg, nullptr, 0, timeout);
		int flowStatus = rxmsg.data8[0] & 0xf;
		// if something is not ok
		if (flowStatus != CAN_FLOW_STATUS_OK) {
			// if the receiver is not ready yet and asks to wait for the next FC frame (give it 3 attempts)
			if (flowStatus == CAN_FLOW_STATUS_WAIT_MORE && numFcReceived < 3) {
				continue;
			}
#ifdef SERIAL_CAN_DEBUG
			efiPrintf("*** ERROR: CAN Flow Control mode not supported");
#endif /* SERIAL_CAN_DEBUG */
			//warning(CUSTOM_ERR_CAN_COMMUNICATION, "CAN Flow Control mode not supported");
			return 0;
		}
		int blockSize = rxmsg.data8[1];
		int minSeparationTime = rxmsg.data8[2];
		if (blockSize != 0 || minSeparationTime != 0) {
			// todo: process other Flow Control fields (see ISO 15765-2)
#ifdef SERIAL_CAN_DEBUG
			efiPrintf("*** ERROR: CAN Flow Control fields not supported");
#endif /* SERIAL_CAN_DEBUG */
			//warning(CUSTOM_ERR_CAN_COMMUNICATION, "CAN Flow Control fields not supported");
		}
		break;
	}
#endif /* EFI_UNIT_TEST */

	// send the rest of the data
	int idx = 1;
	while (numBytes > 0) {
		int len = minI(numBytes, 7);
		// send the consecutive frames
		IsoTpFrameHeader header;
		header.frameType = ISO_TP_FRAME_CONSECUTIVE;
		header.index = ((idx++) & 0x0f);
		header.numBytes = len;
		int numSent = sendFrame(header, txbuf + offset, len, timeout);
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

can_msg_t CanStreamerState::streamAddToTxTimeout(size_t *np, const uint8_t *txbuf, can_sysinterval_t timeout) {
	int numBytes = *np;
	int offset = 0;

	if (engineConfiguration->verboseIsoTp) {
		PRINT("*** INFO: streamAddToTxTimeout adding %d, in buffer %d" PRINT_EOL, numBytes, txFifoBuf.getCount());
	}

	// we send here only if the TX FIFO buffer is getting overflowed
	while (numBytes >= txFifoBuf.getSize() - txFifoBuf.getCount()) {
		int numBytesToAdd = txFifoBuf.getSize() - txFifoBuf.getCount();
		txFifoBuf.put(txbuf + offset, numBytesToAdd);
		int numSent = sendDataTimeout((const uint8_t *)txFifoBuf.getElements(), txFifoBuf.getCount(), timeout);

		if (engineConfiguration->verboseIsoTp) {
			PRINT("*** INFO: streamAddToTxTimeout numBytesToAdd %d / numSent %d / numBytes %d" PRINT_EOL, numBytesToAdd, numSent, numBytes);
		}

		if (numSent < 1)
			break;
		txFifoBuf.clear();
		offset += numBytesToAdd;
		numBytes -= numBytesToAdd;
	}
	
	if (engineConfiguration->verboseIsoTp) {
		PRINT("*** INFO: streamAddToTxTimeout remaining goes to buffer %d" PRINT_EOL, numBytes);
	}

	// now we put the rest on hold
	txFifoBuf.put(txbuf + offset, numBytes);


	if (engineConfiguration->verboseIsoTp) {
		PRINT("*** INFO: in buffer %d" PRINT_EOL, txFifoBuf.getCount());
	}

	return CAN_MSG_OK;
}

can_msg_t CanStreamerState::streamFlushTx(can_sysinterval_t timeout) {
	int numSent = sendDataTimeout((const uint8_t *)txFifoBuf.getElements(), txFifoBuf.getCount(), timeout);
	if (numSent != txFifoBuf.getCount()) {
		//warning(CUSTOM_ERR_CAN_COMMUNICATION, "CAN sendDataTimeout() problems");
	}
	txFifoBuf.clear();
	
	return CAN_MSG_OK;
}

can_msg_t CanStreamerState::streamReceiveTimeout(size_t *np, uint8_t *rxbuf, can_sysinterval_t timeout) {
	int i = 0;
	size_t numBytes = *np;

	// first, fill the data from the stored buffer (saved from the previous CAN frame)
	i = getDataFromFifo(rxbuf, numBytes);

	// if even more data is needed, then we receive more CAN frames
	while (numBytes > 0) {
		CANRxFrame rxmsg;
		if (streamer->receive(CAN_ANY_MAILBOX, &rxmsg, timeout) == CAN_MSG_OK) {
			int numReceived = receiveFrame(&rxmsg, rxbuf + i, numBytes, timeout);

			if (numReceived < 1)
				break;
			numBytes -= numReceived;
			i += numReceived;
		} else {
			break;
		}
	}
	*np -= numBytes;

#ifdef SERIAL_CAN_DEBUG
	efiPrintf("* ret: %d %d (%d)", i, *np, numBytes);
	for (int j = 0; j < i; j++) {
		efiPrintf("* [%d]: %02x", j, rxbuf[j]);
	}
#endif /* SERIAL_CAN_DEBUG */

	return CAN_MSG_OK;
}
static int isoTpPacketCounter = 0;

/**
 * incoming data main entry point
 */
void CanTsListener::decodeFrame(const CANRxFrame& frame, efitick_t /*nowNt*/) {
	// CAN ID filtering happens in base class, by the time we are here we know it's the CAN_ECU_SERIAL_RX_ID packet
	// todo: what if the FIFO is full?
	CanRxMessage msg(frame);
	if (engineConfiguration->verboseIsoTp) {
		PRINT("*** INFO: CanTsListener decodeFrame %d" PRINT_EOL, isoTpPacketCounter++);
	}
	if (!rxFifo.put(msg)) {
		warning(CUSTOM_ERR_CAN_COMMUNICATION, "CAN sendDataTimeout() problems");
	}
}

#if HAL_USE_CAN

void CanStreamer::init() {
	registerCanListener(listener);
}

can_msg_t CanStreamer::transmit(canmbx_t /*mailbox*/, const CanTxMessage */*ctfp*/, can_sysinterval_t /*timeout*/) {
	// we do nothing here - see CanTxMessage::~CanTxMessage()
	return CAN_MSG_OK;
}

can_msg_t CanStreamer::receive(canmbx_t /*mailbox*/, CANRxFrame *crfp, can_sysinterval_t timeout) {
	// see CanTsListener and processCanRxMessage()
	CanRxMessage msg;
	if (listener.get(msg, timeout)) {
		*crfp = msg.frame;
		return CAN_MSG_OK;
	}
	return CAN_MSG_TIMEOUT;
}

void canStreamInit(void) {
	streamer.init();
}

msg_t canStreamAddToTxTimeout(size_t *np, const uint8_t *txbuf, sysinterval_t timeout) {
	return state.streamAddToTxTimeout(np, txbuf, timeout);
}

msg_t canStreamFlushTx(sysinterval_t timeout) {
	return state.streamFlushTx(timeout);
}

msg_t canStreamReceiveTimeout(size_t *np, uint8_t *rxbuf, sysinterval_t timeout) {
	return state.streamReceiveTimeout(np, rxbuf, timeout);
}

#endif /* HAL_USE_CAN */



#endif // HAL_USE_CAN || EFI_UNIT_TEST
