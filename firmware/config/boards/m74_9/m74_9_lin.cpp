/**
 * @file    m74_9_lin.cpp
 *
 * LIN master + smart-alternator control for the m74_9 board.
 *
 * Bus facts (reverse-engineered from the stock I865LB52_w2404b1 firmware and
 * cross-checked against the VDA LIN-Generator-Regulator specification, see the
 * ST L9918 datasheet):
 *   - 19200 baud, single wire, master = ECU
 *   - control frame: ID 0x16 -> PID 0xD6, 6 data bytes, ENHANCED checksum
 *   - status frame:  ID 0x08 -> PID 0xC8, 8 data bytes, CLASSIC checksum
 *     (the regulator chip speaks LIN 1.x classic)
 *   - setpoint encoding: code = (V - 10.6) x 10 (6-bit, 0.1 V steps, 10.6-16 V),
 *     code 0 = 10.6 V = OFF / pre-excitation
 *
 * Control policy (user request 2026-08-25):
 *   - generator OFF while cranking and below minRpm (600)
 *   - smooth load pickup after start via the LRC-Rise field
 *   - generator OFF above maxRpm (5000), LRC-Cut field mirrors the cut
 *   - generator OFF during hard acceleration (MAP > 80 kPa), but never
 *     longer than 60 s continuous (battery protection)
 *   - after any OFF event clears (MAP gate, low rpm/cranking, over-rev) the
 *     generator stays off for a further hold time (5 s) - a gear-shift MAP
 *     dip does not toggle the generator on for a moment just to switch it
 *     off again
 *   - the target voltage comes from the shared alternatorVoltageTargetTable
 *     (load x rpm), filled in the tune
 *
 * The 6-byte payload layout is PROVISIONAL (the VDA field order): byte0 =
 * setpoint code6, byte1 = LRC-rise | LRC-cut, byte2 = excitation current
 * limit (0 = none), byte3 = feedback selection R / BZ / F / WB, bytes 4-5 =
 * 0xFF padding.  Confirm with the linalt debug output and one on-car capture,
 * then adjust the builder below.
 *
 * @date 2026-08-25
 */

#include "pch.h"

#if EFI_PROD_CODE

#include "m74_9_lin.h"
#include "io_pins.h"
#include "engine_math.h"

// ---------------------------------------------------------------------------
// Physical layer
// ---------------------------------------------------------------------------

// LIN UART: USART3 on PD8 (TX) / PD9 (RX), MUX 7 - the classic USART3 pair.
// Buzzed out on the physical board (2026-08-26): the LIN physical layer is
// the L9779's ISO9141 K-line transceiver, NOT a direct UART-to-wire
// connection:
//   PD8 (USART3_TX) -> L9779 K_TX (pin 47)
//   L9779 K_LINE (pin 45) -> alternator wire (AF3 "ALT DFM" / ALT_LT)
//   L9779 K_RX (pin 46) -> PD9 (USART3_RX)
// This matches the stock firmware: its LIN driver uses USART3 with the pin
// table entry on port D. The K-line transceiver is a single-wire,
// dominant-low physical layer - electrically a LIN bus. The MCU-side UART
// connects to K_TX/K_RX directly; the L9779 handles the bus drive.
// (The KiCad schematic draws this net to PB5 - wrong, as usual for this
// board. The 74HC14 -> PC10 cascade and the 16k tap to PC11 are the DFM
// monitor, separate from the LIN path.)
static constexpr brain_pin_e LIN_TX_PIN = Gpio::D8;
static constexpr brain_pin_e LIN_RX_PIN = Gpio::D9;
static constexpr iomode_t LIN_AF_MODE = PAL_MODE_ALTERNATE(7); // USART3 on F4/AT32

static SerialDriver* const linDriver = &SD3;

static constexpr int LIN_BAUD = 19200;

// ---------------------------------------------------------------------------
// Frame identifiers
// ---------------------------------------------------------------------------

/* Control frame id: 0x29 (PID 0xE9, standard parity). This is the L9918
 * Rx_B/Rx_A/Rx_D/Rx_F id for LIN1 (datasheet Table 16: "Identifiers: 0x29
 * (LIN1)") - the ON-CAR scan proved the frame set: the regulator answers
 * the identification frame on id 0x12 (Tx_1B, 2 data + classic checksum)
 * with valid checksum. The earlier 0x1D guess was a decimal/hex misread
 * (29 = 0x1D decimal, but the datasheet column is HEX). Switchable live:
 * 'linctlid <id>'. Layout = Version B Rx (8-bit setpoint, Table 62). */
static volatile uint8_t linControlId = 0x29; // PID 0xE9
static constexpr uint8_t LIN_CONTROL_FRAME_LEN    = 4;
static constexpr uint8_t LIN_STATUS_FRAME_LEN     = 8;

// ---------------------------------------------------------------------------
// LIN protocol helpers (classic protected ID + checksums)
// ---------------------------------------------------------------------------

/**
 * LIN protected identifier: 6-bit id + parity bits P0 (bit 6), P1 (bit 7).
 * P0 = b0 ^ b1 ^ b2 ^ b4; P1 = !(b1 ^ b3 ^ b4 ^ b5).
 */
static uint8_t linComputePid(uint8_t id) {
	id &= 0x3F;

	uint8_t p0 = (id ^ (id >> 1) ^ (id >> 2) ^ (id >> 4)) & 0x01;
	uint8_t p1 = ((id >> 1) ^ (id >> 3) ^ (id >> 4) ^ (id >> 5)) & 0x01;
	p1 = !p1;

	return id | (p0 << 6) | (p1 << 7);
}

/**
 * Classic LIN 1.x checksum: 8-bit sum over PID + data, carry-folded, inverted.
 */
static uint8_t linClassicChecksum(uint8_t pid, const uint8_t* data, size_t len) {
	uint16_t sum = pid;
	for (size_t i = 0; i < len; i++) {
		sum += data[i];
		// Carry fold: for the short LIN frames one fold is sufficient.
		sum = (sum & 0xFF) + (sum >> 8);
	}
	sum = (sum & 0xFF) + (sum >> 8);
	return (uint8_t)~sum;
}

/**
 * Enhanced LIN 2.x checksum: 8-bit sum over data only, carry-folded, inverted.
 */
static uint8_t linEnhancedChecksum(const uint8_t* data, size_t len) {
	uint16_t sum = 0;
	for (size_t i = 0; i < len; i++) {
		sum += data[i];
		sum = (sum & 0xFF) + (sum >> 8);
	}
	sum = (sum & 0xFF) + (sum >> 8);
	return (uint8_t)~sum;
}

/* Break length toggle: SBK in LIN mode = exactly 13 bit times (the spec
 * minimum). If the regulator's break detector is picky, a second break
 * back-to-back gives it a longer dominant level (the ~1-bit recessive
 * glitch between the two is legal - the slave re-arms on the second break).
 * 'linbrk double|single'. */
static volatile bool linDoubleBreak = false;

/* Measured duration of the last transmitted break (us), from the SBK spin
 * with the NT clock (4 MHz). At a TRUE 19200 baud a 13-bit break takes
 * ~677 us; if the UART clock tree is off, this number exposes the real
 * baud (e.g. ~1354 us = 9600) - the own echo CANNOT detect a baud error
 * because TX and RX share the same clock. */
static uint32_t lastBreakUs = 0;

/**
 * Transmit a LIN break (dominant >= 13 bit times) using the hardware SBK.
 * Requires LINEN in the USART CR2 (set in the SerialConfig below).
 */
static void linSendBreak() {
	efitick_t t0 = getTimeNowNt();
	SD3.usart->CR1 |= USART_CR1_SBK;
	// SBK self-clears when the break completes (~700 us at 19200)
	while (SD3.usart->CR1 & USART_CR1_SBK) {
		// spin
	}
	if (linDoubleBreak) {
		SD3.usart->CR1 |= USART_CR1_SBK;
		while (SD3.usart->CR1 & USART_CR1_SBK) {
			// spin
		}
	}
	lastBreakUs = (uint32_t)NT2US(getTimeNowNt() - t0);
}

/**
 * Send a full master frame: break + sync + PID + data + checksum.
 * Classic or enhanced checksum per the frame's convention.
 */
static void linSendFrame(uint8_t id, const uint8_t* data, size_t len, bool classicChecksum) {
	uint8_t pid = linComputePid(id);
	uint8_t cks = classicChecksum
		? linClassicChecksum(pid, data, len)
		: linEnhancedChecksum(data, len);

	uint8_t header[2] = { 0x55, pid }; // sync + protected ID

	linSendBreak();
	chnWrite(linDriver, header, sizeof(header));
	if (len > 0) {
		chnWrite(linDriver, data, len);
	}
	chnWrite(linDriver, &cks, 1);
}

/**
 * Drain whatever is pending in the UART RX queue (our own echo etc.).
 */
static void linDrainRx() {
	uint8_t tmp[16];
	while (chnReadTimeout(linDriver, tmp, sizeof(tmp), TIME_US2I(200)) > 0) {
		// discard
	}
}

// ---------------------------------------------------------------------------
// Control state
// ---------------------------------------------------------------------------

// Last transmitted control frame (for linalt diagnostics)
static uint8_t lastTxFrame[LIN_CONTROL_FRAME_LEN];
// Last received status frame (8 data bytes + checksum byte)
static uint8_t lastRxData[LIN_STATUS_FRAME_LEN];
static uint8_t lastRxChecksum;
static bool lastRxValid = false;
// Raw byte count of the last status-poll read - distinguishes a dead wire
// (0 bytes) from a responding-but-unparseable slave (>0 bytes).
static size_t lastRxByteCount = 0;
// Raw bytes of the last status-poll read (own header echo + any slave
// response), for on-car polarity/content diagnosis: 55 <pid> = own echo
// through a non-inverting transceiver, AA xx = inverting transceiver,
// >= 11 bytes = the slave is alive.
static uint8_t lastRxRaw[3 + LIN_STATUS_FRAME_LEN + 1];

/* The status poll PID: computed with the STANDARD LIN parity (verified in
 * the stock's own PID function at 0x80166e2 - identical formula). Default
 * id 0x12 - PROVEN live on the car (2026-08-27 scan: the regulator
 * answered with 2 data bytes + valid classic checksum). 'linpid <id>'
 * switches the poll id live. */
static volatile uint8_t linStatusPid = 0x92; /* PID of id 0x12 */

/* Control-frame checksum convention: CLASSIC (LIN 1.x, checksum includes
 * the PID) - verified live on the car 2026-08-27: with enhanced the
 * regulator ignored the frame (voltage stayed at its ~13.9 V default),
 * with classic it started regulating to the setpoint (battery climbed
 * 13.13 -> 14.05+). Runtime A/B: 'linck classic' / 'linck enhanced'. */
static volatile bool linControlClassicCks = true;

/* Control-frame send toggle: 'linctl off' polls the status header WITHOUT
 * sending the control frame first - isolates whether the control frame
 * (wrong checksum/layout) poisons the schedule, or whether the regulator
 * answers the poll unconditionally. */
static volatile bool linSendControl = true;

/* Runtime setpoint override for on-car tuning: 'linset 14.6' forces the
 * control setpoint (the regulator follows it), 'linset 0' returns to the
 * alternatorVoltageTargetTable. 0 = no override. */
static volatile float linSetpointOverride = 0.0f;

/* Control frame bytes 2/3 - runtime switches: byte 2 = excitation current
 * limitation (7 bits, Version B Rx), byte 3 = RB(2:0) | BZ(3) | F(6:4) |
 * WB(7). The first live frame [.. 0x1E 0xFF] WAS accepted (the generator
 * reacted - the user reports no charge) but 0xFF has WB=1 = the L9918
 * "regulation Without Battery" loop parameters, and 0x1E caps the
 * excitation at 30 units. Corrected defaults: 0x7F (max limit) and 0x02
 * (RB=2 Vmeas feedback, WB=0 with-battery). Tune live on the car:
 * 'linctl2 <hex>' / 'linctl3 <hex>'. */
static volatile uint8_t linControlByte2 = 0x7F;
static volatile uint8_t linControlByte3 = 0x02;

// Counters / timing
static uint32_t txFrameCount = 0;
static uint32_t rxFrameCount = 0;
/* Status-poll outcome counters: the regulator answers only a fraction of
 * polls (2026-08-27: ~4% - the rest echo-only or partial). Polls sent,
 * full valid responses, partial (bytes beyond the echo but no valid cks)
 * and no-response. Plus the response delay (header -> parse) of the last
 * valid frame. */
static uint32_t linPollCount = 0;
static uint32_t linRespFull = 0;
static uint32_t linRespPartial = 0;
static uint32_t linRespNone = 0;
static uint32_t lastRespDelayMs = 0;
/* Tick parity: the control frame goes out every OTHER tick so the status
 * poll on the alternating tick runs after a quiet ~100 ms window - the
 * regulator seems to miss polls that follow the control frame too
 * closely (its LIN block is busy processing the setpoint). */
static uint32_t linTickCounter = 0;
static float mapOffSeconds = 0.0f;
static float offHoldSeconds = 0.0f;
static const char* offReason = "enabled";

// Setpoint / gating results for diagnostics
static float lastSetpointVoltage = 0.0f;
static float lastTargetVoltage = 0.0f;

// ---------------------------------------------------------------------------
// Setpoint computation
// ---------------------------------------------------------------------------

/**
 * Look up the target voltage from alternatorVoltageTargetTable (load x rpm).
 * The table lives in the main tune (msq), shared with the legacy PWM
 * alternator control.
 */
static float alternatorTargetVoltage(float rpm) {
	float load = getEngineState()->fuelingLoad;
	return interpolate3d(
		config->alternatorVoltageTargetTable,
		config->alternatorVoltageTargetLoadBins, load,
		config->alternatorVoltageTargetRpmBins, rpm
	);
}

/**
 * Encode a voltage as the VDA 8-bit setpoint (Version B/D/E/F Rx frames:
 * id 0x1D uses the 8-bit setpoint, Table 62 VSPFBK8B):
 * code = (V - 10.6) / 0.025 = (V - 10.6) x 40, 0 = 10.6 V = OFF.
 * The old 6-bit code6 belongs to Version A (id 0x29), which the stock
 * does NOT use - the stock's id 0x1D is Version B/D/E/F, 8-bit setpoint.
 */
static uint8_t encodeSetpointCode8(float voltage) {
	float code = (voltage - 10.6f) * 40.0f;
	return (uint8_t)clampF(0.0f, code, 255.0f);
}

// ---------------------------------------------------------------------------
// Frame builder
// ---------------------------------------------------------------------------

/**
 * Build the 4-byte control frame - L9918 Rx_B/Rx_D/Rx_E/Rx_F layout (the
 * stock's id 0x1D, 4-byte TX frame; datasheet 5.3.2.1, Table 62):
 *   [0] A8: setpoint, 8 bits (V = 10.6 + code x 0.025)
 *   [1] B2: LRC-rise (bits 3:0) | C1: LRC-cut (bits 7:4)
 *   [2] D7: excitation current limitation, 7 bits (stock default 0x1E = 30)
 *   [3] RB: output selection (bits 2:0: 000/111 = byte3 invalid),
 *       bit3 BZ (LRC blind zone), bits 6:4 F (setpoint limit delta),
 *       bit7 WB - the stock's default buffer uses 0xFF here.
 */
static void buildControlFrame(uint8_t frame[LIN_CONTROL_FRAME_LEN], float setpointVoltage, bool charging) {
	uint8_t setpointCode = charging ? encodeSetpointCode8(setpointVoltage) : 0x00;

	uint8_t lrcRise = (uint8_t)clampF(0.0f, (float)engineConfiguration->m74_9LinLrcRiseCode, 15.0f);
	uint8_t lrcCut  = (uint8_t)clampF(0.0f, (float)engineConfiguration->m74_9LinLrcCutCode, 15.0f);

	frame[0] = setpointCode;
	frame[1] = (uint8_t)((lrcCut << 4) | lrcRise);
	frame[2] = linControlByte2;              // excitation limitation (7 bits)
	frame[3] = linControlByte3;              // RB/BZ/F/WB
}

// ---------------------------------------------------------------------------
// Periodic control
// ---------------------------------------------------------------------------

/**
 * One control cycle: compute the setpoint, apply the user's gates, send the
 * control frame and poll the status response.
 */
static void linAlternatorControlTick() {
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	float map = Sensor::getOrZero(SensorType::Map);

	bool enabled = engineConfiguration->m74_9LinAltEnabled;
	bool rpmBelowMin = rpm < engineConfiguration->m74_9LinAltMinRpm;
	bool overRev = rpm > engineConfiguration->m74_9LinAltMaxRpm;

	// MAP gate: disable the generator during hard acceleration (MAP above the
	// threshold), but never for longer than m74_9LinAltMapOffMaxSeconds
	// continuous (battery protection - force charging after the cap).
	bool mapAbove = map > engineConfiguration->m74_9LinAltMapOffKpa;
	if (mapAbove) {
		// the control tick runs every 100 ms (dedicated thread)
		mapOffSeconds += 0.10f;
	} else {
		mapOffSeconds = 0.0f;
	}
	bool mapOff = mapAbove && (mapOffSeconds <= engineConfiguration->m74_9LinAltMapOffMaxSeconds);

	// Immediate OFF events: MAP gate, low rpm / cranking, over-rev.
	bool immediateOff = rpmBelowMin || overRev || mapOff;

	// Hold timer: while any OFF event is active the generator is off and the
	// timer is re-armed; once all events clear it keeps the generator off for
	// m74_9LinOffHoldSeconds more - a gear-shift MAP dip does not toggle the
	// generator back on for a moment just to switch it off again.
	if (immediateOff) {
		offHoldSeconds = engineConfiguration->m74_9LinOffHoldSeconds;
	} else if (offHoldSeconds > 0.0f) {
		offHoldSeconds -= 0.10f;
		if (offHoldSeconds < 0.0f) {
			offHoldSeconds = 0.0f;
		}
	}
	bool holdActive = offHoldSeconds > 0.0f;

	bool charging = enabled && !immediateOff && !holdActive;

	// LRC-Rise: the regulator ramps the load response up over the configured
	// time on the transition from OFF to charging - the field itself is
	// static, the chip applies the ramp on state entry (VDA behaviour).

	float targetVoltage = (linSetpointOverride > 0.0f)
		? linSetpointOverride
		: alternatorTargetVoltage(rpm);
	float setpointVoltage = charging ? targetVoltage : 10.6f; // 10.6 V = OFF code

	// Diagnostics
	lastTargetVoltage = targetVoltage;
	lastSetpointVoltage = setpointVoltage;
	if (!enabled) {
		offReason = "disabled";
	} else if (rpmBelowMin) {
		offReason = "rpm below min";
	} else if (overRev) {
		offReason = "over rev";
	} else if (mapOff) {
		offReason = "map gate";
	} else if (holdActive) {
		offReason = "off hold";
	} else {
		offReason = "charging";
	}

	uint8_t frame[LIN_CONTROL_FRAME_LEN];
	buildControlFrame(frame, setpointVoltage, charging);

	// Transmit the control frame (enhanced checksum), then drain its own echo
	// (9 bytes: sync + PID + 6 data + checksum) so the response read below
	// starts on the poll header's echo.
	linTickCounter++;
	bool controlThisTick = linSendControl && ((linTickCounter & 1) == 0);

	linDrainRx();
	if (controlThisTick) {
		linSendFrame(linControlId, frame, LIN_CONTROL_FRAME_LEN, /*classic*/linControlClassicCks);
	}
	memcpy(lastTxFrame, frame, sizeof(frame));
	if (controlThisTick) {
		txFrameCount++;
	}
	linDrainRx();

	// Poll the status response: send the 0x08 (PID 0x08) header, the
	// regulator answers with 8 data bytes + classic checksum.  Our own echo
	// lands in the RX queue first; the slave response is always the LAST 9
	// bytes on the wire, so read everything available and parse the tail -
	// robust to whether or not the UART queues the own-break as a 0x00 byte.
	{
		linSendBreak();
		uint8_t pollHeader[2] = { 0x55, linStatusPid };
		chnWrite(linDriver, pollHeader, sizeof(pollHeader));
		efitick_t tHeader = getTimeNowNt();

		uint8_t buf[3 + LIN_STATUS_FRAME_LEN + 1]; // break echo + sync + PID + data + cks
		size_t read = 0;
		/* A single chnReadTimeout returns the burst available at first-byte
		 * wake-up - with the slave response arriving right behind the echo
		 * this split reads at 2 or 5 bytes and missed the trailing checksum
		 * (the 2026-08-27 flaky frames: rxBytes=5, cks byte absent). Loop
		 * until the full echo+response (>= 6 bytes) or an idle gap. The
		 * follow-up windows are 20 ms because the regulator's answer
		 * sometimes starts tens of ms late (observed on the car: the first
		 * response byte arrives, then a long stall - a 10 ms follow-up
		 * window gave up mid-response, rxBytes=4). */
		for (int i = 0; (i < 5) && (read < 6); i++) {
			size_t n = chnReadTimeout(linDriver, buf + read, sizeof(buf) - read,
				(i == 0) ? TIME_MS2I(50) : TIME_MS2I(20));
			if (n == 0) {
				break;
			}
			read += n;
		}
		lastRxByteCount = read;
		memcpy(lastRxRaw, buf, read);

		linPollCount++;
		if (lastRxValid) {
			linRespFull++;
			lastRespDelayMs = (uint32_t)(NT2US(getTimeNowNt() - tHeader) / 1000);
		} else if (read > 3) {
			linRespPartial++;
		} else {
			linRespNone++;
		}

		lastRxValid = false;
		if (read >= 3) {
			/* L9918 Tx_1B-style response: 2 data bytes + classic checksum
			 * (id 0x12, verified live on the car 2026-08-27). The echo is
			 * 2-3 bytes, so the LAST 3 received bytes are the response. */
			const uint8_t* tail = buf + read - 3;
			uint8_t expectedCks = linClassicChecksum(linStatusPid, tail, 2);
			if (tail[2] == expectedCks) {
				lastRxValid = true;
				rxFrameCount++;
				lastRxData[0] = tail[0];
				lastRxData[1] = tail[1];
				lastRxChecksum = tail[2];
			}
		}
	}
}

// ---------------------------------------------------------------------------
// Control thread
// ---------------------------------------------------------------------------

// The control tick MUST NOT run in the board slow callback: that callback is
// driven by a ChibiOS virtual timer, i.e. it executes INSIDE the SysTick ISR
// (PeriodicTimerController, see periodic_task.h: "virtual_timer_t which works
// on interrupts"), and the blocking serial I/O below (chnWrite/chnReadTimeout
// are S-class, thread-only) is illegal there. The first enabled frame used to
// halt the ECU with the ChibiOS class check (SV#10, xlock ipsr=15 - the
// debug aid showed the L9779 writePad X-lock from the same SysTick context).
// A dedicated low-priority thread is the correct context; 100 ms is plenty
// for alternator control.

static constexpr int LIN_TICK_PERIOD_MS = 100;
static THD_WORKING_AREA(linAlternatorThreadStack, 512);

static void linAlternatorThread(void*) {
	chRegSetThreadName("lin alt");

	while (true) {
		if (engineConfiguration->m74_9LinAltEnabled) {
			linAlternatorControlTick();
		} else {
			// Master switch off: keep the line quiet and the regulator on its
			// internal default setpoint (no frames at all). Reset the gate
			// timers so a re-enable starts from a clean state.
			mapOffSeconds = 0.0f;
			offHoldSeconds = 0.0f;
			offReason = "disabled";
		}

		chThdSleepMilliseconds(LIN_TICK_PERIOD_MS);
	}
}

// ---------------------------------------------------------------------------
// Console diagnostics
// ---------------------------------------------------------------------------

static void printLinAltState() {
	// Live table lookup even while the master switch is off - lets the user
	// verify that the alternatorVoltageTargetTable actually burned into the
	// config (a fresh/unburned config reads the all-zero table as 0.0 V).
	float tableTarget = alternatorTargetVoltage(Sensor::getOrZero(SensorType::Rpm));
	efiPrintf("linalt: enabled=%s state=%s rpm=%.0f map=%.1f vbatt=%.2fV",
		engineConfiguration->m74_9LinAltEnabled ? "yes" : "no",
		offReason,
		Sensor::getOrZero(SensorType::Rpm),
		Sensor::getOrZero(SensorType::Map),
		Sensor::getOrZero(SensorType::BatteryVoltage));
	efiPrintf("linalt: target=%.1fV setpoint=%.1fV code8=%d minRpm=%d maxRpm=%d mapOffKpa=%d maxOff=%.1fs hold=%.1f/%.1fs",
		tableTarget, lastSetpointVoltage, encodeSetpointCode8(lastSetpointVoltage),
		engineConfiguration->m74_9LinAltMinRpm,
		engineConfiguration->m74_9LinAltMaxRpm,
		engineConfiguration->m74_9LinAltMapOffKpa,
		(float)engineConfiguration->m74_9LinAltMapOffMaxSeconds,
		offHoldSeconds,
		(float)engineConfiguration->m74_9LinOffHoldSeconds);
	efiPrintf("linalt: lrcRise=%d lrcCut=%d fbSel=%d txFrames=%u ctlCks=%s brk=%s ctl=%s brkUs=%u",
		engineConfiguration->m74_9LinLrcRiseCode,
		engineConfiguration->m74_9LinLrcCutCode,
		engineConfiguration->m74_9LinAltFeedbackSel,
		(unsigned)txFrameCount,
		linControlClassicCks ? "classic" : "enhanced",
		linDoubleBreak ? "double" : "single",
		linSendControl ? "on" : "off",
		(unsigned)lastBreakUs);
	efiPrintf("linalt: tx=%02x %02x %02x %02x",
		lastTxFrame[0], lastTxFrame[1], lastTxFrame[2], lastTxFrame[3]);
	efiPrintf("linalt: rx=%02x %02x %02x %02x %02x %02x %02x %02x cks=%02x %s frames=%u rxBytes=%u",
		lastRxData[0], lastRxData[1], lastRxData[2], lastRxData[3],
		lastRxData[4], lastRxData[5], lastRxData[6], lastRxData[7],
		lastRxChecksum, lastRxValid ? "OK" : "BAD", (unsigned)rxFrameCount,
		(unsigned)lastRxByteCount);
	/* Tx_1B (id 0x12) content: [0] = AltS(3) | AltL(5), [1] = DieS(3) | DieL(5) */
	if (lastRxValid) {
		efiPrintf("linalt: altS=%d altL=%d dieS=%d dieL=%d",
			lastRxData[0] >> 5, lastRxData[0] & 0x1F,
			lastRxData[1] >> 5, lastRxData[1] & 0x1F);
	}
	efiPrintf("linalt: polls=%u full=%u part=%u none=%u respMs=%u",
		(unsigned)linPollCount, (unsigned)linRespFull, (unsigned)linRespPartial,
		(unsigned)linRespNone, (unsigned)lastRespDelayMs);
	efiPrintf("linalt: raw=%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
		lastRxRaw[0], lastRxRaw[1], lastRxRaw[2], lastRxRaw[3],
		lastRxRaw[4], lastRxRaw[5], lastRxRaw[6], lastRxRaw[7],
		lastRxRaw[8], lastRxRaw[9], lastRxRaw[10], lastRxRaw[11]);
}

// ---------------------------------------------------------------------------
// Init
// ---------------------------------------------------------------------------

static uint8_t parseHexByte(const char* arg, uint8_t current) {
	if (!arg || !*arg) {
		return current;
	}
	uint8_t v = 0;
	for (const char* p = arg; *p; p++) {
		v <<= 4;
		char c = *p;
		if ((c >= '0') && (c <= '9')) {
			v |= (uint8_t)(c - '0');
		} else if ((c >= 'a') && (c <= 'f')) {
			v |= (uint8_t)(c - 'a' + 10);
		} else if ((c >= 'A') && (c <= 'F')) {
			v |= (uint8_t)(c - 'A' + 10);
		} else {
			return current;
		}
	}
	return v;
}

void initM74_9LinAlternator() {
	// USART3, 19200 8N1, LIN mode (LINEN enables break detection + the SBK
	// break generator used by linSendBreak()).
	static SerialConfig linCfg = {
		.speed = LIN_BAUD,
		.cr1 = 0,
		.cr2 = USART_CR2_STOP1_BITS | USART_CR2_LINEN,
		.cr3 = 0
	};

	efiSetPadMode("LIN TX", LIN_TX_PIN, LIN_AF_MODE);
	efiSetPadMode("LIN RX", LIN_RX_PIN, LIN_AF_MODE);

	sdStart(linDriver, &linCfg);

	chThdCreateStatic(linAlternatorThreadStack, sizeof(linAlternatorThreadStack),
		LOWPRIO, linAlternatorThread, nullptr);

	addConsoleAction("linalt", printLinAltState);
	addConsoleActionS("linpid", [](const char* arg) {
		uint8_t id = 0;
		for (const char* p = (arg && *arg) ? arg : ""; *p; p++) {
			id <<= 4;
			char c = *p;
			if ((c >= '0') && (c <= '9')) {
				id |= (uint8_t)(c - '0');
			} else if ((c >= 'a') && (c <= 'f')) {
				id |= (uint8_t)(c - 'a' + 10);
			} else if ((c >= 'A') && (c <= 'F')) {
				id |= (uint8_t)(c - 'A' + 10);
			} else {
				efiPrintf("linpid: bad hex byte '%s'", arg ? arg : "");
				return;
			}
		}
		id &= 0x3F;
		linStatusPid = linComputePid(id);
		efiPrintf("linpid: status poll id 0x%02X -> PID 0x%02X (next poll cycle)", id, linStatusPid);
	});
	addConsoleActionS("linck", [](const char* arg) {
		if (rusefi::stringutil::strEqual(arg, "classic")) {
			linControlClassicCks = true;
		} else if (rusefi::stringutil::strEqual(arg, "enhanced")) {
			linControlClassicCks = false;
		} else {
			efiPrintf("linck: use 'classic' or 'enhanced'");
			return;
		}
		efiPrintf("linck: control frame checksum = %s", linControlClassicCks ? "classic" : "enhanced");
	});
	addConsoleActionS("linbrk", [](const char* arg) {
		if (rusefi::stringutil::strEqual(arg, "double")) {
			linDoubleBreak = true;
		} else if (rusefi::stringutil::strEqual(arg, "single")) {
			linDoubleBreak = false;
		} else {
			efiPrintf("linbrk: use 'single' or 'double'");
			return;
		}
		efiPrintf("linbrk: break = %s", linDoubleBreak ? "double (2x13b)" : "single (13b)");
	});
	addConsoleActionS("linctl", [](const char* arg) {
		if (rusefi::stringutil::strEqual(arg, "off")) {
			linSendControl = false;
		} else if (rusefi::stringutil::strEqual(arg, "on")) {
			linSendControl = true;
		} else {
			efiPrintf("linctl: use 'on' or 'off'");
			return;
		}
		efiPrintf("linctl: control frame = %s", linSendControl ? "on" : "off");
	});
	addConsoleActionS("linctlid", [](const char* arg) {
		uint8_t id = 0;
		for (const char* p = (arg && *arg) ? arg : ""; *p; p++) {
			id <<= 4;
			char c = *p;
			if ((c >= '0') && (c <= '9')) {
				id |= (uint8_t)(c - '0');
			} else if ((c >= 'a') && (c <= 'f')) {
				id |= (uint8_t)(c - 'a' + 10);
			} else if ((c >= 'A') && (c <= 'F')) {
				id |= (uint8_t)(c - 'A' + 10);
			} else {
				efiPrintf("linctlid: bad hex byte '%s'", arg ? arg : "");
				return;
			}
		}
		linControlId = id & 0x3F;
		efiPrintf("linctlid: control id 0x%02X -> PID 0x%02X", linControlId, linComputePid(linControlId));
	});
	addConsoleActionS("linset", [](const char* arg) {
		float v = arg ? (float)atof(arg) : 0.0f;
		linSetpointOverride = v;
		efiPrintf("linset: setpoint override = %.2fV (%s)", (double)v,
			(v > 0.0f) ? "forced" : "back to table");
	});
	addConsoleActionS("linctl2", [](const char* arg) {
		linControlByte2 = parseHexByte(arg, linControlByte2);
		efiPrintf("linctl2: excitation limit byte = 0x%02X", linControlByte2);
	});
	addConsoleActionS("linctl3", [](const char* arg) {
		linControlByte3 = parseHexByte(arg, linControlByte3);
		efiPrintf("linctl3: RB/BZ/F/WB byte = 0x%02X (RB=%d WB=%d)",
			linControlByte3, linControlByte3 & 7, (linControlByte3 >> 7) & 1);
	});

	efiPrintf("LIN alternator master: USART3 19200, control 0x29/PID 0xE9 (4B, enhanced, L9918 Rx_B), status 0x12/PID 0x92 (Tx_1B, 2B + classic)");
}

#endif // EFI_PROD_CODE
