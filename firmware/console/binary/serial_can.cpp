/**
 * @file	serial_can.cpp
 *
 * This code is a bridge between a serial streaming used by TS and a packet-frame CAN-bus, using the ISO-TP protocol.
 * ISO 15765-2, or ISO-TP (Transport Layer), which is an international standard for sending data packets over a CAN-Bus.
 * https://en.wikipedia.org/wiki/ISO_15765-2
 *
 * @date Aug 1, 2020
 * @author andreika <promethePRINTus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

// todo: this file is asking to improve conditional compilation. unit_tests and cypress/kinetis are both special cases
#if HAL_USE_CAN || EFI_UNIT_TEST
#include "serial_can.h"
#include "can.h"
#include "can_msg_tx.h"
#endif // HAL_USE_CAN || EFI_UNIT_TEST


#if HAL_USE_CAN
// this one installs itself as top level CAN bus listener with static frame ID
static CanTsListener g_listener;
// for RX, this one delegates to above FIFO via global field
static CanTransport transport(&g_listener);

static CanStreamerState state(&transport, &transport, /*bus*/0, CAN_ECU_SERIAL_RX_ID, CAN_ECU_SERIAL_TX_ID);
#endif // HAL_USE_CAN

#if HAL_USE_CAN || EFI_UNIT_TEST

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
		warning(ObdCode::CUSTOM_ERR_CAN_COMMUNICATION, "CAN sendDataTimeout() problems");
	}
}

#if HAL_USE_CAN

void CanTransport::init() {
	registerCanListener(g_listener);
}

can_msg_t CanTransport::transmit(CanTxMessage &/*ctfp*/, can_sysinterval_t /*timeout*/) {
	// we do nothing here - see CanTxMessage::~CanTxMessage()
	return CAN_MSG_OK;
}

void CanTransport::onTpFirstFrame() {
  // todo: why nothing? broken iso-tp on ECU side?
}

can_msg_t CanTransport::receive(CANRxFrame *crfp, can_sysinterval_t timeout) {
	// see CanTsListener and processCanRxMessage()
	CanRxMessage msg;
	if (this->source->get(msg, timeout)) {
		*crfp = msg.frame;
		return CAN_MSG_OK;
	}
	return CAN_MSG_TIMEOUT;
}

void tsOverCanInit() {
	transport.init();
}

msg_t canStreamAddToTxTimeout(size_t *np, const uint8_t *txbuf, sysinterval_t timeout) {
	return state.streamAddToTxTimeout(np, txbuf, timeout);
}

msg_t canStreamFlushTx(sysinterval_t timeout) {
	return state.streamFlushTx(timeout);
}

	// np uses in/out parameter approach. Yes ChibiOS does same but still evil!
	// in entry: number of data frames to receive
	// on exit the number of frames actually received
msg_t canStreamReceiveTimeout(size_t *np, uint8_t *rxbuf, sysinterval_t timeout) {
	return state.streamReceiveTimeout(np, rxbuf, timeout);
}

#endif /* HAL_USE_CAN */



#endif // HAL_USE_CAN || EFI_UNIT_TEST
