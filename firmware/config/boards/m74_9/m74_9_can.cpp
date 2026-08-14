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
        return m_immoState == ImmoState::ResponseSent;
    }

    // Accept BCM state/relay frames AND IMMO challenge frames
    bool acceptFrame(const size_t busIndex, const CANRxFrame& frame) const override {
        (void)busIndex;
        uint32_t id = CAN_ID(frame);
        return (id == BCM_STATE_ID) || (id == BCM_RELAY_ID) || (id == IMMO_BCM_ID);
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
            if (ignitionActive && (m_immoState == ImmoState::Idle) && isImmoEnabled()) {
                m_immoState        = ImmoState::WaitingToTrigger;
                m_immoTimerTicks   = 0;
            }
        } else if (id == BCM_RELAY_ID) {
            // byte 5 bit 1 (0x02): starter relay active
            m_relayActive = ((frame.data8[5] & 0x02) != 0);
        } else if (id == IMMO_BCM_ID) {
            if (isImmoEnabled()) {
                handleImmoChallengeFrame(frame, nowNt);
            }
        }
    }

private:
    // -----------------------------------------------------------------------
    // IMMO enable logic
    // -----------------------------------------------------------------------

    bool isImmoEnabled() const {
        return engineConfiguration->m74_9ImmoEnabled
               && !engineConfiguration->m74_9ImmoOff;
    }

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
        m_immoTimerTicks++;
        // Convert 5ms ticks to ms
        uint32_t elapsedMs = m_immoTimerTicks * 5u;

        switch (m_immoState) {
        case ImmoState::WaitingToTrigger:
            if (elapsedMs >= IMMO_TRIGGER_DELAY_MS) {
                sendImmoTrigger();
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
        efiPrintf("IMMO: sent 0x0713 trigger (session 0x%04x)", (unsigned)(m_immoSessionCounter-1));
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
     *   bytes 2-3: 0x20, 0x32 (static idle approximation)
     *   byte 4:   0x00 (no RPM) / 0x70 (running)
     *   byte 5:   0xB9 (fixed CLT-derived approximation)
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
        msg[2] = 0x20;
        msg[3] = 0x32;
        msg[4] = isRunning ? 0x70 : 0x00;
        msg[5] = 0xB9;
        msg[6] = 0x00;
        msg[7] = 0x00;
    }

    /**
     * 0x0186 - RPM aux (10 ms, 7 bytes)
     *   bytes 0-1: 0x00 0x00 (IGN on) / 0x28 0x1C (engine active) - static
     *   bytes 2-3: RPM*16 big-endian (or 0x3200 baseline when stopped)
     *   byte 4:   0x20
     *   byte 5:   0x00
     *   byte 6:   0x20
     *
     * NOTE: bytes 2-3 carry the same RPM baseline as 0x0189[0:1].
     */
    void send0x0186(uint16_t rpmEncoded, bool isEngineActive) {
        CanTxMessage msg(CanCategory::NBC, ECU_RPM_AUX_ID, 7, DEFAULT_BUS_INDEX);
        msg[0] = isEngineActive ? 0x28 : 0x00;
        msg[1] = isEngineActive ? 0x1C : 0x00;
        msg[2] = (uint8_t)(rpmEncoded >> 8);
        msg[3] = (uint8_t)(rpmEncoded & 0xFF);
        msg[4] = 0x20;
        msg[5] = 0x00;
        msg[6] = 0x20;
    }

    /**
     * 0x018A - RPM aux2 (10 ms, 6 bytes)
     *   bytes 0-1: RPM*16 big-endian (or 0x3200 baseline when stopped)
     *   byte 2:   0x00
     *   byte 3:   0x06
     *   byte 4:   0xFE (IGN on) / 0xE8 (running)
     *   byte 5:   0x00
     *
     * NOTE: byte 0 must be 0x32 even when stopped — same baseline rule as
     * 0x0189/0x0186.  Original sends 0x32 0x00 = 0x3200 at rest.
     */
    void send0x018A(uint16_t rpmEncoded, bool isRunning) {
        CanTxMessage msg(CanCategory::NBC, ECU_RPM_AUX2_ID, 6, DEFAULT_BUS_INDEX);
        msg[0] = (uint8_t)(rpmEncoded >> 8);
        msg[1] = (uint8_t)(rpmEncoded & 0xFF);
        msg[2] = 0x00;
        msg[3] = 0x06;
        msg[4] = isRunning ? 0xE8 : 0xFE;
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

        // 0x066A - ECU operating state
        // IGN on:  {0x08, 0xFF, 0x00, 0x01, 0x01, 0xC0, 0x00, 0x00}
        // running: {0x00, 0xFF, 0x00, 0x1D, 0x1D, 0xC0, 0x00, 0x00}
        {
            CanTxMessage msg(CanCategory::NBC, BURST_ID_066A, 8, DEFAULT_BUS_INDEX);
            msg[0] = isRunning ? 0x00 : 0x08;
            msg[1] = 0xFF;
            msg[2] = 0x00;
            msg[3] = isRunning ? 0x1D : 0x01;
            msg[4] = isRunning ? 0x1D : 0x01;
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
        // Snapshot engine state once per call
        float rpm = Sensor::getOrZero(SensorType::Rpm);
        bool isRunning      = (rpm > engineConfiguration->cranking.rpm);
        bool isCranking     = (rpm > 0) && !isRunning;
        bool isEngineActive = isRunning || isCranking;

        // Use idle-target baseline (0x3200) when engine is not rotating.
        // BCM checks this field as "ECU calibration loaded" confirmation;
        // 0x0000 triggers BCM error state CF and blocks the starter relay.
        uint16_t rpmEncoded = encodeRpmOrBaseline(rpm);

        // IMMO state machine: tick every 5ms call
        tickImmo();

        // 10 ms group: every 2nd tick
        if ((m_counter % 2) == 0) {
            send0x01F6(isRunning, isCranking);
            send0x0189(rpmEncoded, isRunning);
            send0x0186(rpmEncoded, isEngineActive);
            send0x018A(rpmEncoded, isRunning);
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
    return engineConfiguration->m74_9ImmoEnabled
           && !engineConfiguration->m74_9ImmoOff
           && !m74_9BcmListener.isImmoAuthenticated();
}

void initM74_9Can() {
    registerCanListener(m74_9BcmListener);
}

#endif // EFI_CAN_SUPPORT
