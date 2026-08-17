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
	// But CAN_ID() compares only the raw 11/29-bit identifier, so an extended frame whose low bits
	// collide with our id would also reach us - reject those, they are not our ISO-TP packets.
	if (frame.IDE) {
		return;
	}
	// ISO-TP flow control frames acknowledge OUR multi-frame TX. Route them to the FC
	// wait (CanTransport::waitForFlowControl) instead of queuing them as RX data -
	// otherwise a concurrent background request would be mis-parsed by the FC wait and
	// corrupt the ISO-TP RX state (see sendDataTimeout in isotp.cpp).
	if ((frame.DLC >= 3) && (((frame.data8[0] >> 4) & 0xf) == ISO_TP_FRAME_FLOW_CONTROL)) {
		onFlowControlFrame(frame.data8[1], frame.data8[2]);
		return;
	}
	// todo: what if the FIFO is full?
	CanRxMessage msg(frame);
	if (engineConfiguration->verboseIsoTp) {
		PRINT("*** INFO: CanTsListener decodeFrame %d" PRINT_EOL, isoTpPacketCounter++);
	}
	if (!rxFifo.put(msg)) {
		rxFifoOverflow++;
		warning(ObdCode::CUSTOM_ERR_CAN_COMMUNICATION, "CAN sendDataTimeout() problems");
	}
}

#if HAL_USE_CAN

void CanTransport::init() {
	registerCanListener(g_listener);
}

// ISO-TP flow-control wait diagnostics (see isotpinfo): how many multi-frame
// sends got their FC and how many timed out waiting for it. A timeout count
// that grows while the host is connected means the host's FC frames are being
// lost or missed - the host then never receives the consecutive frames and
// drops the link.
static uint32_t fcWaitOk = 0;
static uint32_t fcWaitTimeout = 0;

can_msg_t CanTransport::transmit(CanTxMessage &/*ctfp*/, can_sysinterval_t /*timeout*/) {
	// we do nothing here - see CanTxMessage::~CanTxMessage()
	return CAN_MSG_OK;
}

void CanTransport::onTpFirstFrame() {
  // todo: why nothing? broken iso-tp on ECU side?
}

uint32_t CanTransport::getFcCounterSnapshot() {
	return g_listener.getFcCounter();
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

can_msg_t CanTransport::waitForFlowControl(uint32_t initialFcCounter, uint8_t *blockSize, uint8_t *minSeparationTime, can_sysinterval_t timeout) {
		// FC frames never enter the RX FIFO (CanTsListener::decodeFrame bumps a counter
		// instead), so poll that counter. Anything else that arrives while we wait stays
		// queued in the RX FIFO for the next command instead of being consumed here.
		// The counter snapshot was taken BEFORE our FIRST frame went out (see
		// sendDataTimeout), so an FC that arrived while we were still in canTransmit
		// is correctly seen as a hit.
		efitick_t deadline = getTimeNowNt() + MS2NT(TIME_I2MS(timeout));
		while (getTimeNowNt() < deadline) {
			if (g_listener.getFcCounter() != initialFcCounter) {
				g_listener.getLastFc(*blockSize, *minSeparationTime);
				fcWaitOk++;
				return CAN_MSG_OK;
			}
			chThdSleepMilliseconds(1);
		}
		fcWaitTimeout++;
		return CAN_MSG_TIMEOUT;
	}

void tsOverCanInit() {
	transport.init();

	addConsoleAction("isotpinfo", [] {
		efiPrintf("isotp: rxFifoOverflow=%u ignoredFrames=%u desyncResets=%u rxFifoBufOverflow=%u rxFifoCount=%d",
			(unsigned)g_listener.getRxFifoOverflow(),
			(unsigned)state.ignoredFrames,
			(unsigned)state.desyncResets,
			(unsigned)state.rxFifoBufOverflow,
			g_listener.getRxFifoCount());
		efiPrintf("isotp: canWriteOk=%u canWriteNotOk=%u (TX frames dropped when mailboxes busy)",
			(unsigned)engine->outputChannels.canWriteOk,
			(unsigned)engine->outputChannels.canWriteNotOk);
		efiPrintf("isotp: fcWaitOk=%u fcWaitTimeout=%u (multi-frame sends that missed the host's FC)",
			(unsigned)fcWaitOk, (unsigned)fcWaitTimeout);
	});
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
