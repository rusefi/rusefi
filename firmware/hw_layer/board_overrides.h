/*
 * @file board_overrides.h
 * @brief Board-specific override mechanism
 *
 * This header provides a centralized way to define and manage board-specific overrides.
 * Custom boards can implement their own specialized behavior by overriding these function pointers.
 *
 * Usage Example:
 *
 * 1. Define your custom function:
 *    void myBoardCustomHello() {
 *        // Your custom implementation
 *    }
 *
 * 2. Set up the override in your board_configuration.cpp:
 *    void setup_custom_board_overrides() {
 *        custom_board_boardSayHello = myBoardCustomHello;
 *    }
 *
 *
 * @date: jul 09, 2025
 * @author FDSoftware
 */

#pragma once
#include <functional>
#include <optional>
#include "rusefi_types.h"
#include "engine_configuration.h"
#include "obd_error_codes.h"

// function with no parameters and returning void
using setup_custom_board_overrides_type = void (*)();
using setup_custom_board_config_type = void (*)(const engine_configuration_s * /*previousConfiguration*/);
using setup_custom_board_output_type = int (*)();
using setup_custom_board_outputs_type = Gpio* (*)();
using setup_custom_board_engine_type_type = void (*)(engine_type_e);
using setup_custom_get_float_type = float (*)();
using setup_custom_get_adc_float_type = float (*)(adc_channel_e /*hwChannel*/);
using setup_custom_board_adjust_voltage_type = float (*)(float /*voltage*/, adc_channel_e /*hwChannel*/);
using setup_custom_on_board_standby_type = void (*)();
using setup_custom_hack_hellen_board_id_type = int (*)(int /*detectedId*/);
using setup_custom_get_cylinder_ignition_trim_type = angle_t (*)(size_t /*cylinderNumber*/, float /*rpm*/, float /*ignitionLoad*/);
using setup_custom_get_cylinder_fuel_trim_type = float (*)(size_t /*cylinderNumber*/, float /*rpm*/, float /*fuelLoad*/);
using setup_custom_bool_type = bool (*)();

// Read-only MIL display policy. Return true to blink a stored code; an absent
// hook accepts all codes. This never filters diagnostic storage or warning().
// Install during board setup, before periodic callbacks begin.
using board_mil_code_filter_type = bool (*)(ObdCode);
extern std::optional<board_mil_code_filter_type> custom_board_milCodeFilter;
// Translate long OBD code to some short that can be blinked by Check Engine indicator
// Install during board setup, before periodic callbacks begin.
using board_mil_code_translator_type = uint32_t (*)(ObdCode);
extern std::optional<board_mil_code_translator_type> custom_board_milCodeTranslator;

// Five hooks touch configuration at different lifecycle moments: DefaultConfiguration,
// ConfigOverrides, fix_configuration, validateConfig, OnConfigurationChange.
// See docs/board-configuration-override-hooks.md for timelines and a decision guide.

// Read-only configuration validation, invoked from validateConfigOnStartUpOrBurn()
// at ECU startup and on every TS Burn. Returns false if configuration is broken.
// previousConfiguration is nullptr at startup and &activeConfiguration (the configuration
// the ECU is currently running on) on burn, so a board can validate transitions and not
// just the new state. Must NOT mutate configuration - use custom_board_fix_configuration
// for fixes/migrations.
using custom_validate_config_type = bool (*)(const engine_configuration_s* /*previousConfiguration*/);
extern std::optional<custom_validate_config_type> custom_board_validateConfig;

// Board-specific counterpart of applyDefaultsOrFixAfterBurn(): conditionally repair or
// migrate configuration (defaults application, startup, every burn). Returns true if
// anything was changed. previousConfiguration is nullptr while defaults are applied and
// at startup, &activeConfiguration (the currently running configuration) on burn.
// Unlike custom_board_ConfigOverrides this is for guarded fixes, not unconditional
// pinning of values.
using custom_fix_configuration_type = bool (*)(const engine_configuration_s* /*previousConfiguration*/);
extern std::optional<custom_fix_configuration_type> custom_board_fix_configuration;

using setup_custom_board_ts_command_override_type = void (*)(uint16_t /*subsystem*/, uint16_t /*index*/);
extern std::optional<setup_custom_board_ts_command_override_type> custom_board_ts_command;

enum class BasicConfigurationAction : uint16_t;
// Applies board pin/ADC changes for a supported Basic Configurations action.
using board_basic_configuration_type = bool (*)(BasicConfigurationAction);
extern std::optional<board_basic_configuration_type> custom_board_applyBasicConfiguration;

// Board-specific TunerStudio binary command handler. Unlike custom_board_ts_command
// (which rides on TS_EXECUTE and is fire-and-forget), this hook receives the channel and
// the raw request payload so a board can implement a full binary request/response. It is invoked for opcodes the
// base firmware does not recognize (see TS_BOARD_COMMAND / handleCrcCommand). `data`
// points at the payload (the command byte already consumed) and `size` is its length in
// bytes. Return true if the command was handled; false lets the base firmware NAK it.
class TsChannelBase;
using board_ts_binary_command_type = bool (*)(TsChannelBase* /*tsChannel*/, char /*command*/, uint8_t* /*data*/, uint16_t /*size*/);
extern std::optional<board_ts_binary_command_type> custom_board_ts_binary_command;

#if !defined(EFI_BOOTLOADER) && EFI_FILE_LOGGING && EFI_PROD_CODE
#include "ff.h"
using setup_custom_board_write_error_file_type = void (*)(FIL * /*fd*/);
extern std::optional<setup_custom_board_write_error_file_type> custom_board_onBoardWriteErrorFile;
#endif // EFI_FILE_LOGGING

// Board-specific extra columns for the .teeth CSV trigger log (see tooth_logger.cpp,
// active while engineConfiguration->sdTriggerLogCsv is enabled). Both CSV hooks fill
// 'buffer' with an snprintf-style CSV fragment and return the number of characters
// written (excluding the null terminator); a negative value or one >= 'size' is treated
// as an error and fails the row/header. The fragment must start with the ", " column
// separator and must not contain a line terminator. The header hook runs once per file,
// the line hook once per row - both must emit the same number of columns.
//
// The line hook's 'payload' points at this row's per-event board payload, sampled at
// append time by custom_board_toothLogSample (nullptr when
// TOOTH_LOG_BOARD_PAYLOAD_SIZE is 0 and the parallel array is compiled out - see
// tooth_logger.h). The board casts it to its own payload struct; without a payload the
// hook may still emit flush-time values, with the associated time-skew caveat.
using board_tooth_log_csv_header_type = int (*)(char* /*buffer*/, size_t /*size*/);
using board_tooth_log_csv_line_type = int (*)(char* /*buffer*/, size_t /*size*/, const void* /*payload*/);
extern std::optional<board_tooth_log_csv_header_type> custom_board_toothLogCsvHeader;
extern std::optional<board_tooth_log_csv_line_type> custom_board_toothLogCsvLine;

// Per-event payload sampler: called by ToothLoggerBufferPool::appendI for every logged
// tooth/coil/injector event, under the critical section, frequently from interrupt
// context - must be fast and lock-free (plain loads/stores only). Fills exactly
// TOOTH_LOG_BOARD_PAYLOAD_SIZE bytes at 'dst'. Only meaningful when
// TOOTH_LOG_BOARD_PAYLOAD_SIZE > 0; unfilled rows (no hook installed) are zeroed.
using board_tooth_log_sample_type = void (*)(void* /*dst*/);
extern std::optional<board_tooth_log_sample_type> custom_board_toothLogSample;

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "can_msg_tx.h"
using board_can_rx_type = void (*)(const size_t, const CANRxFrame &, efitick_t);
extern std::optional<board_can_rx_type> custom_board_can_rx;

using board_can_update_dash_type = void (*)(CanCycle cycle);
extern std::optional<board_can_update_dash_type> custom_board_update_dash;
#endif // EFI_CAN_SUPPORT

/**
 * @brief Pre-HAL initialization override point
 * Allows boards to perform custom initialization before HAL is initialized
 */
extern std::optional<setup_custom_board_overrides_type> custom_board_preHalInit;

extern std::optional<setup_custom_board_overrides_type> custom_board_boardSayHello;

// Called before configuration is loaded
extern std::optional<setup_custom_board_overrides_type> custom_board_InitHardwareEarly;
extern std::optional<setup_custom_board_overrides_type> custom_board_InitHardware;
extern std::optional<setup_custom_board_overrides_type> custom_board_InitHardwareExtra;
// Pushes configuration state OUT into board hardware (e.g. pull-up enable GPIOs); does not
// edit the tune. Fires after applyNewHardwareSettings() on every live config apply with
// previousConfiguration = &activeConfiguration, and once at boot from initHardware() with
// previousConfiguration = nullptr. See also the pin-release restriction below.
extern std::optional<setup_custom_board_config_type> custom_board_OnConfigurationChange;

// Dynamic hardware re-init participation (see docs/hardware-reinit-and-power-cycle.md).
// custom_board_OnConfigurationChange fires only AFTER applyNewHardwareSettings() has finished
// starting hardware, so it cannot release pins - a stale board claim makes the pin repository
// reject the new owner with a criticalError. Boards that claim user-configurable pins must
// instead use this pair, which follows the all-stops-before-any-starts invariant:
// - custom_board_StopHardware: called during the stop phase of applyNewHardwareSettings(),
//   while activeConfiguration still describes the previously claimed pins - release them here.
// - custom_board_StartHardware: called from startHardware() on BOTH ECU start and configuration
//   change - claim pins here from engineConfiguration.
// Boards claiming only fixed pins (not user-configurable) can keep using custom_board_InitHardware.
extern std::optional<setup_custom_board_overrides_type> custom_board_StopHardware;
extern std::optional<setup_custom_board_overrides_type> custom_board_StartHardware;

extern std::optional<setup_custom_board_overrides_type> custom_board_TriggerResetState;

extern std::optional<setup_custom_board_overrides_type> custom_board_BeforeTuneDefaults;
extern std::optional<setup_custom_board_engine_type_type> custom_board_AfterTuneDefaults;
extern std::optional<setup_custom_board_engine_type_type> custom_board_applyUnknownType;

extern std::optional<setup_custom_board_overrides_type> custom_board_periodicSlowCallback;
extern std::optional<setup_custom_board_overrides_type> custom_board_periodicFastCallback;

// Called from Engine::onEngineStopped() so boards can reset their own per-run state
// (e.g. board-specific trigger/IO hardware) when the engine stops.
extern std::optional<setup_custom_board_overrides_type> custom_board_onEngineStopped;

// Board hardware related:
extern std::optional<setup_custom_board_output_type> custom_board_getMetaOutputsCount;
extern std::optional<setup_custom_board_output_type> custom_board_getMetaLowSideOutputsCount;
extern std::optional<setup_custom_board_outputs_type> custom_board_getMetaOutputs;
extern std::optional<setup_custom_board_output_type> custom_board_getMetaDcOutputsCount;

// LTFT to VE table custom apply algo
extern std::optional<setup_custom_board_overrides_type> custom_board_LtftTrimToVeApply;

// specific firmware builds are meant for specific hardware. In order to provide best user experience on well-known boards sometimes we reduce user flexibility.
// See docs/board-configuration-override-hooks.md for how these two differ from each other
// and from custom_board_fix_configuration.

// Seeds board defaults while a FRESH tune is being built (resetConfigurationExt only -
// blank ECU, engine type change, reset to defaults). Does not run when an existing tune
// is loaded from flash, so users may later change anything set here.
extern std::optional<setup_custom_board_overrides_type> custom_board_DefaultConfiguration;
// Forces values humans must not change. Re-applied after flash load, after defaults and
// after EVERY TunerStudio page write chunk - keep it idempotent and cheap. Anything set
// here is effectively pinned regardless of what the user or an old calibration writes.
extern std::optional<setup_custom_board_overrides_type> custom_board_ConfigOverrides;

extern std::optional<setup_custom_get_float_type> custom_board_getFuncPairAllowedSplit;
extern std::optional<setup_custom_board_adjust_voltage_type> custom_board_boardAdjustVoltage;
// Physical input voltage / MCU ADC voltage, by channel. Without an override,
// getAnalogInputDividerCoefficient uses analogInputDividerCoefficient.
extern std::optional<setup_custom_get_adc_float_type> custom_board_getAnalogInputDividerCoefficient;
// Voltage supplying the thermistor divider, in the same units as the scaled
// sensor voltage. Defaults to 5 V; used only for resistive temperature sensors.
extern std::optional<setup_custom_get_adc_float_type> custom_board_getThermistorSupplyVoltage;
extern std::optional<setup_custom_on_board_standby_type> custom_board_onBoardStandBy;
extern std::optional<setup_custom_hack_hellen_board_id_type> custom_board_hackHellenBoardId;
extern std::optional<setup_custom_get_cylinder_ignition_trim_type> custom_board_getCylinderIgnitionTrim;
extern std::optional<setup_custom_get_cylinder_fuel_trim_type> custom_board_getCylinderFuelTrim;

// Level-triggered board config-error evaluation, invoked from refreshConfigErrorState()
// once per Engine::periodicSlowCallback(). Evaluate current conditions worst-first: when
// one holds, call configError(...) with its message and return true (the first condition
// wins; the next one surfaces automatically once it clears). Return false when all
// conditions are clear - the core then clears the message, but only if this mechanism
// raised it, so the hook must NOT call clearConfigErrorMessage() itself.
// See fw-iws updateConfigError() for the reference implementation of the if/else chain.
extern std::optional<setup_custom_bool_type> custom_board_updateConfigError;

extern std::optional<setup_custom_bool_type> custom_board_isBoardWithPowerManagement;
extern std::optional<setup_custom_bool_type> custom_board_boardAllowTriggerActions;
// When set and returning true, ignition/injection stay off for the first crank
// revolution after a fresh trigger synchronization: the first sync point has no
// tooth count to validate against, so a false gap (stretched tooth pair) can
// sync the decoder at the wrong position. See m74_9 false-sync C9003 case.
extern std::optional<setup_custom_bool_type> custom_board_requireValidatedSync;

// When set and returning true, a ratio-validated sync candidate that arrived
// 1-2 events BEFORE the expected count (count deficit - events were LOST
// between the gaps: the analog VR conditioner swallows 1-2 teeth during the
// first-combustion catch; noise only INSERTS events, so a deficit cannot be
// noise) is accepted as a valid sync while rpm < 2 * crankingRpm instead of
// firing C9003 and desyncing. Desyncing there cuts fuel/spark exactly when
// the engine first catches - the observed m74_9 failure mode (C9003 'got
// 56/0', engine dies right after the catch). The 6-12 degree phase offset of
// the lost events only affects the already-elapsed part of the revolution
// (the sync re-anchors at the gap). Count excess keeps the classic C9003
// path. m74_9 sets true; default false preserves the strict behavior.
extern std::optional<setup_custom_bool_type> custom_board_syncEarlyGapWhileCranking;
extern std::optional<setup_custom_bool_type> custom_board_syncByPositionWhileCranking;
extern std::optional<setup_custom_bool_type> custom_board_syncGapHardening;

// When set and returning true, the decoder corrects the sync anchor by the
// gap geometry: the missing-teeth gap is physically (skipped + 1) slots, so a
// measured sync gap ratio below syncRatioAvg means the sync event fired EARLY
// relative to the true first-tooth crossing. On the m74_9 the L9779's output
// gap is systematically compressed to ~2.36-2.5 (a spurious edge from the
// missing region fires ~0.63 pitch early, the auto-hysteresis having
// re-quantized down during the gap; the squared-signal latch then suppresses
// the real first-tooth edge). The decoder would otherwise anchor the phase
// ~3.8 degrees advanced. The correction (in degrees, EMA-smoothed, clamped to
// [0, 1.5] pitch) is applied to the scheduling phase pair. The board's lambda
// picks the rpm band (m74_9: cranking..7000; below cranking the gap can
// compress from acceleration and the correction would misfire). Default
// false: other boards' VR signals do not have the spurious-edge signature.
extern std::optional<setup_custom_bool_type> custom_board_syncGapAnchorCorrection;

// When set and returning a non-negative value, this is the gap-anchor
// correction's CURRENT expected shift (pitch units) - the correction state
// itself, not a fallback. m74_9 returns its VR amplitude-model per-level
// table value (Vp = k*rpm -> hysteresis level -> learned shift, see
// m74_9_vr_model.cpp); the decoder applies it directly and TRAINS the model
// from in-band measurements via triggerObserveGapShift. Negative = no model
// = the correction stays 0.
extern std::optional<setup_custom_get_float_type> custom_board_vrGapShiftPitch;

// When set and returning true, a ratio-validated sync candidate with a 1-2
// event count DEFICIT is accepted as a valid sync regardless of the cranking
// band (the custom_board_syncEarlyGapWhileCranking acceptance above is
// rpm-limited to 4 * crankingRpm). This is for analog VR conditioners that
// intermittently swallow a single tooth mid-revolution at RUNNING rpm: the
// m74_9 L9779's auto-adaptive hysteresis/filter eats the output edge when the
// squared signal's high level falls below Tfilter (datasheet 6.14: 'if
// int_vrs high level lasts less than Tfilter out_vrs is not set to high') -
// the 2026-08-24 20:43 drive showed 837/837 C9003 events with the gap ratio
// windows passing ('Y') and the count short by exactly one (57/58), the
// signature of a single lost decode edge. The deficit direction proves LOST
// (not noise-inserted) events, the ratio windows and the position/elapsed-time
// gates already passed, and the phase shifts only 6-12 degrees for the rest
// of the revolution before the gap re-anchors. The board decides the rpm band
// in its lambda (m74_9: cranking..7000). Default false: other boards keep the
// strict C9003 desync at running rpm.
extern std::optional<setup_custom_bool_type> custom_board_syncAcceptToothLoss;

// Compatibility hook for board sources. The current MAP averaging module
// uses its existing angle windows and does not consume this override.
extern std::optional<setup_custom_bool_type> custom_board_mapInstantAtArm;

// When set and returning a positive interval (microseconds), trigger edges
// arriving sooner than that after the previous accepted edge are dropped as
// noise before decoding - VR comparator ringing / starter interference that
// would inflate the event count and false-sync the decoder mid-revolution
// (m74_9: 100 us; noise bursts measure <50 us, and 100 us stays below the
// 125 us tooth period of a 60-2 wheel at 8000 rpm on RiseOnly).
extern std::optional<setup_custom_get_float_type> custom_board_triggerDebounceUs;

extern std::optional<setup_custom_bool_type> custom_board_getAcrState;
// Return true to replace the complete ACR strategy, including the movement
// and additive-hold gates. Outputs correspond to acrPin and acrPin2.
// Called from slow/trigger contexts: consume cached states, do not advance
// a controller here. Return false to retain the existing strategy.
using board_acr_outputs_type = bool (*)(bool& /*primary*/, bool& /*secondary*/);
extern std::optional<board_acr_outputs_type> custom_board_getAcrOutputs;
// Additive ACR hold: when set and returning true, the Harley ACR valve is kept
// energized regardless of the acrRevolutions countdown, even before engine
// movement. Unlike custom_board_getAcrState this does NOT
// replace the default strategy - it only extends the open window, e.g. while a
// board-side cranking-phase detector is holding fuel/spark off and needs the
// compression signature to stay stationary. Defined in harley_acr.cpp.
extern std::optional<setup_custom_bool_type> custom_board_holdAcr;
// Dynamic variant of engineConfiguration->isPhaseSyncRequiredForIgnition:
// while set and returning true, fuel and spark stay cut (ClearReason::
// EnginePhase) until the trigger central reports hasSynchronizedPhase().
// Lets a board require phase sync only for a bounded window (e.g. the first
// few cranking cycles while a phase detector votes) and then release to
// wasted-spark/batch operation instead of blocking the start indefinitely.
// Defined in limp_manager.cpp.
extern std::optional<setup_custom_bool_type> custom_board_requirePhaseSyncForFiring;
extern std::optional<setup_custom_bool_type> custom_board_isImmobilizerBlocking;
extern std::optional<setup_custom_bool_type> custom_board_allowFlashNow;

/**
 * This function checks if an override is present and calls it if available.
 * Return true if override is present and was called
 */
template<typename FuncType, typename... Args>
static inline bool call_board_override(std::optional<FuncType> board_override, Args&&... args){
    if (board_override.has_value()) {
        std::invoke(board_override.value(), std::forward<Args>(args)...);
        return true;
    }
    return false;
}

template<typename FuncType, typename... Args>
static inline auto get_board_override_result(std::optional<FuncType> board_override, auto defaultValue, Args&&... args){
    if (board_override.has_value()) {
        return std::invoke(board_override.value(), std::forward<Args>(args)...);
    }
    return defaultValue;
}
