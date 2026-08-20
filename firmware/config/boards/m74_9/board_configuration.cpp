
#include "pch.h"
#include "defaults.h"
#include "board_overrides.h"
#include "smart_gpio.h"
#include "drivers/gpio/l9779.h"
#include "drivers/gpio/tle9201.h"
#include "m74_9_can.h"
#include "m74_9_tooth_diag.h"

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
	/* Individual coils (COP): each of the four L9779 ignition pre-driver
	 * channels (IGN1..4, external IGBTs) drives its own cylinder coil.
	 * Physical wiring (m74_9.yaml): AL1 = IGN_1 -> Coil 1, AM2 = IGN_2 ->
	 * Coil 2, AM1 = IGN_3 -> Coil 3, AM3 = IGN_4 -> Coil 4. */
	engineConfiguration->ignitionPins[0] = Gpio::L9779_IGN_1;
	engineConfiguration->ignitionPins[1] = Gpio::L9779_IGN_2;
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

	/* Individual coils (COP): the board has all four L9779 ignition
	 * pre-driver channels populated, each driving its own coil. Wasted
	 * spark here only schedules ignitionPins[0] and [1]
	 * (getIgnitionPinForIndex: cylinderIndex % (cylinders/2)), so with 4
	 * COP coils two cylinders never get spark - the engine catches
	 * periodically on two cylinders and free-spins in between. */
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	/* SPI1 is used for communication with L9779 */
	engineConfiguration->is_enabled_spi_1 = true;

	engineConfiguration->spi1mosiPin = Gpio::E15;
	engineConfiguration->spi1misoPin = Gpio::E14;
	engineConfiguration->spi1sckPin = Gpio::E13;

  engineConfiguration->triggerInputPins[0] = Gpio::F8;
  engineConfiguration->camInputs[0] = Gpio::B9;

	engineConfiguration->clt.adcChannel = EFI_ADC_39; // ADC3 PF5
	engineConfiguration->iat.adcChannel = EFI_ADC_32; // ADC3 PF6

	/* IAT = the NTC inside the Bosch 0 261 230 217 T-MAP (pin 2). Steinhart-Hart
	 * reference points from the datasheet NTC table:
	 *   -20C = 15458, 40C = 1174, 110C = 144.2 ohm
	 * Board pullup on thermistor inputs is 1500 ohm. */
	engineConfiguration->iat.config.tempC_1 = -20;
	engineConfiguration->iat.config.resistance_1 = 15458;
	engineConfiguration->iat.config.tempC_2 = 40;
	engineConfiguration->iat.config.resistance_2 = 1174;
	engineConfiguration->iat.config.tempC_3 = 110;
	engineConfiguration->iat.config.resistance_3 = 144.2f;
	engineConfiguration->iat.config.bias_resistor = 1500;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_1; // AC3 -> 74HC14 -> RS358A -> PA1

	/* Bosch 0 261 230 217 T-MAP (Lada 21800-1413010):
	 * Vout = (0.85/95 * P[kPa] - 0.1) * Us, datasheet curve 0.4 V @ 20 kPa,
	 * 4.65 V @ 115 kPa at Us = 5 V. Only for fresh configs - the stored tune
	 * keeps its own curve (set it in TS: MAP type Custom, same two points). */
	engineConfiguration->map.sensor.type = MT_CUSTOM;
	engineConfiguration->mapLowValueVoltage = 0.4f;
	engineConfiguration->map.sensor.lowValue = 20;
	engineConfiguration->mapHighValueVoltage = 4.65f;
	engineConfiguration->map.sensor.highValue = 115;

	/* Battery sense: PA6 (EFI_ADC_6). The earlier PA3 assignment was wrong -
	 * it is a regulated line that moves OPPOSITE to the battery (2.302 V at a
	 * 12.4 V battery, 2.184 V at 13.6 V) and stays frozen at ~12.4 in the
	 * console during cranking. Bench tracking test with a charger (12.4 ->
	 * 13.6 -> 14.42 V battery): PA6 1.536 -> 1.699 -> 1.798 V and PA7
	 * 1.577 -> 1.743 -> 1.844 V both follow the battery; PA6 matches the
	 * 33k/4.7k = 8.02 divider cleanly on all three points. PA7 (~68k/10k =
	 * 7.8) is the backup candidate - re-verify on the car: VBatt must dip to
	 * ~9-10 V while cranking, otherwise switch the channel. Must stay in
	 * ConfigOverrides too: the stored tune predates it. */
	engineConfiguration->vbattAdcChannel = EFI_ADC_6; // PA6
	engineConfiguration->vbattDividerCoeff = 8.02f;

	engineConfiguration->analogInputDividerCoefficient = 2.0f;
	engineConfiguration->adcVcc = 3.3f;

	/* Board has a dedicated knock input (connector AA3 -> onboard conditioner
	 * -> MCU ADC, pin/channel in knock_config.h). Enabled for fresh configs
	 * only - the stored tune keeps its own value. */
	engineConfiguration->enableSoftwareKnock = true;

	/* 21129-family engines: 82 mm bore, resonant knock sensors peak at ~7 kHz.
	 * Explicit frequency (Hz) with the single harmonic - the sensors have no
	 * usable output at the second harmonic, so do not use the double-frequency
	 * path. */
	engineConfiguration->knockFrequency = 7000;
	engineConfiguration->knockDetectionUseDoubleFrequency = false;

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

	/* CLT/IAT are wired to ADC3-only pins (PF5/PF6); force the channels on
	 * every boot because the stored tune predates the ADC3 slow sampling. */
	engineConfiguration->clt.adcChannel = EFI_ADC_39; // ADC3 PF5
	engineConfiguration->iat.adcChannel = EFI_ADC_32; // ADC3 PF6

	/* Individual COP coils: the stored tune predates the fix and holds
	 * IM_WASTED_SPARK, which only schedules two of the four L9779 ignition
	 * channels - two cylinders never fire. Force the mode and the 1:1 pin
	 * mapping on every boot (DefaultConfiguration only applies on reset). */
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->ignitionPins[0] = Gpio::L9779_IGN_1;
	engineConfiguration->ignitionPins[1] = Gpio::L9779_IGN_2;
	engineConfiguration->ignitionPins[2] = Gpio::L9779_IGN_3;
	engineConfiguration->ignitionPins[3] = Gpio::L9779_IGN_4;

	/* Battery sense - same reasoning as CLT/IAT: the stored tune predates
	 * the correct wiring (PA6 = EFI_ADC_6, 33k/4.7k divider), so force the
	 * channel on every boot. See the DefaultConfiguration comment for the
	 * calibration data. */
	engineConfiguration->vbattAdcChannel = EFI_ADC_6; // PA6
	engineConfiguration->vbattDividerCoeff = 8.02f;

	/* NA 1.6: fuel-table load axes capped at 100 kPa, 16 bins with 5 kPa
	 * steps across the 30..100 kPa working band (VE and AFR tables share
	 * the same axes, converted from the stock M74 modeled air charge +
	 * mixture maps, see docs/report.md 2026-08-16). Forced on every boot
	 * because the stored tune predates these axes and TS axis editing of
	 * veLoadBins/lambdaLoadBins is easy to get wrong. Remove once the
	 * final tune keeps its own axes. */
	static constexpr uint16_t fuelLoadBins[VE_LOAD_COUNT] =
		{20, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};
	copyArray(config->veLoadBins, fuelLoadBins);
	copyArray(config->lambdaLoadBins, fuelLoadBins);

	/* Cranking fuel is deliberately tuned in 21129.msq (crankingFuelCoef
	 * 2.8..0.85 + crankingCycleBaseFuel 6x8) - the old forced default curve
	 * here was silently reverting the tune on every boot, which is also why
	 * the console kept re-restoring crankingFuelCoef on every tune load. */

	/* IMMO is force-disabled (physical bypass answers the BCM): the ECU-side
	 * challenge-response algorithm is not implemented, so the 0x0713/0x0714
	 * handshake traffic is pure noise. Forced on every boot because the stored
	 * tune predates this bit. Remove once computeImmoResponse() is real. */
	config->m74_9ImmoOff = true;

	//CAN 1 bus overwrites
	engineConfiguration->canRxPin = Gpio::G0;
	engineConfiguration->canTxPin = Gpio::G1;

	/* OpenBLT jump trigger is a board capability (the bootloader lives in the
	 * first 32 KB of this board's flash), not a tune choice: force it on every
	 * boot so a stale/unsynced msq cannot silently disable CAN flashing. */
	engineConfiguration->canOpenBLT = true;

	setupEtb();
}

/**
 * Per-channel analog input divider.
 *
 * The MAP (EFI_ADC_1) and both ETB TPS channels (EFI_ADC_12/13) share one
 * board divider chain (~1.555, looks like 5.6k/10k), NOT the 2:1 the global
 * analogInputDividerCoefficient guess assumes:
 * - MAP: the Bosch 0 261 230 217 T-MAP outputs
 *   Vout = (0.85/95 * P[kPa] - 0.1) * Us (0.4 V @ 20 kPa, 4.65 V @ 115 kPa
 *   at Us = 5 V), i.e. 3.974 V at 100 kPa, while the ADC raw reads 2.555 V
 *   there -> divider 3.974 / 2.555 = 1.555.
 * - TPS: cross-checked via the redundant pair sum. With 1.555 the console
 *   readings 5.067/1.356 V (closed) and 0.520/5.807 V (open) become
 *   3.940+1.054 = 4.99 V and 0.404+4.515 = 4.92 V - a proper complementary
 *   ETB TPS pair summing to the 5 V rail at both ends.
 * With the 2.0 coefficient the MAP/TPS voltages were 1.29x too high - the
 * VE load axis and the TPS %/redundancy checks were all wrong.
 * Pedal channels (EFI_ADC_10/11) go through the same conditioning block
 * (console readings look inflated the same ~1.29x) and share the divider;
 * verify after flashing via the redundant-pair sum: PPS1+PPS2 should read
 * ~5.0 V at rest and at full pedal.
 */
float getAnalogInputDividerCoefficient(adc_channel_e hwChannel) {
	if (hwChannel == EFI_ADC_1 || hwChannel == EFI_ADC_10 || hwChannel == EFI_ADC_11 || hwChannel == EFI_ADC_12 || hwChannel == EFI_ADC_13) {
		return 1.555f;
	}
	return engineConfiguration->analogInputDividerCoefficient;
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
		/* IN1 .. IN7 (parallel inputs for OUT1 .. OUT7). Buzzed out on the
		 * real board: the KiCad netlist/schematic has this mapping REVERSED -
		 * the measured wiring is IN1=PE11, IN2=PE10, IN3=PE9, IN4=PE8. The
		 * old (schematic) order kept every injector channel shut: L9779
		 * outputs are an AND of the SPI command bit and this parallel input,
		 * and with the wrong pins the two terms never agreed. */
		[4] = {.port = GPIOE, .pad = 11}, /* IN1 -> OUT1 (injector, reversed mapping) */
		[5] = {.port = GPIOE, .pad = 10}, /* IN2 -> OUT2 */
		[6] = {.port = GPIOE, .pad = 9},  /* IN3 -> OUT3 */
		[7] = {.port = GPIOE, .pad = 8},  /* IN4 -> OUT4 */
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

/* Bench diagnostic for knock pin identification: watches the slow-loop ADC
 * buffers (EFI_ADC_0..15 from the slow loop + a live burst of the 8 ADC3-only
 * channels) for ~2 s and prints min/max per channel. With the finger (or a
 * wire) on the knock input AA3, the affected channel shows a large min/max
 * spread while the others stay flat. Note: a knock conditioner with a ~7 kHz
 * bandpass may reject 50 Hz hum - then tap the sensor instead (broadband
 * impulse). ADC1 channels are read from the slow-loop buffer (no conversions
 * started here - the ADCD1 DMA has a hang-on-boot quirk on this port). */
extern bool readSlowAdc3All(adcsample_t samples[8]);
extern adcsample_t adcOnchipSlowGetAvgRaw(adc_channel_e hwChannel);

/* Bench diagnostic for knock pin identification: samples each ADC3 candidate
 * channel at the knock rate (286 kHz) in bursts and prints min/max/RMS.
 * Broadband stimulus (tapping the knock sensor, or clicking a wire on AA3)
 * shows up as a large p2p/RMS jump on the knock channel; 50 Hz hum is
 * rejected by the conditioner bandpass and may show nothing. */
extern bool knockBurstSample(uint32_t adcInChannel, adcsample_t* buf, size_t count);

static void m74_9KnockBurst() {
	/* Knock input is PA0 = ADC1 IN0 (verified on the bench). Scan the ADC1
	 * channels for the final check: taps/clicks on AA3 must show up on PA0. */
	static const int CH = 4;
	static const char* names[CH] = {"PA0(IN0)", "PA1(IN1)", "PA2(IN2)", "PA3(IN3)"};
	static const int chans[CH] = {0, 1, 2, 3};
	static NO_CACHE adcsample_t buf[1024];

	/* Clocks and divider register dump - the measured burst rate (see below)
	 * points at a wrong ADC clock divider on this port. */
	{
		efiPrintf("clocks: SYSCLK=%u HCLK=%u PCLK2=%u",
			(unsigned)STM32_SYSCLK, (unsigned)STM32_HCLK, (unsigned)STM32_PCLK2);
		efiPrintf("RCC->CFGR=0x%08x ADC->CCR=0x%08x",
			(unsigned)RCC->CFGR, (unsigned)ADC->CCR);
	}

	/* Measure the real burst duration on the first conversion - the knock
	 * DSP assumes a fixed sample rate (KNOCK_SAMPLE_RATE), verify it. */
	{
		efitick_t t0 = getTimeNowNt();
		if (knockBurstSample(chans[0], buf, 1024)) {
			efitick_t elapsed = getTimeNowNt() - t0;
			efiPrintf("knocktest: 1024-sample burst took %d us -> real rate ~%d Hz",
				(int)US2NT(elapsed), (int)(1024.0f * US_PER_SECOND_F / US2NT(elapsed)));
		}
	}

	/* ADCD1 state distribution over ~1 s - is ADC1 ever free for knock? */
	{
		int counts[6] = {0};
		for (int i = 0; i < 1000; i++) {
			counts[ADCD1.state]++;
			chThdSleepMilliseconds(1);
		}
		efiPrintf("knocktest: ADCD1 states: UNINIT=%d STOP=%d READY=%d ACTIVE=%d COMPLETE=%d ERROR=%d",
			counts[0], counts[1], counts[2], counts[3], counts[4], counts[5]);
	}

	for (int i = 0; i < CH; i++) {
		adcsample_t mn = 4095, mx = 0;
		uint64_t sumSq = 0;
		int ok = 0;
		/* 25 bursts give plenty of samples for min/max/RMS without hogging the
		 * console thread: each burst blocks it for ~7 ms, and the CAN/TS link
		 * dies if it is not serviced for seconds at a time. */
		for (int burst = 0; burst < 25; burst++) {
			if (!knockBurstSample(chans[i], buf, 1024)) {
				continue;
			}
			ok++;
			for (int j = 0; j < 1024; j++) {
				adcsample_t v = buf[j];
				mn = minI(mn, (int)v);
				mx = maxI(mx, (int)v);
				sumSq += (uint64_t)v * v;
			}
			chThdSleepMilliseconds(2);
		}
		efiPrintf("  %s: bursts=%d min=%u max=%u p2p=%u rms=%.1f", names[i], ok,
			(unsigned)mn, (unsigned)mx, (unsigned)(mx - mn),
			ok ? sqrtf((float)sumSq / (float)(ok * 1024)) : 0.0f);
	}
}

/* Sweep the two suspected ADC clock divider fields and measure the real
 * burst rate for each value - identifies the Artery ADC clock divider
 * encoding (the STM32-style CFGR writes collide with it). */
static void m74_9AdcDivTest() {
	static NO_CACHE adcsample_t tmp[256];

	/* Save/restore the real values - the fixed mcuconf setting is DIV4
	 * (adcdiv=1), never the historical hardcoded restores. */
	uint32_t origCfg = RCC->CFGR;
	uint32_t origCcr = ADC->CCR;

	efiPrintf("adcdivtest: sweep CRM CFG bits [18:16]");
	for (uint32_t v = 0; v < 8; v++) {
		uint32_t cfg = RCC->CFGR;
		cfg = (cfg & ~(7u << 16)) | (v << 16);
		RCC->CFGR = cfg;
		chThdSleepMilliseconds(5);
		efitick_t t0 = getTimeNowNt();
		bool ok = knockBurstSample(4, tmp, 256);
		efitick_t elapsed = getTimeNowNt() - t0;
		efiPrintf("  CFG[18:16]=%u: %s %d us -> %d Hz", (unsigned)v,
			ok ? "ok" : "fail", (int)US2NT(elapsed),
			ok ? (int)(256.0f * US_PER_SECOND_F / US2NT(elapsed)) : 0);
	}
	RCC->CFGR = origCfg;
	chThdSleepMilliseconds(5);

	efiPrintf("adcdivtest: sweep ADC->CCR bits [17:16]");
	for (uint32_t v = 0; v < 4; v++) {
		uint32_t ccr = ADC->CCR;
		ccr = (ccr & ~(3u << 16)) | (v << 16);
		ADC->CCR = ccr;
		chThdSleepMilliseconds(5);
		efitick_t t0 = getTimeNowNt();
		bool ok = knockBurstSample(4, tmp, 256);
		efitick_t elapsed = getTimeNowNt() - t0;
		efiPrintf("  ADCPRE[17:16]=%u: %s %d us -> %d Hz", (unsigned)v,
			ok ? "ok" : "fail", (int)US2NT(elapsed),
			ok ? (int)(256.0f * US_PER_SECOND_F / US2NT(elapsed)) : 0);
	}
	ADC->CCR = origCcr;
	chThdSleepMilliseconds(5);
}
#endif /* EFI_PROD_CODE && HAL_USE_ADC */

static void m74_9KnockPinScan() {
	/* convGroupSlow order = IN0..IN15, pin names per adcChannels[] */
	static const int CH1 = 16;
	static const char* names1[CH1] = {
		"PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7",
		"PB0", "PB1", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5"};
	adcsample_t mn1[CH1], mx1[CH1];
	for (int i = 0; i < CH1; i++) {
		mn1[i] = 4095;
		mx1[i] = 0;
	}

	/* convGroupSlowAdc3 order: IN4, IN5, IN6, IN7, IN8, IN9, IN14, IN15 */
	static const int CH3 = 8;
	static const char* names3[CH3] = {"PF6", "PF7", "PF8", "PF9", "PF10", "PF3", "PF4", "PF5"};
	adcsample_t mn3[CH3], mx3[CH3];
	for (int i = 0; i < CH3; i++) {
		mn3[i] = 4095;
		mx3[i] = 0;
	}

	int ok3 = 0;
	for (int iter = 0; iter < 2000; iter++) {
		for (int i = 0; i < CH1; i++) {
			adcsample_t v = adcOnchipSlowGetAvgRaw(static_cast<adc_channel_e>(EFI_ADC_0 + i));
			mn1[i] = minI(mn1[i], (int)v);
			mx1[i] = maxI(mx1[i], (int)v);
		}
		adcsample_t s3[CH3];
		if (readSlowAdc3All(s3)) {
			ok3++;
			for (int i = 0; i < CH3; i++) {
				mn3[i] = minI(mn3[i], (int)s3[i]);
				mx3[i] = maxI(mx3[i], (int)s3[i]);
			}
		}
		chThdSleepMilliseconds(1);
	}

	efiPrintf("knockpin scan: ADC3 %d conversions over ~2s (ADC1 from slow buffer)", ok3);
	for (int i = 0; i < CH1; i++) {
		efiPrintf("  %s: min=%u max=%u spread=%u (%.3f..%.3f V)", names1[i],
			(unsigned)mn1[i], (unsigned)mx1[i], (unsigned)(mx1[i] - mn1[i]),
			mn1[i] * (3.3f / 4095.0f), mx1[i] * (3.3f / 4095.0f));
	}
	if (ok3 == 0) {
		efiPrintf("  ADC3: no conversions (busy)");
	} else {
		for (int i = 0; i < CH3; i++) {
			efiPrintf("  %s: min=%u max=%u spread=%u (%.3f..%.3f V)", names3[i],
				(unsigned)mn3[i], (unsigned)mx3[i], (unsigned)(mx3[i] - mn3[i]),
				mn3[i] * (3.3f / 4095.0f), mx3[i] * (3.3f / 4095.0f));
		}
	}
}

static Gpio OUTPUTS[] = {
	Gpio::L9779_OUT_4, // Injector 1
	Gpio::L9779_OUT_3, // Injector 2
//	Gpio::L9779_OUT_2, // Injector 3
//	Gpio::L9779_OUT_1, // Injector 4
//	Gpio::L9779_OUT_6, // Oxygen sensor 1 heater
//	Gpio::L9779_OUT_5, // EVAP solenoid control
//	Gpio::L9779_OUT_7, // Oxygen sensor 2 heater
//	Gpio::L9779_IGN_1, // Coil 1 (< +2.5v)
//	Gpio::L9779_IGN_2, // Coil 2 (< +2.5v)
//	Gpio::L9779_IGN_3, // Coil 3 (< +2.5v)
//	Gpio::L9779_IGN_4, // Coil 4 (< +2.5v)
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

/* Scheduler-free coil click: drives the L9779 IGN parallel inputs (PF12..15)
 * directly from this thread with exact timing. sparkbench2 routes through the
 * microsecond scheduler - if the two give different click regularity, the
 * scheduler path is the suspect, not the coil hardware. */
static void m74_9CoilClick(int cyl, int count) {
	if (cyl < 1 || cyl > 4 || count < 1 || count > 100) {
		efiPrintf("usage: coilclick <cyl 1..4> <count 1..100>");
		return;
	}

	/* IGN1..4 direct pads (l9779_cfg.direct_gpio[0..3]) */
	static const ioportid_t clickPorts[4] = { GPIOF, GPIOF, GPIOF, GPIOF };
	static const uint8_t clickPads[4] = { 12, 13, 14, 15 };

	efiPrintf("coilclick: cyl %d, %d clicks of 5 ms on / 995 ms off (direct GPIO)", cyl, count);
	for (int i = 0; i < count; i++) {
		palSetPad(clickPorts[cyl - 1], clickPads[cyl - 1]);
		chThdSleepMilliseconds(5);
		palClearPad(clickPorts[cyl - 1], clickPads[cyl - 1]);
		chThdSleepMilliseconds(995);
	}
	efiPrintf("coilclick: done");
}

void setup_custom_board_overrides() {
	custom_board_InitHardware = m74_9_boardInitHardware;
	custom_board_DefaultConfiguration = m74_9_boardDefaultConfiguration;
	custom_board_ConfigOverrides = m74_9_boardConfigOverrides;
	// The first trigger sync point has no tooth count to validate against, and
	// on this engine a stretched tooth pair (gap0 1.7-2.0 vs the real 3.9 gap)
	// false-syncs the decoder mid-crank - firing from it backfires through the
	// intake. Wait one crank revolution so the next gap validates the position.
	custom_board_requireValidatedSync = []() { return true; };
	// Cranking-band sync-by-position skip: the first-combustion acceleration
	// can compress the REAL missing-teeth gap below the ratio window; at the
	// exact expected position the tooth count proves it IS the gap, so accept
	// it while rpm < 2 * crankingRpm (rejecting it there desyncs the decoder
	// right at the catch - C9002, engine dies).
	custom_board_syncByPositionWhileCranking = []() { return true; };
	// Noise-storm sync-gap hardening: dense edge storms inflate the event
	// count so a storm edge can become the 58th event and false-sync the
	// decoder cleanly (count matches, ratio occasionally in window). Extra
	// gates: the tooth before the candidate must be a real tooth (>= 1/4 of
	// the previous revolution's mean tooth time), and the sync-by-position
	// skip only accepts gap ratios >= 0.8 (the gap is physically 3 tooth
	// slots and cannot compress below ~1.0 even at the catch - the observed
	// catch ratio went below 1.2 and a 1.2 floor rejected the real gap with
	// a C9002). See crankingTransition60_2RealCarProfileNoiseStormDoesNotFalseSync.
	custom_board_syncGapHardening = []() { return true; };
	// Cam sync is switched off in the tune (21129.msq: vvtMode1 = Inactive,
	// camInputs1 = NONE) - crank-only until the engine runs. The cam-phase
	// drift cross-check is therefore not opted in (custom_board_vvtDriftLimit
	// stays at the 0 = disabled default).
	// VR input debounce: the trigger logs show noise edge bursts <50 us apart
	// (comparator ringing / starter interference) that inflate the event count
	// and false-sync the decoder mid-crank. The threshold is RPM-adaptive
	// (m74_9TriggerDebounceUs): ~1.2 ms at cranking to thin noise storms,
	// scaling down to the 100 us floor as the tooth rate rises - the software
	// half of the stock ECU's adaptive VR conditioning (we cannot move the
	// fixed comparator threshold on this board).
	custom_board_triggerDebounceUs = m74_9TriggerDebounceUs;
	// Tooth profile learning housekeeping (load/save of the learned wheel
	// profile + auto-save on engine stop).
	custom_board_periodicSlowCallback = m74_9ToothPeriodic;
	#if EFI_PROD_CODE && HAL_USE_ADC
	addConsoleAction("fastadcdiag", m74_9FastAdcDiag);
	addConsoleAction("knockpin", m74_9KnockPinScan);
	addConsoleAction("knocktest", m74_9KnockBurst);
	addConsoleAction("adcdivtest", m74_9AdcDivTest);
#endif
	addConsoleAction("toothdump", m74_9ToothDump);
	addConsoleAction("toothsave", m74_9ToothSave);
	// raw primary-trigger edge stream capture: a digital oscilloscope of the
	// comparator output for noise diagnosis (deltas + histogram, see
	// m74_9_tooth_diag.cpp)
	addConsoleAction("rawtrg", m74_9RawTriggerDump);
	// Scheduler-free coil click: drives the L9779 IGN parallel inputs
	// (PF12..15) directly from this thread with exact timing. sparkbench2
	// routes through the microsecond scheduler - if the two give different
	// click regularity, the scheduler path is the suspect, not the coils.
	addConsoleActionII("coilclick", m74_9CoilClick);
#if EFI_CAN_SUPPORT
	initM74_9Can();
	custom_board_isImmobilizerBlocking = m74_9_isImmobilizerBlocking;
#endif // EFI_CAN_SUPPORT
}

