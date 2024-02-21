/**
 * @file	efi_gpio.cpp
 * @brief	EFI-related GPIO code
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "engine_sniffer.h"

#include "drivers/gpio/gpio_ext.h"

#if HW_HELLEN
#include "hellen_meta.h"
#endif // HW_HELLEN

#if EFI_ELECTRONIC_THROTTLE_BODY
#include "electronic_throttle.h"
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

// todo: clean this mess, this should become 'static'/private
CCM_OPTIONAL EnginePins enginePins;

static const char* const sparkNames[] = { "Coil 1", "Coil 2", "Coil 3", "Coil 4", "Coil 5", "Coil 6", "Coil 7", "Coil 8",
		"Coil 9", "Coil 10", "Coil 11", "Coil 12"};

static const char* const trailNames[] = { "Trail 1", "Trail 2", "Trail 3", "Trail 4", "Trail 5", "Trail 6", "Trail 7", "Trail 8",
		"Trail 9", "Trail 10", "Trail 11", "Trail 12"};

static const char* const trailShortNames[] = { "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "rA", "rB", "rD" };

const char *vvtNames[] = {
		PROTOCOL_VVT1_NAME,
		PROTOCOL_VVT2_NAME,
		PROTOCOL_VVT3_NAME,
		PROTOCOL_VVT4_NAME};

const char *laNames[] = {
		PROTOCOL_WA_CHANNEL_1,
		PROTOCOL_WA_CHANNEL_2,
		PROTOCOL_WA_CHANNEL_3,
		PROTOCOL_WA_CHANNEL_4};

// these short names are part of engine sniffer protocol
static const char* const sparkShortNames[] = { PROTOCOL_COIL1_SHORT_NAME, "c2", "c3", "c4", "c5", "c6", "c7", "c8",
		"c9", "cA", "cB", "cD"};

static const char* const injectorNames[] = { "Injector 1", "Injector 2", "Injector 3", "Injector 4", "Injector 5", "Injector 6",
		"Injector 7", "Injector 8", "Injector 9", "Injector 10", "Injector 11", "Injector 12"};

static const char* const injectorShortNames[] = { PROTOCOL_INJ1_SHORT_NAME, "i2", "i3", "i4", "i5", "i6", "i7", "i8",
		"i9", "iA", "iB", "iC"};

static const char* const injectorStage2Names[] = { "Injector Second Stage 1", "Injector Second Stage 2", "Injector Second Stage 3", "Injector Second Stage 4", "Injector Second Stage 5", "Injector Second Stage 6",
		"Injector Second Stage 7", "Injector Second Stage 8", "Injector Second Stage 9", "Injector Second Stage 10", "Injector Second Stage 11", "Injector Second Stage 12"};

static const char* const injectorStage2ShortNames[] = { PROTOCOL_INJ1_STAGE2_SHORT_NAME, "j2", "j3", "j4", "j5", "j6", "j7", "j8",
		"j9", "jA", "jB", "jC"};

static const char* const auxValveShortNames[] = { "a1", "a2"};

static RegisteredOutputPin * registeredOutputHead = nullptr;

RegisteredNamedOutputPin::RegisteredNamedOutputPin(const char *name, size_t pinOffset,
		size_t pinModeOffset) : RegisteredOutputPin(name, pinOffset, pinModeOffset) {
}

RegisteredOutputPin::RegisteredOutputPin(const char *registrationName, size_t pinOffset,
		size_t pinModeOffset)
	: next(registeredOutputHead)
	, m_registrationName(registrationName)
	, m_pinOffset(static_cast<uint16_t>(pinOffset))
	, m_hasPinMode(true)
	, m_pinModeOffset(static_cast<uint16_t>(pinModeOffset))
{
	// adding into head of the list is so easy and since we do not care about order that's what we shall do
	registeredOutputHead = this;
}

RegisteredOutputPin::RegisteredOutputPin(const char *registrationName, size_t pinOffset)
	: next(registeredOutputHead)
	, m_registrationName(registrationName)
	, m_pinOffset(static_cast<uint16_t>(pinOffset))
	, m_hasPinMode(false)
	, m_pinModeOffset(-1)
{
	// adding into head of the list is so easy and since we do not care about order that's what we shall do
	registeredOutputHead = this;
}

bool RegisteredOutputPin::isPinConfigurationChanged() {
#if EFI_PROD_CODE
	brain_pin_e        curPin = *(brain_pin_e       *) ((void *) (&((char*)&activeConfiguration)[m_pinOffset]));
	brain_pin_e        newPin = *(brain_pin_e       *) ((void *) (&((char*) engineConfiguration)[m_pinOffset]));
	bool pinChanged = curPin != newPin;

	if (!m_hasPinMode) {
		return pinChanged;
	}

	pin_output_mode_e curMode = *(pin_output_mode_e *) ((void *) (&((char*)&activeConfiguration)[m_pinModeOffset]));
	pin_output_mode_e newMode = *(pin_output_mode_e *) ((void *) (&((char*) engineConfiguration)[m_pinModeOffset]));
	return pinChanged || curMode != newMode;
#else
	return true;
#endif // EFI_PROD_CODE
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
		initPin(m_registrationName, newPin, newMode);
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
		mainRelay("Main Relay", CONFIG_PIN_OFFSETS(mainRelay)),
		hpfpValve("HPFP Valve", CONFIG_PIN_OFFSETS(hpfpValve)),
		starterControl("Starter Relay", CONFIG_PIN_OFFSETS(starterControl)),
		starterRelayDisable("Starter Disable Relay", CONFIG_PIN_OFFSETS(starterRelayDisable)),
		fanRelay("Fan Relay", CONFIG_PIN_OFFSETS(fan)),
		fanRelay2("Fan Relay 2", CONFIG_PIN_OFFSETS(fan2)),
		acRelay("A/C Relay", CONFIG_PIN_OFFSETS(acRelay)),
		fuelPumpRelay("Fuel pump Relay", CONFIG_PIN_OFFSETS(fuelPump)),
		harleyAcr("Harley ACR", CONFIG_OFFSET(acrPin)),
		harleyAcr2("Harley ACR 2", CONFIG_OFFSET(acrPin2)),
		boostPin("Boost", CONFIG_PIN_OFFSETS(boostControl)),
		idleSolenoidPin("Idle Valve", CONFIG_OFFSET2(idle, solenoidPin), CONFIG_OFFSET2(idle, solenoidPinMode)),
		secondIdleSolenoidPin("Idle Valve#2", CONFIG_OFFSET(secondSolenoidPin), CONFIG_OFFSET2(idle, solenoidPinMode)),
		alternatorPin("Alternator control", CONFIG_PIN_OFFSETS(alternatorControl)),
		checkEnginePin("checkEnginePin", CONFIG_PIN_OFFSETS(malfunctionIndicator)),
		tachOut("tachOut", CONFIG_PIN_OFFSETS(tachOutput)),
		speedoOut("speedoOut", CONFIG_OFFSET(speedometerOutputPin))
{
	hpfpValve.setName(PROTOCOL_HPFP_NAME);

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
	stopAuxValves();

#if EFI_ELECTRONIC_THROTTLE_BODY
	unregisterEtbPins();
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

	// todo: add pinMode
	unregisterOutputIfPinChanged(sdCsPin, sdCardCsPin);
	unregisterOutputIfPinChanged(accelerometerCs, LIS302DLCsPin);

	RegisteredOutputPin * pin = registeredOutputHead;
	while (pin != nullptr) {
		pin->unregister();
		pin = pin->next;
	}
}

void EnginePins::debug() {
	RegisteredOutputPin * pin = registeredOutputHead;
	while (pin != nullptr) {
		efiPrintf("%s %d", pin->getRegistrationName(), pin->m_currentLogicValue);
		pin = pin->next;
	}
}

void EnginePins::startPins() {
#if EFI_ENGINE_CONTROL
	startInjectionPins();
	startIgnitionPins();
	startAuxValves();
#endif /* EFI_ENGINE_CONTROL */

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

NamedOutputPin::NamedOutputPin() : OutputPin() {
}

NamedOutputPin::NamedOutputPin(const char *name) 
	: m_name(name)
{
}

const char *NamedOutputPin::getName() const {
	return m_name;
}

void NamedOutputPin::setName(const char* name) {
	m_name = name;
}

const char *NamedOutputPin::getShortName() const {
	return shortName ? shortName : m_name;
}

#if EFI_UNIT_TEST
extern bool verboseMode;
#endif // EFI_UNIT_TEST

void NamedOutputPin::setHigh() {
#if EFI_UNIT_TEST
	if (verboseMode) {
		efiPrintf("pin %s goes high", m_name);
	}
#endif // EFI_UNIT_TEST
#if EFI_DEFAILED_LOGGING
//	signal->hi_time = hTimeNow();
#endif /* EFI_DEFAILED_LOGGING */

	// turn the output level ACTIVE
	setValue(true);

#if EFI_ENGINE_SNIFFER
	addEngineSnifferOutputPinEvent(this, true);
#endif /* EFI_ENGINE_SNIFFER */
}

void NamedOutputPin::setLow() {
#if EFI_UNIT_TEST
	if (verboseMode) {
		efiPrintf("pin %s goes low", m_name);
	}
#endif // EFI_UNIT_TEST

	// turn off the output
	setValue(false);

#if EFI_ENGINE_SNIFFER
	addEngineSnifferOutputPinEvent(this, false);
#endif /* EFI_ENGINE_SNIFFER */
}

bool NamedOutputPin::stop() {
#if EFI_GPIO_HARDWARE
	if (isInitialized() && getLogicValue()) {
		setValue(false);
		efiPrintf("turning off %s", m_name);
		return true;
	}
#endif /* EFI_GPIO_HARDWARE */
	return false;
}

void InjectorOutputPin::reset() {
	// If this injector was open, close it and reset state
	if (m_overlappingCounter != 0) {
		m_overlappingCounter = 0;
		setValue(0);
	}

	// todo: this could be refactored by calling some super-reset method
	m_currentLogicValue = 0;
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
	}
}

void IgnitionOutputPin::reset() {
	outOfOrder = false;
	signalFallSparkId = 0;
}

bool OutputPin::isInitialized() const {
	return isBrainPinValid(m_brainPin);
}

void OutputPin::toggle() {
	setValue(!getLogicValue());
}

bool OutputPin::getAndSet(int logicValue) {
	bool oldValue = getLogicValue();
	setValue(logicValue);
	return oldValue;
}

// This function is only used on real hardware
#if EFI_PROD_CODE
void OutputPin::setOnchipValue(int electricalValue) {
	if (m_brainPin == Gpio::Unassigned || m_brainPin == Gpio::Invalid) {
		// todo: make 'setOnchipValue' or 'reportsetOnchipValueError' virtual and override for NamedOutputPin?
		warning(ObdCode::CUSTOM_ERR_6586, "attempting to change unassigned pin");
		return;
	}
	palWritePad(m_port, m_pin, electricalValue);
}
#endif // EFI_PROD_CODE

void OutputPin::setValue(int logicValue) {
#if ENABLE_PERF_TRACE
// todo: https://github.com/rusefi/rusefi/issues/1638
//	ScopePerf perf(PE::OutputPinSetValue);
#endif // ENABLE_PERF_TRACE

#if EFI_UNIT_TEST
	unitTestTurnedOnCounter++;

	if (verboseMode) {
		efiPrintf("pin goes %d", logicValue);
	}
#endif // EFI_UNIT_TEST

	// Always store the current logical value of the pin (so it can be
	// used internally even if not connected to a real hardware pin)
	m_currentLogicValue = logicValue;

	// Nothing else to do if not configured
	if (!isInitialized()) {
		return;
	}

	efiAssertVoid(ObdCode::CUSTOM_ERR_6622, m_mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e");
	int electricalValue = getElectricalValue(logicValue, m_mode);

#if EFI_PROD_CODE
	#if (BOARD_EXT_GPIOCHIPS > 0)
		if (!this->ext) {
			setOnchipValue(electricalValue);
		} else {
			/* external pin */
			gpiochips_writePad(m_brainPin, logicValue);
			/* TODO: check return value */
		}
	#else
		setOnchipValue(electricalValue);
	#endif
#else /* EFI_PROD_CODE */
	setMockState(m_brainPin, electricalValue);
#endif /* EFI_PROD_CODE */
}

bool OutputPin::getLogicValue() const {
	// Compare against 1 since it could also be INITIAL_PIN_STATE (which means logical 0, but we haven't initialized the pin yet)
	return m_currentLogicValue == 1;
}

void OutputPin::setDefaultPinState(pin_output_mode_e outputMode) {
	assertOMode(outputMode);
	m_mode = outputMode;
	setValue(false); // initial state
}

brain_pin_diag_e OutputPin::getDiag() const {
#if BOARD_EXT_GPIOCHIPS > 0
	return gpiochips_getDiag(m_brainPin);
#else
	return PIN_OK;
#endif
}

void initOutputPins() {
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

void OutputPin::initPin(const char *msg, brain_pin_e brainPin) {
	initPin(msg, brainPin, OM_DEFAULT);
}

void OutputPin::initPin(const char *msg, brain_pin_e brainPin, pin_output_mode_e outputMode, bool forceInitWithFatalError) {
#if EFI_UNIT_TEST
	unitTestTurnedOnCounter = 0;
#endif

	if (!isBrainPinValid(brainPin)) {
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
	if (isBrainPinValid(m_brainPin) && m_brainPin != brainPin) {
		firmwareError(ObdCode::CUSTOM_OBD_PIN_CONFLICT, "outputPin [%s] already assigned, cannot reassign without unregister first", msg);
		return;
	}

	if (outputMode > OM_OPENDRAIN_INVERTED) {
		firmwareError(ObdCode::CUSTOM_INVALID_MODE_SETTING, "%s invalid pin_output_mode_e %d %s",
				msg,
				outputMode,
				hwPortname(brainPin)
				);
		return;
	}

#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	#if (BOARD_EXT_GPIOCHIPS > 0)
		this->ext = false;
	#endif
	if (brain_pin_is_onchip(brainPin)) {
		ioportid_t port = getHwPort(msg, brainPin);
		int pin = getHwPin(msg, brainPin);

		// Validate port
		if (port == GPIO_NULL) {
			firmwareError(ObdCode::OBD_PCM_Processor_Fault, "OutputPin::initPin got invalid port for pin idx %d", static_cast<int>(brainPin));
			return;
		}

		m_port = port;
		m_pin = pin;
	}
	#if (BOARD_EXT_GPIOCHIPS > 0)
		else {
			this->ext = true;
		}
	#endif
#endif // briefly leave the include guard because we need to set default state in tests

	m_brainPin = brainPin;

	// The order of the next two calls may look strange, which is a good observation.
	// We call them in this order so that the pin is set to a known state BEFORE
	// it's enabled.  Enabling the pin then setting it could result in a (brief)
	// mystery state being driven on the pin (potentially dangerous).
	setDefaultPinState(outputMode);

#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	iomode_t ioMode = (outputMode == OM_DEFAULT || outputMode == OM_INVERTED) ?
		PAL_MODE_OUTPUT_PUSHPULL : PAL_MODE_OUTPUT_OPENDRAIN;

	efiSetPadMode(msg, m_brainPin, ioMode);
#ifndef DISABLE_PIN_STATE_VALIDATION
	if (brain_pin_is_onchip(m_brainPin)) {
		int actualValue = palReadPad(m_port, m_pin);
		// we had enough drama with pin configuration in board.h and else that we shall self-check

		// todo: handle OM_OPENDRAIN and OM_OPENDRAIN_INVERTED as well
		if (outputMode == OM_DEFAULT || outputMode == OM_INVERTED) {
			const int logicalValue = 
				(outputMode == OM_INVERTED) 
				? !actualValue 
				: actualValue;

			// if the pin was set to logical 1, then set an error and disable the pin so that things don't catch fire
			if (logicalValue) {
				firmwareError(ObdCode::OBD_PCM_Processor_Fault, "HARDWARE VALIDATION FAILED %s: unexpected startup pin state %s actual value=%d logical value=%d mode=%s", msg, hwPortname(m_brainPin), actualValue, logicalValue, getPin_output_mode_e(outputMode));
				OutputPin::deInit();
			}
		}
	}
#endif // DISABLE_PIN_STATE_VALIDATION
#endif /* EFI_GPIO_HARDWARE */
}

void OutputPin::deInit() {
	// Unregister under lock - we don't want other threads mucking with the pin while we're trying to turn it off
	chibios_rt::CriticalSectionLocker csl;

	// nothing to do if not registered in the first place
	if (!isInitialized()) {
		return;
	}

#if (BOARD_EXT_GPIOCHIPS > 0)
	ext = false;
#endif // (BOARD_EXT_GPIOCHIPS > 0)

	efiPrintf("unregistering %s", hwPortname(m_brainPin));

#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	efiSetPadUnused(m_brainPin);
#endif /* EFI_GPIO_HARDWARE */

	// Clear the pin so that it won't get set any more
	m_brainPin = Gpio::Unassigned;
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
void turnAllPinsOff(void) {
	for (int i = 0; i < MAX_CYLINDER_COUNT; i++) {
		enginePins.injectors[i].setValue(false);
		enginePins.coils[i].setValue(false);
		enginePins.trailingCoils[i].setValue(false);
	}
}
#endif /* EFI_GPIO_HARDWARE */
