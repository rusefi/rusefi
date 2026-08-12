# Engine Presets ("Canned Tunes")

An engine preset (informally a "canned tune") is C++ code baked into the firmware that
populates `engineConfiguration` with sensible defaults for a specific engine. It is applied
whenever the user selects the matching `engine_type_e` value in TunerStudio, and again on a
"reset to defaults". Presets are the firmware-side counterpart to a TunerStudio `.msq` tune:
the `.msq` is what a user tweaks and ships; the preset is the starting point the firmware
hands out before any user tuning.

## Where the pieces live

| Piece | Location | Role |
|-------|----------|------|
| Enum value | `firmware/controllers/algo/engine_type_e.java`-derived `engine_type_e` | Identifies the preset (e.g. `BMW_N52`) |
| Dispatch | `firmware/controllers/algo/engine_type_impl.cpp` | `switch` mapping the enum to the setup function |
| Setup function | `firmware/config/engines/<make>.cpp` | Writes fields into `engineConfiguration` |

Example (N52): `engine_type_impl.cpp` case `BMW_N52` -> `bmwN52()` in `config/engines/bmw.cpp`.

## How a preset applies values

A setup function is plain imperative code. It writes directly to `engineConfiguration->...`
or calls shared helpers in `firmware/controllers/algo/defaults/` (declared in `defaults.h`).
Common helpers:

- `setTPS1Calibration(min, max)` / `setTPS1Calibration(min, max, sec2Min, sec2Max)`
- `setPPSCalibration(primaryUp, primaryDown, secondaryUp, secondaryDown)`
- `setTPS1Inputs`, `setPPSInputs`, `setInline4`, `setCustomMap`, `setEtbPID`, ...

Whatever a preset does not set keeps the global default from `setDefaultEngineConfiguration()`.

## Units gotcha: TPS/PPS

These two calibrations use *different* storage units, which trips people up when copying
numbers straight out of a `.msq`:

- **TPS min/max** (`tpsMin`, `tpsMax`, `tps1SecondaryMin/Max`, `tps2*`) are stored as
  **10-bit ADC counts**, not volts. The `.msq` shows volts; convert with
  `convertVoltageTo10bitADC(volts)` (= `volts * 200`, i.e. 1000 counts per 5 V). See
  `firmware/controllers/sensors/tps.h`.
- **PPS voltages** (`throttlePedalUpVoltage`, `throttlePedalWOTVoltage`,
  `throttlePedalSecondaryUpVoltage`, `throttlePedalSecondaryWOTVoltage`) are stored as
  **floats in volts** and are copied through verbatim by `setPPSCalibration`.

## Porting calibration from a `.msq` into a preset

1. Open the `.msq` (XML). Each tunable is a `<constant name="..." units="...">value</constant>`.
2. For TPS: read `tpsMin`, `tpsMax`, `tps1SecondaryMin`, `tps1SecondaryMax` (all `units="V"`)
   and wrap each in `convertVoltageTo10bitADC(...)`.
3. For PPS: read `throttlePedal{,Secondary}{Up,WOT}Voltage` and pass the volts straight into
   `setPPSCalibration`.
4. Ignore uncalibrated second-throttle fields left at defaults (`tps2Min=0`, `tps2Max=5`,
   `tps2SecondaryMin=5`, `tps2SecondaryMax=0`) unless the engine actually runs two throttle
   bodies.
5. Add the calls to the setup function, `#include "defaults.h"`, rebuild. `convertVoltageTo10bitADC`
   is available transitively via `pch.h`.

Note: presets are *code*, not generated config, so no `gen_config` step is needed — a normal
firmware build picks them up. Do not commit any generated files.
