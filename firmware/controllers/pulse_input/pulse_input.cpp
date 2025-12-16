#include "pulse_input.h"
#include "os_access.h"
#include "hal.h"

// ----------------------------
// ISR-shared state
// ----------------------------
static volatile uint32_t lastEdgeUs = 0;
static volatile uint32_t highTimeAccumUs = 0;
static volatile uint32_t totalTimeAccumUs = 0;
static volatile bool lastLevel = false;

static ioportid_t pulsePort = nullptr;
static uint8_t pulsePad = 0;
static brain_pin_e activePin = BRAIN_PIN_UNASSIGNED;

// ----------------------------
// Time helper
// ----------------------------
static inline uint32_t nowUs() {
    return NT2US(chVTGetSystemTimeX());
}

// ----------------------------
// EXTI ISR callback
// ----------------------------
static void pulseExtiCallback(void *) {
    chSysLockFromISR();

    const uint32_t now = nowUs();
    const bool level = palReadPad(pulsePort, pulsePad);

    if (lastEdgeUs != 0) {
        uint32_t delta = now - lastEdgeUs;  // rollover-safe
        totalTimeAccumUs += delta;
        if (lastLevel) {
            highTimeAccumUs += delta;
        }
    }

    lastLevel = level;
    lastEdgeUs = now;

    chSysUnlockFromISR();
}

// ----------------------------
// Initialize pulse input
// ----------------------------
void pulseInputInit() {
    brain_pin_e pin = engineConfiguration->pulseInputPin;
    if (pin == BRAIN_PIN_UNASSIGNED) {
        return;
    }

    // Disable old EXTI if changing pins
    if (activePin != BRAIN_PIN_UNASSIGNED && activePin != pin) {
        const auto oldIo = pinToIoPortAndPad(activePin);
        palDisableLineEvent(oldIo.port, oldIo.pad);
    }

    activePin = pin;

    const auto io = pinToIoPortAndPad(pin);
    pulsePort = io.port;
    pulsePad = io.pad;

    // Reset state
    chSysLock();
    lastEdgeUs = 0;
    highTimeAccumUs = 0;
    totalTimeAccumUs = 0;
    lastLevel = palReadPad(pulsePort, pulsePad);
    chSysUnlock();

    // Enable EXTI on both edges
    palEnableLineEvent(pulsePort, pulsePad, PAL_EVENT_MODE_BOTH_EDGES);
    palSetLineCallback(pulsePort, pulsePad, pulseExtiCallback, nullptr);
}

// ----------------------------
// Fast-loop updater
// ----------------------------
void pulseInputUpdate() {
    uint32_t windowUs = engineConfiguration->pulseMeasureWindowMs * 1000;

    uint32_t hi, tot;

    // Atomic snapshot
    chSysLock();
    hi  = highTimeAccumUs;
    tot = totalTimeAccumUs;
    chSysUnlock();

    if (tot == 0 || windowUs == 0) {
        engine->engineState.pulseAvgWidthUs = 0;
        engine->engineState.pulseDutyCycle  = 0;
        return;
    }

    // Publish results
    engine->engineState.pulseAvgWidthUs = static_cast<float>(hi);
    engine->engineState.pulseDutyCycle  = 100.0f * static_cast<float>(hi) / static_cast<float>(tot);

    // Reset accumulators if window exceeded
    if (tot >= windowUs) {
        chSysLock();
        highTimeAccumUs = 0;
        totalTimeAccumUs = 0;
        lastEdgeUs = nowUs();
        chSysUnlock();
    }
}