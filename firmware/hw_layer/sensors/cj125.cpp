/*
 * @file CJ125.cpp
 *
 * https://github.com/rusefi/rusefi/tree/master/hardware/CJ125_board
 *
 * https://github.com/rusefi/hw_modular/tree/master/cj125_Module
 *
 *
 * See vag_18_Turbo for test configuration
 * set engine_type 102
 *
 * @date: Jun 24, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 */

#include "pch.h"
#include "cj125.h"
#include "thread_priority.h"

#if EFI_CJ125

#include "adc_inputs.h"

#if HAL_USE_SPI
#include "mpu_util.h"
#endif /* HAL_USE_SPI */

//#define CJ125_DEBUG
//#define CJ125_DEBUG_SPI

#include "hardware.h"
#include "backup_ram.h"

static CJ125 globalInstance;

#if ! EFI_UNIT_TEST
static THD_WORKING_AREA(cj125ThreadStack, UTILITY_THREAD_STACK_SIZE);
#endif /* EFI_UNIT_TEST */

#if HAL_USE_SPI

static SPIDriver *driver;

static SPIConfig cj125spicfg = {
		.circular = false,
		.end_cb = NULL,
		.ssport = NULL,
		.sspad = 0,
		.cr1 =
			SPI_CR1_MSTR | SPI_CR1_CPHA |
			SPI_CR1_8BIT_MODE,
		.cr2 =
			SPI_CR2_8BIT_MODE
};

#endif /* HAL_USE_SPI */

static uint32_t lastSlowAdcCounter = 0;

// LSU conversion tables. See cj125_sensor_type_e
// For LSU4.2, See http://www.bosch-motorsport.com/media/catalog_resources/Lambda_Sensor_LSU_42_Datasheet_51_en_2779111435pdf.pdf
// See LSU4.9, See http://www.bosch-motorsport.com/media/catalog_resources/Lambda_Sensor_LSU_49_Datasheet_51_en_2779147659pdf.pdf

// Pump current, mA
static constexpr float pumpCurrentLsu42[] = {
	// LSU 4.2
	-1.85f, -1.08f, -0.76f, -0.47f, 0.0f, 0.34f, 0.68f, 0.95f, 1.4f 
};

static constexpr float pumpCurrentLsu49[] = {
	// LSU 4.9
	-2.0f, -1.602f, -1.243f, -0.927f, -0.8f, -0.652f, -0.405f, -0.183f, -0.106f, -0.04f, 0, 0.015f, 0.097f, 0.193f, 0.250f, 0.329f, 0.671f, 0.938f, 1.150f, 1.385f, 1.700f, 2.000f, 2.150f, 2.250f
};

// Corresponding lambda values for the above pump current
static constexpr float lambdaLsu42[] = {
	// LSU 4.2
	0.7f, 0.8f, 0.85f, 0.9f, 1.009f, 1.18f, 1.43f, 1.7f, 2.42f
};

static constexpr float lambdaLsu49[] = {
	// LSU 4.9
	0.65f, 0.7f, 0.75f, 0.8f, 0.822f, 0.85f, 0.9f, 0.95f, 0.97f, 0.99f, 1.003f, 1.01f, 1.05f, 1.1f, 1.132f, 1.179f, 1.429f, 1.701f, 1.990f, 2.434f, 3.413f, 5.391f, 7.506f, 10.119f
};


static uint8_t cjReadRegister(uint8_t regAddr) {
#if ! EFI_UNIT_TEST
	spiSelect(driver);
	spiPolledExchange(driver, regAddr);
	uint8_t result = spiPolledExchange(driver, 0xFF);
	spiUnselect(driver);

#ifdef CJ125_DEBUG_SPI
	efiPrintf("cjReadRegister: addr=%d answer=%d", regAddr, result);
#endif /* CJ125_DEBUG_SPI */
	return result;
#else /* EFI_UNIT_TEST */
	return 0;
#endif /* EFI_UNIT_TEST */
}

static void cjWriteRegister(uint8_t regAddr, uint8_t regValue) {
#ifdef CJ125_DEBUG_SPI
	efiPrintf("cjWriteRegister: addr=%d value=%d", regAddr, regValue);
#endif /* CJ125_DEBUG_SPI */
	// todo: extract 'sendSync' method?
#if HAL_USE_SPI
	spiSelect(driver);
	spiPolledExchange(driver, regAddr);
	spiPolledExchange(driver, regValue);
	spiUnselect(driver);
#endif /* HAL_USE_SPI */
}

static float getUr() {
#if ! EFI_UNIT_TEST
	if (isAdcChannelValid(CONFIG(cj125ur))) {
#if EFI_PROD_CODE
	if (!engineConfiguration->cj125isUrDivided) {
		// in case of directly connected Ur signal from CJ125 to the ADC pin of MCU
		return getVoltage("cj125ur", CONFIG(cj125ur) PASS_ENGINE_PARAMETER_SUFFIX);
	} else {
		// if a standard voltage division scheme with OpAmp is used
		return getVoltageDivided("cj125ur", CONFIG(cj125ur) PASS_ENGINE_PARAMETER_SUFFIX);
	}
#endif /* EFI_PROD_CODE */
	}
	return 0.0f;
#else /* EFI_UNIT_TEST */
	return 0;
#endif /* EFI_UNIT_TEST */
}

static float getUa() {
#if ! EFI_UNIT_TEST
	if (isAdcChannelValid(CONFIG(cj125ua))) {
#if EFI_PROD_CODE
		if (engineConfiguration->cj125isUaDivided) {
			return getVoltageDivided("cj125ua", CONFIG(cj125ua) PASS_ENGINE_PARAMETER_SUFFIX);
		} else {
			return getVoltage("cj125ua", CONFIG(cj125ua));
		}
#endif /* EFI_PROD_CODE */
	}

	return 0.0f;
#else /* EFI_UNIT_TEST */
	return 0;
#endif /* EFI_UNIT_TEST */
}

static float getVoltageFrom16bit(uint32_t stored) {
	return ((float)stored) / CJ125_VOLTAGE_TO_16BIT_FACTOR;
}

static uint32_t get16bitFromVoltage(float v) {
	return (uint32_t)(v * CJ125_VOLTAGE_TO_16BIT_FACTOR);
}

static const char * getCjState(cj125_state_e stateCode) {
	switch (stateCode) {
	case CJ125_INIT:
		return "INIT";
	case CJ125_IDLE:
		return "IDLE";
	case CJ125_CALIBRATION:
		return "CALIBRATION";
	case CJ125_PREHEAT:
		return "PREHEAT";
	case CJ125_HEAT_UP:
		return "HEAT UP";
	case CJ125_READY:
		return "READY";
	case CJ125_OVERHEAT:
		return "OVERHEAT";
	case CJ125_ERROR:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}

static void cjPrintErrorCode(cj125_error_e errCode) {
	const char *errString = nullptr;
	switch (errCode) {
	case CJ125_ERROR_HEATER_MALFUNCTION:
		errString = "Heater malfunction (Too long preheat)"; 
		break;
	case CJ125_ERROR_OVERHEAT:
		errString = "Sensor overheating";
		break;
	case CJ125_NO_ERROR:
		errString = "N/A";
		break;
	case CJ125_ERROR_WRONG_IDENT:
		errString = "W_IDENT";
		break;
	case CJ125_ERROR_WRONG_INIT:
		errString = "W_INIT";
		break;
	case CJ125_ERROR_DISABLED:
		errString = "DISABLED";
		break;
	}
	efiPrintf("cj125 ERROR: %s.", errString);
}

static void cjPrintState() {
	efiPrintf("cj125: state=%s diag=0x%x (current Ua=%.3f Ur=%.3f) (calibration Ua=%.3f Ur=%.3f)",
			getCjState(globalInstance.state), globalInstance.diag,
			globalInstance.vUa, globalInstance.vUr,
			globalInstance.vUaCal, globalInstance.vUrCal);

	globalInstance.printDiag();

	if (globalInstance.state == CJ125_ERROR) {
		cjPrintErrorCode(globalInstance.errorCode);
	}

	efiPrintf("cj125 P=%f I=%f D=%f",
			globalInstance.heaterPidConfig.pFactor,
			globalInstance.heaterPidConfig.iFactor,
			globalInstance.heaterPidConfig.dFactor);
}

static void cjSetP(float value) {
	globalInstance.heaterPidConfig.pFactor = value;
}

static void cjSetI(float value) {
	globalInstance.heaterPidConfig.iFactor = value;
}

static void cjInfo() {
	cjPrintState();
#if HAL_USE_SPI
	printSpiConfig("cj125", CONFIG(cj125SpiDevice));
#endif /* HAL_USE_SPI */
}

static void cjPrintData() {
#if ! EFI_UNIT_TEST
	if (engineConfiguration->isCJ125Verbose) {
		cjPrintState();
	}
#endif /* EFI_UNIT_TEST */
}

class RealSpi : public Cj125SpiStream {
public:
	uint8_t ReadRegister(uint8_t reg) override {
		return cjReadRegister(reg);
	}

	void WriteRegister(uint8_t regAddr, uint8_t regValue) {
		cjWriteRegister(regAddr, regValue);
	}
};

static RealSpi spi;

static void cjUpdateAnalogValues() {
#if EFI_PROD_CODE
	// todo: some solution for testing
	waitForSlowAdc(lastSlowAdcCounter);
#endif /* EFI_PROD_CODE */

	globalInstance.vUr = getUr();
	globalInstance.vUa = getUa();
#if EFI_PROD_CODE
	// todo: some solution for testing
    lastSlowAdcCounter = getSlowAdcCounter();
#endif /* EFI_PROD_CODE */
}

void CJ125::calibrate(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	cjIdentify(PASS_ENGINE_PARAMETER_SIGNATURE);

	efiPrintf("cj125: Starting calibration...");
	cjSetMode(CJ125_MODE_CALIBRATION);
	int init1 = cjReadRegister(INIT_REG1_RD);
	// check if our command has been accepted
	if (init1 != CJ125_INIT1_CALBRT) {
		efiPrintf("cj125: Calibration error (init1=0x%02x)! Failed!", init1);
		cjSetMode(CJ125_MODE_NORMAL_17);
		return;
	}
#if EFI_PROD_CODE
	// todo: some testing solution
	// wait for the start of the calibration
	chThdSleepMilliseconds(CJ125_CALIBRATION_DELAY);
#endif /* EFI_PROD_CODE */
	vUaCal = 0.0f;
	vUrCal = 0.0f;
	// wait for some more ADC samples
	for (int i = 0; i < CJ125_CALIBRATE_NUM_SAMPLES; i++) {
		cjUpdateAnalogValues();
		cjPrintData();

#if EFI_TUNER_STUDIO
		if (engineConfiguration->debugMode == DBG_CJ125) {
			cjPostState(&tsOutputChannels);
		}
#endif /* EFI_TUNER_STUDIO */

		vUaCal += vUa;
		vUrCal += vUr;
	}
	// find average
	vUaCal /= (float)CJ125_CALIBRATE_NUM_SAMPLES;
	vUrCal /= (float)CJ125_CALIBRATE_NUM_SAMPLES;
	// restore normal mode
	cjSetMode(CJ125_MODE_NORMAL_17);
#if EFI_PROD_CODE
	// todo: testing solution
	chThdSleepMilliseconds(CJ125_CALIBRATION_DELAY);
#endif
	// check if everything is ok
	diag = cjReadRegister(DIAG_REG_RD);
	cjUpdateAnalogValues();
	cjPrintData();

	// store new calibration data
	uint32_t storedLambda = get16bitFromVoltage(vUaCal);
	uint32_t storedHeater = get16bitFromVoltage(vUrCal);
	efiPrintf("cj125: Done! Saving calibration data (%d %d).", storedLambda, storedHeater);
#if EFI_PROD_CODE
	backupRamSave(BACKUP_CJ125_CALIBRATION_LAMBDA, storedLambda);
	backupRamSave(BACKUP_CJ125_CALIBRATION_HEATER, storedHeater);
#endif /* EFI_PROD_CODE */

	state = CJ125_IDLE;
}

static void cjStart(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!CONFIG(isCJ125Enabled)) {
		efiPrintf("cj125 is disabled.");
		return;
	}
	
	globalInstance.cjIdentify(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Load calibration values
#if EFI_PROD_CODE
	uint32_t storedLambda = backupRamLoad(BACKUP_CJ125_CALIBRATION_LAMBDA);
	uint32_t storedHeater = backupRamLoad(BACKUP_CJ125_CALIBRATION_HEATER);
#else
	uint32_t storedLambda = 0;
	uint32_t storedHeater = 0;
#endif
	// if no calibration, try to calibrate now and store new values
	if (storedLambda == 0 || storedHeater == 0) {
		/**
		 * open question if we need special considerations for calibration. Some controllers insist on open air calibration
		 */
		globalInstance.calibrate(PASS_ENGINE_PARAMETER_SIGNATURE);
	} else {
		efiPrintf("cj125: Loading stored calibration data (%d %d)", storedLambda, storedHeater);
		globalInstance.vUaCal = getVoltageFrom16bit(storedLambda);
		globalInstance.vUrCal = getVoltageFrom16bit(storedHeater);
		// Start normal measurement mode
		globalInstance.cjSetMode(CJ125_MODE_NORMAL_17);
	}
	cjPrintData();

#if EFI_PROD_CODE
	// todo: testig solution
	lastSlowAdcCounter = getSlowAdcCounter();
#endif
}

void CJ125::setError(cj125_error_e errCode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	errorCode = errCode;
	state = CJ125_ERROR;
	cjPrintErrorCode(errorCode);
	// This is for safety:
	efiPrintf("cj125: Controller Shutdown!");
	SetHeater(0 PASS_ENGINE_PARAMETER_SUFFIX);
	// Software-reset of CJ125
	cjWriteRegister(INIT_REG2_WR, CJ125_INIT2_RESET);
}

static bool cjStartSpi(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if HAL_USE_SPI
	globalInstance.cj125Cs.initPin("cj125 CS", CONFIG(cj125CsPin),
			&engineConfiguration->cj125CsPinMode);
	// Idle CS pin - SPI CS is high when idle
	globalInstance.cj125Cs.setValue(true);

	cj125spicfg.cr1 += getSpiPrescaler(_150KHz, CONFIG(cj125SpiDevice));

	cj125spicfg.ssport = getHwPort("cj125", CONFIG(cj125CsPin));
	cj125spicfg.sspad = getHwPin("cj125", CONFIG(cj125CsPin));
	driver = getSpiDevice(engineConfiguration->cj125SpiDevice);
	if (driver == NULL) {
		// error already reported
		return false;
	}
	efiPrintf("cj125: Starting SPI driver %s", getSpi_device_e(engineConfiguration->cj125SpiDevice));
	spiStart(driver, &cj125spicfg);
#endif /* HAL_USE_SPI */
	return true;
}

/**
 * @return true if currently in IDLE or ERROR state
 */
static bool cj125periodic(CJ125 *instance DECLARE_ENGINE_PARAMETER_SUFFIX) {
	{
	efitick_t nowNt = getTimeNowNt();
		bool isStopped = engine->rpmCalculator.isStopped();

		cjUpdateAnalogValues();
		
		// If the controller is disabled
		if (instance->state == CJ125_IDLE || instance->state == CJ125_ERROR) {
			return true;
		}

		if (instance->state == CJ125_CALIBRATION) {
			globalInstance.calibrate(PASS_ENGINE_PARAMETER_SIGNATURE);
			// Start normal operation
			instance->state = CJ125_INIT;
			globalInstance.cjSetMode(CJ125_MODE_NORMAL_17);
		}
		
		globalInstance.diag = cjReadRegister(DIAG_REG_RD);

		// check heater state
		if (globalInstance.vUr > CJ125_UR_PREHEAT_THR || instance->heaterDuty < CJ125_PREHEAT_MIN_DUTY) {
			// Check if RPM>0 and it's time to start pre-heating
			if (instance->state == CJ125_INIT && !isStopped) {
				// start preheating
				instance->state = CJ125_PREHEAT;
				instance->startHeatingNt = instance->prevNt = getTimeNowNt();
				globalInstance.cjSetMode(CJ125_MODE_NORMAL_17);
			}
		} else if (instance->vUr > CJ125_UR_GOOD_THR) {
			instance->state = CJ125_HEAT_UP;
		} else if (instance->vUr < CJ125_UR_OVERHEAT_THR) {
			instance->state = CJ125_OVERHEAT;
		} else {
			// This indicates that the heater temperature is optimal for UA measurement
			instance->state = CJ125_READY;
		}

		if (isStopped && instance->isWorkingState()) {
			instance->state = CJ125_INIT;
			instance->SetIdleHeater(PASS_ENGINE_PARAMETER_SIGNATURE);
		}

#if 0
		// Change amplification if AFR gets lean/rich for better accuracy
		globalInstance.cjSetMode(globalInstance.lambda > 1.0f ? CJ125_MODE_NORMAL_17 : CJ125_MODE_NORMAL_8);
#endif

		switch (instance->state) {
		case CJ125_PREHEAT:
			// use constant-speed startup heat-up
			if (nowNt - instance->prevNt >= CJ125_HEATER_PREHEAT_PERIOD) {
				float periodSecs = (float)(nowNt - instance->prevNt) / NT_PER_SECOND;
				// maintain speed at ~0.4V/sec
				float preheatDuty = instance->heaterDuty + periodSecs * CJ125_HEATER_PREHEAT_RATE;
				instance->SetHeater(preheatDuty PASS_ENGINE_PARAMETER_SUFFIX);
				// If we are heating too long, and there's still no result, then something is wrong...
				if (nowNt - instance->startHeatingNt > NT_PER_SECOND * CJ125_PREHEAT_TIMEOUT) {
					instance->setError(CJ125_ERROR_HEATER_MALFUNCTION PASS_ENGINE_PARAMETER_SUFFIX);
				}
				cjPrintData();
				instance->prevNt = nowNt;
			}
			break;
		case CJ125_HEAT_UP:
		case CJ125_READY:
			// use PID for normal heater control
			if (nowNt - instance->prevNt >= CJ125_HEATER_CONTROL_PERIOD) {
				/* PID doesn't care about the target or the input, it knows only the
				 * error value as the difference of (target - input). and if we swap them we'll just get a sign inversion. If target=vUrCal, and input=vUr, then error=vUrCal-vUr, i.e. if vUr<vUrCal then the error will cause the heater to increase it's duty cycle. But it's not exactly what we want! Lesser vUr means HOTTER cell. That's why we even have this safety check for overheating: (vUr < CJ125_UR_OVERHEAT_THR)...
				 * So the simple trick is to inverse the error by swapping the target and input values.
				 */
				float duty = globalInstance.heaterPid.getOutput(globalInstance.vUr, globalInstance.vUrCal, MS2SEC(CJ125_TICK_DELAY));
				instance->SetHeater(duty PASS_ENGINE_PARAMETER_SUFFIX);
				if (engineConfiguration->isCJ125Verbose) {
					globalInstance.heaterPid.showPidStatus("cj heater");
					cjPrintData();
				}
				instance->prevNt = nowNt;
			}
			break;
		case CJ125_OVERHEAT:
			if (nowNt - instance->prevNt >= CJ125_HEATER_OVERHEAT_PERIOD) {
				instance->setError(CJ125_ERROR_OVERHEAT PASS_ENGINE_PARAMETER_SUFFIX);
				instance->prevNt = nowNt;
			}
		default:
			;
		}
}
	return false;
}

#if ! EFI_UNIT_TEST

static msg_t cjThread(void)
{
	chRegSetThreadName("cj125");

	chThdSleepMilliseconds(500);

	globalInstance.startHeatingNt = 0;
	globalInstance.prevNt = getTimeNowNt();
	while (1) {
		bool needIdleSleep = cj125periodic(&globalInstance PASS_ENGINE_PARAMETER_SUFFIX);
		chThdSleepMilliseconds(needIdleSleep ? CJ125_IDLE_TICK_DELAY : CJ125_TICK_DELAY);
	}
	return -1;
}

static bool cjCheckConfig(void) {
	if (!CONFIG(isCJ125Enabled)) {
		efiPrintf("cj125 is disabled. Failed!");
		return false;
	}
	return true;
}

void cjStartCalibration(void) {
	if (!cjCheckConfig())
		return;
	if (globalInstance.isWorkingState()) {
		// todo: change this later for the normal thread operation (auto pre-heating)
		efiPrintf("cj125: Cannot start calibration. Please restart the board and make sure that your sensor is not heating");
		return;
	}
	globalInstance.state = CJ125_CALIBRATION;
}

void cjRestart(void) {
	if (!cjCheckConfig())
		return;
	globalInstance.state = CJ125_INIT;
	globalInstance.errorCode = CJ125_NO_ERROR;
	cjInfo();
	cjStart(PASS_ENGINE_PARAMETER_SIGNATURE);
}
#endif /* EFI_UNIT_TEST */


#ifdef CJ125_DEBUG
static void cjSetInit1(int v) {
	cjWriteRegister(INIT_REG1_WR, v & 0xff);
	v = cjReadRegister(INIT_REG1_RD);
	efiPrintf("cj125 INIT_REG1=0x%02x.", v);
}

static void cjSetInit2(int v) {
	cjWriteRegister(INIT_REG2_WR, v & 0xff);
	v = cjReadRegister(INIT_REG2_RD);
	efiPrintf("cj125 INIT_REG2=0x%02x.", v);
}
#endif /* CJ125_DEBUG */

float cjGetAfr(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// See CJ125 datasheet, page 6
	float pumpCurrent = (globalInstance.vUa - globalInstance.vUaCal) * globalInstance.amplCoeff * (CJ125_PUMP_CURRENT_FACTOR / CJ125_PUMP_SHUNT_RESISTOR);
	
	if (engineConfiguration->cj125isLsu49) {
		globalInstance.lambda = interpolate2d(pumpCurrent, pumpCurrentLsu49, lambdaLsu49);
	} else {
		globalInstance.lambda = interpolate2d(pumpCurrent, pumpCurrentLsu42, lambdaLsu42);
	}

	// todo: make configurable stoich ratio
	return globalInstance.lambda * CJ125_STOICH_RATIO;
}

bool cjHasAfrSensor(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	if (!CONFIG(isCJ125Enabled))
		return false;
	return globalInstance.isValidState();
}

#if EFI_TUNER_STUDIO
// used by DBG_CJ125
void cjPostState(TunerStudioOutputChannels *tsOutputChannels) {
	tsOutputChannels->debugFloatField1 = globalInstance.heaterDuty;
	tsOutputChannels->debugFloatField2 = globalInstance.heaterPid.getIntegration();
	tsOutputChannels->debugFloatField3 = globalInstance.heaterPid.getPrevError();
	tsOutputChannels->debugFloatField4 = globalInstance.vUa;
	tsOutputChannels->debugFloatField5 = globalInstance.vUr;
	tsOutputChannels->debugFloatField6 = globalInstance.vUaCal;
	tsOutputChannels->debugFloatField7 = globalInstance.vUrCal;
	tsOutputChannels->debugIntField1 = globalInstance.state;
	tsOutputChannels->debugIntField2 = globalInstance.diag;
}
#endif /* EFI_TUNER_STUDIO */

void initCJ125(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	globalInstance.spi = &spi;

	if (!CONFIG(isCJ125Enabled)) {
		globalInstance.errorCode = CJ125_ERROR_DISABLED;
		return;
	}

	if (!isAdcChannelValid(CONFIG(cj125ur)) || !isAdcChannelValid(CONFIG(cj125ua))) {
		efiPrintf("cj125 init error! cj125ur and cj125ua are required.");
		warning(CUSTOM_CJ125_0, "cj ur ua");
		globalInstance.errorCode = CJ125_ERROR_DISABLED;
		return;
	}

	if (!isBrainPinValid(CONFIG(wboHeaterPin))) {
		efiPrintf("cj125 init error! wboHeaterPin is required.");
		warning(CUSTOM_CJ125_1, "cj heater");
		globalInstance.errorCode = CJ125_ERROR_DISABLED;
		return;
	}

	globalInstance.cjInitPid(PASS_ENGINE_PARAMETER_SIGNATURE);
	if (!cjStartSpi(PASS_ENGINE_PARAMETER_SIGNATURE))
		return;
	efiPrintf("cj125: Starting heater control");
	globalInstance.StartHeaterControl(PASS_ENGINE_PARAMETER_SIGNATURE);
	cjStart(PASS_ENGINE_PARAMETER_SIGNATURE);
	
#ifdef CJ125_DEBUG
//	addConsoleActionF("cj125_heater", cjConsoleSetHeater);
	addConsoleActionI("cj125_set_init1", cjSetInit1);
	addConsoleActionI("cj125_set_init2", cjSetInit2);
#endif /* CJ125_DEBUG */

#if ! EFI_UNIT_TEST
	addConsoleAction("cj125_info", cjInfo);
	addConsoleActionF("cj125_set_p", cjSetP);
	addConsoleActionF("cj125_set_i", cjSetI);
	addConsoleAction("cj125_restart", cjRestart);
	addConsoleAction("cj125_calibrate", cjStartCalibration);

	chThdCreateStatic(cj125ThreadStack, sizeof(cj125ThreadStack), PRIO_CJ125, (tfunc_t)(void*) cjThread, NULL);
#endif /* ! EFI_UNIT_TEST */
}

#endif /* EFI_CJ125 && HAL_USE_SPI */

#if EFI_CJ125
//	engineConfiguration->spi2SckMode = PAL_STM32_OTYPE_OPENDRAIN; // 4
//	engineConfiguration->spi2MosiMode = PAL_STM32_OTYPE_OPENDRAIN; // 4
//	engineConfiguration->spi2MisoMode = PAL_STM32_PUDR_PULLUP; // 32
//	CONFIG(cj125CsPin) = GPIOA_15;
//	engineConfiguration->cj125CsPinMode = OM_OPENDRAIN;

void cj125defaultPinout(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->cj125ua = EFI_ADC_13; // PC3
	engineConfiguration->cj125ur = EFI_ADC_4; // PA4
	CONFIG(wboHeaterPin) = GPIOC_13;

	CONFIG(spi2mosiPin) = GPIOB_15;
	CONFIG(spi2misoPin) = GPIOB_14;
	CONFIG(spi2sckPin) = GPIOB_13;

	CONFIG(cj125CsPin) = GPIOB_0;
	CONFIG(isCJ125Enabled) = true;
	CONFIG(is_enabled_spi_2) = true;
	CONFIG(cj125SpiDevice) = SPI_DEVICE_2;
}
#endif /* EFI_CJ125 */
