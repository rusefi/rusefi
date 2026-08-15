
#include "pch.h"
#include "defaults.h"
#include "board_overrides.h"
#include "smart_gpio.h"
#include "drivers/gpio/l9779.h"
#include "drivers/gpio/tle9201.h"
#include "m74_9_can.h"

// PB14 is error LED, configured in board.mk
Gpio getCommsLedPin() {
	// this board has LEDs
	return Gpio::Unassigned;
}

Gpio getWarningLedPin() {
	// this board has LEDs
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	// this board has LEDs
	return Gpio::Unassigned;
}

static void setInjectorPins() {
	/* physical wiring (see m74_9.yaml): OUT4 -> Injector 1, OUT3 -> Injector 2,
	 * OUT2 -> Injector 3, OUT1 -> Injector 4 */
	engineConfiguration->injectionPins[0] = Gpio::L9779_OUT_4;
	engineConfiguration->injectionPins[1] = Gpio::L9779_OUT_3;
	engineConfiguration->injectionPins[2] = Gpio::L9779_OUT_2;
	engineConfiguration->injectionPins[3] = Gpio::L9779_OUT_1;

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::L9779_IGN_1;
	engineConfiguration->ignitionPins[1] = Gpio::L9779_IGN_2;
	/* Two following has no IGBT populated, wasted spark is used */
	engineConfiguration->ignitionPins[2] = Gpio::L9779_IGN_3;
	engineConfiguration->ignitionPins[3] = Gpio::L9779_IGN_4;

	engineConfiguration->ignitionPinMode = OM_DEFAULT;
}

static void setupEtb() {
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// The only H-bridge on this board drives the throttle (TS field
	// "H-Bridge #1 function" = etbFunctions1); force it here until the
	// throttle is proven on the bench. Note this overrides the TS field on
	// every boot while it lives in ConfigOverrides.
	engineConfiguration->etbFunctions[0] = DC_Throttle1;

	// PWM pin
	engineConfiguration->etbIo[0].controlPin = Gpio::B14;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::B15;

	// No disablePin here: the TLE9201 DIS pin (pin 11) is enabled by the
	// ETC_EN chain PB13 -> Q5A (NPN, inverts) -> DIS (pulled up to +5V), i.e.
	// the MCU-side polarity is ACTIVE-HIGH (PB13 high = bridge enabled).
	// rusEFI's disable-pin mechanism has no inversion support (fixed
	// OM_DEFAULT: pin low = enable), so disablePin = B13 would drive DIS high
	// (outputs tristate) whenever the firmware thinks the bridge is enabled.
	// PB13 is driven high directly in m74_9_boardInitHardware(); the runtime
	// disable is PWM=0, on which the TLE9201 coasts.

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

/**
 * @brief   Board-specific configuration defaults.

 */
static void m74_9_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	/* SPI1 is used for communication with L9779 */
	engineConfiguration->is_enabled_spi_1 = true;

	engineConfiguration->spi1mosiPin = Gpio::E15;
	engineConfiguration->spi1misoPin = Gpio::E14;
	engineConfiguration->spi1sckPin = Gpio::E13;

  engineConfiguration->triggerInputPins[0] = Gpio::F8;
  engineConfiguration->camInputs[0] = Gpio::B9;

// todo	engineConfiguration->clt.adcChannel = EFI_ADC_; // ADC3 PF5
// todo	engineConfiguration->iat.adcChannel = EFI_ADC_; // ADC3 PF6
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_1; // AC3 -> 74HC14 -> RS358A -> PA1

	// ?k high side/?k low side = ? ratio divider todo is the value below right?
  engineConfiguration->analogInputDividerCoefficient = 2.0f;

//	todo engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

//	engineConfiguration->vbattAdcChannel = EFI_ADC_;
	engineConfiguration->adcVcc = 3.3f;

  setTPS1Inputs(EFI_ADC_12, EFI_ADC_13);

  setPPSInputs(EFI_ADC_10, EFI_ADC_11);
}

static void m74_9_boardConfigOverrides() {
	/* Ignition key detection for isIgnVoltage() (the TLE9201 ETB gate):
	 * the IGN_KEY line (connector BF2) is wired to the L9779 KEY_ON input
	 * (pin 11), not to an MCU pin, so read it back over SPI via DIA_REG9
	 * KEY_ON_STATUS (implemented in the l9779 driver readPad()).
	 * This lives in ConfigOverrides (applied on every boot), not in the
	 * DefaultConfiguration (applied only on config reset), because the
	 * stored tune predates this pin assignment. */
	engineConfiguration->ignitionKeyDigitalPin = Gpio::L9779_PIN_KEY;

	/* SPI2 for the TLE9201 ETB H-bridge diagnostics (see setupEtb). Must live
	 * in ConfigOverrides (applied on every boot), not in the DefaultConfiguration
	 * (applied only on config reset), for the same reason as the ignition key
	 * pin above: the stored tune predates SPI2, so initSpiModules() would skip
	 * the bus and PD1/PD3/PD4 would never be configured for AF6. */
	engineConfiguration->is_enabled_spi_2 = true;
	engineConfiguration->spi2sckPin = Gpio::D1;  // PD1 AF6, ETC_SCK
	engineConfiguration->spi2misoPin = Gpio::D3; // PD3 AF6, ETC_SO
	engineConfiguration->spi2mosiPin = Gpio::D4; // PD4 AF6, ETC_SI

	//CAN 1 bus overwrites
	engineConfiguration->canRxPin = Gpio::G0;
	engineConfiguration->canTxPin = Gpio::G1;
	setupEtb();
}

static struct l9779_config l9779_cfg = {
	.spi_bus = &SPID1,
	.spi_config = {
		.circular = false,
#if defined(_CHIBIOS_RT_CONF_VER_6_1_) || defined(AT32F4XX)
		.end_cb = nullptr,
#else
		.slave = false,
		.data_cb = nullptr,
		.error_cb = nullptr,
#endif
		.ssport = GPIOE,
		.sspad = 12,
		.cr1 =
			SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			/* L9779WD-SPI: MSB-first 16-bit frames (datasheet 6.16.2), data
			 * latched on the rising edge of SCK and shifted on the falling
			 * edge. Mode 0 (CPOL=0, CPHA=0) and mode 1 (CPOL=0, CPHA=1) are
			 * both rejected by the chip with SPI_ERR (DO=0x8000) on every
			 * frame; Figure 73 shows SCK idling high, which together with the
			 * latch-on-rising requirement gives SPI mode 3 (CPOL=1, CPHA=1).
			 * Transfer clock stays under the 8 MHz limit (Table 53):
			 * div 32 = 4.5 MHz @ 144 MHz APB2 (div 16 = 9 MHz exceeds it). */
			((4 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	// div = 32
			SPI_CR1_MSTR |
			SPI_CR1_CPOL |
			SPI_CR1_CPHA |
			0,
		.cr2 = SPI_CR2_16BIT_MODE
	},
	.direct_gpio = {
		/* ignition pre-drivers */
		[0] = {.port = GPIOF, .pad = 12},
		[1] = {.port = GPIOF, .pad = 13},
		[2] = {.port = GPIOF, .pad = 14},
		[3] = {.port = GPIOF, .pad = 15},
		/* IN1 .. IN7 */
		[4] = {.port = GPIOE, .pad = 8},
		[5] = {.port = GPIOE, .pad = 9},
		[6] = {.port = GPIOE, .pad = 10},
		[7] = {.port = GPIOE, .pad = 11},
		[8] = {.port = GPIOG, .pad = 5},
		[9] = {.port = GPIOG, .pad = 6},
		[10] = {.port = GPIOG, .pad = 7},
	},
	/* PWM signal */
	.pwm_gpio = {.port = NULL, .pad = 0}
};

static struct tle9201_config tle9201_cfg = {
	.spi_bus = &SPID2,
	.spi_config = {
		.circular = false,
#if defined(_CHIBIOS_RT_CONF_VER_6_1_) || defined(AT32F4XX)
		.end_cb = nullptr,
#else
		.slave = false,
		.data_cb = nullptr,
		.error_cb = nullptr,
#endif
		/* TLE9201 ETB H-bridge: CS on PD0, GPIO-driven (idle high) */
		.ssport = GPIOD,
		.sspad = 0,
		.cr1 = TLE9201_CONFIG_CR1,
		.cr2 = TLE9201_CONFIG_CR2
	}
};

static void board_init_ext_gpios()
{
	int ret;

	/* L9779 only registers a gpiochip here (safe pre-OS); its thread and SPI
	 * start later via gpiochips_init(). TLE9201 must NOT be added here: its
	 * init() creates a thread (chThdCreateStatic), which hangs when called
	 * from boardInit() before chSysInit() - see m74_9_boardInitHardware. */
	ret = l9779_add(Gpio::L9779_IGN_1, 0, &l9779_cfg);
	if (ret < 0) {
		/* error */
	}
}

/**
 * @brief Called from halInit() BEFORE the kernel exists (chSysInit).
 * @details Only L9779 may be registered here: it just registers a gpiochip,
 * its thread starts later via gpiochips_init(). TLE9201 must NOT be added
 * here - its init() calls chThdCreateStatic() and hangs the board at power-on.
 */
void boardInit() {
	board_init_ext_gpios();
}

/**
 * @brief Board-specific init after the kernel is up.
 * @details Runs from custom_board_InitHardware (post chSysInit, before
 * initSpiModules). TLE9201::init() calls chThdCreateStatic(), which must not
 * run from boardInit() - boardInit() is called from halInit() before the
 * kernel exists and the board hangs at power-on. Same pattern as
 * hellen154hyundai_f7.
 */
	static void m74_9_boardInitHardware() {
	/* TLE9201 ETB H-bridge: ETC_CS on PD0 */
	gpio_pin_markUsed(GPIOD, 0, "TLE9201 ETB CS");
	palSetPadMode(GPIOD, 0, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPad(GPIOD, 0);

	/* ETC_EN on PB13 -> Q5A (NPN, inverts) -> TLE9201 DIS (pin 11, pulled up
	 * to +5V). PB13 high = Q5A on = DIS low = bridge enabled. Must be driven
	 * here (not via etbIo[].disablePin: that path is fixed OM_DEFAULT, and
	 * low=enable would leave DIS high = outputs tristate). At boot PB13 is a
	 * weak pullup only, insufficient to turn Q5A on (internal 10k base-emitter
	 * divider), so DIS stays pulled high -> tristate until this runs. */
	gpio_pin_markUsed(GPIOB, 13, "ETC_EN");
	palSetPadMode(GPIOB, 13, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPad(GPIOB, 13);

	int ret = tle9201_add(0, &tle9201_cfg);
	efiPrintf("tle9201_add()=%d", ret);
}

#if EFI_PROD_CODE && HAL_USE_ADC

#include "adc_device.h"
#if EFI_USE_FAST_ADC
extern AdcDevice fastAdc;
#endif

/* Temporary diagnostic for the AT32 fast ADC (TIM6 -> ADC2) bring-up.
 * Prints the state of every stage of the fast ADC chain. */
static void m74_9FastAdcDiag() {
	/* TIM6 GPT trigger */
	efiPrintf("TIM6 CR1=0x%08x DIER=0x%08x SR=0x%04x CNT=%u PSC=%u ARR=%u",
		(unsigned)TIM6->CR1, (unsigned)TIM6->DIER, (unsigned)TIM6->SR,
		(unsigned)TIM6->CNT, (unsigned)TIM6->PSC, (unsigned)TIM6->ARR);
	efiPrintf("GPTD6 state=%d", (int)GPTD6.state);

	/* ADC2 */
	efiPrintf("ADC2 state=%d SR=0x%08x CR1=0x%08x CR2=0x%08x",
		(int)ADCD2.state, (unsigned)ADC2->SR, (unsigned)ADC2->CR1, (unsigned)ADC2->CR2);
	efiPrintf("ADC2 SQR1=0x%08x SQR2=0x%08x SQR3=0x%08x ADC_CCR=0x%08x",
		(unsigned)ADC2->SQR1, (unsigned)ADC2->SQR2, (unsigned)ADC2->SQR3, (unsigned)ADC->CCR);

	/* ADC2 DMA stream */
	const stm32_dma_stream_t* s = ADCD2.dmastp;
	if (s != nullptr) {
		efiPrintf("ADC2 DMA dma=%p ch=%p mux=%p vector=%d shift=%d",
			s->dma, s->channel, s->mux, (int)s->vector, (int)s->shift);
		if (s->channel != nullptr) {
			efiPrintf("ADC2 DMA CCR=0x%08x CNDTR=%u CPAR=%p CMAR=%p",
				(unsigned)s->channel->CCR, (unsigned)s->channel->CNDTR,
				(void*)s->channel->CPAR, (void*)s->channel->CMAR);
		}
		if (s->mux != nullptr) {
			efiPrintf("ADC2 DMAMUX CTRL=0x%08x", (unsigned)s->mux->CCR);
		}
		if (s->dma != nullptr) {
			efiPrintf("ADC2 DMA ISR=0x%08x IFCR=0x%08x MUXSEL=0x%08x",
				(unsigned)s->dma->ISR, (unsigned)s->dma->IFCR, (unsigned)s->dma->MUXSEL);
		}
	} else {
		efiPrintf("ADC2 dmastp is NULL");
	}

	/* working ADC1 DMA stream for comparison */
	const stm32_dma_stream_t* s1 = ADCD1.dmastp;
	if (s1 != nullptr && s1->channel != nullptr && s1->mux != nullptr) {
		efiPrintf("ADC1 DMA ch=%p mux=%p CCR=0x%08x CNDTR=%u DMAMUX=0x%08x",
			s1->channel, s1->mux,
			(unsigned)s1->channel->CCR, (unsigned)s1->channel->CNDTR, (unsigned)s1->mux->CCR);
	}

#if EFI_USE_FAST_ADC
	efiPrintf("fast err=%d cnt=%d lastErr=%d channels=%d",
		(int)engine->outputChannels.fastAdcErrorCount,
		(int)engine->outputChannels.fastAdcConversionCount,
		(int)engine->outputChannels.fastAdcLastError,
		(int)fastAdc.size());
#else
	efiPrintf("fast ADC compiled out");
#endif
}
#endif /* EFI_PROD_CODE && HAL_USE_ADC */

static Gpio OUTPUTS[] = {
	Gpio::L9779_OUT_4, // Injector 1
	Gpio::L9779_OUT_3, // Injector 2
//	Gpio::L9779_OUT_2, // Injector 3
//	Gpio::L9779_OUT_1, // Injector 4
//	Gpio::L9779_OUT_6, // Oxygen sensor 1 heater
//	Gpio::L9779_OUT_5, // EVAP solenoid control
//	Gpio::L9779_OUT_7, // Oxygen sensor 2 heater
//	Gpio::L9779_IGN_1, // Coil 1 (< +2.5v) / Coils 1,4
//	Gpio::L9779_IGN_3, // Coil 3  (< +2.5v) / Coils 2,4
//	Gpio::L9779_OUT_17, // Air compressor control
//	Gpio::L9779_OUT_14, // Secondary starter relay
//	Gpio::L9779_OUT_15, // FAN 1 relay
//	Gpio::L9779_OUT_16, // FAN 2 relay
//	Gpio::L9779_OUT_13, // Fuel pump relay
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaDcOutputsCount() {
    return 1;
}
void setup_custom_board_overrides() {
	custom_board_InitHardware = m74_9_boardInitHardware;
	custom_board_DefaultConfiguration = m74_9_boardDefaultConfiguration;
	custom_board_ConfigOverrides = m74_9_boardConfigOverrides;
#if EFI_PROD_CODE && HAL_USE_ADC
	addConsoleAction("fastadcdiag", m74_9FastAdcDiag);
#endif
#if EFI_CAN_SUPPORT
	initM74_9Can();
	custom_board_isImmobilizerBlocking = m74_9_isImmobilizerBlocking;
#endif // EFI_CAN_SUPPORT
}

