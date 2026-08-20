/**
 * @file m74_9_tooth_diag.h
 *
 * m74_9 trigger-tooth diagnostics and learning:
 *  - 'toothdump' console command: prints the learned 58-tooth time profile
 *    and the last few raw revolutions (paste into the chat, plot externally).
 *  - 'toothsave' console command: force-save the learned profile to MFS.
 *  - 'rawtrg' console command: prints the RAW primary-trigger edge stream
 *    (every edge the MCU sees, BEFORE the debounce and the decoder) as
 *    inter-edge deltas with a histogram - a digital oscilloscope of the
 *    comparator output for noise-storm diagnosis.
 *  - 'synctrace' console command: prints the decoder's last sync/desync
 *    events (kind, event-count error, gap ratios, rpm) plus the
 *    debounce/noise-filter drop counters - the decoder-side view that
 *    correlates with 'rawtrg' (the pin-side view).
 *  - m74_9ToothPeriodic(): slow-callback housekeeping - lazy load of the
 *    stored profile and auto-save when the engine stops (like the stock ECU
 *    stores its learned wheel data in non-volatile memory).
 *
 * See m74_9_tooth_diag.cpp.
 */

#pragma once

void m74_9ToothDump();
void m74_9ToothSave();
void m74_9ToothPeriodic();
void m74_9RawTriggerDump();
void m74_9SyncTrace();
// 'timecheck' console command: prints systick ms + NT clock ms + raw TIM5->CNT
// in one line for cross-checking the two timebases.
void m74_9TimeCheck();
// RPM-adaptive trigger input debounce for custom_board_triggerDebounceUs.
float m74_9TriggerDebounceUs();
