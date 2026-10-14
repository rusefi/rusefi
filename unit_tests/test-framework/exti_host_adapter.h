#pragma once

#include "digital_input_exti.h"

#if EFI_UNIT_TEST
using ioline_t = uint32_t;

struct ExtiChannelSnapshotForUnitTest {
	ExtiCallback callback;
	void* callbackData;
	const char* name;
};

void efiExtiGetChannelForUnitTest(size_t index, ExtiChannelSnapshotForUnitTest& result);
void efiExtiSnapshotChannelsForUnitTest(ExtiChannelSnapshotForUnitTest* channels);
void efiExtiResetChannelsForUnitTest();
void efiExtiRestoreChannelsForUnitTest(const ExtiChannelSnapshotForUnitTest* channels);

struct ExtiLineEventForUnitTest {
	uint32_t line;
	uint32_t mode;
	bool enabled;
};

void efiExtiRecordLineEventForUnitTest(uint32_t line, uint32_t mode, bool enabled);
void efiExtiResetLineEventsForUnitTest();
size_t efiExtiGetLineEventCountForUnitTest();
ExtiLineEventForUnitTest efiExtiGetLineEventForUnitTest(size_t index);
void efiExtiRestoreLineEventsForUnitTest(const ExtiLineEventForUnitTest* events, size_t count);

ioportid_t getHwPort(const char* msg, brain_pin_e brainPin);
ioportmask_t getHwPin(const char* msg, brain_pin_e brainPin);
ioline_t efiExtiMakeLineForUnitTest(ioportid_t port, int index);
void palEnableLineEvent(ioline_t line, uint32_t mode);
void palDisableLineEvent(ioline_t line);

#define PAL_LINE(port, index) efiExtiMakeLineForUnitTest((port), (index))
#define PAL_MODE_INPUT 0

class ScopedExtiAdapterForUnitTest {
public:
	ScopedExtiAdapterForUnitTest(const brain_pin_e* pins, size_t pinCount);
	~ScopedExtiAdapterForUnitTest();

	ScopedExtiAdapterForUnitTest(const ScopedExtiAdapterForUnitTest&) = delete;
	ScopedExtiAdapterForUnitTest& operator=(const ScopedExtiAdapterForUnitTest&) = delete;

private:
	static constexpr size_t MaxPins = 4;
	struct PinState {
		brain_pin_e pin;
		const char* owner;
		bool mockValue;
	};

	ExtiChannelSnapshotForUnitTest m_channels[16];
	ExtiLineEventForUnitTest m_events[32];
	size_t m_eventCount;
	PinState m_pins[MaxPins];
	size_t m_pinCount;
};
#endif
