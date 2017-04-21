/**
 * @file	efiGpio.cpp
 * @brief	EFI-related GPIO code
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "main.h"
#include "engine.h"
#include "efiGpio.h"

#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)
#include "io_pins.h"
#endif /* EFI_GPIO_HARDWARE */

EXTERN_ENGINE;

// todo: clean this mess, this should become 'static'/private
EnginePins enginePins;
extern LoggingWithStorage sharedLogger;

static pin_output_mode_e OUTPUT_MODE_DEFAULT = OM_DEFAULT;

static const char *sparkNames[IGNITION_PIN_COUNT] = { "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8",
		"c9", "cA", "cB", "cD"};

static const char *injectorNames[INJECTION_PIN_COUNT] = { "i1", "i2", "i3", "i4", "i5", "i6", "i7", "i8",
		"j9", "iA", "iB", "iC"};

EnginePins::EnginePins() {
	dizzyOutput.name = DIZZY_NAME;
	tachOut.name = TACH_NAME;

	for (int i = 0; i < IGNITION_PIN_COUNT;i++) {
		enginePins.coils[i].name = sparkNames[i];
	}
	for (int i = 0; i < INJECTION_PIN_COUNT;i++) {
		enginePins.injectors[i].injectorIndex = i;
		enginePins.injectors[i].name = injectorNames[i];
	}
}

/**
 * Sets the value of the pin. On this layer the value is assigned as is, without any conversion.
 */

#if EFI_PROD_CODE                                                                  \

#define setPinValue(outputPin, electricalValue, logicValue)                        \
  {                                                                                \
    if ((outputPin)->currentLogicValue != (logicValue)) {                          \
	  palWritePad((outputPin)->port, (outputPin)->pin, (electricalValue));         \
	  (outputPin)->currentLogicValue = (logicValue);                               \
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
	return result;
}

void EnginePins::reset() {
	for (int i = 0; i < INJECTION_PIN_COUNT;i++) {
		injectors[i].reset();
	}
	for (int i = 0; i < IGNITION_PIN_COUNT;i++) {
		coils[i].reset();
	}
}

NamedOutputPin::NamedOutputPin() : OutputPin() {
	name = NULL;
}

NamedOutputPin::NamedOutputPin(const char *name) : OutputPin() {
	this->name = name;
}

InjectorOutputPin::InjectorOutputPin() : NamedOutputPin() {
	reset();
	injectorIndex = -1;
}

bool NamedOutputPin::stop() {
#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)
	if (isInitialized() && getLogicValue()) {
		setValue(false);
		scheduleMsg(&sharedLogger, "turning off %s", name);
		return true;
	}
#endif /* EFI_GPIO_HARDWARE */
	return false;
}

void InjectorOutputPin::reset() {
	overlappingScheduleOffTime = 0;
	cancelNextTurningInjectorOff = false;
	overlappingCounter = 0;
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
	modePtr = &OUTPUT_MODE_DEFAULT;
#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)
	port = NULL;
	pin = 0;
#endif /* EFI_GPIO_HARDWARE */
	currentLogicValue = INITIAL_PIN_STATE;
}

bool OutputPin::isInitialized() {
#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)
	return port != NULL;
#else /* EFI_GPIO_HARDWARE */
	return false;
#endif /* EFI_GPIO_HARDWARE */
}

void OutputPin::setValue(int logicValue) {
#if EFI_PROD_CODE
	if (port != GPIO_NULL) {
		efiAssertVoid(modePtr!=NULL, "pin mode not initialized");
		pin_output_mode_e mode = *modePtr;
		efiAssertVoid(mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e");
		int eValue = getElectricalValue(logicValue, mode);
		setPinValue(this, eValue, logicValue);
	}

#else /* EFI_PROD_CODE */
	setPinValue(this, eValue, logicValue);
#endif /* EFI_PROD_CODE */
}

bool OutputPin::getLogicValue() {
	return currentLogicValue;
}

void OutputPin::unregister() {
#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)
	port = NULL;
#endif /* EFI_PROD_CODE */
}

void OutputPin::setDefaultPinState(pin_output_mode_e *outputMode) {
	pin_output_mode_e mode = *outputMode;
	assertOMode(mode);
	this->modePtr = outputMode;
	setValue(false); // initial state
}

pin_output_mode_e DEFAULT_OUTPUT = OM_DEFAULT;
pin_output_mode_e OPENDRAIN_OUTPUT = OM_OPENDRAIN;

void initOutputPins(void) {
#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)
	/**
	 * want to make sure it's all zeros so that we can compare in initOutputPinExt() method
	 */
// todo: it's too late to clear now? this breaks default status LEDs
// todo: fix this?
//	memset(&outputs, 0, sizeof(outputs));
//	outputPinRegister("ext led 1", LED_EXT_1, EXTRA_LED_1_PORT, EXTRA_LED_1_PIN);
//	outputPinRegister("ext led 2", LED_EXT_2, EXTRA_LED_2_PORT, EXTRA_LED_2_PIN);
//	outputPinRegister("ext led 3", LED_EXT_3, EXTRA_LED_2_PORT, EXTRA_LED_3_PIN);
//	outputPinRegister("alive1", LED_DEBUG, GPIOD, 6);

#if HAL_USE_SPI || defined(__DOXYGEN__)
	outputPinRegisterExt2("spi CS5", &enginePins.sdCsPin, boardConfiguration->sdCardCsPin, &DEFAULT_OUTPUT);
#endif /* HAL_USE_SPI */

	// todo: should we move this code closer to the fuel pump logic?
	outputPinRegisterExt2("fuel pump relay", &enginePins.fuelPumpRelay, boardConfiguration->fuelPumpPin, &DEFAULT_OUTPUT);

	outputPinRegisterExt2("main relay", &enginePins.mainRelay, boardConfiguration->mainRelayPin, &boardConfiguration->mainRelayPinMode);

	outputPinRegisterExt2("fan relay", &enginePins.fanRelay, boardConfiguration->fanPin, &DEFAULT_OUTPUT);
	outputPinRegisterExt2("o2 heater", &enginePins.o2heater, boardConfiguration->o2heaterPin, &DEFAULT_OUTPUT);
	outputPinRegisterExt2("A/C relay", &enginePins.acRelay, boardConfiguration->acRelayPin, &boardConfiguration->acRelayPinMode);

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

#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)

/**
 * This method is used for digital GPIO pins only, for peripheral pins see mySetPadMode
 */
static void outputPinRegisterExt(const char *msg, OutputPin *output, ioportid_t port, uint32_t pin,
		pin_output_mode_e *outputMode) {
#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)
	if (port == GPIO_NULL) {
		// that's for GRIO_NONE
		output->port = port;
		return;
	}

	assertOMode(*outputMode);
	iomode_t mode = (*outputMode == OM_DEFAULT || *outputMode == OM_INVERTED) ?
			PAL_MODE_OUTPUT_PUSHPULL : PAL_MODE_OUTPUT_OPENDRAIN;

	initOutputPinExt(msg, output, port, pin, mode);

	output->setDefaultPinState(outputMode);
#endif /* EFI_GPIO_HARDWARE */
}

void outputPinRegister(const char *msg, OutputPin *output, ioportid_t port, uint32_t pin) {
	outputPinRegisterExt(msg, output, port, pin, &DEFAULT_OUTPUT);
}

void initPrimaryPins(void) {
	outputPinRegister("led: ERROR status", &enginePins.errorLedPin, LED_ERROR_PORT, LED_ERROR_PIN);
}

void outputPinRegisterExt2(const char *msg, OutputPin *output, brain_pin_e brainPin, pin_output_mode_e *outputMode) {
	if (brainPin == GPIO_UNASSIGNED)
		return;
	ioportid_t hwPort = getHwPort(brainPin);
	int hwPin = getHwPin(brainPin);

	outputPinRegisterExt(msg, output, hwPort, hwPin, outputMode);
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


/**
 * @deprecated - use hwPortname() instead
 */
const char *portname(ioportid_t GPIOx) {
	if (GPIOx == GPIOA)
		return "PA";
	if (GPIOx == GPIOB)
		return "PB";
	if (GPIOx == GPIOC)
		return "PC";
	if (GPIOx == GPIOD)
		return "PD";
#if defined(STM32F4XX)
	if (GPIOx == GPIOE)
		return "PE";
	if (GPIOx == GPIOH)
		return "PH";
#endif
	if (GPIOx == GPIOF)
		return "PF";
	return "unknown";
}

#else /* EFI_GPIO_HARDWARE */
const char *hwPortname(brain_pin_e brainPin) {
	(void)brainPin;
	return "N/A";
}
#endif /* EFI_GPIO_HARDWARE */


