# Hardware Re-init on Configuration Change, and the `requiresPowerCycle` Exceptions

rusEFI applies most configuration changes at runtime, without a reboot: when the user
presses Burn in TunerStudio, the firmware selectively stops and restarts the affected
hardware with the new settings. This document describes that mechanism and then lists
the settings that are declared as reboot-only (`requiresPowerCycle` in the TunerStudio
ini), with the code-level reason for each.

## The dynamic approach

### Two configuration images

The core idea is a diff between two copies of `engine_configuration_s`:

- `engineConfiguration` - the *new/future* configuration. TunerStudio writes land here
  immediately.
- `activeConfiguration` - the *old/current* configuration, i.e. a snapshot of what the
  hardware was last started with (`firmware/controllers/algo/engine_configuration.cpp`).

Helper macros compare the two (`firmware/controllers/algo/engine_configuration.h`):

```cpp
#define isConfigurationChanged(x) (engineConfiguration->x != activeConfiguration.x)
#define isPinOrModeChanged(pin, mode) (isConfigurationChanged(pin) || isConfigurationChanged(mode))
```

On RAM-constrained chips, `EFI_ACTIVE_CONFIGURATION_IN_FLASH` avoids the second RAM
copy: `activeConfiguration` aliases the flash image of the stored configuration, and
`isActiveConfigurationVoid` widens `isConfigurationChanged` to treat any non-zero new
value as "changed" until the first `rememberCurrentConfiguration()`.

### The Burn flow

TS Burn -> `onBurnRequest()` -> `incrementGlobalConfigurationVersion()`
(`engine_configuration.cpp`), which is the single top-level entry point for "the
configuration changed". It runs, in order:

1. `applyNewHardwareSettings()` (`firmware/hw_layer/hardware.cpp`) - see below.
2. `custom_board_OnConfigurationChange` board hook.
3. `engine->preCalculate()` - refresh derived/cached math.
4. Subsystem callbacks: ETB (`onConfigurationChangeElectronicThrottleCallback` - PID
   parameters only), bench test, trigger (`onConfigurationChangeTriggerCallback`),
   RPM emulator.
5. `onConfigurationChange(&activeConfiguration)` on every registered `EngineModule`.
6. `rememberCurrentConfiguration()` - the new config becomes the active one.

Online tuning of table cells does not count as a configuration change; only Burn (and
console commands that call `incrementGlobalConfigurationVersion`) triggers this path.

### `applyNewHardwareSettings()` - stop old, start new

The invariant: **all stop methods run before any start method**, and stopping uses
`activeConfiguration` (the pins that are actually claimed) while starting uses
`engineConfiguration`. Stop phase:

- `ButtonDebounce::stopConfigurationList()` - all debounced inputs (incl. start/stop button)
- `stopSensors()` -> `deInit*` for TPS, MAP, thermistors, flex, VSS, aux, EGT, old
  analog inputs, Lua digital inputs (`firmware/init/sensor/init_sensors.cpp`)
- `stopTriggerInputPins()`, `stopSent()`, `stopCanPins()`, `stopKLine()`
- `stopHardware()` - board hook `custom_board_StopHardware`, switch pins, smart-GPIO
  CS pins, logic analyzer, trigger emulator, VVT control pins
- `stopSpiModules()` - releases SPI pins for buses whose `is_enabled_spi_N` changed
- `enginePins.unregisterPins()`

Start phase:

- `reconfigureSensors()` - re-runs sensor init (`sensorStartUpOrReconfiguration(false)`)
- `ButtonDebounce::startConfigurationList()`
- `startHardware()` - shared between boot and re-init: trigger input pins, engine
  pins, switch pins, CAN pins, start/stop button, trigger input validation, board
  hook `custom_board_StartHardware`
- `startSmartCsPins()` - workaround re-init of external gpio-chip CS pins (issue
  #2107); the chips themselves are *not* restarted, see below
- `startKLine()`, conditional `initIdleHardware()` (via `isIdleHardwareRestartNeeded()`),
  `startBoostPin()`, `startTriggerEmulatorPins()`, `startLogicAnalyzerPins()`,
  `startVvtControlPins()`, `startSent()`, `calcFastAdcIndexes()`

Two styles coexist inside this flow, both quoted from the code comment in
`applyNewHardwareSettings`: some hardware is restarted *unconditionally* on any burn
(e.g. VVT pins, boost pin), while for other systems extra effort is made to restart
*only if* the relevant settings changed (SPI buses, idle hardware, clutch pin) using
`isConfigurationChanged`.

### Board participation in dynamic pin start/stop

Board code that claims *user-configurable* pins cannot rely on
`custom_board_OnConfigurationChange`: that hook fires from
`incrementGlobalConfigurationVersion()` only after `applyNewHardwareSettings()` has
already finished starting hardware, so the board's stale claim from the previous
configuration is still registered and the pin repository rejects the new owner with
`criticalError("Pin ... required by ... but is used by ...")`
(`hw_layer/pin_repository.cpp`, `brain_pin_markUsed`).

For this, `board_overrides.h` provides a dedicated pair that follows the
all-stops-before-any-starts invariant:

- `custom_board_StopHardware` - called at the top of `stopHardware()`, i.e. during the
  stop phase of `applyNewHardwareSettings()`, while `activeConfiguration` still
  describes the pins the board previously claimed. Release board-owned configurable
  pins here (`efiSetPadUnused` / `OutputPin::deInit`).
- `custom_board_StartHardware` - called at the end of `startHardware()`, which runs on
  BOTH ECU start and configuration change. Claim board-owned configurable pins here
  from `engineConfiguration`.

Boards that only claim fixed pins (not user-adjustable, e.g. the Hellen CAN terminator
output) do not need these and can keep one-time setup in `custom_board_InitHardware`.

### What is intentionally NOT restarted

Anything initialized only from `initHardware()` / `initHardwareNoConfig()` /
`initEngineController()` at boot and absent from the start phase above requires a
reboot. That set is what `requiresPowerCycle` communicates to the tuner.

## `requiresPowerCycle` - declaring a setting reboot-only

`requiresPowerCycle = <fieldName>` entries live in
`firmware/tunerstudio/tunerstudio.template.ini` (search near `requiresPowerCycle =
cylindersCount`). TunerStudio uses them to warn the user that the change takes effect
only after an ECU restart. Board config extensions
(`@@BOARD_CONSTANTS_EXTENSIONS_FROM_FILE@@`) can append board-specific entries.

Note this is *advisory UI metadata only* - the firmware does not enforce it. The
firmware-side truth is whether the subsystem participates in
`applyNewHardwareSettings()`; the ini list is maintained by hand and is deliberately
conservative in a few places (marked below).

### Exceptions with hard boot-only reasons

| Setting(s) | Reason a power cycle is required |
|---|---|
| `cylindersCount` | Pervasive in injection/ignition scheduling (`fuel_schedule.cpp`, `spark_logic.cpp` iterate it, firing-order pairing does modular math on it). Changing it while engine events are in flight is unsafe; there is no quiesce/re-plan path. Added explicitly in commit 41fb61930f7. Also feeds bore-based knock defaults. |
| `afr_hwChannel`, `afr_hwChannel2` | Analog AFR sensor registration happens once at boot: `initNewSensors()` -> `initLambda()`. `reconfigureSensors()` deliberately does not re-run `initLambda` (marked `todo:` in `init_sensors.cpp`), so a changed channel is never re-subscribed. |
| `gppwm1_pin` .. `gppwm4_pin` | `initGpPwm()` runs once from `initEngineController()`; it claims pins and starts PWM with no stop/restart path (`controllers/actuators/gppwm/gppwm.cpp`). |
| `mc33816_cs`, `mc33816_rstb`, `mc33816spiDevice` | `initMc33816()` runs once at boot: performs the chip reset + code-RAM firmware download over SPI and spawns the driver thread (`hw_layer/mc33816.cpp`). No teardown exists. |
| `isSdCardEnabled`, `sdCardSpiDevice`, `sdCardCsPin` | SD stack (MMC thread, FATFS mount, USB MSD LUN) is brought up at boot (`initEarlyMmcCard`). The USB MSD interface is part of the fixed composite USB descriptor enumerated at boot (see CLAUDE.md "CDC console and MSD share ONE composite USB device"). |
| `boardUseCanTerminator` | Applied by `configureHellenCanTerminator()` which is explicitly guarded by a `static bool initialized` - the terminator output is written exactly once (`config/boards/hellen/hellen_common.cpp`). |
| `lps25BaroSensorScl`, `lps25BaroSensorSda` | I2C baro device is probed/initialized once via `initBaro()` in `initNewSensors()`; `reconfigureSensors()` does not re-run it. |
| `binarySerialTxPin`, `binarySerialRxPin`, `tunerStudioSerialSpeed` | The TS communication channel itself: UART pads and speed are configured when the TS port/thread starts at boot (`console/binary/tunerstudio_io_serial_ports.cpp`). You cannot re-plumb the link you are talking over. (Speed is only meaningful for a real UART; USB CDC ignores baud rate.) |
| `is_enabled_spi_1` .. `is_enabled_spi_6` | Asymmetric support: `stopSpiModules()` *is* called on burn (a bus can be released), but `initSpiModules()` runs only at boot - a newly enabled bus is never started until reboot (`hw_layer/spi.cpp`, `hardware.cpp`). |
| `canWriteEnabled`, `canReadEnabled`, `can1ListenMode`, `can2ListenMode` | `initCan()` is boot-only: RX/TX threads are created once (ChibiOS threads cannot be torn down here), and listen-only mode is baked into the controller registers at `canStart()` (`hw_layer/drivers/can/can_hw.cpp`). Only CAN *pin* start/stop is dynamic. |
| `enableAemXSeries`, `enableAemXSeriesEgt` | CAN wideband/EGT sensor objects are registered once at boot (`initCanSensors()`, `initLambda()`, `initEgt()`); the sensor registry has no unregister. The analog-vs-CAN AFR conflict check also runs at init only. |
| `etb_use_two_wires`, `etbFreq`, `etbFunctions1`, `etbFunctions2` | DC-motor hardware wiring mode, PWM frequency and function-to-motor assignment are fixed in `initElectronicThrottle()` -> `initDcMotor()` at boot (`actuators/dc_motors.cpp`). The runtime ETB callback (`onConfigurationChangeElectronicThrottleCallback`) refreshes control parameters (PID etc.) only. |
| `alternatorPwmFrequency` | `initAlternatorCtrl()` starts the alternator PWM once from `initEngineController()`; there is no alternator entry in `applyNewHardwareSettings()`. |
| `auxAnalogInputs1` .. `auxAnalogInputs8` | Slow-ADC channel/pad setup for these raw inputs happens during boot-time ADC init; the burn path only recalculates *fast* ADC indexes (`calcFastAdcIndexes()`). |
| `cylinderBore`, `knockFrequency`, `enableSoftwareKnock` | `initSoftwareKnock()` is boot-only: it derives the target frequency (explicit `knockFrequency`, else bore-based via `bore2frequency(cylinderBore)`), computes bandpass filter coefficients, claims the fixed knock ADC pads and starts the knock DSP thread (`controllers/sensors/impl/software_knock.cpp`). |
| `injectorCompensationMode` | Changes the *interpretation* of the injector flow / rail-pressure settings used by the fueling math; declared reboot-only so the mode and the values it reinterprets are guaranteed to be applied as one coherent set from stored config. |

### Conservative entries (firmware appears to handle these dynamically)

These are listed as `requiresPowerCycle` in the ini, but the corresponding hardware
*is* restarted by `applyNewHardwareSettings()`. They are kept out of caution (a mid-run
PWM/pin restart is tolerated but not a designed feature) or predate the dynamic
support. Treat the ini as the user-facing contract; if you make one of these robustly
dynamic, remove the ini entry in the same change.

| Setting(s) | Current firmware behavior |
|---|---|
| `idle_solenoidFrequency` | `isIdleHardwareRestartNeeded()` explicitly checks `idle.solenoidFrequency` and re-runs `initIdleHardware()` on burn (`actuators/idle_hardware.cpp`). |
| `boostPwmFrequency` | `startBoostPin()` is called unconditionally on every burn and re-applies the frequency (`actuators/boost_control.cpp`). |
| `vvtPins1` .. `vvtPins4`, `vvtOutputFrequency` | `stopVvtControlPins()` / `startVvtControlPins()` run on every burn and restart the PWM with current pins and frequency (`actuators/vvt.cpp`). |
| `startStopButtonPin`, `startStopButtonMode` | The button is a `ButtonDebounce`; the debounce configuration list is stopped/restarted on burn and `initStartStopButton()` is part of `startHardware()`. |
| `etbSplit` | TPS redundancy split threshold is re-applied by `initTps()` which runs inside `reconfigureSensors()` on every burn (`init/sensor/init_tps.cpp`). |

### Known gap, neither dynamic nor declared

External smart gpio chips (TLE8888, MC33972, DRV8860 etc.): `applyNewHardwareSettings`
only re-inits their chip-select pins (`startSmartCsPins()`, workaround for issue
#2107). The chips themselves are configured once at boot; changing a chip's CS pin or
per-chip settings on the fly does not work (see the TODO comment in `hardware.cpp`),
and these fields are not currently in the `requiresPowerCycle` list.

## Adding a new setting - decide which bucket it goes in

1. **Prefer dynamic.** If the setting maps to hardware that already participates in
   `applyNewHardwareSettings()` (pins, sensors, debounced buttons, SENT, K-line, CAN
   pins, idle/boost/VVT actuators), wire it into the existing stop/start pair or a
   module `onConfigurationChange`, guarded by `isConfigurationChanged(field)` if the
   restart is disruptive.
2. **If the owning subsystem is boot-only** (creates threads, downloads chip firmware,
   affects USB descriptors, or the setting is structural like `cylindersCount`), add a
   `requiresPowerCycle = <field>` line to `firmware/tunerstudio/tunerstudio.template.ini`
   next to the existing block, so TunerStudio warns the user.
3. Never rely on `requiresPowerCycle` for safety - it is a UI hint. If applying a value
   live would be dangerous, the firmware must ignore the new value until restart (i.e.
   consume it only during init), not merely hope the user reboots.
