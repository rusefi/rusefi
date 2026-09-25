/**
 * @file pca9685.cpp
 *
 * PCA9685 16-channel 12-bit PWM I2C-bus LED controller
 *
 * @date Jul 02, 2026
 * @author Andrey Gusakov
 */

#include "pch.h"

#include "gpio/pca9685.h"

#include "gpio/gpio_ext.h"

#if EFI_PROD_CODE && (BOARD_PCA9685_COUNT > 0)

/*
 * TODO list:
 */

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME "pca9685"

/* this may be too slow? */
#define PCA9685_POLL_INTERVAL_MS	100

// PCA9685 Registers
#define PCA9685_MODE1      0x00
#define PCA9685_MODE2      0x01
#define PCA9685_SUBADR1    0x02
#define PCA9685_SUBADR2    0x03
#define PCA9685_SUBADR3    0x04
#define PCA9685_ALLCALLADR 0x05
#define PCA9685_LED0_ON_L  0x06
#define PCA9685_LED0_ON_H  0x07
#define PCA9685_LED0_OFF_L 0x08
#define PCA9685_LED0_OFF_H 0x09
#define PCA9685_ALL_LED_ON_L  0xFA
#define PCA9685_ALL_LED_ON_H  0xFB
#define PCA9685_ALL_LED_OFF_L 0xFC
#define PCA9685_ALL_LED_OFF_H 0xFD
#define PCA9685_PRE_SCALE     0xFE

#define PCA9685_GPIO_MASK_ALL	0xFFFF

#define PCA9685_DUTY_MAX		4096	// bit 12
#define PCA9685_DUTY_OFF		0		// constant off
#define PCA9685_DUTY_ON			PCA9685_DUTY_MAX	// constant on

#define MODE1_RESTART 0x80
#define MODE1_SLEEP   0x10
#define MODE1_AI      0x20

#define MODE2_OUTNE_LOW		0x00
#define MODE2_OUTNE_HIGH	0x01
#define MODE2_OUTNE_HIZ		0x02

#define MODE2_OUTDRV_OD		0x00
#define MODE2_OUTDRV_PP		0x04

#define MODE2_INVRT			0x10

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

struct Pca9685 : public GpioChip {
	int init() override;
	int writePad(size_t pin, int value) override;
	int setPadPWM(size_t pin, float frequency, float duty) override;
	void debug() override;

	int chip_init();

	i2cBus		*i2c;

	const pca9685_config* cfg;

	void wake_driver();
	int updateOutputs();

	int errors = 0;

private:
	int writeReg(uint8_t reg, uint8_t value);
	int writeRegs(uint8_t reg, const uint8_t *regs, size_t n);
	int readReg(uint8_t reg, uint8_t *value);

	int updateOutput(size_t pin, uint16_t duty);

	uint16_t 	gpio_need_update = 0;
	uint16_t	duties[PCA9685_OUTPUTS];
};


/* OS */
static thread_t *pca9685_thread = NULL;
SEMAPHORE_DECL(pca9685_wake, 10 /* or BOARD_PCA9685_COUNT ? */);
static THD_WORKING_AREA(pca9685_thread_wa, 256);

static const char* pca9685_pin_names[PCA9685_OUTPUTS] = {
	"LED0",  "LED1",  "LED2",  "LED3",  "LED4",  "LED5",  "LED6",  "LED7",
	"LED8",  "LED9",  "LED10", "LED11", "LED12", "LED13", "LED14", "LED15"
};

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

#if BOARD_PCA9685_COUNT > 0
static Pca9685 chips[BOARD_PCA9685_COUNT];

int Pca9685::chip_init() {
	/* count attempts */
	init_cnt++;

	// 0. Ping chip
	uint8_t reg = 0;
	if (readReg(PCA9685_MODE1, &reg) != 0) {
		return -1;
	}

	// 1. Reset chip: set MODE1 to SLEEP=0
	if (writeReg(PCA9685_MODE1, MODE1_AI) != 0) {
		return -1;
	}

	uint8_t mode2 = MODE2_OUTNE_HIZ;
	if (writeReg(PCA9685_MODE2, mode2) != 0) {
		return -1;
	}

	gpio_need_update = PCA9685_GPIO_MASK_ALL;

	return 0;
}

int Pca9685::updateOutput(size_t pin, uint16_t duty) {
	uint16_t on, off;
	if (duty == PCA9685_DUTY_OFF) {
		// full OFF
		on = 0;
		off = PCA9685_DUTY_MAX;
	} else if (duty == PCA9685_DUTY_ON) {
		// full ON
		on = PCA9685_DUTY_MAX;
		off = 0;
	} else {
		// pase shift
		on = pin * PCA9685_DUTY_MAX / PCA9685_OUTPUTS;
		off = (on + duty) % PCA9685_DUTY_MAX;
	}

	uint8_t reg = PCA9685_LED0_ON_L + (4 * pin);
	uint8_t tx[] = {
		(uint8_t)(on & 0xFF),
		(uint8_t)((on >> 8) & 0x1F),	// [4] = LED full on
		(uint8_t)(off & 0xFF),
		(uint8_t)((off >> 8) & 0x1F)	// [4] = LED full off
	};

	return writeRegs(reg, tx, sizeof(tx));
}

int Pca9685::updateOutputs() {
	for (size_t pin = 0; pin < PCA9685_OUTPUTS; pin++) {
		if (gpio_need_update & BIT(pin)) {
			uint16_t duty = duties[pin];

			if (updateOutput(pin, duty) == 0) {
				chibios_rt::CriticalSectionLocker csl;
				if (duty == duties[pin]) {
					gpio_need_update &= ~BIT(pin);
				}
			} else {
				return -1;
			}
		}
	}

	// values at chip are up to date...
	if (gpio_need_update == 0) {
		return 0;
	}

	// some of values where updated while we were writing over i2c... need another refresh
	return 1;
}

int Pca9685::writeReg(uint8_t reg, uint8_t value) {
	uint8_t tx[] = { reg, value };
	msg_t msg = i2c->write(cfg->i2c_addr, tx, sizeof(tx));
	return (msg == MSG_OK) ? 0 : -1;
}

int Pca9685::writeRegs(uint8_t reg, const uint8_t *vals, size_t n)
{
	uint8_t cmd[1 + n] = { reg };
	memcpy(cmd + 1, vals, n);
	msg_t msg = i2c->write(cfg->i2c_addr, cmd, n + 1);
	return (msg == MSG_OK) ? 0 : -1;
}

int Pca9685::readReg(uint8_t reg, uint8_t *value) {
	msg_t msg = i2c->writeRead(cfg->i2c_addr, &reg, 1, value, 1);
	return (msg == MSG_OK) ? 0 : -1;
}
/**
 * @brief PCA9685 chip driver wakeup.
 * @details Wake up driver. Will cause output register and
 * diagnostic update.
 */

void Pca9685::wake_driver()
{
	/* Entering a reentrant critical zone.*/
	chibios_rt::CriticalSectionLocker csl;
	chSemSignalI(&pca9685_wake);
	if (!port_is_isr_context()) {
		/**
		 * chSemSignalI above requires rescheduling
		 * interrupt handlers have implicit rescheduling
		 */
		chSchRescheduleS();
	}
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(pca9685_driver_thread, p) {
	(void)p;

	chRegSetThreadName(DRIVER_NAME);

	while (true) {
		msg_t msg = chSemWaitTimeout(&pca9685_wake, TIME_MS2I(PCA9685_POLL_INTERVAL_MS));

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

		for (int i = 0; i < BOARD_PCA9685_COUNT; i++) {
			int ret = 0;
			auto chip = &chips[i];

			if (chip->cfg == NULL) {
				chip->need_init = true;
				continue;
			}

			if (chip->need_init) {
				ret = chip->chip_init();
				if (ret == 0) {
					chip->need_init = false;
				} else {
					chip->errors++;
				}
			}

			// Skip output update if chip has failed to init
			if (ret == 0) {
				// TODO: handle ret = 1, repeat update
				ret = chip->updateOutputs();
				if (ret < 0) {
					chip->need_init = true;
					chip->errors++;
				} else {
					chip->alive_cnt++;
				}
			}
		}
	}
}

RUSEFI_STACK_ROOT_EXPLICIT(pca9685_driver_thread, sizeof(pca9685_thread_wa));

/*==========================================================================*/
/* Driver exported functions.												*/
/*==========================================================================*/
int Pca9685::writePad(size_t pin, int value) {
	if (pin >= PCA9685_OUTPUTS) {
		return -1;
	}

	uint16_t dutyInt = value ? PCA9685_DUTY_ON : PCA9685_DUTY_OFF;

	chibios_rt::CriticalSectionLocker csl;
	if (duties[pin] != dutyInt) {
		duties[pin] = dutyInt;
		gpio_need_update |= BIT(pin);
	}

	return 0;
}

int Pca9685::setPadPWM(size_t pin, float /* frequency */, float duty) {
	if (pin >= PCA9685_OUTPUTS) {
		return -1;
	}

	uint16_t dutyInt = clampI(PCA9685_DUTY_OFF, duty * PCA9685_DUTY_MAX, PCA9685_DUTY_ON);

	chibios_rt::CriticalSectionLocker csl;
	if (duties[pin] != dutyInt) {
		duties[pin] = dutyInt;
		gpio_need_update |= BIT(pin);
	}

	return 0;
}

void Pca9685::debug() {
	efiPrintf("@0x%02x", cfg->i2c_addr);
	efiPrintf("errors %d", errors);
	if (!cfg) {
		efiPrintf("no config");
		return;
	}
	for (size_t pin = 0; pin < PCA9685_OUTPUTS; pin++) {
		if ((duties[pin] != PCA9685_DUTY_OFF) && (duties[pin] != PCA9685_DUTY_ON)) {
			efiPrintf("OUT%d: PWM duty %f", pin, duties[pin] / 4095.0);
		} else {
			efiPrintf("OUT%d: OUT %d", pin, !!(duties[pin] != PCA9685_DUTY_OFF));
		}
	}
}

int Pca9685::init() {
	/* Get pointer to actual bus instance, should be ready now */
	i2c = getI2cBus(cfg->i2c_bus);
	if (i2c == nullptr) {
		return -2;
	}

	/* force init from driver thread */
	need_init = true;

	// Default frequency? PCA9685 default is ~200Hz

	if (!pca9685_thread) {
		pca9685_thread = chThdCreateStatic(pca9685_thread_wa, sizeof(pca9685_thread_wa),
										   PRIO_GPIOCHIP, pca9685_driver_thread, nullptr);
	}

	return 0;
}

/**
 * @brief PCA9685 driver add.
 * @details Checks for valid config
 */

int pca9685_add(brain_pin_e base, unsigned int index, const struct pca9685_config *cfg) {
	if  ((!cfg) || (cfg->i2c_bus == I2C_NONE) || (cfg->i2c_addr == 0x00) || (index >= BOARD_PCA9685_COUNT)) {
		return -1;
	}

	Pca9685 &chip = chips[index];

	/* already initted? */
	if (chip.cfg != NULL)
		return -13;

	chip.cfg = cfg;

	int ret = gpiochip_register(base, DRIVER_NAME, chip, PCA9685_OUTPUTS);
	if (ret < 0) {
		return ret;
	}

	/* set default pin names, board init code can rewrite */
	gpiochips_setPinNames(static_cast<brain_pin_e>(ret), pca9685_pin_names);

	return ret;
}

#else

int pca9685_add(brain_pin_e base, unsigned int index, const struct pca9685_config *cfg) {
	(void)base; (void)index; (void)cfg;

	return -1;
}

#endif // PCA9685_COUNT

#endif /* HAL_USE_I2C */
