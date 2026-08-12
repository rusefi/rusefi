# rusEFI Lua examples

General reference: https://wiki.rusefi.com/Lua-Scripting

This folder is a grab-bag of community Lua snippets that talk to the rusEFI Lua API
(CAN RX/TX, sensors, outputs, PWM, timers, PID, TunerStudio buttons, etc.). When
asked to write a new Lua script, **scan this folder first** for an analogous
example before starting from scratch. Files use `.lua` or `.txt` — both are plain
Lua; the `.txt` extension is historical.

For iterating on a live or simulated ECU from an LLM, see
`java_console/mcp_ecu/README.md` (the `:mcp_ecu` MCP server: `set_lua`,
`lua_reset`, `wait_for_message`, …).

---

## Gotchas (read before writing a script)

- **Lua-fed sensors time out — refresh them every `onTick`, not on a slow
  gated cadence.** A `Sensor.new(...)` value goes invalid `setTimeout(ms)`
  after the last `:set()` (see `StoredValueSensor::get()` in
  `controllers/sensors/core/stored_value_sensor.h`). If you only call `:set()`
  from a block gated to, say, every 2s while the timeout is 3s, you have ~1s
  of slack — any tick jitter pushes the gap past the timeout and the consumer
  briefly reads 0/invalid, then your next update restores it (gauges that
  "drop to 0 and jump back"). Push `:set()` unconditionally each tick and gate
  only the *value computation* (e.g. a sweep counter). Or, at minimum, set the
  timeout well above the worst-case refresh interval.
- **Failed `txCan` stalls the Lua thread.** On a bench with no second node to
  ACK (or a mis-wired/unterminated bus), transmits fail and `txCan` blocks
  until timeout; the `canWriteNotOk` output channel climbs. Many `txCan` calls
  per tick at a high `setTickRate(...)` can then delay `onTick` by seconds —
  which is exactly what trips the sensor-timeout gotcha above. Symptom is
  environmental (gone on a live, terminated bus) but design scripts to tolerate
  it.

Related (full engine configs, not just snippets):
- VAG CAN producer — `firmware/config/engines/vw_b6.lua`
- GM CAN producer — `firmware/config/engines/gm_ls_4.cpp`

---

## Categories

### Getting started / building blocks
- `pid.lua` — minimal `Pid.new(...)` usage; a good "hello PID" template.
- `ts-button-example.lua` — wire a TunerStudio button (`fireEvent`) to a Lua
  handler. Reference for any UI-triggered action.
- `etb-test-script.lua` — one-liner to drive the electronic throttle manually.
- `dac.txt` — write an analog voltage via the on-board DAC.
- `analog-gear-input.txt` — decode a gear position from a single analog input
  (voltage windows → gear number).
- `linear-oil-sensor.lua` — convert a linear oil-pressure sensor voltage to a
  rusEFI sensor reading.

### Utilities / helpers (good to copy-paste into other scripts)
- `utils-print-array.lua` — pretty-print a Lua array (debug helper).
- `utils-can-dbc.lua` — bit-field pack/unpack helpers in the spirit of DBC
  signal definitions; the cleanest reference for building CAN payloads.
- `utils-dash-sweep.lua` / `dash-sweep.lua` — sweep gauge needles on power-up
  (a "self-test" animation for digital dashes).

### Launch control / antilag / quickshift
- `launch_control.lua` — full-featured launch control with CAN status frames,
  ignition cut and configurable thresholds. The most complete example in this
  folder.
- `custom-launch-control.txt` — simpler launch-control sketch.
- `CruiseCheck.lua` — cruise-control safety / engagement checks.
- `ALS (simple antilag strategy).txt` and the companion
  `ALS (simple antilag strategy)LUA_MAPS.txt` — minimal antilag with Lua-side
  fuel/ignition maps.
- `quickshift.txt` — flat-shift / quickshifter ignition-cut on a digital input.

### Drive-by-wire / actuators
- `DBW-controller.txt` — closed-loop electronic throttle controller written in
  Lua (useful when the built-in ETB is not enough).
- `can-client-dc-pid.txt` — PID-controlled DC motor over CAN.
- `Volvo-electric-power-steering-pump.txt` — drive a Volvo EPS pump via PWM /
  CAN.

### CAN: man-in-the-middle and gateways
"Cut a CAN bus and put rusEFI in the middle" patterns — rusEFI receives frames
on one bus and re-transmits modified copies, typically to spoof RPM / TPS / etc.
to a downstream module.
- `man-in-the-middle.txt` — generic MITM template, start here.
- `man-in-the-middle-nissan-tcu.txt`, `nissan-xterra-tcu.txt`,
  `nissan-tcu.lua` — feed a Nissan TCU with fabricated ECU frames so the
  factory transmission keeps working behind rusEFI.
- `TCU-4-speed.txt` — a fuller standalone 4-speed TCU written in Lua.
- `vag-headlight.txt` — VAG headlight / lighting CAN MITM.

### CAN: VAG / VW transport protocol (TP 2.0)
- `vw-tp.txt` — VW/Audi TP 2.0 transport layer in Lua (channel setup,
  segmented messages).
- `vw-tp-tcu.txt` — TP 2.0 talking to a VAG TCU.
- `vw-sensors-from-ptbus-pq46.txt` — read sensor values off a PQ46 powertrain
  bus.

### CAN: dashboards / instrument clusters
Drive a factory cluster from rusEFI by emitting the frames it expects.
- `suzuki_can_dash.lua` — Suzuki cluster.
- `dash_output_abit_m11.txt`, `dash_output_abit_m11.16txt` — Abit M11
  aftermarket dash.
- `uaDASH-dev.lua` — uaDASH (rusEFI / community digital dash) output frames.
- `bmw-idrive.txt` — drive BMW iDrive / cluster signals.

### CAN: body / comfort modules (BCM)
- `honda-bcm.txt` — Honda body control module signalling.
- `nissan-350z-bcm.txt`, `350z-ac.txt` — Nissan 350Z BCM + A/C request
  handling.
- `ford-focus-ii-pps.txt` — Ford Focus II accelerator-pedal (PPS) decode from
  CAN.
- `polaris_slingshot.txt` — Polaris Slingshot bus integration.

### CAN: peripherals / smart sensors / dynos
- `motec-ltc-input.lua` — read a MoTeC LTC wideband over CAN.
- `Dyno-Dynamics.lua`, `ECU-to-DynoLog.txt` — exchange data with Dyno
  Dynamics / DynoLog dynamometers.
- `canopen-keypad.lua` — read a CANopen keypad (button matrix over CAN).
- `cantcu.lua` — talk to a generic CAN-controlled TCU.
- `can-adc-server.txt` — expose ADC readings as a CAN server (let other nodes
  ask rusEFI for analog values).
- `gdi4-communication.lua` — communicate with the GDI4 (4-channel direct-
  injection driver) over CAN; the most detailed GDI example here.
- `gdi8-pressure-input.lua` — read fuel-rail pressure reported by a GDI8
  driver.
- `msio-device.lua` — talk to an MSIO expansion board.

### Diagnostics / programming protocols
- `uds-prog.txt` — UDS (ISO 14229) programming session sketch.
- `xcp-prototyping.txt` — XCP measurement/calibration protocol prototype.
- `ms11bit.txt`, `ms29bit.txt` — raw 11-bit vs 29-bit CAN ID handling demos.
