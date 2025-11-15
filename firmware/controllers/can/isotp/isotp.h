#pragma once

#include "fifo_buffer.h"
#include "can_msg_tx.h"

#if EFI_UNIT_TEST
#define PRINT printf
#define PRINT_EOL "\n"
#else
#define PRINT efiPrintf
#define PRINT_EOL ""
#endif

#if EFI_PROD_CODE | EFI_SIMULATOR
#define can_msg_t msg_t
#define can_sysinterval_t sysinterval_t
#define CAN_MSG_OK MSG_OK
#define CAN_MSG_TIMEOUT MSG_TIMEOUT
#else
#include "can_mocks.h"
#endif /* EFI_UNIT_TEST */

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

// todo: what's the point of this wrapper/holder class anyway?
class CanRxMessage {
public:
	CanRxMessage() {}

	CanRxMessage(const CANRxFrame &f) {
		frame = f;
	}

	CanRxMessage(const CanRxMessage& msg) : frame(msg.frame) {}

	CanRxMessage& operator=(const CanRxMessage& msg) {
    // full content copy
		frame = msg.frame;
		return *this;
	}

public:
	CANRxFrame frame;
};

class CanRxMessageSource {
public:
  virtual bool get(CanRxMessage &item, int timeout) = 0;
};

class ICanTransmitter {
public:
  virtual can_msg_t transmit(const CanTxMessage *ctfp, can_sysinterval_t timeout) = 0;
};

class ICanReceiver {
public:
  virtual can_msg_t receive(CANRxFrame *crfp, can_sysinterval_t timeout) = 0;
  virtual void onTpFirstFrame() = 0;
};

// We need an abstraction layer for unit-testing
// todo: no reason for composite entity to exist, keep splitting CanStreamerState into RX and TX!
class ICanTransport : public ICanTransmitter, public ICanReceiver {
};

// most efficient sizes are 6 + x * 7 that way whole buffer is transmitted as (x+1) full packets
#ifndef CAN_FIFO_BUF_SIZE
#define CAN_FIFO_BUF_SIZE 76
#endif // CAN_FIFO_BUF_SIZE

#define CAN_FIFO_FRAME_SIZE 8

class CanStreamerState {
public:
	fifo_buffer<uint8_t, CAN_FIFO_BUF_SIZE> rxFifoBuf;
	fifo_buffer<uint8_t, CAN_FIFO_BUF_SIZE> txFifoBuf;

/*
#if defined(TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME)
	// used to restore the original packet with CRC
    uint8_t shortCrcPacketStagingArea[13];
#endif
*/

	// Offset of first ISO-TP byte, usually 0
	// but some vendors add some specific data in first CAN byte
	size_t isoHeaderByteIndex = 0;

	// used for multi-frame ISO-TP packets
	int waitingForNumBytes = 0;
	int waitingForFrameIndex = 0;

	ICanTransmitter *txTransport;
	ICanReceiver *rxTransport;

	int busIndex;
	int txFrameId;

public:
	CanStreamerState(ICanTransmitter *p_txTransport, ICanReceiver *p_rxTransport, int p_busIndex, int p_txFrameId)
	 :
	 txTransport(p_txTransport),
	 rxTransport(p_rxTransport),
	 busIndex(p_busIndex),
	 txFrameId(p_txFrameId)
	  {}

  bool isComplete{};

  void reset();

	int sendFrame(const IsoTpFrameHeader & header, const uint8_t *data, int num, can_sysinterval_t timeout);
	int receiveFrame(const CANRxFrame *rxmsg, uint8_t *buf, int num, can_sysinterval_t timeout);
	int getDataFromFifo(uint8_t *rxbuf, size_t &numBytes);
	// returns the number of bytes sent
	int sendDataTimeout(const uint8_t *txbuf, int numBytes, can_sysinterval_t timeout);

	// streaming support for TS I/O (see tunerstudio_io.cpp)
	can_msg_t streamAddToTxTimeout(size_t *np, const uint8_t *txbuf, can_sysinterval_t timeout);
	can_msg_t streamFlushTx(can_sysinterval_t timeout);
	can_msg_t streamReceiveTimeout(size_t *np, uint8_t *rxbuf, can_sysinterval_t timeout);
};
