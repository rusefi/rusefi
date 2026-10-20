# VVT control

`VvtController` uses separate intake and exhaust target tables, PID settings and
advanced curves, shared across banks. It runs from the fast callback. RPM,
coolant temperature, elapsed time after engine start and cam feedback gate
control. `vvtControlMinRpm` is independent of the cranking threshold.
Cam feedback is valid for one second after a synchronized position update.
If no position has arrived or cam edges stop, the controller disables PWM and
resets PID state until feedback resumes.

## Basic and advanced modes

Basic mode uses the configured PID offset and P/I/D gains. Its rest-position
hysteresis enables an advancing cam above 3 degrees and disables below 1;
retarding cams use the corresponding negative thresholds.

[tag:vvt_advanced_control] `vvtAdvancedModeEnabled` bypasses only that target
hysteresis. The oil-pressure curve supplies base duty, and the signed error
curve supplies the P gain. Error is `(target - observation)` multiplied by -1
for inverted/retarding control. Missing or invalid oil pressure selects the
curve value at 0 kPa. Intake and exhaust use separate nine-point error curves
and six-point pressure curves.

`Pid::getUnclampedOutputWithPFactor()` preserves normal I/D and error history
without modifying calibration. VVT subtracts the fixed PID offset, adds the
curve's base duty and clamps the total to the existing output limits. The PID
trim runs continuously; there is no fade or pause window. Ordinary PID callers
retain their existing clamping and conditional-integration behavior.

The scheduled P contribution and final bounded command replace the corresponding
PID status fields. `vvtDistance` reports signed advanced-mode error and is zero
in basic mode. Failed setpoint/feedback disables PWM and resets PID state.

## Configuration and defaults

Fields are on the main configuration page, included from
`controllers/actuators/vvt_advanced_engineConfiguration.txt`. Advanced mode is
disabled by default, with zero duty and P-gain values. Migration initializes
missing axes to -40..40 degrees and 0..1000 kPa. TunerStudio displays P gains
multiplied by 100; stored values are the actual gains.

Intake/exhaust integral limits apply in both modes. A zero/zero pair initializes
to -1000/+1000 during defaults repair; one-sided zero bounds remain valid.
The shared PID's existing integral guard also applies. Changing modes or
integral bounds resets PID state; any burn while advanced mode is enabled resets
the trim and target hysteresis so old curve history is discarded.

`unit_tests/tests/actuators/test_vvt.cpp` covers defaults, compatibility, control
direction, bank/cam selection, fallback pressure, integral limits, activation
gates, output clamping and PWM. `unit_tests/tests/test_pid.cpp` covers the shared
scheduled-P API and preservation of existing PID policies.
