/**
 * @file	efi_gpio.cpp
 * @brief	EFI-related GPIO code
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "os_access.h"
#include "drivers/gpio/gpio_ext.h"

#if EFI_ELECTRONIC_THROTTLE_BODY
#include "electronic_throttle.h"
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

// todo: clean this mess, this should become 'static'/private
EnginePins enginePins;

pin_output_mode_e DEFAULT_OUTPUT = OM_DEFAULT;
pin_output_mode_e INVERTED_OUTPUT = OM_INVERTED;

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

// these short names are part of engine sniffer protocol
static const char* const sparkShortNames[] = { PROTOCOL_COIL1_SHORT_NAME, "c2", "c3", "c4", "c5", "c6", "c7", "c8",
		"c9", "cA", "cB", "cD"};

static const char* const injectorNames[] = { "Injector 1", "Injector 2", "Injector 3", "Injector 4", "Injector 5", "Injector 6",
		"Injector 7", "Injector 8", "Injector 9", "Injector 10", "Injector 11", "Injector 12"};

static const char* const injectorShortNames[] = { PROTOCOL_INJ1_SHORT_NAME, "i2", "i3", "i4", "i5", "i6", "i7", "i8",
		"j9", "iA", "iB", "iC"};

static const char* const auxValveShortNames[] = { "a1", "a2"};

static RegisteredOutputPin * registeredOutputHead = nullptr;

RegisteredNamedOutputPin::RegisteredNamedOutputPin(const char *name, short pinOffset,
		short pinModeOffset) : RegisteredOutputPin(name, pinOffset, pinModeOffset) {
}

RegisteredOutputPin::RegisteredOutputPin(const char *registrationName, short pinOffset,
		short pinModeOffset) {
	this->registrationName = registrationName;
	this->pinOffset = pinOffset;
	this->pinModeOffset = pinModeOffset;
	// adding into head of the list is so easy and since we do not care about order that's what we shall do
	this->next = registeredOutputHead;
	registeredOutputHead = this;
}

bool RegisteredOutputPin::isPinConfigurationChanged() {
#if EFI_PROD_CODE
	brain_pin_e        curPin = *(brain_pin_e       *) ((void *) (&((char*)&activeConfiguration)[pinOffset]));
	brain_pin_e        newPin = *(brain_pin_e       *) ((void *) (&((char*) engineConfiguration)[pinOffset]));

    pin_output_mode_e curMode = *(pin_output_mode_e *) ((void *) (&((char*)&activeConfiguration)[pinModeOffset]));
    pin_output_mode_e newMode = *(pin_output_mode_e *) ((void *) (&((char*) engineConfiguration)[pinModeOffset]));
    return curPin != newPin || curMode != newMode;
#else
    return true;
#endif // EFI_PROD_CODE
}

void RegisteredOutputPin::init() {
	brain_pin_e        newPin = *(brain_pin_e       *) ((void *) (&((char*) engineConfiguration)[pinOffset]));
    pin_output_mode_e *newMode = (pin_output_mode_e *) ((void *) (&((char*) engineConfiguration)[pinModeOffset]));

    if (isPinConfigurationChanged()) {
		this->initPin(registrationName, newPin, newMode);
    }
}

void RegisteredOutputPin::unregister() {
	if (isPinConfigurationChanged()) {
		OutputPin::deInit();
	}
}

#define CONFIG_OFFSET(x) x##_offset
// todo: pin and pinMode should be combined into a composite entity
// todo: one of the impediments is code generator hints handling (we need custom hints and those are not handled nice for fields of structs?)
#define CONFIG_PIN_OFFSETS(x) CONFIG_OFFSET(x##Pin), CONFIG_OFFSET(x##PinMode)


EnginePins::EnginePins() :
		mainRelay("Main Relay", CONFIG_PIN_OFFSETS(mainRelay)),
		hpfpValve("HPFP Valve", CONFIG_PIN_OFFSETS(hpfpValve)),
		starterControl("Starter Relay", CONFIG_PIN_OFFSETS(starterControl)),
		starterRelayDisable("Starter Disable Relay", CONFIG_PIN_OFFSETS(starterRelayDisable)),
		fanRelay("Fan Relay", CONFIG_PIN_OFFSETS(fan)),
		fanRelay2("Fan Relay 2", CONFIG_PIN_OFFSETS(fan2)),
		acRelay("A/C Relay", CONFIG_PIN_OFFSETS(acRelay)),
		fuelPumpRelay("Fuel pump Relay", CONFIG_PIN_OFFSETS(fuelPump)),
	    boostPin("Boost", CONFIG_PIN_OFFSETS(boostControl)),
		idleSolenoidPin("Idle Valve", idle_solenoidPin_offset, idle_solenoidPinMode_offset),
		secondIdleSolenoidPin("Idle Valve#2", CONFIG_OFFSET(secondSolenoidPin), idle_solenoidPinMode_offset),
		alternatorPin("Alternator control", CONFIG_PIN_OFFSETS(alternatorControl)),
		checkEnginePin("checkEnginePin", CONFIG_PIN_OFFSETS(malfunctionIndicator)),
		tachOut("tachOut", CONFIG_PIN_OFFSETS(tachOutput)),
		triggerDecoderErrorPin("led: trigger debug", CONFIG_PIN_OFFSETS(triggerError))
{
	tachOut.name = PROTOCOL_TACH_NAME;
	hpfpValve.name = PROTOCOL_HPFP_NAME;

	static_assert(efi::size(sparkNames) >= MAX_CYLINDER_COUNT, "Too many ignition pins");
	static_assert(efi::size(trailNames) >= MAX_CYLINDER_COUNT, "Too many ignition pins");
	static_assert(efi::size(injectorNames) >= MAX_CYLINDER_COUNT, "Too many injection pins");
	for (int i = 0; i < MAX_CYLINDER_COUNT;i++) {
		enginePins.coils[i].name = sparkNames[i];
		enginePins.coils[i].shortName = sparkShortNames[i];

		enginePins.trailingCoils[i].name = trailNames[i];
		enginePins.trailingCoils[i].shortName = trailShortNames[i];

		enginePins.injectors[i].injectorIndex = i;
		enginePins.injectors[i].name = injectorNames[i];
		enginePins.injectors[i].shortName = injectorShortNames[i];
	}

	static_assert(efi::size(auxValveShortNames) >= AUX_DIGITAL_VALVE_COUNT, "Too many aux valve pins");
	for (int i = 0; i < AUX_DIGITAL_VALVE_COUNT;i++) {
		enginePins.auxValve[i].name = auxValveShortNames[i];
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
		efiPrintf("%s %d", pin->registrationName, pin->currentLogicValue);
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
			output->initPin(output->name, engineConfiguration->auxValves[i]);
		}
	}
#endif /* EFI_PROD_CODE */
}

void EnginePins::startIgnitionPins() {
#if EFI_PROD_CODE
	for (size_t i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		NamedOutputPin *trailingOutput = &enginePins.trailingCoils[i];
		if (isPinOrModeChanged(trailingCoilPins[i], ignitionPinMode)) {
			trailingOutput->initPin(trailingOutput->name, CONFIG(trailingCoilPins)[i], &CONFIG(ignitionPinMode));
		}

		NamedOutputPin *output = &enginePins.coils[i];
		if (isPinOrModeChanged(ignitionPins[i], ignitionPinMode)) {
			output->initPin(output->name, CONFIG(ignitionPins)[i], &CONFIG(ignitionPinMode));
		}
	}
#endif /* EFI_PROD_CODE */
}

void EnginePins::startInjectionPins() {
#if EFI_PROD_CODE
	// todo: should we move this code closer to the injection logic?
	for (size_t i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		NamedOutputPin *output = &enginePins.injectors[i];
		if (isPinOrModeChanged(injectionPins[i], injectionPinMode)) {
			output->initPin(output->name, CONFIG(injectionPins)[i],
					&CONFIG(injectionPinMode));
		}
	}
#endif /* EFI_PROD_CODE */
}

NamedOutputPin::NamedOutputPin() : OutputPin() {
}

NamedOutputPin::NamedOutputPin(const char *name) : OutputPin() {
	this->name = name;
}

const char *NamedOutputPin::getName() const {
	return name;
}

const char *NamedOutputPin::getShortName() const {
	return shortName == nullptr ? name : shortName;
}

#if EFI_UNIT_TEST
extern bool verboseMode;
#endif // EFI_UNIT_TEST

void NamedOutputPin::setHigh() {
#if EFI_UNIT_TEST
	if (verboseMode) {
		efiPrintf("pin %s goes high", name);
	}
#endif // EFI_UNIT_TEST
#if EFI_DEFAILED_LOGGING
//	signal->hi_time = hTimeNow();
#endif /* EFI_DEFAILED_LOGGING */

	// turn the output level ACTIVE
	setValue(true);

#if EFI_ENGINE_SNIFFER
	addEngineSnifferEvent(getShortName(), PROTOCOL_ES_UP);
#endif /* EFI_ENGINE_SNIFFER */
}

void NamedOutputPin::setLow() {
#if EFI_UNIT_TEST
	if (verboseMode) {
		efiPrintf("pin %s goes low", name);
	}
#endif // EFI_UNIT_TEST

	// turn off the output
	setValue(false);

#if EFI_ENGINE_SNIFFER
	addEngineSnifferEvent(getShortName(), PROTOCOL_ES_DOWN);
#endif /* EFI_ENGINE_SNIFFER */
}

InjectorOutputPin::InjectorOutputPin() : NamedOutputPin() {
	overlappingCounter = 1; // Force update in reset
	reset();
	injectorIndex = -1;
}

bool NamedOutputPin::stop() {
#if EFI_GPIO_HARDWARE
	if (isInitialized() && getLogicValue()) {
		setValue(false);
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
		setValue(0);
	}

	// todo: this could be refactored by calling some super-reset method
	currentLogicValue = 0;
}

IgnitionOutputPin::IgnitionOutputPin() {
	reset();
}

void IgnitionOutputPin::reset() {
	outOfOrder = false;
	signalFallSparkId = 0;
}

OutputPin::OutputPin() {
	modePtr = &DEFAULT_OUTPUT;
}

bool OutputPin::isInitialized() {
#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
#if (BOARD_EXT_GPIOCHIPS > 0)
	if (ext)
		return true;
#endif /* (BOARD_EXT_GPIOCHIPS > 0) */
	return port != NULL;
#else /* EFI_GPIO_HARDWARE */
	return true;
#endif /* EFI_GPIO_HARDWARE */
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
	palWritePad(port, pin, electricalValue);
}
#endif // EFI_PROD_CODE

void OutputPin::setValue(int logicValue) {
#if ENABLE_PERF_TRACE
// todo: https://github.com/rusefi/rusefi/issues/1638
//	ScopePerf perf(PE::OutputPinSetValue);
#endif // ENABLE_PERF_TRACE

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

	efiAssertVoid(CUSTOM_ERR_6621, modePtr!=NULL, "pin mode not initialized");
	pin_output_mode_e mode = *modePtr;
	efiAssertVoid(CUSTOM_ERR_6622, mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e");
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

void OutputPin::setDefaultPinState(const pin_output_mode_e *outputMode) {
	pin_output_mode_e mode = *outputMode;
	/* may be*/UNUSED(mode);
	assertOMode(mode);
	this->modePtr = outputMode;
	setValue(false); // initial state
}

void initOutputPins() {
#if EFI_GPIO_HARDWARE

#if HAL_USE_SPI
	enginePins.sdCsPin.initPin("SD CS", CONFIG(sdCardCsPin));
#endif /* HAL_USE_SPI */

#if EFI_SHAFT_POSITION_INPUT
	// todo: migrate remaining OutputPin to RegisteredOutputPin in order to get consistent dynamic pin init/deinit
	enginePins.debugTriggerSync.initPin("debug: sync", CONFIG(debugTriggerSync));
#endif // EFI_SHAFT_POSITION_INPUT

	enginePins.o2heater.initPin("O2 heater", CONFIG(o2heaterPin));

#endif /* EFI_GPIO_HARDWARE */
}

void OutputPin::initPin(const char *msg, brain_pin_e brainPin) {
	initPin(msg, brainPin, &DEFAULT_OUTPUT);
}

void OutputPin::initPin(const char *msg, brain_pin_e brainPin, const pin_output_mode_e *outputMode, bool forceInitWithFatalError) {
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
	if (isBrainPinValid(this->brainPin) && this->brainPin != brainPin) {
		firmwareError(CUSTOM_OBD_PIN_CONFLICT, "outputPin [%s] already assigned, cannot reassign without unregister first", msg);
		return;
	}

	if (*outputMode > OM_OPENDRAIN_INVERTED) {
		firmwareError(CUSTOM_INVALID_MODE_SETTING, "%s invalid pin_output_mode_e %d %s",
				msg,
				*outputMode,
				hwPortname(brainPin)
				);
		return;
	}

#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	iomode_t mode = (*outputMode == OM_DEFAULT || *outputMode == OM_INVERTED) ?
		PAL_MODE_OUTPUT_PUSHPULL : PAL_MODE_OUTPUT_OPENDRAIN;

	#if (BOARD_EXT_GPIOCHIPS > 0)
		this->ext = false;
	#endif
	if (brain_pin_is_onchip(brainPin)) {
		ioportid_t port = getHwPort(msg, brainPin);
		int pin = getHwPin(msg, brainPin);

		// Validate port
		if (port == GPIO_NULL) {
			firmwareError(OBD_PCM_Processor_Fault, "OutputPin::initPin got invalid port for pin idx %d", static_cast<int>(brainPin));
			return;
		}

		this->port = port;
		this->pin = pin;
	}
	#if (BOARD_EXT_GPIOCHIPS > 0)
		else {
			this->ext = true;
		}
	#endif
#endif // briefly leave the include guard because we need to set default state in tests

	this->brainPin = brainPin;

	// The order of the next two calls may look strange, which is a good observation.
	// We call them in this order so that the pin is set to a known state BEFORE
	// it's enabled.  Enabling the pin then setting it could result in a (brief)
	// mystery state being driven on the pin (potentially dangerous).
	setDefaultPinState(outputMode);

#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	efiSetPadMode(msg, brainPin, mode);
	if (brain_pin_is_onchip(brainPin)) {
		int actualValue = palReadPad(port, pin);
		// we had enough drama with pin configuration in board.h and else that we shall self-check

		// todo: handle OM_OPENDRAIN and OM_OPENDRAIN_INVERTED as well
		if (*outputMode == OM_DEFAULT || *outputMode == OM_INVERTED) {
			const int logicalValue = 
				(*outputMode == OM_INVERTED) 
				? !actualValue 
				: actualValue;

			// if the pin was set to logical 1, then set an error and disable the pin so that things don't catch fire
			if (logicalValue) {
				firmwareError(OBD_PCM_Processor_Fault, "%s: startup pin state %s actual value=%d logical value=%d mode=%s", msg, hwPortname(brainPin), actualValue, logicalValue, getPin_output_mode_e(*outputMode));
				OutputPin::deInit();
			}
		}
	}
#endif /* EFI_GPIO_HARDWARE */
}

void OutputPin::deInit() {
	// Unregister under lock - we don't want other threads mucking with the pin while we're trying to turn it off
	chibios_rt::CriticalSectionLocker csl;

	// nothing to do if not registered in the first place
	if (!isBrainPinValid(brainPin)) {
		return;
	}

#if (BOARD_EXT_GPIOCHIPS > 0)
	ext = false;
#endif // (BOARD_EXT_GPIOCHIPS > 0)

	efiPrintf("unregistering %s", hwPortname(brainPin));

#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	efiSetPadUnused(brainPin);
#endif /* EFI_GPIO_HARDWARE */

	// Clear the pin so that it won't get set any more
	brainPin = GPIO_UNASSIGNED;
}

#if EFI_GPIO_HARDWARE

// questionable trick: we avoid using 'getHwPort' and 'getHwPin' in case of errors in order to increase the changes of turning the LED
// by reducing stack requirement
ioportid_t criticalErrorLedPort;
ioportmask_t criticalErrorLedPin;
uint8_t criticalErrorLedState;

#ifndef LED_ERROR_BRAIN_PIN_MODE
#define LED_ERROR_BRAIN_PIN_MODE DEFAULT_OUTPUT
#endif /* LED_ERROR_BRAIN_PIN_MODE */

void initPrimaryPins() {
#if EFI_PROD_CODE
	enginePins.errorLedPin.initPin("led: CRITICAL status", LED_CRITICAL_ERROR_BRAIN_PIN, &(LED_ERROR_BRAIN_PIN_MODE));
	criticalErrorLedPort = getHwPort("CRITICAL", LED_CRITICAL_ERROR_BRAIN_PIN);
	criticalErrorLedPin = getHwPin("CRITICAL", LED_CRITICAL_ERROR_BRAIN_PIN);
	criticalErrorLedState = (LED_ERROR_BRAIN_PIN_MODE == INVERTED_OUTPUT) ? 0 : 1;

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
