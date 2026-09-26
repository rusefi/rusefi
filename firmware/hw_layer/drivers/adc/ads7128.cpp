/**
 * I2C ADC/GPIO extender chip
 *
 */

#include "pch.h"

#include "ads7128.h"

#if EFI_PROD_CODE && (BOARD_ADS7128_COUNT > 0)

/*
 * TODO list:
 *
 * - setup and use oversampling. Current refresh rate is low enougth for any oversampling ratio
 */

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME				"ads7128"

/* ADS71x8 operation codes */
#define OPCODE_SINGLE_WRITE	0x08
#define OPCODE_SINGLE_READ	0x10
#define OPCODE_SET_BIT		0x18
#define OPCODE_CLEAR_BIT	0x20
#define OPCODE_BLOCK_WRITE	0x28
#define OPCODE_BLOCK_READ	0x30

/* ADS71x8 registers */
#define REG_SYSTEM_STATUS	0x00
#define REG_GENERAL_CFG		0x01
#define REG_OSR_CFG			0x03
#define REG_OPMODE_CFG		0x04
#define REG_PIN_CFG			0x05
#define REG_GPIO_CFG		0x07
#define REG_GPO_DRIVE_CFG	0x09
#define REG_GPO_VALUE		0x0B
#define REG_GPI_VALUE		0x0D
#define REG_SEQUENCE_CFG	0x10
#define REG_AUTO_SEQ_CH_SEL	0x12
#define REG_ALERT_CH_SEL	0x14
#define REG_EVENT_FLAG		0x18
#define REG_EVENT_HIGH_FLAG	0x1A
#define REG_EVENT_LOW_FLAG	0x1C
#define REG_HIGH_TH_HYS_CH(x)	((x) * 4 + 0x20)
#define REG_LOW_TH_CNT_CH(x)	((x) * 4 + 0x22)
#define REG_MAX_LSB_CH(x)		((x) * 2 + 0x60)
#define REG_MIN_LSB_CH(x)		((x) * 2 + 0x80)
#define REG_RECENT_LSB_CH(x)	((x) * 2 + 0xA0)

#define REG_GENERAL_CFG_RST			BIT(0)
#define REG_GENERAL_CFG_DWC_EN		BIT(4)
#define REG_GENERAL_CFG_STATS_EN	BIT(5)
#define REG_OPMODE_CFG_CONV_MODE	BIT(5)
#define REG_SEQUENCE_CFG_SEQ_MODE	BIT(0)
#define REG_SEQUENCE_CFG_SEQ_START	BIT(4)

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/
/* OS */
static thread_t *ads7128_thread = NULL;
SEMAPHORE_DECL(ads7128_wake, 10 /* or BOARD_ADS7128_COUNT ? */);
static THD_WORKING_AREA(ads7128_thread_wa, 256);

Ads7128 ads7128_chips[BOARD_ADS7128_COUNT];

static const char* ads7128_pin_names[ADS7128_IOS] = {
	"AIN0",	"AIN1",	"AIN2",	"AIN3",
	"AIN4",	"AIN5",	"AIN6",	"AIN7"
};

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

int Ads7128::regs_read(uint8_t reg_addr, uint8_t *regs, size_t n)
{
	const uint8_t cmd[2] = { static_cast<uint8_t>((n == 1) ? OPCODE_SINGLE_READ : OPCODE_BLOCK_READ), reg_addr };
	if (i2c->write(cfg->i2c_addr, cmd, sizeof(cmd)) != MSG_OK) {
		return -1;
	}

	return (i2c->read(cfg->i2c_addr, regs, n) == MSG_OK) ? n : -2;
}

int Ads7128::regs_write(uint8_t reg_addr, const uint8_t *regs, size_t n)
{
	uint8_t cmd[2 + n] = { static_cast<uint8_t>((n == 1) ? OPCODE_SINGLE_WRITE : OPCODE_BLOCK_WRITE), reg_addr };

	memcpy(cmd + 2, regs, n);

	return (i2c->write(cfg->i2c_addr, cmd, n + 2) == MSG_OK) ? n : -1;
}

/**
 * @brief ADS7128 chip reset.
 * @details Reset the chip to get a defined starting configuration.
 */

int Ads7128::chip_reset()
{
	uint8_t cmd[1] = { REG_GENERAL_CFG_RST };

	if (regs_write(REG_GENERAL_CFG, cmd, sizeof(cmd)) != 1) {
		return -1;
	}

	return 0;
}

/**
 * @brief ADS7128 chip init.
 * @details Checks communication. Check chip presence.
 */

int Ads7128::chip_init()
{
	/* count attempts */
	init_cnt++;

	/* ping */
	if (regs_read(REG_SYSTEM_STATUS, &status, 1) != 1) {
		return -7;
	}

	/* configure PIN_CFG, GPIO_CFG, GPO_DRIVE_CFG, GPO_VALUE */
	if (update_gpios() != 0) {
		return -8;
	}

	/* Convertion mode - auto */
	uint8_t cmode = REG_OPMODE_CFG_CONV_MODE;
	if (regs_write(REG_OPMODE_CFG, &cmode, 1) != 1) {
		return -9;
	}

	/* Enable statistics module to update minimum, maximum, and latest output code registers. */
	uint8_t general = REG_GENERAL_CFG_STATS_EN;
	if (regs_write(REG_GENERAL_CFG, &general, 1) != 1) {
		return -10;
	}

	/* Enable all channels for auto sequencing */
	uint8_t channels = 0xff;
	if (regs_write(REG_AUTO_SEQ_CH_SEL, &channels, 1) != 1) {
		return -11;
	}

	/* Set auto sequence mode and start sequencing */
	uint8_t smode = REG_SEQUENCE_CFG_SEQ_START | REG_SEQUENCE_CFG_SEQ_MODE;
	if (regs_write(REG_SEQUENCE_CFG, &smode, 1) != 1) {
		return -12;
	}

	return 0;
}

/**
 * @brief ADS7128 read recent values.
 */

int Ads7128::read_inputs()
{
	uint8_t raw[2 * ADS7128_IOS];

	if (regs_read(REG_RECENT_LSB_CH(0), raw, 2 * ADS7128_IOS) != 2 * ADS7128_IOS) {
		return -1;
	}

	for (size_t i = 0; i < ADS7128_IOS; i++) {
		adcRaw[i] = raw[2 * i] | (raw[2 * i + 1] << 8);
	}

	return 0;
}

/**
 * @brief ADS7128 write gpios states.
 */

int Ads7128::update_gpios()
{
	/* configure PIN_CFG, GPIO_CFG, GPO_DRIVE_CFG, GPO_VALUE */
	if ((regs_write(REG_PIN_CFG, &gpio_mask, 1) != 1) ||
		(regs_write(REG_GPIO_CFG, &gpio_out_mask, 1) != 1) ||
		(regs_write(REG_GPO_DRIVE_CFG, &gpio_pp_mask, 1) != 1) ||
		(regs_write(REG_GPO_VALUE, &gpio_out_state, 1) != 1)) {
		return -1;
	}

	/* TODO: if any inputs? */
	if (regs_read(REG_GPI_VALUE, &gpio_in_state, 1) != 1) {
		return -2;
	}

	return 0;
}

/**
 * @brief ADS7128 chip driver wakeup.
 * @details Wake up driver. Will cause output register and
 * diagnostic update.
 */

void Ads7128::wake_driver()
{
	/* Entering a reentrant critical zone.*/
	chibios_rt::CriticalSectionLocker csl;
	chSemSignalI(&ads7128_wake);
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

static THD_FUNCTION(ads7128_driver_thread, p) {
	(void)p;

	chRegSetThreadName(DRIVER_NAME);

	while (true) {
		msg_t msg = chSemWaitTimeout(&ads7128_wake, TIME_MS2I(ADS7128_POLL_INTERVAL_MS));

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

		for (int i = 0; i < BOARD_ADS7128_COUNT; i++) {
			auto chip = &ads7128_chips[i];

			if (chip->need_init) {
				int ret = chip->chip_init();
				if (ret == 0) {
					chip->drv_state = ADS7128_READY;
					chip->need_init = false;
				} else {
					chip->errors++;
				}
			}

			if ((chip->cfg == NULL) ||
				(chip->drv_state == ADS7128_DISABLED) ||
				(chip->drv_state == ADS7128_FAILED)) {
				chip->need_init = true;
				continue;
			}

			int ret = chip->read_inputs();
			if (ret) {
				chip->errors++;
				/* set state to ADS7128_FAILED? */
			}

			ret = chip->update_gpios();
			if (ret) {
				chip->errors++;
				/* set state to ADS7128_FAILED? */
			}
		}
	}
}

RUSEFI_STACK_ROOT_EXPLICIT(ads7128_driver_thread, sizeof(ads7128_thread_wa));

/*==========================================================================*/
/* Driver exported functions.												*/
/*==========================================================================*/

int Ads7128::setPadMode(unsigned int pin, iomode_t mode) {
	if (pin >= ADS7128_IOS)
		return -1;

	uint8_t mask = BIT(pin);

	chibios_rt::CriticalSectionLocker csl;

	switch (mode) {
	case PAL_MODE_INPUT:
	case PAL_MODE_INPUT_PULLUP:
	case PAL_MODE_INPUT_PULLDOWN:
		gpio_mask |= mask;
		gpio_out_mask &= ~mask;
		gpio_pp_mask &= ~mask;
		break;
	case PAL_MODE_INPUT_ANALOG:
		gpio_mask &= ~mask;
		break;
	case PAL_MODE_OUTPUT_PUSHPULL:
		gpio_mask |= mask;
		gpio_out_mask |= mask;
		gpio_pp_mask |= mask;
		break;
	case PAL_MODE_OUTPUT_OPENDRAIN:
		gpio_mask |= mask;
		gpio_out_mask |= mask;
		gpio_pp_mask &= ~mask;
		break;
	default:
		/* hm? analog input mode - default */
		gpio_mask &= ~mask;
		break;

	}

	wake_driver();

	return 0;
}


int Ads7128::writePad(size_t pin, int value) {
	if (pin >= ADS7128_IOS)
		return -1;

	uint8_t mask = BIT(pin);

	chibios_rt::CriticalSectionLocker csl;

	if (value) {
		gpio_out_state |=  mask;
	} else {
		gpio_out_state &= ~mask;
	}

	wake_driver();

	return 0;
}

int Ads7128::readPad(size_t pin) {
	if (pin >= ADS7128_IOS)
		return -1;

	/* convert to some common enum? */
	return !!(gpio_in_state & BIT(pin));
}

float Ads7128::readAnalog(size_t pin) {
	if (pin >= ADS7128_IOS)
		return -1;

	/* TODO: oversampling? */
	return (adcRaw[pin] * cfg->vref / ADS7128_MAX_ADC_VALUE);
}

void Ads7128::debug() {
	efiPrintf("errors %d", errors);
	for (size_t i = 0; i < ADS7128_IOS; i++) {
		// TODO: show IO mode
		efiPrintf("%s %f(raw %d)", ads7128_pin_names[i], readAnalog(i), adcRaw[i]);
	}
}

int Ads7128::init() {
	/* check for multiple init */
	if (drv_state != ADS7128_WAIT_INIT)
		return -1;

	/* Get pointer to actual bus instance, should be ready now */
	i2c = getI2cBus(cfg->i2c_bus);
	if (i2c == nullptr)
		return -2;

	/* force init from driver thread */
	need_init = true;

	/* instance is ready */
	drv_state = ADS7128_READY;

	if (!ads7128_thread) {
		ads7128_thread = chThdCreateStatic(ads7128_thread_wa, sizeof(ads7128_thread_wa),
										   PRIO_GPIOCHIP, ads7128_driver_thread, nullptr);
	}

	return 0;
}

/**
 * @brief ADS7128 driver add.
 * @details Checks for valid config
 */

int ads7128_add(brain_pin_e base, unsigned int index, const ads7128_config *cfg) {

	/* no config or no such chip */
	if ((!cfg) || (cfg->i2c_bus == I2C_NONE) || (cfg->i2c_addr == 0x00) || (index >= BOARD_ADS7128_COUNT))
		return -16;

	Ads7128& chip = ads7128_chips[index];

	/* already initted? */
	if (chip.cfg != NULL)
		return -13;

	chip.cfg = cfg;
	chip.gpio_mask = 0;
	chip.gpio_out_mask = 0;
	chip.gpio_pp_mask = 0;

	/* register, return gpio chip base */
	int ret = gpiochip_register(base, DRIVER_NAME, chip, ADS7128_IOS);
	if (ret < 0)
		return ret;

	/* set default pin names, board init code can rewrite */
	gpiochips_setPinNames(static_cast<brain_pin_e>(ret), ads7128_pin_names);

	chip.drv_state = ADS7128_WAIT_INIT;

	return ret;
}

#else /* BOARD_ADS7128_COUNT > 0 */

int ads7128_add(brain_pin_e base, unsigned int index, const ads7128_config *cfg)
{
	(void)base; (void)index; (void)cfg;

	return -5;
}

#endif /* BOARD_ADS7128_COUNT */
