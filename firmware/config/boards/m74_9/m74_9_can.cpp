/**
 * @file    m74_9_can.cpp
 *
 * BCM (Body Control Module) CAN protocol for the m74_9 board.
 *
 * Protocol overview
 * -----------------
 * BCM -> ECU (receive):
 *   0x0350 (100 ms) - BCM state machine; byte 4 encodes IGN/crank/relay/running state
 *   0x0303 (100 ms) - BCM relay status; byte 5 bit 1 = starter relay active
 *
 * ECU -> BCM (transmit, emulating the original ECU):
 *   0x01F6 (10 ms)  - ECU engine state
 *   0x0189 (10 ms)  - RPM primary
 *   0x0186 (10 ms)  - RPM aux
 *   0x018A (10 ms)  - RPM aux2
 *   0x0217 (10 ms)  - ECU status
 *   0x02A9 (10 ms)  - Heartbeat
 *   0x02C6 (20 ms)  - Static frame
 *   Burst  (100 ms) - ECU identification / status (11 frames)
 *
 * Timing
 * ------
 * request() is called every 5 ms by CanWrite::PeriodicTask(). m_counter is
 * incremented once per call and used as:
 *   m_counter % 2  == 0  ->  10 ms group
 *   m_counter % 4  == 0  ->  20 ms group
 *   m_counter % 20 == 0  -> 100 ms burst group
 *
 * @date    2026-08-12
 */

#include "pch.h"

#if EFI_CAN_SUPPORT

#include "m74_9_can.h"
#include "can_listener.h"
#include "can_msg_tx.h"
#include "ignition_controller.h"

// ---------------------------------------------------------------------------
// Frame IDs - BCM -> ECU (receive)
// ---------------------------------------------------------------------------
static constexpr uint32_t BCM_STATE_ID = 0x0350;   // BCM state machine
static constexpr uint32_t BCM_RELAY_ID = 0x0303;   // BCM relay status

// Byte 4 threshold for "starter relay active or engine running" in 0x0350
static constexpr uint8_t BCM_STARTER_ACTIVE_THRESHOLD = 0x80;

// ---------------------------------------------------------------------------
// IMMO challenge-response protocol (0x0713 / 0x0714)
// ---------------------------------------------------------------------------
// Protocol direction (confirmed from PCAN-View timing analysis):
//   ECU  -> 0x0713 [8 bytes]  : trigger / session request  (ECU initiates)
//   BCM  -> 0x0714 [8 bytes]  : challenge frame 1           }
//   BCM  -> 0x0714 [8 bytes]  : challenge frame 2  (0.2ms)  } 16-byte challenge
//   ECU  -> 0x0713 [8 bytes]  : computed response (15-20ms after challenge)
//   BCM  -> 0x0714 [8 bytes]  : periodic re-check (single frame, ~200ms)
//   ECU  -> 0x0713 [8 bytes]  : response           (<5ms)
//
// BCM physically closes the starter relay ONLY after a valid IMMO response.
// Without a correct 0x0713 response, BCM shows 0x84 in CAN but does not
// assert the relay GND signal.
//
// The response algorithm is at flash offset 0x203F0C in the original firmware
// (I865LB52_w2404b1 dump).  Load the dump in Ghidra (ARM Cortex-M LE Thumb,
// base 0x08000000) and decompile 0x08203F0C to extract the algorithm and key.
// Fill in computeImmoResponse() below once the algorithm is known.

static constexpr uint32_t IMMO_ECU_ID  = 0x0713;  // ECU sends (trigger + response)
static constexpr uint32_t IMMO_BCM_ID  = 0x0714;  // BCM sends (challenge)

// How long after first 0x0350 (IGN ON, byte4=0x04) before ECU sends the first trigger.
// Original firmware sends the trigger ~1500ms after ignition is first detected.
// rusEFI starts CAN earlier, so 1000ms should be enough for BCM to stabilise.
static constexpr uint32_t IMMO_TRIGGER_DELAY_MS = 1000u;
// Window within which two consecutive 0x0714 frames are treated as one 16-byte challenge
static constexpr uint32_t IMMO_CHALLENGE_WINDOW_MS = 5u;
// How long after receiving a challenge before sending the response
static constexpr uint32_t IMMO_RESPONSE_DELAY_MS = 15u;

// ---------------------------------------------------------------------------
// Frame IDs - ECU -> BCM (transmit)
// ---------------------------------------------------------------------------
static constexpr uint32_t ECU_ENGINE_STATE_ID = 0x01F6;
static constexpr uint32_t ECU_RPM_PRIMARY_ID  = 0x0189;
static constexpr uint32_t ECU_RPM_AUX_ID      = 0x0186;
static constexpr uint32_t ECU_RPM_AUX2_ID     = 0x018A;
static constexpr uint32_t ECU_STATUS_ID        = 0x0217;
static constexpr uint32_t ECU_HEARTBEAT_ID     = 0x02A9;
static constexpr uint32_t ECU_STATIC_ID        = 0x02C6;

// Periodic keepalive (1000 ms)
static constexpr uint32_t ECU_KEEPALIVE_ID  = 0x05E2;

// Burst group (100 ms)
static constexpr uint32_t BURST_ID_05DA = 0x05DA;
static constexpr uint32_t BURST_ID_036E = 0x036E;
static constexpr uint32_t BURST_ID_03A5 = 0x03A5;
static constexpr uint32_t BURST_ID_0511 = 0x0511;
static constexpr uint32_t BURST_ID_06FD = 0x06FD;
static constexpr uint32_t BURST_ID_0648 = 0x0648;
static constexpr uint32_t BURST_ID_041A = 0x041A;
static constexpr uint32_t BURST_ID_066A = 0x066A;
static constexpr uint32_t BURST_ID_0522 = 0x0522;
static constexpr uint32_t BURST_ID_065C = 0x065C;
static constexpr uint32_t BURST_ID_041D = 0x041D;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

/**
 * Encode RPM as RPM*16 big-endian uint16_t (per BCM protocol).
 * High byte -> [0], low byte -> [1].
 *
 * When RPM is zero (engine not rotating) the original ECU still broadcasts
 * ~0x3200 (800 RPM * 16) in the bytes of 0x0189/0x0186/0x018A that carry
 * this value.  BCM treats the non-zero baseline as "ECU calibration loaded
 * and ready"; a zero value triggers BCM error state CF and prevents the
 * starter relay from physically closing.  Use the original idle-target
 * baseline (0x3200) whenever the engine is not yet spinning.
 */
static inline uint16_t encodeRpmOrBaseline(float rpm) {
    if (rpm < 1.0f) {
        // 800 * 16 = 0x3200 — matches original firmware idle-target baseline
        return 0x3200u;
    }
    return (uint16_t)(rpm * 16.0f);
}

/**
 * Instantaneous fuel flow for the dash BC (бортовой компьютер), in mL/h.
 *
 * Reverse-engineered from orig_1/2/3.trc: 0x0186 bytes 0-1 are 0x0000 with the
 * engine stopped, 0x004D..0x00ED (77..237) while cranking, peak ~0x0360 (864)
 * at the catch and settle to ~0x026C (620) at warm idle - and drop to 0x0000
 * during DFCO (fuel cut) even though rpm was still climbing.  That is a fuel
 * FLOW, not rpm and not pulse width (both of those would not zero out in DFCO
 * while rpm rises).  The same field is a static 0x281C in the old rusEFI
 * emulation, which is exactly the "frozen packets" the dash shows.
 *
 * If the BC reads ~25% low vs a known-good flowmeter, the stock field may be
 * g/h instead of mL/h - remove the density division (use gPerHour directly).
 */
static inline uint16_t encodeFuelFlowMlPerHour() {
#ifdef MODULE_ODOMETER
	float gPerSecond = engine->module<TripOdometer>()->getConsumptionGramPerSecond();
#else
	float gPerSecond = 0;
#endif // MODULE_ODOMETER

	// Fuel cut: zero flow. The original ECU does exactly this - the flow
	// field drops to 0x0000 during DFCO even while rpm is still climbing
	// (orig_1/2/3 captures).
	if (engine->module<DfcoController>()->cutFuel()) {
		gPerSecond = 0;
	}

	// The trip-odometer rate is per-injection-event and spiky; the 10 ms CAN
	// sampling would otherwise freeze one event's instantaneous rate between
	// cycles and feed the dash garbage. EMA-smooth it towards a steady value
	// (~200 ms time constant at the 10 ms send rate).
	static float smoothedGps = 0;
	smoothedGps += 0.05f * (gPerSecond - smoothedGps);

	// g/s -> mL/h: *3600 (s->h), /0.745 g/mL (gasoline density)
	float mlPerHour = smoothedGps * 3600.0f / 0.745f;
	return (uint16_t)clampF(0.0f, mlPerHour, 65535.0f);
}

/**
 * Dash tach byte - 0x0186 byte 4. Stock fit (orig_1/2/3): 0x20 at the
 * 800 rpm rest/idle baseline, 0x33-0x35 at the ~1100 rpm rev blip.
 * The bench captures never exceed ~915 rpm, so the dash's real scale above
 * idle is a guess - the encoding is switchable on the car with 'cantach'
 * until the needle agrees with the console rpm at a known engine speed.
 */
static int s_tachEncoding = 0;

static const char* tachEncodingDescription(int mode) {
	switch (mode) {
		case 1: return "rpm - 768 (caps at 1023)";
		case 2: return "rpm/8 - 68 (caps at 2584)";
		case 3: return "rpm/32 + 7 (covers ~7900)";
		default: return "rpm/16 - 18 (capture fit, caps at ~4368)";
	}
}

static uint8_t encodeTachByte(float rpm) {
	if (rpm < 1.0f) {
		// rest baseline - all modes agree at the 800 rpm point (0x20 = 32)
		return 0x20;
	}

	switch (s_tachEncoding) {
		case 1: return (uint8_t)clampF(0.0f, rpm - 768.0f, 255.0f);
		case 2: return (uint8_t)clampF(0.0f, rpm / 8.0f - 68.0f, 255.0f);
		case 3: return (uint8_t)clampF(0.0f, rpm / 32.0f + 7.0f, 255.0f);
		default: return (uint8_t)clampF(0.0f, rpm / 16.0f - 18.0f, 255.0f);
	}
}

static void setTachEncoding(const char* arg) {
	if (arg == nullptr || arg[0] == 0) {
		efiPrintf("cantach: current=%d (%s). 0=rpm/16-18 1=rpm-768 2=rpm/8-68 3=rpm/32+7",
			s_tachEncoding, tachEncodingDescription(s_tachEncoding));
		return;
	}

	int mode = atoi(arg);
	if (mode < 0 || mode > 3) {
		efiPrintf("cantach: bad mode %d (0..3)", mode);
		return;
	}

	s_tachEncoding = mode;
	efiPrintf("cantach: mode=%d (%s)", mode, tachEncodingDescription(mode));
}

/**
 * Battery voltage, little-endian millivolts - 0x0189 bytes 2-3.
 * orig traces: 0x3220 LE = 12832 mV = 12.83 V at rest, 0x38CD..0x3CE9 LE
 * (14.5..15.6 V) while the alternator/bench supply charges.
 */
static inline uint16_t encodeBatteryMillivoltsLE() {
    float volts = Sensor::getOrZero(SensorType::BatteryVoltage);
    return (uint16_t)clampF(0.0f, volts * 1000.0f, 65535.0f);
}

/**
 * Coolant temperature for the dash gauge - 0x066A bytes 3 and 4, raw degC.
 * orig traces: both bytes track the CLT ramp and saturate at the thermostat
 * (95..98 degC in orig_1/orig_3, frozen after shutdown) - the signature of a
 * coolant-temperature gauge value, duplicated in two byte positions.
 */
static inline uint8_t encodeCltDegC() {
    float clt = Sensor::getOrZero(SensorType::Clt);
    return (uint8_t)clampF(0.0f, clt, 255.0f);
}

/**
 * Ignition-advance byte - 0x018A byte 4: 256 - 2*advance while running.
 * orig_1 trace: 0xFA (250 -> 3 deg) at the catch, decaying to 0xE2 (226 ->
 * 15 deg) at warm idle, retarding back to ~0xF2 (242 -> 7 deg) during the DFCO
 * blip, 0x02 cranking and 0x00 at IGN-on.  That is exactly the stock warm-up
 * advance curve (7 deg at catch -> 15 deg warm idle) with the DFCO retard.
 */
static inline uint8_t encodeIgnitionAdvanceByte(float timingAdvanceDeg) {
    // 256 - 2*advance; clamp so the byte stays in [0x02, 0xFE]
    float encoded = 256.0f - 2.0f * timingAdvanceDeg;
    return (uint8_t)clampF(2.0f, encoded, 254.0f);
}

// ---------------------------------------------------------------------------
// IMMO response computation
// ---------------------------------------------------------------------------

/**
 * Compute the 8-byte 0x0713 response from the 16-byte 0x0714 challenge.
 *
 * TODO: reverse-engineer the algorithm from the original firmware. The immo
 * module lives in the second flash bank around 0x08203F0C - 0x08204D00
 * (Ghidra: base 0x08000000, ARM Cortex-M LE Thumb). Relevant functions:
 *   0x08203FFC - immo dispatcher / state machine
 *   0x082047D0 - 0x0713/0x0714 frame staging helper
 *   0x082048E8 - 0x0714 receive handler / buffer copy
 *   0x08204B00 - 0x08204C00 - key constants and registration area
 *
 * Captured challenge/response pairs (full 16-byte challenge):
 *   trigger 0841994c0b81a14c
 *     challenge 4a4f2a204fad58fd273622b70c2b559e -> response fda32d94ae77c121
 *   trigger d081cc982708d806
 *     challenge 66aaeef37037dee0cdeefb22bde96807 -> response efaa66f0caa6f0cd
 *   trigger 5283dba64769c00d
 *     challenge 660be1e2a34b8140b45633a0499a01ec -> response 9cb7f8ca31431bb6
 *   trigger a5ea8a93265530dd
 *     challenge cfcfbbf3cdc0f75ce9efe2eb23b62a25 -> response bff99205ed4ab7a8
 *   trigger 45d4a5c0f6a1ea72
 *     challenge cd4c9070196bbdebb425cb7c4350082c -> response 96739be6b1299f77
 *
 * @param challenge  16 bytes received in two consecutive 0x0714 CAN frames
 *                   (frame1[0..7] || frame2[0..7])
 * @param response   8-byte output buffer, filled by this function
 * @return true if the computation succeeded (key loaded, algorithm known)
 */
static bool computeImmoResponse(const uint8_t challenge[16], uint8_t response[8]) {
    (void)challenge;
    (void)response;
    return false;
}

/**
 * Compute the 8-byte 0x0713 response to a SINGLE-frame (8-byte) 0x0714 challenge.
 * This is the "quick re-check" exchange that BCM sends periodically.
 *
 * Captured quick pairs:
 *   challenge 596e3885f2460f4a -> response 2cdf61de56551b86
 *   challenge a00059b79f90a8ae -> response a225de4e470324b6
 *   challenge 0eabfe9d351af837 -> response b15156d14683cd15
 *   challenge 4a90250e855128bf -> response 14a267bec8d8c3dc
 *   challenge 63b95b1a3fbf41fe -> response 2fa71d4722835ea2
 */
static bool computeImmoQuickResponse(const uint8_t challenge[8], uint8_t response[8]) {
    (void)challenge;
    (void)response;
    return false;
}

// ---------------------------------------------------------------------------
// IMMO logging helpers
// ---------------------------------------------------------------------------

static void immoLog8(const char* label, const uint8_t* data) {
    efiPrintf("IMMO: %s %02x %02x %02x %02x %02x %02x %02x %02x",
        label,
        data[0], data[1], data[2], data[3],
        data[4], data[5], data[6], data[7]);
}

static void immoLog16(const char* label, const uint8_t* data) {
    efiPrintf("IMMO: %s %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x",
        label,
        data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
        data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
}

// ---------------------------------------------------------------------------
// Listener class
// ---------------------------------------------------------------------------

class M74_9BcmCanListener : public CanListener {
public:
    M74_9BcmCanListener()
        : CanListener(BCM_STATE_ID)
    {
    }

    bool isStarterActive() const {
        return m_starterActive;
    }

    bool isImmoAuthenticated() const {
        // With the IMMO force-off bit (physical bypass installed) the BCM does
        // not need our responses - report authenticated so no future gating logic
        // sees a failed handshake.
        return config->m74_9ImmoOff || (m_immoState == ImmoState::ResponseSent);
    }

    // Accept BCM state/relay frames AND IMMO challenge frames.
    // With m74_9ImmoOff set the 0x0714 challenge frames are rejected here, so
    // they never enter the RX path or the IMMO state machine at all.
    bool acceptFrame(const size_t busIndex, const CANRxFrame& frame) const override {
        (void)busIndex;
        uint32_t id = CAN_ID(frame);
        return (id == BCM_STATE_ID) || (id == BCM_RELAY_ID)
            || ((id == IMMO_BCM_ID) && !config->m74_9ImmoOff);
    }

    // Called every 5 ms by CanWrite::PeriodicTask()
    CanListener* request() override {
        sendPeriodic();
        m_counter++;
        return getNext();
    }

protected:
    // Decode received BCM frames and update local state flags
    void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) override {
        uint32_t id = CAN_ID(frame);

        if (id == BCM_STATE_ID) {
            // byte 4: 0x04 = IGN on/no start, 0x44 = crank switch pressed,
            //         0x84 = starter relay active, 0xC4 = engine running.
            uint8_t byte4 = frame.data8[4];
            m_starterActive = (byte4 >= BCM_STARTER_ACTIVE_THRESHOLD);

            // Arm IMMO trigger on ignition-on (byte4 == 0x04), NOT on crank switch.
            //
            // BCM does NOT assert crank-switch (byte4=0x44) until the IMMO
            // handshake is successfully completed.  Waiting for byte4=0x44
            // creates a dead-lock: rusEFI never sends the trigger, BCM never
            // sends the challenge, IMMO is never cleared, engine never starts.
            //
            // The original ECU sends the 0x0713 trigger ~1.5 s after the first
            // 0x0350 frame is received (IGN ON, byte4=0x04).  We mirror that:
            // arm the IMMO timer on any BCM active state so that the trigger
            // fires well before the user presses the crank switch.
            bool ignitionActive = (byte4 != 0x00);
            // With m74_9ImmoOff the IMMO state machine stays in Idle: no trigger,
            // no response, no 0x0713 flood (the physical bypass answers the BCM).
            if (ignitionActive && (m_immoState == ImmoState::Idle) && !config->m74_9ImmoOff) {
                m_immoState        = ImmoState::WaitingToTrigger;
                m_immoTimerTicks   = 0;
            }
        } else if (id == BCM_RELAY_ID) {
            // byte 5 bit 1 (0x02): starter relay active
            m_relayActive = ((frame.data8[5] & 0x02) != 0);
        } else if (id == IMMO_BCM_ID) {
            handleImmoChallengeFrame(frame, nowNt);
        }
    }

private:
    // -----------------------------------------------------------------------
    // IMMO state machine
    // -----------------------------------------------------------------------

    enum class ImmoState : uint8_t {
        Idle,             // no crank request active
        WaitingToTrigger, // crank switch seen; waiting IMMO_TRIGGER_DELAY_MS
        TriggerSent,      // sent 0x0713 trigger; waiting for 0x0714 challenge
        Challenge1Rx,     // received first 0x0714 frame; waiting for second
        ChallengeComplete, // both 0x0714 frames received; compute + send response
        ResponseSent,     // 0x0713 response sent; protocol complete for this session
    };

    /**
     * Process an incoming 0x0714 (IMMO challenge) frame.
     * BCM sends the 16-byte challenge as two consecutive 8-byte CAN frames.
     */
    void handleImmoChallengeFrame(const CANRxFrame& frame, efitick_t /*nowNt*/) {
        if (m_immoState == ImmoState::TriggerSent) {
            // First challenge frame
            for (int i = 0; i < 8; i++) {
                m_immoChallenge[i] = frame.data8[i];
            }
            immoLog8("challenge frame 1", m_immoChallenge);
            m_immoState      = ImmoState::Challenge1Rx;
            m_immoTimerTicks = 0;
        } else if (m_immoState == ImmoState::Challenge1Rx) {
            // Second challenge frame — 16-byte challenge now complete
            for (int i = 0; i < 8; i++) {
                m_immoChallenge[8 + i] = frame.data8[i];
            }
            immoLog16("challenge complete", m_immoChallenge);
            m_immoState      = ImmoState::ChallengeComplete;
            m_immoTimerTicks = 0;
        } else if (m_immoState == ImmoState::ResponseSent) {
            // BCM sent a single-frame quick re-check challenge
            immoLog8("quick challenge", frame.data8);
            uint8_t response[8] = {};
            if (computeImmoQuickResponse(frame.data8, response)) {
                sendImmoResponse(response);
            } else {
                efiPrintf("IMMO: quick challenge received but response algorithm not implemented");
            }
        }
    }

    /**
     * Periodic IMMO state-machine tick, called from sendPeriodic().
     * Manages delays and sends 0x0713 trigger/response at the right time.
     */
    void tickImmo() {
        // m74_9ImmoOff force-disables the whole handshake: no trigger, no
        // retries, no challenge processing (0x0714 frames are rejected in
        // acceptFrame() as well). The physical bypass answers the BCM instead.
        if (config->m74_9ImmoOff) {
            m_immoState = ImmoState::Idle;
            m_immoTimerTicks = 0;
            return;
        }

        m_immoTimerTicks++;
        // Convert 5ms ticks to ms
        uint32_t elapsedMs = m_immoTimerTicks * 5u;

        switch (m_immoState) {
        case ImmoState::WaitingToTrigger:
            if (elapsedMs >= IMMO_TRIGGER_DELAY_MS) {
                sendImmoTrigger();
                // log the session only on the initial send - retries below
                // would otherwise repeat this line every 500 ms forever
                efiPrintf("IMMO: sent 0x0713 trigger (session 0x%04x)", (unsigned)(m_immoSessionCounter - 1));
                m_immoState      = ImmoState::TriggerSent;
                m_immoTimerTicks = 0;
            }
            break;

        case ImmoState::TriggerSent:
            // If no challenge received within 500ms, retry trigger
            if (elapsedMs >= 500u) {
                sendImmoTrigger();
                m_immoTimerTicks = 0;
            }
            break;

        case ImmoState::Challenge1Rx:
            // If second frame does not arrive within the window, discard and retry
            if (elapsedMs >= IMMO_CHALLENGE_WINDOW_MS * 2u) {
                m_immoState      = ImmoState::TriggerSent;
                m_immoTimerTicks = 0;
            }
            break;

        case ImmoState::ChallengeComplete:
            if (elapsedMs >= IMMO_RESPONSE_DELAY_MS) {
                uint8_t response[8] = {};
                if (computeImmoResponse(m_immoChallenge, response)) {
                    sendImmoResponse(response);
                    m_immoState = ImmoState::ResponseSent;
                    efiPrintf("IMMO: authenticated");
                } else {
                    // Algorithm not yet implemented — log and stay in this state
                    // so it can be observed with the CAN sniffer.
                    efiPrintf("IMMO: challenge received but response algorithm not implemented");
                    m_immoState = ImmoState::TriggerSent;
                }
                m_immoTimerTicks = 0;
            }
            break;

        case ImmoState::Idle:
        case ImmoState::ResponseSent:
        default:
            break;
        }
    }

    /**
     * Send 0x0713 trigger frame.
     * The first frame ECU sends to initiate the IMMO session.  The exact
     * content is session-specific in original firmware (a counter + session
     * data); we use a simple rolling 16-bit counter that matches the
     * observed byte-6/byte-7 pattern from captures.
     */
    void sendImmoTrigger() {
        CanTxMessage msg(CanCategory::NBC, IMMO_ECU_ID, 8, DEFAULT_BUS_INDEX);
        // Rolling session counter in bytes 0-1 (observed pattern from captures)
        msg[0] = (uint8_t)(m_immoSessionCounter >> 8);
        msg[1] = (uint8_t)(m_immoSessionCounter & 0xFF);
        // Bytes 2-7: TODO — fill with real session data once algorithm is known.
        // For now use observed typical values from captures to look "alive".
        msg[2] = 0x00;
        msg[3] = 0x00;
        msg[4] = 0x00;
        msg[5] = 0x00;
        msg[6] = 0xC0;
        msg[7] = 0x00;
        m_immoSessionCounter++;
    }

    /**
     * Send 0x0713 response frame with the computed 8-byte answer.
     */
    void sendImmoResponse(const uint8_t response[8]) {
        CanTxMessage msg(CanCategory::NBC, IMMO_ECU_ID, 8, DEFAULT_BUS_INDEX);
        for (int i = 0; i < 8; i++) {
            msg[i] = response[i];
        }
        immoLog8("sent 0x0713 response", response);
    }

    // -----------------------------------------------------------------------
    // Transmit helpers - one method per CAN frame
    // -----------------------------------------------------------------------

    /**
     * 0x01F6 - ECU engine state (10 ms, 8 bytes)
     *   byte 0: 0x00
     *   byte 1: 0x00 (IGN on) / 0x20 (running)
     *   byte 2: 0x02 (ECU active, always) | 0x40 (cranking) | 0x80 (running)
     *   byte 3: 0x2D (fixed battery-derived constant)
     *   bytes 4-7: 0x00
     */
    void send0x01F6(bool isRunning, bool isCranking) {
        CanTxMessage msg(CanCategory::NBC, ECU_ENGINE_STATE_ID, 8, DEFAULT_BUS_INDEX);
        msg[0] = 0x00;
        msg[1] = isRunning ? 0x20 : 0x00;
        msg[2] = 0x02
               | (isCranking ? 0x40 : 0x00)
               | (isRunning  ? 0x80 : 0x00);
        msg[3] = 0x2D;  // approx battery-derived constant
        msg[4] = 0x00;
        msg[5] = 0x00;
        msg[6] = 0x00;
        msg[7] = 0x00;
    }

    /**
     * 0x0189 - RPM primary (10 ms, 8 bytes)
     *   bytes 0-1: RPM*16 big-endian (or 0x3200 baseline when engine stopped)
     *   bytes 2-3: battery voltage, little-endian millivolts (live)
     *   byte 4:   0x00 (no RPM) / rolling down-counter (running)
     *   byte 5:   0xB9 at IGN-only, 0xB8 while running (original captures)
     *   bytes 6-7: 0x00
     *
     * NOTE: bytes 0-1 must not be 0x0000 even when stopped — BCM uses the
     * non-zero baseline (0x3200) as "ECU calibration loaded" confirmation.
     * Sending 0x0000 causes BCM error state CF and blocks the starter relay.
     */
    void send0x0189(uint16_t rpmEncoded, bool isRunning) {
        CanTxMessage msg(CanCategory::NBC, ECU_RPM_PRIMARY_ID, 8, DEFAULT_BUS_INDEX);
        msg[0] = (uint8_t)(rpmEncoded >> 8);
        msg[1] = (uint8_t)(rpmEncoded & 0xFF);
        // battery voltage, little-endian millivolts (orig: 0x3220 = 12.83 V
        // at rest, 0x3CE9 = 15.59 V while charging)
        uint16_t vbattMv = encodeBatteryMillivoltsLE();
        msg[2] = (uint8_t)(vbattMv & 0xFF);
        msg[3] = (uint8_t)(vbattMv >> 8);
        // byte 4: rolling down-counter (original: F0->00 sawtooth while
        // running, 0x00 at rest) - the dash may use it as a frame-freshness
        // check; a frozen value makes the cluster treat the frames as stale.
        msg[4] = isRunning ? m_dashCounter : 0x00;
        m_dashCounter--;
        // byte 5: 0xB9 at IGN-only, 0xB8 while running (original captures)
        msg[5] = isRunning ? 0xB8 : 0xB9;
        msg[6] = 0x00;
        msg[7] = 0x00;
    }

    /**
     * 0x0186 - RPM aux / fuel flow (10 ms, 7 bytes)
     *   bytes 0-1: instantaneous fuel flow, mL/h big-endian (0 when stopped,
     *             ~0x026C = 620 mL/h at idle, 0 during DFCO)
     *   bytes 2-3: RPM*16 big-endian (or 0x3200 baseline when stopped)
     *   byte 4:   rpm - 768 (THE dash tach byte - exact linear fit vs the
     *             original captures: 775 rpm -> 0x07, 800 -> 0x20, 889 -> 0x79)
     *   byte 5:   0x00
     *   byte 6:   0x20
     *
     * NOTE: bytes 2-3 carry the same RPM baseline as 0x0189[0:1].
     */
    void send0x0186(uint16_t rpmEncoded, float rpm) {
        CanTxMessage msg(CanCategory::NBC, ECU_RPM_AUX_ID, 7, DEFAULT_BUS_INDEX);
        uint16_t fuelFlowMlPerHour = encodeFuelFlowMlPerHour();
        msg[0] = (uint8_t)(fuelFlowMlPerHour >> 8);
        msg[1] = (uint8_t)(fuelFlowMlPerHour & 0xFF);
        msg[2] = (uint8_t)(rpmEncoded >> 8);
        msg[3] = (uint8_t)(rpmEncoded & 0xFF);
        // dash tach: rpm/16 - 18, 0x20 at rest (see encodeTachByte)
        msg[4] = encodeTachByte(rpm);
        msg[5] = 0x00;
        msg[6] = 0x20;
    }

    /**
     * 0x018A - RPM aux2 (10 ms, 6 bytes)
     *   bytes 0-1: RPM*16 big-endian (or 0x3200 baseline when stopped)
     *   byte 2:   0x00
     *   byte 3:   0x07 at IGN-only, 0x06 while running (original captures)
     *   byte 4:   ignition advance: 256 - 2*advance while running, 0x02
     *             cranking, 0x00 at IGN-on (orig: 0xE2 = 15 deg idle,
     *             0xFA = 3 deg at the catch)
     *   byte 5:   0x00
     *
     * NOTE: byte 0 must be 0x32 even when stopped — same baseline rule as
     * 0x0189/0x0186.  Original sends 0x32 0x00 = 0x3200 at rest.
     */
    void send0x018A(uint16_t rpmEncoded, bool isRunning, bool isCranking, float timingAdvanceDeg) {
        CanTxMessage msg(CanCategory::NBC, ECU_RPM_AUX2_ID, 6, DEFAULT_BUS_INDEX);
        msg[0] = (uint8_t)(rpmEncoded >> 8);
        msg[1] = (uint8_t)(rpmEncoded & 0xFF);
        msg[2] = 0x00;
        // byte 3: 0x07 at IGN-only, 0x06 while running (original captures)
        msg[3] = isRunning ? 0x06 : 0x07;
        msg[4] = isRunning ? encodeIgnitionAdvanceByte(timingAdvanceDeg)
                           : (isCranking ? 0x02 : 0x00);
        msg[5] = 0x00;
    }

    /**
     * 0x0217 - ECU status (10 ms, 8 bytes)
     *   bytes 0-1: 0x00 0x00
     *   byte 2:   0x00 (IGN on only) / 0x70 (cranking or running)
     *   bytes 3-6: 0x00
     *   byte 7:   0x88 (constant)
     */
    void send0x0217(bool isEngineActive) {
        CanTxMessage msg(CanCategory::NBC, ECU_STATUS_ID, 8, DEFAULT_BUS_INDEX);
        msg[0] = 0x00;
        msg[1] = 0x00;
        msg[2] = isEngineActive ? 0x70 : 0x00;
        msg[3] = 0x00;
        msg[4] = 0x00;
        msg[5] = 0x00;
        msg[6] = 0x00;
        msg[7] = 0x88;
    }

    /**
     * 0x02A9 - Heartbeat (10 ms, 1 byte): 0xF4
     */
    void send0x02A9() {
        CanTxMessage msg(CanCategory::NBC, ECU_HEARTBEAT_ID, 1, DEFAULT_BUS_INDEX);
        msg[0] = 0xF4;
    }

    /**
     * 0x05E2 - ECU keepalive (1000 ms, 2 bytes): 0x00 0x00
     * Present in original firmware at ~1 Hz; absent from rusEFI caused BCM
     * to log an ECU communication fault on longer timescales.
     */
    void send0x05E2() {
        CanTxMessage msg(CanCategory::NBC, ECU_KEEPALIVE_ID, 2, DEFAULT_BUS_INDEX);
        msg[0] = 0x00;
        msg[1] = 0x00;
    }

    /**
     * 0x02C6 - Static frame (20 ms, 6 bytes): 0x00 0x00 0x00 0x40 0x00 0x00
     */
    void send0x02C6() {
        CanTxMessage msg(CanCategory::NBC, ECU_STATIC_ID, 6, DEFAULT_BUS_INDEX);
        msg[0] = 0x00;
        msg[1] = 0x00;
        msg[2] = 0x00;
        msg[3] = 0x40;
        msg[4] = 0x00;
        msg[5] = 0x00;
    }

    /**
     * Burst group (100 ms) - ECU identification and status frames.
     * Emulates the original ECU's 100 ms broadcast to keep BCM happy.
     */
    void sendBurst(bool isRunning) {
        // 0x05DA - ECU identification primary
        // byte 1: 0x89 (IGN on) / 0x9D (running)
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_05DA, 8, DEFAULT_BUS_INDEX);
            msg[0] = 0x41;
            msg[1] = isRunning ? 0x9D : 0x89;
            msg[2] = 0x00;
            msg[3] = 0x00;
            msg[4] = 0xB9;
            msg[5] = 0xFF;
            msg[6] = 0xFF;
            msg[7] = 0x00;
        }

        // 0x036E - Static identification
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_036E, 4, DEFAULT_BUS_INDEX);
            msg[0] = 0x09;
            msg[1] = 0x99;
            msg[2] = 0x09;
            msg[3] = 0x99;
        }

        // 0x03A5 - Static
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_03A5, 3, DEFAULT_BUS_INDEX);
            msg[0] = 0x00;
            msg[1] = 0x00;
            msg[2] = 0x00;
        }

        // 0x0511 - Static (7 bytes, all zeros)
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_0511, 7, DEFAULT_BUS_INDEX);
            msg[0] = 0x00;
            msg[1] = 0x00;
            msg[2] = 0x00;
            msg[3] = 0x00;
            msg[4] = 0x00;
            msg[5] = 0x00;
            msg[6] = 0x00;
        }

        // 0x06FD - ECU mode indicator
        // byte 3: 0x20 (IGN on) / 0x00 (running)
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_06FD, 4, DEFAULT_BUS_INDEX);
            msg[0] = 0x00;
            msg[1] = 0x00;
            msg[2] = 0x99;
            msg[3] = isRunning ? 0x00 : 0x20;
        }

        // 0x0648 - Static calibration marker
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_0648, 8, DEFAULT_BUS_INDEX);
            msg[0] = 0xC3;
            msg[1] = 0xFF;
            msg[2] = 0xF0;
            msg[3] = 0x00;
            msg[4] = 0x00;
            msg[5] = 0x7F;
            msg[6] = 0xFE;
            msg[7] = 0x18;
        }

        // 0x041A - ECU init status
        // IGN on: {0x00, 0x03, 0x00, ...}  running: all zeros
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_041A, 8, DEFAULT_BUS_INDEX);
            msg[0] = 0x00;
            msg[1] = isRunning ? 0x00 : 0x03;
            msg[2] = 0x00;
            msg[3] = 0x00;
            msg[4] = 0x00;
            msg[5] = 0x00;
            msg[6] = 0x00;
            msg[7] = 0x00;
        }

        // 0x066A - ECU operating state + coolant temperature for the dash
        // bytes 3-4: CLT degC (both bytes carry the same value - orig traces
        // show the warm-up ramp 53->95 degC saturating at the thermostat,
        // frozen after shutdown)
        // IGN on:  {0x08, 0xFF, 0x00, <clt>, <clt>, 0xC0, 0x00, 0x00}
        // running: {0x00, 0xFF, 0x00, <clt>, <clt>, 0xC0, 0x00, 0x00}
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_066A, 8, DEFAULT_BUS_INDEX);
            msg[0] = isRunning ? 0x00 : 0x08;
            msg[1] = 0xFF;
            msg[2] = 0x00;
            uint8_t cltDegC = encodeCltDegC();
            msg[3] = cltDegC;
            msg[4] = cltDegC;
            msg[5] = 0xC0;
            msg[6] = 0x00;
            msg[7] = 0x00;
        }

        // 0x0522 - Static (all zeros)
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_0522, 8, DEFAULT_BUS_INDEX);
            msg[0] = 0x00;
            msg[1] = 0x00;
            msg[2] = 0x00;
            msg[3] = 0x00;
            msg[4] = 0x00;
            msg[5] = 0x00;
            msg[6] = 0x00;
            msg[7] = 0x00;
        }

        // 0x065C - Calibration checksum indicator
        // IGN on: {0x7A, 0xFF, 0x00}  running: {0x78, 0xFF, 0x00}
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_065C, 3, DEFAULT_BUS_INDEX);
            msg[0] = isRunning ? 0x78 : 0x7A;
            msg[1] = 0xFF;
            msg[2] = 0x00;
        }

        // 0x041D - Static identification tail
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_041D, 4, DEFAULT_BUS_INDEX);
            msg[0] = 0x48;
            msg[1] = 0xFE;
            msg[2] = 0xFF;
            msg[3] = 0x00;
        }
    }

    // -----------------------------------------------------------------------
    // Periodic scheduler (called from request() every 5 ms)
    // -----------------------------------------------------------------------

    void sendPeriodic() {
        // Stock behavior: with ignition off the ECU's CAN node is COMPLETELY
        // silent (verified in ignoff.trc - zero ECU frames of any kind; the
        // remaining traffic is dash/cluster chatter). The BCM drops the main
        // relay when the 0x0189 heartbeat dies - if we keep streaming with
        // the key off, the BCM treats the ECU as alive and the relay never
        // drops. So the whole dash/IMMO/keepalive stream stops until ignition
        // returns. The console ISO-TP path is a separate protocol (rx-driven)
        // and is unaffected. isIgnVoltage() reads the L9779 KEY_ON cache
        // (DIA_REG9), false for the first ~100 ms after boot - safe default.
        if (!isIgnVoltage()) {
            return;
        }

        // Snapshot engine state once per call. The dash rpm is the LIVE sensor
        // value - exactly what the Java console shows, no holding/filtering:
        // the console rpm has no dropouts, so any dash dropout is an encoding
        // problem, not a sensor problem.
        float rpm = Sensor::getOrZero(SensorType::Rpm);
        bool isRunning      = (rpm > engineConfiguration->cranking.rpm);
        bool isCranking     = (rpm > 0) && !isRunning;
        bool isEngineActive = isRunning || isCranking;

        // Use idle-target baseline (0x3200) when engine is not rotating.
        // BCM checks this field as "ECU calibration loaded" confirmation;
        // 0x0000 triggers BCM error state CF and blocks the starter relay.
        uint16_t rpmEncoded = encodeRpmOrBaseline(rpm);

        // Snapshot the current spark advance once per call (used by 0x018A[4])
        float timingAdvanceDeg = engine->engineState.timingAdvance[0];

        // IMMO state machine: tick every 5ms call. Kept outside the serial-session gate
        // below so the one-shot trigger/response handshake still completes even while
        // TS is connected (only 2 frames per ignition-on cycle, negligible traffic).
        tickImmo();

        		// While a serial (ISO-TP/TS) session is active:
        		//   - engine off: mute the BCM emulation entirely. It sends ~660 frames/s,
        		//     which contends with TS responses for CAN mailboxes (truncated
        		//     multi-frame responses = "Got only N bytes" on the host) and floods
        		//     the host's CAN reader during a tune write.
        		//   - engine running: the BCM needs these frames (IMMO/fuel pump/dash),
        		//     so keep sending, but at 1/4 rate. The serial response bursts are
        		//     ~150 frames per output-channels poll; at full flood rate the BCM
        		//     frames keep the 3 TX mailboxes occupied through the serial frames'
        		//     1s per-frame transmit timeout and the burst dies mid-stream (the
        		//     "gauges dead while the engine runs" failure - the ECU keeps
		//     sending BCM frames, only the 720 responses stop). 1/4 rate is ~165
		//     frames/s, which the dash/IMMO/pump logic tolerates for the 10 s
		//     pause window.
        		bool serialSessionActive = (engine->pauseCANdueToSerialUntil > getTimeNowNt());
        		if (serialSessionActive) {
        			if (!isEngineActive) {
        				return;
        			}
        			if ((m_counter & 3) != 0) {
        				return;
        			}
        		}

        // 10 ms group: every 2nd tick
        if ((m_counter % 2) == 0) {
            send0x01F6(isRunning, isCranking);
            send0x0189(rpmEncoded, isRunning);
            send0x0186(rpmEncoded, rpm);
            send0x018A(rpmEncoded, isRunning, isCranking, timingAdvanceDeg);
            send0x0217(isEngineActive);
            send0x02A9();
        }

        // 20 ms group: every 4th tick
        if ((m_counter % 4) == 0) {
            send0x02C6();
        }

        // 100 ms burst group: every 20th tick
        if ((m_counter % 20) == 0) {
            sendBurst(isRunning);
        }

        // 1000 ms keepalive: every 200th tick
        if ((m_counter % 200) == 0) {
            send0x05E2();
        }
    }

    // -----------------------------------------------------------------------
    // State
    // -----------------------------------------------------------------------

    // True when BCM signals starter relay active (0x0350 byte 4 >= 0x80)
    bool m_starterActive = false;
    // True when BCM relay status frame confirms relay active (0x0303 byte 5 bit 1)
    bool m_relayActive   = false;

    // Counter incremented once per 5 ms call to request(); drives timing logic
    uint32_t m_counter = 0;

    // Rolling down-counter embedded in 0x0189[4] while running (original F0->00
    // sawtooth) - the cluster may use it as a frame-freshness check.
    uint8_t m_dashCounter = 0xF0;

    // -----------------------------------------------------------------------
    // IMMO state
    // -----------------------------------------------------------------------
    ImmoState m_immoState        = ImmoState::Idle;
    // Ticks elapsed in current IMMO state (1 tick = 5ms)
    uint32_t  m_immoTimerTicks   = 0;
    // Rolling session counter embedded in 0x0713 trigger frames
    uint16_t  m_immoSessionCounter = 0;
    // Buffer for the 16-byte BCM challenge (two consecutive 0x0714 frames)
    uint8_t   m_immoChallenge[16] = {};
};

// ---------------------------------------------------------------------------
// Module-level static instance (no heap allocation)
// ---------------------------------------------------------------------------

static M74_9BcmCanListener m74_9BcmListener;

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

bool m74_9_bcmStarterActive() {
    return m74_9BcmListener.isStarterActive();
}

bool m74_9_immoAuthenticated() {
    return m74_9BcmListener.isImmoAuthenticated();
}

bool m74_9_isImmobilizerBlocking() {
    // The ECU never blocks fuel/ignition on IMMO today: with m74_9ImmoOff the
    // handshake is disabled entirely (physical bypass answers the BCM), and even
    // with it enabled computeImmoResponse() is not implemented yet, so blocking
    // here would prevent every start.
    //
    // When the algorithm is ready, restore:
    //   return engineConfiguration->m74_9ImmoEnabled
    //          && !m74_9BcmListener.isImmoAuthenticated();
    return false;
}

void initM74_9Can() {
    registerCanListener(m74_9BcmListener);
    addConsoleActionS("cantach", setTachEncoding);
}

#endif // EFI_CAN_SUPPORT
