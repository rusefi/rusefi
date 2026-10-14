#include "pch.h"

#include "exti_host_adapter.h"
#include "io_pins.h"

#if EFI_UNIT_TEST
namespace {
constexpr size_t MaxRecordedEvents = 32;
ExtiLineEventForUnitTest recordedEvents[MaxRecordedEvents] = {};
size_t recordedEventCount = 0;
}

ioportid_t getHwPort(const char* msg, brain_pin_e brainPin) {
	UNUSED(msg);
	const int offset = static_cast<int>(brainPin) - static_cast<int>(Gpio::A0);
	const int gpioRange = static_cast<int>(Gpio::K15) - static_cast<int>(Gpio::A0) + 1;
	if (!isBrainPinValid(brainPin) || offset < 0 || offset >= gpioRange) {
		return 0;
	}

	return 1 + static_cast<uint32_t>(offset / 16);
}

ioportmask_t getHwPin(const char* msg, brain_pin_e brainPin) {
	UNUSED(msg);
	const int offset = static_cast<int>(brainPin) - static_cast<int>(Gpio::A0);
	const int gpioRange = static_cast<int>(Gpio::K15) - static_cast<int>(Gpio::A0) + 1;
	if (!isBrainPinValid(brainPin) || offset < 0 || offset >= gpioRange) {
		return 0;
	}

	return static_cast<ioportmask_t>(offset % 16);
}

ioline_t efiExtiMakeLineForUnitTest(ioportid_t port, int index) {
	return port * 16 + static_cast<uint32_t>(index);
}

void palEnableLineEvent(ioline_t line, uint32_t mode) {
	efiExtiRecordLineEventForUnitTest(line, mode, true);
}

void palDisableLineEvent(ioline_t line) {
	efiExtiRecordLineEventForUnitTest(line, 0, false);
}

void efiExtiRecordLineEventForUnitTest(uint32_t line, uint32_t mode, bool enabled) {
	if (recordedEventCount < MaxRecordedEvents) {
		recordedEvents[recordedEventCount++] = { line, mode, enabled };
	}
}

void efiExtiResetLineEventsForUnitTest() {
	recordedEventCount = 0;
}

size_t efiExtiGetLineEventCountForUnitTest() {
	return recordedEventCount;
}

ExtiLineEventForUnitTest efiExtiGetLineEventForUnitTest(size_t index) {
	if (index >= recordedEventCount) {
		return {};
	}
	return recordedEvents[index];
}

void efiExtiRestoreLineEventsForUnitTest(const ExtiLineEventForUnitTest* events, size_t count) {
	recordedEventCount = count < MaxRecordedEvents ? count : MaxRecordedEvents;
	for (size_t i = 0; i < recordedEventCount; i++) {
		recordedEvents[i] = events[i];
	}
}

ScopedExtiAdapterForUnitTest::ScopedExtiAdapterForUnitTest(const brain_pin_e* pins, size_t pinCount)
	: m_eventCount(efiExtiGetLineEventCountForUnitTest())
	, m_pinCount(pinCount < MaxPins ? pinCount : MaxPins) {
	for (size_t i = 0; i < m_eventCount; i++) {
		m_events[i] = efiExtiGetLineEventForUnitTest(i);
	}

	efiExtiSnapshotChannelsForUnitTest(m_channels);
	efiExtiResetChannelsForUnitTest();

	efiExtiResetLineEventsForUnitTest();

	for (size_t i = 0; i < m_pinCount; i++) {
		const brain_pin_e pin = pins[i];
		m_pins[i] = { pin, getBrainUsedPin(brainPin_to_index(pin)), efiReadPin(pin) };
		efiSetPadUnused(pin);
		setMockState(pin, false);
	}
}

ScopedExtiAdapterForUnitTest::~ScopedExtiAdapterForUnitTest() {
	efiExtiRestoreChannelsForUnitTest(m_channels);

	efiExtiRestoreLineEventsForUnitTest(m_events, m_eventCount);

	for (size_t i = 0; i < m_pinCount; i++) {
		efiSetPadUnused(m_pins[i].pin);
		getBrainUsedPin(brainPin_to_index(m_pins[i].pin)) = m_pins[i].owner;
		setMockState(m_pins[i].pin, m_pins[i].mockValue);
	}
}
#endif
