/*
 * tle9201.c
 *
 * TLE9201 H-Bridge with SPI
 *
 * @date Nov 1, 2024
 *
 * @author andreika, (c) 2024
 * @author Andrey Belomutskiy, (c) 2012-2024
 */

#include "pch.h"

#include "gpio/gpio_ext.h"
#include "gpio/tle9201.h"

#if EFI_PROD_CODE && (BOARD_TLE9201_COUNT > 0)

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME				"tle9201"

#define TLE9201_REG_RD_DIA			0x00	// read diagnosis register
#define TLE9201_REG_RES_DIA			0x80	// reset diagnosis register
#define TLE9201_REG_RD_REV			0x20	// read device revision number
#define TLE9201_REG_RD_CTRL			0x60	// read control register
#define TLE9201_REG_WR_CTRL			0xE0	// write control register
#define TLE9201_REG_WR_CTRL_RD_DIA	0xC0	// write control and read diagnosis

#define TLE9201_REV_MAJOR_MASK		0xF0
#define TLE9201_REV_MAJOR			0x20

#define TLE9201_DIAG_CL				(1<<4) // over current
#define TLE9201_DIAG_TV				(1<<5) // transmission validation
#define TLE9201_DIAG_OT				(1<<6) // over temperature
#define TLE9201_DIAG_EN				(1<<7) // outputs enabled
#define TLE9201_DIAG_OUT_MASK		0xf // bits 0..3

#define TLE9201_GET_VAL(rx)			((rx) & 0xff)

static bool drv_task_ready = false;

typedef enum {
	TLE9201_DISABLED = 0,
	TLE9201_READY,
	TLE9201_FAILED
} tle9201_drv_state;

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

static size_t diagOkCounter = 0;
constexpr size_t DIAG_OK = 0xF;

// Output states
static const char *diagDiaOut[16] = { "?", "?", "?",
	"VS Undervoltage", // 0x3
	"?",
	"Short to Bat at OUT1 and OUT2", // 0x5
	"Short to GND at OUT1, short to Bat at OUT2", // 0x6
	"Short to Bat at OUT2", // 0x7
	"?",
	"Short to Bat at OUT1, short to GND at OUT2", // 0x9
	"Short to GND at OUT1 and OUT2", // 0xA
	"Short to GND at OUT2", // 0xB
	DRIVER_NAME " Open Load", // 0xC
	"Short to Bat at OUT1", // 0xD
	"Short to GND at OUT1", // 0xE
	"No failure" // 0xF
};

/* OS */
SEMAPHORE_DECL(tle9201_wake, 10 /* or BOARD_TLE9201_COUNT ? */);
static THD_WORKING_AREA(tle9201_thread_1_wa, 256);

/* Driver */
struct Tle9201 {
	int init(int i);

	int spi_rw(uint16_t tx, uint16_t *rx);
	int read_reg(uint8_t addr, uint8_t *val);

	bool get_diag_and_rev(uint8_t *diag, uint8_t *rev);
	void process_diag_and_rev(uint8_t diag, uint8_t rev);

	const tle9201_config		*cfg = NULL;

	tle9201_drv_state			drv_state;
	int idx;
	int detectedRev = 0;
	uint8_t savedDiag = 0;
	char name[10];
};

static Tle9201 chips[BOARD_TLE9201_COUNT];

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

/**
 * @brief TLE9201 send+receive routine.
 */

int Tle9201::spi_rw(uint16_t tx, uint16_t *rx) {
	SPIDriver *spi = cfg->spi_bus;

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	uint16_t rxd = spiPolledExchange(spi, tx);
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);

	// return data
	if (rx) {
		*rx = rxd;
	}

	return 0;
}

int Tle9201::read_reg(uint8_t addr, uint8_t *val) {
	int ret;

	ret = spi_rw(addr, nullptr);
	if (ret) {
		return ret;
	}

	uint16_t rxd;
	ret = spi_rw(addr, &rxd);
	if (ret) {
		return ret;
	}

	if (val) {
		*val = TLE9201_GET_VAL(rxd);
	}

	return 0;
}

bool Tle9201::get_diag_and_rev(uint8_t *diag, uint8_t *rev) {
	int retDiag = read_reg(TLE9201_REG_RD_DIA, diag);
	int retRev = read_reg(TLE9201_REG_RD_REV, rev);
	return (retDiag == 0 && retRev == 0);
}

void Tle9201::process_diag_and_rev(uint8_t diag, uint8_t rev) {
	// react on revision change
	if (rev != detectedRev) {
		if ((rev & TLE9201_REV_MAJOR_MASK) == TLE9201_REV_MAJOR) {
			efiPrintf("%s Detected! (rev=%08x)", name, rev);
		} else {
			efiPrintf("%s ERROR: Unknown revision (%08x)!", name, rev);
		}

		detectedRev = rev;
	}

	// react on diagnosis byte change
	if (diag != savedDiag) {
		efiPrintf("%s Diag (%08x):", name, diag);
		// this bit should be always 0
		if (diag & TLE9201_DIAG_TV) {
			efiPrintf("* Diag status incorrect!");
		}
		// these bits are 0 if something happened
		if (!(diag & TLE9201_DIAG_CL)) {
			efiPrintf("* Overcurrent shutdown!");
		}
		if (!(diag & TLE9201_DIAG_OT)) {
			efiPrintf("* Overtemperature shutdown!");
		}
		if (!(diag & TLE9201_DIAG_EN)) {
			efiPrintf("* Outputs disabled.");
		}
		size_t statusCode = diag & TLE9201_DIAG_OUT_MASK;
		if (statusCode == DIAG_OK) {
		  diagOkCounter++;
		}
		// print the status of the outputs
		efiPrintf("* %s OK=%d", diagDiaOut[statusCode], diagOkCounter);

		savedDiag = diag;
	}
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(tle9201_driver_thread, p) {
	bool wasSpiFailure = false;

	(void)p;

	chRegSetThreadName(DRIVER_NAME);

	while (1) {
		chThdSleepMilliseconds(TLE9201_POLL_INTERVAL_MS);

static bool isInitialized = false;

    if (!isIgnVoltage()) {
      if (isInitialized) {
        efiPrintf("Power loss? Would have to re-init TLE9201?");
        isInitialized = false;
      }
      continue;
    }


		for (int i = 0; i < BOARD_TLE9201_COUNT; i++) {
			auto chip = &chips[i];
			if ((chip->cfg == NULL) ||
				(chip->drv_state == TLE9201_DISABLED) ||
				(chip->drv_state == TLE9201_FAILED)) {
				continue;
      }

			uint8_t diag, rev;
			// get diagnosis and revision bytes
			if (!chip->get_diag_and_rev(&diag, &rev)) {
				if (!wasSpiFailure) {
					efiPrintf("%s ERROR: SPI failure!", chip->name);
					wasSpiFailure = true;
				}
			} else {
				wasSpiFailure = false;
			}

			chip->process_diag_and_rev(diag, rev);
		}
	}
}

/*==========================================================================*/
/* Driver interrupt handlers.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver exported functions.												*/
/*==========================================================================*/

int Tle9201::init(int i) {
	drv_state = TLE9201_READY;
	idx = i;
	sprintf(name, "TLE9201[%d]", idx);

	if (!drv_task_ready) {
		chThdCreateStatic(tle9201_thread_1_wa, sizeof(tle9201_thread_1_wa),
						  PRIO_GPIOCHIP, tle9201_driver_thread, NULL);
		drv_task_ready = true;
	}

	return 0;
}

/**
 * @brief TLE9201 driver add.
 * @details Checks for valid config
 */

int tle9201_add(unsigned int index, const tle9201_config *cfg) {
	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_TLE9201_COUNT)) {
		return -1;
	}

	if (index == 0) {
	  addConsoleAction("reset_9201", [](){
	    auto chip = &chips[0];
	    chip->spi_rw(TLE9201_REG_RES_DIA, nullptr);
	    efiPrintf(DRIVER_NAME "reset!");
	  });
	}

	/* check for valid cs.
	 * TODO: remove this check? CS can be driven by SPI */
	//if (cfg->spi_config.ssport == NULL)
	//	return -1;

	auto& chip = chips[index];

	/* already initted? */
	if (chip.cfg != NULL)
		return -1;

	chip.cfg = cfg;
	chip.init(index);

	return 0;
}

#else /* BOARD_TLE9201_COUNT > 0 */

int tle9201_add(unsigned int index, const tle9201_config *cfg) {
	(void)index; (void)cfg;

	return -1;
}

#endif /* BOARD_TLE9201_COUNT */
