/**
 * @file	serial_can.h
 *
 * @date Aug 1, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "isotp.h"
#include "can_listener.h"

class CanTsListener : public CanListener, public CanRxMessageSource {
public:
	CanTsListener(uint32_t id)
		: CanListener(id)
	{
	}

	virtual void decodeFrame(const CANRxFrame& frame, efitick_t nowNt);

	bool get(CanRxMessage &item, int timeout) {
		return rxFifo.get(item, timeout);
	}

protected:
	fifo_buffer_sync<CanRxMessage, CAN_FIFO_FRAME_SIZE> rxFifo;
};

#if HAL_USE_CAN
class CanTransport : public ICanTransport {
public:
	CanTransport(uint32_t p_rxFrameId, uint32_t p_txFrameId)
		: source(p_rxFrameId), txFrameId(p_txFrameId)
	{
	}
	void init();

	virtual can_msg_t transmit(const CanTxMessage *ctfp, can_sysinterval_t timeout) override;
	virtual can_msg_t receive(CANRxFrame *crfp, can_sysinterval_t timeout) override;

	CanTsListener source;
	uint32_t txFrameId;
};

void tsOverCanInit();

// we don't have canStreamSendTimeout() because we need to "bufferize" the stream and send it in fixed-length packets
msg_t canStreamAddToTxTimeout(size_t *np, const uint8_t *txbuf, sysinterval_t timeout);
msg_t canStreamFlushTx(sysinterval_t timeout);

msg_t canStreamReceiveTimeout(size_t *np, uint8_t *rxbuf, sysinterval_t timeout);
#endif /* HAL_USE_CAN */
