/**
 * @file	efi_gpio.cpp
 * @brief	EFI-related GPIO code
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "bench_test.h"
#include "engine_sniffer.h"

#include "drivers/gpio/gpio_ext.h"

#if HW_HELLEN
#include "hellen_all_meta.h"
#endif // HW_HELLEN

#if EFI_ELECTRONIC_THROTTLE_BODY
#include "electronic_throttle.h"
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

// todo: clean this mess, this should become 'static'/private
EnginePins enginePins;

static const char* const sparkNames[] = { "Coil 1", "Coil 2", "Coil 3", "Coil 4", "Coil 5", "Coil 6", "Coil 7", "Coil 8",
		"Coil 9", "Coil 10", "Coil 11", "Coil 12"};

static const char* const trailNames[] = { "Trail 1", "Trail 2", "Trail 3", "Trail 4", "Trail 5", "Trail 6", "Trail 7", "Trail 8",
		"Trail 9", "Trail 10", "Trail 11", "Trail 12"};

static const char* const trailShortNames[] = { "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "rA", "rB", "rD" };

const char *vvtNames[] = {
		"VVT1",
		"VVT2",
		"VVT3",
		"VVT4"};

const char *laNames[] = {
		"input1",
		"input2",
		"input3",
		"input4"};

// these short names are part of engine sniffer protocol
static const char* const sparkShortNames[] = { PROTOCOL_COIL_SHORT_PREFIX "1", PROTOCOL_COIL_SHORT_PREFIX "2", "c3", "c4", "c5", "c6", "c7", "c8",
		"c9", "cA", "cB", "cD"};

static const char* const injectorNames[] = { "Injector 1", "Injector 2", "Injector 3", "Injector 4", "Injector 5", "Injector 6",
		"Injector 7", "Injector 8", "Injector 9", "Injector 10", "Injector 11", "Injector 12"};

static const char* const injectorShortNames[] = { PROTOCOL_INJ_SHORT_PREFIX "1", PROTOCOL_INJ_SHORT_PREFIX "2", "i3", "i4", "i5", "i6", "i7", "i8",
		"i9", "iA", "iB", "iC"};

static const char* const injectorStage2Names[] = { "Injector Second Stage 1", "Injector Second Stage 2", "Injector Second Stage 3", "Injector Second Stage 4", "Injector Second Stage 5", "Injector Second Stage 6",
		"Injector Second Stage 7", "Injector Second Stage 8", "Injector Second Stage 9", "Injector Second Stage 10", "Injector Second Stage 11", "Injector Second Stage 12"};

static const char* const injectorStage2ShortNames[] = { PROTOCOL_INJ_STAGE2_SHORT_PREFIX "1", PROTOCOL_INJ_STAGE2_SHORT_PREFIX "2", "j3", "j4", "j5", "j6", "j7", "j8",
		"j9", "jA", "jB", "jC"};

static const char* const auxValveShortNames[] = { "a1", "a2"};

static RegisteredOutputPin * registeredOutputHead = nullptr;

RegisteredNamedOutputPin::RegisteredNamedOutputPin(const char *p_name, size_t pinOffset,
		size_t pinModeOffset) : RegisteredOutputPin(p_name, pinOffset, pinModeOffset) {
}

RegisteredNamedOutputPin::RegisteredNamedOutputPin(const char *p_name, size_t pinOffset) :
    RegisteredOutputPin(p_name, pinOffset) {
}

RegisteredOutputPin::RegisteredOutputPin(const char *p_registrationName, size_t pinOffset,
		size_t pinModeOffset)
	: next(registeredOutputHead)
	, registrationName(p_registrationName)
	, m_pinOffset(static_cast<uint16_t>(pinOffset))
	, m_hasPinMode(true)
	, m_pinModeOffset(static_cast<uint16_t>(pinModeOffset))
{
	// adding into head of the list is so easy and since we do not care about order that's what we shall do
	registeredOutputHead = this;
}

RegisteredOutputPin::RegisteredOutputPin(const char *p_registrationName, size_t pinOffset)
	: next(registeredOutputHead)
	, registrationName(p_registrationName)
	, m_pinOffset(static_cast<uint16_t>(pinOffset))
	, m_hasPinMode(false)
	, m_pinModeOffset(-1)
{
	// adding into head of the list is so easy and since we do not care about order that's what we shall do
	registeredOutputHead = this;
}

bool RegisteredOutputPin::isPinConfigurationChanged() {
	brain_pin_e        curPin = *(brain_pin_e       *) ((void *) (&((char*)&activeConfiguration)[m_pinOffset]));
	brain_pin_e        newPin = *(brain_pin_e       *) ((void *) (&((char*) engineConfiguration)[m_pinOffset]));
	bool pinChanged = curPin != newPin;

	if (!m_hasPinMode) {
		return pinChanged;
	}

	pin_output_mode_e curMode = *(pin_output_mode_e *) ((void *) (&((char*)&activeConfiguration)[m_pinModeOffset]));
	pin_output_mode_e newMode = *(pin_output_mode_e *) ((void *) (&((char*) engineConfiguration)[m_pinModeOffset]));
	return pinChanged || curMode != newMode;
}

void RegisteredOutputPin::init() {
	brain_pin_e        newPin = *(brain_pin_e       *) ((void *) (&((char*) engineConfiguration)[m_pinOffset]));

	pin_output_mode_e newMode;
	if (m_hasPinMode) {
		newMode = *(pin_output_mode_e *) ((void *) (&((char*) engineConfiguration)[m_pinModeOffset]));
	} else {
		newMode = OM_DEFAULT;
	}

    if (isPinConfigurationChanged()) {
		this->initPin(registrationName, newPin, newMode);
    }
}

void RegisteredOutputPin::unregister() {
	if (isPinConfigurationChanged()) {
		OutputPin::deInit();
	}
}

#define CONFIG_OFFSET(x) (offsetof(engine_configuration_s, x))
// todo: pin and pinMode should be combined into a composite entity
// todo: one of the impediments is code generator hints handling (we need custom hints and those are not handled nice for fields of structs?)
#define CONFIG_PIN_OFFSETS(x) CONFIG_OFFSET(x##Pin), CONFIG_OFFSET(x##PinMode)

// offset of X within engineConfiguration, plus offset of Y within X
// decltype(engine_configuration_s::x) resolves the typename of the struct X inside engineConfiguration
#define CONFIG_OFFSET2(x, y) (offsetof(engine_configuration_s, x) + offsetof(decltype(engine_configuration_s::x), y))
#define CONFIG_PIN_OFFSETS2(x, y) CONFIG_OFFSET2(x, y##Pin), CONFIG_OFFSET2(x, y##PinMode)

EnginePins::EnginePins() :
 // [tag:coding_by_convention] 'mainRelay' member here uses 'mainRelayPin' and 'mainRelayPinMode' configuration fields
		mainRelay("Main Relay", CONFIG_PIN_OFFSETS(mainRelay)),
		hpfpValve("HPFP Valve", CONFIG_PIN_OFFSETS(hpfpValve)),
		starterControl("Starter Relay", CONFIG_PIN_OFFSETS(starterControl)),
		starterRelayDisable("Starter Disable Relay", CONFIG_PIN_OFFSETS(starterRelayDisable)),
		fanRelay("Fan Relay", CONFIG_PIN_OFFSETS(fan)),
		fanRelay2("Fan Relay 2", CONFIG_PIN_OFFSETS(fan2)),
		acRelay("A/C Relay", CONFIG_PIN_OFFSETS(acRelay)),
		fuelPumpRelay("Fuel pump Relay", CONFIG_PIN_OFFSETS(fuelPump)),
		nitrousRelay("Nitrous Relay", CONFIG_PIN_OFFSETS(nitrousRelay)),
		vvlRelay("VVL Relay", CONFIG_PIN_OFFSETS(vvlRelay)),
#if EFI_HD_ACR
		harleyAcr("Harley ACR", CONFIG_OFFSET(acrPin)),
		harleyAcr2("Harley ACR 2", CONFIG_OFFSET(acrPin2)),
#endif // EFI_HD_ACR
		boostPin("Boost", CONFIG_PIN_OFFSETS(boostControl)),
		idleSolenoidPin("Idle Valve", CONFIG_OFFSET2(idle, solenoidPin), CONFIG_OFFSET2(idle, solenoidPinMode)),
		secondIdleSolenoidPin("Idle Valve#2", CONFIG_OFFSET(secondSolenoidPin), CONFIG_OFFSET2(idle, solenoidPinMode)),
		alternatorPin("Alternator control", CONFIG_PIN_OFFSETS(alternatorControl)),
		checkEnginePin("checkEnginePin", CONFIG_PIN_OFFSETS(malfunctionIndicator)),
		tachOut("tachOut", CONFIG_PIN_OFFSETS(tachOutput)),
		triggerDecoderErrorPin("led: trigger debug", CONFIG_PIN_OFFSETS(triggerError)),
		speedoOut("speedoOut", CONFIG_OFFSET(speedometerOutputPin))
{
	hpfpValve.setName("hpfp");
#if EFI_HD_ACR
	harleyAcr.setName("acr");
#endif // EFI_HD_ACR

	static_assert(efi::size(sparkNames) >= MAX_CYLINDER_COUNT, "Too many ignition pins");
	static_assert(efi::size(trailNames) >= MAX_CYLINDER_COUNT, "Too many ignition pins");
	static_assert(efi::size(injectorNames) >= MAX_CYLINDER_COUNT, "Too many injection pins");
	for (int i = 0; i < MAX_CYLINDER_COUNT;i++) {
		enginePins.coils[i].coilIndex = i;
		enginePins.coils[i].setName(sparkNames[i]);
		enginePins.coils[i].shortName = sparkShortNames[i];

		enginePins.trailingCoils[i].setName(trailNames[i]);
		enginePins.trailingCoils[i].shortName = trailShortNames[i];

		enginePins.injectors[i].injectorIndex = i;
		enginePins.injectors[i].setName(injectorNames[i]);
		enginePins.injectors[i].shortName = injectorShortNames[i];

		enginePins.injectorsStage2[i].injectorIndex = i;
		enginePins.injectorsStage2[i].setName(injectorStage2Names[i]);
		enginePins.injectorsStage2[i].shortName = injectorStage2ShortNames[i];
	}

	static_assert(efi::size(auxValveShortNames) >= AUX_DIGITAL_VALVE_COUNT, "Too many aux valve pins");
	for (int i = 0; i < AUX_DIGITAL_VALVE_COUNT;i++) {
		enginePins.auxValve[i].setName(auxValveShortNames[i]);
	}
}

/**
 * Sets the value of the pin. On this layer the value is assigned as is, without any conversion.
 */

#define unregisterOutputIfPinChanged(output, pin) {                                \
	if (isConfigurationChanged(pin)) {                                             \
		(output).deInit();                        \
	}                                                                              \
}

#define unregisterOutputIfPinOrModeChanged(output, pin, mode) {                    \
	if (isPinOrModeChanged(pin, mode)) {                                           \
		(output).deInit();                        \
	}                                                                              \
}

bool EnginePins::stopPins() {
	bool result = false;
	for (int i = 0; i < MAX_CYLINDER_COUNT; i++) {
		result |= coils[i].stop();
		result |= injectors[i].stop();
		result |= injectorsStage2[i].stop();
		result |= trailingCoils[i].stop();
	}
	for (int i = 0; i < AUX_DIGITAL_VALVE_COUNT; i++) {
		result |= auxValve[i].stop();
	}
	return result;
}

void EnginePins::unregisterPins() {
	stopInjectionPins();
	stopIgnitionPins();
#if EFI_AUX_VALVES
	stopAuxValves();
#endif

#if EFI_ELECTRONIC_THROTTLE_BODY
	unregisterEtbPins();
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

	// todo: add pinMode
	unregisterOutputIfPinChanged(sdCsPin, sdCardCsPin);
	unregisterOutputIfPinChanged(accelerometerCs, accelerometerCsPin);

	RegisteredOutputPin * pin = registeredOutputHead;
	while (pin != nullptr) {
		pin->unregister();
		pin = pin->next;
	}
}

void EnginePins::debug() {
	RegisteredOutputPin * pin = registeredOutputHead;
	while (pin != nullptr) {
		efiPrintf("%s %d", pin->getRegistrationName(), pin->currentLogicValue);
		pin = pin->next;
	}
}

void EnginePins::startPins() {
#if EFI_ENGINE_CONTROL
	startInjectionPins();
	startIgnitionPins();
#endif /* EFI_ENGINE_CONTROL */

#if EFI_AUX_VALVES
	startAuxValves();
#endif // EFI_AUX_VALVES

	RegisteredOutputPin * pin = registeredOutputHead;
	while (pin != nullptr) {
		pin->init();
		pin = pin->next;
	}
}

void EnginePins::reset() {
	for (int i = 0; i < MAX_CYLINDER_COUNT;i++) {
		injectors[i].reset();
		coils[i].reset();
		trailingCoils[i].reset();
	}
}

void EnginePins::stopIgnitionPins() {
	for (int i = 0; i < MAX_CYLINDER_COUNT; i++) {
		unregisterOutputIfPinOrModeChanged(enginePins.coils[i], ignitionPins[i], ignitionPinMode);
		unregisterOutputIfPinOrModeChanged(enginePins.trailingCoils[i], trailingCoilPins[i], ignitionPinMode);
	}
}

void EnginePins::stopInjectionPins() {
	for (int i = 0; i < MAX_CYLINDER_COUNT; i++) {
		unregisterOutputIfPinOrModeChanged(enginePins.injectors[i], injectionPins[i], injectionPinMode);
		unregisterOutputIfPinOrModeChanged(enginePins.injectorsStage2[i], injectionPinsStage2[i], injectionPinMode);
	}
}

#if EFI_AUX_VALVES
void EnginePins::stopAuxValves() {
	for (int i = 0; i < AUX_DIGITAL_VALVE_COUNT; i++) {
		NamedOutputPin *output = &enginePins.auxValve[i];
		// todo: do we need auxValveMode and reuse code?
		if (isConfigurationChanged(auxValves[i])) {
			(output)->deInit();
		}
	}
}

void EnginePins::startAuxValves() {
#if EFI_PROD_CODE
	for (int i = 0; i < AUX_DIGITAL_VALVE_COUNT; i++) {
		NamedOutputPin *output = &enginePins.auxValve[i];
		// todo: do we need auxValveMode and reuse code?
		if (isConfigurationChanged(auxValves[i])) {
			output->initPin(output->getName(), engineConfiguration->auxValves[i]);
		}
	}
#endif /* EFI_PROD_CODE */
}
#endif // EFI_AUX_VALVES

void EnginePins::startIgnitionPins() {
#if EFI_PROD_CODE
	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		NamedOutputPin *trailingOutput = &enginePins.trailingCoils[i];
		if (isPinOrModeChanged(trailingCoilPins[i], ignitionPinMode)) {
			trailingOutput->initPin(trailingOutput->getName(), engineConfiguration->trailingCoilPins[i], engineConfiguration->ignitionPinMode);
		}

		NamedOutputPin *output = &enginePins.coils[i];
		if (isPinOrModeChanged(ignitionPins[i], ignitionPinMode)) {
			output->initPin(output->getName(), engineConfiguration->ignitionPins[i], engineConfiguration->ignitionPinMode);
		}
	}
#endif /* EFI_PROD_CODE */
}

void EnginePins::startInjectionPins() {
#if EFI_PROD_CODE
	// todo: should we move this code closer to the injection logic?
	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		NamedOutputPin *output = &enginePins.injectors[i];
		if (isPinOrModeChanged(injectionPins[i], injectionPinMode)) {
			output->initPin(output->getName(), engineConfiguration->injectionPins[i],
					engineConfiguration->injectionPinMode);
		}

		output = &enginePins.injectorsStage2[i];
		if (isPinOrModeChanged(injectionPinsStage2[i], injectionPinMode)) {
			output->initPin(output->getName(), engineConfiguration->injectionPinsStage2[i],
					engineConfiguration->injectionPinMode);
		}
	}
#endif /* EFI_PROD_CODE */
}

OutputPin *EnginePins::getOutputPinForBenchMode(bench_mode_e index) {
	switch(index) {
#if EFI_VVT_PID
	case BENCH_VVT0_VALVE:
		return getVvtOutputPin(0);
	case BENCH_VVT1_VALVE:
		return getVvtOutputPin(1);
	case BENCH_VVT2_VALVE:
		return getVvtOutputPin(2);
	case BENCH_VVT3_VALVE:
		return getVvtOutputPin(3);
#endif // EFI_VVT_PID
	case BENCH_MAIN_RELAY:
		return &mainRelay;
	case BENCH_HPFP_VALVE:
		return &hpfpValve;
	case BENCH_FUEL_PUMP:
		return &fuelPumpRelay;
	case BENCH_STARTER_ENABLE_RELAY:
		return &starterControl;
	case BENCH_CHECK_ENGINE_LIGHT:
		return &checkEnginePin;
	case BENCH_AC_COMPRESSOR_RELAY:
		return &acRelay;
	case BENCH_FAN_RELAY:
		return &fanRelay;
#if EFI_HD_ACR
	case HD_ACR:
		return &harleyAcr;
	case HD_ACR2:
		return &harleyAcr2;
#endif
	case BENCH_IDLE_VALVE:
		return &idleSolenoidPin;
	case BENCH_FAN_RELAY_2:
		return &fanRelay;
	default:
		criticalError("Unexpected bench pin %d", index);
	}
	return nullptr;
}

#if EFI_UNIT_TEST
/*
* this function goes through the whole pin repository and sets them all to "GPIO::Unassigned",
* this is done as a clean-up for testing, since several motor configurations can have conflicting pins
* at the same time the productive de-init uses "isPinConfigurationChanged" to reset only the pins that have been changed,
* so in order for it to be properly de-initialized as it is done in prod, all pins are re-configured as unassigned,
* previously unused pins by tests will not be de-initialized since the configuration on them will be the same (Unassigned => Unassigned)
*/
void EnginePins::resetForUnitTest() {
	RegisteredOutputPin * pin = registeredOutputHead;
	while (pin != nullptr) {
		pin->brainPin = Gpio::Unassigned;
		pin = pin->next;
	}
}
#endif

NamedOutputPin::NamedOutputPin() : OutputPin() {
}

NamedOutputPin::NamedOutputPin(const char *p_name) : OutputPin() {
	name = p_name;
}

const char *NamedOutputPin::getName() const {
	return name;
}

void NamedOutputPin::setName(const char* p_name) {
	name = p_name;
}

const char *NamedOutputPin::getShortName() const {
	return shortName == nullptr ? name : shortName;
}

#if EFI_UNIT_TEST
extern bool verboseMode;
#endif // EFI_UNIT_TEST

void NamedOutputPin::setHigh() {
    setHigh(nullptr);
}

void NamedOutputPin::setHigh(const char *msg) {
#if EFI_UNIT_TEST
	if (verboseMode) {
		efiPrintf("pin %s goes high", name);
	}
#endif // EFI_UNIT_TEST
#if EFI_DETAILED_LOGGING
//	signal->hi_time = hTimeNow();
#endif /* EFI_DETAILED_LOGGING */

	// turn the output level ACTIVE
	setValue(msg, true);

#if EFI_ENGINE_SNIFFER
    addEngineSnifferOutputPinEvent(this, FrontDirection::UP);
#endif /* EFI_ENGINE_SNIFFER */
}

void NamedOutputPin::setLow() {
    setLow(nullptr);
}

void NamedOutputPin::setLow(const char *msg) {
#if EFI_UNIT_TEST
	if (verboseMode) {
		efiPrintf("pin %s goes low", name);
	}
#endif // EFI_UNIT_TEST

	// turn off the output
	setValue(msg, false);

#if EFI_ENGINE_SNIFFER
	addEngineSnifferOutputPinEvent(this, FrontDirection::DOWN);
#endif /* EFI_ENGINE_SNIFFER */
}

bool NamedOutputPin::stop() {
#if EFI_GPIO_HARDWARE
	if (isInitialized() && getLogicValue()) {
		setValue("stop", false);
		efiPrintf("turning off %s", name);
		return true;
	}
#endif /* EFI_GPIO_HARDWARE */
	return false;
}

void InjectorOutputPin::reset() {
	// If this injector was open, close it and reset state
	if (overlappingCounter != 0) {
		overlappingCounter = 0;
		setValue("reset", 0);
	}

	// todo: this could be refactored by calling some super-reset method
	currentLogicValue = 0;
}

IgnitionOutputPin::IgnitionOutputPin() {
	reset();
}

void IgnitionOutputPin::setHigh() {
	NamedOutputPin::setHigh();
	// this is NASTY but what's the better option? bytes? At cost of 22 extra bytes in output status packet?
	switch (coilIndex) {
	case 0:
		engine->outputChannels.coilState1 = true;
		break;
	case 1:
		engine->outputChannels.coilState2 = true;
		break;
	case 2:
		engine->outputChannels.coilState3 = true;
		break;
	case 3:
		engine->outputChannels.coilState4 = true;
		break;
	case 4:
		engine->outputChannels.coilState5 = true;
		break;
	case 5:
		engine->outputChannels.coilState6 = true;
		break;
	}
}

void IgnitionOutputPin::setLow() {
	NamedOutputPin::setLow();
	// this is NASTY but what's the better option? bytes? At cost of 22 extra bytes in output status packet?
	switch (coilIndex) {
	case 0:
		engine->outputChannels.coilState1 = false;
		break;
	case 1:
		engine->outputChannels.coilState2 = false;
		break;
	case 2:
		engine->outputChannels.coilState3 = false;
		break;
	case 3:
		engine->outputChannels.coilState4 = false;
		break;
	case 4:
		engine->outputChannels.coilState5 = false;
		break;
	case 5:
		engine->outputChannels.coilState6 = false;
		break;
	}
}

void IgnitionOutputPin::reset() {
	signalFallSparkId = 0;
	currentLogicValue = INITIAL_PIN_STATE;
}

bool OutputPin::isInitialized() const {
#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
#if (BOARD_EXT_GPIOCHIPS > 0)
	if (ext)
		return true;
#endif /* (BOARD_EXT_GPIOCHIPS > 0) */
	return m_port != NULL;
#else /* EFI_GPIO_HARDWARE */
	return true;
#endif /* EFI_GPIO_HARDWARE */
}

void OutputPin::toggle() {
	setValue("toggle", !getLogicValue());
}

bool OutputPin::getAndSet(int logicValue) {
	bool oldValue = getLogicValue();
	setValue(logicValue);
	return oldValue;
}

// This function is only used on real hardware
#if EFI_PROD_CODE
void OutputPin::setOnchipValue(int electricalValue) {
	if (brainPin == Gpio::Unassigned || brainPin == Gpio::Invalid) {
	    // todo: make 'setOnchipValue' or 'reportsetOnchipValueError' virtual and override for NamedOutputPin?
		warning(ObdCode::CUSTOM_ERR_6586, "attempting to change unassigned pin");
		return;
	}
	palWritePad(m_port, m_pin, electricalValue);
}
#endif // EFI_PROD_CODE

void OutputPin::setValue(int logicValue, bool isForce) {
    setValue(nullptr, logicValue, isForce);
}

#if EFI_SIMULATOR
void OutputPin::resetToggleStats() {
	durationsInStateMs[0] = durationsInStateMs[1] = 0;
	pinToggleCounter = 0;
}
#endif // EFI_SIMULATOR

void OutputPin::setValue(const char *msg, int logicValue, bool isForce) {
    UNUSED(msg);
    if ((isHwQcMode() || getOutputOnTheBenchTest() == this) && !isForce) {
        return;
    }

#if ENABLE_PERF_TRACE
// todo: https://github.com/rusefi/rusefi/issues/1638
//	ScopePerf perf(PE::OutputPinSetValue);
#endif // ENABLE_PERF_TRACE

#if EFI_UNIT_TEST
    if (currentLogicValue != logicValue) {
	    pinToggleCounter++;
	}
#endif // EFI_UNIT_TEST

#if EFI_SIMULATOR
    if (currentLogicValue != logicValue) {
	    if (pinToggleCounter > 0) {
	        durationsInStateMs[0] = durationsInStateMs[1];
            durationsInStateMs[1] = pinToggleTimer.getElapsedUs() / 1000;
	    }
	    pinToggleCounter++;
	    pinToggleTimer.reset();
	}
#endif // EFI_SIMULATOR

#if EFI_UNIT_TEST
	if (verboseMode) {
		efiPrintf("pin goes %d", logicValue);
	}
#endif // EFI_UNIT_TEST

	// Always store the current logical value of the pin (so it can be
	// used internally even if not connected to a real hardware pin)
	currentLogicValue = logicValue;

	// Nothing else to do if not configured
	if (!isBrainPinValid(brainPin)) {
		return;
	}

	efiAssertVoid(ObdCode::CUSTOM_ERR_6622, mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e");
	int electricalValue = getElectricalValue(logicValue, mode);

#if EFI_PROD_CODE
	#if (BOARD_EXT_GPIOCHIPS > 0)
		if (!this->ext) {
			setOnchipValue(electricalValue);
		} else {
			/* external pin */
			gpiochips_writePad(this->brainPin, logicValue);
			/* TODO: check return value */
		}
	#else
		setOnchipValue(electricalValue);
	#endif
#else /* EFI_PROD_CODE */
	setMockState(brainPin, electricalValue);
#endif /* EFI_PROD_CODE */
}

bool OutputPin::getLogicValue() const {
	// Compare against 1 since it could also be INITIAL_PIN_STATE (which means logical 0, but we haven't initialized the pin yet)
	return currentLogicValue == 1;
}

void OutputPin::setDefaultPinState(pin_output_mode_e outputMode) {
	assertOMode(mode);
	this->mode = outputMode;
	setValue(false, /*force*/true); // initial state
}

brain_pin_diag_e OutputPin::getDiag() const {
#if EFI_PROD_CODE
#if BOARD_EXT_GPIOCHIPS > 0
	if (!brain_pin_is_onchip(brainPin)) {
		return gpiochips_getDiag(brainPin);
	}
#endif
#endif /* EFI_PROD_CODE */
	// TODO: add hook to board code for custom diagnostic, like it is done on S105
	return PIN_UNKNOWN;
}

void initMiscOutputPins() {
#if EFI_GPIO_HARDWARE

#if HAL_USE_SPI
	enginePins.sdCsPin.initPin("SD CS", engineConfiguration->sdCardCsPin);
#endif /* HAL_USE_SPI */

#if EFI_SHAFT_POSITION_INPUT
	// todo: migrate remaining OutputPin to RegisteredOutputPin in order to get consistent dynamic pin init/deinit
	enginePins.debugTriggerSync.initPin("debug: sync", engineConfiguration->debugTriggerSync);
#endif // EFI_SHAFT_POSITION_INPUT

	enginePins.o2heater.initPin("O2 heater", engineConfiguration->o2heaterPin);

#endif /* EFI_GPIO_HARDWARE */
}

void OutputPin::initPin(const char *p_msg, brain_pin_e p_brainPin) {
	initPin(p_msg, p_brainPin, OM_DEFAULT);
}

void OutputPin::initPin(const char *msg, brain_pin_e p_brainPin, pin_output_mode_e outputMode, bool forceInitWithFatalError) {
#if EFI_UNIT_TEST
	pinToggleCounter = 0;
#endif

	if (!isBrainPinValid(p_brainPin)) {
		return;
	}

	// Enter a critical section so that other threads can't change the pin state out from underneath us
	chibios_rt::CriticalSectionLocker csl;

	if (!forceInitWithFatalError && hasFirmwareError()) {
		// Don't allow initializing more pins if we have a fatal error.
		// Pins should have just been reset, so we shouldn't try to init more.
		return;
	}

	// Check that this OutputPin isn't already assigned to another pin (reinit is allowed to change mode)
	// To avoid this error, call deInit() first
	if (isBrainPinValid(brainPin) && brainPin != p_brainPin) {
		firmwareError(ObdCode::CUSTOM_OBD_PIN_CONFLICT, "outputPin [%s] already assigned, cannot reassign without unregister first", msg);
		return;
	}

	if (outputMode > OM_OPENDRAIN_INVERTED) {
		firmwareError(ObdCode::CUSTOM_INVALID_MODE_SETTING, "%s invalid pin_output_mode_e %d %s",
				msg,
				outputMode,
				hwPortname(p_brainPin)
				);
		return;
	}

#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	iomode_t l_mode = (outputMode == OM_DEFAULT || outputMode == OM_INVERTED) ?
		PAL_MODE_OUTPUT_PUSHPULL : PAL_MODE_OUTPUT_OPENDRAIN;

	#if (BOARD_EXT_GPIOCHIPS > 0)
		this->ext = false;
	#endif
	if (brain_pin_is_onchip(p_brainPin)) {
		m_port = getHwPort(msg, p_brainPin);
		m_pin = getHwPin(msg, p_brainPin);

		// Validate port
		if (!m_port) {
			criticalError("OutputPin::initPin got invalid port for pin idx %d", static_cast<int>(p_brainPin));
			return;
		}
	}
	#if (BOARD_EXT_GPIOCHIPS > 0)
		else {
			this->ext = true;
		}
	#endif
#endif // briefly leave the include guard because we need to set default state in tests

	brainPin = p_brainPin;

	// The order of the next two calls may look strange, which is a good observation.
	// We call them in this order so that the pin is set to a known state BEFORE
	// it's enabled.  Enabling the pin then setting it could result in a (brief)
	// mystery state being driven on the pin (potentially dangerous).
	setDefaultPinState(outputMode);

#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	efiSetPadMode(msg, brainPin, l_mode);
	if (brain_pin_is_onchip(brainPin)) {
		// todo: handle OM_OPENDRAIN and OM_OPENDRAIN_INVERTED as well
		if (outputMode == OM_DEFAULT || outputMode == OM_INVERTED) {
#ifndef DISABLE_PIN_STATE_VALIDATION
		    int actualValue = palReadPad(m_port, m_pin);
		    // we had enough drama with pin configuration in board.h and else that we shall self-check

			const int logicalValue =
				(outputMode == OM_INVERTED)
				? !actualValue
				: actualValue;

			// if the pin was set to logical 1, then set an error and disable the pin so that things don't catch fire
			if (logicalValue) {
				criticalError("HARDWARE VALIDATION FAILED %s: unexpected startup pin state %s actual value=%d logical value=%d mode=%s", msg, hwPortname(brainPin), actualValue, logicalValue, getPin_output_mode_e(outputMode));
				OutputPin::deInit();
			}
#endif
		}
	}
#endif /* EFI_GPIO_HARDWARE */
}

void OutputPin::deInit() {
	efiPrintf("unregistering %s", hwPortname(brainPin));

	// Unregister under lock - we don't want other threads mucking with the pin while we're trying to turn it off
	chibios_rt::CriticalSectionLocker csl;

	// nothing to do if not registered in the first place
	if (!isBrainPinValid(brainPin)) {
		return;
	}

#if (BOARD_EXT_GPIOCHIPS > 0)
	ext = false;
#endif // (BOARD_EXT_GPIOCHIPS > 0)

#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	efiSetPadUnused(brainPin);
#endif /* EFI_GPIO_HARDWARE */

	// Clear the pin so that it won't get set any more
	brainPin = Gpio::Unassigned;
}

#if EFI_GPIO_HARDWARE

// questionable trick: we avoid using 'getHwPort' and 'getHwPin' in case of errors in order to increase the changes of turning the LED
// by reducing stack requirement
ioportid_t criticalErrorLedPort;
ioportmask_t criticalErrorLedPin;
uint8_t criticalErrorLedState;

#if EFI_PROD_CODE
static void initErrorLed(Gpio led) {
	enginePins.errorLedPin.initPin("led: CRITICAL status", led, (LED_PIN_MODE));
	criticalErrorLedPort = getHwPort("CRITICAL", led);
	criticalErrorLedPin = getHwPin("CRITICAL", led);
	criticalErrorLedState = (LED_PIN_MODE == OM_INVERTED) ? 0 : 1;
}
#endif /* EFI_PROD_CODE */

void initPrimaryPins() {
#if EFI_PROD_CODE
    initErrorLed(LED_CRITICAL_ERROR_BRAIN_PIN);

	addConsoleAction("gpio_pins", EnginePins::debug);
#endif /* EFI_PROD_CODE */
}

/**
 * This method is part of fatal error handling.
 * The whole method is pretty naive, but that's at least something.
 */
void turnAllPinsOff() {
	for (int i = 0; i < MAX_CYLINDER_COUNT; i++) {
		enginePins.injectors[i].setValue(false);
		enginePins.coils[i].setValue(false);
		enginePins.trailingCoils[i].setValue(false);
	}
	enginePins.mainRelay.setValue(false);
	enginePins.fuelPumpRelay.setValue(false);
	enginePins.checkEnginePin.setValue(true); // yes this one can go ON
#if EFI_PROD_CODE && HW_HELLEN
  hellenDisableEnSilently();
#endif
}
#endif /* EFI_GPIO_HARDWARE */

