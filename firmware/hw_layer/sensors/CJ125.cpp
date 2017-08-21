/*
 * @file CJ125.cpp
 *
 * Based on Jeff "Turbo SOB" work
 *
 * @date: Jun 24, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 *
 */

#include "engine.h"
#include "CJ125.h"
#include "pwm_generator.h"
#include "pid.h"

#if EFI_CJ125 || defined(__DOXYGEN__)

#include "pin_repository.h"
#include "hardware.h"

EXTERN_ENGINE;

static SimplePwm wboHeaderControl;
static OutputPin wboHeaderPin;
static OutputPin cj125Cs;
static Logging *logger;
static unsigned char tx_buff[1];
static unsigned char rx_buff[1];


static THD_WORKING_AREA(cjThreadStack, UTILITY_THREAD_STACK_SIZE);

// todo: make this configurable
spi_device_e cj125SpiDevice = SPI_DEVICE_2;

static SPIDriver *driver;


static SPIConfig cj125spicfg = { NULL,
/* HW dependent part.*/
NULL, 0,
SPI_CR1_MSTR |
		SPI_CR1_CPHA | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 };

/**********************************************************************************
    INIT_REG1 - CJ125 Initialization Register 1
        00x000x0
        ||||||||---- VL - Pump Current Sense Amplifier - 0 = x8, 1 = x17
                            If VL is set to "0" the amplification of Ip is 8.
                            If VL is "1" the amplification is 17. The higher
                            amplification is needed for a better resolution
                            at Lambda > 1, amplification 8 for Lambda < 1.
                            Note: It seems to make no difference, always set to 1.
        |||||||----- Unused
        ||||||------ LA - Pump Current Sense Amplifier - 0 = measurement, 1 = calibration
                            If LA is "0" the measurement mode for Ip is active.
                            The current pump current is displayed on Ua. If LA is
                            set to "1" the calibration mode is active. The shown
                            voltage on Ua must been subtracted from the later measurement.
        |||||------- Undocumented Bit - Note: CJ125 only seems to work when set to 1.
        ||||-------- RA - Measurement Current for Ri - 0 = measurement, 1 = calibration
                            If RA is "0" the measurement mode for Ri is active and
                            the current resistance of the probe is "displayed" on Ur.
                            If RA is "1" the calibration mode is active. You get the
                            optimal voltage for the inner resistance of the probe on Ur.
        |||--------- Unused
        ||---------- PA - Pump Current Control - Set to 0 to be active
        |----------- ENABLE/HOLD - Must be set to 1 to enable
***********************************************************************************/

static cj125_state_e state = CJ125_IDLE;

static msg_t cjThread(void)
{
	chRegSetThreadName("cj125");

//	while(1) {

//	}
	return -1;
}

static void cj125test(void) {

	// read identity command
	spiSelect(driver);
	tx_buff[0] = IDENT_REG_RD;
	spiSend(driver, 1, tx_buff);

	spiReceive(driver, 1, rx_buff);
	int value = rx_buff[0] & 0xF8;

	spiUnselect(driver);

	scheduleMsg(logger, "cj125 got %x", value);



}

void initCJ125(Logging *sharedLogger) {
	logger = sharedLogger;
	// still a lot to be done here :)

	if (!boardConfiguration->isCJ125Enabled)
		return;

	cj125spicfg.ssport = getHwPort("cj125", boardConfiguration->cj125CsPin);
	cj125spicfg.sspad = getHwPin("cj125", boardConfiguration->cj125CsPin);

	driver = getSpiDevice(engineConfiguration->cj125SpiDevice);

	cj125Cs.initPin("cj125 CS", boardConfiguration->cj125CsPin,
			&engineConfiguration->cj125CsPinMode);

	if (boardConfiguration->wboHeaterPin != GPIO_UNASSIGNED) {
		// todo: use custom pin state method, turn pin off while not running
		startSimplePwmExt(&wboHeaderControl, "heater control", boardConfiguration->wboHeaterPin,
				&wboHeaderPin,
				300, 0.1, applyPinState);


	}

	scheduleMsg(logger, "Starting cj125 spi driver");
	spiStart(driver, &cj125spicfg);

	addConsoleAction("cj125", cj125test);

	chThdCreateStatic(cjThreadStack, sizeof(cjThreadStack), LOWPRIO, (tfunc_t) cjThread, NULL);

}

#endif /* EFI_CJ125 */
