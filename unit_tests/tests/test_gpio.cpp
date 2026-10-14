#include "pch.h"

#include "pin_repository.h"

enum class CylinderOutputFamily : size_t {
	Coil,
	TrailingCoil,
	Injector,
	SecondStageInjector,
};

static brain_pin_e cylinderOutputPin(CylinderOutputFamily family, size_t cylinder) {
	return static_cast<brain_pin_e>(static_cast<int>(Gpio::A0) +
		static_cast<size_t>(family) * MAX_CYLINDER_COUNT + cylinder);
}

static void configureCylinderOutputPins(size_t cylinderCount) {
	engineConfiguration->cylindersCount = cylinderCount;
	for (size_t i = 0; i < MAX_CYLINDER_COUNT; i++) {
		engineConfiguration->ignitionPins[i] = cylinderOutputPin(CylinderOutputFamily::Coil, i);
		engineConfiguration->trailingCoilPins[i] = cylinderOutputPin(CylinderOutputFamily::TrailingCoil, i);
		engineConfiguration->injectionPins[i] = cylinderOutputPin(CylinderOutputFamily::Injector, i);
		engineConfiguration->injectionPinsStage2[i] = cylinderOutputPin(CylinderOutputFamily::SecondStageInjector, i);
	}
	engineConfiguration->ignitionPinMode = OM_DEFAULT;
	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void expectPinOwner(brain_pin_e pin, const char* expectedOwner) {
	const char* owner = getBrainUsedPin(brainPin_to_index(pin));
	if (expectedOwner == nullptr) {
		EXPECT_EQ(owner, nullptr);
	} else {
		ASSERT_NE(owner, nullptr);
		EXPECT_STREQ(owner, expectedOwner);
	}
}

class ScopedIssue10311PinCleanup {
public:
	explicit ScopedIssue10311PinCleanup(brain_pin_e pin)
		: m_pin(pin), m_owner(getBrainUsedPin(brainPin_to_index(pin))), m_state(efiReadPin(pin)) {
		efiSetPadUnused(m_pin);
	}
	~ScopedIssue10311PinCleanup() {
		efiSetPadUnused(m_pin);
		getBrainUsedPin(brainPin_to_index(m_pin)) = m_owner;
		setMockState(m_pin, m_state);
	}

private:
	brain_pin_e m_pin;
	const char* m_owner;
	bool m_state;
};

static void establishEmptyCylinderOutputBaseline() {
	configureCylinderOutputPins(0);
	enginePins.updateCylinderOutputPinsForUnitTest();
	rememberCurrentConfiguration();
}

static void updateCylinderOutputPins(size_t cylinderCount) {
	configureCylinderOutputPins(cylinderCount);
	enginePins.updateCylinderOutputPinsForUnitTest();
	rememberCurrentConfiguration();
}

class ScopedCylinderOutputPinCleanup {
public:
	~ScopedCylinderOutputPinCleanup() {
		rememberCurrentConfiguration();
		configureCylinderOutputPins(0);
		enginePins.updateCylinderOutputPinsForUnitTest();
		rememberCurrentConfiguration();
	}
};

TEST(gpio, testPinInitNonInverted) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	OutputPin dut;

	// Set the "hardware" pin to on initially
	setMockState(Gpio::A6, true);
	ASSERT_TRUE(efiReadPin(Gpio::A6));

	// this should turn it off
	pin_output_mode_e mode = OM_DEFAULT;
	dut.initPin("test", Gpio::A6, mode);

	EXPECT_FALSE(efiReadPin(Gpio::A6));
}

TEST(gpio, testPinInitInverted) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	OutputPin dut;

	// Set the "hardware" pin to off initially
	setMockState(Gpio::A6, false);
	ASSERT_FALSE(efiReadPin(Gpio::A6));

	// this should turn it off
	pin_output_mode_e mode = OM_INVERTED;
	dut.initPin("test", Gpio::A6, mode);

	EXPECT_TRUE(efiReadPin(Gpio::A6));
}

TEST(gpio, multipleInit) {
	OutputPin dut;

	// Initial setup should be ok
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", Gpio::A6));

	// Reinit with the same pin should be ok
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", Gpio::A6));

	// Reinit with DIFFERENT pin should fail
	EXPECT_FATAL_ERROR(dut.initPin("testPin", Gpio::B5));
}

TEST(gpio, deInit) {
	OutputPin dut;

	// Initial setup should be ok
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", Gpio::A6));

	dut.deInit();

	// Reinit with DIFFERENT pin should work after deinit
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", Gpio::B5));
}

// #10311: these tests assert current bugs, not the desired behavior.
// A rejected pin user should leave the existing owner untouched.
// Mode 0 tests pin ownership only, not ADC/EXTI hardware setup.
// Keep the same setup when changing expectations for the fix.
// https://github.com/rusefi/rusefi/issues/10311
TEST(gpio, issue10311_duplicateInputInputClaimPreservesOriginalOwner) {
	ScopedPinOwnershipForUnitTest pinOwnership;
	ScopedIssue10311PinCleanup cleanup(Gpio::A6);

	efiSetPadMode("issue10311 original input", Gpio::A6, static_cast<iomode_t>(0));
	EXPECT_FATAL_ERROR(efiSetPadMode("issue10311 duplicate input", Gpio::A6, static_cast<iomode_t>(0)));

	expectPinOwner(Gpio::A6, "issue10311 original input");
}

TEST(gpio, issue10311_inputThenOutputDuplicateClaimClobbersInputAndCleanup) {
	ScopedPinOwnershipForUnitTest pinOwnership;
	ScopedIssue10311PinCleanup cleanup(Gpio::A6);
	OutputPin outputClaimant;

	efiSetPadMode("issue10311 input owner", Gpio::A6, static_cast<iomode_t>(0));
	setMockState(Gpio::A6, true);
	ASSERT_TRUE(efiReadPin(Gpio::A6));
	EXPECT_FATAL_ERROR(outputClaimant.initPin("issue10311 output claimant", Gpio::A6));

	expectPinOwner(Gpio::A6, "issue10311 input owner");
	// The failed output init still changes the input's mock level.
	EXPECT_FALSE(efiReadPin(Gpio::A6));
	// It also remembers the pin it failed to acquire.
	EXPECT_EQ(outputClaimant.brainPin, Gpio::A6);
	outputClaimant.deInit();
	// Cleanup wrongly removes the existing owner's registration.
	expectPinOwner(Gpio::A6, nullptr);
}

TEST(gpio, issue10311_outputThenInputDuplicateClaimRetainsOwnerAndLevel) {
	ScopedPinOwnershipForUnitTest pinOwnership;
	ScopedIssue10311PinCleanup cleanup(Gpio::A6);
	OutputPin outputOwner;

	outputOwner.initPin("issue10311 output owner", Gpio::A6);
	outputOwner.setValue(true);
	ASSERT_TRUE(efiReadPin(Gpio::A6));
	EXPECT_FATAL_ERROR(efiSetPadMode("issue10311 input claimant", Gpio::A6, static_cast<iomode_t>(0)));

	expectPinOwner(Gpio::A6, "issue10311 output owner");
	EXPECT_TRUE(efiReadPin(Gpio::A6));
	outputOwner.deInit();
	expectPinOwner(Gpio::A6, nullptr);
}

TEST(gpio, issue10311_duplicateOutputDefaultWriteAndFailedClaimantCleanup) {
	ScopedPinOwnershipForUnitTest pinOwnership;
	ScopedIssue10311PinCleanup cleanup(Gpio::A6);
	OutputPin originalOwner;
	OutputPin failedClaimant;

	originalOwner.initPin("issue10311 original output", Gpio::A6);
	originalOwner.setValue(true);
	ASSERT_TRUE(efiReadPin(Gpio::A6));

	// Init writes the default level before checking ownership.
	EXPECT_FATAL_ERROR(failedClaimant.initPin("issue10311 duplicate output", Gpio::A6));
	expectPinOwner(Gpio::A6, "issue10311 original output");
	EXPECT_EQ(failedClaimant.brainPin, Gpio::A6);
	EXPECT_FALSE(efiReadPin(Gpio::A6));

	// The failed output can still write the pin and remove its owner's registration.
	failedClaimant.setValue(true);
	EXPECT_TRUE(efiReadPin(Gpio::A6));
	failedClaimant.deInit();
	expectPinOwner(Gpio::A6, nullptr);
	originalOwner.deInit();
}

TEST(gpio, issue10311_sameOutputObjectReinitCurrentlyRaisesDuplicateClaim) {
	ScopedPinOwnershipForUnitTest pinOwnership;
	ScopedIssue10311PinCleanup cleanup(Gpio::A6);
	OutputPin output;

	// Reinitializing the same output wrongly reports a conflict.
	EXPECT_NO_FATAL_ERROR(output.initPin("issue10311 same output", Gpio::A6));
	EXPECT_FATAL_ERROR(output.initPin("issue10311 same output", Gpio::A6));
	expectPinOwner(Gpio::A6, "issue10311 same output");
	EXPECT_EQ(output.brainPin, Gpio::A6);
}

TEST(gpio, pinSetNotInverted) {
	OutputPin dut;

	pin_output_mode_e mode = OM_DEFAULT;
	dut.initPin("test", Gpio::A6, mode);

	// Initial state should be logic 0
	EXPECT_FALSE(efiReadPin(Gpio::A6));

	dut.setValue(true);
	EXPECT_TRUE(efiReadPin(Gpio::A6));
	EXPECT_TRUE(dut.getLogicValue());

	dut.setValue(false);
	EXPECT_FALSE(efiReadPin(Gpio::A6));
	EXPECT_FALSE(dut.getLogicValue());
}

TEST(gpio, pinSetInverted) {
	OutputPin dut;

	pin_output_mode_e mode = OM_INVERTED;
	dut.initPin("test", Gpio::A6, mode);

	// Initial state should be logic 0
	EXPECT_TRUE(efiReadPin(Gpio::A6));

	dut.setValue(true);
	EXPECT_FALSE(efiReadPin(Gpio::A6));
	EXPECT_TRUE(dut.getLogicValue());

	dut.setValue(false);
	EXPECT_TRUE(efiReadPin(Gpio::A6));
	EXPECT_FALSE(dut.getLogicValue());
}

// #10312.
TEST(gpio, cylinderOutputCountIncreaseClaimsConfiguredPins) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ScopedPinOwnershipForUnitTest pinOwnership;
	ScopedCylinderOutputPinCleanup cleanup;
	establishEmptyCylinderOutputBaseline();
	updateCylinderOutputPins(4);

	EXPECT_EQ(engineConfiguration->cylindersCount, 4u);
	for (size_t i = 0; i < MAX_CYLINDER_COUNT; i++) {
		for (auto family : { CylinderOutputFamily::Coil, CylinderOutputFamily::TrailingCoil,
				CylinderOutputFamily::Injector, CylinderOutputFamily::SecondStageInjector }) {
			const auto pin = cylinderOutputPin(family, i);
			ASSERT_TRUE(isBrainPinValid(pin));
			if (family == CylinderOutputFamily::Coil) {
				EXPECT_EQ(engineConfiguration->ignitionPins[i], pin);
			} else if (family == CylinderOutputFamily::TrailingCoil) {
				EXPECT_EQ(engineConfiguration->trailingCoilPins[i], pin);
			} else if (family == CylinderOutputFamily::Injector) {
				EXPECT_EQ(engineConfiguration->injectionPins[i], pin);
			} else {
				EXPECT_EQ(engineConfiguration->injectionPinsStage2[i], pin);
			}
			EXPECT_EQ(getBrainUsedPin(brainPin_to_index(pin)) != nullptr, i < 4);
		}
	}

	// Tune assignments remain configured while cylinder slots five and six are inactive.
	for (size_t i = 4; i < 6; i++) {
		OutputPin anotherConsumer;
		auto pin = cylinderOutputPin(CylinderOutputFamily::Coil, i);
		EXPECT_NO_FATAL_ERROR(anotherConsumer.initPin("inactive pin consumer", pin, OM_DEFAULT));
		expectPinOwner(pin, "inactive pin consumer");
		anotherConsumer.deInit();
		expectPinOwner(pin, nullptr);
	}

	OutputPin conflictOwner;
	auto conflictPin = cylinderOutputPin(CylinderOutputFamily::SecondStageInjector, 5);
	conflictOwner.initPin("conflict owner", conflictPin, OM_DEFAULT);
	configureCylinderOutputPins(6);
	EXPECT_FATAL_ERROR(enginePins.updateCylinderOutputPinsForUnitTest());
	EXPECT_STREQ(getBrainUsedPin(brainPin_to_index(conflictPin)), "conflict owner");
	// OutputPin records its requested pin before the registry reports a duplicate.
	// Clear that failed claimant without disturbing the real owner's registry entry.
	enginePins.injectorsStage2[5].brainPin = Gpio::Unassigned;
	conflictOwner.deInit();
}

TEST(gpio, cylinderOutputCountDecreaseReleasesPins) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ScopedPinOwnershipForUnitTest pinOwnership;
	ScopedCylinderOutputPinCleanup cleanup;
	establishEmptyCylinderOutputBaseline();
	updateCylinderOutputPins(6);
	updateCylinderOutputPins(4);

	for (size_t i = 4; i < 6; i++) {
		for (auto family : { CylinderOutputFamily::Coil, CylinderOutputFamily::TrailingCoil,
				CylinderOutputFamily::Injector, CylinderOutputFamily::SecondStageInjector }) {
			auto pin = cylinderOutputPin(family, i);
			expectPinOwner(pin, nullptr);
		}
	}

	OutputPin anotherConsumer;
	auto releasedPin = cylinderOutputPin(CylinderOutputFamily::Injector, 4);
	EXPECT_NO_FATAL_ERROR(anotherConsumer.initPin("released pin consumer", releasedPin, OM_DEFAULT));
	expectPinOwner(releasedPin, "released pin consumer");
	anotherConsumer.deInit();
	expectPinOwner(releasedPin, nullptr);
}

TEST(gpio, cylinderOutputCountRoundTripRetainsOutputState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ScopedPinOwnershipForUnitTest pinOwnership;
	ScopedCylinderOutputPinCleanup cleanup;
	establishEmptyCylinderOutputBaseline();
	updateCylinderOutputPins(4);

	enginePins.coils[0].setHigh();
	const auto retainedToggleCount = enginePins.coils[0].pinToggleCounter;
	updateCylinderOutputPins(6);
	EXPECT_EQ(enginePins.coils[0].pinToggleCounter, retainedToggleCount);
	EXPECT_TRUE(enginePins.coils[0].getLogicValue());
	for (size_t i = 0; i < 6; i++) {
		EXPECT_EQ(enginePins.coils[i].brainPin, cylinderOutputPin(CylinderOutputFamily::Coil, i));
		EXPECT_EQ(enginePins.trailingCoils[i].brainPin, cylinderOutputPin(CylinderOutputFamily::TrailingCoil, i));
		EXPECT_EQ(enginePins.injectors[i].brainPin, cylinderOutputPin(CylinderOutputFamily::Injector, i));
		EXPECT_EQ(enginePins.injectorsStage2[i].brainPin, cylinderOutputPin(CylinderOutputFamily::SecondStageInjector, i));
		expectPinOwner(cylinderOutputPin(CylinderOutputFamily::Coil, i), enginePins.coils[i].getName());
		expectPinOwner(cylinderOutputPin(CylinderOutputFamily::TrailingCoil, i), enginePins.trailingCoils[i].getName());
		expectPinOwner(cylinderOutputPin(CylinderOutputFamily::Injector, i), enginePins.injectors[i].getName());
		expectPinOwner(cylinderOutputPin(CylinderOutputFamily::SecondStageInjector, i), enginePins.injectorsStage2[i].getName());
	}
	updateCylinderOutputPins(4);
	EXPECT_EQ(enginePins.coils[0].pinToggleCounter, retainedToggleCount);
	EXPECT_TRUE(enginePins.coils[0].getLogicValue());
	for (size_t i = 4; i < 6; i++) {
		EXPECT_EQ(enginePins.coils[i].brainPin, Gpio::Unassigned);
		EXPECT_EQ(enginePins.trailingCoils[i].brainPin, Gpio::Unassigned);
		EXPECT_EQ(enginePins.injectors[i].brainPin, Gpio::Unassigned);
		EXPECT_EQ(enginePins.injectorsStage2[i].brainPin, Gpio::Unassigned);
		for (auto family : { CylinderOutputFamily::Coil, CylinderOutputFamily::TrailingCoil,
				CylinderOutputFamily::Injector, CylinderOutputFamily::SecondStageInjector }) {
			expectPinOwner(cylinderOutputPin(family, i), nullptr);
		}
	}
}

TEST(gpio, cylinderOutputCountAndPinChange) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ScopedPinOwnershipForUnitTest pinOwnership;
	ScopedCylinderOutputPinCleanup cleanup;
	establishEmptyCylinderOutputBaseline();
	updateCylinderOutputPins(6);

	configureCylinderOutputPins(4);
	const auto originalCoil0 = engineConfiguration->ignitionPins[0];
	engineConfiguration->ignitionPins[0] = cylinderOutputPin(CylinderOutputFamily::Coil, 1);
	engineConfiguration->ignitionPins[1] = originalCoil0;
	// Each third output takes the pin released by its newly inactive fifth output.
	engineConfiguration->ignitionPins[2] = cylinderOutputPin(CylinderOutputFamily::Coil, 4);
	engineConfiguration->trailingCoilPins[2] = cylinderOutputPin(CylinderOutputFamily::TrailingCoil, 4);
	engineConfiguration->injectionPins[2] = cylinderOutputPin(CylinderOutputFamily::Injector, 4);
	engineConfiguration->injectionPinsStage2[2] = cylinderOutputPin(CylinderOutputFamily::SecondStageInjector, 4);
	EXPECT_NO_FATAL_ERROR(enginePins.updateCylinderOutputPinsForUnitTest());
	rememberCurrentConfiguration();

	EXPECT_EQ(enginePins.coils[0].brainPin, cylinderOutputPin(CylinderOutputFamily::Coil, 1));
	expectPinOwner(cylinderOutputPin(CylinderOutputFamily::Coil, 0), enginePins.coils[1].getName());
	expectPinOwner(cylinderOutputPin(CylinderOutputFamily::Coil, 1), enginePins.coils[0].getName());

	EXPECT_EQ(enginePins.coils[2].brainPin, cylinderOutputPin(CylinderOutputFamily::Coil, 4));
	EXPECT_EQ(enginePins.trailingCoils[2].brainPin, cylinderOutputPin(CylinderOutputFamily::TrailingCoil, 4));
	EXPECT_EQ(enginePins.injectors[2].brainPin, cylinderOutputPin(CylinderOutputFamily::Injector, 4));
	EXPECT_EQ(enginePins.injectorsStage2[2].brainPin, cylinderOutputPin(CylinderOutputFamily::SecondStageInjector, 4));
	expectPinOwner(cylinderOutputPin(CylinderOutputFamily::Coil, 4), enginePins.coils[2].getName());
	expectPinOwner(cylinderOutputPin(CylinderOutputFamily::TrailingCoil, 4), enginePins.trailingCoils[2].getName());
	expectPinOwner(cylinderOutputPin(CylinderOutputFamily::Injector, 4), enginePins.injectors[2].getName());
	expectPinOwner(cylinderOutputPin(CylinderOutputFamily::SecondStageInjector, 4), enginePins.injectorsStage2[2].getName());
	for (auto family : { CylinderOutputFamily::Coil, CylinderOutputFamily::TrailingCoil,
			CylinderOutputFamily::Injector, CylinderOutputFamily::SecondStageInjector }) {
		expectPinOwner(cylinderOutputPin(family, 2), nullptr);
		expectPinOwner(cylinderOutputPin(family, 5), nullptr);
	}
	for (size_t i = 4; i < 6; i++) {
		EXPECT_EQ(enginePins.coils[i].brainPin, Gpio::Unassigned);
		EXPECT_EQ(enginePins.trailingCoils[i].brainPin, Gpio::Unassigned);
		EXPECT_EQ(enginePins.injectors[i].brainPin, Gpio::Unassigned);
		EXPECT_EQ(enginePins.injectorsStage2[i].brainPin, Gpio::Unassigned);
	}
}
