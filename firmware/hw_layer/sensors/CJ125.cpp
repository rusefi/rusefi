/*
 * @file CJ125.cpp
 *
 * @date: Jun 24, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 *
 */

#include "engine.h"
#include "CJ125.h"
#include "pwm_generator.h"
#include "rpm_calculator.h"
#include "pid.h"

#if EFI_CJ125 || defined(__DOXYGEN__)

// looks like 3v range should be enough, divider not needed
#define EFI_CJ125_DIRECTLY_CONNECTED_UR TRUE

#include "adc_inputs.h"

//#define CJ125_DEBUG
//#define CJ125_DEBUG_SPI

EXTERN_ENGINE;

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
#include "hardware.h"
#include "backup_ram.h"
#include "pin_repository.h"
extern TunerStudioOutputChannels tsOutputChannels;
#endif

struct CJ125_state {
	efitick_t startHeatingNt;
	efitick_t prevNt;
};

static SimplePwm wboHeaterControl("wbo");
static OutputPin wboHeaterPin;
static OutputPin cj125Cs;
static Logging *logger;
static unsigned char tx_buff[2];
static unsigned char rx_buff[1];

static pid_s heaterPidConfig;
static Pid heaterPid(&heaterPidConfig);
static float heaterDuty = 0.0f;

// todo: only define this variable in EIF_PROD
static CJ125 globalInstance;


#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)

static THD_WORKING_AREA(cjThreadStack, UTILITY_THREAD_STACK_SIZE);

static SPIDriver *driver;

#if EFI_PROD_CODE
static SPIConfig cj125spicfg = { NULL,
	/* HW dependent part.*/
	NULL, 0, SPI_CR1_MSTR | SPI_CR1_CPHA | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 };
#endif /* EFI_PROD_CODE */

#endif /* EFI_UNIT_TEST */

CJ125::CJ125() {
	state = CJ125_IDLE;
	errorCode = CJ125_ERROR_NONE;
}

// Chip diagnostics register contents
static volatile int diag = 0;

// Current values
static volatile float vUa = 0.0f;
static volatile float vUr = 0.0f;
// Calibration values
static volatile float vUaCal = 0.0f, vUrCal = 0.0f;

static volatile int lastSlowAdcCounter = 0;

static volatile cj125_mode_e mode = CJ125_MODE_NONE;
// Amplification coefficient, needed by cjGetAfr()
static volatile float amplCoeff = 0.0f;
// Calculated Lambda-value
static volatile float lambda = 1.0f;

// LSU conversion tables. See cj125_sensor_type_e
// For LSU4.2, See http://www.bosch-motorsport.com/media/catalog_resources/Lambda_Sensor_LSU_42_Datasheet_51_en_2779111435pdf.pdf
// See LSU4.9, See http://www.bosch-motorsport.com/media/catalog_resources/Lambda_Sensor_LSU_49_Datasheet_51_en_2779147659pdf.pdf
static const int CJ125_LSU_CURVE_SIZE = 25;
// This is a number of bins for each sensor type (should be < CJ125_LSU_CURVE_SIZE)
static const float cjLSUTableSize[2] = {
	9, 24,
};
// Pump current, mA
static const float cjLSUBins[2][CJ125_LSU_CURVE_SIZE] = {	{ 
	// LSU 4.2
	-1.85f, -1.08f, -0.76f, -0.47f, 0.0f, 0.34f, 0.68f, 0.95f, 1.4f }, { 
	// LSU 4.9
	-2.0f, -1.602f, -1.243f, -0.927f, -0.8f, -0.652f, -0.405f, -0.183f, -0.106f, -0.04f, 0, 0.015f, 0.097f, 0.193f, 0.250f, 0.329f, 0.671f, 0.938f, 1.150f, 1.385f, 1.700f, 2.000f, 2.150f, 2.250f },
};
// Lambda value
static const float cjLSULambda[2][CJ125_LSU_CURVE_SIZE] = { {
	// LSU 4.2
	0.7f, 0.8f, 0.85f, 0.9f, 1.009f, 1.18f, 1.43f, 1.7f, 2.42f }, {
	// LSU 4.9
	0.65f, 0.7f, 0.75f, 0.8f, 0.822f, 0.85f, 0.9f, 0.95f, 0.97f, 0.99f, 1.003f, 1.01f, 1.05f, 1.1f, 1.132f, 1.179f, 1.429f, 1.701f, 1.990f, 2.434f, 3.413f, 5.391f, 7.506f, 10.119f },
};


static int cjReadRegister(unsigned char regAddr) {
#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
	spiSelect(driver);
	tx_buff[0] = regAddr;
	spiSend(driver, 1, tx_buff);
	// safety?
	chThdSleepMilliseconds(10);
	
	rx_buff[0] = 0;
	spiReceive(driver, 1, rx_buff);
	spiUnselect(driver);

#ifdef CJ125_DEBUG_SPI
	scheduleMsg(logger, "cjReadRegister: addr=%d answer=%d", regAddr, rx_buff[0]);
#endif /* CJ125_DEBUG_SPI */
	return rx_buff[0];
#else /* EFI_UNIT_TEST */
	return 0;
#endif /* EFI_UNIT_TEST */
}


static void cjWriteRegister(unsigned char regAddr, unsigned char regValue) {
#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
#ifdef CJ125_DEBUG_SPI
	scheduleMsg(logger, "cjWriteRegister: addr=%d value=%d", regAddr, regValue);
#endif /* CJ125_DEBUG_SPI */
	spiSelect(driver);
	tx_buff[0] = regAddr;
	tx_buff[1] = regValue;
	spiSend(driver, 2, tx_buff);
	spiUnselect(driver);
#endif /* EFI_UNIT_TEST */
}

static float getUr() {
#if ! EFI_UNIT_TEST
	if (CONFIG(cj125ur) != EFI_ADC_NONE) {
#if EFI_PROD_CODE
#ifdef EFI_CJ125_DIRECTLY_CONNECTED_UR
		// in case of directly connected Ur signal from CJ125 to the ADC pin of MCU
		return getVoltage("cj125ur", CONFIG(cj125ur));
#else
		// if a standard voltage division scheme with OpAmp is used
		return getVoltageDivided("cj125ur", CONFIG(cj125ur));
#endif /* EFI_CJ125_DIRECTLY_CONNECTED_UR */
#endif /* EFI_PROD_CODE */
	}
	return 0.0f;
#else /* EFI_UNIT_TEST */
	return 0;
#endif /* EFI_UNIT_TEST */
}

static float getUa() {
#if ! EFI_UNIT_TEST
	if (CONFIG(cj125ua) != EFI_ADC_NONE) {
#if EFI_PROD_CODE
		if (engineConfiguration->cj125isUaDivided) {
			return getVoltageDivided("cj125ua", CONFIG(cj125ua));
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

static void cjPrintData(void) {
#ifdef CJ125_DEBUG
	scheduleMsg(logger, "cj125: state=%d diag=0x%x (vUa=%.3f vUr=%.3f) (vUaCal=%.3f vUrCal=%.3f)", state, diag, vUa, vUr, vUaCal, vUrCal);
#endif
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
	case CJ125_ERROR_NONE:
		errString = "N/A";
		break;
	case CJ125_ERROR_WRONG_IDENT:
		errString = "W_IDENT";
		break;
	case CJ125_ERROR_WRONG_INIT:
		errString = "W_INIT";
		break;
	}
	scheduleMsg(logger, "cj125 ERROR: %s.", errString);
}

static void cjSetMode(cj125_mode_e m) {
	if (mode == m)
		return;
	switch (m) {
	case CJ125_MODE_NORMAL_8:
		cjWriteRegister(INIT_REG1_WR, CJ125_INIT1_NORMAL_8);
		amplCoeff = 1.0f / 8.0f;
		break;
	case CJ125_MODE_NORMAL_17:
		cjWriteRegister(INIT_REG1_WR, CJ125_INIT1_NORMAL_17);
		amplCoeff = 1.0f / 17.0f;
		break;
	case CJ125_MODE_CALIBRATION:
		cjWriteRegister(INIT_REG1_WR, CJ125_INIT1_CALBRT);
		amplCoeff = 0.0f;
		break;
	default:
		;
	}
	mode = m;
}

static void cjIdentify(void) {
	// read Ident register
	int ident = cjReadRegister(IDENT_REG_RD) & CJ125_IDENT_MASK;

	// set initial registers
	cjWriteRegister(INIT_REG1_WR, CJ125_INIT1_NORMAL_17);
	cjWriteRegister(INIT_REG2_WR, CJ125_INIT2_DIAG);
	// check if regs are ok
	int init1 = cjReadRegister(INIT_REG1_RD);
	int init2 = cjReadRegister(INIT_REG2_RD);

	diag = cjReadRegister(DIAG_REG_RD);
	scheduleMsg(logger, "cj125: Check ident=0x%x diag=0x%x init1=0x%x init2=0x%x", ident, diag, init1, init2);
	if (ident != CJ125_IDENT) {
		scheduleMsg(logger, "cj125: Error! Wrong ident! Cannot communicate with CJ125!");
	}
	if (init1 != CJ125_INIT1_NORMAL_17 || init2 != CJ125_INIT2_DIAG) {
		scheduleMsg(logger, "cj125: Error! Cannot set init registers! Cannot communicate with CJ125!");
	}
#if 0
	if (diag != CJ125_DIAG_NORM) {
		scheduleMsg(logger, "cj125: Diag error!");
	}
#endif
}

static void cjUpdateAnalogValues() {
#if EFI_PROD_CODE
	// todo: some solution for testing
	waitForSlowAdc(lastSlowAdcCounter);
#endif
	vUr = getUr();
    vUa = getUa();
#if EFI_PROD_CODE
	// todo: some solution for testing
    lastSlowAdcCounter = getSlowAdcCounter();
#endif
}

static void cjCalibrate(void) {
	cjIdentify();

	scheduleMsg(logger, "cj125: Starting calibration...");
	cjSetMode(CJ125_MODE_CALIBRATION);
	int init1 = cjReadRegister(INIT_REG1_RD);
	// check if our command has been accepted
	if (init1 != CJ125_INIT1_CALBRT) {
		scheduleMsg(logger, "cj125: Calibration error (init1=0x%02x)! Failed!", init1);
		cjSetMode(CJ125_MODE_NORMAL_17);
		return;
	}
#if EFI_PROD_CODE
	// todo: some testing solution
	// wait for the start of the calibration
	chThdSleepMilliseconds(CJ125_CALIBRATION_DELAY);
#endif
	vUaCal = 0.0f;
	vUrCal = 0.0f;
	// wait for some more ADC samples
	for (int i = 0; i < CJ125_CALIBRATE_NUM_SAMPLES; i++) {
		cjUpdateAnalogValues();
		cjPrintData();

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
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
	scheduleMsg(logger, "cj125: Done! Saving calibration data (%d %d).", storedLambda, storedHeater);
#if EFI_PROD_CODE
	backupRamSave(BACKUP_CJ125_CALIBRATION_LAMBDA, storedLambda);
	backupRamSave(BACKUP_CJ125_CALIBRATION_HEATER, storedHeater);
#endif /* EFI_PROD_CODE */

	globalInstance.state = CJ125_IDLE;
}

static void cjStart(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!CONFIGB(isCJ125Enabled)) {
		scheduleMsg(logger, "cj125 is disabled.");
		return;
	}
	
	cjIdentify();

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
		cjCalibrate();
	} else {
		scheduleMsg(logger, "cj125: Loading stored calibration data (%d %d)", storedLambda, storedHeater);
		vUaCal = getVoltageFrom16bit(storedLambda);
		vUrCal = getVoltageFrom16bit(storedHeater);
		// Start normal measurement mode
		cjSetMode(CJ125_MODE_NORMAL_17);
	}
	cjPrintData();

#if EFI_PROD_CODE
	// todo: testig solution
	lastSlowAdcCounter = getSlowAdcCounter();
#endif
}

static void cjSetHeater(float value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// limit duty cycle for sensor safety
	float maxDuty = (engine->sensors.vBatt > CJ125_HEATER_LIMITING_VOLTAGE) ? CJ125_HEATER_LIMITING_RATE : 1.0f;
	heaterDuty = (value < CJ125_HEATER_MIN_DUTY) ? 0.0f : minF(maxF(value, 0.0f), maxDuty);
#ifdef CJ125_DEBUG
	scheduleMsg(logger, "cjSetHeater: %.2f", heaterDuty);
#endif
	// a little trick to disable PWM if needed.
	// todo: this should be moved to wboHeaterControl.setPwmDutyCycle()
	wboHeaterControl.setFrequency(heaterDuty == 0.0f ? NAN : CJ125_HEATER_PWM_FREQ);
	wboHeaterControl.setSimplePwmDutyCycle(heaterDuty);
}

static void cjSetIdleHeater(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// small preheat for faster start & moisture anti-shock therapy for the sensor
	cjSetHeater(CJ125_HEATER_IDLE_RATE PASS_ENGINE_PARAMETER_SUFFIX);
}

static void cjStartHeaterControl(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (CONFIGB(wboHeaterPin) != GPIO_UNASSIGNED) {
		scheduleMsg(logger, "cj125: Starting heater control");
		// todo: use custom pin state method, turn pin off while not running
		startSimplePwmExt(&wboHeaterControl, "wboHeaterPin",
				&engine->executor,
				CONFIGB(wboHeaterPin),
				&wboHeaterPin, CJ125_HEATER_PWM_FREQ, 0.0f, applyPinState);
		cjSetIdleHeater(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
}

void CJ125::cjSetError(cj125_error_e errCode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	errorCode = errCode;
	state = CJ125_ERROR;
	cjPrintErrorCode(errorCode);
	// This is for safety:
	scheduleMsg(logger, "cj125: Controller Shutdown!");
	cjSetHeater(0 PASS_ENGINE_PARAMETER_SUFFIX);
	// Software-reset of CJ125
	cjWriteRegister(INIT_REG2_WR, CJ125_INIT2_RESET);
}

bool CJ125::cjIsWorkingState(void) {
	return state != CJ125_ERROR && state != CJ125_INIT && state != CJ125_IDLE;
}

static void cjInitPid(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if(engineConfiguration->cj125isLsu49) {
		heaterPidConfig.pFactor = CJ125_PID_LSU49_P;
		heaterPidConfig.iFactor = CJ125_PID_LSU49_I;
	} else {
		heaterPidConfig.pFactor = CJ125_PID_LSU42_P;
		heaterPidConfig.iFactor = CJ125_PID_LSU42_I;
	}
	heaterPidConfig.dFactor = 0.0f;
	heaterPidConfig.minValue = 0;
	heaterPidConfig.maxValue = 1;
	heaterPidConfig.offset = 0;
	// todo: period?
	heaterPidConfig.period = 1.0f;
	heaterPid.reset();
}

//	engineConfiguration->spi2SckMode = PAL_STM32_OTYPE_OPENDRAIN; // 4
//	engineConfiguration->spi2MosiMode = PAL_STM32_OTYPE_OPENDRAIN; // 4
//	engineConfiguration->spi2MisoMode = PAL_STM32_PUDR_PULLUP; // 32
//	CONFIGB(cj125CsPin) = GPIOA_15;
//	engineConfiguration->cj125CsPinMode = OM_OPENDRAIN;

void cj125defaultPinout(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->cj125ua = EFI_ADC_13; // PC3
	engineConfiguration->cj125ur = EFI_ADC_4; // PA4
	CONFIGB(wboHeaterPin) = GPIOC_13;

	CONFIGB(isCJ125Enabled) = false;

	CONFIGB(spi2mosiPin) = GPIOB_15;
	CONFIGB(spi2misoPin) = GPIOB_14;
	CONFIGB(spi2sckPin) = GPIOB_13;

	CONFIGB(cj125CsPin) = GPIOB_0;
	CONFIGB(isCJ125Enabled) = true;
	CONFIGB(is_enabled_spi_2) = true;
}

static void cjStartSpi(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	cj125Cs.initPin("cj125 CS", CONFIGB(cj125CsPin),
			&engineConfiguration->cj125CsPinMode);
	// Idle CS pin - SPI CS is high when idle
	cj125Cs.setValue(true);

#if EFI_PROD_CODE
	cj125spicfg.ssport = getHwPort("cj125", CONFIGB(cj125CsPin));
	cj125spicfg.sspad = getHwPin("cj125", CONFIGB(cj125CsPin));
	driver = getSpiDevice(engineConfiguration->cj125SpiDevice);
	scheduleMsg(logger, "cj125: Starting SPI driver");
	spiStart(driver, &cj125spicfg);
#endif /* EFI_PROD_CODE */
}

/**
 * @return true if currently in IDLE or ERROR state
 */
static bool cj125periodic(CJ125 *instance, CJ125_state *anotherState DECLARE_ENGINE_PARAMETER_SUFFIX) {
	{
	efitick_t nowNt = getTimeNowNt();
		bool isStopped = engine->rpmCalculator.isStopped(PASS_ENGINE_PARAMETER_SIGNATURE);

		cjUpdateAnalogValues();
		
		// If the controller is disabled
		if (instance->state == CJ125_IDLE || instance->state == CJ125_ERROR) {
			return true;
		}

		if (instance->state == CJ125_CALIBRATION) {
			cjCalibrate();
			// Start normal operation
			instance->state = CJ125_INIT;
			cjSetMode(CJ125_MODE_NORMAL_17);
		}
		
		diag = cjReadRegister(DIAG_REG_RD);

		// check heater state
		if (vUr > CJ125_UR_PREHEAT_THR || heaterDuty < CJ125_PREHEAT_MIN_DUTY) {
			// Check if RPM>0 and it's time to start pre-heating
			if (instance->state == CJ125_INIT && !isStopped) {
				// start preheating
				instance->state = CJ125_PREHEAT;
				anotherState->startHeatingNt = anotherState->prevNt = getTimeNowNt();
				cjSetMode(CJ125_MODE_NORMAL_17);
			}
		} else if (vUr > CJ125_UR_GOOD_THR) {
			instance->state = CJ125_HEAT_UP;
		} else if (vUr < CJ125_UR_OVERHEAT_THR) {
			instance->state = CJ125_OVERHEAT;
		} else {
			// This indicates that the heater temperature is optimal for UA measurement
			instance->state = CJ125_READY;
		}

		if (isStopped && instance->cjIsWorkingState()) {
			instance->state = CJ125_INIT;
			cjSetIdleHeater(PASS_ENGINE_PARAMETER_SIGNATURE);
		}

#if 0
		// Change amplification if AFR gets lean/rich for better accuracy
		cjSetMode(lambda > 1.0f ? CJ125_MODE_NORMAL_17 : CJ125_MODE_NORMAL_8);
#endif

		switch (instance->state) {
		case CJ125_PREHEAT:
			// use constant-speed startup heat-up
			if (nowNt - anotherState->prevNt >= CJ125_HEATER_PREHEAT_PERIOD) {
				float periodSecs = (float)(nowNt - anotherState->prevNt) / US2NT(US_PER_SECOND_LL);
				// maintain speed at ~0.4V/sec
				float preheatDuty = heaterDuty + periodSecs * CJ125_HEATER_PREHEAT_RATE;
				cjSetHeater(preheatDuty PASS_ENGINE_PARAMETER_SUFFIX);
				// If we are heating too long, and there's still no result, then something is wrong...
				if (nowNt - anotherState->startHeatingNt > US2NT(US_PER_SECOND_LL) * CJ125_PREHEAT_TIMEOUT) {
					instance->cjSetError(CJ125_ERROR_HEATER_MALFUNCTION PASS_ENGINE_PARAMETER_SUFFIX);
				}
				cjPrintData();
				anotherState->prevNt = nowNt;
			}
			break;
		case CJ125_HEAT_UP:
		case CJ125_READY:
			// use PID for normal heater control
			if (nowNt - anotherState->prevNt >= CJ125_HEATER_CONTROL_PERIOD) {
				/* PID doesn't care about the target or the input, it knows only the
				 * error value as the difference of (target - input). and if we swap them we'll just get a sign inversion. If target=vUrCal, and input=vUr, then error=vUrCal-vUr, i.e. if vUr<vUrCal then the error will cause the heater to increase it's duty cycle. But it's not exactly what we want! Lesser vUr means HOTTER cell. That's why we even have this safety check for overheating: (vUr < CJ125_UR_OVERHEAT_THR)...
				 * So the simple trick is to inverse the error by swapping the target and input values.
				 */
				float duty = heaterPid.getValue(vUr, vUrCal);
				heaterPid.showPidStatus(logger, "cj");
				cjSetHeater(duty PASS_ENGINE_PARAMETER_SUFFIX);
				cjPrintData();
				anotherState->prevNt = nowNt;
			}
			break;
		case CJ125_OVERHEAT:
			if (nowNt - anotherState->prevNt >= CJ125_HEATER_OVERHEAT_PERIOD) {
				instance->cjSetError(CJ125_ERROR_OVERHEAT PASS_ENGINE_PARAMETER_SUFFIX);
				anotherState->prevNt = nowNt;
			}
		default:
			;
		}
}
	return false;
}

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)

static CJ125_state globalStateInstance;

static msg_t cjThread(void)
{
	chRegSetThreadName("cj125");

	chThdSleepMilliseconds(500);

	globalStateInstance.startHeatingNt = 0;
	globalStateInstance.prevNt = getTimeNowNt();
	while (1) {
		bool needIdleSleep = cj125periodic(&globalInstance, &globalStateInstance PASS_ENGINE_PARAMETER_SUFFIX);
		chThdSleepMilliseconds(needIdleSleep ? CJ125_IDLE_TICK_DELAY : CJ125_TICK_DELAY);
	}
	return -1;
}
#endif /* EFI_UNIT_TEST */

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
static bool cjCheckConfig(void) {
	if (!CONFIGB(isCJ125Enabled)) {
		scheduleMsg(logger, "cj125 is disabled. Failed!");
		return false;
	}
	return true;
}

static void cjStartCalibration(void) {
	if (!cjCheckConfig())
		return;
	if (globalInstance.cjIsWorkingState()) {
		// todo: change this later for the normal thread operation (auto pre-heating)
		scheduleMsg(logger, "cj125: Cannot start calibration. Please restart the board and make sure that your sensor is not heating");
		return;
	}
	globalInstance.state = CJ125_CALIBRATION;
}

static void cjStartTest(void) {
	if (!cjCheckConfig())
		return;
	globalInstance.state = CJ125_INIT;
}
#endif /* EFI_UNIT_TEST */


#ifdef CJ125_DEBUG
static void cjSetInit1(int v) {
	cjWriteRegister(INIT_REG1_WR, v & 0xff);
	v = cjReadRegister(INIT_REG1_RD);
	scheduleMsg(logger, "cj125 INIT_REG1=0x%02x.", v);
}

static void cjSetInit2(int v) {
	cjWriteRegister(INIT_REG2_WR, v & 0xff);
	v = cjReadRegister(INIT_REG2_RD);
	scheduleMsg(logger, "cj125 INIT_REG2=0x%02x.", v);
}
#endif /* CJ125_DEBUG */

float cjGetAfr(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	cj125_sensor_type_e sensorType;
	if (engineConfiguration->cj125isLsu49) {
		sensorType = CJ125_LSU_49;
	} else {
		sensorType = CJ125_LSU_42;
	}
	
	// See CJ125 datasheet, page 6
	float pumpCurrent = (vUa - vUaCal) * amplCoeff * (CJ125_PUMP_CURRENT_FACTOR / CJ125_PUMP_SHUNT_RESISTOR);
	lambda = interpolate2d("cj125Lsu", pumpCurrent, (float *)cjLSUBins[sensorType], (float *)cjLSULambda[sensorType], cjLSUTableSize[sensorType]);
	// todo: make configurable stoich ratio
	return lambda * CJ125_STOICH_RATIO;
}

bool cjHasAfrSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!CONFIGB(isCJ125Enabled))
		return false;
#if ! EFI_UNIT_TEST
	// check if controller is functioning
	if (!globalInstance.cjIsWorkingState())
		return false;
#endif /* EFI_UNIT_TEST */
	// check if amplification is turned on
	if (amplCoeff == 0.0f)
		return false;
	// check if UA calibration value is valid
	if (vUaCal < CJ125_UACAL_MIN || vUaCal > CJ125_UACAL_MAX)
		return false;
	return true;
}

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
// used by DBG_CJ125
void cjPostState(TunerStudioOutputChannels *tsOutputChannels) {
	tsOutputChannels->debugFloatField1 = heaterDuty;
	tsOutputChannels->debugFloatField2 = heaterPid.getIntegration();
	tsOutputChannels->debugFloatField3 = heaterPid.getPrevError();
	tsOutputChannels->debugFloatField4 = vUa;
	tsOutputChannels->debugFloatField5 = vUr;
	tsOutputChannels->debugFloatField6 = vUaCal;
	tsOutputChannels->debugFloatField7 = vUrCal;
	tsOutputChannels->debugIntField1 = globalInstance.state;
	tsOutputChannels->debugIntField2 = diag;
}
#endif /* EFI_TUNER_STUDIO */

void initCJ125(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	logger = sharedLogger;

	if (!CONFIGB(isCJ125Enabled))
		return;

	if (CONFIG(cj125ur) == EFI_ADC_NONE || CONFIG(cj125ua) == EFI_ADC_NONE) {
		scheduleMsg(logger, "cj125 init error! cj125ur and cj125ua are required.");
		return;
	}

	if (CONFIGB(wboHeaterPin) == GPIO_UNASSIGNED) {
		scheduleMsg(logger, "cj125 init error! wboHeaterPin is required.");
		return;
	}

	cjInitPid(PASS_ENGINE_PARAMETER_SIGNATURE);
	cjStartSpi(PASS_ENGINE_PARAMETER_SIGNATURE);
	cjStartHeaterControl(PASS_ENGINE_PARAMETER_SIGNATURE);
	cjStart(PASS_ENGINE_PARAMETER_SIGNATURE);

#if 1
	globalInstance.state = CJ125_INIT;
#endif
	
#ifdef CJ125_DEBUG
	addConsoleActionF("cj125_heater", cjSetHeater);
	addConsoleActionI("cj125_set_init1", cjSetInit1);
	addConsoleActionI("cj125_set_init2", cjSetInit2);
#endif /* CJ125_DEBUG */

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
	addConsoleAction("cj125", cjStartTest);
	addConsoleAction("cj125_calibrate", cjStartCalibration);

	chThdCreateStatic(cjThreadStack, sizeof(cjThreadStack), LOWPRIO, (tfunc_t)(void*) cjThread, NULL);
#endif /* EFI_UNIT_TEST */
}

#endif /* EFI_CJ125 */
