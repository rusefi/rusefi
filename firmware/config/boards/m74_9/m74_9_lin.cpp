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

static constexpr uint8_t LIN_ID_ALTERNATOR_CONTROL = 0x16; // PID 0xD6, 6 bytes
static constexpr uint8_t LIN_CONTROL_FRAME_LEN    = 6;
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

/**
 * Transmit a LIN break (dominant >= 13 bit times) using the hardware SBK.
 * Requires LINEN in the USART CR2 (set in the SerialConfig below).
 */
static void linSendBreak() {
	SD3.usart->CR1 |= USART_CR1_SBK;
	// SBK self-clears when the break completes (~700 us at 19200)
	while (SD3.usart->CR1 & USART_CR1_SBK) {
		// spin
	}
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

/* The status poll PID: the STOCK's literal frame-table value 0xC8 (LIN 1.x
 * parity for id 0x08), NOT the LIN 2.x-computed 0x08. The stock firmware
 * stores the full PID byte in its frame table and the regulator answers it;
 * with the computed 0x08 the regulator stays silent (2026-08-27 on the car:
 * rxBytes=2 = own header echo only, zero response frames). Runtime A/B
 * switch for the on-car test: 'linpid 08' / 'linpid c8'. */
static volatile uint8_t linStatusPid = 0xC8;

/* Control-frame checksum convention toggle: the stock frame-table bit said
 * enhanced (LIN 2.x, data only), but the extraction also notes the regulator
 * speaks LIN 1.x classic. If the regulator rejects the control frame it may
 * stay inactive and never answer the status poll at all (2026-08-27: both
 * PIDs tested, regulator silent, echo clean, polarity verified non-inverting).
 * Runtime A/B on the car: 'linck classic' / 'linck enhanced'. */
static volatile bool linControlClassicCks = false;

// Counters / timing
static uint32_t txFrameCount = 0;
static uint32_t rxFrameCount = 0;
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
 * Encode a voltage as the VDA 6-bit setpoint code:
 * code = (V - 10.6) x 10, clamped to [0, 63]; 0 = 10.6 V = OFF.
 */
static uint8_t encodeSetpointCode6(float voltage) {
	float code = (voltage - 10.6f) * 10.0f;
	return (uint8_t)clampF(0.0f, code, 63.0f);
}

// ---------------------------------------------------------------------------
// Frame builder
// ---------------------------------------------------------------------------

/**
 * Build the 6-byte control frame.  Layout is the VDA field order, PROVISIONAL
 * until the on-car capture confirms it (see the file header):
 *   [0] setpoint code6 (bits 0-5)
 *   [1] LRC-rise (bits 0-3) | LRC-cut (bits 4-7)
 *   [2] excitation current limit (0 = no limit)
 *   [3] R feedback selection (bits 0-2) | BZ (3) | F (4-6) | WB (7)
 *   [4..5] 0xFF padding
 */
static void buildControlFrame(uint8_t frame[LIN_CONTROL_FRAME_LEN], float setpointVoltage, bool charging) {
	uint8_t setpointCode = charging ? encodeSetpointCode6(setpointVoltage) : 0x00;

	uint8_t lrcRise = (uint8_t)clampF(0.0f, (float)engineConfiguration->m74_9LinLrcRiseCode, 15.0f);
	uint8_t lrcCut  = (uint8_t)clampF(0.0f, (float)engineConfiguration->m74_9LinLrcCutCode, 15.0f);
	uint8_t feedbackSel = (uint8_t)clampF(0.0f, (float)engineConfiguration->m74_9LinAltFeedbackSel, 7.0f);

	frame[0] = setpointCode;
	frame[1] = (uint8_t)((lrcCut << 4) | lrcRise);
	frame[2] = 0x00;                       // excitation current limit: no limit
	frame[3] = (uint8_t)(feedbackSel & 0x07); // R selection, BZ/F/WB = 0
	frame[4] = 0xFF;
	frame[5] = 0xFF;
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

	float targetVoltage = alternatorTargetVoltage(rpm);
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
	linDrainRx();
	linSendFrame(LIN_ID_ALTERNATOR_CONTROL, frame, LIN_CONTROL_FRAME_LEN, /*classic*/linControlClassicCks);
	memcpy(lastTxFrame, frame, sizeof(frame));
	txFrameCount++;
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

		uint8_t buf[3 + LIN_STATUS_FRAME_LEN + 1]; // break echo + sync + PID + 8 data + cks
		size_t read = chnReadTimeout(linDriver, buf, sizeof(buf), TIME_MS2I(30));
		lastRxByteCount = read;
		memcpy(lastRxRaw, buf, read);

		lastRxValid = false;
		if (read >= LIN_STATUS_FRAME_LEN + 1) {
			const uint8_t* response = buf + read - (LIN_STATUS_FRAME_LEN + 1);
			memcpy(lastRxData, response, LIN_STATUS_FRAME_LEN);
			lastRxChecksum = response[LIN_STATUS_FRAME_LEN];
			uint8_t expectedCks = linClassicChecksum(linStatusPid, response, LIN_STATUS_FRAME_LEN);
			if (lastRxChecksum == expectedCks) {
				lastRxValid = true;
				rxFrameCount++;
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
	efiPrintf("linalt: enabled=%s state=%s rpm=%.0f map=%.1f",
		engineConfiguration->m74_9LinAltEnabled ? "yes" : "no",
		offReason,
		Sensor::getOrZero(SensorType::Rpm),
		Sensor::getOrZero(SensorType::Map));
	efiPrintf("linalt: target=%.1fV setpoint=%.1fV code6=%d minRpm=%d maxRpm=%d mapOffKpa=%d maxOff=%.1fs hold=%.1f/%.1fs",
		tableTarget, lastSetpointVoltage, encodeSetpointCode6(lastSetpointVoltage),
		engineConfiguration->m74_9LinAltMinRpm,
		engineConfiguration->m74_9LinAltMaxRpm,
		engineConfiguration->m74_9LinAltMapOffKpa,
		(float)engineConfiguration->m74_9LinAltMapOffMaxSeconds,
		offHoldSeconds,
		(float)engineConfiguration->m74_9LinOffHoldSeconds);
	efiPrintf("linalt: lrcRise=%d lrcCut=%d fbSel=%d txFrames=%u ctlCks=%s",
		engineConfiguration->m74_9LinLrcRiseCode,
		engineConfiguration->m74_9LinLrcCutCode,
		engineConfiguration->m74_9LinAltFeedbackSel,
		(unsigned)txFrameCount,
		linControlClassicCks ? "classic" : "enhanced");
	efiPrintf("linalt: tx=%02x %02x %02x %02x %02x %02x",
		lastTxFrame[0], lastTxFrame[1], lastTxFrame[2],
		lastTxFrame[3], lastTxFrame[4], lastTxFrame[5]);
	efiPrintf("linalt: rx=%02x %02x %02x %02x %02x %02x %02x %02x cks=%02x %s frames=%u rxBytes=%u",
		lastRxData[0], lastRxData[1], lastRxData[2], lastRxData[3],
		lastRxData[4], lastRxData[5], lastRxData[6], lastRxData[7],
		lastRxChecksum, lastRxValid ? "OK" : "BAD", (unsigned)rxFrameCount,
		(unsigned)lastRxByteCount);
	efiPrintf("linalt: raw=%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
		lastRxRaw[0], lastRxRaw[1], lastRxRaw[2], lastRxRaw[3],
		lastRxRaw[4], lastRxRaw[5], lastRxRaw[6], lastRxRaw[7],
		lastRxRaw[8], lastRxRaw[9], lastRxRaw[10], lastRxRaw[11]);
}

// ---------------------------------------------------------------------------
// Init
// ---------------------------------------------------------------------------

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
		uint8_t v = 0;
		for (const char* p = (arg && *arg) ? arg : ""; *p; p++) {
			v <<= 4;
			char c = *p;
			if ((c >= '0') && (c <= '9')) {
				v |= (uint8_t)(c - '0');
			} else if ((c >= 'a') && (c <= 'f')) {
				v |= (uint8_t)(c - 'a' + 10);
			} else if ((c >= 'A') && (c <= 'F')) {
				v |= (uint8_t)(c - 'A' + 10);
			} else {
				efiPrintf("linpid: bad hex byte '%s'", arg ? arg : "");
				return;
			}
		}
		linStatusPid = v;
		efiPrintf("linpid set to 0x%02x (status poll uses it from the next cycle)", (unsigned)v);
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

	efiPrintf("LIN alternator master: USART3 19200, control 0x16/PID 0xD6 (enhanced), status PID 0xC8 stock-literal (switchable via 'linpid', classic cks)");
}

#endif // EFI_PROD_CODE
