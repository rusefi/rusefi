/**
 * @file	can_msg_tx.cpp
 *
 * CAN message transmission
 *
 * @date Mar 13, 2020
 * @author Matthew Kennedy, (c) 2012-2020
 */

#include "pch.h"

#include "can_msg_tx.h"
#include "auto_generated_can_category.h"

#include "can.h"

#include "fifo_buffer.h"
#if EFI_SIMULATOR || EFI_UNIT_TEST
fifo_buffer<CANTxFrame, TEST_CAN_BUFFER_SIZE> txCanBuffer;
#endif // EFI_SIMULATOR

#if EFI_PROD_CODE && HAL_USE_USB_CDC_2
#include "can_sniffer.h"
extern CanSniffer canSniffer;
#endif

bool verboseCanTxError = false;

#if EFI_UNIT_TEST && !EFI_CAN_SUPPORT
int txErrorCount[EFI_CAN_BUS_COUNT] = {};
#endif

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
struct CanTxQueuedFrame {
	CANTxFrame frame;
	CanCategory category;
	int8_t completionSlot;
};

// A waiting sender reserves a slot until it reads the result. If it times out,
// the worker must finish handling the frame before that slot can be reused.
enum class CompletionState : uint8_t { Free, Pending, Done, Cancelled };

struct CanTxCompletionSlot {
	CompletionState state = CompletionState::Free;
	msg_t result = MSG_TIMEOUT;
#if !EFI_UNIT_TEST
	threads_queue_t waiters;

	CanTxCompletionSlot() {
		osalThreadQueueObjectInit(&waiters);
	}
#endif
};

struct CanTxBusState {
	fifo_buffer<CanTxQueuedFrame, CAN_TX_QUEUE_CAPACITY> queue;
	CanTxQueuedFrame inFlight;
	bool hasInFlight = false;
	uint32_t inFlightStartMs = 0;
	bool inFlightLogged = false;
	int dropCount = 0;
	uint32_t generation = 0;
	CanTxCompletionSlot slots[2];
#if !EFI_UNIT_TEST
	threads_queue_t producerWaiters;
	mutex_t serviceMutex;

	CanTxBusState() {
		osalThreadQueueObjectInit(&producerWaiters);
		chMtxObjectInit(&serviceMutex);
	}
#endif
};

static CanTxBusState txBuses[EFI_CAN_BUS_COUNT];

// Queue/slot changes are protected by chSysLock. This separate per-bus lock
// spans serviceOne's HAL call, so reset/remove quiesces a bus before canStop.
class CanTxBusServiceLock {
public:
	explicit CanTxBusServiceLock(CanTxBusState& bus) : m_bus(bus) {
#if !EFI_UNIT_TEST
		chMtxLock(&m_bus.serviceMutex);
#endif
	}

	~CanTxBusServiceLock() {
#if !EFI_UNIT_TEST
		chMtxUnlock(&m_bus.serviceMutex);
#endif
	}

private:
	CanTxBusState& m_bus;
};

#if !EFI_UNIT_TEST
static threads_queue_t workWaiters;
static bool workNotified;
struct CanTxWorkWaitersInit {
	CanTxWorkWaitersInit() {
		osalThreadQueueObjectInit(&workWaiters);
	}
} canTxWorkWaitersInit;
#endif

#if EFI_UNIT_TEST
static void (*unitTestWaitHook)(size_t) = nullptr;
static void (*unitTestRescheduleHook)() = nullptr;
#endif

/*static*/ CANDriver* CanTxMessage::s_devices[EFI_CAN_BUS_COUNT] = {
	nullptr,
	nullptr,
#if (EFI_CAN_BUS_COUNT >= 3)
	nullptr
#endif
};

// Waking a thread does not immediately run it. After updating the queue and
// result slots, let higher-priority threads run before unlocking. ChibiOS
// reports a "priority order violation" if we skip this step.
static void rescheduleAfterWakeS() {
#if !EFI_UNIT_TEST
	osalOsRescheduleS();
#else
	if (unitTestRescheduleHook) {
		unitTestRescheduleHook();
	}
#endif
}

static void wakeProducerWaitersI(CanTxBusState& bus) {
#if !EFI_UNIT_TEST
	osalThreadDequeueAllI(&bus.producerWaiters, MSG_OK);
#else
	UNUSED(bus);
#endif
}

static void notifyWorkI() {
#if !EFI_UNIT_TEST
	workNotified = true;
	osalThreadDequeueAllI(&workWaiters, MSG_OK);
#endif
}

static void releaseSlotI(CanTxBusState& bus, CanTxCompletionSlot& slot) {
	slot.state = CompletionState::Free;
	wakeProducerWaitersI(bus);
}

static void resetBusI(size_t idx) {
	auto& bus = txBuses[idx];
	bus.queue.clear();
	bus.hasInFlight = false;
	bus.generation++;
	for (auto& slot : bus.slots) {
		if (slot.state == CompletionState::Pending) {
			slot.result = MSG_RESET;
			slot.state = CompletionState::Done;
#if !EFI_UNIT_TEST
			osalThreadDequeueAllI(&slot.waiters, MSG_RESET);
#endif
		} else if (slot.state == CompletionState::Cancelled) {
			releaseSlotI(bus, slot);
		}
	}
	wakeProducerWaitersI(bus);
	notifyWorkI();
}

/*static*/ void CanTxMessage::setDevice(size_t idx, CANDriver* device) {
	if (idx >= efi::size(s_devices)) {
		criticalError("Cannot install CAN%d bus!", idx + 1);
		return;
	}
	CanTxBusServiceLock serviceLock(txBuses[idx]);
	chSysLock();
	s_devices[idx] = device;
	notifyWorkI();
	rescheduleAfterWakeS();
	chSysUnlock();
}

/*static*/ void CanTxMessage::stopBus(size_t idx) {
	if (idx < EFI_CAN_BUS_COUNT) {
		CanTxBusServiceLock serviceLock(txBuses[idx]);
		chSysLock();
		resetBusI(idx);
		rescheduleAfterWakeS();
		chSysUnlock();
	}
}

/*static*/ void CanTxMessage::removeDevice(size_t idx) {
	if (idx >= EFI_CAN_BUS_COUNT) {
		return;
	}
	// This serializes reset/remove with serviceOne. Call canStop only after
	// removeDevice returns, so no HAL transmit can overlap the controller stop.
	CanTxBusServiceLock serviceLock(txBuses[idx]);
	// Block new sends and clear this bus's queue under the same lock.
	// Changing the generation tells waiting senders that their bus was reset.
	chSysLock();
	s_devices[idx] = nullptr;
	resetBusI(idx);
	rescheduleAfterWakeS();
	chSysUnlock();
}

/*static*/ int CanTxMessage::getQueueDropCount(size_t idx) {
	if (idx >= EFI_CAN_BUS_COUNT) {
		return 0;
	}
	chSysLock();
	auto result = txBuses[idx].dropCount;
	chSysUnlock();
	return result;
}

#if EFI_UNIT_TEST
/*static*/ void CanTxMessage::setWaitHookForUnitTest(void (*hook)(size_t)) {
	unitTestWaitHook = hook;
}

/*static*/ void CanTxMessage::setRescheduleHookForUnitTest(void (*hook)()) {
	unitTestRescheduleHook = hook;
}
#endif

static void recordCanTransmitResult(size_t busIndex, CanCategory category, const CANTxFrame& frame, msg_t msg) {
#if EFI_PROD_CODE && HAL_USE_USB_CDC_2
	if ((msg == MSG_OK) && (engineConfiguration->canSniffer[busIndex].listenOurs)) {
		canSniffer.handle_can_message(busIndex, frame, getTimeNowNt());
	}
#endif
#if EFI_TUNER_STUDIO
	chSysLock();
	if (msg == MSG_OK) {
		engine->outputChannels.canWriteOk++;
	} else {
		extern int txErrorCount[EFI_CAN_BUS_COUNT];
		engine->outputChannels.canWriteNotOk++;
		txErrorCount[busIndex]++;
	}
	chSysUnlock();

	if (msg != MSG_OK && verboseCanTxError) {
		efiPrintf("%s TX ERR CAN%d message: ID=%x/l=%x", getCanCategory(category), busIndex + 1,
			(unsigned int)CAN_ID(frame), frame.DLC);
	}
#endif
}

/*static*/ bool CanTxMessage::serviceOne(size_t busIndex) {
	if (busIndex >= EFI_CAN_BUS_COUNT) {
		return false;
	}

	CanTxBusServiceLock serviceLock(txBuses[busIndex]);
	CanTxQueuedFrame item;
	CANDriver* device;
	chSysLock();
	auto& bus = txBuses[busIndex];
	if (!bus.hasInFlight && bus.queue.isEmpty()) {
		chSysUnlock();
		return false;
	}
	if (!bus.hasInFlight) {
		bus.inFlight = bus.queue.get();
		bus.hasInFlight = true;
		bus.inFlightStartMs = getTimeNowMs();
		bus.inFlightLogged = false;
		wakeProducerWaitersI(bus);
	}
	item = bus.inFlight;
	device = s_devices[busIndex];
	// If the sender timed out while this frame was queued, discard the frame
	// instead of sending it late, and free its result slot.
	const bool cancelled = item.completionSlot >= 0 && bus.slots[item.completionSlot].state == CompletionState::Cancelled;
	if (cancelled) {
		bus.hasInFlight = false;
		releaseSlotI(bus, bus.slots[item.completionSlot]);
	}
	rescheduleAfterWakeS();
	chSysUnlock();
	if (cancelled) {
		return true;
	}

	ScopePerf pc(PE::CanDriverTx);
	// Unsigned subtraction deliberately handles the millisecond clock wrapping.
	const bool expired = static_cast<uint32_t>(getTimeNowMs() - bus.inFlightStartMs) >= 100;
	msg_t result = expired ? MSG_TIMEOUT : MSG_RESET;
	const bool shouldRecord = device && engine->allowCanTx;
	if (!expired && shouldRecord) {
		bool verboseCan = engineConfiguration->verboseCan && busIndex == 0;
		verboseCan |= engineConfiguration->verboseCan2 && busIndex == 1;
#if (EFI_CAN_BUS_COUNT >= 3)
		verboseCan |= engineConfiguration->verboseCan3 && busIndex == 2;
#endif
		if (verboseCan && !bus.inFlightLogged) {
			efiPrintf("%s Sending CAN%d message: ID=%x/l=%x %x %x %x %x %x %x %x %x",
				getCanCategory(item.category), busIndex + 1,
				(unsigned int)CAN_ID(item.frame), item.frame.DLC,
				item.frame.data8[0], item.frame.data8[1], item.frame.data8[2], item.frame.data8[3],
				item.frame.data8[4], item.frame.data8[5], item.frame.data8[6], item.frame.data8[7]);
		}
		bus.inFlightLogged = true;
		result = canTransmit(device, CAN_ANY_MAILBOX, &item.frame, TIME_IMMEDIATE);
	}
	if (result == MSG_TIMEOUT) {
		if (!expired) {
			return false;
		}
	}
	if (shouldRecord) {
		recordCanTransmitResult(busIndex, item.category, item.frame, result);
	}

	chSysLock();
	// Release the head before notifying a waiter or unit-test reschedule hook.
	bus.hasInFlight = false;
	if (item.completionSlot >= 0) {
		auto& slot = bus.slots[item.completionSlot];
		if (slot.state == CompletionState::Pending) {
			slot.result = result;
			slot.state = CompletionState::Done;
#if !EFI_UNIT_TEST
			osalThreadDequeueAllI(&slot.waiters, MSG_OK);
#endif
		} else if (slot.state == CompletionState::Cancelled) {
			// The sender timed out after the worker took the frame. The worker
			// has now finished with it, so its result slot can be reused.
			releaseSlotI(bus, slot);
		}
	}
	rescheduleAfterWakeS();
	chSysUnlock();
	return true;
}

/*static*/ void CanTxMessage::service(size_t busIndex) {
	while (serviceOne(busIndex)) {
	}
}

/*static*/ msg_t CanTxMessage::waitForWork(sysinterval_t timeout) {
#if !EFI_UNIT_TEST
	chSysLock();
	if (workNotified) {
		workNotified = false;
		chSysUnlock();
		return MSG_OK;
	}
	auto result = osalThreadEnqueueTimeoutS(&workWaiters, timeout);
	chSysUnlock();
	return result;
#else
	UNUSED(timeout);
	return MSG_TIMEOUT;
#endif
}
#endif // EFI_CAN_SUPPORT || EFI_UNIT_TEST

CanTxMessage::CanTxMessage(CanCategory p_category, uint32_t eid, uint8_t dlc, size_t bus, bool isExtended) {
    category = p_category;
#if HAS_CAN_FRAME
#ifndef STM32H7XX
	// ST bxCAN device
	m_frame.IDE = isExtended ? CAN_IDE_EXT : CAN_IDE_STD;
	m_frame.RTR = CAN_RTR_DATA;
#else /* if STM32H7XX */
	// Bosch M_CAN FDCAN device
	m_frame.common.XTD = isExtended;
	m_frame.common.RTR = 0;
#endif

	if (isExtended) {
		CAN_EID(m_frame) = eid;
	} else {
	    if (eid >= 0x800) {
	        criticalError("CAN TX error extended CAN ID 0x%x/%d %s", eid, eid, getCanCategory(category));
	        return;
	    }
		CAN_SID(m_frame) = eid;
	}

	setDlc(dlc);

	setBus(bus);

	setArrayValues(m_frame.data8, 0);
#endif // HAS_CAN_FRAME
}

CanTxMessage::~CanTxMessage() {
#if EFI_SIMULATOR || EFI_UNIT_TEST
	txCanBuffer.put(m_frame);

#if 0 && EFI_UNIT_TEST
	printf("%s Sending CAN%d message: ID=%x/l=%x %x %x %x %x %x %x %x %x \n",
		   getCanCategory(category),
		   busIndex + 1,
		   (unsigned int)CAN_ID(m_frame),
		   m_frame.DLC,
		   m_frame.data8[0], m_frame.data8[1],
		   m_frame.data8[2], m_frame.data8[3],
		   m_frame.data8[4], m_frame.data8[5],
		   m_frame.data8[6], m_frame.data8[7]);
#endif
#endif // EFI_SIMULATOR

#if EFI_CAN_SUPPORT
	ScopePerf pc(PE::CanDriverTx);

	if (!engine->allowCanTx) {
		return;
	}

	if (busIndex >= EFI_CAN_BUS_COUNT) {
		// Error already throuwn from CanTxMessage::setBus
		// just do not access out of bounds
		return;
	}

	auto device = s_devices[busIndex];
	if (!device) {
		criticalError("Send: CAN%d device not configured %s %x", busIndex + 1, getCanCategory(category),
		   (unsigned int)CAN_ID(m_frame));
		return;
	}

	bool verboseCan = engineConfiguration->verboseCan && busIndex == 0;
	verboseCan |= engineConfiguration->verboseCan2 && busIndex == 1;
#if (EFI_CAN_BUS_COUNT >= 3)
	verboseCan |= engineConfiguration->verboseCan3 && busIndex == 2;
#endif

	if (verboseCan) {
		efiPrintf("%s Sending CAN%d message: ID=%x/l=%x %x %x %x %x %x %x %x %x",
				getCanCategory(category),
				busIndex + 1,
				(unsigned int)CAN_ID(m_frame),
				m_frame.DLC,
				m_frame.data8[0], m_frame.data8[1],
				m_frame.data8[2], m_frame.data8[3],
				m_frame.data8[4], m_frame.data8[5],
				m_frame.data8[6], m_frame.data8[7]);
	}

	// Wait for a free mailbox. Serial (ISO-TP) frames carry the TS/console session:
	// a dropped frame truncates the response mid-burst for the host and kills the
	// session, so serial sends retry transient mailbox failures instead of giving
	// up after one timeout. The per-attempt budget is short (500ms - with the board
	// BCM flood throttled during serial sessions the bus has ~99% idle time): the
	// previous single 5s attempt could hold the FIRST frame in the mailbox longer
	// than the 1s flow-control wait, so the host's FC arrived after the wait had
	// already expired and the whole exchange died. On persistent failure the TX
	// mailboxes are aborted (ABRQ) to clear any un-ACKed frame that would otherwise
	// retry forever and wedge the bus for every later burst. Non-serial broadcast
	// traffic keeps the short 100ms budget - dropping a BCM frame is harmless (it
	// repeats at ~165/s).
	msg_t msg;
	if (category == CanCategory::SERIAL) {
		bool sent = false;
		for (int attempt = 0; attempt < 3; attempt++) {
			msg = canTransmit(device, CAN_ANY_MAILBOX, &m_frame, TIME_MS2I(500));
			if (msg == MSG_OK) {
				sent = true;
				break;
			}
			if (attempt < 2) {
				chThdSleepMilliseconds(2);
			}
		}
		if (!sent) {
			// Clear any wedged mailbox so the next burst starts clean.
			for (canmbx_t mailbox = 1; mailbox <= CAN_TX_MAILBOXES; mailbox++) {
				canTryAbortX(device, mailbox);
			}
			msg = MSG_TIMEOUT;
		}
	} else {
		msg = canTransmit(device, CAN_ANY_MAILBOX, &m_frame, TIME_MS2I(100));
	}
#if EFI_PROD_CODE && HAL_USE_USB_CDC_2
	if ((msg == MSG_OK) && (engineConfiguration->canSniffer[busIndex].listenOurs)) {
		canSniffer.handle_can_message(busIndex, m_frame, getTimeNowNt());
	}
#endif
#if EFI_TUNER_STUDIO
	if (msg == MSG_OK) {
		engine->outputChannels.canWriteOk++;
	} else {
extern int txErrorCount[EFI_CAN_BUS_COUNT];
		engine->outputChannels.canWriteNotOk++;
		txErrorCount[busIndex]++;

		if (verboseCanTxError) {
		  efiPrintf("%s TX ERR CAN%d message: ID=%x/l=%x %x %x %x %x %x %x %x %x",
				getCanCategory(category),
				busIndex + 1,
				(unsigned int)CAN_ID(m_frame),
				m_frame.DLC,
				m_frame.data8[0], m_frame.data8[1],
				m_frame.data8[2], m_frame.data8[3],
				m_frame.data8[4], m_frame.data8[5],
				m_frame.data8[6], m_frame.data8[7]);
		}
	}
#endif // EFI_TUNER_STUDIO
#endif /* EFI_CAN_SUPPORT */
}

bool CanTxMessage::submit() {
	if (m_submitted) {
		return false;
	}
	m_submitted = true;

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
	// Tests without a CAN driver mock only collect frames in txCanBuffer.
#if EFI_UNIT_TEST
	if (!canTransmitMock) {
		return true;
	}
#endif
	if (!engine->allowCanTx || busIndex >= EFI_CAN_BUS_COUNT) {
		return false;
	}

	chSysLock();
	auto& bus = txBuses[busIndex];
	if (!s_devices[busIndex]) {
		chSysUnlock();
		criticalError("Send: CAN%d device not configured %s %x", busIndex + 1, getCanCategory(category),
			(unsigned int)CAN_ID(m_frame));
		return false;
	}
	if (bus.queue.isFull()) {
		bus.dropCount++;
		chSysUnlock();
		return false;
	}
	bus.queue.put({ m_frame, category, -1 });
	notifyWorkI();
	rescheduleAfterWakeS();
	chSysUnlock();
	return true;
#endif /* EFI_CAN_SUPPORT || EFI_UNIT_TEST */
	return true;
}

msg_t CanTxMessage::submitAndWait(sysinterval_t timeout) {
	// Mark this message as submitted even if sending fails, so its destructor
	// does not queue another attempt.
	if (m_submitted) {
		return MSG_TIMEOUT;
	}
	m_submitted = true;

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
	if (!engine->allowCanTx || busIndex >= EFI_CAN_BUS_COUNT) {
		return MSG_RESET;
	}

#if !EFI_UNIT_TEST
	const auto start = chVTGetSystemTimeX();
#endif
	int slotIndex = -1;
	uint32_t generation = 0;
	bool generationCaptured = false;
	for (;;) {
		chSysLock();
		auto& bus = txBuses[busIndex];
		if (!generationCaptured) {
			generation = bus.generation;
			generationCaptured = true;
		}
		if (!s_devices[busIndex] || bus.generation != generation) {
			chSysUnlock();
			return MSG_RESET;
		}
		if (!bus.queue.isFull()) {
			for (size_t i = 0; i < efi::size(bus.slots); i++) {
				if (bus.slots[i].state == CompletionState::Free) {
					slotIndex = i;
					bus.slots[i].state = CompletionState::Pending;
					bus.queue.put({ m_frame, category, static_cast<int8_t>(slotIndex) });
					notifyWorkI();
					break;
				}
			}
		}
		if (slotIndex >= 0) {
			rescheduleAfterWakeS();
			chSysUnlock();
			break;
		}
#if EFI_UNIT_TEST
		chSysUnlock();
		if (timeout == TIME_IMMEDIATE || !unitTestWaitHook) {
			chSysLock();
			txBuses[busIndex].dropCount++;
			chSysUnlock();
			return MSG_TIMEOUT;
		}
		unitTestWaitHook(busIndex);
#else
		if (chVTTimeElapsedSinceX(start) >= timeout) {
			bus.dropCount++;
			chSysUnlock();
			return MSG_TIMEOUT;
		}
		auto remaining = timeout - chVTTimeElapsedSinceX(start);
		osalThreadEnqueueTimeoutS(&bus.producerWaiters, remaining);
		chSysUnlock();
#endif
	}

	for (;;) {
		chSysLock();
		auto& bus = txBuses[busIndex];
		auto& slot = bus.slots[slotIndex];
		if (slot.state == CompletionState::Done) {
			auto result = slot.result;
			releaseSlotI(bus, slot);
			rescheduleAfterWakeS();
			chSysUnlock();
			return result;
		}
		if (slot.state == CompletionState::Cancelled) {
			chSysUnlock();
			return MSG_TIMEOUT;
		}
#if EFI_UNIT_TEST
		chSysUnlock();
		if (timeout == TIME_IMMEDIATE || !unitTestWaitHook) {
			chSysLock();
			if (slot.state == CompletionState::Pending) {
				slot.state = CompletionState::Cancelled;
			}
			chSysUnlock();
			return MSG_TIMEOUT;
		}
		unitTestWaitHook(busIndex);
		// The test hook runs the worker or resets the bus so this wait can finish.
		// Tests leave the hook unset to simulate a timeout.
		continue;
#else
		if (chVTTimeElapsedSinceX(start) >= timeout) {
			slot.state = CompletionState::Cancelled;
			chSysUnlock();
			return MSG_TIMEOUT;
		}
		auto remaining = timeout - chVTTimeElapsedSinceX(start);
		osalThreadEnqueueTimeoutS(&slot.waiters, remaining);
		chSysUnlock();
#endif
	}
#else
	UNUSED(timeout);
	return MSG_TIMEOUT;
#endif
}
#if HAS_CAN_FRAME
void CanTxMessage::setDlc(uint8_t dlc) {
	// TODO: CAN vs CANFD
	if (dlc > sizeof(m_frame.data8)) {
		criticalError("CAN: incorrect DLC %d", dlc);
		return;
	}

	m_frame.DLC = dlc;
}

void CanTxMessage::setBus(size_t bus) {
	if (bus >= EFI_CAN_BUS_COUNT) {
		criticalError("CAN: CAN%d incorect bus", bus + 1);
		return;
	}

	busIndex = bus;
}

// LSB Little-endian System, "Intel"
void CanTxMessage::setShortValue(uint16_t value, size_t offset) {
	m_frame.data8[offset] = value & 0xFF;
	m_frame.data8[offset + 1] = value >> 8;
}

void CanTxMessage::setIntValueLsb(uint32_t value, size_t offset) {
	m_frame.data8[offset] = value & 0xFF;
	m_frame.data8[offset + 1] = (value >> 8) & 0xFF;
	m_frame.data8[offset + 2] = (value >> 16) & 0xFF;
	m_frame.data8[offset + 3] = (value >> 24) & 0xFF;
}

// MOTOROLA order, MSB (Most Significant Byte/Big Endian) comes first.
void CanTxMessage::setShortValueMsb(uint16_t value, size_t offset) {
	m_frame.data8[offset] = value >> 8;
	m_frame.data8[offset + 1] = value & 0xFF;
}

void CanTxMessage::setBit(size_t byteIdx, size_t bitIdx) {
	m_frame.data8[byteIdx] |= 1 << bitIdx;
}

uint8_t& CanTxMessage::operator[](size_t index) {
	return m_frame.data8[index];
}
#endif // HAS_CAN_FRAME
