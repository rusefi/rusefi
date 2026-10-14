#include "pch.h"

#include "digital_input_exti.h"
#include "exti_host_adapter.h"
#include "io_pins.h"

namespace {
constexpr uint32_t TestMode = 0x1234;
int firstCallbackData = 1;
int secondCallbackData = 2;

void extiTestCallback(void*, efitick_t) { }
void rejectedExtiTestCallback(void*, efitick_t) { }

void expectOwner(brain_pin_e pin, const char* expected) {
	const char* owner = getBrainUsedPin(brainPin_to_index(pin));
	if (expected == nullptr) {
		EXPECT_EQ(owner, nullptr);
	} else {
		ASSERT_NE(owner, nullptr);
		EXPECT_STREQ(owner, expected);
	}
}

ExtiChannelSnapshotForUnitTest channel(int index) {
	ExtiChannelSnapshotForUnitTest result{};
	efiExtiGetChannelForUnitTest(index, result);
	return result;
}

// #10311: these tests assert current bugs, not the desired behavior.
// A rejected input should leave the existing owner's pin and callback untouched.
// Keep the same setup when changing expectations for the fix.
// https://github.com/rusefi/rusefi/issues/10311
// Host tests record PAL calls, not real pin levels or interrupts.
// Fatal errors throw on the host; firmware shuts down outputs and returns.
// A same-pin conflict therefore stops earlier in these tests.
constexpr brain_pin_e scopedPins[] = { Gpio::A6, Gpio::B6, Gpio::A7 };

class ExtiTestScope {
public:
	ExtiTestScope()
		: adapter(scopedPins, sizeof(scopedPins) / sizeof(scopedPins[0]))
		, ownership() { }

private:
	ScopedExtiAdapterForUnitTest adapter;
	ScopedPinOwnershipForUnitTest ownership;
};
}

TEST(digitalInputExti, issue10311_lineConflictAcquiresRejectedPadThenCleanupClearsIncumbent) {
	ExtiTestScope scope;
	const char* const incumbentName = "issue10311 PA6 incumbent";
	const char* const rejectedName = "issue10311 PB6 rejected";

	ASSERT_EQ(efiExtiEnablePin(incumbentName, Gpio::A6, TestMode, extiTestCallback, &firstCallbackData), 0);
	expectOwner(Gpio::A6, incumbentName);
	auto incumbent = channel(6);
	EXPECT_EQ(incumbent.callback, extiTestCallback);
	EXPECT_EQ(incumbent.callbackData, &firstCallbackData);
	ASSERT_NE(incumbent.name, nullptr);
	EXPECT_STREQ(incumbent.name, incumbentName);
	ASSERT_EQ(efiExtiGetLineEventCountForUnitTest(), 1u);
	auto event = efiExtiGetLineEventForUnitTest(0);
	EXPECT_TRUE(event.enabled);
	EXPECT_EQ(event.mode, TestMode);
	EXPECT_EQ(event.line, 22u); // mock PA6 line

	EXPECT_FATAL_ERROR(efiExtiEnablePin(rejectedName, Gpio::B6, TestMode + 1, rejectedExtiTestCallback, &secondCallbackData));
	expectOwner(Gpio::A6, incumbentName);
	expectOwner(Gpio::B6, rejectedName);
	EXPECT_EQ(efiExtiGetLineEventCountForUnitTest(), 1u);
	incumbent = channel(6);
	EXPECT_EQ(incumbent.callback, extiTestCallback);
	EXPECT_EQ(incumbent.callbackData, &firstCallbackData);
	ASSERT_NE(incumbent.name, nullptr);
	EXPECT_STREQ(incumbent.name, incumbentName);

	// Cleaning up rejected PB6 wrongly clears PA6's callback.
	efiExtiDisablePin(Gpio::B6);
	expectOwner(Gpio::B6, nullptr);
	expectOwner(Gpio::A6, incumbentName);
	EXPECT_EQ(efiExtiGetLineEventCountForUnitTest(), 2u);
	event = efiExtiGetLineEventForUnitTest(1);
	EXPECT_FALSE(event.enabled);
	EXPECT_EQ(event.line, 38u); // mock PB6 line
	EXPECT_NE(event.line, efiExtiGetLineEventForUnitTest(0).line);
	incumbent = channel(6);
	EXPECT_EQ(incumbent.callback, nullptr);
	EXPECT_EQ(incumbent.callbackData, nullptr);
	EXPECT_EQ(incumbent.name, nullptr);
}

TEST(digitalInputExti, issue10311_independentLinesEnableAndTeardownNormally) {
	ExtiTestScope scope;
	ASSERT_EQ(efiExtiEnablePin("issue10311 PA6", Gpio::A6, TestMode, extiTestCallback, &firstCallbackData), 0);
	ASSERT_EQ(efiExtiEnablePin("issue10311 PA7", Gpio::A7, TestMode + 1, rejectedExtiTestCallback, &secondCallbackData), 0);
	expectOwner(Gpio::A6, "issue10311 PA6");
	expectOwner(Gpio::A7, "issue10311 PA7");
	ASSERT_EQ(efiExtiGetLineEventCountForUnitTest(), 2u);
	EXPECT_NE(efiExtiGetLineEventForUnitTest(0).line, efiExtiGetLineEventForUnitTest(1).line);
	EXPECT_EQ(channel(7).callback, rejectedExtiTestCallback);
	EXPECT_EQ(channel(7).callbackData, &secondCallbackData);
	ASSERT_NE(channel(7).name, nullptr);
	EXPECT_STREQ(channel(7).name, "issue10311 PA7");

	efiExtiDisablePin(Gpio::A7);
	expectOwner(Gpio::A7, nullptr);
	EXPECT_EQ(channel(7).callback, nullptr);
	ASSERT_EQ(efiExtiGetLineEventCountForUnitTest(), 3u);
	EXPECT_FALSE(efiExtiGetLineEventForUnitTest(2).enabled);
	EXPECT_EQ(efiExtiGetLineEventForUnitTest(2).line, efiExtiGetLineEventForUnitTest(1).line);
	EXPECT_EQ(channel(6).callback, extiTestCallback);
	EXPECT_EQ(channel(6).callbackData, &firstCallbackData);

	efiExtiDisablePin(Gpio::A6);
	expectOwner(Gpio::A6, nullptr);
	EXPECT_EQ(channel(6).callback, nullptr);
	EXPECT_EQ(channel(6).callbackData, nullptr);
	EXPECT_EQ(channel(6).name, nullptr);
}

TEST(digitalInputExti, issue10311_invalidPinDoesNotClaimOrRecordLineOperation) {
	ExtiTestScope scope;
	for (auto pin : { Gpio::Unassigned, Gpio::Invalid }) {
		EXPECT_EQ(efiExtiEnablePin("issue10311 invalid input", pin, TestMode, extiTestCallback, nullptr), -1);
		efiExtiDisablePin(pin);
	}
	EXPECT_EQ(efiExtiGetLineEventCountForUnitTest(), 0u);
	expectOwner(Gpio::A6, nullptr);
	expectOwner(Gpio::B6, nullptr);
	expectOwner(Gpio::A7, nullptr);
	EXPECT_EQ(channel(6).callback, nullptr);
	EXPECT_EQ(channel(7).callback, nullptr);
}

TEST(digitalInputExti, issue10311_samePhysicalPinDuplicateThenRejectedCleanupDropsOwner) {
	ExtiTestScope scope;
	const char* const ownerName = "issue10311 duplicate-pin incumbent";
	ASSERT_EQ(efiExtiEnablePin(ownerName, Gpio::A6, TestMode, extiTestCallback, &firstCallbackData), 0);

	EXPECT_FATAL_ERROR(efiExtiEnablePin("issue10311 duplicate-pin claimant", Gpio::A6, TestMode + 1, rejectedExtiTestCallback, &secondCallbackData));
	expectOwner(Gpio::A6, ownerName);
	auto incumbent = channel(6);
	EXPECT_EQ(incumbent.callback, extiTestCallback);
	EXPECT_EQ(incumbent.callbackData, &firstCallbackData);
	ASSERT_NE(incumbent.name, nullptr);
	EXPECT_STREQ(incumbent.name, ownerName);
	EXPECT_EQ(efiExtiGetLineEventCountForUnitTest(), 1u);

	// Cleaning up the rejected input wrongly removes the existing owner.
	efiExtiDisablePin(Gpio::A6);
	expectOwner(Gpio::A6, nullptr);
	EXPECT_EQ(channel(6).callback, nullptr);
	EXPECT_EQ(channel(6).callbackData, nullptr);
	EXPECT_EQ(channel(6).name, nullptr);
	EXPECT_EQ(efiExtiGetLineEventCountForUnitTest(), 2u);
}
