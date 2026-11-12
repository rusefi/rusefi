# Fuel Top-Up Pulse

[tag:fuel_top_up] A top-up pulse adds fuel that the main pulse of a cylinder did not deliver because the requested mass rose after that pulse was scheduled, typically a throttle opening after the injection timing of port injection. The main pulse of a cylinder is computed when its start angle is reached and fixed from then on (see [Fueling System](fueling_system.md)), so without a top-up pulse that cylinder inducts its next charge with the old fuel mass.

Implementation: `firmware/controllers/engine_cycle/fuel_top_up.cpp`, hooks in `InjectionEvent::onTriggerTooth()` (`main_trigger_callback.cpp`). Tests: `unit_tests/tests/ignition_injection/test_fuel_top_up.cpp`.

## Settings

All in the outer `persistent_config_s` (`config->`), TunerStudio **Fuel -> AE / Transient fueling -> Fuel top-up pulse**:

| Setting | Meaning |
|---|---|
| `fuelTopUpMode` | Off, Log only (evaluate and report without injecting) or Inject. Default Off. |
| `fuelTopUpLimitAngle` vs `fuelTopUpRpmBins` (4 points, `FUEL_TOP_UP_CURVE_SIZE`) | End of the window in degrees after compression TDC of the cylinder, the injection timing convention (negative is before TDC), normally near intake valve closing: -180 plus the cam card's closing angle after BDC, taken at about 1 mm lift. Default flat -140 over 1000..4000 RPM, the range up to the default `fuelTopUpMaxRpm`; above the last point the curve holds its last value. There is no start setting: the window opens with the main pulse. |
| `fuelTopUpMinMass` (mg), `fuelTopUpMinPulse` (ms of flow, excluding dead time) | Smaller top-ups are not injected. The pulse minimum keeps a separate pulse inside the injector linear range, which matters most without small pulse correction. |
| `fuelTopUpMaxPercent` | Largest top-up as a percentage of the main pulse mass (default 50). A skipped or zero main pulse therefore gets no top-up. |
| `fuelTopUpMinOffTime` (ms) | Minimum injector off time between the main close and a separate top-up pulse (default 0.8). |
| `fuelTopUpMaxRpm` | No top-up above this RPM (default 4000). |
| `fuelTopUpExtendOpen` | Extend a main pulse that is still open instead of waiting for a separate pulse (default yes). |
| `fuelTopUpFollowRequest` | While the top-up pulse is scheduled or open, move its close later as the request keeps rising (default yes). |

Tunes without these settings have an all-zero RPM axis; `applyDefaultsOrFixAfterBurn()` seeds the defaults, mode stays Off.

The limit is a curve rather than one angle because two RPM effects pull in opposite directions: at low RPM part of the charge flows back into the port between BDC and intake valve closing, so the effective closing is earlier; at higher RPM charge inertia moves it later, while the fuel's travel time from injector to valve covers more degrees and moves the useful limit earlier. Four points cover the top-up's working range; a flat curve is a reasonable start.

## Behaviour

1. **Registration.** When `onTriggerTooth()` schedules the main pulse it registers the requested pre-film mass (`injectionMass[cylinder]`), the engine phase at which the pulse opens, the open time and the stage 1 close time, and marks a top-up as pending. A skipped pulse (too short, zero mass) registers too; a rotational-idle skip, NaN or negative pulse clears the pending top-up.
2. **Window.** It belongs to the registered main pulse: it opens when that pulse opens (`m_mainStartNt`) and ends at the **first** limit angle after the pulse's start phase (`wrapToCycle(limit + cylinder offset - start phase)`). The first tooth past the limit clears the pending top-up, so a request that rises later is left to the next main pulse, which already uses it. Without this, a window reaching back before the next main pulse compared the request with the previous cycle's pulse and could add the same fuel twice. A main pulse that opens after this cycle's intake valve closing (injection for the next intake stroke) gets a window up to the next cycle's limit.
3. **Evaluation.** `evaluateTopUp()` runs at the start of `onTriggerTooth()` on every trigger tooth, before a new main pulse can replace the registration. Inside the window, `missing = min(requested now - registered, registered * maxPercent)`. Without missing fuel it waits for later teeth.
4. **Gates**, reported in `fuelTopUpSkipReason` (`FuelTopUpSkip` in `fuel_schedule.h`): not sequential (1), cranking (2), RPM above the maximum (3), injector duty high enough to lock the start angle (`shouldUpdateInjectionTiming` false, 4), below the minimum mass or pulse (5, stays pending), nothing fits before the limit (6), waiting for a later tooth (7), previous top-up still scheduled (8), fault latched (9), main pulse closed before an extension opened (10).
5. **Late decision.** One top-up per main pulse, so it is decided as late as possible to cover as much of the rise as the window allows. `onTriggerTooth()` passes the phase of the next tooth, so the wait is exact for any wheel and missing-tooth gap. An extension waits while the next tooth still comes more than 200 us before the main close; a separate pulse waits while, opening at the next tooth, a pulse 25 % longer (`FUEL_TOP_UP_WAIT_GROWTH`, room for further growth) would still fit. Waiting is reported as 7.
6. **Extension.** With extension enabled and the main pulse more than 200 us from its close, the injector is opened again now (`overlappingCounter` 2) and an extra close is scheduled at the main close plus the plain flow time of the commanded mass, without dead time. The main close then leaves the pin high.
7. **Separate pulse.** Otherwise the pulse opens at the later of now and the main close plus the minimum off time, with the full injector-model duration (dead time and small pulse correction included).
8. **Limits and what fits.** Both kinds must end before the limit angle and at least the minimum off time before the next main pulse of the cylinder (`injectionStartAngle`), converted with the current `oneDegreeUs`. When the whole difference does not fit, the part that fits is injected: an extension is shortened to the remaining time, a separate pulse keeps its dead time and its flow is scaled (up to three passes, since small pulse corrections are not linear). If that is below the minimum pulse or mass, nothing is injected (6) and the top-up is dropped for this cycle.
9. **Following the request.** With `fuelTopUpFollowRequest`, a scheduled top-up keeps `m_topUpFollowing` set. On each later tooth in the window, `followTopUp()` takes the newly missing fuel (still bounded in total by `fuelTopUpMaxPercent` of the main pulse, `m_mainMass`) and moves the close later by its plain flow time, no dead time, clamped to the same end budget (limit and next main pulse). Growth below 20 us of flow (`FUEL_TOP_UP_MIN_MOVE_US`) waits for the next tooth. Following stops when the close has run or was cancelled (the injector is never reopened), at the limit, on a new main pulse registration, on a fault, outside Inject mode and when no room is left. `fuelTopUpMoveCount` counts the moves.
10. **Bookkeeping.** At most one top-up pulse per main pulse. The registered mass grows by the pre-film mass actually injected, the commanded mass goes through `WallFuel::getTopUpCommandedMass()` (`desired / (1 - beta)`) and only after scheduling `WallFuel::commitTopUp()` adds the deposit `beta * commanded` to the film, without another evaporation step. The trip odometer counts the commanded mass.

Log only performs every step up to scheduling, then sets `fuelTopUpWouldInject` and the last mass/pulse.

Telemetry (in `tps_accel_state`): `fuelTopUpCount`, `fuelTopUpExtensionCount`, `fuelTopUpMoveCount`, `fuelTopUpLastMass` (mg, commanded, including moves), `fuelTopUpLastPulse` (ms; for an extension the extra flow time; including moves), `fuelTopUpRequested`/`fuelTopUpRegistered` for injection event 0, `fuelTopUpSkipReason`, `fuelTopUpWouldInject`, `fuelTopUpFault`.

## Timing budget

At 1500 RPM one degree is about 0.11 ms. With the main pulse ending at -400 deg and the limit at -140 deg, the window after the main pulse is about 29 ms. The late decision fires at the last tooth from which the pulse fits, so the fuel request has to rise before roughly the limit minus the pulse and one tooth: the TPS rate detector (200 Hz, see [Acceleration Enrichment](acceleration_enrichment.md)) and MAP Prediction provide that; a 20 Hz detector would not.

Resolution: decisions happen on trigger teeth, but the request itself (`injectionMass`) only changes in the 200 Hz fast callback, every 5 ms. A 60-2 wheel gives a tooth every 0.67 ms at 1500 RPM, a 12-tooth wheel every 3.3 ms; both are finer than the request updates at that speed, so the decision lands within one tooth of the ideal point either way. The pulses themselves are timed by the microsecond scheduler, independent of the wheel. Very sparse wheels (for example 4 teeth, 10 ms per tooth at 1500 RPM) decide earlier and cover less of the rise.

## Safety invariants

The scheduler (`docs/AI/scheduling_system.md`) executes actions in the timer interrupt under the system lock, and a separate open and close are independent queue entries. A close that is lost while its open runs would leave the injector open. The top-up is built around that:

- Top-up events use the `InjectionEvent` members `m_topUpOpenScheduling` and `m_topUpCloseScheduling`, never the shared 64-entry pool. A still-pending previous top-up blocks a new one (`SchedulerBusy`) instead of silently failing `insertTask()`.
- The close is inserted first, both under `chibios_rt::CriticalSectionLocker`, and `onTopUpOpen()` refuses to open unless that close is armed.
- A moved close is cancelled and rescheduled inside one `CriticalSectionLocker`, and only while `m_topUpCloseScheduling.action` is still set, so a close is armed at every instant; `SingleTimerExecutor::schedule()`/`cancel()` take the same nesting-safe `chSysGetStatusAndLockX()` lock. A close that already ran is never replaced, so the injector is not reopened.
- A separate pulse opens only on an idle injector (`overlappingCounter` 0), an extension only on an open one. On a mismatch the close is cancelled, so the top-up can neither leave the injector open nor cut another pulse short. A mismatch on a separate pulse latches `fuelTopUpFault`, which blocks top-up until the engine stops; a late extension is only dropped.
- Top-up actions open and close the captured output mask and never call `InjectionEvent::update()`, which the main close (`endInjection()`) uses to move the next start angle.
- Evaluation runs inside `handleFuel()`, after `LimpManager::allowInjection()`, so every fuel cut also stops top-up. `mainTriggerCallback()` returns while a firmware error is latched. `firmwareError()` turns all injector pins off directly (`turnAllPinsOff()`), but an open action already in the queue still runs afterwards; its paired close follows, as for main pulses.
- Pulses shorter than 50 us are never scheduled (the scheduler may swap the order of very close events).
- `StressNeverLeavesInjectorOpen` drives thousands of random registrations, requests and timings and checks that every injector ends closed with no counter left raised. `FuelTopUpIntegration` spins a 60-2 sequential engine through the real trigger path.

Watchdog: the IWDG (500 ms) is fed from the 20 Hz slow callback. Top-up adds a window check per cylinder per tooth in the trigger interrupt and at most two queue entries per cylinder per cycle, far below the executor's 500-event run limit; it does not change watchdog timing. After a watchdog reset the injector outputs revert to their reset state until the firmware reinitialises them.

## Limitations

- Sequential injection only; batch and simultaneous modes report `NotSequential`.
- Stage 2 injectors are not used; with staging, the whole top-up goes to the primary injector and an extension extends only stage 1.
- One top-up pulse per main pulse; fuel requested after that pulse closed is handled by the next main pulse.
- The late decision puts the fuel closer to intake valve closing, with less time to evaporate.
- `fuelTopUpMaxPercent` is relative to the main pulse, so a main pulse cut by DFCO gets no top-up when fuel returns.
