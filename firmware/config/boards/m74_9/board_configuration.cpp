
#include "pch.h"
#include "defaults.h"
#include "board_overrides.h"
#include "smart_gpio.h"
#include "drivers/gpio/l9779.h"
#include "drivers/gpio/tle9201.h"
#include "m74_9_can.h"
#include "m74_9_tooth_diag.h"
#include "m74_9_vr_model.h"
#include "m74_9_lin.h"
#include "runtime_state.h"
#include "digital_input_exti.h"
#include "pwm_generator_logic.h"
#include "ignition_controller.h"
#include "thermistors.h"
#include "angle_clock.h"

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
	// The board has exactly ONE TLE9201 H-bridge (see getBoardMetaDcOutputsCount).
	// Force ETB#2 to None regardless of what the stored tune carries: a leftover
	// "Throttle 2" (with all pins NONE) makes doInitElectronicThrottle create a
	// phantom soft-PWM channel on pin 0 that floods the microsecond executor
	// (~100k+ events, visible as the lockstats "soft 800Hz" channel) while
	// writing to no physical output.
	engineConfiguration->etbFunctions[1] = DC_None;

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

	// LIN smart-alternator gates: off while cranking / below 600 rpm, off
	// above 5000 rpm, off during MAP > 80 kPa acceleration for at most 60 s,
	// smooth load pickup via LRC-Rise (5 s), LRC-Cut at 4790 rpm (code 10).
	engineConfiguration->m74_9LinAltMinRpm = 600;
	engineConfiguration->m74_9LinAltMaxRpm = 5000;
	engineConfiguration->m74_9LinAltMapOffKpa = 80;
	engineConfiguration->m74_9LinAltMapOffMaxSeconds = 60;
	// After any OFF event clears (MAP gate, low rpm/cranking, over-rev) keep
	// the generator off for this many more seconds - prevents on/off churn
	// during gear shifts when MAP dips below the threshold for a moment.
	engineConfiguration->m74_9LinOffHoldSeconds = 5;
	engineConfiguration->m74_9LinLrcRiseCode = 5;
	engineConfiguration->m74_9LinLrcCutCode = 10;
	engineConfiguration->m74_9LinAltFeedbackSel = 2; // R = 0b010: BV8 B+ voltage feedback

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

	// Raw EXTI9_5 capture (crank PF8/line 8) MUST be highest-priority IRQ.
	// The handoff ISR (I2C1_EV) decodes the previous edge; if EXTI9_5 were
	// delayed by the handoff (44 us avg) the timestamp would be wrong by up
	// to ~50 us, enough to push the gap ratio outside [1.6, 3.75] at 1887 rpm.
	nvicEnableVector(EXTI9_5_IRQn, 0);   // Raw edge capture: priority 0 (fast IRQ)
	nvicEnableVector(I2C1_EV_IRQn, 4);   // Handoff ISR: priority 4 (below executor)

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

	/* Narrowband lambda input AK3 -> PF3 (EFI_ADC_37) is a direct 1:1 input,
	 * NO divider. Proven from the 2026-08-22 logs with a known-very-rich
	 * mixture: a rich narrowband saturates at ~0.85-0.9 V and PF3 raw read
	 * exactly 0.77-0.87 V (MLGs 13_02_39 min 0.843 V, 14_46_52 min 0.770 V).
	 * A 2:1 divider would read ~0.42-0.45 V, the board's 1.555 divider
	 * ~0.55 V - neither matches. With the global 2.0 coefficient the hot
	 * sensor shows 1.7 V - above the AFR curve top (0.9 V), so the gauge
	 * extrapolates into meaningless AFRs. Cold/disconnected sensor floats at
	 * 3.23 V raw (6.46 V "input") - the board pull-up for open-circuit
	 * detection, not a mixture value. */
	if (hwChannel == EFI_ADC_37) {
		return 1.0f;
	}
	return engineConfiguration->analogInputDividerCoefficient;
}

/* CLT/IAT pull-ups sit on the L9779 VTRK1/2 tracking sensor rail:
 * VTRK = VBATT/2.5 BY DESIGN (battery-proportional - the "5V" name only
 * holds at a 12.5 V battery). The resistance math must therefore use the
 * measured battery voltage as the bias, not a fixed 5.0 V. Proven on the
 * car: at VBATT 14.4 V (VTRK 5.76 V) the fixed-5.0 assumption read +5C as
 * -30C and 90C as ~86C; the 0.4 ratio makes all observations exact
 * (08-24/08-25 MLG fits). Ratio to confirm against the L9779 datasheet
 * VTRK section.
 *
 * Software-only fix, no board changes: the on-board 2:1 divider keeps the
 * junction inside the 3.3 V ADC range across the full -40..+130C sensor
 * range (at -40C the junction reaches ~VTRK = 5.76 V -> pin 2.88 V < 3.3),
 * so the whole curve is measurable once the bias tracks the battery. */
float getThermistorBiasTrackingRatio(const char* msg) {
	if (strcmp(msg, "clt") == 0 || strcmp(msg, "iat") == 0) {
		return 0.4f;
	}

	return 0;
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
	 * low=enable would leave DIS high = outputs tristate). Boot default is
	 * LOW (bridge disabled): the ignition gate in m74_9IgnitionGatePeriodic()
	 * raises PB13 only after the L9779 KEY_ON read confirms the key is on,
	 * and drops it on key-off - with the key off the blade must NOT stay
	 * energized for hours (the 2026-08-26 report: only unplugging the ECU
	 * reset it). The weak pullup alone cannot turn Q5A on (internal 10k
	 * base-emitter divider), so the disabled state is safe by construction. */
	gpio_pin_markUsed(GPIOB, 13, "ETC_EN");
	palSetPadMode(GPIOB, 13, PAL_MODE_OUTPUT_PUSHPULL);
	palClearPad(GPIOB, 13);

	/* Park the L9779 power stages until the ignition gate confirms the key
	 * (this flag is picked up by the driver thread, which starts later via
	 * gpiochips_init - with it false the boot chip_init and the WDA feed are
	 * skipped and the chip stays in its power-on-off state). */
	l9779_setPowerStage(false);

#if HAL_USE_WDG
	{
		/* The bootloader starts the IWDG (500 ms) to recover its own wedges;
		 * the IWDG keeps running across the jump and cannot be stopped, so the
		 * app must re-configure it right here and feed it from then on. 4 s
		 * nominal (~3.3 s real at the AT32 40 kHz LSI) covers the longest known
		 * stall (MFS GC ~2.3 s) with margin; the 20 Hz slow callback (SysTick
		 * virtual timer) feeds it every 50 ms. */
		static WDGConfig wdgcfg;
		wdgcfg.pr = STM32_IWDG_PR_64;
		wdgcfg.rlr = STM32_IWDG_RL((uint32_t)((32.768f / 64.0f) * 4000));
		wdgStart(&WDGD1, &wdgcfg);
	}
#endif // HAL_USE_WDG

	int ret = tle9201_add(0, &tle9201_cfg);
	efiPrintf("tle9201_add()=%d", ret);

	// LIN alternator master (USART3 + console diagnostics). Must run here:
	// sdStart() uses OS primitives, so starting it from
	// setup_custom_board_overrides() (pre-halInit, before chSysInit) hangs
	// the board at power-on - the same trap as tle9201_add() above.
	initM74_9LinAlternator();
}

/* -----------------------------------------------------------------------
 * Ignition-gated power stage
 *
 * With the key off: the L9779 power stages are PSOFF'd, the TLE9201 bridge
 * is disabled (PB13 low) and the dash CAN stream stops (m74_9_can.cpp -
 * the stock ECU is completely CAN-silent with ignition off, and the BCM
 * drops the main relay when the 0x0189 heartbeat dies). On the key-on edge
 * the L9779 is fully re-initialized (SW_RST + START + REG6 + RESPTIME +
 * VRS + outputs) through its driver thread.
 *
 * Runs from custom_board_periodicSlowCallback = the 20 Hz slow callback,
 * which executes inside the SysTick ISR (PeriodicTimerController virtual
 * timer): only flags and a plain GPIO are touched here, ALL L9779 SPI work
 * happens in the l9779 driver thread. */

static bool m74_9_ignitionOn = false;

/* TLE9201 bridge enable: PB13 -> Q5A (NPN, inverts) -> DIS (pulled up to
 * +5V). PB13 high = Q5A on = DIS low = bridge enabled. */
static void m74_9_setEtcEnable(bool on) {
	palWritePad(GPIOB, 13, on ? 1 : 0);
}

static void m74_9IgnitionGatePeriodic() {
	bool ign = isIgnVoltage();

	if (ign == m74_9_ignitionOn) {
		return;
	}

	m74_9_ignitionOn = ign;
	efiPrintf("m74_9 ignition gate: %s",
		ign ? "ON (L9779 re-init + ETB enable)" : "OFF (PSOFF + ETB disable)");

	/* ETB bridge first (plain GPIO, independent of the L9779 SPI), then the
	 * L9779 power stages. */
	m74_9_setEtcEnable(ign);
	l9779_setPowerStage(ign);
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

// The NZW-flash continue-read path is sensitive to code alignment: the same
// loop measured 88 vs 100 ms across linker layouts. Pin the probe to a
// 256-byte boundary so the diagnostic number is comparable across builds.
static __attribute__((aligned(256), noinline)) uint32_t flashperfLoop() {
	volatile uint32_t acc = 0;
	for (uint32_t i = 0; i < 1000000; i++) {
		acc += i * 2654435761u;
	}
	return acc;
}

// Printable name of an executor command class. A helper instead of an
// in-lambda array because commas outside parentheses split macro arguments
// (addConsoleAction is a function-like macro).
static const char* m74_9ExecKindName(int kind) {
	switch (kind) {
	case (int)ExecEventKind::Dwell:
		return "dwell";
	case (int)ExecEventKind::Spark:
		return "spark";
	case (int)ExecEventKind::Overdwell:
		return "overdwell";
	case (int)ExecEventKind::Fuel:
		return "fuel";
	default:
		return "other";
	}
}

void setup_custom_board_overrides() {
	custom_board_InitHardware = m74_9_boardInitHardware;
	custom_board_DefaultConfiguration = m74_9_boardDefaultConfiguration;
	custom_board_ConfigOverrides = m74_9_boardConfigOverrides;
	// The first trigger sync point has no tooth count to validate against, and
	// on this engine a stretched tooth pair (gap0 1.7-2.0 vs the real 3.9 gap)
	// false-syncs the decoder mid-crank - firing from it backfires through the
	// intake. Wait one crank revolution so the next gap validates the position.
	// DISABLED 2026-08-22: stock-rollback test - the L9779 ramp config now
	// gives a clean signal and the flash-timing fix removed the C9002 class.
	// Re-enable if mid-crank false sync (intake backfire) returns.
	custom_board_requireValidatedSync = []() { return false; };
	// Cranking-band sync-by-position skip: the first-combustion acceleration
	// can compress the REAL missing-teeth gap below the ratio window; at the
	// exact expected position the tooth count proves it IS the gap, so accept
	// it while rpm < 2 * crankingRpm (rejecting it there desyncs the decoder
	// right at the catch - C9002, engine dies).
	// DISABLED 2026-08-21: the C9002 root cause was the slow flash (no DIVR /
	// NZW setup), fixed in the ChibiOS fork. This path double-counts sync
	// validations per revolution and races the cranking-taper counter ~1.5-2x.
	// Re-enable only if a catch C9002 returns.
	custom_board_syncByPositionWhileCranking = []() { return false; };
	// Noise-storm sync-gap hardening: dense edge storms inflate the event
	// count so a storm edge can become the 58th event and false-sync the
	// decoder cleanly (count matches, ratio occasionally in window). Extra
	// gates: the tooth before the candidate must be a real tooth (>= 1/4 of
	// the previous revolution's mean tooth time), and the sync-by-position
	// skip only accepts gap ratios >= 0.8 (the gap is physically 3 tooth
	// slots and cannot compress below ~1.0 even at the catch - the observed
	// catch ratio went below 1.2 and a 1.2 floor rejected the real gap with
	// See crankingTransition60_2RealCarProfileNoiseStormDoesNotFalseSync.
	// DISABLED 2026-08-22: stock-rollback test (clean L9779 signal).
	// Re-enable if a noise storm false-syncs mid-crank (C9003 with a clean
	// count, backfires).
	custom_board_syncGapHardening = []() { return false; };
	// Cam sync is switched off in the tune (21129.msq: vvtMode1 = Inactive,
	// camInputs1 = NONE) - crank-only until the engine runs. The cam-phase
	// drift cross-check is therefore not opted in (custom_board_vvtDriftLimit
	// stays at the 0 = disabled default).
	// Early-gap acceptance: a ratio-validated sync candidate that arrives 1-2
	// events before the expected count means the L9779 VR conditioner lost
	// 1-2 teeth between the gaps (it does this at the first-combustion catch
	// and below ~70 rpm; noise only INSERTS events, so a deficit cannot be
	// noise). Accept it while cranking instead of C9003-desyncing - the
	// observed failure mode was C9003 'got 56/0' killing the engine right at
	// the catch.
	// DISABLED 2026-08-21: with the flash timing fixed the early gap no
	// longer appears; this path was the second source of the cranking-taper
	// counter race (extra sync validation per revolution). Re-enable only if
	// a catch C9003 'got 56/0' returns.
	custom_board_syncEarlyGapWhileCranking = []() { return false; };
	// Running-band tooth-loss tolerance: the L9779 VR conditioner (full
	// adaptive mode) intermittently EATS one output edge per revolution at
	// 1763-4005 rpm (datasheet 6.14: the auto-adaptive filter suppresses the
	// output edge when the squared signal high level falls below Tfilter,
	// driven by the auto-hysteresis re-quantizing near a peak-detector
	// boundary). The 2026-08-24 20:43 drive: 19 C9003 clusters, 837/837
	// 'newerr' lines with the gap ratio windows PASSING and the count short
	// by exactly one (57/58) - a single lost decode edge, not noise. Accept
	// the deficit at the ratio-validated real gap as a valid sync: the phase
	// shifts 6-12 deg only for the already-elapsed part of the revolution
	// and re-anchors at the gap. The alternative (C9003 desync) cuts
	// fuel/spark, flaps the rpm sensor, and opens the storage gate into an
	// MFS-write storm - strictly worse. Deficit-only bound: noise inserts
	// edges, it never deletes them, so a deficit cannot be noise; 3+ lost
	// teeth keep the strict path. Active from cranking to 7000 rpm - above
	// that the 148 us tooth period approaches the conditioner's ~100 us
	// minimum output period and a whole different regime starts.
	custom_board_syncAcceptToothLoss = []() {
		if (engine->triggerCentral.directSelfStimulation) {
			// bench self-stim generates a clean tooth stream; keep the strict
			// path so the simulator stays a meaningful decoder test bench
			return false;
		}

		float rpm = Sensor::getOrZero(SensorType::Rpm);
		return rpm >= engineConfiguration->cranking.rpm && rpm < 7000;
	};
	// Sync-anchor correction: the L9779's gap output is systematically
	// compressed - a spurious missing-region edge fires ~0.63 pitch early
	// (the auto-hysteresis re-quantizes down during the gap, the latch then
	// suppresses the real first-tooth edge; measured gap0 2.36-2.5 vs the
	// physical 3.0, constant in angle across rpm). The decoder would anchor
	// all scheduling ~3.8 deg advanced; the correction measures the deficit
	// per validated sync and retards the scheduling basis by it. Same rpm
	// band as the tooth-loss tolerance: below cranking the gap can compress
	// from acceleration and the correction would misfire (it only corrects
	// the compression direction anyway, clamped to [0, 1.5] pitch).
	custom_board_syncGapAnchorCorrection = []() {
		if (engine->triggerCentral.directSelfStimulation) {
			// clean synthetic gap: nothing to correct
			return false;
		}

		float rpm = Sensor::getOrZero(SensorType::Rpm);
		return rpm >= engineConfiguration->cranking.rpm && rpm < 7000;
	};
	// Model fallback -> now the DEFAULT for the gap-anchor correction: the
	// per-level shift table IS the correction state; the decoder trains it
	// from the per-sync measurement (triggerObserveGapShift) and applies
	// table[level] as the scheduling anchor correction.
	custom_board_vrGapShiftPitch = []() {
		return vrExpectedShiftPitchForRpm(Sensor::getOrZero(SensorType::Rpm));
	};
	// Storage writes must be deferred while the engine runs: the MFS lives on
	// the AT32 internal flash (bank 2) and a sector erase stalls the whole CPU
	// (no read-while-erase on this silicon). The periodic LTFT save hitting an
	// MFS garbage collection froze the firmware for ~2.4 s mid-run and wedged
	// the NT clock (14:39 'gap in time' flood, identical now= forever, only a
	// power cycle recovers - throttle stayed driven). Deferred writes are
	// flushed by the storage manager once the engine stops; self-stimulation
	// (bench) stays allowed.
	// Storage writes must be deferred while the engine runs: the MFS lives on
	// the AT32 internal flash (bank 2) and a sector erase stalls the whole CPU
	// (no read-while-erase on this silicon). The periodic LTFT save hitting an
	// MFS garbage collection froze the firmware for ~2.4 s mid-run and wedged
	// the NT clock (14:39 'gap in time' flood, identical now= forever, only a
	// power cycle recovers - throttle stayed driven). Deferred writes are
	// flushed by the storage manager once the engine stops; self-stimulation
	// (bench) stays allowed.
	//
	// STOPPED DEBOUNCED (2026-08-24): a trigger storm flaps the rpm sensor
	// 0/300+ at ~1 kHz, and a single isStopped() poll admitted the deferred
	// LTFT write right INTO the storm - the 17-38 ms stall then lost teeth
	// and deepened it (the 20:43-21:05 drive: the ID-3 writes landed
	// millisecond-exact on the C9003 clusters, e.g. 20:45:49.267). Require
	// N consecutive stopped polls (~1 s at the storage manager's 100 ms
	// cadence) before the flash is allowed.
	custom_board_allowFlashNow = []() {
		if (engine->triggerCentral.directSelfStimulation) {
			return true;
		}

		static int stoppedPolls = 0;
		if (engine->rpmCalculator.isStopped()) {
			stoppedPolls++;
		} else {
			stoppedPolls = 0;
		}

		return stoppedPolls >= 10;
	};
	// TS burns are rejected while the engine runs, same rationale as the
	// storage-deferral gate above (the extra-page burns go straight to the
	// flash from the TS thread).
	// NOTE: do NOT copy the stoppedPolls debounce from custom_board_allowFlashNow
	// here. That gate is polled every 100 ms by the storage manager while a
	// deferred write is pending, so the counter can accumulate; a TS burn is a
	// ONE-SHOT request (finishPendingBurn runs exactly once per burn), so a
	// '10 consecutive polls' requirement can never be met and every burn gets
	// refused even with the engine stopped (the 2026-08-26 bricked-tune report:
	// 'TS burn skipped - engine is running' with the engine off). A single
	// isStopped() check is the only workable policy here; the trigger-storm
	// flap case that motivated the flash-gate debounce is a periodic-write
	// concern and does not apply to an explicit user-initiated burn.
	custom_board_allowTsBurn = []() {
		if (engine->triggerCentral.directSelfStimulation) {
			return true;
		}

		return engine->rpmCalculator.isStopped();
	};
	// VR input debounce: the trigger logs show noise edge bursts <50 us apart
	// (comparator ringing / starter interference) that inflate the event count
	// and false-sync the decoder mid-crank. The threshold is RPM-adaptive
	// (m74_9TriggerDebounceUs): ~1.2 ms at cranking to thin noise storms,
	// scaling down to the 100 us floor as the tooth rate rises - the software
	// half of the stock ECU's adaptive VR conditioning (we cannot move the
	// fixed comparator threshold on this board).
	// DISABLED 2026-08-22: stock-rollback test - no software debounce (the
	// default 0.0f in trigger_central.cpp skips the window entirely).
	// Re-enable if edge-burst noise storms return (event count inflation /
	// mid-crank false syncs).
	// custom_board_triggerDebounceUs = m74_9TriggerDebounceUs;
	// Tooth profile learning: the load/save housekeeping is switched off
	// (stock-rollback test), and since 2026-08-23 the decoder CONSUMPTION is
	// off too - triggerGetToothProfileFactor() in m74_9_tooth_diag.cpp returns
	// 1.0 unconditionally. The earlier "without the periodic load the factors
	// stay 1.0" comment was WRONG: the learner runs on a trigger hook and the
	// decoder's useProfile gate (rpm >= 2*cranking) consumed the RAM profile
	// anyway, warping the sync ratio at stable high rpm (periodic C9002 +
	// coil-recharge jerks at constant rpm). The synctrace/rawtrg/toothdump
	// diagnostics are fed by separate trigger hooks and keep working.
	// custom_board_periodicSlowCallback = m74_9ToothPeriodic;
	// VR amplitude model: boot read + save-on-stop through the debounced
	// flash gate (see m74_9_vr_model.cpp). The LIN alternator master runs
	// in its own thread (see m74_9_lin.cpp) - NOT here: the slow callback
	// executes inside the SysTick ISR (PeriodicTimerController virtual
	// timer), where the LIN blocking serial I/O is illegal (SV#10 crash).
	custom_board_periodicSlowCallback = []() {
#if HAL_USE_WDG
		/* Feed the hardware watchdog started by the bootloader (and re-configured
		 * in m74_9_boardInitHardware). SysTick keeps firing through thread stalls
		 * (e.g. the ~2.3 s MFS GC), so this never causes a spurious reset while
		 * the core runs; only a real core hang trips the reset. */
		wdgResetI(&WDGD1);
#endif // HAL_USE_WDG
		m74_9VrModelPeriodic();
		m74_9IgnitionGatePeriodic();
	};
	#if EFI_PROD_CODE && HAL_USE_ADC
	addConsoleAction("fastadcdiag", m74_9FastAdcDiag);
	addConsoleAction("knockpin", m74_9KnockPinScan);
	addConsoleAction("knocktest", m74_9KnockBurst);
	addConsoleAction("adcdivtest", m74_9AdcDivTest);
#endif
	addConsoleAction("toothdump", m74_9ToothDump);
	addConsoleAction("toothsave", m74_9ToothSave);
	addConsoleAction("synctrace", m74_9SyncTrace);
	// one-shot multi-clock dump: run twice with a known wall-clock gap to
	// verify which timebase (if any) diverges
	addConsoleAction("timecheck", m74_9TimeCheck);
	// interrupt-lock / trigger-path statistics: prints the longest single
	// IRQ-masked window (maxLockedDuration), the longest trigger event
	// callback and the trigger ISR reentrancy, then resets the counters.
	// Run before/after a crank to see who stalls the system tick.
	addConsoleAction("lockstats", [](){
		printRuntimeStats();
		resetMaxValues();

		// Event execution lateness: proves whether scheduled commands
		// (spark, injection, dwell) go out at their scheduled moment. A
		// large maxLateUs or a fat >=16us bucket means the TIM5 dispatch
		// was delayed - the command timing floats and the engine misfires.
		// A clean histogram (<4us) means the executor is deterministic and
		// any misfire jitter comes from upstream (trigger edge timing).
		auto& sched = engine->scheduler;
		efiPrintf("sched exec=%u late>=10us=%u maxLateUs=%u hist<1/1-4/4-16/16-64/64-256/256-1k/>=1k us: %u %u %u %u %u %u %u",
			(unsigned)sched.executedEventCount, (unsigned)sched.lateEventCount,
			(unsigned)(sched.maxLateNt / (NT_PER_SECOND / 1000000)),
			(unsigned)sched.lateHistogram[0], (unsigned)sched.lateHistogram[1], (unsigned)sched.lateHistogram[2],
			(unsigned)sched.lateHistogram[3], (unsigned)sched.lateHistogram[4], (unsigned)sched.lateHistogram[5],
			(unsigned)sched.lateHistogram[6]);

		// Per-command-class breakdown: WHICH commands float. The engine
		// timing commands (dwell/spark/overdwell/fuel) must show
		// late>=10us=0 and small maxLateUs - the executor now runs above
		// the trigger decode and the ADC, so it dispatches with a fixed
		// ~1-2 us entry latency. 'other' carries the periodic/ADC/TS
		// events where a few us of jitter does not matter.
		// NOTE: commas outside parentheses split macro arguments, so the
		// kind name comes from a helper, not an in-lambda array.
		for (int i = 0; i < (int)ExecEventKind::Count; i++) {
			const auto& k = sched.kindStats[i];
			efiPrintf("sched %s: n=%u late>=10us=%u maxLateUs=%u cbmaxUs=%u",
				m74_9ExecKindName(i), (unsigned)k.executedEventCount, (unsigned)k.lateEventCount,
				(unsigned)(k.maxLateNt / (NT_PER_SECOND / 1000000)),
				(unsigned)(sched.maxCbDurationNt[i] / (NT_PER_SECOND / 1000000)));
		}

		// Ground truth for the interrupt-priority swap: read the NVIC
		// priority registers directly. IRQ numbers per the Artery CMSIS:
		// EXTI9_5=23 (raw trigger capture, want 0), I2C1_EV=31 (trigger
		// handoff, want 4), TIM5=50 (executor, want 3), CAN1_TX=19/
		// CAN1_RX0=20 (want 11), ADC1_2=18 (want 6).
		efiPrintf("nvic: exti=%u handoff=%u tim5=%u canTx=%u canRx0=%u adc=%u (want 0/4/3/11/11/6)",
			(unsigned)((NVIC->IP[23] >> 4) & 0xF), (unsigned)((NVIC->IP[31] >> 4) & 0xF),
			(unsigned)((NVIC->IP[50] >> 4) & 0xF), (unsigned)((NVIC->IP[19] >> 4) & 0xF),
			(unsigned)((NVIC->IP[20] >> 4) & 0xF), (unsigned)((NVIC->IP[18] >> 4) & 0xF));

		// What the unclassified 'other' events actually are: raw callback
		// addresses, mapped to function names with arm-none-eabi-nm on the
		// matching build.
		for (int i = 0; i < 8; i++) {
			const auto& s = sched.otherCbStats[i];
			if (s.cbAddr == 0) {
				break;
			}
			efiPrintf("sched othercb %08x: n=%u late>=10us=%u maxLateUs=%u maxDurUs=%u",
					(unsigned)s.cbAddr, (unsigned)s.count, (unsigned)s.lateCount,
					(unsigned)(s.maxLateNt / (NT_PER_SECOND / 1000000)),
					(unsigned)(s.maxDurationNt / (NT_PER_SECOND / 1000000)));
		}

		// Which soft-PWM channels are loading the executor: they all share the
		// static timerCallback address above, so name them individually here.
		printPwmStats();

#if EFI_ANGLE_CLOCK
		// Hardware angle clock (TMR2): attempts = arm calls; refuse = target
		// beyond the 30 deg lookahead (stale phase basis); noCh = all 4
		// channels busy; lateArm = armed past the tick; dropped = stale
		// charge/injection starts cancelled. The last-refusal snapshot (target
		// angle / stored phase / basis) and maxBusyUs (max |ccr - CNT| of a
		// busy channel, in us) settle which failure dominates and how far out
		// the stuck ticks are. init rate = the TMR2 rate measurement from
		// boot. nvic 28 = TIM2 (want 3).
		efiPrintf("angclk att=%u fire=%u refuse=%u noCh=%u lateArm=%u drop=%u maxLateUs=%u busyUs=%u",
			(unsigned)angleClockArmAttempts(), (unsigned)angleClockFiredCount(),
			(unsigned)angleClockRefuseCount(), (unsigned)angleClockNoChannelCount(),
			(unsigned)angleClockProgrammedLateCount(), (unsigned)angleClockDroppedCount(),
			(unsigned)(angleClockMaxLateTicks() / (NT_PER_SECOND / 1000000)),
			(unsigned)(angleClockMaxBusyDeltaTicks() / (NT_PER_SECOND / 1000000)));
		efiPrintf("angclk lastRefuse target=%.1f phase=%.1f callerPhase=%.1f callerNext=%.1f rem=%.1f cb=%08x basis=%.1f initRate=%u/%u psc=%u nvic=%u (want 3)",
			(double)angleClockLastRefuseTarget(), (double)angleClockLastRefusePhase(),
			(double)angleClockLastRefuseCallerPhase(), (double)angleClockLastRefuseCallerNext(),
			(double)angleClockLastRefuseRemaining(), (unsigned)angleClockLastRefuseCallback(),
			(double)angleClockLastRefuseBasis(),
			(unsigned)angleClockInitAcDelta(), (unsigned)angleClockInitNtDelta(),
			(unsigned)angleClockInitPsc(), (unsigned)((NVIC->IP[28] >> 4) & 0xF));
		angleClockResetStats();
#endif // EFI_ANGLE_CLOCK

		sched.resetExecutionLatenessStats();
	});
	// Systemic CPU speed probe: reads the AT32 flash performance/divider/
	// continue-read registers and times a fixed 1M-iteration loop. At 288 MHz
	// with correct flash timing (DIVR=/3 -> 96 MHz flash clock + CONTR
	// continue-read, see stm32_clock_init in the ChibiOS AT32 port) the loop
	// takes ~20-35 ms; hundreds of ms means every instruction fetch pays
	// maximum wait states.
	// The NZW-flash continue-read path is sensitive to code alignment: the
	// same loop measured 88 vs 100 ms across linker layouts. Pin the probe to
	// a 256-byte boundary so the diagnostic number is comparable across builds.
	addConsoleAction("flashperf", [](){
		uint32_t psr = FLASH1->PSR;
		uint32_t divr = FLASH1->DIVR;
		uint32_t contr = FLASH1->CONTR;
		uint16_t eopb0 = USD->eopb0;
		// EOPB0: 0x00=SRAM512K/ZW128K 0x01=448/192 0x02=384/256 0x03=320/320
		//        0x04=256/384 0x05=192/448 0x06=128/512; 0xFF = factory default
		const char* zw = (eopb0 == 0x00 || eopb0 == 0xFF) ? "128K" :
		                 (eopb0 == 0x01) ? "192K" : (eopb0 == 0x02) ? "256K" :
		                 (eopb0 == 0x03) ? "320K" : (eopb0 == 0x04) ? "384K" :
		                 (eopb0 == 0x05) ? "448K" : (eopb0 == 0x06) ? "512K" : "?";
		uint32_t t0 = getTimeNowLowerNt();
		uint32_t acc = flashperfLoop();
		uint32_t t1 = getTimeNowLowerNt();
		efiPrintf("flashperf: PSR=0x%08x (NZW_BST=%lu) DIVR=0x%08x (FDIV=%lu) CONTR=0x%08x (contRead=%lu) EOPB0=0x%02x (ZW=%s) loop@0x%08x loop1000k=%lu ticks (%.2f ms) acc=%lu",
			(unsigned)psr, (unsigned long)((psr & FLASH_PSR_NZW_BST_Msk) >> FLASH_PSR_NZW_BST_Pos),
			(unsigned)divr, (unsigned long)(divr & FLASH_DIVR_FDIV_Msk),
			(unsigned)contr, (unsigned long)((contr & FLASH_CONTR_FCONTR_EN_Msk) >> FLASH_CONTR_FCONTR_EN_Pos),
			(unsigned)eopb0, zw,
			(unsigned)(uintptr_t)&flashperfLoop,
			(unsigned long)(t1 - t0), (t1 - t0) / 4000.0f, (unsigned long)acc);
	});
	// NZW_BST is read-only here on purpose: toggling PSR.NZW_BST at full
	// HCLK hangs the flash read path (hard lockup, no fault, recoverable only
	// by power cycle) - it is now set at boot in stm32_clock_init.
	addConsoleActionS("flashnzw", [](const char*){
		efiPrintf("flashnzw: NZW_BST is %s - live toggle is DISABLED (hangs the flash read path at 288 MHz)."
			" It is set at boot in stm32_clock_init (ChibiOS AT32 port).",
			(FLASH1->PSR & FLASH_PSR_NZW_BST_Msk) ? "ON" : "OFF");
	});
	// raw primary-trigger edge stream capture: a digital oscilloscope of the
	// comparator output for noise diagnosis (deltas + histogram, see
	// m74_9_tooth_diag.cpp)
	addConsoleAction("rawtrg", m74_9RawTriggerDump);
	// VR amplitude model (L9779 auto-hysteresis): 'vrk <mV/rpm>' sets the
	// calibration scalar (0 = off), 'vrmodel' prints the level/shift/risk.
	addConsoleAction("vrmodel", m74_9VrModel);
	addConsoleActionS("vrk", [](const char* arg) {
		m74_9VrModelSetK(arg);
	});
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

