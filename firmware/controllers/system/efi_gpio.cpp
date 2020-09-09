/**
 * @file	efi_gpio.cpp
 * @brief	EFI-related GPIO code
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "engine.h"
#include "efi_gpio.h"
#include "drivers/gpio/gpio_ext.h"
#include "perf_trace.h"

#if EFI_GPIO_HARDWARE
#include "pin_repository.h"
#include "io_pins.h"
#endif /* EFI_GPIO_HARDWARE */

#if EFI_ELECTRONIC_THROTTLE_BODY
#include "electronic_throttle.h"
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

EXTERN_ENGINE;

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

// todo: clean this mess, this should become 'static'/private
EnginePins enginePins;
static Logging* logger;

static const pin_output_mode_e DEFAULT_OUTPUT = OM_DEFAULT;

static const char *sparkNames[] = { "Coil 1", "Coil 2", "Coil 3", "Coil 4", "Coil 5", "Coil 6", "Coil 7", "Coil 8",
		"Coil 9", "Coil 10", "Coil 11", "Coil 12"};

// these short names are part of engine sniffer protocol
static const char *sparkShortNames[] = { PROTOCOL_COIL1_SHORT_NAME, "c2", "c3", "c4", "c5", "c6", "c7", "c8",
		"c9", "cA", "cB", "cD"};

static const char *injectorNames[] = { "Injector 1", "Injector 2", "Injector 3", "Injector 4", "Injector 5", "Injector 6",
		"Injector 7", "Injector 8", "Injector 9", "Injector 10", "Injector 11", "Injector 12"};

static const char *injectorShortNames[] = { PROTOCOL_INJ1_SHORT_NAME, "i2", "i3", "i4", "i5", "i6", "i7", "i8",
		"j9", "iA", "iB", "iC"};

static const char *auxValveShortNames[] = { "a1", "a2"};

EnginePins::EnginePins() {
	tachOut.name = PROTOCOL_TACH_NAME;

	static_assert(efi::size(sparkNames) >= IGNITION_PIN_COUNT, "Too many ignition pins"); 
	for (int i = 0; i < IGNITION_PIN_COUNT;i++) {
		enginePins.coils[i].name = sparkNames[i];
		enginePins.coils[i].shortName = sparkShortNames[i];
	}

	static_assert(efi::size(injectorNames) >= INJECTION_PIN_COUNT, "Too many injection pins"); 
	for (int i = 0; i < INJECTION_PIN_COUNT;i++) {
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

#if EFI_PROD_CODE
#define setPinValue(outputPin, electricalValue, logicValue)                        \
  {                                                                                \
    if ((outputPin)->currentLogicValue != (logicValue)) {                          \
	  palWritePad((outputPin)->port, (outputPin)->pin, (electricalValue));         \
	  (outputPin)->currentLogicValue = (logicValue);                               \
    }                                                                              \
  }

#define unregisterOutputIfPinChanged(output, pin) {                                \
	if (isConfigurationChanged(pin)) {                                             \
		(output).unregisterOutput(activeConfiguration.pin);                        \
	}                                                                              \
}

#define unregisterOutputIfPinOrModeChanged(output, pin, mode) {                    \
	if (isPinOrModeChanged(pin, mode)) {                                           \
		(output).unregisterOutput(activeConfiguration.pin);                        \
	}                                                                              \
}

#else /* EFI_PROD_CODE */

#define setPinValue(outputPin, electricalValue, logicValue)                        \
  {                                                                                \
    if ((outputPin)->currentLogicValue != (logicValue)) {                          \
	  (outputPin)->currentLogicValue = (logicValue);                               \
    }                                                                              \
  }
#endif /* EFI_PROD_CODE */

bool EnginePins::stopPins() {
	bool result = false;
	for (int i = 0; i < IGNITION_PIN_COUNT; i++) {
		result |= coils[i].stop();
	}
	for (int i = 0; i < INJECTION_PIN_COUNT; i++) {
		result |= injectors[i].stop();
	}
	for (int i = 0; i < AUX_DIGITAL_VALVE_COUNT; i++) {
		result |= auxValve[i].stop();
	}
	return result;
}

void EnginePins::unregisterPins() {
#if EFI_ELECTRONIC_THROTTLE_BODY
	unregisterEtbPins();
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */
#if EFI_PROD_CODE
	unregisterOutputIfPinOrModeChanged(fuelPumpRelay, fuelPumpPin, fuelPumpPinMode);
	unregisterOutputIfPinOrModeChanged(fanRelay, fanPin, fanPinMode);
	unregisterOutputIfPinOrModeChanged(acRelay, acRelayPin, acRelayPinMode);
	unregisterOutputIfPinOrModeChanged(hipCs, hip9011CsPin, hip9011CsPinMode);
	unregisterOutputIfPinOrModeChanged(triggerDecoderErrorPin, triggerErrorPin, triggerErrorPinMode);
	unregisterOutputIfPinOrModeChanged(checkEnginePin, malfunctionIndicatorPin, malfunctionIndicatorPinMode);
	unregisterOutputIfPinOrModeChanged(tachOut, tachOutputPin, tachOutputPinMode);
	unregisterOutputIfPinOrModeChanged(idleSolenoidPin, idle.solenoidPin, idle.solenoidPinMode);
	unregisterOutputIfPinOrModeChanged(secondIdleSolenoidPin, secondSolenoidPin, idle.solenoidPinMode);
	unregisterOutputIfPinChanged(sdCsPin, sdCardCsPin);
	unregisterOutputIfPinChanged(accelerometerCs, LIS302DLCsPin);

	for (int i = 0;i < FSIO_COMMAND_COUNT;i++) {
		unregisterOutputIfPinChanged(fsioOutputs[i], fsioOutputPins[i]);
	}

    unregisterOutputIfPinOrModeChanged(boostPin, boostControlPin, boostControlPinMode);
	unregisterOutputIfPinOrModeChanged(alternatorPin, alternatorControlPin, alternatorControlPinMode);
	unregisterOutputIfPinOrModeChanged(mainRelay, mainRelayPin, mainRelayPinMode);
	unregisterOutputIfPinOrModeChanged(starterRelayDisable, starterRelayDisablePin, starterRelayDisableMode);

	unregisterOutputIfPinChanged(starterControl, starterControlPin);

#endif /* EFI_PROD_CODE */
}

void EnginePins::startPins() {
	startInjectionPins();
	startIgnitionPins();
	startAuxValves();
}

void EnginePins::reset() {
	for (int i = 0; i < INJECTION_PIN_COUNT;i++) {
		injectors[i].reset();
	}
	for (int i = 0; i < IGNITION_PIN_COUNT;i++) {
		coils[i].reset();
	}
}

void EnginePins::stopIgnitionPins(void) {
#if EFI_PROD_CODE
	for (int i = 0; i < IGNITION_PIN_COUNT; i++) {
		unregisterOutputIfPinOrModeChanged(enginePins.coils[i], ignitionPins[i], ignitionPinMode);
	}
#endif /* EFI_PROD_CODE */
}

void EnginePins::stopInjectionPins(void) {
#if EFI_PROD_CODE
	for (int i = 0; i < INJECTION_PIN_COUNT; i++) {
		unregisterOutputIfPinOrModeChanged(enginePins.injectors[i], injectionPins[i], injectionPinMode);
	}
#endif /* EFI_PROD_CODE */
}

void EnginePins::startAuxValves(void) {
#if EFI_PROD_CODE
	for (int i = 0; i < AUX_DIGITAL_VALVE_COUNT; i++) {
		NamedOutputPin *output = &enginePins.auxValve[i];
		output->initPin(output->name, engineConfiguration->auxValves[i]);
	}
#endif /* EFI_PROD_CODE */
}

void EnginePins::startIgnitionPins(void) {
#if EFI_PROD_CODE
	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		NamedOutputPin *output = &enginePins.coils[i];
		if (isPinOrModeChanged(ignitionPins[i], ignitionPinMode)) {
			output->initPin(output->name, CONFIG(ignitionPins)[i], &CONFIG(ignitionPinMode));
		}
	}
#endif /* EFI_PROD_CODE */
}

void EnginePins::startInjectionPins(void) {
#if EFI_PROD_CODE
	// todo: should we move this code closer to the injection logic?
	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		NamedOutputPin *output = &enginePins.injectors[i];
		if (isPinOrModeChanged(injectionPins[i], injectionPinMode)) {
			output->initPin(output->name, CONFIG(injectionPins)[i],
					&CONFIG(injectionPinMode));
		}
	}
#endif /* EFI_PROD_CODE */
}

NamedOutputPin::NamedOutputPin() : OutputPin() {
	name = NULL;
}

const char *NamedOutputPin::getName() const {
	return name;
}

const char *NamedOutputPin::getShortName() const {
	return shortName == NULL ? name : shortName;
}

NamedOutputPin::NamedOutputPin(const char *name) : OutputPin() {
	this->name = name;
}

void NamedOutputPin::setHigh() {
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
	// turn off the output
	setValue(false);

#if EFI_ENGINE_SNIFFER
	addEngineSnifferEvent(getShortName(), PROTOCOL_ES_DOWN);
#endif /* EFI_ENGINE_SNIFFER */
}

InjectorOutputPin::InjectorOutputPin() : NamedOutputPin() {
	reset();
	injectorIndex = -1;
}

bool NamedOutputPin::stop() {
#if EFI_GPIO_HARDWARE
	if (isInitialized() && getLogicValue()) {
		setValue(false);
		scheduleMsg(logger, "turning off %s", name);
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
	currentLogicValue = INITIAL_PIN_STATE;
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
	bool oldValue = currentLogicValue;
	setValue(logicValue);
	return oldValue;
}

void OutputPin::setValue(int logicValue) {
#if ENABLE_PERF_TRACE
// todo: https://github.com/rusefi/rusefi/issues/1638
//	ScopePerf perf(PE::OutputPinSetValue);
#endif // ENABLE_PERF_TRACE

#if EFI_PROD_CODE
	efiAssertVoid(CUSTOM_ERR_6621, modePtr!=NULL, "pin mode not initialized");
	pin_output_mode_e mode = *modePtr;
	efiAssertVoid(CUSTOM_ERR_6622, mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e");
	int eValue = getElectricalValue(logicValue, mode);

	#if (BOARD_EXT_GPIOCHIPS > 0)
		if (!this->ext) {
			/* onchip pin */
			if (port != GPIO_NULL) {
				setPinValue(this, eValue, logicValue);
			}
		} else {
			/* external pin */
			gpiochips_writePad(this->brainPin, logicValue);
			/* TODO: check return value */
			currentLogicValue = logicValue;
		}
	#else
		if (port != GPIO_NULL) {
			setPinValue(this, eValue, logicValue);
		}
	#endif

#else /* EFI_PROD_CODE */
	setPinValue(this, eValue, logicValue);
#endif /* EFI_PROD_CODE */
}

bool OutputPin::getLogicValue() const {
	return currentLogicValue;
}

void OutputPin::setDefaultPinState(const pin_output_mode_e *outputMode) {
	pin_output_mode_e mode = *outputMode;
	/* may be*/UNUSED(mode);
	assertOMode(mode);
	this->modePtr = outputMode;
	setValue(false); // initial state
}

void initOutputPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_GPIO_HARDWARE
	/**
	 * want to make sure it's all zeros so that we can compare in initOutputPinExt() method
	 */
// todo: it's too late to clear now? this breaks default status LEDs
// todo: fix this?
//	memset(&outputs, 0, sizeof(outputs));

#if HAL_USE_SPI
	enginePins.sdCsPin.initPin("SD CS", CONFIG(sdCardCsPin));
#endif /* HAL_USE_SPI */

	// todo: should we move this code closer to the fuel pump logic?
	enginePins.fuelPumpRelay.initPin("Fuel pump", CONFIG(fuelPumpPin), &CONFIG(fuelPumpPinMode));

	enginePins.mainRelay.initPin("Main relay", CONFIG(mainRelayPin), &CONFIG(mainRelayPinMode));
	enginePins.starterRelayDisable.initPin("Starter disable", CONFIG(starterRelayDisablePin), &CONFIG(starterRelayDisableMode));
	enginePins.starterControl.initPin("Starter control", CONFIG(starterControlPin));

	enginePins.fanRelay.initPin("Fan", CONFIG(fanPin), &CONFIG(fanPinMode));
	enginePins.o2heater.initPin("O2 heater", CONFIG(o2heaterPin));
	enginePins.acRelay.initPin("A/C relay", CONFIG(acRelayPin), &CONFIG(acRelayPinMode));

	// digit 1
	/*
	 ledRegister(LED_HUGE_0, GPIOB, 2);
	 ledRegister(LED_HUGE_1, GPIOE, 7);
	 ledRegister(LED_HUGE_2, GPIOE, 8);
	 ledRegister(LED_HUGE_3, GPIOE, 9);
	 ledRegister(LED_HUGE_4, GPIOE, 10);
	 ledRegister(LED_HUGE_5, GPIOE, 11);
	 ledRegister(LED_HUGE_6, GPIOE, 12);

	 // digit 2
	 ledRegister(LED_HUGE_7, GPIOE, 13);
	 ledRegister(LED_HUGE_8, GPIOE, 14);
	 ledRegister(LED_HUGE_9, GPIOE, 15);
	 ledRegister(LED_HUGE_10, GPIOB, 10);
	 ledRegister(LED_HUGE_11, GPIOB, 11);
	 ledRegister(LED_HUGE_12, GPIOB, 12);
	 ledRegister(LED_HUGE_13, GPIOB, 13);

	 // digit 3
	 ledRegister(LED_HUGE_14, GPIOE, 0);
	 ledRegister(LED_HUGE_15, GPIOE, 2);
	 ledRegister(LED_HUGE_16, GPIOE, 4);
	 ledRegister(LED_HUGE_17, GPIOE, 6);
	 ledRegister(LED_HUGE_18, GPIOE, 5);
	 ledRegister(LED_HUGE_19, GPIOE, 3);
	 ledRegister(LED_HUGE_20, GPIOE, 1);
	 */
#endif /* EFI_GPIO_HARDWARE */
}

void OutputPin::initPin(const char *msg, brain_pin_e brainPin) {
	initPin(msg, brainPin, &DEFAULT_OUTPUT);
}

void OutputPin::initPin(const char *msg, brain_pin_e brainPin, const pin_output_mode_e *outputMode) {
#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	if (brainPin == GPIO_UNASSIGNED)
		return;

	if (*outputMode > OM_OPENDRAIN_INVERTED) {
		firmwareError(CUSTOM_INVALID_MODE_SETTING, "%s invalid pin_output_mode_e", msg);
		return;
	}
	iomode_t mode = (*outputMode == OM_DEFAULT || *outputMode == OM_INVERTED) ?
		PAL_MODE_OUTPUT_PUSHPULL : PAL_MODE_OUTPUT_OPENDRAIN;

	#if (BOARD_EXT_GPIOCHIPS > 0)
		this->ext = false;
	#endif
	if (brain_pin_is_onchip(brainPin)) {
		ioportid_t port = getHwPort(msg, brainPin);
		int pin = getHwPin(msg, brainPin);

		/**
		 * This method is used for digital GPIO pins only, for peripheral pins see mySetPadMode
		 */
		if (port == GPIO_NULL) {
			// that's for GRIO_NONE
			this->port = port;
			return;
		}

		/**
		 * @brief Initialize the hardware output pin while also assigning it a logical name
		 */
		if (this->port != NULL && (this->port != port || this->pin != pin)) {
			/**
			 * here we check if another physical pin is already assigned to this logical output
			 */
		// todo: need to clear '&outputs' in io_pins.c
			warning(CUSTOM_OBD_PIN_CONFLICT, "outputPin [%s] already assigned to %x%d", msg, this->port, this->pin);
			engine->withError = true;
			return;
		}
		this->port = port;
		this->pin = pin;
	}
	#if (BOARD_EXT_GPIOCHIPS > 0)
		else {
			this->ext = true;
			this->brainPin = brainPin;
		}
	#endif

	this->currentLogicValue = INITIAL_PIN_STATE;

	// The order of the next two calls may look strange, which is a good observation.
	// We call them in this order so that the pin is set to a known state BEFORE
	// it's enabled.  Enabling the pin then setting it could result in a (brief)
	// mystery state being driven on the pin (potentially dangerous).
	setDefaultPinState(outputMode);
	efiSetPadMode(msg, brainPin, mode);
#endif /* EFI_GPIO_HARDWARE */
}

void OutputPin::unregisterOutput(brain_pin_e oldPin) {
	if (oldPin != GPIO_UNASSIGNED) {
		scheduleMsg(logger, "unregistering %s", hwPortname(oldPin));
#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
		brain_pin_markUnused(oldPin);
		port = nullptr;
#endif /* EFI_GPIO_HARDWARE */
	}
}

#if EFI_GPIO_HARDWARE

// questionable trick: we avoid using 'getHwPort' and 'getHwPin' in case of errors in order to increase the changes of turning the LED
// by reducing stack requirement
ioportid_t criticalErrorLedPort;
ioportmask_t criticalErrorLedPin;

void initPrimaryPins(Logging *sharedLogger) {
	logger = sharedLogger;
#if EFI_PROD_CODE
	enginePins.errorLedPin.initPin("led: CRITICAL status", LED_CRITICAL_ERROR_BRAIN_PIN);
	criticalErrorLedPort = getHwPort("CRITICAL", LED_CRITICAL_ERROR_BRAIN_PIN);
	criticalErrorLedPin = getHwPin("CRITICAL", LED_CRITICAL_ERROR_BRAIN_PIN);
#endif /* EFI_PROD_CODE */
}

/**
 * This method is part of fatal error handling.
 * Please note that worst case scenario the pins might get re-enabled by some other code :(
 * The whole method is pretty naive, but that's at least something.
 */
void turnAllPinsOff(void) {
	for (int i = 0; i < INJECTION_PIN_COUNT; i++) {
		enginePins.injectors[i].setValue(false);
	}
	for (int i = 0; i < IGNITION_PIN_COUNT; i++) {
		enginePins.coils[i].setValue(false);
	}
}
#endif /* EFI_GPIO_HARDWARE */
