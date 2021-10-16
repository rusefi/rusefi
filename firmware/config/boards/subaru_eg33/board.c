/**
 * @file boards/subaru_eg33/board.c
 *
 * @date Feb 06, 2021
 * @author Andrey Gusakov, 2021
 */

#include "hal.h"
#include "stm32_gpio.h"
#include "efilib.h"
#include "rusefi_hw_enums.h"

#include "board_io.h"

/* drivers */

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
	gpio_setup_t	PIData;
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
	{VAL_GPIOI_MODER, VAL_GPIOI_OTYPER, VAL_GPIOI_OSPEEDR, VAL_GPIOI_PUPDR,
	 VAL_GPIOI_ODR,	 VAL_GPIOI_AFRL, VAL_GPIOI_AFRH},
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
	gpio_init(GPIOI, &gpio_default_config.PIData);
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
