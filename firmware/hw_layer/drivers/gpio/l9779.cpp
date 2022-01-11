/*
 * l9779.cpp
 *
 *  Created on: Jan 10, 2022
 */

#include "pch.h"

#include "gpio/l9779.h"

#if (BOARD_L9779_COUNT > 0)

#include "persistent_configuration.h"
#include "hardware.h"
#include "gpio/gpio_ext.h"
#include "os_util.h"
/*
 * TODO list:
 *  - just write code
 */
/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/
#define DRIVER_NAME					"l9779"

/* SPI communication helpers */
/* Out frame */
/* D0 - parity */
/* D8:D1 - DATA OUT or SUBADDRESS if ADD[4:0] = 0x10 (for read) */
#define MSG_SET_DATA(d)				(((d) & 0xff) <<  1)
/* sub-address is 5 bit */
#define MSG_SET_SUBADDR(s)			(((s) & 0x1f) <<  1)
/* D9 - x */
/* D14:D10 - ADDRESS */
#define MSG_SET_ADDR(a)				(((a) & 0x1f) << 10)
/* D15 - x */

/* In frame */
/* D0 - parity */
#define MSG_GET_PARITY(rx)			(((rx) >>  0) & 0x01)
/* D8:D1 - DATA IN */
#define MSG_GET_DATA(rx)			(((rx) >>  1) & 0xff)
/* D 9 - W/R flag, 1 if we read */
#define MSG_GET_WR(rx)				(((rx) >>  9) & 0x01)
/* D14:D10 - Addr of DATA IN */
#define MSG_GET_ADDR(rx)			(((rx) >> 10) & 0x1f)
/* D15 - SPI error flag */
#define MSG_GET_SPIERROR(rx)		(((rx) >> 15) & 0x01)

/* register address that never can be replyed */
#define REG_INVALID					0xff

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* Driver private data */
struct L9779 : public GpioChip {
	int init() override;
	int deinit() override;

	int setPadMode(size_t pin, iomode_t mode) override;
	int writePad(size_t pin, int value) override;
	int readPad(size_t pin) override;
	brain_pin_diag_e getDiag(size_t pin) override;

	bool spi_parity_odd(uint16_t x);
	int spi_validate(uint16_t rx);
	int spi_rw(uint16_t tx, uint16_t *rx_ptr);

	const l9779_config	*cfg;

	/* last accessed register, for validation on next SPI access */
	uint8_t						last_reg;

	/* statistic */
	//int						por_cnt;
	//int						wdr_cnt;
	//int						comfe_cnt;
	//int						init_cnt;
	//int						init_req_cnt;
	int							spi_cnt;
	int							spi_err_parity;		/* parity errors in rx data */
	int							spi_err_frame;		/* rx messages with bit 15 set */
	int							spi_err;			/* rx messages with incorrect ADDR or WR fields */
	uint16_t					tx;
	uint16_t					rx;
};

static L9779 chips[BOARD_L9779_COUNT];

static const char* l9779_pin_names[L9779_SIGNALS] = {
	"L9779.IGN1",	"L9779.IGN2",	"L9779.IGN3",	"L9779.IGN4",
	"L9779.OUT1",	"L9779.OUT2",	"L9779.OUT3",	"L9779.OUT4",
	"L9779.OUT5",	"L9779.OUT6",	"L9779.OUT7",	"L9779.OUT8",
	"L9779.OUT9",	"L9779.OUT10",	"L9779.OUT11",	"L9779.OUT12",
	"L9779.OUT13",	"L9779.OUT14",	"L9779.OUT15",	"L9779.OUT16",
	"L9779.OUT17",	"L9779.OUT18",	"L9779.OUT19",	"L9779.OUT20",
	"L9779.OUTA",	"L9779.OUTB",	"L9779.OUTC",	"L9779.OUTD",
	"L9779.OUT21",	"L9779.OUT22",	"L9779.OUT23",	"L9779.OUT24",
	"L9779.MRD",	"L9779.KEY"
};

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

/* true if parity of input x is odd */
bool L9779::spi_parity_odd(uint16_t x)
{
	x ^= x >> 8;
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;

	return (x & 1);
}

int L9779::spi_validate(uint16_t rx)
{
	if (!spi_parity_odd(rx)) {
		spi_err_parity++;
		return -1;
	}

	if (MSG_GET_SPIERROR(rx)) {
		/* not clear what does this means */
		spi_err_frame++;
		return -1;
	}

	uint8_t reg = MSG_GET_DATA(rx);

	/* TODO: also check WR bit */
	if ((last_reg != REG_INVALID) && (last_reg != reg)) {
		/* unexpected SPI answer */
		spi_err++;

		/* should ve restart? */
		//need_init = true;

		return -1;
	}

	return 0;
}

/**
 * @returns -1 in case of communication error
 */
int L9779::spi_rw(uint16_t tx, uint16_t *rx_ptr)
{
	int ret;
	uint16_t rx;
	SPIDriver *spi = cfg->spi_bus;

	/* set parity */
	tx |= !spi_parity_odd(tx);

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	rx = spiPolledExchange(spi, tx);
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);

	/* statisctic and debug */
	this->tx = tx;
	this->rx = rx;
	this->spi_cnt++;

	if (rx_ptr)
		*rx_ptr = rx;

	/* validate reply and save last accessed register */
	ret = spi_validate(rx);
	last_reg = MSG_GET_ADDR(tx);

	/* no errors for now */
	return ret;
}

/*==========================================================================*/
/* Driver interrupt handlers.												*/
/*==========================================================================*/
/*==========================================================================*/
/* Driver exported functions.												*/
/*==========================================================================*/

int L9779::setPadMode(unsigned int pin, iomode_t mode) {
	if (pin >= L9779_SIGNALS)
		return -1;

	(void)mode;

	return 0;
}

int L9779::writePad(unsigned int pin, int value) {
	if (pin >= L9779_OUTPUTS)
		return -1;

	(void)value;

	return 0;
}

int L9779::readPad(size_t pin) {
	if (pin >= L9779_SIGNALS)
		return -1;

	/* unknown pin */
	return -1;
}

int L9779::init()
{
	return 0;
}

int L9779::deinit()
{
	return 0;
}

/**
 * @brief L9779 driver add.
 * @details Checks for valid config
 * @return return gpio chip base
 */

int l9779_add(brain_pin_e base, unsigned int index, const l9779_config *cfg) {

	efiAssert(OBD_PCM_Processor_Fault, cfg != NULL, "L9779CFG", 0)

	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_L9779_COUNT))
		return -1;

	L9779* chip = &chips[index];

	/* already initted? */
	if (chip->cfg)
		return -1;

	/* config */
	chip->cfg = cfg;
	/* reset to defaults */


	/* register */
	int ret = gpiochip_register(base, DRIVER_NAME, *chip, L9779_SIGNALS);
	if (ret < 0)
		return ret;

	/* set default pin names, board init code can rewrite */
	gpiochips_setPinNames(base, l9779_pin_names);

	return ret;
}

#endif /* (BOARD_L9779_COUNT > 0) */
