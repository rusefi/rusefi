/**
 * @file m74_9_vr_model.h
 *
 * VR amplitude model for the L9779 auto-adaptive hysteresis (datasheet 6.14):
 * the chip quantizes the VR peak voltage into 5 levels (PV1..PV4: 930/1600/
 * 2300/3000 mV, Table 42) and selects the hysteresis current per level
 * (HI1..HI5: 5/10/17/32/51 uA, Table 43 - 50/100/173/322/510 mV on this
 * board's 10k resistor). The VR peak scales with rpm (Vp = k * rpm), so with
 * a single calibration scalar k the firmware can predict, from rpm alone:
 *
 *  - the hysteresis level the chip is in,
 *  - the expected missing-teeth gap shift per level,
 *  - the eat-risk: Vp sitting near a PV quantization boundary is where the
 *    auto-H can momentarily reach the local amplitude and eat a tooth.
 *
 * The per-level shift TABLE is the correction state (the model is the
 * DEFAULT): the decoder trains it from the per-sync gap measurement (weak
 * hook triggerObserveGapShift, alpha 0.1) and applies table[level] as the
 * scheduling anchor correction. The table + k persist to MFS on engine stop
 * (EFI_VR_MODEL_RECORD_ID, magic/version/CRC), loaded at boot - after the
 * first drive the car starts already calibrated.
 *
 * k is set with the 'vrk <mV/rpm>' console command. Calibration: one bench
 * measurement of the VR peak at a known rpm, or a fit from the measured gap
 * shift. The eat-risk flag self-validates k: eats should correlate with
 * risk=YES.
 */

#pragma once

// VR peak voltage = vrAmplitudePerRpm * rpm (mV).
float vrAmplitudePerRpmGet();

// hysteresis level index (0..4, HI1..HI5) the chip's quantizer selects for
// Vp = k * rpm. 0 when k is unset (all learning goes into entry 0).
int vrLevelForRpm(float rpm);

// hysteresis voltage (mV on this board's 10k resistor) for a level 0..4.
float vrHysteresisMvForLevel(int level);

// the model's expected missing-teeth gap shift (pitch units) for the current
// rpm: the learned per-level table value. Always >= 0 (seeded 0.63).
float vrExpectedShiftPitchForRpm(float rpm);

// eat-risk proximity (0..1): how close Vp is to the NEAREST PV quantization
// boundary, relative to that boundary. 1 = exactly on a boundary (highest
// predicted eat risk), 0 = far from any boundary. 0 when k is unset.
float vrBoundaryProximityForRpm(float rpm);

// console commands
void m74_9VrModel();
void m74_9VrModelSetK(const char* arg);

// console command: reset the learned model back to seed (shift 0.63 pitch,
// k=0, rev counters cleared) and persist the reset so it survives a reboot.
void m74_9VrModelReset();

// periodic slow callback: one boot read request + save-on-stop.
void m74_9VrModelPeriodic();
