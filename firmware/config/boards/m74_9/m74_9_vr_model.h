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
 *  - the expected missing-teeth gap shift (the spurious-edge signature is a
 *    function of the level: the auto-H tracks amplitude, so the shift is
 *    constant in angle within a level),
 *  - the eat-risk: Vp sitting near a PV quantization boundary is where the
 *    auto-H can momentarily reach the local amplitude and eat a tooth.
 *
 * k lives in RAM, set with the 'vrk <mV/rpm>' console command (0 = model
 * off). Calibration: one bench measurement of the VR peak at a known rpm, or
 * a fit from the measured gap shift. Promoting k to a persisted tune field is
 * the follow-up once the model is validated on the car.
 */

#pragma once

// VR peak voltage = vrAmplitudePerRpm * rpm (mV). 0 = model disabled.
float vrAmplitudePerRpmGet();

// hysteresis level index (0..4, HI1..HI5) the chip's quantizer selects for
// Vp = k * rpm. 0 when the model is off.
int vrLevelForRpm(float rpm);

// hysteresis voltage (mV on this board's 10k resistor) for a level 0..4.
float vrHysteresisMvForLevel(int level);

// expected missing-teeth gap shift (pitch units) for the current rpm:
// the spurious-edge signature per hysteresis level. Negative = model off.
float vrExpectedShiftPitchForRpm(float rpm);

// eat-risk proximity (0..1): how close Vp is to the NEAREST PV quantization
// boundary, relative to that boundary. 1 = exactly on a boundary (highest
// predicted eat risk), 0 = far from any boundary. 0 when the model is off.
float vrBoundaryProximityForRpm(float rpm);

// console commands
void m74_9VrModel();
void m74_9VrModelSetK(const char* arg);
