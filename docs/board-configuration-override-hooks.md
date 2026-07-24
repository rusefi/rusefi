# Board configuration override hooks: which one do I use?

Boards customize configuration handling via optional function pointers declared in
`firmware/hw_layer/board_overrides.h` and registered in the board's
`setup_custom_board_overrides()`. Five of these hooks look deceptively similar -
they all "do something board-specific with configuration" - but they run at
different lifecycle moments and have different contracts.

## Summary

| hook                              | signature                                  | touches config?             | when it runs                                                                 |
|-----------------------------------|--------------------------------------------|-----------------------------|------------------------------------------------------------------------------|
| `custom_board_DefaultConfiguration` | `void ()`                                | writes - seeds defaults     | only while building a fresh default tune (`resetConfigurationExt`)           |
| `custom_board_ConfigOverrides`      | `void ()`                                | writes - forces values      | after flash load, after defaults, after every TS page write chunk            |
| `custom_board_fix_configuration`    | `bool ()` -> true if changed             | writes - fixes/migrations   | from `applyDefaultsOrFixAfterBurn()`: defaults, startup, every burn          |
| `custom_board_validateConfig`       | `bool ()` -> false if broken             | READ-ONLY                   | from `validateConfigOnStartUpOrBurn()`: startup, every burn                  |
| `custom_board_OnConfigurationChange`| `void (const engine_configuration_s* prev)` | reads only - drives hardware | after `applyNewHardwareSettings()` on live config change; boot `initHardware()` |

## The two mental axes

1. **Direction**: the first three hooks write INTO the configuration; `validateConfig`
   only judges it; `OnConfigurationChange` reads it and pushes state OUT to board
   hardware (GPIOs, pull-up enables, ...).
2. **Trigger**: defaults-time only (`DefaultConfiguration`) vs every-time-config-
   might-have-changed (`ConfigOverrides`, `fix_configuration`, `validateConfig`,
   `OnConfigurationChange`).

## Per-hook contracts

### custom_board_DefaultConfiguration - "what a fresh tune looks like on this board"

Called from `resetConfigurationExt()` (`firmware/controllers/algo/engine_configuration.cpp`),
i.e. only when a default tune is being constructed: blank ECU, engine type change,
"reset to defaults". It runs after `setDefaultEngineConfiguration()` and
`custom_board_BeforeTuneDefaults`, and before `applyEngineType()`. `EFI_PROD_CODE` only.

Use it for starting-point values the user is free to change later: enable SD card,
software knock, default CAN pins, sensible sensor assignments. Example:
`uaefi_boardDefaultConfiguration` (`config/boards/hellen/uaefi/board_configuration.cpp`).

It does NOT run when an existing tune is loaded from flash - values set here can
drift in the field. If a value must survive user editing, that is `ConfigOverrides`;
if an old tune must be migrated, that is `fix_configuration`.

### custom_board_ConfigOverrides - "values humans must not change"

Called from three places:
- `loadConfiguration()` right after the tune is read from flash (or defaulted);
- `resetConfigurationExt()` right after `custom_board_DefaultConfiguration` (`EFI_PROD_CODE` only);
- `TunerStudio::handleWriteChunkCommand()` after EVERY page write chunk from TS
  (`firmware/console/binary/tunerstudio.cpp`).

Because it re-runs on every TS write, anything set here is effectively pinned:
the user (or a stale calibration image) may write a different value, but it is
overwritten before it can take effect. Use it for facts of the PCB: fixed
EN/vbatt pins, SD wiring, CAN pins on boards where they are not user-selectable.
Example: `uaefi_boardConfigOverrides`.

Keep it idempotent and cheap - it runs on every write chunk, not once per burn.

### custom_board_fix_configuration - "repair or migrate a loaded tune"

Board-side extension point of `applyDefaultsOrFixAfterBurn()`
(`firmware/controllers/algo/defaults/default_base_engine.cpp`), which is the ONLY
place allowed to mutate configuration in the startup/burn path. Runs when defaults
are applied (`setDefaultBaseEngine`), at ECU startup, and on every TS Burn.
Returns true if anything was changed.

Use it for conditional fixes: a value that older tunes have wrong or unset and
that should be corrected without stomping legitimate user choices. Fixes must be
guarded (`if (looksWrong) { fix; return true; }`) so the changed-flag stays
meaningful. Example: `uaefi_fixConfiguration` re-applies CAN2 pins only when
`can2RxPin` drifted off `Gpio::B12`.

Contrast with `ConfigOverrides`: an override is unconditional pinning; a fix is a
conditional migration that reports whether it did anything.

### custom_board_validateConfig - "go/no-go, strictly read-only"

Called from `validateConfigOnStartUpOrBurn()` (`firmware/controllers/engine_controller.cpp`)
at ECU startup (a false return prevents engine-controller initialization) and on
every TS Burn. Must NOT mutate configuration - report problems via
`criticalError()`/`firmwareError()` or by returning false, and leave repairs to
`custom_board_fix_configuration`. Note that `applyDefaultsOrFixAfterBurn()` (and
therefore `fix_configuration`) runs partway through validation, so checks placed
after it see the repaired tune.

### custom_board_OnConfigurationChange - "push (new) config state into board hardware"

Called from two places:
- `incrementGlobalConfigurationVersion()` (`engine_configuration.cpp`), AFTER
  `applyNewHardwareSettings()`, with `previousConfiguration = &activeConfiguration`.
  This fires on every live settings apply: TS Burn (`onBurnRequest`), console `set`
  commands, engine type change, Lua config writes, trigger emulator toggles.
- `initHardware()` (`hw_layer/hardware.cpp`, `EFI_PROD_CODE` only) at boot, with
  `previousConfiguration = nullptr` - so the same handler also applies initial state.

This hook does not edit the tune; it makes board hardware match it - the AlphaX
boards drive pull-up/pull-down enable GPIOs from `boardUse*PullUp` config flags
(`customBoardOnConfigurationChange` in `config/boards/hellen/alphax-4chan/board_configuration.cpp`).

Restriction (see the comment block in `board_overrides.h` and
`docs/hardware-reinit-and-power-cycle.md`): it fires only after
`applyNewHardwareSettings()` has already started the new hardware, so it cannot
release previously claimed user-configurable pins. Boards that claim such pins
must use the `custom_board_StopHardware`/`custom_board_StartHardware` pair instead.

## Lifecycle timelines

Boot (production):

```
runRusEfi
 -> loadConfiguration
     -> readFromFlash                      (or resetConfigurationExt on blank ECU:
                                            ... -> custom_board_DefaultConfiguration
                                                -> custom_board_ConfigOverrides
                                                -> applyDefaultsOrFixAfterBurn
                                                    -> custom_board_fix_configuration)
     -> custom_board_ConfigOverrides
 -> initEfiWithConfig -> commonEarlyInit -> initHardware
     -> custom_board_InitHardware
     -> custom_board_OnConfigurationChange(nullptr)
     -> custom_board_InitHardwareExtra
 -> validateConfigOnStartUpOrBurn
     -> custom_board_validateConfig        (read-only)
     -> applyDefaultsOrFixAfterBurn
         -> custom_board_fix_configuration
```

TS tune upload + Burn:

```
every write chunk  -> custom_board_ConfigOverrides
Burn command       -> validateConfigOnStartUpOrBurn(true)
                       -> custom_board_validateConfig
                       -> applyDefaultsOrFixAfterBurn -> custom_board_fix_configuration
                   -> requestBurn -> onBurnRequest -> incrementGlobalConfigurationVersion
                       -> applyNewHardwareSettings
                       -> custom_board_OnConfigurationChange(&activeConfiguration)
```

## Decision guide

- Starting-point value for fresh tunes, user may change it later
  -> `custom_board_DefaultConfiguration`
- Hardware fact of the board, user must never change it
  -> `custom_board_ConfigOverrides`
- Old/broken tunes need a conditional repair or migration
  -> `custom_board_fix_configuration` (return true when you changed something)
- Tune value combination is invalid, refuse to run
  -> `custom_board_validateConfig` (read-only; `criticalError` or return false)
- Board hardware (GPIO state etc.) must track a config value
  -> `custom_board_OnConfigurationChange` (or Stop/StartHardware if pins are
    user-configurable)
