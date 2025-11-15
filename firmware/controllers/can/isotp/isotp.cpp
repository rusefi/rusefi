/**
 *
 * https://en.wikipedia.org/wiki/ISO_15765-2
 */

#include "pch.h"
#include "isotp.h"

#if HAL_USE_CAN || EFI_UNIT_TEST

int CanStreamerState::sendFrame(const IsoTpFrameHeader & header, const uint8_t *data, int num, can_sysinterval_t timeout) {
	int dlc = 8; // standard 8 bytes
	CanTxMessage txmsg(CanCategory::SERIAL, txFrameId, dlc, busIndex, IS_EXT_RANGE_ID(txFrameId));

	// fill the frame data according to the CAN-TP protocol (ISO 15765-2)
	txmsg[isoHeaderByteIndex] = (uint8_t)((header.frameType & 0xf) << 4);
	int offset, maxNumBytes;
	switch (header.frameType) {
	case ISO_TP_FRAME_SINGLE:
		offset = isoHeaderByteIndex + 1;
		maxNumBytes = minI(header.numBytes, dlc - offset);
		txmsg[isoHeaderByteIndex] |= maxNumBytes;
		break;
	case ISO_TP_FRAME_FIRST:
		txmsg[isoHeaderByteIndex] |= (header.numBytes >> 8) & 0xf;
		txmsg[isoHeaderByteIndex + 1] = (uint8_t)(header.numBytes & 0xff);
		offset = isoHeaderByteIndex + 2;
		maxNumBytes = minI(header.numBytes, dlc - offset);
		break;
	case ISO_TP_FRAME_CONSECUTIVE:
		txmsg[isoHeaderByteIndex] |= header.index & 0xf;
		offset = isoHeaderByteIndex + 1;
		// todo: is it correct?
		maxNumBytes = dlc - offset;
		break;
	case ISO_TP_FRAME_FLOW_CONTROL:
		txmsg[isoHeaderByteIndex] |= header.fcFlag & 0xf;
		txmsg[isoHeaderByteIndex + 1] = (uint8_t)(header.blockSize);
		txmsg[isoHeaderByteIndex + 2] = (uint8_t)(header.separationTime);
		offset = isoHeaderByteIndex + 3;
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
	if (txTransport->transmit(&txmsg, timeout) == CAN_MSG_OK)
		return numBytes;
	return 0;
}

// returns the number of copied bytes
int CanStreamerState::receiveFrame(const CANRxFrame *rxmsg, uint8_t *destinationBuff, int availableAtBuffer, can_sysinterval_t timeout) {
	if (rxmsg == nullptr || rxmsg->DLC < 1)
		return 0;
	engine->pauseCANdueToSerial = true;
	int frameType = (rxmsg->data8[isoHeaderByteIndex] >> 4) & 0xf;
	if (engineConfiguration->debugIsoTp) {
	  efiPrintf("receiveFrame frameType=%d", frameType);
	}
	int numBytesAvailable, frameIdx;
	const uint8_t *srcBuf;
	switch (frameType) {
	case ISO_TP_FRAME_SINGLE:
		numBytesAvailable = rxmsg->data8[isoHeaderByteIndex] & 0xf;
		this->waitingForNumBytes = numBytesAvailable;
		srcBuf = rxmsg->data8 + 1;
		break;
	case ISO_TP_FRAME_FIRST:
		this->waitingForNumBytes = ((rxmsg->data8[isoHeaderByteIndex] & 0xf) << 8) | rxmsg->data8[isoHeaderByteIndex + 1];
		this->waitingForFrameIndex = 1;
		numBytesAvailable = minI(this->waitingForNumBytes, 6 - isoHeaderByteIndex);
		srcBuf = rxmsg->data8 + 2 + isoHeaderByteIndex;
		rxTransport->onTpFirstFrame(); // used to send flow control message
		break;
	case ISO_TP_FRAME_CONSECUTIVE:
		frameIdx = rxmsg->data8[isoHeaderByteIndex] & 0xf;
		if (this->waitingForNumBytes < 0 || this->waitingForFrameIndex != frameIdx) {
			// todo: that's an abnormal situation, and we probably should react?
			return 0;
		}
		numBytesAvailable = minI(this->waitingForNumBytes, 7 - isoHeaderByteIndex);
		srcBuf = rxmsg->data8 + 1 + isoHeaderByteIndex;
		this->waitingForFrameIndex = (this->waitingForFrameIndex + 1) & 0xf;
		break;
	case ISO_TP_FRAME_FLOW_CONTROL:
		// todo: currently we just ignore the FC frame
		return 0;
	default:
		// bad frame type
		return 0;
	}

/** performance optimization specific to TS over CAN tunnelling
TODO: refactor into child class if we ever choose to revive this logic
#if defined(TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME)
	if (frameType == ISO_TP_FRAME_SINGLE) {
		// restore the CRC on the whole packet
		uint32_t crc = crc32((void *) srcBuf, numBytesAvailable);
		// we need a separate buffer for crc because srcBuf may not be word-aligned for direct copy
		uint8_t crcBuffer[sizeof(uint32_t)];
		*(uint32_t *) (crcBuffer) = SWAP_UINT32(crc);

		// now set the packet size
		*(uint16_t *) shortCrcPacketStagingArea = SWAP_UINT16(numBytesAvailable);
		// copy the data
		if (numBytesAvailable > 0)
			memcpy(shortCrcPacketStagingArea + sizeof(uint16_t), srcBuf, numBytesAvailable);
		// copy the crc to the end
		memcpy(shortCrcPacketStagingArea + sizeof(uint16_t) + numBytesAvailable, crcBuffer, sizeof(crcBuffer));

		// use the reconstructed tmp buffer as a source buffer
		srcBuf = shortCrcPacketStagingArea;
		// we added the 16-bit size & 32-bit crc bytes
		numBytesAvailable += sizeof(uint16_t) + sizeof(crcBuffer);
	}
#endif *//* TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME */

	int numBytesToCopy = minI(availableAtBuffer, numBytesAvailable);
	if (destinationBuff != nullptr) {
		memcpy(destinationBuff, srcBuf, numBytesToCopy);
	}
	srcBuf += numBytesToCopy;
	waitingForNumBytes -= numBytesAvailable;
	isComplete = (waitingForNumBytes == 0);
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

void CanStreamerState::reset() {
  waitingForNumBytes = 0;
  waitingForFrameIndex = 0;
  isComplete = false;
}

int CanStreamerState::sendDataTimeout(const uint8_t *txbuf, int numBytes, can_sysinterval_t timeout) {
	int offset = 0;

	if (engineConfiguration->verboseIsoTp) {
		PRINT("*** INFO: sendDataTimeout %d" PRINT_EOL, numBytes);
	}

	if (numBytes < 1)
		return 0;

	// 1 frame
	if (numBytes <= 7 - isoHeaderByteIndex) {
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
	for (size_t numFcReceived = 0; ; numFcReceived++) {
		if (rxTransport->receive(&rxmsg, timeout) != CAN_MSG_OK) {
#ifdef SERIAL_CAN_DEBUG
			PRINT("*** ERROR: CAN Flow Control frame not received" PRINT_EOL);
#endif /* SERIAL_CAN_DEBUG */
			//warning(ObdCode::CUSTOM_ERR_CAN_COMMUNICATION, "CAN Flow Control frame not received");
			return 0;
		}
		receiveFrame(&rxmsg, nullptr, 0, timeout);
		uint8_t frameType = (rxmsg.data8[isoHeaderByteIndex] >> 4) & 0xf;
		uint8_t flowStatus = rxmsg.data8[isoHeaderByteIndex] & 0xf;
		// if something is not ok
		if ((frameType != ISO_TP_FRAME_FLOW_CONTROL) || (flowStatus != CAN_FLOW_STATUS_OK)) {
			// if the receiver is not ready yet and asks to wait for the next FC frame (give it 3 attempts)
			if ((frameType == ISO_TP_FRAME_FLOW_CONTROL) && (flowStatus == CAN_FLOW_STATUS_WAIT_MORE) && (numFcReceived < 3)) {
				continue;
			}
#ifdef SERIAL_CAN_DEBUG
			efiPrintf("*** ERROR: CAN Flow Control mode not supported");
#endif /* SERIAL_CAN_DEBUG */
			//warning(ObdCode::CUSTOM_ERR_CAN_COMMUNICATION, "CAN Flow Control mode not supported");
			return 0;
		}
		uint8_t blockSize = rxmsg.data8[isoHeaderByteIndex + 1];
		uint8_t minSeparationTime = rxmsg.data8[isoHeaderByteIndex + 2];
		if (blockSize != 0 || minSeparationTime != 0) {
			// todo: process other Flow Control fields (see ISO 15765-2)
#ifdef SERIAL_CAN_DEBUG
			efiPrintf("*** ERROR: CAN Flow Control fields not supported");
#endif /* SERIAL_CAN_DEBUG */
			//warning(ObdCode::CUSTOM_ERR_CAN_COMMUNICATION, "CAN Flow Control fields not supported");
		}
		break;
	}
#endif /* EFI_UNIT_TEST */

	// send the rest of the data
	int idx = 1;
	while (numBytes > 0) {
		int len = minI(numBytes, 7);
		// send the consecutive frames
		header.frameType = ISO_TP_FRAME_CONSECUTIVE;
		header.index = ((idx++) & 0x0f);
		header.numBytes = len;
		numSent = sendFrame(header, txbuf + offset, len, timeout);
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
		//warning(ObdCode::CUSTOM_ERR_CAN_COMMUNICATION, "CAN sendDataTimeout() problems");
	}
	txFifoBuf.clear();

	return CAN_MSG_OK;
}

can_msg_t CanStreamerState::streamReceiveTimeout(size_t *np, uint8_t *rxbuf, can_sysinterval_t timeout) {
	size_t availableBufferSpace = *np;

	// first, fill the data from the stored buffer (saved from the previous CAN frame)
	int receivedSoFar = getDataFromFifo(rxbuf, availableBufferSpace);

	// if even more data is needed, then we receive more CAN frames
	while (availableBufferSpace > 0) {
		CANRxFrame rxmsg;
		if (rxTransport->receive(&rxmsg, timeout) == CAN_MSG_OK) {
			int numReceived = receiveFrame(&rxmsg, rxbuf + receivedSoFar, availableBufferSpace, timeout);

			if (numReceived < 1)
				break;
			availableBufferSpace -= numReceived;
			receivedSoFar += numReceived;
		} else {
			break;
		}
	}
	*np -= availableBufferSpace;

#ifdef SERIAL_CAN_DEBUG
	efiPrintf("* ret: %d %d (%d)", i, *np, availableBufferSpace);
	for (int j = 0; j < receivedSoFar; j++) {
		efiPrintf("* [%d]: %02x", j, rxbuf[j]);
	}
#endif /* SERIAL_CAN_DEBUG */

	return CAN_MSG_OK;
}

#endif // HAL_USE_CAN || EFI_UNIT_TEST
