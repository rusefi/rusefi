/**
 * @file    engine.h
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global_shared.h"
#include "engine_module.h"
#include "engine_state.h"
#include "rpm_calculator.h"
#include "event_registry.h"
#include "table_helper.h"
#include "listener_array.h"
#include "accel_enrichment.h"
#include "trigger_central.h"
#include "local_version_holder.h"
#include "buttonshift.h"
#include "gear_controller.h"
#include "dynoview.h"
#include "high_pressure_fuel_pump.h"
#include "limp_manager.h"
#include "pin_repository.h"
#include "ac_control.h"
#include "knock_logic.h"
#include "idle_state_generated.h"
#include "sent_state_generated.h"
#include "closed_loop_idle.h"
#include "dc_motors_generated.h"
#include "idle_thread.h"
#include "injector_model.h"
#include "launch_control.h"
#include "shift_torque_reduction_controller.h"
#include "nitrous_controller.h"
#include "antilag_system.h"
#include "start_stop.h"
#include "trigger_scheduler.h"
#include "fuel_pump.h"
#include "main_relay.h"
#include "ac_control.h"
#include "type_list.h"
#include "boost_control.h"
#include "ignition_controller.h"
#include "alternator_controller.h"
#include "harley_acr.h"
#include "dfco.h"
#include "fuel_computer.h"
#include "advance_map.h"
#include "ignition_state.h"
#include "sensor_checker.h"
#include "fuel_schedule.h"
#include "prime_injection.h"
#include "throttle_model.h"
#include "gc_generic.h"
#include "lambda_monitor.h"
#include "efi_output.h"
#include "vvt.h"
#include "closed_loop_fuel.h"
#include "long_term_fuel_trim.h"
#include "electronic_throttle_generated.h"
#include "engine_cylinder.hpp"

#include "tunerstudio/tunerstudio.h"       // TunerStudioOutputChannels
#include "controllers/core/engine_module.h"
#include "controllers/trigger/trigger_central.h"

#if EFI_PULSE_INPUT
#include "modules/pulse_input_module/pulse_input_module.h"
#endif

#include <functional>

#ifndef EFI_BOOTLOADER
#include "engine_modules_generated.h"
#endif

static_assert(MAX_CYLINDER_COUNT > 0);

#ifndef EFI_UNIT_TEST
#error EFI_UNIT_TEST must be defined!
#endif

#ifndef EFI_SIMULATOR
#error EFI_SIMULATOR must be defined!
#endif

#ifndef EFI_PROD_CODE
#error EFI_PROD_CODE must be defined!
#endif

struct AirmassModelBase;

#define MAF_DECODING_CACHE_SIZE 256

#define MAF_DECODING_CACHE_MULT (MAF_DECODING_CACHE_SIZE / 5.0)

/**
 * I am not sure if this needs to be configurable.
 *
 * Also technically the whole feature might be implemented as cranking fuel coefficient curve by TPS.
 */
// todo: not great location for these
#define CLEANUP_MODE_TPS 90
#define STEPPER_PARKING_TPS CLEANUP_MODE_TPS

class IEtbController;

class Engine final : public TriggerStateListener {
public:
    Engine();

    StartStopState startStopState{};


    TunerStudioOutputChannels outputChannels{};

    /**
     * Sometimes for instance during shutdown we need to completely supress CAN TX
     */
    bool allowCanTx = true;

    // used by HW CI
    bool isPwmEnabled = true;

    /**
     * ELM327 cannot handle both RX and TX at the same time, we have to stay quite once first ISO/TP packet was detected
     * this is a pretty temporary hack only while we are trying ELM327, long term ISO/TP and rusEFI broadcast should find a way to coexists
     */
    bool pauseCANdueToSerial = false;

#if EFI_ELECTRONIC_THROTTLE_BODY
    IEtbController *etbControllers[ETB_COUNT] = {nullptr};
#endif // EFI_ELECTRONIC_THROTTLE_BODY


#if EFI_DYNO_VIEW
    DynoView dynoInstance{};
#endif

#if EFI_IDLE_CONTROL
	LongTermIdleTrim m_ltit;
#endif // EFI_IDLE_CONTROL

#if EFI_ENGINE_CONTROL
    FuelComputer fuelComputer{};
#endif // EFI_ENGINE_CONTROL

    type_list<
        Mockable<InjectorModelPrimary>,
        Mockable<InjectorModelSecondary>,
#if EFI_IDLE_CONTROL
        Mockable<IdleController>,
#endif // EFI_IDLE_CONTROL

        TriggerScheduler,
#if EFI_HPFP && EFI_ENGINE_CONTROL
        Mockable<HpfpController>,
#endif // EFI_HPFP && EFI_ENGINE_CONTROL
#if EFI_ENGINE_CONTROL
        Mockable<ThrottleModel>,
#endif // EFI_ENGINE_CONTROL
#if EFI_ALTERNATOR_CONTROL
        AlternatorController,
#endif /* EFI_ALTERNATOR_CONTROL */
        MainRelayController,
        Mockable<IgnitionController>,
        Mockable<AcController>,
        PrimeController,
        DfcoController,
#if EFI_HD_ACR
        HarleyAcr,
#endif // EFI_HD_ACR
        Mockable<WallFuelController>,
        KnockController,
        SensorChecker,
#if EFI_ENGINE_CONTROL
        Mockable<LimpManager>,
#endif // EFI_ENGINE_CONTROL
#if EFI_VVT_PID
        VvtController1,
        VvtController2,
        VvtController3,
        VvtController4,
#endif // EFI_VVT_PID
#if EFI_BOOST_CONTROL
        BoostController,
#endif // EFI_BOOST_CONTROL
        TpsAccelEnrichment,
#if EFI_LAUNCH_CONTROL
        NitrousController,
#endif // EFI_LAUNCH_CONTROL
#if EFI_LTFT_CONTROL
        LongTermFuelTrim,
#endif
        ShortTermFuelTrim,

#if EFI_PULSE_INPUT
    PulseInputModule,
#endif // EFI_PULSE_INPUT

#include "modules_list_generated.h"

        EngineModule // dummy placeholder so the previous entries can all have commas
    > engineModules{};

    /**
     * Slightly shorter helper function to keep the code looking clean.
     */
    template<typename get_t>
    constexpr auto & module() {
        return engineModules.get<get_t>();
    }

    template<typename get_t>
    constexpr auto const & module() const {
        return engineModules.get<get_t>();
    }

#if EFI_TCU
    GearControllerBase *gearController = nullptr;
#endif

    // todo: boolean sensors should leverage sensor framework #6342
    SwitchedState clutchUpSwitchedState{&engineState.clutchUpState};
    SwitchedState brakePedalSwitchedState{&engineState.brakePedalState};
    SwitchedState acButtonSwitchedState{&engineModules.get<AcController>().unmock().acButtonState};
    SimpleSwitchedState luaDigitalInputState[LUA_DIGITAL_INPUT_COUNT]{};

#if EFI_LAUNCH_CONTROL
    LaunchControlBase launchController{};
    ShiftTorqueReductionController shiftTorqueReductionController{};
    SoftSparkLimiter softSparkLimiter{false};
    // technically not directly related to EFI_LAUNCH_CONTROL since useful for TCU
    SoftSparkLimiter hardSparkLimiter{true};
#endif // EFI_LAUNCH_CONTROL

#if EFI_ANTILAG_SYSTEM
    AntilagSystemBase antilagController{};
#endif // EFI_ANTILAG_SYSTEM

#if EFI_ANTILAG_SYSTEM
//    SoftSparkLimiter ALSsoftSparkLimiter{false};
#endif /* EFI_ANTILAG_SYSTEM */

#if EFI_SHAFT_POSITION_INPUT
    LambdaMonitor lambdaMonitor{};
#endif // EFI_ENGINE_CONTROL

    IgnitionState ignitionState{};
    void resetLua();

#if EFI_SHAFT_POSITION_INPUT
    void OnTriggerStateProperState(efitick_t nowNt, size_t triggerStateIndex) override;
    void OnTriggerSynchronization(bool wasSynchronized, bool isDecodingError) override;
    void OnTriggerSynchronizationLost() override;
    TriggerStateListener* nextListener() override;
    TriggerStateListener* secondListener = nullptr;
#endif

    void setConfig();

#if EFI_AUX_VALVES
    AuxActor auxValves[AUX_DIGITAL_VALVE_COUNT][2]{};
#endif // EFI_AUX_VALVES

#if EFI_UNIT_TEST
    bool needTdcCallback = true;
private:
    int bailedOnDwellCount = 0;
public:
    int getBailedOnDwellCount() const { return bailedOnDwellCount; }
    void incrementBailedOnDwellCount() { bailedOnDwellCount++; }
#endif /* EFI_UNIT_TEST */

    int getGlobalConfigurationVersion() const;

    // a pointer with interface type would make this code nicer but would carry extra runtime
    // cost to resolve pointer, we use instances as a micro optimization
#if EFI_SIGNAL_EXECUTOR_ONE_TIMER
  // while theoretically PROD could be using EFI_SIGNAL_EXECUTOR_SLEEP, as of 2024 all PROD uses SingleTimerExecutor
    SingleTimerExecutor scheduler{};
#endif
#if EFI_SIGNAL_EXECUTOR_SLEEP
  // at the moment this one is used exclusively by x86 simulator it should theoretically be possible to make it available in embedded if needed
    SleepExecutor scheduler{};
#endif
#if EFI_UNIT_TEST
    TestExecutor scheduler{};

    std::function<void(IgnitionEvent*, bool)> onIgnitionEvent;
    std::function<void(const IgnitionEvent&, efitick_t, angle_t, efitick_t)> onScheduleTurnSparkPinHighStartCharging
            = [](const IgnitionEvent&, efitick_t, angle_t, efitick_t) -> void {};
    std::function<void(const IgnitionEvent&, efitick_t)> onScheduleOverFireSparkAndPrepareNextSchedule
            = [](const IgnitionEvent&, efitick_t) -> void {};
#endif // EFI_UNIT_TEST

#if EFI_ENGINE_CONTROL
    FuelSchedule injectionEvents{};
    IgnitionEventList ignitionEvents{};
    scheduling_s tdcScheduler[2]{};
    OneCylinder cylinders[MAX_CYLINDER_COUNT]{};
#endif /* EFI_ENGINE_CONTROL */

#if EFI_ELECTRONIC_THROTTLE_BODY
    // todo: move to electronic_throttle something?
    bool etbAutoTune = false;
    bool etbIgnoreJamProtection = false;
#endif // EFI_ELECTRONIC_THROTTLE_BODY

#if EFI_UNIT_TEST
    bool tdcMarkEnabled = true;
#endif // EFI_UNIT_TEST

    bool slowCallBackWasInvoked = false;

    RpmCalculator rpmCalculator{};

    Timer configBurnTimer{};
    Timer engineTypeChangeTimer{};

    /**
     * This counter is incremented every time user adjusts ECU parameters online (either via rusEfi console or other
     * tuning software)
     */
    int globalConfigurationVersion = 0;

#if EFI_SHAFT_POSITION_INPUT
    TriggerCentral triggerCentral{};
#endif // EFI_SHAFT_POSITION_INPUT

    /**
      * See FAST_CALLBACK_PERIOD_MS
      */
  void periodicFastCallback();
    /**
      * See SLOW_CALLBACK_PERIOD_MS
      */
  void periodicSlowCallback();
	void onEngineStopped();
  void updateSlowSensors();
  void updateSwitchInputs();
  void updateTriggerConfiguration();

    bool isRunningPwmTest = false;

    /**
     * are we running any kind of functional test? this affect
     * some areas
     */
    bool isFunctionalTestMode = false;

    void resetEngineSnifferIfInTestMode();

    EngineState engineState{};

    dc_motors_s dc_motors{};
#if EFI_SENT_SUPPORT
    sent_state_s sent_state{};
#endif

    efitimeus_t timeToStopIdleTest{};

    SensorsState sensors{};

    void preCalculate();

    void efiWatchdog();
    void onEngineHasStopped();

    /**
     * Needed by EFI_MAIN_RELAY_CONTROL to shut down the engine correctly.
     * This method cancels shutdown if the ignition voltage is detected.
     */
    void checkShutdown();

    /**
     * Allows to finish some long-term shutdown procedures (stepper motor parking etc.)
       Called when the ignition switch is turned off (vBatt is too low).
       Returns true if some operations are in progress on background.
     */
    bool isInShutdownMode() const;

    /**
     * The stepper does not work if the main relay is turned off (it requires +12V).
     * Needed by the stepper motor code to detect if it works.
     */
    bool isMainRelayEnabled() const;

    void onSparkFireKnockSense(uint8_t cylinderIndex, efitick_t nowNt);

#if EFI_UNIT_TEST
    AirmassModelBase* mockAirmassModel{};
#endif

private:
    void reset();

    void injectEngineReferences();
};

trigger_type_e getVvtTriggerType(vvt_mode_e vvtMode);

void applyNonPersistentConfiguration();
void prepareOutputSignals();

void scheduleReboot();
bool isLockedFromUser();
void unlockEcu(int password);

// These externs aren't needed for unit tests - everything is injected instead
#if !EFI_UNIT_TEST
extern Engine ___engine;
static constexpr Engine* engine_ptr = &___engine;

struct EngineAccessor {
    constexpr Engine* operator->() { return engine_ptr; }
    constexpr Engine const* operator->() const { return engine_ptr; }
    constexpr operator Engine*() { return engine_ptr; }
    constexpr operator Engine const*() const { return engine_ptr; }
    constexpr operator bool() const { return true; }
};

[[maybe_unused]] static EngineAccessor engine;

#else // EFI_UNIT_TEST
extern Engine* engine;
#endif // EFI_UNIT_TEST
