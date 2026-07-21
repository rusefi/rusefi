# Scheduling & Timing Architecture

rusEFI has two distinct scheduling domains:

1. **Precise event scheduling** - microsecond-accurate, hardware-timer-driven execution of injection/ignition edges at exact times or crank angles.
2. **Periodic control loops** - fixed-rate thread/timer callbacks (1000 Hz down to 20 Hz) for everything that is engine-asynchronous: sensor sampling, fuel/ignition math, actuator controllers.

## Time Units

- `efitick_t` / "NT" = native ticks of the scheduling timebase.
- On STM32, TIM5 runs as a free-running 32-bit counter at **4 MHz** (`US_TO_NT_MULTIPLIER = 4`, so 1 NT = 250 ns), extended to 64 bits in software. The *same* timer is used both to read the current time (`getTimeNowLowerNt()` returns `TIM5->CNT`) and to fire scheduled events (output-compare on channel 0) - one timebase, no cross-clock jitter. See `firmware/hw_layer/ports/stm32/microsecond_timer_stm32.cpp` and `port_mpu_util.h`.
- Generic GPT ports (cypress/kinetis) use a 1 MHz one-shot GPT instead (`microsecond_timer_gpt.cpp`).
- Convert with `US2NT`/`NT2US`/`MS2NT`.

## Precise Event Scheduling (the "executor")

Key files, all in `firmware/controllers/system/timer/` unless noted:

- `scheduler.h` - `Scheduler` interface (`schedule(msg, scheduling_s*, efitick_t, action_s)` / `cancel`), `scheduling_s` (an intrusive sorted-list node holding target time + action), and `action_s` (type-safe callback + argument wrapper built via `action_s::make<Func>(arg)`).
- `event_queue.{h,cpp}` - `EventQueue`: sorted singly-linked list of pending `scheduling_s`, plus a fixed pool of 64 slots for callers that pass `scheduling = nullptr` (pool exhaustion is a `criticalError`). Not thread-safe by itself; callers lock. `executeOne()` busy-waits the last few microseconds for near events instead of re-arming the timer.
- `single_timer_executor.{h,cpp}` - `SingleTimerExecutor` (prod implementation, `EFI_SIGNAL_EXECUTOR_ONE_TIMER`): one hardware timer serves the whole queue. `schedule()` inserts under a critical section; if the new event becomes the queue head, the hardware timer is re-armed. The timer ISR (`globalTimerCallback` -> `onTimerCallback`) executes everything due, then arms the timer for the new head. Events within 8 us of now (`lateDelay = US2NT(8)`) are executed immediately rather than scheduled - that is roughly the cost of one timer interrupt. A run of more than 500 back-to-back executions panics ("Maximum scheduling run length exceeded").
- `firmware/hw_layer/microsecond_timer/microsecond_timer.cpp` - hardware layer + guard rails:
  - Scheduling in the past increments `timerFreezeCounter` + `warning()`; targets closer than 2 us are pushed to now+2 us (arming "now" on a running counter would wrap 2^32 ticks into the future).
  - Targets more than **10 seconds** out (`TOO_FAR_INTO_FUTURE_MS`) are a `firmwareError` - nothing legitimate schedules that far ahead.
  - `validateHardwareTimer()` at boot schedules a 10 ms test callback and errors if it fires late/never/at the wrong time (>20% clock error).
  - A `watchDogBuddy` self-rescheduling event keeps the queue non-empty; a separate 2 s watchdog thread errors if the timer stops being armed entirely.
  - The timer ISR runs at interrupt priority `EFI_IRQ_SCHEDULING_TIMER_PRIORITY` (4 on STM32) - injection/ignition edges preempt all normal threads.

Access the scheduler via `engine->scheduler` or `getScheduler()`.

### Angle-based scheduling

- `scheduleByAngle(timer, nowNt, angle, action)` (`firmware/controllers/engine_cycle/rpm_calculator.cpp`) converts an angle delta to time using `rpmCalculator.oneDegreeUs` (recomputed on each RPM update) and calls the plain time scheduler. So "angle" scheduling is really "time as of the most recent tooth" - accuracy depends on how recently RPM/phase was refreshed.
- `TriggerScheduler` (`firmware/controllers/system/timer/trigger_scheduler.{h,cpp}`, an `EngineModule`) adds a tooth-synchronized layer for events far in the future (e.g. a spark many teeth away). `scheduleOrQueue()`: if the target angle falls before the next expected tooth, it time-schedules immediately via `scheduleByAngle`; otherwise the `AngleBasedEvent` is parked on a linked list and `scheduleEventsUntilNextTriggerTooth()` (called on every decoded tooth from `mainTriggerCallback`) converts it to a time-based schedule once its angle window arrives. This keeps long-range events phase-accurate under RPM change instead of extrapolating stale RPM over many degrees.
- Consumers: `spark_logic.cpp` (dwell start via `scheduleByAngle`, spark fire via `scheduleOrQueue`, overdwell protection auto-fires at 1.5x nominal dwell if the fire event stays queued), `main_trigger_callback.cpp` (injector open via `scheduleByAngle`, close at open-time + duration via `getScheduler()->schedule`).

## Periodic Control Loops

Rates are centralized in `firmware/controllers/core/main_loop.h`:

| Name | Rate | Period | Drives |
|------|------|--------|--------|
| `MAIN_LOOP_RATE` | 1000 Hz | 1 ms | `MainLoop` thread tick |
| `ADC_UPDATE_RATE` | 500 Hz | 2 ms | slow ADC + sensor subscriber updates (board-overridable) |
| `ETB_UPDATE_RATE` | 500 Hz | 2 ms | electronic throttle (still on its own `DcThread` today) |
| `FAST_CALLBACK_RATE` | 200 Hz | 5 ms (`FAST_CALLBACK_PERIOD_MS`) | `Engine::periodicFastCallback()` |
| `SLOW_CALLBACK_RATE` | 20 Hz | 50 ms (`SLOW_CALLBACK_PERIOD_MS`) | `Engine::periodicSlowCallback()` |

**Migration in progress**: `MainLoop` (`main_loop.cpp`, a 1000 Hz `PeriodicController` at `PRIO_MAIN_LOOP = NORMALPRIO+10`) derives per-tick `LoopPeriod` flags (1000/500/250/200/20 Hz, integer divisors of 1000 enforced by `static_assert`) and currently drives **only the ADC update**. The fast/slow/ETB branches are `//TODO` stubs; the real drivers of `periodicFastCallback`/`periodicSlowCallback` are still the legacy `PeriodicFastController`/`PeriodicSlowController` virtual timers in `engine_controller.cpp` (started by `initPeriodicEvents()`). Both are started in `rusefi.cpp`. When touching rates, `main_loop.h` is the single source of truth either way.

`PeriodicController` base (`periodic_thread_controller.h`) sleeps until a window measured from loop *start* (`chThdSleepUntilWindowed`), so the frequency stays constant regardless of how long the task body takes.

### periodicFastCallback (200 Hz)

`Engine::periodicFastCallback()` (`engine.cpp`) -> `EngineState::periodicFastCallback()` (`engine2.cpp`) is the fuel/ignition math pipeline: dwell, IAT/CLT/baro corrections, DFCO, rev limit, tCharge, injection mass + duration (incl. stage 2 split), STFT/LTFT application, ignition advance + knock/limp retard, per-cylinder trims and timing, multispark - then `onFastCallback()` on every `EngineModule` (IdleController, BoostController, VvtController, AlternatorController, KnockController, WallFuelController, HpfpController, MapAveragingModule, LimpManager, TachometerModule, ...).

It is also invoked outside the 200 Hz timer:
- from `mainTriggerCallback` when trigger config changed (recompute before scheduling);
- from `RpmCalculator::assignRpmValue` on the first cranking revolution (seed sane values immediately rather than up to 5 ms later).

So `periodicFastCallback` must be safe to call re-entrantly with respect to engine position (it only computes state; scheduling consumes it).

### periodicSlowCallback (20 Hz)

Prod entry is `doPeriodicSlowCallback()` (`engine_controller.cpp`): start/stop button, `RpmCalculator::onSlowCallback()` (RPM timeout -> engine-stopped detection), accel-enrichment reset when stopped, then `Engine::periodicSlowCallback()` (`engine.cpp`): trigger config re-read, O2 heater + starter relay logic, `efiWatchdog()`, `updateSlowSensors()`, shutdown check, TPS accel enrichment sampling, VR threshold PWM, GPPWM, `onSlowCallback()` on every module (FanController, FuelPumpController, AcController, GearDetector, SensorChecker, TripOdometer, PrimeController, MainRelayController, ShortTermFuelTrim, LongTermFuelTrim, IgnitionController, ...), then TCU update and watchdog reset back in the wrapper.

### Other fixed rates worth knowing

| What | Rate | Where |
|------|------|-------|
| Fast ADC (knock/MAP sampling trigger) | 10 kHz GPT | `adc_inputs.h` (`GPT_FREQ_FAST/GPT_PERIOD_FAST`), `adc_onchip_fast.cpp` |
| ETB `DcThread` | 500 Hz | `electronic_throttle_impl.h` (`ETB_LOOP_FREQUENCY`), PRIO_ETB |
| CAN TX thread | 200 Hz | `can.h` (`CAN_CYCLE_FREQ`) |
| Lua tick | 200 Hz default (`luaTickPeriodUs = 5 ms`, configurable) | `lua.cpp`, lowest priority |
| MIL / malfunction indicator | 100 Hz | `malfunction_indicator.cpp` |
| Accelerometer SPI poll | 50 Hz | `accelerometer.cpp` |
| Blinking LEDs, TS serial poll | 20 Hz / 10 ms sleeps | `engine_controller.cpp`, `tunerstudio.cpp` |

Thread priorities live in `firmware/controllers/thread_priority.h`.

## Unit Tests and Simulator

The `Scheduler` implementation is selected by feature flags in `engine.h`:

- **Prod**: `SingleTimerExecutor` (hardware timer, above).
- **Unit tests** (`EFI_UNIT_TEST`): `TestExecutor` (`unit_tests/test-framework/test_executor.cpp`) - same `EventQueue`, but no hardware and no autonomous execution: tests advance virtual time explicitly (`EngineTestHelper` / `executeAll`), and scheduled actions fire synchronously at that point. Inspect pending events with `getHead()`/`getForUnitTest(index)`.
- **Simulator** (`EFI_SIMULATOR`): `SleepExecutor` (`simulator/simulator/system/signal_executor_sleep.cpp`) - ChibiOS virtual timers, millisecond-class accuracy, no EventQueue.

Periodic callbacks are likewise not driven by threads in unit tests: `initMainLoop()`/`initPeriodicEvents()` are prod-only. `EngineTestHelper` calls `periodicSlowCallback()` once at construction; `TestBase::updateRpm/updateApp/...` helpers call `periodicFastCallback()` after each sensor change. If a test changes sensors or config and fueling/timing does not react, the missing manual `periodicFastCallback()` call is the usual cause.
