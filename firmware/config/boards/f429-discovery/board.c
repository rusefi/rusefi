/**
 * @file boards/f429-discovery/board.c
 *
 * @date Jan 08, 2022
 * @author Andrey Gusakov, 2022
 */

#include "hal.h"
#include "hal_community.h"
#include "hal_sdram_lld.h"
/* for UNUSED() */
#include "efilib.h"

#include "board.h"

/*
 * SDRAM driver configuration structure.
 */
static const SDRAMConfig sdram_cfg = {
	.sdcr = (uint32_t) (FMC_ColumnBits_Number_8b |
						FMC_RowBits_Number_12b |
						FMC_SDMemory_Width_16b |
						FMC_InternalBank_Number_4 |
						FMC_CAS_Latency_3 |
						FMC_Write_Protection_Disable |
						FMC_SDClock_Period_2 |
						FMC_Read_Burst_Disable |
						FMC_ReadPipe_Delay_1),

	.sdtr = (uint32_t)( (2   - 1) |	// FMC_LoadToActiveDelay = 2 (TMRD: 2 Clock cycles)
						(7 <<  4) |	// FMC_ExitSelfRefreshDelay = 7 (TXSR: min=70ns (7x11.11ns))
						(4 <<  8) |	// FMC_SelfRefreshTime = 4 (TRAS: min=42ns (4x11.11ns) max=120k (ns))
						(7 << 12) |	// FMC_RowCycleDelay = 7 (TRC:  min=70 (7x11.11ns))
						(2 << 16) |	// FMC_WriteRecoveryTime = 2 (TWR:  min=1+ 7ns (1+1x11.11ns))
						(2 << 20) |	// FMC_RPDelay = 2 (TRP:  20ns => 2x11.11ns)
						(2 << 24)),	// FMC_RCDDelay = 2 (TRCD: 20ns => 2x11.11ns)

	.sdcmr = (uint32_t)(((4 - 1) << 5) |
						((FMC_SDCMR_MRD_BURST_LENGTH_2 |
							FMC_SDCMR_MRD_BURST_TYPE_SEQUENTIAL |
							FMC_SDCMR_MRD_CAS_LATENCY_3 |
							FMC_SDCMR_MRD_OPERATING_MODE_STANDARD |
							FMC_SDCMR_MRD_WRITEBURST_MODE_SINGLE) << 9)),

	/* if (STM32_SYSCLK == 180000000) ->
		 64ms / 4096 = 15.625us
		 15.625us * 90MHz = 1406 - 20 = 1386 */
	//.sdrtr = (1386 << 1),
	.sdrtr = (uint32_t)(683 << 1),
};

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of STM32 GPIO port setup.
 */
typedef struct {
  uint32_t              moder;
  uint32_t              otyper;
  uint32_t              ospeedr;
  uint32_t              pupdr;
  uint32_t              odr;
  uint32_t              afrl;
  uint32_t              afrh;
} gpio_setup_t;

/**
 * @brief   Type of STM32 GPIO initialization data.
 */
typedef struct {
#if STM32_HAS_GPIOA || defined(__DOXYGEN__)
  gpio_setup_t          PAData;
#endif
#if STM32_HAS_GPIOB || defined(__DOXYGEN__)
  gpio_setup_t          PBData;
#endif
#if STM32_HAS_GPIOC || defined(__DOXYGEN__)
  gpio_setup_t          PCData;
#endif
#if STM32_HAS_GPIOD || defined(__DOXYGEN__)
  gpio_setup_t          PDData;
#endif
#if STM32_HAS_GPIOE || defined(__DOXYGEN__)
  gpio_setup_t          PEData;
#endif
#if STM32_HAS_GPIOF || defined(__DOXYGEN__)
  gpio_setup_t          PFData;
#endif
#if STM32_HAS_GPIOG || defined(__DOXYGEN__)
  gpio_setup_t          PGData;
#endif
#if STM32_HAS_GPIOH || defined(__DOXYGEN__)
  gpio_setup_t          PHData;
#endif
#if STM32_HAS_GPIOI || defined(__DOXYGEN__)
  gpio_setup_t          PIData;
#endif
#if STM32_HAS_GPIOJ || defined(__DOXYGEN__)
  gpio_setup_t          PJData;
#endif
#if STM32_HAS_GPIOK || defined(__DOXYGEN__)
  gpio_setup_t          PKData;
#endif
} gpio_config_t;

/**
 * @brief   STM32 GPIO static initialization data.
 */
static const gpio_config_t gpio_default_config = {
#if STM32_HAS_GPIOA
  {VAL_GPIOA_MODER, VAL_GPIOA_OTYPER, VAL_GPIOA_OSPEEDR, VAL_GPIOA_PUPDR,
   VAL_GPIOA_ODR,   VAL_GPIOA_AFRL,   VAL_GPIOA_AFRH},
#endif
#if STM32_HAS_GPIOB
  {VAL_GPIOB_MODER, VAL_GPIOB_OTYPER, VAL_GPIOB_OSPEEDR, VAL_GPIOB_PUPDR,
   VAL_GPIOB_ODR,   VAL_GPIOB_AFRL,   VAL_GPIOB_AFRH},
#endif
#if STM32_HAS_GPIOC
  {VAL_GPIOC_MODER, VAL_GPIOC_OTYPER, VAL_GPIOC_OSPEEDR, VAL_GPIOC_PUPDR,
   VAL_GPIOC_ODR,   VAL_GPIOC_AFRL,   VAL_GPIOC_AFRH},
#endif
#if STM32_HAS_GPIOD
  {VAL_GPIOD_MODER, VAL_GPIOD_OTYPER, VAL_GPIOD_OSPEEDR, VAL_GPIOD_PUPDR,
   VAL_GPIOD_ODR,   VAL_GPIOD_AFRL,   VAL_GPIOD_AFRH},
#endif
#if STM32_HAS_GPIOE
  {VAL_GPIOE_MODER, VAL_GPIOE_OTYPER, VAL_GPIOE_OSPEEDR, VAL_GPIOE_PUPDR,
   VAL_GPIOE_ODR,   VAL_GPIOE_AFRL,   VAL_GPIOE_AFRH},
#endif
#if STM32_HAS_GPIOF
  {VAL_GPIOF_MODER, VAL_GPIOF_OTYPER, VAL_GPIOF_OSPEEDR, VAL_GPIOF_PUPDR,
   VAL_GPIOF_ODR,   VAL_GPIOF_AFRL,   VAL_GPIOF_AFRH},
#endif
#if STM32_HAS_GPIOG
  {VAL_GPIOG_MODER, VAL_GPIOG_OTYPER, VAL_GPIOG_OSPEEDR, VAL_GPIOG_PUPDR,
   VAL_GPIOG_ODR,   VAL_GPIOG_AFRL,   VAL_GPIOG_AFRH},
#endif
#if STM32_HAS_GPIOH
  {VAL_GPIOH_MODER, VAL_GPIOH_OTYPER, VAL_GPIOH_OSPEEDR, VAL_GPIOH_PUPDR,
   VAL_GPIOH_ODR,   VAL_GPIOH_AFRL,   VAL_GPIOH_AFRH},
#endif
#if STM32_HAS_GPIOI
  {VAL_GPIOI_MODER, VAL_GPIOI_OTYPER, VAL_GPIOI_OSPEEDR, VAL_GPIOI_PUPDR,
   VAL_GPIOI_ODR,   VAL_GPIOI_AFRL,   VAL_GPIOI_AFRH},
#endif
#if STM32_HAS_GPIOJ
  {VAL_GPIOJ_MODER, VAL_GPIOJ_OTYPER, VAL_GPIOJ_OSPEEDR, VAL_GPIOJ_PUPDR,
   VAL_GPIOJ_ODR,   VAL_GPIOJ_AFRL,   VAL_GPIOJ_AFRH},
#endif
#if STM32_HAS_GPIOK
  {VAL_GPIOK_MODER, VAL_GPIOK_OTYPER, VAL_GPIOK_OSPEEDR, VAL_GPIOK_PUPDR,
   VAL_GPIOK_ODR,   VAL_GPIOK_AFRL,   VAL_GPIOK_AFRH}
#endif
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void gpio_init(stm32_gpio_t *gpiop, const gpio_setup_t *config) {

  gpiop->OTYPER  = config->otyper;
  gpiop->OSPEEDR = config->ospeedr;
  gpiop->PUPDR   = config->pupdr;
  gpiop->ODR     = config->odr;
  gpiop->AFRL    = config->afrl;
  gpiop->AFRH    = config->afrh;
  gpiop->MODER   = config->moder;
}

static void stm32_gpio_init(void) {

  /* Enabling GPIO-related clocks, the mask comes from the
     registry header file.*/
  rccResetAHB1(STM32_GPIO_EN_MASK);
  rccEnableAHB1(STM32_GPIO_EN_MASK, true);

  /* Initializing all the defined GPIO ports.*/
#if STM32_HAS_GPIOA
  gpio_init(GPIOA, &gpio_default_config.PAData);
#endif
#if STM32_HAS_GPIOB
  gpio_init(GPIOB, &gpio_default_config.PBData);
#endif
#if STM32_HAS_GPIOC
  gpio_init(GPIOC, &gpio_default_config.PCData);
#endif
#if STM32_HAS_GPIOD
  gpio_init(GPIOD, &gpio_default_config.PDData);
#endif
#if STM32_HAS_GPIOE
  gpio_init(GPIOE, &gpio_default_config.PEData);
#endif
#if STM32_HAS_GPIOF
  gpio_init(GPIOF, &gpio_default_config.PFData);
#endif
#if STM32_HAS_GPIOG
  gpio_init(GPIOG, &gpio_default_config.PGData);
#endif
#if STM32_HAS_GPIOH
  gpio_init(GPIOH, &gpio_default_config.PHData);
#endif
#if STM32_HAS_GPIOI
  gpio_init(GPIOI, &gpio_default_config.PIData);
#endif
#if STM32_HAS_GPIOJ
  gpio_init(GPIOJ, &gpio_default_config.PJData);
#endif
#if STM32_HAS_GPIOK
  gpio_init(GPIOK, &gpio_default_config.PKData);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

#define SDRAM   ((FSMC_SDRAM_TypeDef *)FSMC_Bank5_6_R_BASE)

/**
 * FMC_Command_Mode
 */
#define FMCCM_NORMAL              ((uint32_t)0x00000000)
#define FMCCM_CLK_ENABLED         ((uint32_t)0x00000001)
#define FMCCM_PALL                ((uint32_t)0x00000002)
#define FMCCM_AUTO_REFRESH        ((uint32_t)0x00000003)
#define FMCCM_LOAD_MODE           ((uint32_t)0x00000004)
#define FMCCM_SELFREFRESH         ((uint32_t)0x00000005)
#define FMCCM_POWER_DOWN          ((uint32_t)0x00000006)

static void __early_sdram_wait_ready(void) {
  /* Wait until the SDRAM controller is ready */
  while (SDRAM->SDSR & FMC_SDSR_BUSY);
}

static void __early_sdram_delay(void)
{
  /* something > 100uS */
  volatile int tmp = 168 * 1000 * 100;

  do {
    tmp--;
  } while(tmp);
}

static void __early_sdram_init(const SDRAMConfig *config)
{
  uint32_t command_target = 0;

  #ifdef rccResetFSMC
    rccResetFSMC();
  #endif
  rccEnableFSMC(FALSE);

  SDRAM->SDCR1 = config->sdcr;
  SDRAM->SDTR1 = config->sdtr;
  SDRAM->SDCR2 = config->sdcr;
  SDRAM->SDTR2 = config->sdtr;

#if STM32_SDRAM_USE_SDRAM1
  command_target |= FMC_SDCMR_CTB1;
#endif
#if STM32_SDRAM_USE_SDRAM2
  command_target |= FMC_SDCMR_CTB2;
#endif

  /* Step 3: Configure a clock configuration enable command.*/
  __early_sdram_wait_ready();
  SDRAM->SDCMR = FMCCM_CLK_ENABLED | command_target;

  /* Step 4: Insert delay (tipically 100uS).*/
  __early_sdram_delay();

  /* Step 5: Configure a PALL (precharge all) command.*/
  __early_sdram_wait_ready();
  SDRAM->SDCMR = FMCCM_PALL | command_target;

  /* Step 6.1: Configure a Auto-Refresh command: send the first command.*/
  __early_sdram_wait_ready();
  SDRAM->SDCMR = FMCCM_AUTO_REFRESH | command_target |
      (config->sdcmr & FMC_SDCMR_NRFS);

  /* Step 6.2: Send the second command.*/
  __early_sdram_wait_ready();
  SDRAM->SDCMR = FMCCM_AUTO_REFRESH | command_target |
      (config->sdcmr & FMC_SDCMR_NRFS);

  /* Step 7: Program the external memory mode register.*/
  __early_sdram_wait_ready();
  SDRAM->SDCMR = FMCCM_LOAD_MODE | command_target |
      (config->sdcmr & FMC_SDCMR_MRD);

  /* Step 8: Set clock.*/
  __early_sdram_wait_ready();
  SDRAM->SDRTR = config->sdrtr & FMC_SDRTR_COUNT;

  __early_sdram_wait_ready();
}

static int __early_sdram_test(void *base, size_t size)
{
  size_t i;
  uint32_t *ptr = base;

  /* test 0 */
  for (i = 0; i < size / sizeof(uint32_t); i++) {
    ptr[i] = 0;
  }

  for (i = 0; i < size / sizeof(uint32_t); i++) {
    if (ptr[i] != 0)
      return -1;
  }

  /* test 1 */
  for (i = 0; i < size / sizeof(uint32_t); i++) {
    ptr[i] = 0xffffffff;
  }

  for (i = 0; i < size / sizeof(uint32_t); i++) {
    if (ptr[i] != 0xffffffff)
      return -1;
  }

  /* test 2 */
  for (i = 0; i < size / sizeof(uint32_t); i++) {
    ptr[i] = i;
  }

  for (i = 0; i < size / sizeof(uint32_t); i++) {
    if (ptr[i] != i)
      return -1;
  }

  return 0;
}

/**
 * @brief   Early initialization code.
 * @details GPIO ports and system clocks are initialized before everything
 *          else.
 */
void __early_init(void) {

  stm32_gpio_init();
  stm32_clock_init();

  /*
   * Initialise FSMC for SDRAM.
   */
#if 0
  /* clear driver struct */
  memset(&SDRAMD1, 0 sizeof(SDRAMD1));
  sdramInit();
  sdramStart(&SDRAMD1, &sdram_cfg);
#else
  __early_sdram_init(&sdram_cfg);
#endif

  if (0) {
    /* yes, hardcoded values */
    __early_sdram_test((void *) 0xD0000000, 8 * 1024 * 1024);
  }
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
