/*
 * ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *		 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/*
 * This file has been automatically generated using ChibiStudio board
 * generator plugin. Do not edit manually.
 */

#include "hal.h"
#include "stm32_gpio.h"
#include "efilib.h"

/* drivers */
#include "drivers/gpio/tle6240.h"
#include "drivers/gpio/mc33972.h"

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/**
 * @brief Type of STM32 GPIO port setup.
 */
typedef struct {
	uint32_t	moder;
	uint32_t	otyper;
	uint32_t	ospeedr;
	uint32_t	pupdr;
	uint32_t	odr;
	uint32_t	afrl;
	uint32_t	afrh;
} gpio_setup_t;

/**
 * @brief Type of STM32 GPIO initialization data.
 */
typedef struct {
	gpio_setup_t	PAData;
	gpio_setup_t	PBData;
	gpio_setup_t	PCData;
	gpio_setup_t	PDData;
	gpio_setup_t	PEData;
	gpio_setup_t	PFData;
	gpio_setup_t	PGData;
	gpio_setup_t	PHData;
} gpio_config_t;

/**
 * @brief STM32 GPIO static initialization data.
 */
static const gpio_config_t gpio_default_config = {
	{VAL_GPIOA_MODER, VAL_GPIOA_OTYPER, VAL_GPIOA_OSPEEDR, VAL_GPIOA_PUPDR,
	 VAL_GPIOA_ODR,	 VAL_GPIOA_AFRL, VAL_GPIOA_AFRH},
	{VAL_GPIOB_MODER, VAL_GPIOB_OTYPER, VAL_GPIOB_OSPEEDR, VAL_GPIOB_PUPDR,
	 VAL_GPIOB_ODR,	 VAL_GPIOB_AFRL, VAL_GPIOB_AFRH},
	{VAL_GPIOC_MODER, VAL_GPIOC_OTYPER, VAL_GPIOC_OSPEEDR, VAL_GPIOC_PUPDR,
	 VAL_GPIOC_ODR,	 VAL_GPIOC_AFRL, VAL_GPIOC_AFRH},
	{VAL_GPIOD_MODER, VAL_GPIOD_OTYPER, VAL_GPIOD_OSPEEDR, VAL_GPIOD_PUPDR,
	 VAL_GPIOD_ODR,	 VAL_GPIOD_AFRL, VAL_GPIOD_AFRH},
	{VAL_GPIOE_MODER, VAL_GPIOE_OTYPER, VAL_GPIOE_OSPEEDR, VAL_GPIOE_PUPDR,
	 VAL_GPIOE_ODR,	 VAL_GPIOE_AFRL, VAL_GPIOE_AFRH},
	{VAL_GPIOF_MODER, VAL_GPIOF_OTYPER, VAL_GPIOF_OSPEEDR, VAL_GPIOF_PUPDR,
	 VAL_GPIOF_ODR,	 VAL_GPIOF_AFRL, VAL_GPIOF_AFRH},
	{VAL_GPIOG_MODER, VAL_GPIOG_OTYPER, VAL_GPIOG_OSPEEDR, VAL_GPIOG_PUPDR,
	 VAL_GPIOG_ODR,	 VAL_GPIOG_AFRL, VAL_GPIOG_AFRH},
	{VAL_GPIOH_MODER, VAL_GPIOH_OTYPER, VAL_GPIOH_OSPEEDR, VAL_GPIOH_PUPDR,
	 VAL_GPIOH_ODR,	 VAL_GPIOH_AFRL, VAL_GPIOH_AFRH},
};

const struct tle6240_config tle6240 = {
	.spi_bus = &SPID4,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = GPIOF,
		.sspad = 0U,
		.cr1 =
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			/* SPI_CR1_LSBFIRST | */
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
			SPI_CR1_MSTR |
			/* SPI_CR1_CPOL | */ // = 0
			SPI_CR1_CPHA | // = 1
			0,
		/* 16-bit transfer */
		.cr2 = SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
	},
	.direct_io = {
		/* IN1  - D_TACH_OUT */
		[0] = {.port = GPIOG, .pad = 2},
		/* IN2..4 grounded */
		[1] = {.port = NULL},
		[2] = {.port = NULL},
		[3] = {.port = NULL},
		/* IN9  - D_INJ_5 */
		[4] = {.port = GPIOD, .pad = 15},
		/* IN10 - D_WASTGATE */
		[5] = {.port = GPIOD, .pad = 14},
		/* IN11 - D_IDLE_OPEN */
		[6] = {.port = GPIOC, .pad = 6},
		/* IN12 - D_IDLE_CLOSE */
		[7] = {.port = GPIOC, .pad = 7},
	},
	.reset = {.port = GPIOG, .pad = 3}
};


const struct mc33972_config mc33972 = {
	.spi_bus = &SPID4,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = GPIOB,
		.sspad = 4U,
		.cr1 =
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			/* SPI_CR1_LSBFIRST | */
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
			SPI_CR1_MSTR |
			/* SPI_CR1_CPOL | */ /* = 0 */
			SPI_CR1_CPHA | /* = 1 */
			0,
		/* 3 x 8-bit transfer */
		.cr2 = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
	},
};

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

static void gpio_init(stm32_gpio_t *gpiop, const gpio_setup_t *config) {

	gpiop->OTYPER	= config->otyper;
	gpiop->OSPEEDR	= config->ospeedr;
	gpiop->PUPDR	= config->pupdr;
	gpiop->ODR		= config->odr;
	gpiop->AFRL		= config->afrl;
	gpiop->AFRH		= config->afrh;
	gpiop->MODER	= config->moder;
}

static void stm32_gpio_init(void) {

	/* Enabling GPIO-related clocks, the mask comes from the
	 * registry header file.*/
	rccResetAHB1(STM32_GPIO_EN_MASK);
	rccEnableAHB1(STM32_GPIO_EN_MASK, true);

	/* Initializing all the defined GPIO ports.*/
	gpio_init(GPIOA, &gpio_default_config.PAData);
	gpio_init(GPIOB, &gpio_default_config.PBData);
	gpio_init(GPIOC, &gpio_default_config.PCData);
	gpio_init(GPIOD, &gpio_default_config.PDData);
	gpio_init(GPIOE, &gpio_default_config.PEData);
	gpio_init(GPIOF, &gpio_default_config.PFData);
	gpio_init(GPIOG, &gpio_default_config.PGData);
	gpio_init(GPIOH, &gpio_default_config.PHData);
}

/*==========================================================================*/
/* Driver interrupt handlers.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver exported functions.												*/
/*==========================================================================*/

/**
 * @brief Early initialization code.
 * @details GPIO ports and system clocks are initialized before everything
 * else.
 */
void __early_init(void)
{
	/* allow debug in all low-power modes */
	DBGMCU->CR |= DBGMCU_CR_DBG_SLEEP | DBGMCU_CR_DBG_STOP | DBGMCU_CR_DBG_STANDBY;

	stm32_gpio_init();
	stm32_clock_init();
}

#if HAL_USE_SDC || defined(__DOXYGEN__)
/**
 * @brief SDC card detection.
 */
bool sdc_lld_is_card_inserted(SDCDriver *sdcp)
{
	UNUSED(sdcp);
	/* TODO: Fill the implementation.*/
	return true;
}

/**
 * @brief SDC card write protection detection.
 */
bool sdc_lld_is_write_protected(SDCDriver *sdcp)
{
	UNUSED(sdcp);
	/* TODO: Fill the implementation.*/
	return false;
}
#endif /* HAL_USE_SDC */

#if HAL_USE_MMC_SPI || defined(__DOXYGEN__)
/**
 * @brief MMC_SPI card detection.
 */
bool mmc_lld_is_card_inserted(MMCDriver *mmcp)
{
	UNUSED(mmcp);
	/* TODO: Fill the implementation.*/
	return true;
}

/**
 * @brief MMC_SPI card write protection detection.
 */
bool mmc_lld_is_write_protected(MMCDriver *mmcp)
{
	UNUSED(mmcp);
	/* TODO: Fill the implementation.*/
	return false;
}
#endif

/**
 * @brief Board-specific initialization code.
 * @todo  Add your board-specific code, if any.
 */
void boardInit(void)
{
	tle6240_add(0, &tle6240);

	mc33972_add(0, &mc33972);
}

/**
 * @brief   Board-specific ADC channel configuration code overrides.
 * @todo    Add your board-specific code, if any.
 */
void setAdcChannelOverrides(void) {
}


#define STATUS_LED_PIN	8
#define STATUS_LED_PORT	GPIOG

void BLIIINK(int t) {
	int i, j;
	palSetPadMode(STATUS_LED_PORT, STATUS_LED_PIN, PAL_MODE_OUTPUT_PUSHPULL);
	//while (true) {
	for (j = 0; j < 2; j++) {
		palClearPad(STATUS_LED_PORT, STATUS_LED_PIN);
		for (i = 0; i < 2 * t; i++) {
			palTogglePad(STATUS_LED_PORT, STATUS_LED_PIN);
			chThdSleepMilliseconds(250);
		}
		chThdSleepMilliseconds(1000);
	}
}
