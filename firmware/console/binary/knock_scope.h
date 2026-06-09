/**
 * @file knock_scope.h
 * @brief Streams raw knock-channel ADC samples to the host for off-MCU FFT/spectrogram.
 *
 * Streams the same per-cylinder crank-angle windows that software_knock already samples.
 * Activated by `engineConfiguration->enableKnockScope` AND build-time `KNOCK_SCOPE` define.
 */

#pragma once

#include <cstddef>
#include <cstdint>

class TsChannelBase;

void knockScopeEnable();
void knockScopeDisable();
void knockScopePublishWindow(const adcsample_t* samples, size_t count, uint8_t cylinderNumber, uint8_t channelNumber);

/**
 * Stream all pending ring frames to the host (batch header v2 + frames). No extra RAM buffer.
 * @return false when the ring is empty (host should treat as not ready).
 */
bool knockScopeSendPending(TsChannelBase* tsChannel);

bool isKnockScopeActive();

void initKnockScope();
