# Work Report

## 2026-09-19 - m74_9: dwell lateArm 0->54->100% analysis + failure-split telemetry

Investigated why angclk dwell falls back to TIM5 while spark/inj stay on the
hardware angle clock. Log (efi_log_2026-09-19_13_16_33_241) showed the
progression across three lockstats snapshots:

  angclk dwell fired=20380 lateArm=0     (13:22:28)
  angclk dwell fired=13594 lateArm=16198 (13:26:47)
  angclk dwell fired=0     lateArm=24364 (13:30:11)

Invariant confirmed: spark fired == dwell fired + dwell lateArm (every dwell,
TMR2 or TIM5, produces exactly one spark via angleClockArmSparkFromNow). The
engine runs; only the dwell arming degrades. A 6424 rpm excursion at 13:24:08
sits between snapshots 1 and 2; ignition gate went OFF at 13:30:59.

Hardware ruled OUT by the full lockstats second line (identical across all
three snapshots): psc=71 (correct), nvic T2/T4/T3=3 (correct),
maxLateUs=71/69/79 us (not ~2^32 -> no race/wrap), initRate=40001/40006
(0.0125% measurement skew; real drift would show as maxLateUs growth, which
is absent). So this is a LOGIC bug in dwell arming, not a timer/priority/
clock issue.

angleClockArmDwell fails in exactly two places (armGuard: no basis OR
remaining > 30 deg; tick-past: atTick already behind TMR2->CNT), but both
incremented the single s_lateArmDwell counter, so the branch was
indistinguishable. Since spark/inj share s_ticksPerDegree/s_currentPhase and
stay healthy, the failure must be dwell-specific: the dwell is armed in the
wrong window/phase (early window stops matching -> current window -> tick-past
at ~150-250 us elapsed), or a caller-vs-angle-clock phase divergence.

Change inventory:
| File | Change |
| --- | --- |
| firmware/hw_layer/angle_clock/angle_clock.h/.cpp | split s_lateArmDwell into s_lateArmDwellGuard + s_lateArmDwellTickPast; added DwellArmRefusal snapshot (branch/window/cyl/target/caller/current phase/cycleDeg/ticksPerDegree/remaining/atTick/CNT); angleClockArmDwell inlines the armGuard checks, records the snapshot on each refusal, gains a bool earlyWindow caller marker |
| firmware/controllers/engine_cycle/spark_logic.h/.cpp | scheduleSparkEvent threads earlyWindow through (true from scheduleDwellEarlyIfDue, false from onTriggerEventSparkLogic) |
| firmware/config/boards/m74_9/board_configuration.cpp | lockstats prints 'angclk dwell lateArm split: guard=%u tickPast=%u' and the last-refusal snapshot line |

Validation:
- compile_m74_9.sh -> BUILD SUCCESSFUL.
- unit_tests make -j12 -> compiled + linked (rusefi_test) successfully.

Open follow-ups:
- Next car session: run lockstats; the split (guard vs tickPast) plus the
  refusal snapshot (target/callerPh/curPh/rem) will name the exact branch and
  the phase divergence, enabling a targeted fix instead of guessing.

## 2026-09-18 - m74_9: removed the CLT/IAT battery-tracking bias logic (revert to fixed 5.0V)

Reverted the VTRK battery-tracking bias for CLT/IAT back to the fixed 5.0V
supply. The tracking made the coolant/intake readings JUMP with battery
voltage instead of holding steady, so it was removed at the user's request.

Root cause of the jumping: the resistance math read the live BatteryVoltage
sensor (20 Hz low-pass) as the divider supply, while the thermistor raw
voltage is low-passed at 2 Hz. On any battery transient the supply stepped
fast and the raw lagged, so the temperature spiked. The tracking was
mathematically correct in steady state (the VBATT/adcVcc factors cancel),
but the filter mismatch made it dynamically noisy.

Files:
- firmware/config/boards/m74_9/board_configuration.cpp - removed
  getThermistorBiasTrackingRatio().
- firmware/init/sensor/init_thermistors.cpp - removed the weak
  getThermistorBiasTrackingRatio() default and the tracking branch in
  configureTempSensorFunction(); always configure(5.0f, ...).
- firmware/controllers/sensors/thermistors.h - removed the declaration.
- firmware/controllers/sensors/converters/resistance_func.h/.cpp - removed
  configureTrackingBias() and m_trackingRatio.
- unit_tests/tests/sensor/resist_func.cpp - removed the two tracking tests.

Decisions:
- Full removal, not just the m74_9 override: the tracking feature was used
  only by m74_9, so the shared machinery was dead code once the override
  was gone.
- Tradeoff: the static bias error at a battery voltage other than 12.5V is
  back (the original VTRK analysis still holds). The durable hardware fix
  is to move the thermistor pull-ups to the regulated VDDA rail.

Validation:
- unit_tests: make -j12 -> BUILD SUCCESSFUL; resistance.* -> 4/4 passed.

Open follow-ups:
- If stable AND battery-independent CLT/IAT is required, move the pull-ups
  to VDDA (hardware) rather than re-adding live-battery tracking.

## 2026-09-18 - m74_9: 'vrmodelreset' console command to clear the VR model training

Added a console command to reset the L9779 VR amplitude model back to its
seed, for when the learned per-level gap-shift table (or the k scalar) has
been poisoned by a bad drive and the operator wants to start learning fresh.

Files:
- firmware/config/boards/m74_9/m74_9_vr_model.h/.cpp - new
  m74_9VrModelReset(): resets shiftTablePitch[] to 0.63 pitch, clears
  vrTableRevolutions[], sets vrAmplitudePerRpm=0, marks vrDirty, then
  persists the reset.
- firmware/config/boards/m74_9/board_configuration.cpp - registered as
  addConsoleAction("vrmodelreset", m74_9VrModelReset).

Decisions:
- Persistence: when the engine is stopped the command queues a NON-forced
  write (storageRequestWriteID(EFI_VR_MODEL_RECORD_ID, false)) so the
  storage manager's own storageAllowWriteID -> custom_board_allowFlashNow
  gate (incl. its 10-poll debounce) still decides, instead of bypassing it
  like the periodic saver's forced write. When the engine runs it only
  resets RAM and defers to m74_9VrModelPeriodic(), which saves on the next
  stop - same contract as 'vrk'.
- The reset must reach MFS or the next boot reloads the stale stored
  record; marking vrDirty + the queued write guarantees the seed (k=0,
  0.63 pitch) overwrites the old 'VRML' record.

Validation:
- firmware/config/boards/m74_9/compile_m74_9.sh -> BUILD SUCCESSFUL.

## 2026-09-16 - VE-table autotune in the Java console (VeAnalyzePane, no firmware changes)

Added a lambda-driven VE autotune tab to the rusEFI console, analogous to
TunerStudio's VE Analyze Live but running inside the console and writing
back through the existing tune-burn path.

Files:
- java_console/ui/src/main/java/com/rusefi/ui/VeAnalyzeModel.java (new)
- java_console/ui/src/main/java/com/rusefi/ui/VeAnalyzePane.java (new)
- java_console/ui/src/test/java/com/rusefi/ui/test/VeAnalyzeModelTest.java (new)
- java_console/ui/src/main/java/com/rusefi/ConsoleUI.java (tab "VE Analyze"
  added next to Knock Analyzer)

Decisions:
- No firmware changes. Only existing output channels are used: RPMValue,
  MAPValue/TPSValue, lambdaValue/AFRValue, Gego. The VE load axis is
  resolved from config: veOverrideMode (VE_MAP -> MAPValue, VE_TPS ->
  TPSValue) and, for VE_None, fuelAlgorithm (Speed-Density -> MAPValue,
  Alpha-N -> TPSValue; MAF/Lua -> unsupported). Lambda source and target
  table follow useLambdaOnInterface (lambdaValue + lambdaTableTbl vs
  AFRValue + afrTableTbl; both target tables share zBins = lambdaTable).
- Correction is the classic VE-Analyze formula:
  newVe = ve * (1 + damping * (actual/target - 1)), damping 0.5. The target
  lambda is interpolated from the target table at (rpm, load) via bilinear
  interpolation in VeAnalyzeModel.interpolate().
- Per-cell confidence (NONE/LOW/MEDIUM/HIGH) from accumulated sample counts -
  the MapDelta idea ported from the MxT optimizer; cells without data are
  left unchanged and grayed out.
- Sampling is one row per output-channels frame via a SensorCentral
  ResponseListener; the pane reads getValue() for RPM/load/lambda/Gego.
- Write-back is ONLY on the explicit "Apply Corrections" button, via
  image.clone() + veZField.writeRawValue + bp.uploadChanges(). No auto-burn -
  consistent with the console's removal of the live-edit upload loop.

Validation:
- ./gradlew :ui:compileTestJava -> BUILD SUCCESSFUL (only pre-existing
  deprecation warnings).
- ./gradlew :ui:test --tests com.rusefi.ui.test.VeAnalyzeModelTest ->
  10/10 passed (tests=10 failures=0 errors=0).

Open follow-ups:
- The lambda-valid gate is a heuristic because lambdaCurrentlyGood /
  fuelingLoad / veTableYAxis are LiveData-only (not output channels):
  RPM >= 500, lambda in a sane band (0.5..1.5 lambda or 7..25 AFR), and
  |Gego - 100| <= 20 (not a big transient). A cleaner gate would need one
  or two extra output channels added to firmware/console/binary/
  output_channels.txt.
- The target lambda table is interpolated on the SAME load axis as the VE
  table (MAP/TPS). If afrLoadSrc selects a different axis this is a first
  approximation.
- Display currently shows suggested VE; a "delta %" mode is the next step.

## 2026-09-08 (part 2) - AT32 bootloader IWDG arm race: the REAL root cause of the latch (fixed)

Part 1 documented the latch mechanics. SWD on the bench proved the actual
latch trigger on m74_9 was wd_counter = 13 (not sw_counter): the
bootloader's IWDG arm (commit 07aad871352, CopInitHook wdgStart 500 ms)
races the app boot.

Chain: the app (m74_9 board.mk) also arms and feeds the IWDG, but only
from boardInit + the 20 Hz slow callback. The bootloader arms its own
500 ms window and jumps to the app at BOOT_BACKDOOR_ENTRY_TIMEOUT_MS = 1 s;
with the AT32 LSI at 40 kHz the real window is ~410 ms. boardInit is reached
LATER than that, so the bootloader's watchdog fires on every app start:
reset loop (period ~1 s), wd_counter climbs past 10, latch parks the ECU in
the bootloader forever. Timing captured on SWD: jump ~1000 ms, app in
SysTick at +1200 ms, IWDG reset ~+1410 ms, bootloader again at +1900 ms;
wd_counter climbed 0->2 in ~3 s of observation.

Why it did not break before 09-08: the bootloader is only updated by a FULL
rusefi.bin flash (openocd); CAN flashes write the app region only
(rusefi_update.srec). The ECU carried a pre-07aad871352 bootloader (no IWDG
arm) through the 08-31/09-01 sessions, while the app-side watchdog (4 s +
20 Hz feed) worked fine. Today's full flash installed the IWDG-arming
bootloader and the race started.

Fix (this change):
- firmware/config/boards/m74_9/board.mk: HAL_USE_WDG=TRUE now guarded by
  `ifneq ($(IS_RE_BOOTLOADER),yes)` (app build only).
- firmware/bootloader/Makefile: HAL_USE_WDG is now FALSE for AT32
  (IS_AT32F435) bootloader builds, TRUE otherwise (single -D per build -
  avoids the "HAL_USE_WDG redefined" -Werror). The bootloader must NOT arm
  the IWDG on AT32; the app's own watchdog is unaffected.
- BL marker bumped BL09 -> BL10 (set_bl_bin_version.sh + mpu_util.h).

Recovery: re-flashed deliver/rusefi.bin via openocd, then cleared the
SharedParams buffer (write 0 to 0x20000000, invalidates the buffer so
SharedParamsInit zeroes both counters on next boot) and reset. Verified on
SWD: app stable in idle (PC 0x0806c034 identical 2 s apart), counters 0/0
(app's errorHandlerResetCounters cleared them after 3 s).

Validation: bundle build (compile.sh -b m74_9) BUILD SUCCESSFUL; new
bootloader ELF has zero IWDG symbols (nm: no wdgStart/WDGD1); merged bin
stamped BL10 at 0x24. Console reconnect + tune re-burn is the remaining
user step (config size changed 17548 -> 18500, so the app resets to
defaults on first boot).

## 2026-09-08 - "ECU dead after flash, bootloader answers" - bootloader reset-loop latch (NOT the config changes)

Symptom: after a CAN flash the app never comes up (console gets 0 bytes on
CAN), the OpenBLT bootloader answers; re-flashing OLD known-good firmware
does not help.

Root cause (bootloader_main.cpp):
- checkIfResetLoop() classifies each bootloader boot's reset cause. Soft
  resets (NVIC_SystemReset: the canOpenBLT jump into the bootloader, the
  flasher's PROGRAM_RESET, probes) increment sw_counter (SharedParams slot 2,
  RAM that survives resets); watchdog resets increment wd_counter (slot 1).
  Only POR/NRST (real power loss / reset pin) clears them.
- rebootLoop = (wd_counter > 10) || (sw_counter > 15); then
  stayInBootloader = true and the main loop NEVER calls CpuStartUserProgram
  ("if (stayInBootloader || wasConnected) continue;").
- Today's many CAN-flash sessions each add >=2 soft resets, so the counter
  crossed 15 and the ECU locked itself in the bootloader - by design (anti-
  brick loop detection). The flasher (XCP) keeps working, the console
  (ISO-TP) hears nothing. Every re-flash only adds MORE soft resets.
- The firmware changes were NOT the cause: the stored-config size mismatch
  (18500 vs 17548) takes the IncompatibleVersion path -> RAM reset to
  defaults, no write, no crash; defaults have canWriteEnabled=true and the
  board ConfigOverrides force canRx/canTx = PG0/PG1 + canOpenBLT=true.

Fix: FULL power removal for >=5-10 s (on m74_9 the L9779 SBC holds VCC
through quick key flicks - a key cycle does NOT clear the counters; only
VCC collapse or NRST does). After power-up the bootloader jumps to the app
within ~1 s and the console connects. With the new firmware the old-size
stored tune will reset to defaults on every boot until a tune is burned.

Prevention: power-cycle (or NRST) after long CAN-flash sessions - do not
accumulate >15 soft resets between power cycles.

## 2026-09-08 - Heater pin correction: AC4 -> L9779 OUT5 (user-buzzed fact)

User fact: AC4 (connector) goes to L9779 OUT5, and that is the first
narrowband lambda heater. The yaml had AC4 = L9779_OUT_6 (heater) and
AE4 = L9779_OUT_5 (EVAP) - the physical board has them swapped.

Changes:
- m74_9.yaml: AC4 id L9779_OUT_6 -> L9779_OUT_5 (function "Oxygen sensor 1
  heater" stays - the tune's o2heaterPin is name-based, so it now drives
  OUT5/IN5/PG5 automatically after regen); AE4 id L9779_OUT_5 ->
  L9779_OUT_6 (EVAP) - the EVAP-on-OUT6 half is an inference by symmetry,
  still to be buzzed out.
- Consequences of the old mapping: the heater never heated (PG6->IN6->OUT6,
  nothing on OUT6 or the EVAP valve if it is there), AND the heater logic
  (ON whenever the engine runs) was switching OUT6 the whole run time - if
  EVAP is physically on OUT6, the canister purge solenoid was held OPEN the
  whole time the engine ran: fuel vapors into the intake = rich/unstable
  idle, one candidate contributor to the idle fueling observations.
- To verify on the car: heater now heats at engine run (isO2HeaterOn + a
  current probe on AC4); buzz AE4->OUT6 and PG5/6->IN5/6 to close the two
  remaining schematic assumptions.

Validation: bundle rebuild SUCCESSFUL; generated_ts_name_by_pin.cpp now maps
L9779_OUT_5 -> "Oxygen sensor 1 heater" and OUT_6 -> "EVAP solenoid
control"; PIN_AC4 = L9779_OUT_5 / PIN_AE4 = L9779_OUT_6; the ini output_pin_e
labels swapped accordingly. The tune's o2heaterPin is name-based, so
"Oxygen sensor 1 heater" now drives OUT5/IN5/PG5 with no msq edit.

CAVEAT (same session, 13:04): a TunerStudio/console re-save rewrote the repo
21129.msq over the working copy: veTable was replaced with the OLD 16-column
hand tune resampled to 30 columns (the stock ME17 VE data was lost in the
file - recoverable from the previous commit a5c1c84bbea), the page-4 block
was dropped (nPages back to 1, second tables lost from the msq),
o2heaterPin = "NONE", forceO2Heating = "yes" (bench heater test). The bundle
zip of the next run must be re-done after the msq settles.

### Final L9779 parallel-input mapping (user-buzzed 2026-09-08)

| net | buzzed | direct_gpio |
| --- | --- | --- |
| IN5 -> OUT5 (AC4, lambda 1 heater) | PG7 | [8] = PG7 |
| IN6 -> OUT6 (AE4, EVAP) | PG5 | [9] = PG5 |
| IN7 -> OUT7 (AK4, lambda 2 heater) | PG6 | [10] = PG6 |
| IN1..4 -> OUT1..4 (injectors) | PE11/PE10/PE9/PE8 | unchanged |

Applied in board_configuration.cpp l9779_cfg.direct_gpio; bundle rebuilt
(260908_1654173061). msq NOT touched per user.

RESOLVED: the earlier "AK4 -> OUT4" reading was a mis-trace - AK4 -> OUT7
(IN7 = PG6) confirms the yaml, so injectors stay on OUT1..4 and both
heaters + EVAP are now correctly wired. The old IN5/6/7 assignment was a
cyclic rotation (PG5/PG6/PG7 -> actually PG7/PG5/PG6).

## 2026-09-07 - Narrowband O2 heater on m74_9: on/off only, NO PWM

Question: how does the analog (narrowband) lambda heater work, is there PWM?

Verified chain:
- Control: Engine::periodicSlowCallback (engine.cpp L167-168):
  heaterControlEnabled = forceO2Heating || rpmCalculator.isRunning();
  enginePins.o2heater.setValue(on). forceO2Heating = "no" in 21129.msq ->
  the heater is ON only while the engine runs; OFF on the bench.
- Pin: enginePins.o2heater.initPin("O2 heater", o2heaterPin) with OM_DEFAULT
  (efi_gpio.cpp L704) - plain push-pull GPIO, no PWM generator.
- o2heaterPin = "Oxygen sensor 1 heater" = Gpio::L9779_OUT_6. On m74_9
  direct_gpio[9] = PG6 -> L9779 IN6; the driver's setPad does palSetPort/
  palClearPort(GPIOG, 6) (l9779.cpp L1086-1094).
- L9779 OUT6 = protected LOW-SIDE driver, max 5 A (O2 heaters), driven by
  logical-AND of the SPI CONTR bit and parallel input IN6 (l9779.cpp header).
  o_oe_mask latches the CONTR enable ON at boot, so the GPIO alone gates the
  heater; heater high side sits on battery +12 V. OUT5..7 are NOT in the WDA
  forced-off set (only OUT1..4 + IGN1..4).
- PWM: NONE anywhere. Firmware = on/off at the 20 Hz slow loop; hardware: the
  L9779 IN6 parallel input is a digital AND (only IN8-PWM has a PWM input,
  used for stepper), and m74_9's l9779_cfg.pwm_gpio is NULL. PG6 is a plain
  GPIO in this design. So the heater runs at full battery voltage whenever
  active - correct for a 4-wire narrowband, but no condensation-shock/
  duty-cycle management exists.
- Diagnostics: isO2HeaterOn output channel (status_loop.cpp L622 reads the pin
  logic), CAN verbose o2Heater bit. Caveat: the IN5-7 -> PG5/6/7 mapping comes
  from the schematic (only IN1-4 buzzed) - verify PG6->IN6 if no heat.
- forceO2Heating = yes: the heater pin is driven high on every 20 Hz slow
  loop pass REGARDLESS of engine state - engine off (key on) included, no
  timeout, no duty cycle: the heater runs for as long as the MCU is powered
  (~1-1.5 A drain on the battery). BUT the ignition gate still caps it: on
  the KEY-off edge m74_9IgnitionGatePeriodic calls l9779_setPowerStage(false)
  -> the driver thread writes CONFIG_REG6=0x16 (PSOFF) which kills ALL L9779
  power stages including OUT6 - so with the ignition key OFF the heater draws
  nothing, forceO2Heating or not. At boot with the key off m74_9 starts with
  power_stage_on=false, so the heater stays dead until the first key-on
  re-init. On the bench the KEY_ON line must be high (power stage gate open)
  for the heater to actually heat with forceO2Heating.

## 2026-09-06 - Fueling analysis: VE vs target AFR, how to add fuel at a given load

Question: target AFR 12.5 but measured 13.5 - which way to turn VE, and what
is the role of the target AFR table? Analysis only.

### Verified math chain (this fork)

- Air estimate (speed-density): m_air = VE%/100 x V_cyl x MAP[kPa] / (0.28705 x T_charge)
  (SpeedDensityBase::getAirmassImpl; V_cyl = displacement/cylinders).
- Fuel mass (FuelComputerBase::getCycleFuel): m_fuel = m_air / (lambda_target x stoich),
  stoich = stoichRatioPrimary (14.7 for gasoline) unless a flex sensor blends.
- Injector model (InjectorModelBase::getInjectionDuration): PW = f(m_fuel / injectorFlow)
  + battLag (additive dead time) - the only non-proportional term.
- Wall fuel (wall_fuel.cpp): transient-only - at steady state the correction
  converges to 0 (film = beta*M_cmd/(1-alpha) -> M_cmd = desired), so it does
  not participate in a steady-state AFR fix.
- Therefore in open loop: measured_AFR = 14.7 x lambda_target x true_VE / tuned_VE.
  Universal correction rule: VE_new = VE_old x measured_AFR / target_AFR.
  For 13.5 measured vs 12.5 target: VE UP by x1.08 = +8% at the operating cells.

### Target AFR table role

- Open loop (STFT inactive): the table scales fuel directly, inverse-proportional:
  changing target 14.7 -> 12.5 adds +17.6% fuel.
- Closed loop: the table IS the STFT target. STFT gates in this tune (21129.msq):
  load 35..85 only (stft_maxOverrunLoad 35 / stft_minPowerLoad 85), target AFR in
  [12.0, 17.0] (stft_minAfr/maxAfr), CLT >= 60, cells maxAdd/maxRemove = 5%,
  timeConstant 30 s. An 8% base error exceeds the 5% authority - closed loop
  alone cannot reach 12.5; VE must be fixed.
- The power zone (load > 85, i.e. MAP 85-100 kPa) is OUTSIDE the STFT window:
  WOT fueling is pure open loop - VE (and the target table) are the only levers.
- Current lambdaTable (msq, lambda-scaled text): cruise zone lambda 1.0 (AFR 14.7),
  power rows 0.823..0.837 (AFR 12.1..12.3). If the goal is exactly 12.5 at power,
  set the target cell to 12.5 first (this LEANS vs the current 12.1-12.3), then
  VE_new = VE_old x measured/12.5.

### Recipe for more fuel at one operating point

1. Identify the cell: axes are MAP 20..100 kPa (16 rows) x RPM 600..6250 (30 cols).
2. Open-loop zones: raise the VE cells at (MAP, rpm) by +measured/target; if the
   target cell differs from the goal, set it first, then fix VE.
3. Closed-loop zone (35..85 load): same VE fix; STFT then sits near 0 trim.
4. Do NOT touch stoichRatioPrimary (scales ALL fueling + AFR math), injector
   flow/dead-time (real injector data), or the wall-fuel model for this.
5. VE also feeds air-flow-based tCharge interpolation and the Cyl Filling %
   channels, but NOT the ignition load here (ignOverrideMode = MAP) and not
   the VE/ignition table axes (veOverrideMode/ignOverrideMode = MAP).

### Narrowband caveat (m74_9)

The O2 input (AK3/PF3) is a NARROWBAND sensor. At 12.5-13.5 AFR it saturates
(~0.85-0.9 V) and the custom curve tops out at 0.9 V -> 14.0 AFR, so any
reading in the rich zone is curve extrapolation - the "13.5" has large
uncertainty. A wideband is required to tune a real 12.5; with a narrowband,
STFT at a 12.5 target is also blind (deadband 0.5% meaningless in saturation).

## 2026-09-06 - 30-column VE/ignition tables filled from stock ME17 maps

Implemented the plan from the analysis above: all four tables are now 30 columns
and filled from the stock ME17 .clb maps.

### Firmware

- `firmware/config/boards/m74_9/prepend.txt`: `VE_RPM_COUNT 30` + `IGN_RPM_COUNT 30`
  (per-board override of the 16/16 defaults in rusefi_config_shared.txt). Load
  axes stay 16 rows (MAP kPa 20..100). RPM axis = 600, 800, ..., 6200, 6250.
- Regenerated sizes (rusefi_generated_m74_9.h): TOTAL_CONFIG_SIZE 17548 -> 18500,
  PAGE_SIZE_3 (LTFT) 2048 -> 3840, PAGE_SIZE_4 (second tables) 1268 -> 2220.
- No board code for the second tables (per user): the console Load Tune applies
  the msq page-4 block (DefaultTuneMigrator copies secondary ini fields,
  CalibrationsUpdater burns only changed pages).

### Data conversion (stock .clb -> rusEFI)

| table | source | conversion |
| --- | --- | --- |
| veTable | fill IM=0 (long intake) | VE% = mg x 21.037 / MAP[kPa] |
| secondVeTable | fill IM=1 (short intake) | same |
| ignitionTable | УОЗ ЧН (partial load) | mg = fill IM=0(rpm,kPa); advance = ЧН(rpm,mg), mg clamped [50,525] |
| secondIgnitionTable | УОЗ ПМ (full power) | same via fill IM=1 |

- Fill clb: X=rpm (24 bins), Z=pressure hPa (100..1500), output mg/cylinder/cycle;
  the exported data block is missing the first 2 cells of the 100 hPa row
  (382 of 384 values) - detected via strict per-column monotonicity in Z, the
  missing cells are irrelevant (used >= 200 hPa).
- Ignition clb: X=rpm (17 bins 500..6250), Z=load mg/cycle (50..525), row-major.
- rusEFI 100%% VE reference = idealGasLaw(0.4 L, 101.325 kPa, 20 C) = 481.65 mg
  (fuel_math.cpp getStandardAirCharge), so VE% = mg x 21.037 / MAP[kPa].
- Regridded to the 30-bin rpm axis, light gaussian smoothing (sigma 0.6),
  rounded to 0.1 (msq digits=1). VE IM=0 45..95%%, peak ~95%% at 4400-4600 rpm;
  VE IM=1 52..91%%, peak ~91%% at ~5400 rpm. ЧН 5.8..43 deg, ПМ 7.6..44.6 deg.
  WOT 100 kPa advance rises 5.8@600 -> ~33@6250 rpm (MBT follows rpm) -
  sane for a 1.6L 16V; idle advance is NOT affected (useSeparateAdvanceForIdle
  is enabled in the tune).
- Idle VE changes: the tune uses the main VE table at idle
  (useSeparateVeForIdle=disabled); at 30 kPa the new values (53-63%) are higher
  than the old hand-tuned row (48-50%) - expect a rich idle until STFT/retune.

### 21129.msq

- ignitionRpmBins/veRpmBins: 16 -> 30 rows (600..6250).
- ignitionTable/veTable: cols=16 -> cols=30 with the converted data.
- New `<page number="3" size="2220">` block: secondVeTable + bins,
  secondIgnitionTable + bins (TS + rusEFI console apply it).
- versionInfo nPages 1 -> 2, page sizes 18500/2220 match the generated ini;
  signature/firmwareInfo/bibliography re-stamped by gen_config_board.sh.

### Validation

- Bundle build `compile.sh -b config/boards/m74_9/meta-info.env` BUILD SUCCESSFUL
  (twice - the first zip packed the msq before it was stamped; the second run
  packs the stamped tune, verified byte-identical to the repo copy).
- Generated ini checked: [30x16] for all four tables, pageSize list
  18500, 256, 3840, 2220, 8000.
- Programmatic msq-vs-ini cross-check: every array constant (110 arrays) has
  matching dims and value counts incl. the four new tables (480 values each).
- Not car-tested: first start must be checked with the wideband (idle richer
  per the VE change above).

## 2026-09-06 - Table column limits analysis (VE / ignition / second tables)

Question: what is the maximum column count for `ignitionTable`,
`secondIgnitionTable`, `veTable` and `secondVeTable` (all 16 columns today),
and can the RPM axis be split 600..6250 rpm at 200 rpm steps? Analysis only,
no code changes.

### Where the sizes come from

All four tables share TWO pairs of compile-time defines:

| tables | defines | default | page |
| --- | --- | --- | --- |
| veTable | VE_LOAD_COUNT x VE_RPM_COUNT | 16x16 | 0 (settings) |
| ignitionTable | IGN_LOAD_COUNT x IGN_RPM_COUNT | 16x16 | 0 (settings) |
| secondVeTable | VE_LOAD_COUNT x VE_RPM_COUNT | 16x16 | 4 (second tables) |
| secondIgnitionTable | IGN_LOAD_COUNT x IGN_RPM_COUNT | 16x16 | 4 (second tables) |
| ltft_table_bank1/2 | VE_LOAD_COUNT x VE_RPM_COUNT (float) | 16x16 | 3 (LTFT) |

Defaults are `#define ... 16` in `firmware/integration/rusefi_config_shared.txt`;
m74_9 does not override them (prepend.txt only sets INJ_PHASE_* and PEDAL_TO_TPS_*).
Proteus H7 is the in-tree precedent for larger tables: `VE_RPM_COUNT 24`,
`IGN_RPM_COUNT 24`, `FUEL_RPM_COUNT 24` in prepend_proteus_h7.txt. A board override
in `prepend.txt` wins over the shared defaults.

### Current m74_9 page sizes (generated rusefi_generated_m74_9.h)

- TOTAL_CONFIG_SIZE (page 0) = 17548 B (already above the legacy 16 KB TS limit -
  the fork's page-table protocol reads chunks with uint16 offset/count, so the
  real ceiling is 65536; firmware enforces `static_assert(sizeof(*config) <= 65536)`
  in tunerstudio.cpp L117).
- PAGE_SIZE_4 (second tables) = 1268 B.
- PAGE_SIZE_3 (LTFT) = 2048 B (2 banks of 16x16 float).
- MFS storage: 128 KB bank on internal flash (at_start_f435/board_storage.cpp),
  records: settings ID 1 + backup ID 2 + page 4 + LTFT + Lua page (8 KB).

### Growth per added column (load axis stays 16, 2-byte cells)

| page | per-column growth | 16 -> 30 columns | new size |
| --- | --- | --- | --- |
| 0: veTable+ignitionTable+2 rpm bin arrays | 68 B | +952 B | 18500 B |
| 4: secondVe+secondIgnition+2 rpm bin arrays | 68 B | +952 B | 2220 B |
| 3: LTFT 2 banks float | 128 B | +1792 B | 3840 B |
| total MFS growth | 264 B/col | +3696 B | ~55 KB of 128 KB bank |

### Theoretical maxima (columns, load=16)

- Page 0: 16 + (65536 - 17548) / 68 = ~721 columns (uint16 TS protocol +
  static_assert ceiling).
- Page 4: 16 + (65536 - 1268) / 68 = ~961 columns (same uint16 chunk protocol).
- Page 3 LTFT: 16 + (65536 - 2048) / 128 = ~512 columns.
- MFS bank becomes the ceiling before the TS protocol at large N (two settings
  copies + pages must share 128 KB), ~150-270 columns depending on GC reserve -
  still far beyond any practical use.
- RAM is a non-issue: +3.7 KB of static config on the 384 KB AT32F435.

So the practical ceiling is TunerStudio usability, not the firmware: 24 columns
are already proven in-tree (Proteus), 32x16 = 512 cells is the classic MS table
size TS renders comfortably. 30 columns is trivially safe.

### 600..6250 rpm at 200 rpm steps

(6250 - 600) / 200 = 28.25 - not integer. Options, both legal because bins are
free-form arrays (no uniform-step requirement):
- 29 columns: 600..6200 at even 200 rpm steps.
- 30 columns: 600, 800, ..., 6200, then 6250 as the last bin (last step 50 rpm).

`setRpmTableBin` fills any column count with sensible defaults, so a
non-16x16 config still gets usable axes; canned engine tables guarded by
`#if (... == 16)` are simply skipped for other sizes (m74_9 = MINIMAL_PINS uses
`buildTimingMap` for ignition and the flat-80% fallback for VE).

### How to do it (not done - analysis only)

1. Add to `firmware/config/boards/m74_9/prepend.txt`:
   `#define VE_RPM_COUNT 30` + `#define IGN_RPM_COUNT 30` (one pair covers all
   four tables plus LTFT automatically).
2. `touch firmware/integration/rusefi_config.txt` to force config regeneration
   (board prepend edits do not always retrigger it), rebuild the board bundle
   (regen also stamps the ini and the default msq signature).
3. Migration cost: the existing 21129.msq is NOT load-compatible - the 16-column
   tables/bins do not match the new 30-column layout (TS fills what fits or
   rejects the fields), so the tune must be re-saved/imported with the new ini;
   LTFT page 3 changes shape too. Plan a fresh table fill after flashing.

### Validation

Read-only investigation: no firmware changes, no builds, no tests. All numbers
cross-checked against `rusefi_config_shared.txt`, `rusefi_config.txt`,
`config_page_3.txt`, `config_page_4.txt`, the generated
`rusefi_generated_m74_9.h` / `engine_configuration_generated_structures_m74_9.h`,
`tunerstudio.cpp` (TS_PAGE_SETTINGS static_assert L117, uint16 chunk requests
in tunerstudio.h), `storage_mfs.cpp` and `at_start_f435/board_storage.cpp`.

## 2026-09-03 - angle_clock: fix 59% dwell lateArm (scheduleDwellEarlyIfDue moved before rpm==0 gate)

Goal: eliminate the 59% `angclk dwell lateArm` rate observed in the 2026-09-03
19:14 car session (20 min, avg ~1550 rpm, peaks 3452+).

Stats that triggered the investigation:

```
angclk dwell fired=25412 lateArm=36299  spark fired=61711 lateArm=0  inj fired=49353 lateArm=284
sched dwell: n=36299 (== lateArm, 1:1 TIM5 fallback per failed TMR2 arm)
```

25412 + 36299 = 61711 = spark fired -> every coil charge went through TMR2 or TIM5.
59% went to TIM5, precision loss: 2-tooth lead -> 0-1 tooth lead + executor lateness.

Root causes (two independent problems):

1. **rpm==0 storm-flap teeth skipped scheduleDwellEarlyIfDue.** The m74_9 catch
   trigger storm flaps rpm 0/nonzero at ~1 kHz. `scheduleDwellEarlyIfDue` was
   called AFTER the `rpm==0` early return in `mainTriggerCallback`. Every flap
   tooth skipped it -> next tooth saw the dwell in the CURRENT window [current-
   Phase, nextPhase) = 0-6 deg remaining -> `onTriggerEventSparkLogic` ran at
   ~150-250 us elapsed (after `engineModules.apply_all`) -> tick already past
   -> lateArm. At 3452 rpm, 6 deg = 290 us; 150 us elapsed -> 52% failure rate.

2. **scheduleDwellEarlyIfDue only checked the early window.** The current window
   was handled exclusively in `onTriggerEventSparkLogic` at ~150-250 us elapsed.
   Injection (`InjectionEvent::onTriggerTooth`) checks BOTH windows in one call
   at ~13 us elapsed, which is why `inj lateArm=284` (<<1%) vs `dwell lateArm=59%`.

Key insight: under EFI_ANGLE_CLOCK, `scheduleSparkEvent` has `UNUSED(rpm)` and
`UNUSED(dwellMs)`, so calling `scheduleDwellEarlyIfDue` with rpm==0 is safe.
Same reason `angleClockOnTooth` already runs before the rpm==0 gate.

Fix (commit 51637606bd4 on branch maccan-tx-fix):

- **main_trigger_callback.cpp**: moved `scheduleDwellEarlyIfDue` from position 3
  (after `handleFuel`, ~33 us elapsed) to BEFORE the `rpm==0` gate (right after
  `angleClockOnTooth`, ~5 us elapsed). Now runs on ALL teeth including storm-flap
  teeth.
- **spark_logic.cpp**: added current-window check `[currentPhase, nextPhase)` in
  `scheduleDwellEarlyIfDue` alongside the existing early-window check, mirroring
  `InjectionEvent::onTriggerTooth`. At ~5 us elapsed even 0.5 deg remaining
  (~54 us at 1550 rpm) leaves 49 us before the arm tick -> tick-past always passes.

Expected result: dwell lateArm drops from 59% to ~0%. Remaining lateArms: only
sub-margin events caught as `immediate` by `angleClockRefresh`, and the first 4
teeth after engine sync while `ignitionEvents.isReady` is still false.

Validation performed: unit tests 1169/1169 pass (12 pre-existing EFI_ANGLE_CLOCK
test failures unchanged), m74_9 firmware BUILD SUCCESSFUL. Car validation pending.

Open follow-up: on-car `lockstats` after a drive should show `angclk dwell
lateArm ~= 0` and `immediate` count should stay low (< 5 per engine cycle).

---

## 2026-08-24 - m74_9: test-log error analysis (car sessions 10:04 and 10:52)

Goal: inventory the errors in the 2026-08-24 test logs, find the FIRST error, and
explain the cause. Two user hypotheses to check: (1) interrupts not keeping up
with event processing, (2) the L9779 watchdog not being fed in time (the TLE9201
blade-drop warning as a scheduler/IRQ symptom).

Files analyzed:

| File | What it is |
| --- | --- |
| efi_log_2026-08-24_10_04_00_849.log.0 | console log, session 1, ~57 min, 218 `newerr` trigger dumps |
| efi_log_2026-08-24_10_52_50_618.log.0 | console log, session 2, ~17 min, 74 `newerr` dumps |
| rusEFI_outputChannels_2026-08-24_10_54_37_190.mlg | output channels, 17.4 s, clean idle ~1000 rpm (car, `isbenchtest=0`) |
| rusEFI_outputChannels_2026-08-24_11_09_27_798.mlg | output channels, 21.5 s, clean idle ~1000 rpm, AFTER the 10:56 storm |

Error inventory (OBD C-codes and warnings, both sessions):

| Code | S1 | S2 | Meaning |
| --- | --- | --- | --- |
| C9002 too many teeth "expected 58/0 got 58/0" | 13 | 6 | sync point not accepted, full 58-tooth revolution counted |
| C9003 not enough teeth "got 56/0" | 7 | 2 | 2 teeth lost between sync points |
| C9007 tooth #114 early | 7 | 2 | tooth-angle diagnostic (does NOT drop teeth) |
| C9008 tooth #30/32/34/36 late | 7 | 2 | tooth-angle diagnostic (does NOT drop teeth) |
| C9009 skipped spark event | 29 | 17 | coil still high when next dwell requested |
| C9351..C9354 coil overcharge 6.2-8.3 ms | 31 | 12 | dwell ran past the requested spark |
| TLE9201 "outputs disabled while engine running" | 9 | 5 | blade drop; see taxonomy below |

First-error timeline (session 1):

- 100513.476-100515.973: settings burn (MFS ID 1, 2327 ms, status 2) - engine
  stopped, not an error.
- **100523.247: FIRST warning** - `TLE9201 outputs disabled while engine running
  (l9779 WDA ec=4 wda_int=0 ok=781 fail=0 miss=6)`. WDA healthy -> not a watchdog
  kill, the DIS line went high through the PB13/ETC_EN or ETC_WD hardware chain.
- **100524.417: FIRST OBD code** - `C9354 cylinder 4 Coil 2 overcharge 8.25 ms`,
  immediately followed by `C9002 expected 58/0 got 58/0` and the VRS ramp re-arm
  (engine stopped -> next cranking attempt).
- Session 2 first error: C9009 skipped spark at 105416; first blade drop at
  105531 with `ec=3` -> 1 ms later `ec=5 wda_int=1` (WDA kill), 0.7 s later all
  four coils overcharge + engine stop.

The chain is: blade/output drop -> fuel/spark cut -> crank speed collapses at the
catch -> the missing-teeth gap leaves the ratio window -> C9002/C9003 -> engine
stops -> VRS ramp re-arms -> next attempt, repeating all session.

Root-cause findings:

1. Hypothesis 1 (ISR overload): RULED OUT. `extioverflowcount` (uint8 lifetime
   EXTI-queue-overflow counter) reads 0 in the MLG taken AFTER the 4300-5100 rpm
   desync storms, as do `maxtriggerreentrant` and `triggerignoredtoothcount`.
   `newerr TRG` dumps show the gap ratio windows PASSED (`Y`, gap 2.31-2.34 /
   0.99-1.01 inside [1.6,4.5]/[0.85,1.35]) while the event count at the gap was
   wrong (`eventIndex=110/112` vs ~116) - 2-3 teeth are lost per revolution in
   the ANALOG VR path (L9779 VRS at ramp step 3 = full adaptive + 17 uA
   hysteresis floor), consistently, at 4300-5100 rpm only. The clean idle MLG
   windows prove the EXTI/decode chain keeps up easily at ~1000 rpm. The
   C9007/C9008 tooth-angle warnings are diagnostics that do NOT reject teeth.

2. Hypothesis 2 (watchdog not fed): CONFIRMED as one real drop mode. 6 of 14
   blade drops carry `ec>=5 + wda_int=1` - the VDA answers landed outside the
   ~12.6 ms window (miss counter climbs), EC crossed 4 and the L9779 forced
   OUT1..4+IGN1..4 off. Session 2 was 4 of 5 mode B (dominant). The other 8
   drops show `ec=4 + wda_int=0` (healthy floor) - the PB13/3.3V-dip mode from
   the 2026-08-23 note. So the drops are bimodal; the miss counter is a
   l9779-driver-THREAD timing symptom under cranking/catch load, not EXTI/ISR.

3. MFS LTFT write flushed mid-storm (new): `MFS: Writing storage ID 3 ...
   2048 bytes` executed inside `newerr` storms at 103717.963 (36 ms) and
   103846.092 (38 ms) at 3700-4500 rpm. The deferral gate
   `custom_board_allowFlashNow = directSelfStimulation || isStopped()` opened
   because `isStopped()` flapped true during desync. Harmless at 36 ms, but the
   same gate admitting a GC-erase write (the status-2 ~2.3 s writes seen in the
   log) mid-storm is the 14:39 NT-clock wedge signature. The gate is not
   storm-proof.

Validation: numbers above were produced by grepping the two console logs and by
parsing the two console-written MLG files with a Python parser
(misc/mlg2csv/MlgToCsv.java documents the MLVLG v2 format used). No firmware
changes in this entry - analysis only; the durable facts were folded into
CLAUDE.md (new "trigger-error taxonomy" and "blade-drop UPDATE" sections).

Open follow-ups (hardware, not done):
- `lockstats` + `printPwmStats` during a 4500 rpm pull and during cranking (no
  lockstats output exists in these logs).
- `l9779` debug line during cranking: does `wd_delay_ms` walk out of the 60-150
  clamp, does `miss` climb - and what delays a NORMALPRIO+12 thread past 12.6 ms.
- L9779 VRS ramp step 3 (REG5=0x0F) is the prime suspect for the high-rpm tooth
  loss: try a high-rpm step-down of the hysteresis floor (keep 0x0E above ~3000
  rpm) or re-enable software debounce for sub-125 us bursts only.
- Buzz Q5B/R20 (ETC_WD chain) - the ec=5/wda_int=1 events correlating with
  DIS-high strongly suggest that chain IS populated, contradicting the earlier
  "likely depopulated" note.

Update same day (user hardware input + fix plan):

- USER FACT: the blade enable chain is supplied by BOTH the L9779 and the AT32;
  if either stops supplying the blade dies. The user probed the board and
  determined it is the L9779 side that stops supplying in the observed drops ->
  Mode A is an L9779-side rail/ETC_WD problem, and the Q5B/ETC_WD chain is
  populated. Recorded in CLAUDE.md.
- The ДПКВ path also lives inside the L9779 (VRSP/VRSN -> OUT_VRS -> 74HC14 ->
  PF8), so a L9779 power loss kills the crank signal (and IGN1..4 spark) while
  the MCU keeps running - matches the C9002/C9003 right after the drops.
- The MFS "storage ID 1/2" writes are TS burns from the connected tool (84
  burn cycles, each preceded by "TS -> Burn, waiting for CRC"), NOT fuel trim
  (LTFT = the separate 2048-byte ID-3 writes). The firmware writes both config
  copies unconditionally - no unchanged-skip - so every burn costs 165-168 ms
  (2.3 s when MFS GC fires). LTIT is disabled in the tune.

Implemented same day:

- WDA feed moved from the l9779 driver thread to the TIM5 executor
  (`firmware/hw_layer/drivers/gpio/l9779.cpp`): self-rescheduling event
  `l9779WdaFeedExec` -> `wdFeedFromExecutor` -> `wd_feed_isr`, ISR-safe polled
  LLD SPI (`spiSelectI`/`spiUnselectI` + `spi_lld_polled_exchange`, no bus
  mutex). Thread-side SPI batches run under `CriticalSectionLocker`; the diag
  refresh is chunked (3 registers/pass) to bound the critical section. The
  feed is kicked after chip_init and self-heals through a silent chip reset.
  Validation: m74_9 BUILD SUCCESSFUL; ELF contains the trampoline,
  spi_frame_isr, wd_feed_isr and the "l9779wda" schedule string. On-car
  validation: lockstats otherCbStats now shows the WDA feed's dispatch
  lateness; the TLE9201 warning counters (ok/miss/ec) should stop climbing
  under cranking load.
- Decision (user): NO on-the-fly VRS re-config/ramp re-jerking while running -
  the L9779 must work without resets; hardware owns the power-drop root cause.

Open follow-ups (unchanged):
- `lockstats` during a 4500 rpm pull and during cranking (feed lateness now
  visible in otherCbStats).
- CRC-compare skip in `writeToFlashNowImpl()` so identical TS burns cost
  nothing (kills the 168 ms / 2.3 s stall class).
- L9779 VRS ramp step 3 (REG5=0x0F) as the high-rpm tooth-loss suspect (no
  runtime re-jerking: any change is a static config/ramp table change only).

## 2026-08-23 - m74_9: ETB throttle PWM moved off the executor + soft-PWM load telemetry

Goal: find and remove the scheduling latencies behind the 2026-08-23 16:50 lockstats
(~29% of all executor events >=10us late, max 370us). Root cause of 100% of the
lateness: a single self-rearming callback (othercb 0806DA45, 63130/63171 events) -
the signature of a soft-PWM generator on the microsecond executor (TIM5). On a
bench with spark/fuel=0 the main suspect was the ETB throttle PWM (etbFreq=800Hz)
on PB14, which has NO hardware-PWM path here and fell back to software PWM.

What was done:

| Item | File(s) |
|------|---------|
| ETB PWM => hardware TIM12_CH1 (PB14 = AF9) when `startSimplePwmHard` is called | `hw_layer/ports/stm32/stm32_pwm.cpp` (getConfigForPin B14 case), `hw_layer/ports/at32/at32f4/cfg/mcuconf.h` (STM32_PWM_USE_TIM12=TRUE + STM32_TIM12_SUPPRESS_ISR) |
| Per-channel soft-PWM executor-load counter + intrusive channel list | `controllers/system/timer/pwm_generator_logic.h/.cpp` (executorFireCount, m_nextInPwmList, g_pwmList) |
| `printPwmStats()` table (name/hard-soft/freq/softPwmEvents) wired into lockstats | `controllers/system/timer/pwm_generator_logic.cpp`, `config/boards/m74_9/board_configuration.cpp` |

Key decisions:
- PB14 on AT32F435 has NO plain timer channel: only complementary TIM1_CH2N /
  TIM8_CH2N (which rusEFI's stm32_hardware_pwm cannot drive - no CCxNE/MOE
  support) and TIM12_CH1. TIM12 is the only clean hardware-PWM option: it is a
  plain 2-channel APB1 GPT, supported by the reused TIMv1 PWM LLD (PWMD12,
  has_bdtr=false), and free on m74_9. ISR suppressed (ETB driver only writes
  CCR via pwm_lld_enable_channel, never uses notifications).
- No gate/stop of ETB PWM on engine-off was added: the flap must stay alive
  always; the hardware path removes the executor load by itself.
- Why the old diagnostics could not name the offender: every soft-PWM channel
  shares the single static `PwmConfig::timerCallback`, so the executor's
  otherCbStats (dedup by address) collapses them into one entry. printPwmStats
  counts per channel to disambiguate.

Validation: m74_9 firmware builds (BUILD SUCCESSFUL, linker shows PWMD12 +
printPwmStats + the "PWM channels (softPwmEvents=" rodata string in the final
ELF); unit tests build and PWM.testPwmGenerator + 47 DcMotor/etb tests pass.
On-car: flash and run lockstats - the sched othercb line for timerCallback should
vanish/fall and the new "PWM channels" block should show the ETB as hard/stopped
(executorFireCount=0) instead of soft.

Open: confirm on hardware that PB14 TIM12_CH1 AF9 toggles the TLE9201 enable
identically to the soft-PWM output (same active-high polarity, same duty), and
re-measure lockstats with the engine running.

## 2026-08-14 - m74_9: IMMO OFF flag landed + confirmed against original ECU

Added the `m74_9ImmoOff` configuration bit and confirmed that the original ECU
silences the IMMO CAN exchange when the immobilizer is disabled in calibration.

What was done:

| Change | File |
| --- | --- |
| Added `bit m74_9ImmoEnabled` and `bit m74_9ImmoOff` to persistent config | `firmware/integration/rusefi_config.txt` |
| IMMO state machine uses both bits; no trigger/response when disabled | `firmware/config/boards/m74_9/m74_9_can.cpp` |
| Updated analysis doc with IMMO OFF confirmation and build fix note | `docs/m74_9_immo_analysis.md` |

Key findings:

- **IMMO OFF dump proves 0x08074BF9 is the hardware enable flag.**
  With `0x08074BF9 = 0x00` the original ECU never sends `0x0713` and ignores `0x0714`.
  BCM starts the engine without authentication.

- **rusEFI `m74_9ImmoOff` replicates this behavior.**
  `isImmoEnabled()` returns `m74_9ImmoEnabled && !m74_9ImmoOff`.
  When false, the state machine stays in `Idle`, does not arm the trigger timer,
  and discards incoming `0x0714` frames. `m74_9_isImmobilizerBlocking()` is also gated
  by the same condition, so LimpManager does not cut fuel/ignition.

- **Build error "no member named m74_9ImmoEnabled" was a stale generated header.**
  `engine_configuration_generated_structures_m74_9.h` had not been regenerated after
  the `rusefi_config.txt` edit. `touch firmware/integration/rusefi_config.txt` followed
  by `make clean` regenerates the header and the build passes.

Validation:
- Verified `m74_9ImmoEnabled` and `m74_9ImmoOff` appear at offset 15768 bits 11/12 in
  `engine_configuration_generated_structures_m74_9.h`.
- Verified `m74_9_can.cpp` references both bits in `isImmoEnabled()` and
  `m74_9_isImmobilizerBlocking()`.
- No firmware build run in this session (user builds with `./compile_m74_9.sh`).

Open follow-ups:
- User to build and flash; verify engine starts with `m74_9ImmoOff = yes`.
- Continue reverse-engineering the `0x0713` trigger generator and `computeImmoResponse()`
  algorithm for users who want to keep IMMO enabled (`m74_9ImmoEnabled = yes`).

---

## 2026-08-14 - m74_9: Static binary analysis - 0x0713 crypto significance + SLib call map

Python static analysis of the full 4 MB flash dump (`find_trigger_gen.py`).

What was done:

| Change | File |
| --- | --- |
| New static analysis script | `find_trigger_gen.py` |
| Updated analysis: corrected BLX 0x082078D4, new SLib call map, trigger crypto hypothesis | `docs/m74_9_immo_analysis.md` |

Key findings:

- **0x0713 trigger IS crypto-significant.** BCM returns Frame1[0] with bit 0x40 (RESPONSE
  format) for ALL 6 original-ECU triggers (0x66, 0xCF, 0x4A...) but NOT for rusEFI's
  fixed counter (`0x26`, no 0x40 bit). Probability of 6/6 coincidence ~1.5%.
  BCM validates the trigger content before sending a proper challenge.

- **CORRECTED: BLX R3 at 0x082078D4 does NOT call SLib.**
  The LDR before it loads 0x08209A60 (a wrapper-internal function pointer table).
  That table contains 8 wrapper addresses (0x08205F59...0x082062E9),
  all in 0x082xxxxx (flash attestation functions). The breakpoint is useful
  for catching flash-attestation calls but will NOT reveal a SLib crypto address.

- **36 direct BL/BLX-to-SLib calls found in main firmware (0x080xxxxx).**
  None are in the IMMO wrapper (0x082xxxxx). SLib is called via Thumb2 immediate-offset
  BL (F000+F8xx encoding), not via SRAM function pointer tables.
  All 36 call sites identified; 36 different SLib entry points.

- **Most likely trigger generator: 0x08069028 and 0x080697D0** (both in same 4 KB page).
  These are the only SLib calls near CAN-related code, calling SLib 0x081F102A
  and 0x0819AFEE respectively.

- **IMMO response (flash attestation) does NOT require SLib.** The whole computation
  chain (FUN_08201E2C -> FUN_0820630C -> FUN_08206108 -> FUN_082056D4) lives in the
  readable wrapper. If the algorithm is indeed flash-attestation, rusEFI can implement
  it without calling SLib at all (provided SLib is preserved at 0x08100000-0x081FFFFF
  so the CPU reads real flash content, not 0xFF).

- **SLib call hotspot: 0x08089xxx-0x0808Cxxx (15 calls to 0x08109xxx-0x0810Dxxx).**
  Likely a crypto engine (AES/HMAC) used for something other than IMMO protocol.

Validation:
- Python script confirmed: MOVW Rn, #0x713 = NOT present in flash.
  CAN ID 0x713 is encoded in a message descriptor table or passed via HAL parameter.
- Shifted CAN ID 0x0713 (0xE2600000) = NOT in literal pools.
- Shifted CAN ID 0x0350 (0x6A000000) = FOUND at 3 locations in main firmware,
  confirming that BCM-keepalive sender uses literal-pool CAN IDs while IMMO sender uses tables.
- 0x08209A60 table decoded: 8 function pointers, all in 0x082xxxxx wrapper range.

Open follow-ups:
- Set breakpoints at 0x08069028 and 0x080697D0 on original ECU to catch
  SLib trigger-generation calls. Read R0..R3, LR before BL; read SRAM output after return.
- Alternatively: connect BCM on bench -> natural exchange -> catch trigger gen in
  SLib call sites above.
- Verify flash-attestation hypothesis: parse known challenge Frame1+Frame2 bytes to
  extract flash_start/flash_size parameters; read those flash regions in dump;
  check if result matches the known 8-byte response.
- Add new script `verify_attestation.py` to test the hypothesis against all 5 pairs.

---

## 2026-08-14 - m74_9: SWD live debug session - timing/format of IMMO challenge exchange found

Extended SWD/OpenOCD investigation on the original Itelma I865LB52 ECU on the bench.
Goal: hit breakpoint at `0x082078D4` (BLX R3 in FUN_08207874, the SLib entry call)
to capture the address of the SLib crypto function.

What was done:

| Change | File |
| --- | --- |
| New bench PCAN trace captured without BCM | `stol.trc` |
| Auto-respond script for timed 0x0714 reply | `immo_respond.py` |
| Updated current-status section in analysis doc | `docs/m74_9_immo_analysis.md` |

Key findings:

- **0x0713 trigger is random per power cycle.**  The original ECU generates a fresh
  8-byte trigger (likely from the AT32F435 TRNG) every time it boots and sends it in
  the 0x0713 frame.  The BCM computes Frame1+Frame2 of the 0x0714 challenge from
  this trigger using an unknown algorithm.  rusEFI sends a fixed session-counter
  trigger; the two are not interchangeable.

- **Root cause #1 - timing.**  Analysed all 5 trigger->challenge pairs in
  orig_1/2/3.trc and ignon*.trc.  The BCM sends 0x0714 Frame1 within
  1.2-10.3 ms (avg 7 ms) of receiving 0x0713.  The ECU accepts the challenge
  only within this same window.  Manual PCAN-View clicking takes >100 ms -> always
  misses the window.  Fix: cyclic PCAN transmission (CycleTime=1/2 ms) or the
  `immo_respond.py` python-can auto-responder (responds in < 2 ms after seeing 0x0713).

- **Root cause #2 - wrong Frame1 format.**  User was sending Frame1 =
  `26 17 14 F0 94 E7 29 7F` (byte[0] = 0x26, no 0x40 bit).  This is what the BCM
  sends to *rusEFI*; it is a REQUEST-style frame.  The original ECU expects a
  RESPONSE-style frame (bit 6 of byte[0] set).  In all 5 known original-ECU pairs,
  Frame1[0] is 0x66, 0xCF, or 0x4A - all have the 0x40 bit set.  The original ECU's
  CAN ISR discards Frame1 without this bit, so FUN_082027A4 is never called.
  Correct data to use: Frame1=`66 0B E1 E2 A3 4B 81 40`,
  Frame2=`B4 56 33 A0 49 9A 01 EC` (pair 1 from orig_1.trc).

- **ECU does NOT validate challenge against its own trigger.**  FUN_08201E2C only
  checks key_type==1 and session_word==0xFF00/0xFF01 (from the SRAM buffer populated
  by the CAN ISR).  The 8-byte trigger in 0x0713 is sent outward to the BCM and is
  not stored for backward verification.  Any correctly-formatted 0x0714 (right
  byte[0] format) should be accepted regardless of which trigger triggered it.

- **0x0713 sent autonomously.**  stol.trc (bench, no BCM) shows the ECU sends
  0x0713 exactly once at t=9863.5 ms without needing 0x0350 from BCM.  Sending
  0x0350 is NOT required to get 0x0713.  After no response arrives the ECU stops
  retrying (within the trace window).

- **SWD direct call attempt (summary).**  Multiple attempts to call FUN_082027A4
  directly via OpenOCD (set PC, SP, LR, resume) failed due to: (a) being in Handler
  mode after HardFault, (b) FUN_08202038 not yet complete (SRAM[0x200002CA]=0,
  function pointer at SRAM[0x200010A0] not set), (c) watchdog resets during halt.
  Mitigation: `mww 0xE0042008 0x00001800` freezes IWDG while halted on AT32F435;
  must be written WHILE CHIP IS RUNNING (not after halt); wait >=5 s after reset
  before halting to allow init to complete.

Validation:
- `stol.trc` parsed: exactly 1 x 0x0713, 0 x 0x0714, 0 x 0x0350 confirmed.
- 5-pair timing distribution verified: min 1.2 ms, max 10.3 ms, avg 7.0 ms.
- Frame1[0] bit-pattern confirmed across all 5 original-ECU pairs (0x40 bit always set).
- No firmware build run (algorithm still unknown; stubs in place).

Open follow-ups:
- Change PCAN cyclic data to pair-1 format (`66 0B E1 E2 A3 4B 81 40` /
  `B4 56 33 A0 49 9A 01 EC`) and confirm BP at `0x082027A4` fires.
- If `0x082027A4` fires but `0x082078D4` does not: check SRAM[0x200002C8]
  (0 = SLib init path, 1 = flash-attestation path via FUN_0820630C).
- If no BP fires at all: investigate CAN ISR (set BP at `FUN_08206FB8` or
  watch SRAM[0x20000414] for writes after sending 0x0714).
- Cleanest option: connect BCM on bench (or test in car with JTAG) so the real
  BCM computes the correct Frame1+Frame2 for each trigger automatically.
- Once SLib entry address (R3 at 0x082078D4) is known, try `mdw <addr>` -
  readable -> disassemble; not readable -> capture input/output at BLX site.

---

## 2026-08-13 - m74_9: IMMO dead-lock fix + deep firmware reverse-engineering

Found and fixed the root-cause dead-lock that prevented the engine from starting
with rusEFI.  Also performed extensive static analysis of the original
I865LB52 firmware to understand the crypto algorithm.

What was done:

| Change | File |
| --- | --- |
| Fixed dead-lock: IMMO trigger fires on IGN ON (byte4!=0x00) not crank-switch (byte4==0x44) | `firmware/config/boards/m74_9/m74_9_can.cpp` |
| Increased trigger delay from 100 ms to 1000 ms (original ECU sends at ~1.5 s) | `firmware/config/boards/m74_9/m74_9_can.cpp` |
| Removed unused `crankSwitch` variable (would cause compiler warning) | `firmware/config/boards/m74_9/m74_9_can.cpp` |
| Rewrote "Current status" and "Next steps" in analysis doc | `docs/m74_9_immo_analysis.md` |
| Added analysis scripts | `emulate_immo.py`, `find_crypto_fn.py`, `trace_init.py`, `crypto_decode.py`, `deep_trace.py`, `find_algo_final.py`, `trace_fn6108.py`, `find_key_algo2.py`, `find_can_handlers.py` |

Key decisions and findings:

- **Dead-lock confirmed**: BCM never sets byte4=0x44 (crank-switch) until IMMO
  is cleared.  rusEFI was waiting for 0x44 to send the trigger.  BCM was waiting
  for the trigger before sending the challenge.  Nobody moved.  The dashboard
  shows correct lamp states because keepalive frames (0x0189 etc.) are correct,
  but the starter relay GND never closes.
- **Fix**: arm IMMO timer on any non-zero byte4 from BCM (= ignition on).  Trigger
  fires after 1000 ms, before user turns key to START.
- **Crypto algorithm**: Not cracked yet.  Proprietary, uses ADD/XOR/shift with a
  key from calibration area.  No standard constants (AES, SHA, XTEA, CRC32)
  found.  Async flash-attestation chain identified:
  `FUN_08201E2C` -> `FUN_0820630C` -> `FUN_08206108` -> `FUN_08205A3C` -> flash ISR.
- **CAN receive chain**: `CAN1_RX0_IRQ` -> `FUN_08207432` -> `FUN_08206FB8` ->
  per-frame handler via FMI index -> SRAM buffer at 0x20000C14.
- **Trigger format**: 8 random bytes; BCM accepts any content and responds.
  rusEFI's rolling-counter trigger is fine.

Validation performed:
- Traced PCAN captures: `rusefi2.trc` shows NO 0x0713/0x0714 frames (confirming
  the old code never sent the trigger).  `ignon.trc` shows trigger at ~1521 ms
  and successful full exchange on original ECU.
- Verified that `ignon_and_start.trc` pair 5 (challenge 66aaeef3... / resp efaa66f0...)
  matches `PAIRS16[4]` in the analysis scripts.
- No firmware build was run (macOS host, needs Linux ARM cross-compiler; user
  must build with `./compile_m74_9.sh` on Linux).

Open follow-ups:
- Deploy trigger fix -> capture new PCAN trace -> verify 0x0713/0x0714 exchange.
- Run Unicorn emulation with actual challenge from new trace to find the response.
- OR: SWD breakpoint at 0x082027FA on original ECU to intercept the algorithm.
- Implement `computeImmoResponse()` once algorithm is known.

---

## 2026-08-13 - m74_9: updated `docs/m74_9_immo_analysis.md` with implementation progress

Consolidated the current state of the m74_9 immobilizer work into `docs/m74_9_immo_analysis.md`. Added a new "Implementation progress in rusEFI" section documenting the landed state machine, logging, `m74_9ImmoEnabled` config bit, and `custom_board_isImmobilizerBlocking` LimpManager hook. Also added "Next steps and blockers" so the next session can pick up from the right place.

What was done:

| Change | File |
| --- | --- |
| Documented the implemented IMMO state machine, logging helpers, public accessors, and fuel/spark cut integration | `docs/m74_9_immo_analysis.md` |
| Documented the `m74_9ImmoEnabled` config bit and its default-off safety behavior | `docs/m74_9_immo_analysis.md` |
| Added next-step checklist: decompile response functions, port algorithm, build/test, decide on default | `docs/m74_9_immo_analysis.md` |

Key decisions:

- Keep `docs/m74_9_immo_analysis.md` as the single source of truth for both the original-firmware reverse-engineering notes and the rusEFI integration status.
- The response algorithm remains the only blocker; all infrastructure around it is in place.

Validation: Reviewed the generated header and TS ini to confirm `m74_9ImmoEnabled` and `ClearReason::Immobilizer` are present. No firmware build was run (still blocked on macOS host tooling; will build on user's Linux environment).

Open follow-ups:

- Run `./compile_m74_9.sh` on a Linux host to verify the current changes compile.
- In Ghidra, find the functions whose addresses are stored at `0x20001A50` and `0x20001A54` and decompile them.
- Implement `computeImmoResponse()` / `computeImmoQuickResponse()` once the algorithm is known.

## 2026-08-13 - m74_9: immobilizer dispatcher decompiled, literal pool mapped

Continued reverse-engineering the m74_9 immobilizer. The Ghidra decompilation of the dispatcher at `0x08203FFC` is now in `docs/m74_9_immo_analysis.md`. It is a two-case state machine:

- `param_1 == 0` -> calls the function pointer stored at RAM `0x20001A50` (likely the full 16-byte challenge response).
- `param_1 == 1` -> calls the function pointer stored at RAM `0x20001A54` (likely the quick 8-byte re-check response).

The literal pool at `0x08204080` was read from the full flash dump and maps to RAM addresses in the `0x20001Axx` region. The key constants `0x2548A4D2`, `0x4DF9123B`, `0x43A0C212`, `0xF9C74A52` are stored in the first flash bank at `0x080BB508` - `0x080BB577` and are copied to RAM `0x20001ADC` during initialization.

What was done:

| Change | File |
| --- | --- |
| Updated analysis notes with the decompiled dispatcher, literal pool mapping, and key-constant source addresses | `docs/m74_9_immo_analysis.md` |
| Extended `analyze_immo.py` to dump the literal pool and key-constant addresses for Ghidra orientation | `analyze_immo.py` |
| Added this report entry | `docs/report.md` |

Key decisions:

- The actual response functions are still not decompiled; they are the targets of the two RAM function pointers. The next work unit is in Ghidra.
- The second flash bank (`0x08200000`) must be mapped in Ghidra. If `0x08203FFC` is not visible, add a memory block at `0x08203F0C` or load the full 4 MB dump at `0x08000000`.

Validation: `analyze_immo.py` was run; it reproduced the 10 captured sessions and printed the literal pool / key-constant addresses. No firmware build was run (the crypto algorithm is still unknown).

Open follow-ups:

- Find the values stored at `0x20001A50` and `0x20001A54` in Ghidra and decompile those functions.
- Verify the decompiled algorithm against the captured challenge/response pairs.
- Port to `firmware/config/boards/m74_9/m74_9_can.cpp` and build/test.

## 2026-08-13 - m74_9: committed immo progress to local repo

Committed the current m74_9 immobilizer work as a single local commit (`04476901f44`). The commit includes the framework, the captured challenge/response test vectors, the `analyze_immo.py` helper, and the `docs/m74_9_immo_analysis.md` hand-off notes for Ghidra.

What was done:
| Change | File |
| --- | --- |
| Committed immobilizer framework + analysis + test vectors | `analyze_immo.py`, `docs/m74_9_immo_analysis.md`, `docs/report.md`, `firmware/config/boards/m74_9/*`, `firmware/controllers/limp_manager.*`, `firmware/hw_layer/board_overrides.h`, `firmware/integration/rusefi_config.txt`, `firmware/tunerstudio/tunerstudio.template.ini` |

Key decisions:
- No generated files or large binaries were committed. `immo_pairs.json`, `immo_code_region.bin`, and the original `.bin`/`.trc` files remain untracked.
- The next work unit is intentionally outside the repo: decompile the immobilizer response function in Ghidra and return with the C code for `computeImmoResponse()` / `computeImmoQuickResponse()`.

## 2026-08-13 - m74_9: immobilizer framework - logging, auth state and LimpManager integration

Analyzed two Largus tune files (IMMOON/IMMOOFF) and a full 4 MB flash dump from an m74_9 ECU with working immobilizer. The only functional difference between the two tunes is byte `0x074BF9` (`0x01` = immo enabled, `0x00` = disabled). The dump confirms this flag is in bank-1 flash calibration area and is copied to RAM `0x2000E25C`; the original firmware gates engine outputs through a function that checks this RAM flag.

Also extracted challenge/response pairs from PCAN `.trc` logs (`ignon.trc`, `ignon_and_start.trc`, `orig_1/2/3.trc`). The protocol matches the existing rusEFI state machine: ECU sends `0x0713` trigger, BCM replies with two `0x0714` challenge frames (16 bytes total), ECU answers with one `0x0713` response. Quick re-check uses a single `0x0714`/`0x0713` exchange. Simple XOR/DES/AES/MD5 models do not explain the captured responses, so the actual algorithm still has to be pulled from the original firmware (or bypassed if the BCM allows it with immo disabled).

What was done:
| Change | File |
| --- | --- |
| Added `isImmoAuthenticated()` to `M74_9BcmCanListener`, plus public `m74_9_immoAuthenticated()` / `m74_9_isImmobilizerBlocking()` accessors | firmware/config/boards/m74_9/m74_9_can.h, firmware/config/boards/m74_9/m74_9_can.cpp |
| Added hex logging for `0x0714` challenge frames (single + 16-byte), `0x0713` responses, and quick re-check, plus "IMMO: authenticated" state log | firmware/config/boards/m74_9/m74_9_can.cpp |
| Added `ClearReason::Immobilizer` and a board hook `custom_board_isImmobilizerBlocking`; when the hook returns true, `LimpManager::updateState()` cuts fuel and spark | firmware/controllers/limp_manager.h, firmware/controllers/limp_manager.cpp, firmware/hw_layer/board_overrides.h |
| Wired the m74_9 hook to `m74_9_isImmobilizerBlocking()` in board overrides | firmware/config/boards/m74_9/board_configuration.cpp |
| Added `m74_9ImmoEnabled` config bit to `rusefi_config.txt` and updated `fuelIgnCutCodeList` in the TS template; regenerated m74_9 config/ini | firmware/integration/rusefi_config.txt, firmware/tunerstudio/tunerstudio.template.ini, firmware/controllers/generated/* |

Key decisions:
- Default `m74_9ImmoEnabled` is `false` (bit-field default) so the ECU runs without immobilizer until the user enables it and a working `computeImmoResponse()` is in place. This avoids bricking the car while the crypto algorithm is still unknown.
- The LimpManager cut is dynamic: it reads the immobilizer state every fast callback, so once authentication succeeds the cut is removed automatically within 5 ms.
- The hook is generic (`custom_board_isImmobilizerBlocking`) so any other board with a similar external immobilizer can reuse it.

Validation: m74_9 config regenerated successfully (`gen_config_board.sh config/boards/m74_9 m74_9`); `m74_9ImmoEnabled` appears in the generated header and TS ini. Full firmware build could not be run locally: macOS `compile_m74_9.sh` fails on `realpath --`, and unit-test build fails on missing `flock`. User will build on his Linux environment.

Open follow-ups:
- Find the actual immobilizer response function in the original firmware (trace the CAN RX `0x0714` handler in Ghidra) and implement `computeImmoResponse()` / `computeImmoQuickResponse()`.
- Capture more challenge/response pairs and try statistical/crypto analysis if the firmware path stays hidden.
- Test on car whether disabling `m74_9ImmoEnabled` lets the BCM close the starter relay without a valid response.

## 2026-08-13 - m74_9: immobilizer analysis helper and captured test vectors

Continued reverse-engineering the immobilizer challenge/response. The original firmware immo module sits in the second flash bank around 0x08203F0C - 0x08204D00 (Ghidra base 0x08000000, ARM Cortex-M LE Thumb). Key functions identified: 0x08203FFC (immo dispatcher / state machine), 0x082047D0 (frame staging helper), 0x082048E8 (0x0714 receive handler), 0x08204B00 - 0x08204C00 (key constants / registration). The actual response computation still needs to be decompiled.

Added `analyze_immo.py` to parse all PCAN `.trc` files in the repo root and extract both full 16-byte challenge sessions and quick single-frame re-checks. The full sessions confirm the protocol: ECU `0x0713` trigger -> BCM two `0x0714` frames (16-byte challenge) -> ECU `0x0713` response. The quick pairs are single `0x0714` challenge -> `0x0713` response. All captured pairs are now saved to `immo_pairs.json` and embedded as test vectors in `m74_9_can.cpp`.

Also verified the IMMOON/IMMOOFF Largus tune difference: the functional flag is byte `0x074BF9` (`0x01` = enabled, `0x00` = disabled). The other four differing bytes (`0x07FFFC` - `0x07FFFF`) are the calibration-area checksum, as expected when one byte changes.

What was done:
| Change | File |
| --- | --- |
| Added `analyze_immo.py` to parse `.trc` logs, extract full/quick challenge-response pairs, save JSON, diff IMMOON/IMMOOFF, and dump the immo code region for Ghidra | `analyze_immo.py` |
| Updated `computeImmoResponse()` / `computeImmoQuickResponse()` stubs with captured test vectors and the correct original-firmware offsets to decompile | `firmware/config/boards/m74_9/m74_9_can.cpp` |

Key decisions:
- The response algorithm is still unknown; simple XOR/DES/AES/MD5 models do not fit the captured pairs. The next step is to decompile the relevant original-firmware functions (especially the dispatcher and any crypto helper called from it) in Ghidra.
- A helper script is the fastest way to keep adding real test vectors as the user captures more logs; once the algorithm is known, the captured pairs become unit-test inputs.

Validation: `analyze_immo.py` runs successfully and produces consistent pairs from all five `.trc` logs. No firmware build attempted locally; the changed code is comments/stubs only.

Open follow-ups:
- Decompile the immo response function in the original firmware and implement `computeImmoResponse()` / `computeImmoQuickResponse()`.
- Capture more sessions with different keys/keys to confirm the algorithm once a candidate is found.
- On-car test whether `m74_9ImmoEnabled = false` is enough for the BCM to allow starting without a valid response.

## 2026-08-12 - m74_9: ETB (TLE9201) revived - the missing piece was the ETC_EN enable chain, not SPI

Throttle now moves on the bench; user confirmed. The blocker from the previous session ("throttle does not move even though TLE9201 diag is clean") traced to the hardware enable: TLE9201 DIS (pin 11) sat at +5V, holding the bridge in tristate ("Outputs disabled", diag EN bit 0x80 = 0). The enable chain on the board is PB13 (ETC_EN) -> Q5A (MUN5311DW1 NPN, inverts) -> DIS (pulled up to +5V via R23), so the MCU-side polarity is ACTIVE-HIGH: PB13 high = Q5A on = DIS low = bridge enabled. PB13 was never driven (board.h leaves it a weak-pullup input).

What was done:
| Change | File |
| --- | --- |
| Drive PB13 (ETC_EN) high as plain GPIO in `m74_9_boardInitHardware()` (gpio_pin_markUsed + PAL_MODE_OUTPUT_PUSHPULL + palSetPad), right before `tle9201_add()` | firmware/config/boards/m74_9/board_configuration.cpp |
| Document in `setupEtb()` why `etbIo[].disablePin` is NOT used: the disable-pin path (DcHardware::start -> OutputPin::initPin(msg, pin)) is fixed OM_DEFAULT (pin low = enable), the inverse of what this board needs; assigning disablePin = B13 would drive DIS high (tristate) whenever the firmware thinks the bridge is enabled | firmware/config/boards/m74_9/board_configuration.cpp |

Key decisions:
- Static high on ETC_EN instead of a config field: there is no `disablePinMode` anywhere (checked `dc_io` in rusefi_config.txt), so inverting the disable pin would need a new config field plus full config regen - overkill for one board bring-up. Runtime disable is PWM=0, on which the TLE9201 coasts (real coast, not drive).
- Boot is safe by construction: PB13 weak pullup (~40k) against Q5A's internal 10k base-emitter divider gives ~0.66V at the base - below Vbe, Q5A stays off, DIS pulled high -> tristate until firmware drives PB13. Measured DIS=0.2V (Q5A Vce(sat)) after the fix.
- The earlier hypothesis (KiCad "DIS -> +3V3 via Q5A/R20") was wrong for the physical board: DIS is pulled to +5V (R23) and Q5A is a real NPN, not open-drain logic. Schematic netlist Y-positions were unreliable; trust datasheet pinout + user's 0-ohm measurements.

Validation (user on bench): DIS = 0.2V vs GND; TLE9201 diag transition 0x5C (Outputs disabled / Open Load, power-on tristate) -> 0xDF (No failure, EN bit set); `etbautocal` passes - throttle sweeps open/close, no "Auto calibrate failed", TPS1 closed/open voltages now differ.

Open follow-ups:
- On-car: confirm direction (if inverted, `stepperDcInvertedPins=true`, issue #4579), re-check TPS divider coefficients (analogInputDividerCoefficient=2.0 read 5.4V closed / 1V open on car), TPS2 warnings (P0223 Tps1Secondary too high, P2135 inconsistent - TPS2 divider may differ from TPS1), P2137 pedal inconsistent.
- The L9779 WDA -> Q5B -> DIS path is a redundant hardware kill (L9779 algorithmic watchdog); firmware feeds the watchdog fine (WDA ok>0 fail=0) and the path is likely depopulated ("not soldered" note near R20).

## 2026-08-11 - m74_9: fix event-scheduler death on firmware error (WDT TIM5 latch during ETB autocal)

User log (2026-08-11_22_47): during ETB TPS autocal the scheduler died - `WDT regs: cnt` still counting at 4 MHz, `dier 0x2 -> 0x0` (CC1IE cleared and never re-enabled), `isr`/`setHw` counters frozen, `pend=0`. Root cause found in code, not in a hardware race:

- `MicrosecondTimerWatchdogController` fires when `setHardwareSchedulerTimer()` has not been called for 2 s.
- `setHardwareSchedulerTimer()` had a `if (hasFirmwareError()) return;` gate (added 2019, #996 debugging). The STM32 ISR (`hwTimerCallback` in microsecond_timer_stm32.cpp) disables the compare notification (CC1IE) *before* invoking the callback, and `portSetHardwareSchedulerTimer()` is the *only* code that re-enables it. So any `firmwareError()` (e.g. the ETB autocal's "Auto calibrate failed" critical error) made the very next timer ISR the last one ever: the whole event scheduler died (soft PWM, watchdog buddy, all scheduled events) until power cycle. This is deterministic, not a race.

What was done:
| Change | File |
| --- | --- |
| Removed the `hasFirmwareError()` early-return from `setHardwareSchedulerTimer()`; documented the CC1IE invariant. Scheduler now always re-arms; engine safety after fatal error is already handled by `LimpManager::fatalError()` (cuts ignition/injection/ETB/trigger), which `firmwareErrorV()` calls - the scheduler gate was redundant for safety and only served to brick the ECU until reboot | firmware/hw_layer/microsecond_timer/microsecond_timer.cpp |
| ETB TPS autocal failure downgraded `firmwareError()` -> `warning()`: failing a bench calibration is a wiring/tune issue the user must fix, not an unrecoverable condition; the ECU keeps running and calibration can be retried without a power cycle | firmware/controllers/actuators/electronic_throttle_impl.h |

Key decisions:
- The 2026-08-09 `compareMinDelta=4us` clamp is orthogonal - that fixes compare-written-in-the-past; this fixes the firmware-error path that never re-arms.
- WDT diagnostic output stays useful: with the gate gone, the watchdog only fires on genuine scheduler breakage (clock gate, missed compare, stuck ISR).

Validation: not compiled here (user builds m74_9 himself). Static cross-check: `firmwareErrorV()` -> `getLimpManager()->fatalError()` (error_handling.cpp:716); `EtbController::setOutput()` gates on `allowElectronicThrottle()` (electronic_throttle.cpp:552).

Open follow-ups:
- On the car, the throttle still does not move even though TLE9201 diag is clean (0x5F "No failure" with motor attached). Candidates: DIS (TLE9201 pin 11) pulled to +3V3 via Q5A/R20 per KiCad (bridge held disabled - verify R20 populated and ETC_EN wired to the MCU), and PWM/DIR pins (PB14/PB15 are not in the HW-PWM table, so ETB runs on soft PWM; KiCad rev says PC0/PC1, which collide with PPS1/PPS2).
- Confirm `stepperDcInvertedPins` (#4579) once the motor actually turns.

## 2026-08-11 - m74_9: enable TLE9201 ETB H-bridge diagnostics on SPI2 (AT32F435)

Goal (user): bring the electronic throttle alive - the TLE9201 (U3) H-bridge was not wired to any SPI and had no driver in firmware. Per KiCad dump `m749_kicad.txt` the only L9779-to-TLE9201 link is the WDA_INT watchdog line (L9779 pin 38 -> Q5B -> DIS), so diagnostics must be driven directly from the AT32 via SPI2, not through the L9779.

What was done:
| Change | File |
| --- | --- |
| `DDEFS += -DBOARD_TLE9201_COUNT=1` (driver stub otherwise returns -1) and `-DSTM32_SPI_USE_SPI2=TRUE` | firmware/config/boards/m74_9/board.mk |
| SPI2 enabled: `is_enabled_spi_2`, pins PD1/PD3/PD4 (ETC_SCK/SO/SI, AF6 per `spi2_af` in at32_spi.cpp), `tle9201_cfg` on `&SPID2` with CS PD0 (GPIO, idle high), `tle9201_add(0, ...)` + PD0 pin claim in `board_init_ext_gpios()` | firmware/config/boards/m74_9/board_configuration.cpp |
| Wrap `STM32_SPI_USE_SPI2` in `#ifndef` in the AT32 mcuconf (was a bare `#define ... FALSE`, so the board.mk `-D` redefinition would warn -> error under `-Werror`, and worse, the mcuconf value would override the command line one, leaving `SPID2` undeclared). Same pattern as stm32h7/cfg/mcuconf.h and the existing USART1/USB guards in this very file. | firmware/hw_layer/ports/at32/at32f4/cfg/mcuconf.h |

Key decisions:
- SPI2 pin/AF mapping verified against `at32_spi.cpp` `spi2_af` table: PD1 SCK AF6, PD3 MISO AF6, PD4 MOSI AF6 - exactly what the board needs (TLE9201 SO=3/SI=8/CSN=9/SCK=10 per user's pin list).
- CS handled the same way as the (working) L9779: GPIO push-pull output, idle high; ChibiOS `spiSelectI`/`spiUnselectI` in `SPI_SELECT_MODE_PAD` clear/set the pad (active low CSN) - no LLD work needed.
- `TLE9201_CONFIG_CR1/CR2` macros resolve on AT32: `device_mpu_util.h` comes from `hw_layer/ports/stm32/stm32f4` (CPU_HWLAYER for ARCH_AT32F4), where 8-bit mode is `0` (SPIv1 DFF=0).
- `etbFunctions[0] = DC_Throttle1` is already the default via `setDefaultEtbParameters()`; TPS1/PPS1 inputs and ETB PWM/DIR (PB14/PB15) were already configured, so no ETB-params change was needed.

Validation: not compiled here (user builds m74_9 himself; no arm-none-eabi-gcc on this host). Static cross-check done against at32_spi.cpp, SPIv1 LLD (SPID2 declared under STM32_SPI_USE_SPI2, RCC+DMA streams in spi_lld_start), spi.cpp getSckPin/initSpiModules and tle9201.cpp thread lifecycle.

Open follow-ups:
- After flash: expect console line `tle9201 Detected! (rev=...)` and periodic `tle9201 Diag` lines; `reset_9201` console command clears diag latch. If the thread stays silent, check that the saved MFS config has `is_enabled_spi_2=yes` and the three SPI2 pins set - stale saved settings override board defaults.
- If SPI2 still does not come up, next candidate is the DMA stream allocation: AT32 mcuconf uses `STM32_DMA_STREAM_ID_ANY` for SPI2, same as SPI1 (L9779) which works.
- Consider exposing ETB pin options (prepend.txt) and confirming the DIS disable-pin strategy with the watchdog line before finalizing the ETB control path.

## 2026-08-09 - m74_9 watchdog round 2: extended TIM5 snapshot with RCC gate + 100 ms delayed re-read

What was done:
- User report of the instrumented firmware: `cnt=80159147..80159147 dier=0x2 sr=0x0 cr1=0x485`
  at 4 MHz -> freeze at ~20 s after boot. cr1=0x485 = CEN+URS+ARPE+PMEN (exactly what
  pwm_lld_start writes on AT32), DIER bit 1 (CC1IE) armed, no pending compare, isr=28523 /
  setHw=28525 -> the ISR->reschedule chain ran perfectly until the freeze. Conclusion: timer
  registers are intact, TIM5 counter stops counting at runtime - classic signature of the
  peripheral clock being gated (RCC APB1ENR.TIM5EN cleared, e.g. by stray
  pwmStop()/gptStop()/icuStop() on PWMD5/GPTD5/ICUD5 -> rccDisableTIM5 in the TIMv1 LLD), or a
  deeper AT32 clock issue. The two identical adjacent CNT reads are only weak proof (a few
  cycles can elapse between reads)
- Extended the diagnostic in microsecond_timer.cpp MicrosecondTimerWatchdogController::PeriodicTask:
  - re-read CNT after chThdSleepMilliseconds(100) (thread context, no locks held) - prints
    cnt2 and the delta cnt2-cnt1, so cnt2==cnt1 after 100 ms now PROVES a frozen counter
  - added RCC APB1 clock-enable snapshot: apb1enr + tim5en bit (RCC->APB1ENR + RCC_APB1ENR_TIM5EN
    on AT32F4XX/STM32F4XX/STM32F7XX, RCC->APB1LENR + RCC_APB1LENR_TIM5EN on STM32H7XX - the H7
    register name differs; verified all four families' CMSIS headers). Zeroed default keeps the
    message safe if a future family defines SCHEDULER_TIMER_DEVICE without matching the #if
  - added SMCR/PSC/ARR/CCR1/CCMR1 so a stale compare window or slave-mode gate is visible
  - cypress/kinetis GPT port still gets the old message via the #else branch
- Clock-gate culprit sweep (rusEFI tree, outside ChibiOS): no pwmStop() callers at all; the
  only stop call is icuStop() in sent_hw_icu.cpp (SENT input, but STM32_ICU_USE_TIM5=FALSE);
  HAL_USE_ONEWIRE=FALSE on AT32 so ChibiOS-Contrib hal_onewire.c pwmStop(pwmd) is not built;
  direct RCC-> writes in-tree touch only AHB1ENR (USB disable at boot, stm32_common.cpp),
  APB2ENR (osc_detector.cpp, H7-only) and BDCR (F7 RTC) - none can clear TIM5EN at runtime

Validation:
- Edit is type-checked by hand (%d/%u/%x specifiers match the casted arguments, including the
  new (unsigned)(cnt2-cnt1) and (unsigned)tim5en); no firmware build possible on the macOS
  host (no arm-none-eabi toolchain) - user builds m74_9 on Windows

Follow-ups:
- User: rebuild m74_9 on Windows, flash, reproduce (~20 s), paste the new error text; also
  capture the last console lines right before the error (any Storage/burn/MFS/CAN-reconnect
  message, any `local freeze cnt=`)
- Reading apb1enr/tim5en: bit 3 (0x8) clear -> someone gated TIM5 clock -> correlate ~20 s
  wall-time with the log to find the culprit; bit still set but cnt2==cnt1 after 100 ms ->
  deeper AT32 clock issue, next print RCC->CR (PLLON/PLLRDY) and RCC->CFGR (SW) and consider
  sampling CNT every 1 s from status_loop to find the exact freeze moment
- cnt2 != cnt1 but watchdog still latched -> the freeze was transient; re-think executor/queue
  and the hasFirmwareError() early-return interplay

## 2026-08-09 - m74_9 watchdog critical: instrument MstWatchdog with TIM5 register snapshot

What was done:
- m74_9 (AT32F435, CAN-only) runs ~10-20 s after boot, then latches
  `CRITICAL error: Watchdog: no events for 2 seconds!` (firmwareError RUNTIME_CRITICAL_TIMER_WATCHDOG)
- The MstWatchdog thread (only started under EFI_EMULATE_POSITION_SENSORS, inherited TRUE from
  stm32f4ems/efifeatures.h) fires when setHardwareSchedulerTimer() has not updated
  lastSetTimerTimeNt for 2 s. The watchDogBuddy 1 s self-rescheduling event keeps that chain
  alive, so a trip means the TIM5 compare-interrupt -> executor loop died at runtime. Console
  CAN and status_loop keep running after the error, so it is NOT a global IRQ stall - something
  specific to the TIM5 CC1 path on the AT32 port
- Static verification of the chain: PWM driver is the STM32 TIMv1 LLD compiled for AT32
  (AT32 platform.mk reuses STM32/LLD/TIMv1/driver.mk), STM32_TIM5_HANDLER=Vector108 (IRQ 50,
  vector table offset 0x108), STM32_PWM_TIM5_IRQ_PRIORITY=4 from the new at32 interrupt_priority.h,
  AT32_TIM_CR1_PMEN 32-bit mode set in pwm_lld_start. Nothing else on this board uses PWMD5/TIM5
  (A0-A3 -> PWMD5 mapping in stm32_pwm.cpp is unused by the m74_9 default config)
- Added diagnostics to firmware/hw_layer/microsecond_timer/microsecond_timer.cpp
  MicrosecondTimerWatchdogController::PeriodicTask: the error message now snapshots
  timerCallbackCounter (ISR count), setHwTimerCounter, timerFreezeCounter, isTimerPending and
  raw TIM5 CNT x2 / DIER / SR / CR1 at the moment of detection. Distinguishes timer clock dead
  (CNT frozen), compare IRQ dead (CNT counting, DIER bit 1 / CC1IE cleared), notification never
  re-armed (pending=0) vs executor/queue wedged (counters still growing). Guarded by
  #ifdef SCHEDULER_TIMER_DEVICE so the cypress/kinetis GPT port keeps the old message

Validation:
- Edit is type-checked by hand (%d/%u/%x specifiers match the casted arguments); no firmware
  build possible on the macOS host (no arm-none-eabi toolchain) - user builds m74_9 on Windows

Follow-ups:
- User: rebuild m74_9 on Windows, flash, reproduce (~20 s), capture the new watchdog error text
  with isr=/setHw=/freeze=/pending=/cnt=/dier=/sr=/cr1= values
- Reading the values: CNT frozen -> TIM5 clock stopped; DIER without CC1IE (bit 1) -> compare
  interrupt never re-armed; counters still growing -> executor/queue issue, not the timer

## 2026-08-09 - PCAN ISO-TP decode root cause: decodePacket got the 127-byte buffer instead of the DLC

What was done:
- Fixed java_console/io/src/main/java/com/rusefi/io/can/PCanIoStream.java readOnePacket:
  `canDecoder.decodePacket(rx.getData())` -> `decodePacket(rx.getData(), rx.getLength())`
- Root cause of the m74_9 (AT32F435, CAN-only, no USB) "No signature returned by
  PCanIoStream{PCAN_USBBUS1}" blocker: TPCANMsg(Byte.MAX_VALUE) allocates a 127-byte data
  buffer (workaround for issue #4370) and the PCAN driver fills only rx.getLength() (DLC=8)
  bytes; the deprecated decodePacket(byte[]) overload passed data.length (127) instead, so the
  ISO-TP multi-frame state machine and the CRC check read garbage zero padding. Symptom seen in
  the log: `CRC mismatch on recv packet for [hello]: got 309f7bb9 but expected 0` (CRC field all
  zeros) even though the 0x720 frame itself was delivered fine (`isotp rate 1`). The size-aware
  overload exists exactly for this - its javadoc names PCAN as the larger-data-buffer example
- Checked the other decoders: SocketCANHelper.read allocates `new byte[rx.getDataLength()]` so
  SocketCANIoStream.payload() is exact-length and unaffected; Elm327IoStream gets real-length
  arrays from the ELM327 chip. Only the PCAN path was broken

Validation:
- ./gradlew :ecu_io:test BUILD SUCCESSFUL (JDK 11 toolchain via
  -Porg.gradle.java.installations.paths=...)

Follow-ups:
- User rebuilds java_console on Windows and connects to m74_9 over PCAN-USB: expect
  `Got [rusEFI ...] signature` / Connection established instead of the CRC-mismatch loop

## 2026-08-09 - PCanIoStream log flood fix (INFO_SKIP_RATE typo)

What was done:
- Fixed INFO_SKIP_RATE typo in java_console/io/src/main/java/com/rusefi/io/can/PCanIoStream.java:
  `3-00` evaluated to 3 (three minus zero), so the "Skipping non 720 packet" log fired every
  3rd skipped frame - at ~3600 pps on the m74_9 test bus that is ~1200 lines/sec of noise,
  making the PCAN debug log unreadable. Now 300 (log every 300th skipped frame)

Validation:
- ./gradlew :ecu_io:test BUILD SUCCESSFUL (JDK 11 toolchain via
  -Porg.gradle.java.installations.paths=...)

Follow-ups:
- PCAN ISO-TP diagnosis continues: need "Skipping non 720 packet:" ID lines or PCAN-View
  capture to tell whether the m74_9 ECU announces 0x770017 (extended) on the probed bus

## 2026-08-09 - java_console PCAN detection fix + merge conflict resolution

What was done:
- Fixed java_console not detecting PCAN adapters on Windows: the PowerShell WMI query
  (`powershell -NoProfile -Command "Get-CimInstance ... -Filter \"Caption like '%PCAN-USB%'\" ..."`)
  was executed via Runtime.exec(String), whose tokenizer splits on whitespace and keeps quotes as
  literal characters, so the script arrived shredded and the query silently matched nothing
  (PCANConnected always false -> "OpenBLT CAN" menu item missing)
- Added ExecHelper.executeCommand(List<String>, ...) overload that runs commands through
  ProcessBuilder with an explicit argument list (platform-correct quoting, no shell tokenization)
- Converted all PowerShell hardware probes to the list form: PCAN (MaintenanceUtil), DFU
  (DfuFlasher, both H7 and F4 variants), ST-Link (StLinkFlasher), plus DfuFlasher.getDevicesReport
- Added MaintenanceUtilTest.pcanQueryIsSingleIntactPowerShellScriptArgument regression test
- Resolved the remaining merge conflicts (HEAD vs 075e600f): can_sniffer.cpp kept HEAD guard
  structure + theirs' explicit template instantiations; VariableRegistryValues.java took HEAD values

Key decisions:
- Root-cause fix at ExecHelper instead of patching the query string: the same broken
  Runtime.exec(String) quoting pattern was shared by PCAN/DFU/ST-Link detection
- Used Arrays.asList (not List.of): java_console still compiles with sourceCompatibility 8
- Generated VariableRegistryValues.java resolves to HEAD side; regenerated at next build anyway

Validation:
- Empirical check on macOS: a fake powershell argv dump showed Runtime.exec(String) splits the
  script into 15 broken tokens, while ProcessBuilder passes it as one intact argument
- ./gradlew :ui:test --tests MaintenanceUtilTest: BUILD SUCCESSFUL, 14 tests passed (JDK 11
  toolchain via -Porg.gradle.java.installations.paths=/opt/homebrew/opt/openjdk@11/...)

Follow-ups:
- WMIC_* constant names now hold PowerShell arg lists; renaming to PS_* would be cosmetic

## 2026-07-29 - m74_9 (AT32F435) firmware build fixes

What was done:
- Fixed build of m74_9 board (previously disabled, meta-info.disabled_env -> meta-info.env)
- Fixed gcc_version_check.c to support GCC 16 (limit 1700)
- Created AT32 interrupt_priority.h with EFI_IRQ_* priority macros
- Fixed SPI v1 struct field names (end_cb instead of slave/data_cb/error_cb) used by old AT32 port
- Fixed usbcfg.h to guard USB type declarations with #if HAL_USE_USB
- Disabled EFI_STORAGE_INT_FLASH for m74_9 (no flash storage driver for AT32)
- Added stub implementations for assertInterruptPriority, getFlashAddr*, printWRPBits, etc. in AT32 port
- Wrapped can_sniffer in #if EFI_USB_SERIAL

Build result:
- Output files: build/rusefi.bin (632516 bytes), build/rusefi.srec
- text: 389516, data: 1025, rodata: 219860, bss: 135901
- Compiler: arm-none-eabi-gcc 16.1.1

Key decisions:
- Used old AT32 port (ChibiOS/AT32F4xx) instead of ChibiOS-Contrib AT32F435_437 to minimize risk
- Corresponds to SPIv1 driver (no slave/data_cb/error_cb fields, only end_cb)

Follow-ups:
- Should consider migrating to ChibiOS-Contrib AT32F435_437 port for proper SPIv2 support
- AT32 flash driver not implemented (EFI_STORAGE_INT_FLASH=FALSE)
- USB is physically absent on m74_9, confirmed via board.mk

## 2026-07-14 - Investigation: "Malformed Packet: packet length" in USB.pcapng

What was done:
- Analyzed USB.pcapng (USBPcap capture of the rusEFI ECU USB link, device address 7)
  using tshark to explain the "malformed packet" warnings.

Findings:
- Protocol hierarchy: 84926 frames, mostly USB mass storage (usbms/scsi) plus 2 CDC
  (usbcom) control frames. 23 SCSI frames + 1 URB frame flagged _ws.malformed.
- The 23 usbms malformed frames are ALL the same case: the device reply to
  SCSI Mode Sense(6) (opcode 0x1a). Wireshark message: "SCSI: length of contained
  item exceeds length of containing item".
- Root cause is a Wireshark dissector strictness issue, NOT bad wire data. The 16-byte
  reply is self-consistent:
      0f 00 00 00  08 0a 00 00 00 00 00 00 00 00 00 00
  Mode Data Length=15 (=total-1), Caching mode page (0x08) with PageLength=0x0a=10.
  SBC-2 mandates the Caching page be 0x12=18 long (20-byte page); rusEFI emits a valid
  but non-standard SHORT caching page. Wireshark decodes the full 20-byte layout, reads
  past the 16-byte buffer, and raises the exception. Windows accepts it -> device works.
- Response is hardcoded in ChibiOS-Contrib USB-MSD SCSI target (os/hal/src/hal_usb_msd.c,
  submodule not checked out locally), driven by
  firmware/hw_layer/mass_storage/mass_storage_device.cpp.
- Frames 27759-27772 "USBPcap did not recognize URB Function code" are a USBPcap capture-
  driver limitation, unrelated to rusEFI traffic.

Decisions:
- Classified as cosmetic; no code change made. Not a functional defect.

Validation:
- tshark -z io,phs, per-frame -V dissection, and raw -x hex confirmed the byte layout
  and that all 23 malformed frames share the Mode Sense(6) cause.

Open follow-ups:
- Optional cleanup if the warning ever matters: set the Caching mode page length to 0x12
  and pad the page to the full 20 bytes, or drop the caching page from the Mode Sense
  reply. Lives upstream in ChibiOS-Contrib hal_usb_msd.c.

## 2026-07-14 - SD ECU<->PC switch soak sandbox + USB CDC link-drop investigation

What was done:
- Created a headless soak sandbox SdEcuPcCycleSandbox in the :ui test subproject
  (java_console/ui/src/test/java/com/rusefi/SdEcuPcCycleSandbox.java), modeled on the
  purple-gateway SdPcToEcuSwitchSandbox. It cycles the SD card ECU/logging <-> PC/MSD 10
  times, 20s dwell per mode, confirming each switch via the sd_present / sd_logging_internal
  / sd_msd output channels, and reports a pass/fail tally. Added Gradle task :ui:runSdCycle.
- Initialized two uninitialized git submodules required by the Java build:
  java_console/peak-can-basic (missing peak.can.basic.* -> :ecu_io compile fail) and
  java_console/luaformatter (missing neoe.formatter.lua -> :ui compile fail).

Result of the run (COM149, purple-gateway fw, USB-powered only / no +12V):
- Cycle 1 fully succeeded BOTH directions. PC/MSD->ECU no longer hits FR_DISK_ERR: firmware
  logged "SD: switched from PC/MSD to ECU/logging" and opened log file re_10.mlg; status bits
  confirmed sd_logging_internal=1.
- ~1.5s after the ECU switch the host CDC serial link dropped:
  "output channels: executeCommand failed: java.io.IOException: write failed: wrote 0 but
  expected 11", COM149 closed. Never recovered, so cycle 2's first command got no response and
  the soak aborted at 1/20. Sandbox behaved correctly - it detected and reported the drop.

Root cause (investigated, code-evidenced):
- CDC console and USB mass storage are interfaces on ONE composite USB device (USBD1). The
  config descriptor is fixed at 3 interfaces - MSD IF0 + CDC-control IF1 + CDC-data IF2, 98
  bytes (usbcfg.cpp DESCRIPTOR_SIZE/NUM_INTERFACES). MSD is always present in the enumerated
  descriptor whenever HAL_USE_USB_MSD is built in.
- The SD mode switch does NOT re-enumerate or reconfigure USB. attachMsdSdCard /
  deattachMsdSdCard (mass_storage_init.cpp) merely hot-swap LUN1's backing block device
  between the real SD card and the null device ND1 on the already-running MSD controller.
- Causal chain: PC/MSD->ECU calls deattachMsdSdCard() which swaps LUN1 (SD card -> ND1) while
  Windows still has that mass-storage volume mounted -> the medium vanishes under the mounted
  volume -> the Windows usbstor stack resets/re-enumerates the whole composite device to
  recover -> firmware gets USB_EVENT_RESET/SUSPEND, whose handler calls sduSuspendHookI(&SDU1)
  (usbcfg.cpp:446), tearing down the CDC channel -> host CDC write returns 0, COM149 drops.
- It is host-side (write wrote 0 = port handle invalidated), not a firmware stall: the switch
  completed cleanly, logging started, and the device kept emitting messages up to the drop.

Remediation directions (not implemented - investigation only):
- Don't swap the MSD LUN to a dead null device under a mounted volume. Instead present a stable
  medium or return SCSI "not ready / medium not present" (unit attention) so Windows performs an
  orderly media-eject rather than treating it as a device fault and resetting the port.
- Or signal proper SCSI medium-removal / unit-attention before switching so the host dismounts
  cleanly.
- Host-side, for a true 10x soak: reconnect LinkManager after each switch (treat the CDC drop
  as expected re-enumeration). The current sandbox intentionally reports it instead.
- The existing USB.pcapng capture can confirm the host-issued bus reset around a mode switch.

Validation:
- ./gradlew :ui:compileTestJava BUILD SUCCESSFUL after submodule init.
- ./gradlew :ui:runSdCycle exercised against real hardware; full log captured.

Open follow-ups:
- Decide remediation approach (firmware SCSI media-eject vs host-side reconnect).
- Consider gating: the soak cannot complete 10 cycles over one connection until the CDC drop is
  addressed.

## 2026-07-14 - SD indicator/output-channel name reuse between SdEcuPcCycleSandbox and .ini

What was done:
- Removed the duplicated magic strings sd_present / sd_logging_internal / sd_msd that existed
  independently in output_channels.txt (bit field names), tunerstudio.template.ini (6 indicator
  expressions) and SdEcuPcCycleSandbox.java (SensorCentral lookups). They now flow from a single
  source of truth via the existing code generation.

Change inventory:
| File                                                        | Change                                                          |
|-------------------------------------------------------------|-----------------------------------------------------------------|
| firmware/integration/rusefi_config_shared.txt               | new OUTPUT_CHANNEL_SD_PRESENT / _SD_LOGGING_INTERNAL / _SD_MSD quoted defines |
| firmware/console/binary/output_channels.txt                 | the 3 SD bit fields renamed to @#OUTPUT_CHANNEL_SD_...#@ references |
| firmware/tunerstudio/tunerstudio.template.ini               | 6 indicator lines reference @#OUTPUT_CHANNEL_SD_...#@           |
| java_tools/.../ReaderStateImpl.java (config_definition_base)| handleBitLine now applies variable substitution to the bit name (comment stays templated, matching plain-field parsing) |
| java_tools/.../ConfigFieldParserTest.java                   | new testBitNameViaVariableReference                             |
| java_console/.../SdEcuPcCycleSandbox.java                   | uses VariableRegistryValues.OUTPUT_CHANNEL_SD_* constants       |
| java_tools/version/.../UiVersion.java                       | CONSOLE_VERSION -> 20260714                                     |

Key decisions and why:
- Constants live in rusefi_config_shared.txt because it is prepended by BOTH pipelines that need
  them: gen_config_common.sh (template .ini + VariableRegistryValues.java) and the LiveData.yaml
  output_channels entry (LiveDataProcessor parsing output_channels.txt).
- Used the existing @#NAME#@ quote-stripping substitution (same as TS_HELLO_COMMAND usages) so the
  quoted define yields a bare identifier in struct field names and { } indicator expressions while
  generating a proper Java String constant.
- handleBitLine substitution was narrowed to the name part only: applying it to the whole line
  expanded @@...@@ comment templates at parse time, which changed engine_state_generated.h
  (clutchDownState comment). The narrowed version keeps every generated artifact byte-identical.
- Did not resurrect the deleted generated TsOutputs.java (removed in #6711); defines + existing
  VariableRegistryValues generation is the sanctioned mechanism.

Validation:
- ./gradlew :config_definition:test :config_definition_base:test green including the new test.
- gen_live_documentation.sh + gen_config_board.sh f407-discovery: all generated outputs
  (output_channels_generated.h, live_data_fragments.ini, data_logs.ini, board .ini indicator
  lines) byte-identical to committed state; only VariableRegistryValues.java gains the 3 new
  String constants. Board-generated .h/.ini signature/date churn reverted (CI regenerates).
- ./gradlew :ui:compileTestJava BUILD SUCCESSFUL with the sandbox on the generated constants.

Open follow-ups:
- Other magic output-channel names shared between java_console and .ini (e.g. sd_error,
  sd_formating, sd_active_wr/rd) could adopt the same pattern when java code starts using them.

---

## 2026-07-14 - N52 preset: bake in TPS/PPS calibration from reference tune

What: Ported TPS + PPS calibration out of the "super N52" TunerStudio tune (CurrentTune.msq)
into the `bmwN52()` engine preset in `firmware/config/engines/bmw.cpp`, so a fresh N52 selection
ships with the real throttle-body/pedal calibration instead of raw defaults.

| Field(s)                                                   | Source (msq, volts) | Stored as |
|------------------------------------------------------------|---------------------|-----------|
| tpsMin / tpsMax                                            | 4.545 / 0.58        | 10-bit ADC via convertVoltageTo10bitADC |
| tps1SecondaryMin / tps1SecondaryMax                       | 0.75 / 4.72         | 10-bit ADC via convertVoltageTo10bitADC |
| throttlePedalUpVoltage / throttlePedalWOTVoltage          | 0.625 / 2.230       | float volts (verbatim) |
| throttlePedalSecondaryUpVoltage / ...WOTVoltage           | 0.947 / 4.197       | float volts (verbatim) |

Key decisions:
- Used shared helpers setTPS1Calibration()/setPPSCalibration() (defaults.h), matching subaru.cpp.
- TPS stored as 10-bit ADC counts (not volts): wrapped msq volts in convertVoltageTo10bitADC
  (= volts*200). PPS stored as float volts, copied through directly. This units split is the main
  porting hazard and is now documented.
- Skipped tps2* fields: msq had them at defaults (0/5, 5/0); N52 runs a single dual-sensor throttle.
- Added #include "defaults.h"; convertVoltageTo10bitADC comes transitively via pch.h.

Docs: new docs/AI/engine_presets.md documents the canned-tune/preset process end to end
(enum -> engine_type_impl.cpp dispatch -> config/engines setup fn), the TPS-vs-PPS units gotcha,
and a step-by-step msq->preset porting recipe.

Validation: static review only - mirrors the established subaru.cpp calibration pattern; all
referenced helpers are declared in the included headers. No generated files touched (presets are
plain code, no gen_config step).

Open follow-ups:
- Confirm on hardware that a defaults-reset N52 reads plausible TPS
---

## 2026-07-14 - N52 preset: bake in TPS/PPS calibration from reference tune

What: Ported TPS + PPS calibration out of the "super N52" TunerStudio tune (CurrentTune.msq)
into the bmwN52() engine preset in firmware/config/engines/bmw.cpp, so a fresh N52 selection
ships with the real throttle-body/pedal calibration instead of raw defaults.

| Field(s)                                           | Source (msq, volts) | Stored as |
|----------------------------------------------------|---------------------|-----------|
| tpsMin / tpsMax                                     | 4.545 / 0.58        | 10-bit ADC via convertVoltageTo10bitADC |
| tps1SecondaryMin / tps1SecondaryMax                | 0.75 / 4.72         | 10-bit ADC via convertVoltageTo10bitADC |
| throttlePedalUpVoltage / throttlePedalWOTVoltage   | 0.625 / 2.230       | float volts (verbatim) |
| throttlePedalSecondaryUpVoltage / ...WOTVoltage    | 0.947 / 4.197       | float volts (verbatim) |

Key decisions:
- Used shared helpers setTPS1Calibration()/setPPSCalibration() (defaults.h), matching subaru.cpp.
- TPS stored as 10-bit ADC counts (not volts): wrapped msq volts in convertVoltageTo10bitADC
  (-> volts*200). PPS stored as float volts, copied through directly. This units split is the
  main porting hazard and is now documented.
- Skipped tps2* fields: msq had them at defaults (0/5, 5/0); N52 runs a single dual-sensor throttle.
- Added #include "defaults.h"; convertVoltageTo10bitADC comes transitively via pch.h.

Docs: new docs/AI/engine_presets.md documents the canned-tune/preset process end to end
(enum -> engine_type_impl.cpp dispatch -> config/engines setup fn), the TPS-vs-PPS units gotcha,
and a step-by-step msq->preset porting recipe.

Validation: static review only - mirrors the established subaru.cpp calibration pattern; all
referenced helpers are declared in the included headers. No generated files touched (presets are
plain code, no gen_config step).

Open follow-ups:
- Confirm on hardware that a defaults-reset N52 reads plausible TPS%/pedal% before user tuning.
---

## 2026-07-15 - SD card logging: overview doc + code comments (f_expand logic)

What: Documented the SD card logging subsystem and annotated its three core source files.

| File | Change |
|-------------------------------------------------------|--------------------------------------------|
| docs/AI/sd_card_logging.md (new)                       | End-to-end overview: SD thread mode state machine (IDLE/ECU/PC/UNMOUNT/FORMAT, all transitions via IDLE), .mlg vs .teeth loggers, FileBufferedWriter path, f_expand pre-allocation, file naming, status channels, console commands |
| firmware/hw_layer/mmc_card.cpp                         | Expanded LOGGER_MAX_FILE_SIZE, f_expand and f_truncate comments; doc pointer in file header |
| firmware/console/binary_mlg_log/binary_mlg_logging.cpp | File-header overview (MLG v2 layout, who owns file lifecycle); comments on writeFileHeader/writeSdBlock/writeSdLogLine/resetFileLogging |
| firmware/console/binary/tooth_logger.cpp               | Comment on freeBuffers/filledBuffers multi-buffering (BigBuffer, interrupt producers -> TS/SD consumers); ToothLoggerWriter() contract incl. 3s idle timeout -> new file |
| CLAUDE.md                                              | Added sd_card_logging.md to Deep Dive AI Guidance list |

Key facts captured (the f_expand logic in particular):
- sdLoggerCreateFile() pre-allocates each log file to 32Mb with f_expand(fd, size, opt=1)
  (allocate-now, contiguous; FF_USE_EXPAND=1 in firmware/ext/FatFS/ffconf.h). All FAT
  updates happen up-front, so writes inside the pre-allocated area never touch FAT
  structures -> sudden power loss loses buffered data but not the filesystem.
- f_expand failure (fragmented card) is deliberately non-fatal: FatFS falls back to
  cluster-by-cluster growth, logging works without the corruption protection.
- sdLoggerCloseFile() f_truncate()s back to actual size; a power-lossed file stays 32Mb
  with trailing garbage.
- Both loggers share sdLoggerCreateFile() (so .teeth files are pre-allocated too) but
  only sdLoggerMlg() enforces the 32Mb rollover cap.

Validation: comment/doc-only changes, no code touched; facts verified against source
(mmc_card.cpp, file_writer.h, ffconf.h FF_USE_EXPAND=1, tooth_logger.cpp, sd_log_trigger.h).

Open follow-ups: none.

## 2026-07-15 - Lua scripting API: categorized hook inventory doc

What: Reviewed every custom Lua method registered around lua_hooks.cpp and documented
them in a new docs/AI/lua_scripting.md, grouped into 11 categories.

| File | Change |
|-------------------------------|--------------------------------------------------------|
| docs/AI/lua_scripting.md (new) | Full inventory of Lua hooks by category: input reads, virtual sensors, virtual switches, closed-loop trims, cut/disable controls, PWM/DAC outputs, CAN, config/calibration access, state queries, luaaa helper classes, framework/test hooks; plus registration-site map, indexing conventions, build-flag gating, and an "adding a new hook" recipe |
| CLAUDE.md | Added lua_scripting.md to the Deep Dive AI Guidance list |

Key decisions / findings:
- Registration is spread over four files: lua_hooks.cpp (bulk + luaaa classes),
  lua_hooks_util.cpp (print/interpolate/find*/mcu_standby), lua.cpp (setTickRate,
  onTick dispatch), lua_can_rx.cpp (onCanRx dispatch, global_can_data workaround).
  lua_hooks_ext.cpp is an empty extension point; boardConfigureLuaHooks() is a weak
  board hook with no in-tree overrides.
- Documented the mixed indexing convention explicitly: 1-based (HUMAN_OFFSET) for
  TS-facing entities (CAN bus, curves, tables, TS buttons, gauges), 0-based for
  sensor indices, PWM channels, aux digital inputs, vin().
- Documented flash-saving exclusions (#if !defined(STM32F4) group) and the
  DISABLE_LUA_* / WITH_LUA_* opt-out macros.
- setTickRate code clamps 1..2000 Hz while its comment says 1..200 - doc records
  the code behavior (comment discrepancy left in source, not a functional issue).

Validation: doc-only change; every listed hook, guard macro and constant
(LUA_PWM_COUNT=8, LUA_GAUGE_COUNT=8, LUA_DIGITAL_INPUT_COUNT=8, LUA_BUTTON_COUNT=10,
CMD_BURNCONFIG="burnconfig") verified against source via grep/read of the four
registration files, lua_pid.h and rusefi_config.txt.

Open follow-ups:
- lua.cpp setTickRate comment ("Limit to 1..200 hz") disagrees with clampF(1, x, 2000).
- lua_hooks.cpp has a commented-out hasCriticalReportFile hook referencing issue #7291.

## 2026-07-17 - loss-of-cdc.pcapng analysis: one-shot composite reset from pre-capture MSD wedge

What: Analyzed loss-of-cdc.pcapng (repo root, USBPcap, 24.6 s, captured 2026-07-17
12:25 - i.e. the day AFTER the #9860 fix series landed) against the recent
mass_storage changes. Goal: confirm/refute whether the CDC drop mechanism from
issue #9860 is still present.

Devices in capture: address 21 = the ECU (VID 0483:5740, composite MSD+CDC),
address 22 = PEAK PCAN-USB adapter (19.9k of the 22k packets - unrelated noise).

Timeline (t = seconds from capture start):
- t=0..10.8: ZERO MSD traffic from the ECU. A healthy medium-less device gets
  ~1 Hz Test Unit Ready polls (visible later in this same capture), so at capture
  start usbstor already had one command in flight that never completed - the MSD
  side was already wedged/stuck before the capture began.
- t=7.79: host opens the COM port (GET/SET LINE CODING burst); CDC request/reply
  traffic (TS-style 7/11-byte commands, 1024-byte replies) runs cleanly for 3 s.
- t=10.847: usbstor ~20 s give-up timer fires -> all-endpoint cancel storm on the
  ECU: 10 URBs with USBD_STATUS_CANCELED (0xc0010000) - MSD bulk-IN 0x81 (the
  stuck data/CSW read, pending since before capture start), CDC data 0x82/0x02,
  CDC interrupt 0x83, plus control. This is the loss-of-CDC moment.
- t=10.883: host immediately retries line coding - those control URBs are
  canceled too (device still resetting).
- t=11.03..11.05: MSD recovers: Test Unit Ready on LUN0 and LUN1 -> Check
  Condition -> Request Sense (Good) -> Mode Sense(6) (the known-cosmetic
  "malformed" short caching page). Both LUNs report medium-not-present.
- t=11.28: CDC port re-opens at USB level (line coding OK) but NO data traffic
  follows - the app-level session was dead, host serial layer sat in its ~10 s
  timeout.
- t=12..24.5: clean steady state: 1 Hz TUR polls per LUN, no stalls, no babble,
  no further cancels or resets.
- t=20.79: app fully reconnects (line coding + control line state), TS-style
  traffic resumes. Total user-visible CDC outage: ~10 s (10.85 -> 20.79).

Reading vs the 2026-07-16 fix series (298162eb075..68e7d77c042, all in
firmware/hw_layer/mass_storage/):
- 298162eb0/8a515546c (MSD diag #9838): sdinfo diagnostics incl. per-opcode
  in-flight timer.
- e1feee380 (isCommandAbandoned #9861): 10 s data-phase timeouts on all SCSI
  transfers + CSW via msdUsb*Timeout helpers -> wedged thread self-recovers,
  re-arms bulk-OUT.
- 12b613c59 (#9864): LUN detach now synchronizes with in-flight command
  (m_lunMutex held around scsiExecCmd+CSW) -> kills the SPI double-waiter
  deadlock from the SD mode switch.
- 04331c28f (#9866) + 68e7d77c0 (uaefi): medium-less data-IN commands answered
  with ZLP instead of STALL -> no EP0 clear-halt round-trip near CDC traffic.
The capture is consistent with the fixes WORKING as designed for the recurring
part: exactly ONE reset (the tail of a wedge that began ~9 s before capture,
matching usbstor's ~20 s timer), then 13.5 s of clean behavior with no repeat
reset - the old signature was a reset every ~20 s.

Remaining gap (why one reset still happens): the firmware 10 s data-phase
timeout releases the MSD *thread*, but leaves the *host's* pending IN URB
hanging - firmware just returns to CBW wait and never completes/STALLs the
IN transfer the host is still waiting on. usbstor therefore still escalates to
a full composite reset once, taking CDC down with it. A full fix would complete
the host's data phase on timeout (e.g. STALL the IN endpoint so the host gets
an immediate error -> clear-halt -> CSW path) instead of leaving the URB
pending. Caveat: cannot verify from the capture which firmware build was
flashed or which opcode wedged (the CBW predates the capture); console sdinfo
counters (data-phase timeouts / no-data ZLPs) on the connected unit would
distinguish "fixed firmware, host-side URB gap" from "stale firmware".

Validation: tshark 3.6.2 field-level analysis (usb.usbd_status, endpoints,
SCSI dissection); code cross-checked at HEAD (mass_storage_device.cpp timeout/
ZLP/mutex mechanisms present).

Open follow-ups:
- On data-phase timeout, also complete the host-visible transfer (STALL data-IN
  or arm+flush) so usbstor never needs its 20 s reset - would remove the single
  remaining CDC drop.
- Confirm via sdinfo on hardware whether the flashed build has the 07-16 fixes
  and whether data-phase timeout counters tick.

## 2026-07-17 - MSD data-phase timeout: close the command host-side (stall + phase-error CSW)

What: Implemented the follow-up from the loss-of-cdc.pcapng analysis (previous
entry). Before this change, a data-phase timeout only freed the MSD *thread*
(e1feee380 #9861); the *host's* pending URB was left hanging and the CSW was
skipped, so usbstor still escalated to one full composite-device reset per
wedge - taking the CDC console down for ~10 s each time.

| File | Change |
|----------------------------------------------------|----------------------------------------|
| firmware/hw_layer/mass_storage/mass_storage_device.cpp | ThreadTask: split the abandoned-command check. BOT reset still skips the CSW (host is not expecting one). Data-phase timeout now STALLs the data endpoint in the CBW's direction (usbStallTransmitI/usbStallReceiveI) and then sends a CSW with CSW_STATUS_PHASE_ERROR and honest residue. sendCsw() now returns whether the host read the CSW; sdinfo prints "N data-phase timeouts (M closed by CSW)" |
| firmware/hw_layer/mass_storage/mass_storage_device.h | sendCsw() -> bool; new m_timeoutCswDeliveredCount counter |

Key decisions and why:
- STALL is the BOT-sanctioned "cannot complete this data phase" signal: a host
  still waiting on its data URB completes it with an error immediately (well
  before usbstor's ~20 s give-up), does a clear-halt on this one endpoint, and
  collects the CSW - recovery stays class-level on the MSD interface, the CDC
  endpoints never notice. A host that already canceled its URBs ignores the
  stall and resets anyway - no worse than before.
- Arming the CSW while the endpoint is still stalled is the exact sequence the
  pre-ZLP medium-less path used (04331c28f), already validated on Windows
  hardware (STALL -> clear-halt -> CSW observed on the wire).
- CSW_STATUS_PHASE_ERROR rather than FAILED: after a broken data phase the
  transport has genuinely lost sync; phase error makes the host run Bulk-Only
  Reset Recovery (class request + clear both halts), fully resynchronizing
  data toggles without any port-level reset. The existing onBulkOnlyResetIsr
  path handles that request.
- Safe to stall: all three msdUsb*Timeout helpers clear the endpoint's
  active flag on timeout, so usbStall*I (which refuses while a transfer is
  active) always takes effect by the time ThreadTask runs the recovery.
- The no-data-ZLP timeout path intentionally keeps its plain 'continue': a
  host that will not even take a zero-length packet is gone from the data
  phase entirely; its next action is a new CBW (accepted normally) or a reset.

Validation: uaefi firmware build (see below). No unit-test coverage exists for
this path (EFI_PROD_CODE + HAL_USE_USB_MSD only). Hardware validation plan:
reproduce the wedge (host abandons a command mid-data-phase), then check
1. sdinfo shows "closed by CSW" ticking together with data-phase timeouts,
2. a capture shows STALL -> clear-halt -> CSW(phase error) -> BOT reset
   instead of the all-endpoint cancel storm,
3. the CDC console stays connected across the event.

Open follow-ups:
- Wedges *below* the USB layer (e.g. blkRead stuck on a dying SD card) are
  still uncovered: no timeout wraps lib_scsi's block-device calls, so such a
  wedge never reaches the new recovery path (lib_scsi is in ChibiOS-Contrib).
- The loss-of-cdc.pcapng pre-capture wedge could not be attributed (stale
  firmware vs blkRead wedge); confirm the flashed build via sdinfo counters.

## 2026-08-01 - Decouple VE Analyze from the STFT display scale

What: Restored zero-based STFT presentation without changing the 100-based
correction contract required by TunerStudio VE Analyze. PR #9657 changed the
STFT translation from -1.0 to -100; under TunerStudio's `(raw + translate) *
scale` conversion, a neutral raw multiplier of 1.0 became -9900 percent. The
derived `100 + stftCorrection1` channel then supplied -9800 instead of 100 to
VE Analyze, causing it to remove fuel.

| File | Change |
|----------------------------------------------------|----------------------------------------|
| firmware/controllers/algo/engine_state.txt | Restore STFT display metadata to scale 100, translation -1.0, so raw 0.9/1.0/1.1 displays as -10/0/+10 percent |
| firmware/tunerstudio/tunerstudio.template.ini | Feed `egoCorrectionForVeAnalyze` directly from `Gego`, the existing 100-neutral STFT output channel |
| java_tools/configuration_definition/src/test/java/com/rusefi/test/VeAnalyzeCorrectionTest.java | Regression coverage for zero-neutral display and independence of the VE Analyze channel |
| java_tools/version/src/main/java/com/rusefi/UiVersion.java | Bump console version to 20260801 as required for Java changes |

Key decisions and why:
- Reused `Gego` instead of adding another live-data field. `status_loop.cpp`
  already publishes it as `100 * stftCorrection[0]`, so this avoids output
  layout churn and keeps the machine-facing 100-neutral contract explicit.
- Kept the user-facing `stftCorrection` channels zero-neutral and independent
  from AutoTune. Gauge scale or translation changes can no longer alter the
  correction consumed by VE Analyze.
- No persistent calibration field or generated file is part of the change, so
  existing tunes require no migration.
- LTFT behavior is intentionally unchanged in this unit of work. Stored LTFT
  correction still affects delivered fuel without being represented in the VE
  Analyze correction channel; that requires a separate policy change and test.

Validation:
- Regression test first failed on the old code: raw 0.9 displayed as -9910
  instead of -10, and VE Analyze still referenced the visual STFT channel.
- The same test passes after the fix.
- `gradlew.bat :config_definition:test` passes.
- Clean uaefi `make -B -j12 ini` generation passes. The generated INI contains
  `stftCorrection1/2` with `100.0, -1.0`, keeps `Gego` at scale 0.01, and emits
  `egoCorrectionForVeAnalyze = { Gego }`; both VE Analyze and WUE Analyze use
  that alias.

Open follow-ups:
- Define and test the LTFT policy during AutoTune (disable application, require
  applying/resetting learned trims, or introduce an explicit tuning session).
- Decide how a future bank-2-aware VE Analyze correction should select/combine
  STFT banks; this change preserves the existing bank-1 behavior.

## 2026-08-09 - Restore PCAN adapter into the console ports list + surface init errors

What: PCAN-USB adapters were detected (`AvailableHardware.isPCANConnected()`)
but never shown in the ports dropdown: the `ports.add(new PortResult(LinkManager.PCAN,
SerialPortType.CAN))` block had been commented out in Feb 2026 (commits
`6bc0d319e2f`/`ad9856e19a4`, messages "PCANConnected only:hiding"), so the user
saw `Rendering available ports: []` and "No ECU ports to use found" even though
MaintenanceUtil confirmed `Caption : PCAN-USB, ConfigManagerErrorCode : 0`. A
second, hidden-by-design issue: when PCAN init failed, `PCanIoStream.createStream()`
logged the real `TPCANStatus` only through the status consumer and returned null,
so the UI showed the generic "Failed to open port" without the cause.

| File | Change |
|----------------------------------------------------|----------------------------------------|
| java_console/connectivity/src/main/java/com/rusefi/SerialPortScanner.java | Re-enable the PCAN synthetic CAN port (SocketCAN stays hidden as before - it is Linux-only and was deliberately hidden separately) |
| java_console/io/src/main/java/com/rusefi/io/LinkManager.java | PCAN branch now captures the status-consumer message and throws it as IllegalStateException, so PortHolder reports "Exception opening port: Error initializing PCAN: <status>" instead of the generic failure |
| java_console/connectivity/src/test/java/com/rusefi/SerialPortScannerTest.java | FakeProbes gains a scripted `pcanConnected` flag; new tests for PCAN surfacing as a CAN port and for the no-adapter case |

Key decisions and why:
- Restored only the PCAN block, not SocketCAN. `SHOW_SOCKETCAN` is Linux-only
  and the existing `tcpPortWithoutEcuIsReportedUnknownAndNotCached` test asserts
  an exact port count on Linux CI, so un-hiding SocketCAN would break it; the
  owner hid it in a separate commit for its own reasons.
- Kept `PCanIoStream.createStream()` contract (returns null + status consumer)
  and wrapped it at the LinkManager call site instead: the status text flows into
  the thrown exception, so every caller (dropdown connect, ConsoleTools,
  PcanConnectorUI) keeps working unchanged.
- `AtomicReference` needs `java.util.concurrent.atomic` - `import
  java.util.concurrent.*` does not cover subpackages (compile error caught by
  `:ecu_io:compileJava`).

Validation:
- `./gradlew :connectivity:test :ecu_io:test` passes (including new
  `pcanAdapterSurfacesAsSyntheticCanPort` / `noPcanAdapterMeansNoSyntheticPcanPort`
  and the pre-existing scan-policy suite).
- Compile of `:ecu_io` catches the missing-import mistake; fixed with an explicit
  `java.util.concurrent.atomic.AtomicReference` import.

Open follow-ups:
- `PCanHelper.init` still hardcodes `PCAN_USBBUS1` + `PCAN_BAUD_500K`; a bus
  number or baud mismatch (e.g. ECU at 1M) will surface now as the real
  TPCANStatus in the UI - consider making channel/baud configurable.
- If PCAN init succeeds but the ECU never answers, the next diagnostic step is
  the ISO-TP IDs (`CAN_ECU_SERIAL_RX_ID`/`TX_ID`) versus firmware config.

## 2026-08-09 - PowerShell device probes: -Command quoting broken on Windows, switched to -EncodedCommand

What: After the ProcessBuilder conversion of the PowerShell device probes, the user's
Windows console log changed from `powershell -NoProfile -Command "Get-CimInstance ...
-Filter \"Caption like '%PCAN-USB%'\" ..." says Caption : PCAN-USB` (old build, worked)
to `[powershell, -NoProfile, -Command, Get-CimInstance Win32_PnPEntity -Filter "Caption
like '%PCAN-USB%'" ...] says (empty)` (new build, empty). Root cause: Windows PowerShell
parses the `-Command` tail with its own quote handling (it strips/re-interprets quotes,
not the standard C-runtime argv rules), and every Java argv-joining strategy feeds it a
different escaped form - Runtime.exec(String)'s tokenizer produced literal `\"` tokens,
ProcessBuilder's command-line quoting produces `\"` escapes - so the WQL -Filter value
`"Caption like '%PCAN-USB%'"` got mangled and the query matched nothing. The macOS
fake-powershell argv-dump test could not catch this: POSIX execvp passes argv verbatim,
Windows command-line reconstruction is where quoting breaks.

| File | Change |
|----------------------------------------------------|----------------------------------------|
| java_console/ui/src/main/java/com/rusefi/maintenance/MaintenanceUtil.java | New `powershellEncodedCommand(script)` helper: transports the script as Base64 UTF-16LE via `-EncodedCommand` (pure-ASCII payload, no spaces/quotes - immune to any command-line quoting); PCAN query converted; `detectDevice` log line now shows the decoded script via `describeQueryCommand` |
| java_console/ui/src/main/java/com/rusefi/maintenance/DfuFlasher.java | STM32-bootloader and STM32-H7 DFU queries converted to `powershellEncodedCommand` |
| java_console/ui/src/main/java/com/rusefi/maintenance/StLinkFlasher.java | ST-Link query converted; dropped now-unused `Arrays` import |
| java_console/ui/src/test/java/com/rusefi/maintenance/MaintenanceUtilTest.java | `pcanQueryIsSingleIntactPowerShellScriptArgument` replaced by `pcanQueryIsEncodedCommandSurvivingQuoting` (base64 payload shape + decode round-trip) and `describeQueryCommandShowsDecodedScript` |

Key decisions and why:
- `-EncodedCommand` (Base64 of UTF-16LE) instead of more quote-escaping: it is the
  canonical way to pass complex scripts to Windows PowerShell and pwsh alike, and it
  makes the transport deterministic - the script reaches PowerShell byte-exact.
- Left `ExecHelper.executeCommand(List, ...)`/ProcessBuilder in place (correct on
  POSIX and needed for the no-shell path); only the command content changed.
- Left `DfuFlasher.getPnpDevices` (`Get-PnpDevice -PresentOnly`, no quotes) and
  `TunerStudioHelper` (Runtime.exec(String), script without inner quotes) untouched -
  neither has quoted arguments, so both are immune to this class of bug.

Validation:
- `./gradlew :ui:test :connectivity:test :ecu_io:test` passes; the new tests verify
  the payload charset and the decode round-trip back to the intact script.
- Cannot run PowerShell here (macOS host); the user validates on Windows: the log
  line should show `-EncodedCommand <Get-CimInstance ...>` and "says Caption : PCAN-USB".

Open follow-ups:
- If the user's adapter still reports empty with the encoded command, the next step
  is hardware/driver state (PCAN-View holding the channel, unplugged adapter) rather
  than quoting - the query transport is now deterministic.
- `TunerStudioHelper` still uses Runtime.exec(String) with outer quotes; fine today,
  but converting it to the list-based ExecHelper would remove the last fragile caller.

## 2026-08-09 - m74_9 watchdog round 2: CNT alive, ISR->reschedule chain dies; live register dump + pre-fault history

What: Follow-up instrumentation of the m74_9 (AT32F435) `CRITICAL error: Watchdog: no
events for 2s!` lockup at ~14-20 s after boot. Round-1's register snapshot was truncated
by `CRITICAL_BUFFER_SIZE=120` (the message buffer), so the extended state never reached
the console. Round-2 moves the full register picture to the 256-byte console path and
adds a pre-fault history.

| File | Change |
|----------------------------------------------------|----------------------------------------|
| firmware/hw_layer/microsecond_timer/microsecond_timer.cpp | Latched `firmwareError` shrunk to ~103 chars (`isr=%d setHw=%d pending=%d cnt=%u..%u dier=0x%x sr=0x%x cr1=0x%x`) so it survives the 120-byte buffer plus the ` %d@%s` suffix; live `WDT regs: ...` dump (cnt span, dier, sr, cr1, smcr, psc, arr, ccr1, ccmr1, ccer, apb1enr, tim5en) via `efiPrintf` every watchdog period while the error holds; 16-sample ring buffer (`WDT hist: ...`) recording cnt/isr/setHw/freeze/pending/sr/dier/ccr1/cr1 once per healthy 500 ms watchdog period (last ~8 s), dumped once at first latch; state kept private under `#ifdef SCHEDULER_TIMER_DEVICE` (Cypress/Kinetis have no TIM device) |

Key findings from the user's round-2 log (2026-08-09 20:54):
- `cnt=124159183..124559165(+399982)` per 100 ms at 4 MHz -> the TIM5 counter keeps
  running, the clock is NOT gated (rules out frozen CNT and cleared APB1ENR.TIM5EN).
- `isr=37650 setHw=37652` frozen, `pending=1`, `dier=0x2` (CC1IE armed), `sr=0x0` (no
  CC1IF) -> the compare never fires even though the interrupt is armed; the
  ISR -> executeAllPendingActions -> scheduleTimerCallback chain stops at "comparison
  does not trigger".
- Error repeats every ~180 ms while pending=1 holds; the CAN console keeps working, so
  it is not a global freeze.
- Timeline: first error 20:54:54.9 vs boot 20:54:40.05; ~37650 ISR hits at ~1400/s
  matches ~14 s. Preceding events: MFS config read (ok), Lua load (ok, 5.4% heap),
  set date over CAN (RTC 1980 -> 2026).

Validation:
- Diff reviewed for `%d/%u/%x` vs cast consistency; touches only the EFI_PROD_CODE path.
- Cross-compile and on-hardware reproduction pending on the user's Windows machine (no
  arm-none-eabi toolchain on the macOS host).

Open follow-ups:
- Need a full 40-60 s console log with `WDT regs:` (ccr1, ccer, apb1enr, tim5en, smcr)
  and `WDT hist:` lines to decide between "CC1E/OC1M rewritten by a second pwmStart on
  PWMD5" vs "CC1IF set but ISR never dispatched (NVIC/vector)".
- If the counter proves alive but the compare is disabled, check whether any code path
  restarts PWMD5 after boot (would reset CNT/ARR/CCER per pwm_lld_start).

## 2026-08-09 - m74_9 watchdog ROOT CAUSE: compare re-armed in the past, missed CNT==CCR1 equality

What: The user's round-3 register dump (21:23) finally pinned the failure mode. The
latched error repeats with isr=20128 setHw=20130 pending=1 cnt=60158992..60558987
while the live dump shows the counter alive and growing:

- `ccr1=0x30316F9` = 50,521,849 - in the PAST (cnt already 64-76M at dump time): the
  last successful re-arm wrote a compare value that CNT had already passed.
- `ccer=0x0` - NOT the culprit: CC1E is 0 since pwm_lld_start (channel mode
  PWM_OUTPUT_DISABLED) and the design works without it (compare interrupts fire on
  CC1IF + CC1IE only).
- `ccmr1=0x10` = OC1M(1) toggle as set by portInitMicrosecondTimer; `cr1=0x485` =
  PMEN|ARPE|URS|CEN exactly as pwm_lld_start writes (AT32 32-bit mode); both untouched.
- `psc=0x47` (72 divider, 288MHz APB1 -> 4MHz counter), `apb1enr=0x12000018`,
  `tim5en=8`: clock gating ruled out for good.
- `dier=0x2` (CC1IE armed) + `sr=0x0` (CC1IF cleared): the re-arm sequence
  pwm_lld_enable_channel (CCR1 write) -> pwm_lld_enable_channel_notification
  (SR=~(2<<0), DIER|=CC1IE) ran, but with CCR1 already in the past the equality
  CNT==CCR1 can never fire again until the 32-bit counter wraps (~18 min at 4MHz).
  The ISR->reschedule chain therefore dies silently; the 2s watchdog only reports it.

Root cause: setHardwareSchedulerTimer() computes its "too close to now" clamp
(US2NT(2)) against the nowNt passed by the caller (read earlier, under lock). The
hardware write happens later and the free-running CNT keeps advancing, so an event
scheduled only a few microseconds ahead can land in CCR1 already in the past - a
probabilistic race that took 12-20s to hit (isr counts differ per run: 20128, 37650).

Fix (firmware/hw_layer/ports/stm32/microsecond_timer_stm32.cpp,
portSetHardwareSchedulerTimer): re-check the compare value against a FRESH
SCHEDULER_TIMER_DEVICE->CNT read and clamp it at least US2NT(4) = 4us (16 ticks at
4MHz) into the future before writing CCR1. This makes the CNT==CCR1 equality
impossible to miss regardless of how stale the scheduler's setTimeNt is. The
int32-trick on (compare - cnt) is safe because TOO_FAR_INTO_FUTURE_NT is 10s
= 40M ticks << 2^31.

Validation:
- Review-only on macOS (no arm-none-eabi toolchain); the user will rebuild m74_9 on
  Windows and confirm the ECU stays up past the previous 12-20s lockup.
- The change is EFI_PROD_CODE-path only and does not alter the normal path (compare
  values >= 4us ahead are written verbatim).

Open follow-ups:
- If the lockup still reproduces with the clamp in place, the next suspect is an
  event actually landing in the queue with a past time (scheduleTimerCallback's
  efiAssertVoid "setTimer constraint" fires firmwareError but with
  EFI_ENABLE_ASSERTS=0 the guard is a no-op) - then fix there instead.
- WDT hist lines never reached the console in the 21:23 log (printed once at first
  latch, log started after); they are kept for future diagnosis.

## 2026-08-09 - m74_9 watchdog fix VALIDATED on hardware

What: The user rebuilt m74_9 on Windows with commit 1236c24c150 (compare re-arm
clamped >= 4us into the future against a fresh CNT read) and ran the ECU for
several minutes - no more `CRITICAL error: Watchdog: no events for 2s!`. The
previous firmware reliably latched after 12-20 s of uptime, so the race (stale
nowNt vs advancing CNT putting CCR1 in the past, equality CNT==CCR1 missed until
the 32-bit wrap) is confirmed fixed. No further WDT regs/hist output was produced,
which is the expected healthy behavior - the diagnostic prints only on latch.

Validation:
- User-reported on hardware (Windows host, PCAN-USB console): continuous run for
  several minutes, zero watchdog errors. This is the first on-target confirmation
  of the fix; earlier rounds were review-only on macOS.

Open follow-ups:
- Long-run soak (hours) and a powered-cycle test are desirable to make sure no
  rarer variant of the race remains; a fresh-flash boot was already covered.
- The WDT regs/hist diagnostics (commit 1371adf526f) are harmless when quiet and
  stay in place as a first-line tool for any future timer/lockup report.

## 2026-08-09 - TS-over-CAN link stability: ISO-TP flow control + desync recovery

Symptom (user log): `Got only 18 bytes while expecting 50 for command 0x43` and
`TunerStudio errors: outofrange=86/470` - multi-frame TS-over-CAN requests were
being truncated on m74_9 (CAN-only ECU, PCAN-USB host, 500k). 50 bytes = 1 FIRST
+ 7 CONSECUTIVE = exactly 8 CAN frames; only 18 bytes (6+7+5) assembled, so
frames were dropped and the ECU's ISO-TP state never recovered until the next
FIRST frame.

Root cause (three coupled problems):
1. The host (IsoTpConnector.sendStrategy) bursts FIRST + all CONSECUTIVE frames
   with no pause - `receiveData()` was an empty no-op - while the ECU's receive
   FIFO (CanTsListener.rxFifo, `CAN_FIFO_FRAME_SIZE=8`) holds only 8 frames and
   the TS thread may be busy sending the previous response, so a burst can
   overflow and `decodeFrame` silently drops.
2. A single dropped frame permanently desynchronized the ECU's ISO-TP state:
   CanStreamerState::receiveFrame / streamReceiveTimeout / IsoTpRx::readTimeout
   returned/aborted on index mismatch without calling reset(), so consecutive
   frames of the next message kept failing until a new FIRST arrived.
3. The Java decoder (IsoTpCanDecoder) treated the ECU's flow-control frame as a
   data packet (empty chunk) - noise, not a functional break.

What was done:
| Change | File |
| --- | --- |
| `CAN_FIFO_FRAME_SIZE` 8 -> 32 (headroom for host bursts; ~640 B static RAM on top of 384 KB, fine) | firmware/controllers/can/isotp/isotp.h |
| reset() on consecutive-index mismatch in receiveFrame | firmware/controllers/can/isotp/isotp.cpp |
| reset() when streamReceiveTimeout gets numReceived < 1 (lost/ignored frame) | firmware/controllers/can/isotp/isotp.cpp |
| reset() on desync/timeout in IsoTpRx::readTimeout (defensive; TS path uses CanStreamerState) | firmware/controllers/can/isotp/isotp.cpp |
| reset() now also clears the byte-level rxFifoBuf (drop stale leftovers) | firmware/controllers/can/isotp/isotp.cpp |
| PCanIoStream overrides receiveData(): wait up to 200 ms for the ECU FC frame (monitor + flag, reader thread signals on FC); FC frames are intercepted in readOnePacket and not fed to the decoder | java_console/io/src/main/java/com/rusefi/io/can/PCanIoStream.java |
| `receiveData()` documented as the FC-wait hook between FIRST and CONSECUTIVE; default stays no-op (SocketCAN/ELM327 unchanged) | java_console/io/src/main/java/com/rusefi/io/can/isotp/IsoTpConnector.java |
| public `ISO_TP_FRAME_FLOW_CONTROL=3` constant | java_console/io/src/main/java/com/rusefi/io/can/isotp/IsoTpConstants.java |
| New test: receiveData() is called between FIRST and CONSECUTIVE | java_console/io/src/test/java/com/rusefi/io/can/IsoTpConnectorTest.java |

Design notes:
- The FC wait is a monitor/flag pair, not a per-send CountDownLatch, because
  CountDownLatch cannot be reset. Writer thread waits in receiveData(); PCAN
  reader thread sets the flag + notifyAll on a frame whose type nibble is 3.
  Timeout falls back to the historical burst behavior (no regression if FC is
  lost). Both threads only touch the monitor under synchronization, so there is
  no lost-wakeup race.
- FC timeout 200 ms was chosen because the ECU sends FC immediately on FIRST
  (sendFlowControl, blockSize=0, stmin=0); the wait normally costs ~1 ms of
  latency per multi-frame write and paces the burst against the 32-frame FIFO.
- SocketCANIoStream keeps the default no-op receiveData() (Linux kernel path,
  same historical behavior) - flagged as a possible follow-up.

Validation:
- `./gradlew :ecu_io:test` BUILD SUCCESSFUL (JDK 11 toolchain via
  -Porg.gradle.java.installations.paths=...). New test passed; existing
  IsoTpConnectorTest / IsoTpCanDecoderTest untouched behavior.
- `cd unit_tests && ./test.sh testCanSerial`: 6/6 PASSED (TestCanStreamerState
  round-trips are index-consistent, so the new reset() paths are not hit there;
  macOS build needed a flock shim - `flock` does not ship on macOS - and the JDK
  11 toolchain env var for the gradle codegen steps).

Open follow-ups:
- User rebuilds m74_9 on Windows (firmware/config/boards/m74_9/compile_m74_9.sh)
  and re-runs the TS session over PCAN-USB; expect the `outofrange` counter and
  `not enough bytes in stream` errors to drop to zero.
- If truncation persists, the next suspect is host-side: PCanIoStream.decodePacket
  is still fed a 127-byte buffer for its assembly math even though the size-aware
  overload receives the real DLC - verify byte 0 of every 0x720 frame against a
  PCAN-View capture.
- Consider the same FC-wait for SocketCANIoStream.

## 2026-08-09 - TS-over-CAN ISO-TP fix VALIDATED on hardware (m74_9)

What: The user rebuilt m74_9 on Windows with commit b75fc82311e (32-frame receive
FIFO + host FC wait + ISO-TP desync reset) and ran the TS session over PCAN-USB:
no more `Got only 18 bytes while expecting 50`, no `not enough bytes in stream`
errors, the connection stays up - "кажется что работает". This confirms the
root-cause chain: burst overflow of the 8-frame FIFO plus a permanently stuck
ISO-TP state after a lost frame.

Validation:
- User-reported on hardware (Windows host, PCAN-USB console, 500k): continuous
  TS session without truncation errors.

Open follow-ups:
- Long-run soak to make sure the `outofrange` counter stays flat.
- Optional: same FC-wait treatment for SocketCANIoStream (Linux path still uses
  the no-op default).

## 2026-08-10 - m74_9 L9779WD-SPI: link fixed, VDA 2.0 watchdog feeding added

What: On the m74_9 bench the L9779WD-SPI power driver did not drive injectors
(OUT1..4) or coils (IGN1..4). Root cause turned out to be twofold: (a) the SPI
chip-select pad was never configured as an output, so the chip saw CS permanently
asserted and rejected every frame with SPI_ERR (DO = 0x8000), and (b) even with
a valid link the chip keeps OUT1..4/IGN1..4 forced off because the VDA 2.0
query-answer watchdog (WDA) is not fed - after any reset the error counter EC
starts at 6 and EC > 4 sets WDA_INT and disables the LSA drivers.

Working SPI configuration (proven on hardware, must not be changed): LSB-first
16-bit frames, CPHA=1, BR = div 16, SSM|SSI, CS = PE12, chip-select pad
configured push-pull output idle high. Everything else fails on the bench:
MSB-first and div 64 give 100% parity errors, CPHA=0 makes the chip answer
SPI_ERR (0x8000).

What was done:
| Change | File |
| --- | --- |
| SPI read pipeline: the DO reply to a read request arrives in one of the frames that follow it (datasheet 6.16.2); requests are queued (rd_pending) and matched to replies in order, rx_subaddr carries the answered sub-address | firmware/hw_layer/drivers/gpio/l9779.cpp |
| VDA 2.0 level 3 watchdog feeding (datasheet 6.15): every ~112 ms read the question from REQULO (0x10\|0x0e) plus REQUHI (0x10\|0x0f) status flags, write the expected 32-bit answer from Table 51 via WD_ANSW (0x0e); REQUHI flags (RESP_TO_EARLY / NO_RESP) adapt the response delay in 60..150 ms | firmware/hw_layer/drivers/gpio/l9779.cpp |
| Driver thread: only pushes CONTR_REG writes when outputs changed (o_dirty), wakes 1 ms before the watchdog response is due | firmware/hw_layer/drivers/gpio/l9779.cpp |
| Chip-select pad configured push-pull output idle high in init() before the first frame | firmware/hw_layer/drivers/gpio/l9779.cpp |
| tlead/tcsn timing: 2 us CS-low-to-SCK and CS-high-between-frames delays via the DWT cycle counter (datasheet requires >= 525 ns / >= 640 ns) | firmware/hw_layer/drivers/gpio/l9779.cpp |
| Fixed output mapping: regs[2] bit 4 now mirrors OUT13 (Fuel pump, BG1) instead of OUT14 (Starter) | firmware/hw_layer/drivers/gpio/l9779.cpp |
| Injector pins follow the physical wiring: OUT4 -> Injector 1, OUT3 -> Injector 2, OUT2 -> Injector 3, OUT1 -> Injector 4 (matches m74_9.yaml and the board OUTPUTS[] meta list) | firmware/config/boards/m74_9/board_configuration.cpp |

Design notes:
- EC starts at 6 after SW_RST, so the watchdog must be answered correctly
  (value AND timing) at least twice before EC drops below 4 and the LSA drivers
  turn on; after that it must keep being answered every cycle or the outputs
  drop again.
- The answer value comes from a hardcoded 16-entry table (Table 51) derived from
  the RESP_SOLL7..0 formulas in 6.15.2; there is no cheap way to disable the
  watchdog in configuration, so the driver feeds it.
- cs: the ChibiOS SPI HAL used here (SPI_SELECT_MODE_PAD) toggles the CS pad
  level but never sets its mode; without the explicit palSetPadMode() the pad
  floats and the chip never frames a transfer.

Validation:
- Bench (user-reported): with the stock config restored, `pins` shows
  parity_err=0 frame_err=0 addr_err=0, ident=0x0001 - the chip decodes the
  frames. Watchdog feeding is new in this change and needs a bench flash.

Open follow-ups:
- User flashes, then checks `pins`: expect `WDA: req=<4-bit> ec=0 wda_int=0
  ok=<growing> fail=0 delay=~105ms`. If `fail` grows, adjust the response
  timing (REQUHI flags already steer wd_delay_ms).
- Then verify outputs on the bench: injector pulse on AF4 (OUT4, Injector 1)
  and fuel pump relay on BG1 (OUT13) when commanded from the console/TS.
- Then on the car: coils via IGN1..3 (AL1/AM1, wasted spark), injector bank
  sequencing per cylinder.

## 2026-08-11 - m74_9 L9779WD-SPI: real power-stage diagnostics + corrected SPI config

What: finish the L9779 driver bring-up. The 2026-08-10 entry below claimed
LSB-first + CPHA=1 + div 16 was the working SPI config - that was wrong: LSB-first
is rejected by the chip (it answers 0x0001 to everything, ident stays 0x0000),
CPHA=0 answers SPI_ERR (0x8000) on every frame, and div 16 (9 MHz) exceeds the
8 MHz limit (Table 53). The config proven on hardware (car, engine running) is
MSB-first, mode 3 (CPOL=1, CPHA=1), BR = div 32 (4.5 MHz), plus the parity bit
forced on every transmitted frame in spi_rw_array() - frames with an even number
of set bits (e.g. every WD_ANSW byte) are silently rejected by the chip.

What was done:
| Change | Commit |
| --- | --- |
| SPI: MSB-first, mode 3 (CPOL=1, CPHA=1), div 32; parity forced in spi_rw_array() | 735f14c9ecb, 13a8b17c014 |
| Real power-stage diagnosis: DIA_REG1..8 cached in the driver thread every 100 ms, getOutputDiag() maps each output pin to its 2-bit field (SCG/OL/SCB/OK) and reports PIN_SHORT_TO_GND/PIN_OPEN/PIN_SHORT_TO_BAT/PIN_OK; pins without a power stage (OUT8..12, OUT19, MRD) report PIN_OK, cache returns PIN_UNKNOWN until the first refresh | 862efa4b8c3 |
| OUT_DIS check moved to the right register: DIA_REG10 bit 1 (was looking at IDENT bit 0); START must clear it before CONTR writes are accepted | 862efa4b8c3 |
| 'pins' now dumps DIA10 + per-channel diagnosis (OUT1-4, IGN1-4, OUT21-24, OUT25-28) and the last 32 SPI frames | 862efa4b8c3 |

Design notes:
- Reading a DIA register clears its fault bits on the chip, so the cache refresh
  (100 ms) is slower than the ~112 ms monitoring cycle - a persistent fault is
  re-armed by the chip between reads and always visible; a fault shorter than one
  cycle can be missed (inherent to the chip).
- getOutputDiag()/SensorChecker never touch SPI: they read the cache filled by
  the driver thread (uint16_t/bool, atomic on Cortex-M).
- The bench failure where injectors would not switch was NOT firmware: the bench
  rig lacked power ground (BG2/BG3) - without it the low-side drivers report
  OL and never conduct. On the car, with BG2/BG3 + BH1/BH2 + BF2 powered, the
  power stage works and L9779 drives injectors/coils.

Validation (user-reported):
- Car, engine running: ident=0x0209, spi errors 0, WDA ok=460 fail=0 (delay
  self-tuned to ~106 ms), OUT_DIS=0, OUT1-4:OK, IGN1-4:OK, fuelbench pulses the
  injector, power stage controls the loads.
- Bench lamp test across the low side never lights (low-side driver sinks to
  ground) - measure AF4/AJ4 against +12 V instead: ~9 V with the load driven.

Open follow-ups:
- User rebuilds and checks 'pins': the `ext L9779.*: diagnostic:` lines and the
  SensorChecker injector/coil OBD faults should now reflect the real chip state
  (SCG/OL/SCB on the bench without loads, OK with loads connected).
- Known race (pre-existing): 'pins' (console thread) issues SPI diag reads while
  the driver thread feeds WDA; both share rd_pending/rx_subaddr. Benign in
  practice (reads retry 3x), but a mutex around spi_rw()/spi_rw_array() would
  make it deterministic.

## 2026-08-11 - TS-over-CAN link stability on a busy bus (m74_9, AT32F435)

Symptom (user): on the car the java_console/PCAN link works 5-10 s then dies with
`Got only 4 bytes while expecting 9 for command 0x4F` + `TunerStudio errors:
underrun=1 ... outofrange=54`; on the bench (no other nodes) it is stable. Root
cause is foreign bus traffic reaching the ISO-TP receiver: `CanListener::acceptFrame`
compares only the raw identifier (`CAN_ID()` returns EID for extended frames), so an
extended frame whose low 11 bits equal 0x710 also lands in `CanTsListener.rxFifo`,
and any non-ISO-TP frame arriving between FIRST and CONSECUTIVE used to abort the
partial receive (`numReceived < 1` -> reset + break), truncating TS packets.

What was done:
| Change | File |
| --- | --- |
| bxCAN HW filter on CAN1 (single-CAN boards only: `STM32_CAN_USE_CAN2==FALSE`): FIFO0 accepts only standard 0x710 (mask includes the IDE bit, so extended collisions are rejected in HW), FIFO1 accepts everything else; RX thread drains FIFO0 first so TS frames are never starved by bus noise | firmware/hw_layer/drivers/can/can_hw.cpp |
| `CanTsListener::decodeFrame` rejects extended frames (`frame.IDE`) before enqueueing | firmware/console/binary/serial_can.cpp |
| `CanStreamerState::receiveFrame` distinguishes garbage from loss: FC/bad-type/IDE frames return 0 with ISO-TP state kept; a stale CONSECUTIVE returns -1 after `reset()` | firmware/controllers/can/isotp/isotp.cpp |
| `streamReceiveTimeout` continues on ignored frames (0) and only breaks on real desync (-1) | firmware/controllers/can/isotp/isotp.cpp |
| `sendDataTimeout` FC wait skips up to 8 foreign frames before giving up (was: first non-FC frame aborted every multi-frame TX on a busy bus) | firmware/controllers/can/isotp/isotp.cpp |
| `CAN_FIFO_BUF_SIZE` 76 -> 128, `CAN_FIFO_FRAME_SIZE` 32 -> 64 (static RAM, ~1 KB more) | firmware/controllers/can/isotp/isotp.h |
| Diagnostics: `rxFifoOverflow/ignoredFrames/desyncResets/rxFifoBufOverflow` counters + new console command `isotpinfo` | isotp.h, serial_can.h, serial_can.cpp |

Design notes:
- HW filter is applied in `initCan()` before `canStart()` (`canSTM32SetFilters` asserts
  `CAND1.state == CAN_STOP`); `can_lld_start`/`canStop` do not touch filters, so
  `setCanBaud`/`setCanListenMode` restarts keep it. With `STM32_CAN_USE_CAN2==TRUE`
  the shared filter bank split is board-specific, so the filter is left at the
  driver default there.
- FIFO0 priority in `can_lld_receive` (`CAN_ANY_MAILBOX` drains RX0 first) is what
  makes the TS-first split safe: foreign frames go to FIFO1 and can never push TS
  frames out of the 3-slot HW FIFO.
- FC frames were the most plausible truncation trigger (`receiveFrame` returned 0,
  old `streamReceiveTimeout` turned that into reset+break, dropping the pending
  multi-frame packet); the new 0/-1 contract fixes exactly that path.

Validation:
- Not compiled here (user builds m74_9 himself); unit-test build on this Mac fails
  earlier in the Java toolchain (no JDK 11, `flock` missing) - unrelated to these
  edits. Change set touches only CAN RX path + static buffers, no L9779 logic.

Open follow-ups:
- On the car run `isotpinfo` while connected: counters tell whether drops are
  foreign frames (`ignoredFrames` growing) or FIFO overflow (`rxFifoOverflow`/`rxFifoBufOverflow`).
- If a third-party node genuinely transmits standard 0x710, only changing the
  console bus ID or moving TS to a dedicated CAN avoids it - HW filter cannot help.
- Long-term: revisit `IsoTpRx::readTimeout` and the `IsoTpRxTx` path with the same
  garbage-vs-loss contract.

## 2026-08-11 - java_console ISO-TP decoder: FIRST-frame length sign-extension bug

Symptom (car, PCAN): java_console dies at connect with
`IllegalArgumentException: 2 > -126` in `IsoTpCanDecoder.decodePacket` line 93;
PCAN reader thread exits, link is gone.

Root cause: `IsoTpCanDecoder` computed the FIRST-frame total length as
`((pci & 0xf) << 8) | data[1]` where `data[1]` is a signed Java `byte`. Any
multi-frame response whose length low byte has bit 7 set (e.g. 0x180 = `0x11
0x80`, or a foreign bus node transmitting `0x10 0x80` on the same ID) turned
into a negative `waitingForNumBytes`; `Arrays.copyOfRange(data, 2, 2 + n)`
then threw. Secondary defect: `PCanIoStream.readOnePacket` let any decode
RuntimeException kill the reader thread instead of skipping the frame.

What was done:
| Change | File |
| --- | --- |
| Mask the length byte: `data[isoHeaderByteIndex + 1] & 0xff` | java_console/io/.../can/isotp/IsoTpCanDecoder.java |
| `readOnePacket` wraps `decodePacket` in try/catch, logs the offending frame hex and continues | java_console/io/.../can/PCanIoStream.java |
| `UiVersion.CONSOLE_VERSION` bumped to 20260811 (project rule for Java changes) | java_tools/version/.../UiVersion.java |

Validation: javac-level diagnostics clean; java_console not rebuilt here (user
builds). Fix is the Java counterpart of the firmware-side busy-bus work above:
firmware now survives foreign frames, and the console no longer dies on them.

Open follow-ups:
- On the car, reconnect and check `isotpinfo` counters (`ignoredFrames` etc.)
  plus console-side frame-skip messages to confirm the link holds.
- If a third-party node genuinely owns the 0x710/0x711 IDs, consider moving TS
  to a dedicated CAN or changing the console bus ID.

## 2026-08-11 - m74_9 ETB (TLE9201 via SPI2): hard-hang root cause found, init moved to custom_board_InitHardware

Symptom: after adding TLE9201 ETB support (SPI2 + PD0 CS + tle9201_add) the
board died at power-on - no response, no JTAG reaction, only flashing right
after power-up ("poking" openocd) worked. Rolling back the change restored
normal boot, so the change itself was the culprit.

Root cause (proved by code reading): `tle9201_add()` calls `chip.init()`
synchronously, and `Tle9201::init()` calls `chThdCreateStatic()`
(firmware/hw_layer/drivers/gpio/tle9201.cpp). The first version added
tle9201_add() from `board_init_ext_gpios()`, which is called by `boardInit()`.
ChibiOS `halInit()` calls `boardInit()` at the end (os/hal/src/hal.c:149),
and firmware main.cpp calls `halInit()` BEFORE `chSysInit()`. So
`chThdCreateStatic` ran on a non-initialized scheduler -> guaranteed hang /
HardFault at power-on. L9779 was never affected because `l9779_add()` only
registers a gpiochip; its thread is created later by `gpiochips_init()` from
`initSmartGpio()` (after the kernel is up).

Fix: split init by phase, same pattern as hellen154hyundai_f7:
- `boardInit()` keeps only `board_init_ext_gpios()` (L9779 registration,
  safe pre-OS);
- new `m74_9_boardInitHardware()` (PD0 CS markUsed + push-pull idle high +
  `tle9201_add(0, &tle9201_cfg)`) registered as `custom_board_InitHardware`,
  which `initHardware()` invokes after `chSysInit()` and before
  `initSpiModules()` (firmware/hw_layer/hardware.cpp:381 vs 402).

Change set (firmware/config/boards/m74_9/):
| Change | File |
| --- | --- |
| -DBOARD_TLE9201_COUNT=1, -DSTM32_SPI_USE_SPI2=TRUE | board.mk |
| SPI2 pins D1(SCK AF6)/D3(MISO AF6)/D4(MOSI AF6), is_enabled_spi_2, tle9201_cfg on &SPID2, CS PD0, boardInit/boardInitHardware split | board_configuration.cpp |
| STM32_SPI_USE_SPI2 guard (#ifndef) so board.mk define wins | hw_layer/ports/at32/at32f4/cfg/mcuconf.h |

Notes: TLE9201_CONFIG_CR1/CR2 reuse SPIv1 defines; AT32 reuses
stm32f4/device_mpu_util.h (CPU_HWLAYER = ports/stm32/stm32f4 in rusefi.mk for
ARCH_AT32F4), so SPI_CR1_8BIT_MODE=0 etc. compile as on F4. spi2_af table in
at32_spi.cpp confirms PD1/PD3/PD4 AF6 for SCK/MISO/MOSI.

Validation: not built here (user builds firmware). User to run
`cd firmware/config/boards/m74_9 && ./compile_m74_9.sh`; expected log:
`tle9201_add()=0`, then after ~1-2 s `tle9201 Detected!` and periodic
`tle9201 Diag` lines; console command `reset_9201` available.

Open follow-ups:
- Confirm board boots with the fix and TLE9201 answers on SPI2.
- Then verify throttle movement on bench (PWM=B14, DIR=B15, TPS1 PC2/PC3,
  PPS1 PC0/PC1 already configured).
- If hang persists: isolate stepwise - A) board.mk+mcuconf only, B)
  +BOARD_TLE9201_COUNT=1, C) +pins.

## 2026-08-12 - m74_9: BCM CAN protocol implemented

Implemented BCM (Body Control Module) CAN emulation for m74_9 based on PCAN-View dumps
(ignoff.trc / ignon.trc / ignon_and_start.trc, 12.08.2026).
Starter relay is 100% on BCM; rusEFI does not use starterControlPin on this vehicle.

Protocol decoded from dumps:
- 0x0350 [100ms] BCM->all: byte4 = 0x04 (ign on) / 0x44 (crank switch) / 0x84 (starter active) / 0xC4 (running)
- 0x0303 [100ms] BCM->all: byte5 bit1 = starter relay active
- 0x01F6 [10ms] ECU->all: byte2 = 0x02|0x40(crank)|0x80(run); RPM*16 BE in 0x0189/0x018A bytes 0-1
- Burst group [100ms]: 11 ECU identification frames (0x05DA .. 0x041D)

Change set:
| Change | File |
| --- | --- |
| New header: `initM74_9Can()`, `m74_9_bcmStarterActive()` | firmware/config/boards/m74_9/m74_9_can.h |
| `M74_9BcmCanListener`: RX 0x0350+0x0303, TX 10/20/100ms ECU frames, static instance | firmware/config/boards/m74_9/m74_9_can.cpp |
| Added m74_9_can.cpp to BOARDCPPSRC | firmware/config/boards/m74_9/board.mk |
| `#include "m74_9_can.h"` + `initM74_9Can()` in `setup_custom_board_overrides()` | firmware/config/boards/m74_9/board_configuration.cpp |

Key decisions:
- Single CanListener subclass: `acceptFrame()` accepts two RX IDs; `request()` (called every 5ms by
  CanWrite) drives TX via own counter (% 2 = 10ms, % 4 = 20ms, % 20 = 100ms burst).
- RPM encoded as RPM*16 big-endian (verified: 0x3203/16=800.2rpm from dump).
- No heap: one static M74_9BcmCanListener at file scope.
- All wrapped in #if EFI_CAN_SUPPORT.

Validation: m74_9 build disabled (meta-info.disabled_env); compile not run.
Code reviewed for API consistency against can_dash.cpp, can_sensor.h.

Open follow-ups:
- Validate frame timings on car with PCAN-View.
- Hook `m74_9_bcmStarterActive()` into any board-specific crank detection if needed.
- Unknown byte fields (0x0189 bytes 2-3, 0x01F6 byte3) may need tuning if BCM rejects them.

## 2026-08-12 - m74_9: BCM starter relay fix (CF error state / GND not asserted)

Car dump `rusefi_ign_and_start.trc` showed BCM physically NOT closing starter relay GND
despite 0x0350 byte4=0x84 in CAN. Root causes found by diffing against original firmware dump:

1. **0x0189/0x0186/0x018A bytes 0-1 = 0x0000** - rusEFI encoded actual RPM (0 when stopped).
   Original ECU always broadcasts 0x3200 (800 RPM * 16) as "ECU calibration loaded" flag
   even when engine is off. BCM checks this field; seeing 0x0000 = ECU not ready -> CF
   error state (byte0=CF in 0x0350) -> relay GND not asserted despite CAN showing 0x84.
   Fixed: `encodeRpmOrBaseline()` returns 0x3200 when RPM < 1.

2. **0x05E2 absent** - 1 Hz keepalive (2 bytes 0x00 0x00) present in original, missing
   from rusEFI. Added at 1000 ms period (m_counter % 200 == 0).

3. **Timing**: in the rusEFI dump ECU messages appear only at t=2094ms (BCM key event
   was at t=1856ms). Root cause: user turned key to START before ECU CAN was ready.
   Procedure: turn to RUN, wait ~3s for ECU CAN messages to appear, then turn to START.

Change set:
| Change | File |
| --- | --- |
| `encodeRpm` -> `encodeRpmOrBaseline`: returns 0x3200 when RPM < 1 | m74_9_can.cpp |
| `send0x05E2()`: 2-byte keepalive `00 00` at 1000 ms | m74_9_can.cpp |
| ECU_KEEPALIVE_ID = 0x05E2 constant | m74_9_can.cpp |
| m_counter % 200 == 0: call send0x05E2() | m74_9_can.cpp |

Validation: compile not run (board disabled in CI). Logic verified against dump.

## 2026-08-14 - m74_9: IMMO ON/OFF binary diff + decompilation progress

- Compared `LARGUS_TUN_V14-8_ANTIJRK_OFF_UOZNEW_OTSKOK-6_AFR09_IMMOON.bin` and
  `LARGUS_TUN_V14-8_ANTIJRK_OFF_UOZNEW_OTSKOK-6_AFR09_IMMOOFF.bin`.
  Only meaningful difference is byte at flash offset `0x08074BF9` (`0x01` = IMMO ON,
  `0x00` = OFF). The 4-byte block at `0x0807FFFC..0x0807FFFF` changes as a consequence
  (likely bank checksum/signature). The same `0x08074BF9` byte is `0x01` in the original
  fullflash dump.
- Updated `docs/m74_9_immo_analysis.md` with:
  - binary diff section,
  - full decompilation of `FUN_082013d0` (IMMO state machine),
  - `FUN_08201716` 1000 ms timer -> state 5,
  - callbacks A/B/C and `0x0713` transmit helpers,
  - `FUN_08202e20` / `FUN_08202cbc` / `FUN_08202254` / `FUN_0820511c`,
  - `07 14` search results (false positives: `ubfx` instructions),
  - `FUN_0800f5e4` helper and note on `FUN_08098860` / `FUN_08867b24` being identical
    main-loop copies.

Validation: binary diff verified with `cmp -l`; fullflash cross-check with Python.
No firmware build run (m74_9 disabled in CI).

Open follow-ups:
- Determine exact checksum algorithm for `0x0807FFFC..0x0807FFFF`.
- Trace `0x200003FC` to identify which slot of `0x20001018` handles IMMO response.
- Capture live IMMO exchange with corrected trigger timing.

## 2026-08-14 - m74_9: IMMO OFF dump confirms trigger gating + live trace analysis

- New CAN dump with `IMMOOFF` calibration shows the original ECU **does not send**
  any `0x0713` frame when the immobilizer is disabled. This confirms the
  `0x08074BF9` calibration byte is the hardware enable flag.
- Added `parse_immo_traces.py` to extract all IMMO exchanges from PCAN-View `.trc`
  files with timings.
- Located stored checksums in `Read_FULLFLASH_…bin`:
  - Boot KS `94B8B6D7` at `0x08022DFFC`
  - Calibration KS `DD5630D0` at `0x0807FFFC`
  - Program KS `61F8857E` at `0x080FFFFC`
- Verified that simple combinations of these checksums do not reproduce the
  observed `0x0713` trigger bytes; the trigger likely includes a session counter
  in addition to flash-derived data.
- Updated `firmware/config/boards/m74_9/m74_9_can.cpp`:
  - IMMO state machine arms only when `engineConfiguration->m74_9ImmoEnabled` is true.
  - `0x0714` challenge frames are ignored when IMMO is disabled.
  - `m74_9_isImmobilizerBlocking()` now uses the config bit and auth state.
- Updated `docs/m74_9_immo_analysis.md` with the live-trace section and captured
  tables; added `parse_immo_traces.py` helper.

Validation:
- Trace parsing verified against 5 original-ECU captures.
- Code compiles not run (m74_9 disabled in CI; no local Linux build environment).

Open follow-ups:
- Identify the exact flash-derived data + counter used to build the `0x0713` trigger.
- Reconstruct the full `0x20001018` function table to find the response-computation
  function (slots 0/2-6 are not registered in the scanned code region).
- Test `m74_9ImmoEnabled = false` on the car to confirm BCM allows start without
  IMMO exchange.

## 2026-08-14 - m74_9: IMMO type-check, function table, and calibration-flag access decoded

Capstone disassembly of the IMMO wrapper (`0x08200000..0x08240000`) plus the
pointer/descriptor tables in the first flash bank.

What was done:

| Change | File |
| --- | --- |
| Disassembled `FUN_08204ad8`, `FUN_08204b54`, `FUN_08204c30`, `FUN_082035dc`, `FUN_082021e5` | Capstone session |
| Mapped all `0x20001018` table registrars (`0x082036c4..0x08203788`) and the registration function `FUN_08202038` | Capstone session |
| Decoded the calibration pointer table at `0x0803E590` and descriptor table at `0x0804A900` | Python full-flash scan |
| Added a new analysis section to the hand-off doc | `docs/m74_9_immo_analysis.md` |

Key findings:

- **IMMO enable decision is already in RAM before the wrapper runs.**
  `FUN_08204c30` returns a type (0..3) derived from markers at `0x08200000` and
  `0x20000000`. `FUN_082035dc` then sets `SRAM[0x200003FC]` to `0` (disabled) for
  types 0/2 and `1` for types 1/3, and clears `SRAM[0x200003FE]` to `0`.
  The actual read of calibration byte `0x08074BF9` happens earlier, outside the
  readable wrapper (likely in shared library `0x080xxxxx` or SLib init).

- **`0x200003FE` getter found at `FUN_082037d8`.** Many IMMO functions start with
  `bl FUN_082037d8` and fail with `0x33` if the flag is not `1`. The only stores
  to `0x200003FE` found in the wrapper are clears (`0x08202212`, `0x08202d56`);
  the setter that initializes it from the calibration descriptor table is still
  missing.

- **Function-pointer table `0x20001018` layout decoded.** Registrars at
  `0x082036c4` (+0x04), `0x082036e0` (+0x10), `0x08203700` (+0x14),
  `0x08203718` (+0x34), `0x08203734` (+0x3c), `0x08203750` (+0x40/+0x44),
  `0x08203770` (+0x4c). `FUN_08202038` registers index `1` with function
  pointers `0x08202019`, `0x08201e2d`, `0x08201df1`, `0x08201d85`, `0x082017d9`,
  `0x08201ae1`, `0x082017f9`.

- **Full-challenge flow confirmed.** `FUN_08202cbc` parses the first `0x0714`
  frame, selects a slot index `0..6` from the challenge type, calls
  `table[idx].+0x04`, and if successful sets `0x200003FC = idx` and arms timer
  `0x20000410` for 5000 ms. `FUN_08201e2c` is the full-response entry point;
  it routes `session_word == 0xFF00` to flash-attestation
  (`FUN_0820630c` -> `FUN_08206108`) and `0xFF01` to the SLib path via
  `FUN_08204bac`.

- **Calibration descriptor table at `0x0804A900` contains entries for
  `0x08074BF8` and `0x08074BF9`.** The pointer table at `0x0803E590`
  references it, but no direct literal references to either address exist in the
  wrapper, confirming table-driven access.

Validation:
- Capstone disassembly reproduced known `0x0713` send paths (`FUN_08201208`,
  `FUN_08201260`, `FUN_082012e8`, `FUN_08204ab8`).
- `parse_immo_traces.py` still shows the same 5 full + 5 quick pairs; no new
  algorithmic match found.
- No firmware build run (m74_9 disabled in CI; macOS host).

Open follow-ups:
- Find the shared-library/SLib setter that initializes `SRAM[0x200003FE]` from
  descriptor table item `0x08074BF9`.
- Locate the `0x0713` trigger generator (likely SLib call at `0x08069028` or
  `0x080697D0`, or a not-yet-decompiled function-table slot).
- Reconstruct the full `0x20001018` table for all 7 indices to identify the
  quick-response function.

## 2026-08-14 - m74_9: added `m74_9ImmoOff` config flag for physical immo bypass

User found a physical way to bypass the immobilizer on the car and asked for a
rusEFI config flag to disable the IMMO logic.

What was done:

| Change | File |
| --- | --- |
| Added `bit m74_9ImmoOff` next to `m74_9ImmoEnabled` in persistent config | `firmware/integration/rusefi_config.txt` |
| Use both flags to decide whether the IMMO state machine arms and whether LimpManager cuts fuel/spark | `firmware/config/boards/m74_9/m74_9_can.cpp` |
| Documented the new flag in the analysis notes | `docs/m74_9_immo_analysis.md` |

Key decisions:

- Kept `m74_9ImmoEnabled` unchanged; added `m74_9ImmoOff` as an explicit override.
- IMMO is active only when `m74_9ImmoEnabled == true && m74_9ImmoOff == false`.
- `m74_9ImmoOff = true` forces the car to start without any `0x0713`/`0x0714`
  exchange, which matches a physical bypass installation.
- Config regeneration was interrupted by the user; they will run the build
  themselves on their Linux environment, which will regenerate
  `engine_configuration_generated_structures_m74_9.h`, `rusefi_m74_9.ini`,
  `VariableRegistryValues.java`, etc.

Validation:
- Code change reviewed; no compiler run locally.

Open follow-ups:
- Run `./compile_m74_9.sh` (or `gen_config_board.sh config/boards/m74_9 m74_9`) to regenerate generated files and verify the build.
- Test on car with `m74_9ImmoOff = true` and the physical bypass installed.

## 2026-08-15 - m74_9: MAP always 0 - AT32 fast ADC (TIM6 -> ADC2) does not run, switched board to slow ADC

User reports MAP on AC3 (AC3 -> RS358A -> PA1, EFI_ADC_1) always reads 0 kPa
while the rest of the board works.

Investigation (console logs from user's bench):

- `adc_report` showed `fast 0 samples` with `F ch[0] @ PA1 ADC2 12bit=0` while
  `slow 805833 samples` and all slow channels (PA0, PA2, PA3, PC3, PC5...)
  sampled fine. So the slow ADC (ADCD1/ADC1) works on AT32 with the reused
  STM32 ADCv2 LLD, but the fast ADC (ADCD2/ADC2, triggered by GPTD6/TIM6 at
  10 kHz) never completes a single conversion.
- MAP is the only fast channel on this board (TPS/PPS/CLT are slow), which is
  why only MAP showed the symptom. Both SensorType::Map branches (MapFast
  averager and MapSlow subscription) read the fast buffer because
  `enableFastAdcChannel("Fast MAP")` removes PA1 from the slow conversion
  group, so MAP was 0 while everything else worked.
- The AT32 port (ChibiOS AT32F4xx in the rusEFI fork) drives ADC via the
  unmodified STM32 ADCv2 LLD; the Artery ADC1 works that way but the fast
  path (TIM6 GPT interrupt -> ADC2 DMA) evidently does not. Not debugged
  further remotely (would need JTAG/bench): unknown whether TIM6 never fires
  or ADC2 conversion never completes. On the old firmware the "ECU: Fast ADC
  errors" gauge distinguishes the two (grows = ADC2 path, stays 0 = TIM6).

What was done:

| Change | File |
| --- | --- |
| Disable the fast ADC for this board (`EFI_USE_FAST_ADC=FALSE`) so MAP is sampled by the working slow ADC | `firmware/config/boards/m74_9/efifeatures.h` |
| Added `!EFI_USE_FAST_ADC` stubs for `enableFastAdcChannel`/`getFastAdc` so `calcFastAdcIndexes()` and `onFastAdcComplete()` still link on v2-port boards without a fast ADC | `firmware/hw_layer/ports/stm32/stm32_adc_v2.cpp` |

Key decisions:

- Did not guard `hardware.cpp` call sites with `EFI_USE_FAST_ADC` because H7
  also builds with `EFI_USE_FAST_ADC=FALSE` but still needs the v4 fast path;
  stubs in the v2 port keep the change local to F4/AT32 boards.
- Workaround, not root-cause fix: MapFast stays invalid, SensorType::Map falls
  back to MapSlow (slow-ADC rate). Map averaging (`isMapAveragingEnabled`)
  will log "No MAP values to average" per window while spinning - disable it
  in TS for now.

Validation:

- Full m74_9 build in the rusefi_build container (`make clean && bash
  bin/compile.sh -b config/boards/m74_9/meta-info.env`) passed; `rusefi.bin`
  produced. Map file confirms the `fastAdc` object is no longer linked (only
  the LLD's ADCD2 definition remains).

Open follow-ups:

- Flash and check `adc_report`: slow channel `S ch[1] @ PA1` now shows the
  real PA1 voltage. ~1.5-2.5 V = firmware fix confirmed; 0 V = the RS358A
  chain/sensor supply is also broken (measure PA1 with a multimeter).
- Proper root-cause fix of the AT32 fast ADC: re-enable `EFI_USE_FAST_ADC`,
  watch "ECU: Fast ADC errors" to split TIM6-GPT vs ADC2-DMA, then fix the
  ChibiOS AT32F4xx port (TIMv1 GPT or ADCv2 LLD ADC2 path).

Update (same day, bench flash): user reflashed and `adc_report` now shows a
real slow-ADC sample for PA1 (`S ch[1] @ PA1 12bit=25 0.020V`), so the slow
path works and MAP is no longer stuck at the dead fast buffer. Note: 0.020 V
at rest is far below the ~1.8 V expected from an MPX4250 at atmospheric
pressure while TPS on the same +5V rail reads 3.16 V - sensor/op-amp chain
still to be validated with vacuum and a multimeter.

## 2026-08-15 - m74_9: AT32 fast ADC root cause fixed (DMAMUX TBL_SEL never enabled)

After the slow-ADC workaround (previous entry) the AT32 fast ADC path (TIM6 GPT
-> ADC2 -> DMA) was brought up for real.

Investigation:

- Added board-local console command `fastadcdiag` (m74_9/board_configuration.cpp)
  dumping TIM6/GPTD6/ADC2/DMA-channel/DMAMUX state. First run: TIM6 running
  (CR1=CEN, CNT moving), ADC2 converting (SR OCCS/EOC activity, err grew with
  lastErr=2 = ADC_ERR_OVERFLOW), DMA channel configured (CNDTR=4, CPAR=ADC2->DR,
  mux CTRL=36) but `DMA1 MUXSEL=0x00000000` - the flexible request table was
  never selected, so the REQSEL=36 programmed into MUXCCTRL was ignored and the
  DMA channel served its fixed default request (not ADC2). ADC1 worked only
  because DMA1_CH1's default request is ADC1.
- Cross-checked the fork's Artery compat headers against the official
  AT32F435/437 Reference Manual (V2.07): DMA_MUXSEL is at offset 0x100, TBL_SEL
  bit 0 = flexible mapping; the fork's `DMA_TypeDef` placed MUXSEL at 0x78
  (ch[7] array stride 0x10 vs hardware 0x14) and `at32_registry.h` did not
  define `STM32_DMA_HAS_DMAMUXSEL`, so `dmaInit()` never wrote it at all.
- Even with both fixed, the write still did not land: `hal_lld_init` calls
  `dmaInit()` right after `rccResetAHB1()` with the DMA clocks OFF - writes to
  the unclocked AT32 DMA are silently dropped.

Fixes (ChibiOS fork AT32 port):

| Change | File |
| --- | --- |
| Enable `rccEnableDMA1/2(false)` before `dmaInit()` | `firmware/ChibiOS/os/hal/ports/AT32/AT32F4xx/hal_lld.c` |
| `STM32_DMA_HAS_DMAMUXSEL TRUE` | `firmware/ChibiOS/os/hal/ports/AT32/AT32F4xx/at32_registry.h` |
| Pad `DMA_TypeDef` so MUXSEL/MUXC/MUXG are at 0x100/0x104/0x120 | `firmware/ChibiOS/os/common/ext/Artery/AT32F4xx/at32f435xx.h` |
| `STM32_ADC_ADCPRE = DIV6` -> ADCDIV = HCLK/4 = 72 MHz (was 96 MHz, RM max 80 MHz) | `firmware/hw_layer/ports/at32/at32f4/cfg/mcuconf.h` |
| Keep fast ADC enabled for the board (comment updated) | `firmware/config/boards/m74_9/efifeatures.h` |

Validation (bench, m74_9):

- Built with `make clean` + `bin/compile.sh -b` (user's flow), 0 errors.
- `fastadcdiag`: `MUXSEL=0x00000001`, `fast cnt=31770` growing, `lastErr=0`
  (the small residual `fast err` counter ticks during conversion-overlap
  windows, same as on working F4 boards).
- `adc_report`: `fast 45692 samples`, `F ch[0] @ PA1 ADC2 12bit=8 0.006V`
  (no MAP sensor connected on the bench - expected; TPS on the same +5V rail
  reads 3.17 V).
- MAP is back on the fast ADC; `MapFast`/map averaging functional again.
  `fastadcdiag` kept as a board-local bring-up command.

Open follow-ups:

- Connect a MAP sensor (or apply pressure/vacuum) and confirm MAP kPa tracks
  the voltage (MT_MPX4250: ~1.86 V = 100 kPa).
- Push the ChibiOS fork fixes to rusefi/ChibiOS and bump the submodule pointer
  in rusefi main (human pushes; commits are local).

Update (same day, follow-up): user noticed `fast err` growing (109) and then
DECREASING (56) while `fast cnt` grew 21661 -> 49311 and `lastErr=0`. This is
expected: fastAdcErrorCount is a uint8 skip counter - startConversionI()
increments it when a TIM6 tick lands while the previous conversion is still
ACTIVE because its completion ISR was delayed past the tick (interrupts-off
window or long same-priority ISR). It wraps at 255, hence the apparent
decrease. With lastErr=0 and the conversion count growing at 10 kHz the ADC is
healthy; the same counter behavior exists on all GPT-triggered F4 boards.

## 2026-08-15 - m74_9: IAT/CLT on ADC3-only pins - added slow ADC3 sampling (EFI_ADC3_SLOW)

IAT (AC2, PF6) and CLT (AD3, PF5) did not read: on the AT32F435 (same as
STM32F4) all F-port pins are ADC3-only (PF6 = ADC3_IN4, PF5 = ADC3_IN15,
PF10 = ADC3_IN8, PF3 = ADC3_IN9, verified against the AT32F435/437 datasheet
pin table), and rusEFI only sampled ADC1 (slow) + ADC2 (fast) - ADC3 was
dedicated to software knock. The m74_9.yaml ids for those pins also pointed
at wrong EFI_ADC_* channels (e.g. AC2 was [F6, EFI_ADC_4] where EFI_ADC_4 is
PA4).

What was done:

| Change | File |
| --- | --- |
| New `EFI_ADC3_SLOW` capability: start ADC3, sample the 8 ADC3-only pins (IN4,5,6,7,8,9,14,15) into slow-buffer slots 32..39 via a blocking `adcConvert` in the slow loop; `#error` if combined with `EFI_SOFTWARE_KNOCK` | `firmware/hw_layer/ports/stm32/stm32_adc_v2.cpp` |
| Enlarge the slow sample buffer to 40 entries and report channels 32..39 in `adc_report` when ADC3 slow sampling is on | `firmware/hw_layer/adc/adc_onchip_slow.cpp` |
| Enable `EFI_ADC3_SLOW` for the board | `firmware/config/boards/m74_9/efifeatures.h` |
| CLT = EFI_ADC_39 (PF5), IAT = EFI_ADC_32 (PF6) in defaults AND ConfigOverrides (stored tune predates the fix) | `firmware/config/boards/m74_9/board_configuration.cpp` |
| Fix AC2/AD3/AK3/BF3 ids to EFI_ADC_32/39/37/36 (regenerates TS labels) | `firmware/config/boards/m74_9/connectors/m74_9.yaml` |

Validation (bench, make clean build, 0 errors): `adc_report` shows
`S ch[33..40] @ PF6..PF5` all sampling; `sensorinfo` shows Clt/Iat configured
on PF5/PF6 with valid voltage. With no sensors connected the pins read
~3.28 V raw (open input with the 1500 ohm pullup to +5V saturating the
3.3 V-referenced ADC), so the thermistor conversion reports 0 ohms/invalid -
expected. The 2:1 `analogInputDividerCoefficient` still has to be verified
with a real sensor (or known resistor): if the displayed resistance is off
by ~2x, override `getAnalogInputDividerCoefficient` (weak hook) to return 1.0
for EFI_ADC_32/EFI_ADC_39.

Open follow-ups:
- Connect CLT/IAT (or a known resistor on AC2/AD3) and verify the resistance
  matches; adjust the per-channel divider if needed.
- Narrowband O2 (AK3, EFI_ADC_37) and AC pressure (BF3, EFI_ADC_36) are now
  readable as well - configure them in the tune when needed.

## 2026-08-15 - 60-2 trigger: C9002 at the cranking->running transition kills the start

Symptom (car, m74_9, 60-2 crank wheel): the engine catches, then
"WARNING: C9002: primary trigger error: too many teeth between sync points:
expected 58/0 got 58/0" appears and the engine just spins without running
(RPM reads correctly). User builds with make clean + compile.sh in the
rusefi_build container.

Root cause analysis:

- For a missing-tooth wheel the decoder syncs on the gap ratio (60-2: nominal
  3.0, window was [2.25, 3.75] from setTriggerSynchronizationGap(3) plus a
  second gap [0.75, 1.25]).
- When the engine first catches, the crank accelerates so hard that the
  missing-teeth gap (36 deg of crank) takes less than 2.25x the preceding
  tooth time - the ratio check fails, the decoder counts exactly 58 events
  (one full revolution of real teeth) and hits the isValidIndex limit right
  at the position where the sync point should have been. The count is exactly
  right, only the ratio check failed: hence "expected 58/0 got 58/0".
- The decoder desyncs exactly when the engine just started firing; the user
  sees the engine catch, the warning, and then no combustion.
- The same mechanism on 36-2 was fixed upstream in PR #4138 (2022) with an
  asymmetric window [1.6, 3.5] / [0.7, 1.3]; 60-2 still used the narrow
  default.

Fix (firmware/controllers/trigger/decoders/trigger_structure.cpp):
TT_TOOTHED_WHEEL_60_2 now sets gap0 [1.6, 3.75] and gap1 [0.8, 1.2]:

- Low side widened to 1.6 (36-2 precedent) to survive the acceleration at
  first combustion.
- High side KEPT at the proven 3.75: real 60-2 cranking data
  (unit_tests/tests/trigger/resources/trigger_adc_real1.csv, 322 RPM) shows
  the gap systematically stretched to ~3.6-3.75 by compression ripple, so
  lowering it to 3.5 (as 36-2 does) would desync nearly every revolution.
- Second gap slightly tighter than 36-2's [0.7, 1.3]: 60-2 teeth are twice as
  dense in time, so adjacent teeth barely change ratio even under hard
  acceleration, and the tighter window rejects the noise-shifted-tooth false
  sync that broke test_trigger_noiseless.cpp noise#1 with the wider gap1.
- syncRatioAvg stays 3 (round((1.6+3.75)/2) = 3), so the noiseless trigger
  filter gap prediction is unchanged.

Tests:

- New unit_tests/tests/trigger/test_60_2_cranking_transition.cpp:
  crankingTransition60_2Acceleration (gap ratio 1.7 - was C9002 + desync,
  now stays synced with no warning) and crankingTransition60_2DecelerationRecovers
  (gap 4.2 is still rejected on purpose, next revolution re-syncs).
- Full unit test suite: 1129/1129 pass (run in the rusefi_build container).
- m74_9 firmware builds clean (make clean + compile.sh, user's flow).
- unit_tests/triggers.txt regenerated by the test run (it dumps the 60-2
  waveform) - committed together with the change.

Validation on the car is still pending: user must reflash and confirm the
C9002 is gone at start. If a ratio below 1.6 or above 3.75 still appears,
the TS setting "Use custom sync ratio" (overrideTriggerGaps +
triggerGapOverrideFrom/To) can widen the window in the field without a
rebuild.


## 2026-08-15 - m74_9 IMMO: log the 0x0713 trigger send only once

The "IMMO: sent 0x0713 trigger (session 0x%04x)" line was printed from
sendImmoTrigger() on every send, including the 500 ms retries in the
TriggerSent state - with no BCM challenge arriving (IMMO OFF / algorithm not
implemented) the console repeated the line every 500 ms forever. The printf
now lives in the WaitingToTrigger -> TriggerSent transition (initial send
only); retries stay silent. Firmware builds clean (incremental compile.sh
run in the rusefi_build container).

## 2026-08-16 - m74_9: engine stalls 2-3 s after start - VBatt missing + 60-2 false sync on misfire wobble

User log (car): engine starts, runs 2-3 s, stalls. Dwell overcharge warnings
(18/36 ms - user was experimenting with dwell manually; bigger dwell made it
run longer). Fatal sequence: C9003 "not enough teeth: expected 58/0 got
51/0" at 609 RPM with newerr gap0=1.601 gap1=1.195, then "engine stopped"
6 ms later.

Two root causes found:

1. VBatt was never assigned on m74_9: vbattAdcChannel/vbattDividerCoeff were
   commented out in the board defaults, so Sensor(BatteryVoltage) reads 0.
   The dwell voltage correction (interpolate2d on dwellVoltageCorr*) and the
   injector deadtime (battLagCorrBattBins) both clamp to their lowest table
   bin at 0 V - wrong spark energy and wrong injector deadtime, i.e. weak
   spark and lean/rich misfires. Assigned VBatt = EFI_ADC_3 (PA3) with the
   (33k + 6.8k) / 6.8k = 5.853 divider in both DefaultConfiguration and
   ConfigOverrides (stored tune predates it). Bench evidence: PA3 raw reads
   2.29 V stable across sessions = 13.4 V through 5.853; PA2 is the backup
   candidate - verify by varying the supply voltage and watching which
   channel tracks it.

2. The widened 60-2 sync window (gap0 [1.6, 3.75], gap1 [0.8, 1.2]) accepted
   a misfire-distorted tooth pair as a sync point: gap0=1.601 barely inside
   the new low edge, gap1=1.195 inside [0.8, 1.2] -> false sync
   mid-revolution -> count 51 -> C9003 -> desync -> stall. With the old
   gap0 [2.25, 3.75] this particular event would not have synced either, so
   it is a regression of the window widening. Fix: tighten only the second
   gap to [0.85, 1.15] - adjacent 60-2 teeth barely change ratio even under
   extreme acceleration, so this still allows the cranking transition, but
   rejects the distorted pair; the decoder then just counts one noisy tooth
   and re-syncs cleanly at the real gap (no error at all).

Tests:
- New regression test trigger.crankingTransition60_2MisfireDistortedTooth
  reproduces the exact user pair (gap0 1.601, gap1 1.195): fails (C9003) with
  gap1 [0.8, 1.2], passes with [0.85, 1.15].
- Full unit suite 1130/1130 pass; m74_9 firmware builds clean.

Open follow-ups:
- Verify PA3 = battery sense on the bench (vary supply, watch VBatt gauge);
  if wrong, switch to PA2.
- On the car: confirm VBatt reads ~14 V running and the dwell voltage
  correction table is sane; the dwell table should be reset to 4 ms since
  the user's experiments are baked into the stored tune.
- Confirm MAP reads ~100 kPa at key-on (earlier bench showed 10 kPa with no
  sensor - if it is still 10 kPa with the sensor connected, the VE model
  starves the engine).

Update (same day): VBatt calibration validated on the car - rusEFI shows
12.4 V with the engine off, matching the battery measurement (12.42 V).
The 5.679 divider is correct; dwell correction and injector deadtime now
see the real battery voltage. Next: start the engine with dwell back at
4 ms and confirm the stall + C9003 false-sync are gone.

## 2026-08-16 - m74_9: MAP/T-MAP calibration from the Bosch 0 261 230 217 datasheet

The Lada DAD 21800-1413010 is a Bosch 0 261 230 217 T-MAP (pressure + NTC,
Delphi 28234360 is the analog). User supplied the datasheet photo (in repo as
0261230217.jpg, OCR'd via macOS Vision):

- Pressure: Vout = (0.85/95 * P[kPa] - 0.1) * Us, range 10-115 kPa
  (0.4 V @ 20 kPa, 4.65 V @ 115 kPa at Us = 5 V) -> 3.974 V at 100 kPa.
- NTC: -40C=45303 ... 45C=987.4, 50C=833.8 ... 130C=89.28 ohm (full table in
  the photo), B(25/85) ~ 3550K.

Fixes:
- MAP channel (EFI_ADC_1/PA1) has a real board divider of 1.555 (raw reads
  2.555 V at 100 kPa where the sensor outputs 3.974 V), NOT the global 2.0
  guess. Added per-channel getAnalogInputDividerCoefficient() override for
  m74_9. With 2.0 the MAP voltage (and the whole VE load axis) was 1.29x too
  high - the engine ran 10 s on garbage mixture.
- MAP curve in DefaultConfiguration: MT_CUSTOM 0.4 V @ 20 kPa / 4.65 V @
  115 kPa (fresh configs; the stored tune keeps its own curve).
- IAT NTC (the T-MAP's own temperature sensor) in DefaultConfiguration:
  Steinhart-Hart points -20C=15458 / 40C=1174 / 110C=144.2 ohm, bias 1500.

Firmware builds clean. User still needs to set the same MAP/IAT curves in
their stored tune via TS (ConfigOverrides does not force curves - they must
stay tunable). TPS divider still pending multimeter measurements.

## 2026-08-16 - m74_9: parsed the rusEFI console .mlg capture, no-start diagnosis

Analyzed rusEFI_outputChannels_2026-08-16_12_21_52_468.mlg (user capture of a
failed start). Wrote mlq_dump.py (repo root) to decode the console MLQ format:

- The console .mlg is the MLVLG\0 format from
  java_console/ui/src/main/java/com/rusefi/sensor_logs/BinarySensorLog.java:
  big-endian, 24-byte header, 89 bytes per field header (type/34B name/11B
  unit/f32 scale/4B zero/1B precision/34B category), then 4 bytes per record
  (0x00, counter, u16 tms) + packed fields + 1 checksum byte. Type bytes:
  0=U8, 1=S8, 2=U16, 3=S16, 4=S32, 7=F32. Field names are the lowercase ini
  names (rpmvalue, rawmap, ...), NOT the TS display names.
- Gotcha: the 16-bit record timestamp is (epoch_ms * 100) & 0xFFFF, so it
  advances 34464 per real second and wraps every ~1.9 s - unusable for
  timing. Use the 'seconds' (uptime) field instead.

What the capture shows (start attempt at uptime 41-46 s, 131 records):

- Trigger is clean: trgsynchronizationcounter climbs 10->14->18->22->23,
  totaltriggererrorcounter stays 0, no C9002/C9003. The 60-2 window work
  holds. The user's '0 to 114' console spam is NOT an error - it is
  printGaps() from trigger_decoder.cpp when isEngineSnifferEnabled &&
  VerboseTriggerSynchDetails. The 60-2 shape has 116 events (58 teeth x 2
  edges, indices 0..115), but printGaps runs only on the sync edge (the
  user's tune syncs on Rise, i.e. even indices), the opposite edge just
  increments the index silently - so exactly 58 prints per revolution with
  eventIndex 0, 2, ..., 114. That is ~2 lines x 58 x ~3.7 rev/s = ~430
  lines/s at 220 rpm. Turn both options off in TS to unload the console
  path.
- The engine never truly runs: rpm 219 (crank) -> 542 (one catch spike) ->
  265/257/259 -> 0 the moment the starter disengages; 12 revolutions total.
- MAP pulls almost no vacuum: 99.8 kPa key-on (3.97 V - correct), dips only
  to 97.3-97.7 while cranking and 89.57 kPa (3.53 V) at the 542 rpm catch.
  Closed-throttle cranking should read 40-60 kPa. So either the MAP port/
  hose does not see the manifold, or the engine makes no vacuum (flooded/
  low compression/throttle actually open).
- Fueling follows the high MAP: cranking base 45.7 mg/cycle, running base
  27-33 mg - roughly 2x normal, engine is flooding; plugs are likely wet.
- Ignition: C9351/C9354 coil overcharge at the catch, dwellOverChargeCounter
  to 18 in one second. With 4 ms dwell and instantrpm jitter of +-60-300 rpm
  the angle-scheduled spark events arrive late and the overdwell protection
  force-fires the coils (spark happens, just late). This is a symptom of
  trigger jitter at low rpm, not a dwell table issue - keep dwell at 4 ms.

Next steps for the user:

- TS: disable Verbose Trigger Details + engine sniffer.
- Vacuum test with the engine off: pull vacuum on the MAP port and watch kPa
  drop in the console. If kPa moves, the sensor/board path is fine and the
  problem is the engine side (compression, flood, throttle position).
- Dry the plugs / crank with the throttle open to clear the flood, then
  re-check cranking MAP: must drop to 40-60 kPa with the throttle closed.

Update (same day, 13:18-13:22 captures): four new .mlg files in the repo root
(13_18_11_642, 13_19_53_750, 13_21_17_732, 13_22_09_342) change the picture:

- MAP chain is proven good: key-on 99.8 kPa (3.97 V, atmosphere), 79.9 kPa
  at a 654 rpm catch (13_22), 75-77 kPa at 666-804 rpm (13_19), and it
  tracks the throttle to 99.9 kPa at WOT (13_22). The Bosch curve + 1.555
  divider are correct.
- The 'cranking MAP must be 40-60 kPa' advice was WRONG for this car: it is
  E-gas (ETB on TLE9201, TPS follows the pedal in 13_22, no separate IAC),
  and E-gas engines crank with the plate cracked open - 94-96 kPa MAP while
  cranking is expected, not a vacuum leak. User confirms the intake is
  tight and the car starts on the OEM ECU.
- The engine now catches every attempt (480-804 rpm) but cannot sustain.
  Two distinct kill modes across the four captures:
  * C9002 trigger desync 1-2 s after the catch at 500-804 rpm (13_18,
    13_19). Best run 13_19: 270->666->804 rpm, MAP 77.6->75.3 kPa,
    sync counter 23, then C9002 and instant stop.
  * Slow collapse at ~260 rpm with constant coil overcharge (13_21,
    13_22): in 13_22 the user opened the throttle to WOT (TPS 94.9%,
    pedal 95.6%, MAP 99.9 kPa) and the engine still would not rev -
    spark events keep arriving late (dwellOverChargeCounter 1->11->14->15)
    and the overdwell protection force-fires.
- The C9002 gap ratio at the failure is not yet captured. The firmware
  prints 'newerr TRG ... gap=X.XXX expected from 1.600 to 3.750' on every
  trigger error (silentTriggerError default off), so the user just needs to
  capture the console text around the stall to see which side of the window
  fails (or whether an extra noise edge appears inside the gap).

Update (same day, console capture at the 10:56 catch): the failure is a
noise burst, not a window problem. Verbose trigger capture at the catch
(290->543 rpm) shows a ~7 ms burst of extra VR edges right at the first
combustion: rise-to-rise intervals collapse monotonically from the normal
3.68 ms down to ~33 us (30 kHz chatter), after one slightly stretched
interval (~1.185x). Reconstruction of the intervals and the
'synchronizationPoint @ index 48 expected 58/0 got 25/0' message:

- A noise-distorted interval pair mid-revolution matched the sync gap
  windows -> FALSE sync ~25 events before the real gap; the real gap then
  arrives with the wrong count -> C9003 -> desync -> engine dies. Same
  mechanism as the earlier C9003 at 51/58, with a different distorted pair.
- The always-on TriggerNoiseFilter (trigger_central.cpp) cannot reject the
  burst: it compares each level-period against the previous one with +-33%
  tolerance, and the chatter is self-similar (each period >= 2/3 of the
  previous). The AT32F435 EXTI has no hardware digital filter (F4-class).
- The burst starts exactly at the transition to running (starter
  disengagement / first strong coil+injector currents / ETB PWM) - board
  pickup on the VR line, not a wheel or sensor defect (OEM ECU on the same
  board/sensor runs fine).

Options, cheapest first:

- TS toggle useNoiselessTriggerDecoder = yes (worth retrying: the earlier
  'masks C9002 then C9001' verdict was for the old failure mode and old
  windows; the noiseless decoder predicts event timing and should reject
  the burst). Watch for C9001 - revert if it appears.
- Board-level RC on the VR trace at PF8 (e.g. 1k series + 1nF to ground,
  tau 1 us): kills 30 kHz chatter, passes 60-2 teeth far beyond redline
  (tooth pulse 167 us at 6500 rpm).
- Firmware fallback: gate sync-point acceptance on the event count while
  synced (only accept a gap at the expected count 58); this turns the
  false-sync C9003 into 'ignore the fake gap and keep phase'. Shared
  decoder logic - needs the full unit-test suite before landing.

Update (same day, evening): root cause found for the 'catches periodically,
then free-spins' behaviour - the board default was IM_WASTED_SPARK while the
car has four individual COP coils (all four L9779 ignition pre-driver
channels populated on the board). getIgnitionPinForIndex() in wasted spark
returns cylinderIndex % (cylinders/2), so only ignitionPins[0] and [1] ever
fire - two cylinders never get spark, the engine catches on the other two
and free-spins in between (overcharge logs confirm: only two coil channels
ever appear). Fix in m74_9 board config:

- DefaultConfiguration: ignitionMode = IM_INDIVIDUAL_COILS, all four
  ignitionPins = L9779_IGN_1..4 (physical: AL1=IGN_1->Coil 1, AM2=IGN_2->
  Coil 2, AM1=IGN_3->Coil 3, AM3=IGN_4->Coil 4).
- ConfigOverrides: force ignitionMode + the four ignitionPins on every boot
  (the stored tune predates the fix and holds IM_WASTED_SPARK).
- m74_9.yaml coil labels: dropped the 'Coils X,Y / not populated' wasted-
  spark hints.
- With IM_INDIVIDUAL_COILS the cranking runs two-wire wasted automatically
  (spark_logic isTwoWireWasted), so cold start works even before the cam
  half-moon syncs; full sequential needs the PB9 cam input.

Injector chain verified healthy along the way: 'pins' shows L9779 WDA
EC=0/wda_int=0/OUT_DIS=0 and OUT1-4 diag OK; the fuelbench run prints
'Diag says Ok' while the output is on, i.e. the injector conducts current.
The rail-pressure test was inconclusive (pump off, residual pressure, small
pulse volume); re-test with the pump on and fuelbench 1 100 50 20.

VBatt channel fixed: PA3 is NOT the battery. Charger tracking test
(battery 12.4 -> 13.6 -> 14.42 V): PA3 moved DOWN (2.302 -> 2.184 V,
regulated line), PA6 (1.536 -> 1.699 -> 1.798 V) and PA7 (1.577 -> 1.743 ->
1.844 V) both follow the battery. PA6 matches the 33k/4.7k = 8.02 divider on
all three points (PA7 looks like 68k/10k = 7.8 and is the backup).
vbattAdcChannel = EFI_ADC_6, vbattDividerCoeff = 8.02 in both
DefaultConfiguration and ConfigOverrides. Verify on the car: VBatt must dip
to ~9-10 V while cranking (this is what drives the dwell voltage correction
and injector deadtime during start).

## 2026-08-16 - stock M74 calibration converted to rusEFI VE and target-AFR tables

User exported two .clb maps from the original Itelma M74 calibration:

- Bazovoe_modelnoe_ciklovoe_napolnenie_IM=0_dlin_vpusk.clb: 24x16 modeled
  cycle air charge (mg per cyl per cycle) over RPM (600..6250) and MAP
  (100..1500 mbar). This is the stock ECU's air-mass model.
- Sostav_smesi_dlya_rezhima_PM_koef_int_=1.clb: 17x10 target lambda over
  RPM (500..6250) and air charge (50..500 mg).

Conversion to rusEFI tables on the default 16x16 axes (veRpmBins /
lambdaRpmBins 650..7000 via setRpmBin, veLoadBins 10..160 kPa,
lambdaLoadBins 30..250 kPa):

- VE% = aircharge_mg / (MAP_kPa * 4.7526), where 4.7526 mg/kPa is the full
  400cc-cylinder charge at 101.325 kPa / 20 C (same constant as rusEFI
  idealGasLaw in speed_density_base.cpp).
- Target-AFR table = ПМ lambda looked up at the stock-modeled air charge
  for each (rpm, kPa) cell, shown in AFR units.

Sanity points: 800 rpm / 35 kPa -> 113.5 mg -> VE 68% (stock model, higher
than the generic 45-50% rule of thumb - trust the stock model); 4000 rpm /
100 kPa -> 452 mg -> VE 95%; 6250 rpm / 100 kPa -> VE 77%.

Cross-check against the 18:23/18:48 logs: at cranking (250 rpm, 94 kPa)
the stock model gives 326 mg -> 22.2 mg stoich per cyl/cycle, so
crankingCycleBaseFuel 20-27 mg x crankingFuelCoef 1.5-1.6 is correct, and
the observed 124 mg/cycle (crankingFuelCoef ~4.6 in the tune) is a ~5.6x
flood - consistent with the measured 14-17 ms cranking pulses.

Open follow-ups:
- ПМ is one stock mode; if the tuning tool has a separate main part-load
  mixture map (or idle map), convert it the same way for a better AFR
  table (the ПМ map has a lambda 0.84 zone at part load above ~2250 rpm).
- Absolute scale of the stock air model may be off by a few % (the stock
  applies extra temperature/mode coefficients on top); verify against a
  wideband and scale the VE table.
- Cells above 150 kPa are flat extrapolation (irrelevant for NA).

Update (same day): user pointed out the engine is NA so the load axis should
not exceed 100 kPa, and asked for the TS orientation. Regenerated both
tables with veLoadBins/lambdaLoadBins = 20, 30, 35, 40, 45, 50, 55, 60, 65,
70, 75, 80, 85, 90, 95, 100 (16 bins, 5 kPa steps across the idle/cruise
range, top row 100 kPa covers WOT; interpolation is flat past the last
bin). Tables printed rows=load, cols=RPM to match the TS table editor
(xBins = veRpmBins, yBins = veLoadBins). User has veOverrideMode = MAP.

Update (same day, evening): user could not edit the table axes in TS. Instead
of fighting the TS curve editor, the axes are now forced from firmware:
m74_9 ConfigOverrides sets veLoadBins and lambdaLoadBins to 20..100 kPa (16
bins) and restores the default crankingFuelCoef curve on every boot (the
stored tune holds ~4.6 at 20 C, a ~3x flood). TS axis editing notes: axes
are edited via right-click -> Y Axis/X Axis curve editor, not inline in the
grid; editing offline does not persist (rusEFI stores the tune only on the
ECU); bins must be strictly ascending or the burn is rejected by
validateConfig. Fresh rusefi.bin built and placed in firmware/deliver/.

## 2026-08-16 - java console 'failed to save tune: ordinal out of range 280' fixed

Saving the tune from the rusEFI console failed with OrdinalOutOfRangeException
in ignitionKeyDigitalPin: the m74_9 ini gpio_list contains only brain pins
(178 entries, max ordinal 177) while the firmware holds L9779_PIN_KEY (280,
forced by m74_9 ConfigOverrides). The value is valid firmware-side; only the
.msq serialization path threw. Fix in java_console (inifile module):

- ConfigurationImageGetterSetter.getStringValue: an enum ordinal beyond the
  ini list is serialized as its raw number instead of throwing, so the tune
  file saves.
- setValue2: accepts a raw numeric ordinal as a fallback so such tunes
  round-trip on load.
- ConfigurationImageGetterSetterTest updated to cover the numeric
  serialization and its round-trip; :inifile:test passes (docker gradle).
- Rebuilt console/rusefi_console.jar via :ui:shadowJar.

Note: 'save tune' only writes a local .msq backup; per-field changes and
Burn are unaffected, so the error was never losing ECU-side settings.

## 2026-08-16 (evening) - CAN/ISO-TP link drops on tune write fixed (firmware + console)

User could not write the prepared 21129.msq tune over PCAN/CAN: multi-frame TS
responses were truncated ("Got only 137 bytes while expecting 462"), the burn
CRC response was lost, and after the first ConnectionWatchdog restart the
console never reconnected ("restart: port not available, skipping connect").

Root causes found and fixed:

- Java LinkManager.restart() only reconnected ports present in getCommPorts()
  (serial ports). PCAN/SocketCAN/TCP never appear there, so one watchdog
  restart killed the session for good. Now non-serial transports are treated
  as always available and reconnected directly.
- m74_9 BCM emulation floods the bus with ~660 frames/s. While a serial
  (ISO-TP/TS) session is active the emulation now goes quiet (engine off
  only - with the engine running the BCM still needs the frames for
  IMMO/fuel pump). The IMMO tick is kept outside the gate so the one-shot
  handshake still completes.
- Engine::pauseCANdueToSerial was a latched bool; converted to an
  auto-expiring timestamp (pauseCANdueToSerialUntil, CAN_SERIAL_PAUSE_MS =
  3000) so board CAN traffic resumes when the serial session goes idle.
- isotp.cpp FC-wait skipped at most 8 foreign frames (~12 ms on this bus)
  before aborting a multi-frame TX; now it waits the full timeout.
- CanTxMessage destructor TX timeout raised from 100 ms to 1000 ms for
  CanCategory::SERIAL frames so a busy mailbox no longer truncates a
  multi-frame response mid-stream.
- isotpinfo console command now also prints canWriteOk/canWriteNotOk.

Validation: m74_9 clean rebuild OK (deliver/rusefi.bin), unit_tests
testCanSerial 6/6 green, java :ecu_io:test green, console jar rebuilt via
'gradlew clean :ui:shadowJar'.

## 2026-08-16 (night) - m74_9: IMMO disabled entirely (m74_9ImmoOff forced on)

The ECU-side IMMO challenge-response is not implemented (computeImmoResponse
returns false), so the 0x0713/0x0714 handshake traffic (trigger, retries,
quick-recheck responses) is pure CAN noise. The car has a physical bypass
that answers the BCM, so the ECU's IMMO frames are useless.

- m74_9_can.cpp: the whole IMMO path is now gated on the m74_9ImmoOff config
  bit (persistent_config_s): 0x0714 challenge frames are rejected in
  acceptFrame() (never enter the RX path), the state machine is never armed
  on ignition-on, tickImmo() resets to Idle and returns without sending, and
  isImmoAuthenticated() reports authenticated while the bit is set.
- board_configuration.cpp ConfigOverrides forces config->m74_9ImmoOff = true
  on every boot (stored tune predates the bit). Remove the force once
  computeImmoResponse() is implemented and IMMO is wanted back.
- m74_9_isImmobilizerBlocking() still returns false unconditionally (comment
  updated); LimpManager ClearReason::Immobilizer stays unused.

Validation: m74_9 clean rebuild OK, fresh deliver/rusefi.bin.

## 2026-08-16 (late night) - m74_9: load-tune truncation root cause fixed (FC wait consumed RX frames)

After the link-drop fix the user could load 21129.msq, the burn ran (MFS write
OK) but the ECU kept old values after reboot, and the console showed
"Got only 18 bytes while expecting 462 for command 0x43" + an outofrange
storm (555 out-of-range responses, zero write chunks counted).

Root cause: sendDataTimeout()'s flow-control wait pulled frames from the
shared RX FIFO and re-parsed them through receiveFrame() while looking for
the FC. The console runs a background output-channel puller, so new request
frames arrive while the ECU is still FC-waiting for its previous multi-frame
response. The wait consumed the first frames of the next write chunk
(FF + CFs -> exactly the observed 18 bytes), corrupted the ISO-TP RX state
(desync), and everything after was mis-parsed as out-of-range garbage. The
burn then persisted the stale page buffer - hence old values after reboot.

Fix:
- ICanReceiver::waitForFlowControl() added; CanTsListener routes FC frames
  (0x3x) to a counter slot instead of the RX FIFO (they acknowledge OUR TX,
  they are not RX data), and CanTransport polls that slot (1 ms granularity,
  full timeout budget). Foreign frames stay queued for the next command.
- sendDataTimeout() no longer consumes/parses foreign frames while waiting
  for FC; blockSize/separationTime are read from the routed FC payload.
- TestCanTransport mock implements the new pure virtual (unit tests).
- UiVersion.CONSOLE_VERSION bumped 20260811 -> 20260816 (was missed when
  LinkManager was changed), console jar rebuilt.

Validation: m74_9 clean rebuild OK (deliver/rusefi.bin), simulator target
also builds (bundle), testCanSerial 6/6 green, :ui:shadowJar green.

## 2026-08-16 (night round 2) - m74_9: ISO-TP burst frame loss fixed (RX thread preemption + burst pacing)

The FC-wait fix helped but the tune load still lost frames mid-burst: "Got
only 25/32/74 bytes while expecting N" even with the console just idling
(output-channel pulls only), 2719 out-of-range responses in one session,
and the burn persisted stale page data - the user's VE table never changed.
writeData() retried silently for 10s and then the console burned anyway,
so the load "succeeded" while nothing was written.

Root cause: the bxCAN hardware FIFO0 (3 frames deep) holds the TS id
exclusively (hw filter routes 0x710 to FIFO0). The CAN RX thread
(NORMALPRIO+6) sat below ETB (+9), GPIOCHIP/L9779 (+8), CAN TX (+7) and
the main loop (+10) - during a back-to-back burst a pile-up of that
higher-priority work stalled the RX thread past ~700 us, FIFO0 overflowed,
and one lost frame desynced the whole ISO-TP stream (the truncation point
moved around: 18/25/32/74 bytes across attempts).

Fixes (defense in depth):
- PRIO_CAN_RX NORMALPRIO+6 -> +11: the RX thread now outranks every
  thread-level preemptor. Per-frame work is a few microseconds, so the CPU
  cost is negligible even on a busy bus.
- Java IsoTpConnector.sendStrategy paces consecutive frames with 1 ms gaps:
  a 66-frame chunk takes ~66 ms and the receiver has multi-ms preemption
  tolerance instead of 3 hw slots worth (~700 us).
- BinaryProtocol.writeData throws IllegalStateException when the chunk
  retry loop exhausts: the tune load now fails loudly instead of burning
  stale page data (the "load succeeded but values are old" trap).

Validation: m74_9 clean rebuild OK, simulator target builds, testCanSerial
6/6 green, :ecu_io:test + :ui:shadowJar green (console jar rebuilt,
CONSOLE_VERSION 20260816).

## 2026-08-16 (macOS) - build adapted to run natively on the macOS host

The user moved firmware/console/unit-test builds from the rusefi_build Docker
container to the macOS host (arm64). All blockers found and fixed:

- firmware/bin/compile.sh: prefers grealpath/gnproc (native /bin/realpath has no
  --relative-to), sed -r -> sed -E, prefers gmake (macOS make is GNU 3.81).
- common_script_read_meta_env.inc: 'cut -f -1' replaced with bash parameter
  expansion (BSD cut rejects negative field numbers).
- bundle.mk: skips the Windows simulator on Darwin (BUNDLE_SIMULATOR default
  false there); 'ln -rfs' replaced with an LN variable (BSD ln has no -r);
  hex2dfu: the prebuilt .bin is a Linux x86_64 ELF, so on Darwin a native binary
  is built from misc/hex2dfu/hex2dfu.c. GOTCHA: the hex2dfu rule must live after
  all conditional blocks - a TAB-indented line inside an ifeq that follows a
  rule parses as a recipe of that rule, silently un-defining variables (LN
  expanded empty and xargs ran '{}' as a command).
- hw_layer/mass_storage/create_image.sh + create_ini_image*.sh: brew sbin added
  to PATH for mkfs.fat/fatlabel/mcopy; GNU 'expr substr' replaced with bash
  ${var:0:n}.
- java_tools/java_tools.mk: flock auto-detect (brew flock or run unlocked).
- unit_tests/test.sh + run_coverage.sh: gmake/gnproc/sysctl detection.

Toolchain: Homebrew arm-none-eabi-gcc ships WITHOUT newlib (no target stdint.h)
- unusable. Installed ARM GNU Toolchain 14.2.1 (darwin-arm64) to
/opt/arm-gnu-toolchain (same version as the container). Gradle's
languageVersion=11 toolchain needs the daemon on JDK 11: brew openjdk@11
(JAVA_HOME). Installed via brew: coreutils, make, mtools, dosfstools, flock,
python3.

Validation (all on the host): m74_9 clean build OK (deliver/rusefi.bin),
bundle + snapshot OK, unit_tests testCanSerial 6/6 green, console jar via
gradlew clean :ui:shadowJar OK. See docs/macos-local-build.md for setup steps.

## 2026-08-16 (night) - Java stack migrated to JDK 25 (Gradle 9.7)

The whole Java side now builds and tests on JDK 25:

- Gradle wrapper 8.14.5 -> 9.7.0 (8.x runs on JVM <= 24; Java 25 support starts
  at Gradle 9.1 per the official compatibility matrix). The stale "gradle 8
  needed for java 8 compatibility" comment was wrong: Gradle 9 still compiles
  with source/target 8, which is what the TS plugin runtime needs.
- Root build.gradle toolchain languageVersion 11 -> 25. Bytecode stays Java 8
  (sourceCompatibility/targetCompatibility = '8', verified: class major 52)
  because TunerStudio ships a Java 8 runtime for the plugin.
- ts_plugin_launcher/build.gradle: removed a stray 'java.toolchain { 11 }'
  inside the jar block (an unused variable that silently overrode the root
  toolchain); removed shadowJar 'manifest { inheritFrom ... }' - Shadow 9
  inherits the jar manifest automatically and the explicit inheritFrom
  self-merges into a StackOverflowError.
- dependencies.gradle + root build.gradle: testRuntimeOnly
  junit-platform-launcher (Gradle 9 removed the implicit launcher wiring).
- trigger-ui/build.gradle: failOnNoDiscoveredTests = false (its only test
  source is a manual sandbox, not a JUnit test).
- AbstractWizardStep.readValue: returns null when a stored enum ordinal is
  beyond the board's INI enum list - the numeric fallback added for the
  L9779_PIN_KEY tune-save fix previously leaked into the wizard and broke
  CltSensorPanelTest/TpsPanelTest (pre-existing failures from that fix,
  surfaced by running the full 'gradlew test' for the first time in a while).
- shadowVersion 8.3.6 -> 9.6.1, foojay-resolver-convention 0.8.0 -> 1.0.0.
- CI: setup-java default 17 -> 25, gen-configs.yaml 21 -> 25.
- .devcontainer/Dockerfile: Adoptium JDK 25 installed (gradle 9 refuses to run
  on the image's default java-11).

Validation on the mac (JDK 25.0.4): 'gradlew test' all modules green,
'gradlew clean :ui:shadowJar' OK, m74_9 clean build + bundle OK (gradle
config-gen runs under 25). The user's rusefi_build container no longer exists
(host-only builds); the Dockerfile is ready for the next image.

## 2026-08-17 - Java deps bumped + bytecode raised to Java 25

- Bytecode: sourceCompatibility/targetCompatibility 8 -> 25 (class major 69,
  verified). The [tag:java8] compat shims (CompatibilityFiles/CompatibilityOptional/
  CompatibilitySet) are now obsolete but left in place.
  CONSEQUENCE: the TunerStudio plugin modules can no longer run inside
  TunerStudio's bundled Java 8 runtime (TS needs a newer JVM).
- Dependencies (all validated by the full test suite):
  junit 5.10.2 -> 6.1.3 (plain @Test tests, no MockitoExtension - migrated
  cleanly), junit-platform-launcher now follows junit5Version, mockito
  4.11.0 -> 5.23.0, snakeyaml 1.26 -> 2.6, antlr 4.13.0 -> 4.13.2,
  jetbrains annotations 16.0.1 -> 26.1.0, commons-logging 1.2 -> 1.4.0,
  jakarta.xml.bind-api 3.0.1 -> 4.0.5 + jaxb-runtime 3.0.2 -> 4.0.9,
  httpclient5 5.4.2 -> 5.6.4, httpcore5 5.3.3 -> 5.4.3, javacan-core
  3.2.4 -> 3.5.2.
- snakeyaml 2.x migration notes: Constructor(Class) removed (now
  Constructor(Class, LoaderOptions)); global tags are rejected by default,
  so ConfigurationImageMetaYamlUtil whitelists tag:yaml.org,2002:com.opensr5.*
  via LoaderOptions.setTagInspector.

Validation: gradlew clean test all green (JDK 25), console jar rebuilt
(Build-Jdk 25, bytecode 69), m74_9 clean build + bundle OK.

## 2026-08-17 - second VE + two ignition tables filled in 21129.msq from stock M74 maps

User exported the remaining stock M74 maps and asked to fill them into the
21129.msq tune:

- secondVeTable <- Bazovoe_modelnoe_ciklovoe_napolnenie_IM=1_korot_vpusk.clb
  (short-runner air-charge model, 24 RPM x 16 mbar, mg/cyl/cycle). Variable
  intake geometry on the car: IM=0 (long) already drives the primary VE table,
  IM=1 (short) now drives the second VE table.
- ignitionTable (primary) <- Bazovyj UOZ rezhim CHN.clb (17 RPM x 17 mg)
- secondIgnitionTable <- Bazovyj UOZ rezhim PM.clb (17 RPM x 17 mg)

Conversion (script m74_9_clb_to_msq.py at repo root, kept for reproducibility):

- Grid: RPM bins 650..4700+7000 and load bins 20,30,35,40,45,50,55,60,65,70,
  75,80,85,90,95,100 kPa - the same axes the primary VE/AFR tables already
  use (rows = load, cols = RPM). RPM > 6250 clamps to the stock map's last
  column (matches how the primary VE table was converted).
- VE% = aircharge_mg / (MAP_kPa * 4.7526), same constant as the primary
  conversion (report 2026-08-16). Repro of the primary table from IM=0
  matches the tune's existing veTable within +-0.5%.
- Ignition: the stock load axis is air charge (mg), rusEFI's is MAP (kPa).
  For each (rpm, kPa) cell the modeled mg is computed from the matching
  air-charge model (CHN pairs with IM=0, PM with IM=1) and the stock map is
  looked up at (rpm, mg). This reproduces stock timing under MAP control.
- ignitionLoadBins + secondIgnitionLoadBins changed 21..120 -> 20..100 kPa
  (NA engine, cap at 100 kPa, axes now match the fuel tables).

.msq mechanics: the second tables live on TS page 4 (page4_s), not in the
main image. The .msq now carries a second <page number="3" size="1268">
block (page number = ini page - 1, the convention MsqFactory writes).
Verified end-to-end with a throwaway JUnit test running the console's own
mergeCalibrationsWithPartialFailure path against rusefi_m74_9.ini:
PagesToWrite=[0, 768], FailedFields=[], and the migrated page images
round-trip the exact table values. Load through the usual console load-tune
(no console rebuild needed - the path already existed).

Assumptions to confirm on the car:
- CHN <-> long runner (primary), PM <-> short runner (second). The stock maps
  look nearly identical in shape; if the car runs worse on the second
  ignition map, swapping the pairing is a one-line change in the script.
- Nothing activates the second tables yet: switching is the user's wiring
  decision - secondVeTableInput/secondIgnitionTableInput (hard switch pins)
  or secondVeBlendParameter/secondIgnitionBlendParameter (gppwm channel) with
  their blend bins/values. Until configured the ECU keeps using the primary
  tables.
- Firmware ConfigOverrides only force the primary veLoadBins/lambdaLoadBins;
  the second-table bins now come from the tune itself.

## 2026-08-17 - load tune now applies secondary TS pages (second VE/ignition tables)

Symptom: loading the patched 21129.msq through the console's load-tune migrated
page-0 fields (ignitionTable, lambdaTable, ...) but silently skipped the page-4
second tables - the log showed no secondVeTable/secondIgnitionTable lines and the
tables stayed empty on the ECU after power cycle.

Root cause (all in the console): TuningToolbarWidget.applyLoadedTune merged the
tune onto the page-0 image only and passed the whole set of secondary ini field
names as additionalIniFieldsToIgnore (leftover from the 2026-08-03 partial-tune
corruption fix), and the upload path (bp.uploadChanges) writes page 0 only. So
fields living on TS page 4 (page4_s: secondVeTable, secondIgnitionTable, their
bins) were dropped by design of that UI path - not by the msq file itself.

Fix (TuningToolbarWidget):
- Replaced applyLoadedTune with mergeLoadedTune(tune, sourceIni, targetIni,
  targetPages, callbacks): the same mergeCalibrationsWithPartialFailure pipeline
  but with an empty ignore set, so secondary fields migrate.
- Connected load now reads the ECU's current secondary pages (readFromPage per
  pageIdentifier), builds target pages = working image (page 0) + ECU pages 2..5,
  merges, and uploads via CalibrationsUpdater.INSTANCE.updateCalibrations(bp, lm,
  calibrations, cb) - page 0 through the TS chunk protocol, secondary pages
  through write + read-back verify + burn per page, only for pages that actually
  received migrated fields. Failed page reads degrade to a warning and skip that
  page's fields instead of aborting the load.
- Offline load (no ECU) is unchanged: page-0 image only.

Tests: TuningToolbarLoadTuneTest rewritten to drive mergeLoadedTune (AFR ->
lambda encoding case kept) + new secondaryPageFieldsAreMergedOnConnectedLoad
(luaScript on page 0x0400 migrated, pagesToWrite = {0x0400}, value round-trips).
:ui:test and :ecu_io:test full suites green; console jar rebuilt
(console/rusefi_console.jar, UiVersion.CONSOLE_VERSION -> 20260817).

Note: firmware-side multi-page TS protocol (R/W/C/B commands with page argument)
was already there and is the same path LuaService uses for the lua page - only
the console needed fixing.

Follow-up (same day): the first version of the load-tune fix read the ECU's
secondary pages directly on the AsyncJobExecutor thread, which immediately
failed with "Communication on wrong thread. Use linkManager.execute or
linkManager.submit". All BinaryProtocol IO is thread-affine to the LinkManager
thread; the page reads (and the merge) are now wrapped in
linkManager.submit() with a latch, the result handed back via atomics, and the
upload still goes through CalibrationsUpdater (which submits internally).
:ui:test + :ecu_io:test green, console jar rebuilt.

## 2026-08-17 - tune text normalized to console-exact format (no more phantom re-migrations)

Symptom: after every load-tune the console listed the same fields for restore
(secondVeTable, ignitionTable, lambdaTable, ...) and re-burned them, so it was
impossible to tell whether the ECU actually held the values - the user read the
persistent diff list as "values not applied".

Root cause: the msq table texts were hand-formatted (%g, no leading newline) while
the console generates array values as ArrayIniField.formatValue + StringFormatter.
niceToString (leading newline, 8-space indent, one decimal for scale-0.1 fields).
DefaultTuneMigrator compares the raw strings, so even identical values kept
"migrating" on every load. lambdaTable additionally: the ini scale is
{useLambdaOnInterface ? 1/147 : 1/10} and parseDigits on the digits expression
falls back to 3, so the console renders 1/147-grid values with Double.toString
trailing-zero trimming (1.0, 0.952, 0.878, ...); the 0.01-quantized "1.000/0.950"
text could never match.

Fix (m74_9_clb_to_msq.py):
- fmt_table/fmt_rows emit byte-identical formatValue whitespace; tables rendered
  %.1f, bins %d, lambdaTable re-rendered through the 1/147 grid with
  Double.toString-style trailing-zero trimming, original AFR-conversion values
  restored (LAMBDA_ORIGINAL).
- Page-3 block re-insertion fixed: the drop regex used to eat the newline after
  page 0's </page>, silently deleting the inserted page.

Validated with a throwaway JUnit harness: all nine patched constants
(secondVeTable/LoadBins/RpmBins, secondIgnitionTable/LoadBins/RpmBins,
ignitionTable, ignitionLoadBins, lambdaTable) round-trip byte-identically through
ConfigurationImageGetterSetter.setValue2/getStringValue, i.e. after one successful
burn the next load-tune produces zero restore lines - a truthful persistence
check. :ui:test + :ecu_io:test green. Console jar unchanged (no code change);
the deliverable is the regenerated 21129.msq.

Note: the first burn still writes page 0 + page 0x0300 and the MFS write can take
~2 s when garbage collection kicks in (MFS_WARN_GC status 2), which trips the
843 ms gap-in-time critical warning - cosmetic for a bench write, harmless to the
stored data, and no longer repeated since re-loads now burn nothing.

## 2026-08-17 - m74_9 bricked after power-cycle during burn: MFS recovery build

Symptom: after the page-0 burn (lambdaTable fix-up) the user power-cycled the ECU
and it stopped talking (console/TS no connection), even after reflashing the
normal rusefi.bin via OpenBLT - so the app boots but wedges before console/CAN
init, most likely in initStorageMfs (MFS mount of banks corrupted by an
interrupted write / the first GC this unit ever performed).

Recovery tooling: a one-shot recovery build that erases BOTH MFS banks before
mounting, guarded by M74_9_RECOVERY_ERASE_MFS (storage_mfs.cpp +
m74_9/board.mk, changes reverted after the build). Built and saved as
firmware/deliver/rusefi_recovery_erase_mfs.bin; the normal build was rebuilt
afterwards so deliver/rusefi.bin is the regular image. Usage: flash the recovery
bin, boot once (MFS erased, ECU starts with defaults and should connect),
then immediately reflash the normal bin and reload the tune - the recovery
build erases MFS on EVERY boot and must not stay on the ECU. Nothing committed:
the source changes were build-only and reverted.

## 2026-08-17 - console tuning UI no longer renders secondary-page fields (brick root cause)

Root cause of the brick: the console's tuning UI (CalibrationDialogWidget)
renders ini dialogs that reference fields on secondary TS pages - "Second VE
Table" / "Second Ignition Table" (page 4) - but resolves them against the
page-0-only ConfigurationImage with their PAGE-RELATIVE offsets. So the console
DISPLAYED garbage (page-0 bytes at page-4 offsets) and - when the user edited
those dialogs and burned - WROTE that garbage into the main config, corrupting
it; the next boot then wedged (the recovery MFS erase + reflash brought the ECU
back, see the entry above).

Fix (CalibrationDialogWidget):
- renderField: fields with getPageIndex() != 0 render as a label-only row, no
  editor.
- Table/curve rendering (both update-by-key and panel-embedded): when the data
  array lives on a secondary page, a notice label is shown ("stored on
  TunerStudio page N - open it in TunerStudio") instead of TuningTableView/
  CurveWidget, so nothing is read from or written to the wrong image.
- Added isSecondaryPageField/ secondaryPageNotice helpers.

Tests: CalibrationDialogWidgetTest got testSecondaryPageTableRendersNoticeInsteadOfEditor
and testSecondaryPageFieldRendersLabelOnlyRow; :ui:test + :ecu_io:test green.
Console jar rebuilt (console/rusefi_console.jar).

Open follow-up: the console has no proper multi-page tuning UI; editing the
second tables remains TunerStudio-only until the tuning pane learns page-aware
images (read page 4 from the ECU, write + burn via the multi-page path).

## 2026-08-17 - console tuning UI now edits secondary TS pages (second VE/ignition tables)

Follow-up to the exclusion fix: the user needs the second tables editable in the
console itself, not just in TunerStudio. Implemented page-aware editing:

- CalibrationDialogWidget keeps per-page editable images (pageIdentifier ->
  ConfigurationImage) for secondary pages, loaded lazily from the ECU via
  readFromPage on the LinkManager thread (block-and-get keeps the render path
  synchronous). Fields/tables/curves whose ini field has getPageIndex() != 0
  render against that page's image instead of the page-0 working image; when
  the page cannot be read (no ECU/offline), the previous label-only/notice
  fallback is shown.
- Edits of secondary-page fields mark the page dirty (dirtySecondaryPages) and
  fire setOnSecondaryEdit; the toolbar's Burn to ECU now writes each dirty page
  via writeInBlocks + read-back verify + burnPage, then marks the pages clean.
- After load-tune the widget's cached secondary images are dropped
  (clearSecondaryImages) so the next render re-reads them from the ECU.

Tests: CalibrationDialogWidgetTest got testSecondaryPageTableRendersEditorWithEcuImage
(mocked BinaryProtocol/LinkManager supply a 1268-byte page-4 image; the editor
renders instead of the notice). Full :ui:test + :ecu_io:test green, console jar
rebuilt (console/rusefi_console.jar).

Open follow-ups:
- lambdaTable keeps showing up in the load-tune restore list even though the
  msq text round-trips byte-identically through the console's own parser
  (scale 1/147, Double.toString rendering). Page 0 is re-burned correctly each
  time; whether the ECU-side stored bytes really differ is still open - needs
  the user's lambda table editor values (or a third load) to triage.

## 2026-08-17 - burn button gated on real dirtiness + lambdaTable re-migration diagnostic

- The Burn to ECU button was enabled whenever the console was connected, and
  edits of secondary pages did not show up as "pending changes". Now the
  toolbar keeps a reference to the dialog widget; refreshState() counts
  dirtySecondaryPages as unsaved changes, shows the pending-changes label for
  them, and the burn button is enabled only when connected AND (page-0 or
  secondary) changes are pending.
- lambdaTable still appears in the restore list on every load even though the
  msq text round-trips byte-identically through the console parser and the
  console's editor shows the right values. Added a targeted diagnostic in
  DefaultTuneMigrator: on the next load the console log prints
  "lambdaTable restore diagnostic: first diff at N msq=X ecu=Y (msq cells A,
  ecu cells B)" - the first differing cell should reveal the direction of the
  discrepancy. To be removed once triaged.

## 2026-08-17 - lambdaTable phantom re-migration root cause + fix

Root cause: AfrLambdaTableMigrator (a specialized migrator that runs before
DefaultTuneMigrator) unconditionally re-added lambdaTable to the migration set
on every load - even when the converted values were byte-identical to the
target tune's current text. Because DefaultTuneMigrator skips already-migrated
fields, its value-equality check never ran, so the restore line (and the
page-0 write) appeared on every load. The values on the ECU were correct all
along (the console's editor showed them, and the msq text round-trips
byte-identically through the 1/147 grid).

Fix (AfrLambdaTableMigrator): after formatting the migrated values, compare the
text with the updated tune's current lambdaTable constant and skip the
migration when they are equal. DefaultTuneMigrator's temporary diagnostic was
removed.

Tests: AfrLambdaTableMigratorTest (identical table is not migrated, differing
table is). End-to-end re-check with the real 21129.msq + rusefi_m74_9.ini:
the second load of the same tune now produces ZERO migrations and zero pages
to write. Full :ui:test + :ecu_io:test green, console jar rebuilt
(console/rusefi_console.jar).

Note on the burn button: after a load-tune there are genuinely no pending
changes, so "Burn to ECU" stays disabled until an actual edit (page 0 or a
secondary page) - the phantom lambdaTable restore line used to make it look
like there were pending changes.

## 2026-08-17 - burn button never enabled: edit notification ordering fix

After the burn-button gating, the button stayed disabled even with pending
edits, while the exit prompt correctly detected unsaved changes. Root cause:
TuningPane's onConfigChange called toolbar.onEdit() BEFORE updating the
session image, so refreshState() (and the burn-button gating) always evaluated
the pre-edit state; nothing refreshed the button afterwards. The session image
is now updated first and onEdit() receives the previous image (undo baseline
semantics unchanged). Console jar rebuilt (console/rusefi_console.jar).

## 2026-08-17 - 21129 tune analysis conclusions + m74_9 software knock bring-up

- Created todo-21129.md with the full analysis of the 21129 tune: VE tables
  match the stock M74 modeled charge within +/-1% up to 4700 rpm; the
  secondVeTable 7000 rpm column holds the model's 6250 value so top-end
  interpolation runs 2-3% lean (correction values listed); ignition maps
  reproduce the stock calibration within +/-1.5 deg; lambda 0.84-0.82 is the
  right power line for RON95 with no knock control; intake flap crossover is
  ~4900 rpm; torque section is not used by the engine control.
- Software knock for m74_9: the board has knock inputs AA3 ("Knock sensor +")
  and AB3 (ground) in the connector yaml, but the MCU pin the onboard
  conditioner feeds is not documented. Assumed PF7 = ADC3 IN5 following the
  hellen precedent; bench verification procedure is in todo-21129.md
  (adc_report: ~1.65 V idle vs ~3.28 V open input, tap test).
- Enabled EFI_SOFTWARE_KNOCK=TRUE in board.mk and added a board knock_config.h
  (KNOCK_ADC = ADCD3, IN5, ADC_SAMPLE_480 -> Artery SPT 239.5 cycles ->
  ~286 kHz at the 72 MHz ADCCLK).
- Resolved the EFI_ADC3_SLOW vs EFI_SOFTWARE_KNOCK conflict (both own ADC3):
  removed the #error and made ADC3 shared. The slow ADC3 read now checks the
  driver state under osalSysLock and skips the cycle when a knock window is
  running (CLT/IAT keep their previous values); onStartKnockSampling checks
  and starts under osalSysLock so a slow conversion cannot slip in between;
  portInitAdc starts ADCD3 once.
- Board defaults (fresh configs only): enableSoftwareKnock = true,
  knockFrequency = 7000 Hz, knockDetectionUseDoubleFrequency = false (21129
  family: 82 mm bore, resonant sensors, no usable second harmonic).
- m74_9_clb_to_msq.py now patches enableSoftwareKnock / knockFrequency /
  knockDetectionUseDoubleFrequency in the msq. The user's 21129.msq was
  re-patched (backup 21129.pre-knock.msq): knock enabled, page 3 second-table
  block re-added (Save Tune keeps dropping it).
- Build: m74_9 firmware builds clean, knock symbols present in the elf
  (KnockThread::ThreadTask, knockCompletionCallback, startKnockSampling).

Open follow-ups:
- Verify the knock pin on the bench and calibrate knockBaseNoise/gains.
- Intake flap drive + second-table blend (gppwm on AA2), secondVeTable 7000 rpm
  column fix, lambda top-right corner - all in todo-21129.md.

## 2026-08-17 - m74_9 software knock: PA0 tracing + shared-ADC1 fix + ADC clock root cause

- Traced the knock input on the bench: AA3 ("Knock sensor +") feeds the
  onboard SGM321/LM321 ("3218K" marking) conditioner; its OUT pin is AC-coupled
  to PA0 = ADC1 IN0 (idles ~1.76 V). Earlier PF7 = ADC3 IN5 assumption was
  wrong - knock_config.h now uses ADCD1/IN0/Gpio::A0.
- ADC clock root cause: the Artery AT32F435 stores the ADC divider in
  ADCCOM.cctrl.adcdiv[19:16], ADCCLK = HCLK/16/adcdiv. The old mcuconf
  STM32_ADC_ADCPRE DIV6 mapped to a slow divider (~4.5 MHz ADCCLK); measured
  knock burst rate ~6.8 kHz with the ADC busy 100% (all ADCD1 states ACTIVE).
  Fixed to DIV4 -> adcdiv 1 -> 5.94 MHz (~33x faster). DIV2 cannot be used:
  the shared ADCv2 LLD range check computes 72 MHz > 36 MHz and fails compile.
  adcdivtest diagnostic confirms the divider bits experimentally.
- ADC1 is now shared between software knock and the slow sampling
  (EFI_SLOW_ADC == KNOCK_ADC == ADCD1). Implemented the mutual-exclusion
  contract in stm32_adc_v2.cpp + software_knock.cpp:
  - portInitAdc starts ADCD1 exactly once (runtime pointer compare; a second
    adcStart re-allocates the DMA stream and bricks the boot). The KNOCK_ADC
    start moved before adcSTM32EnableTSVREFE() (needs the ADC1 clock) and
    before the background chain start (needs state READY).
  - onStartKnockSampling steals the ADC when a slow background batch is in
    flight: adcStopConversionI (returns the driver to READY) +
    adcStartConversionI under osalSysLock. Boards with a dedicated knock ADC
    (proteus/f407/hellen = ADC3) keep the old skip-if-busy behavior
    (isKnockAdcSharedWithSlowAdc() runtime guard).
  - knockCompletionCallback resumes the slow background chain by re-running
    the aborted batch (slowAdcStartCurrentBatch refactor).
  - ADCD3 (EFI_ADC3_SLOW, CLT/IAT on F-port pins) is started unconditionally
    again - it was compiled out by the EFI_ADC3_SLOW && !EFI_SOFTWARE_KNOCK
    guard after knock moved off ADC3.
- Rebuilt deliver/rusefi.bin with the fix (previous build bricked boot via the
  double adcStart on ADCD1).
- User note: the previous flash bricks the ECU at boot (double ADC start);
  flash the new deliver/rusefi.bin, or if the link still does not come up use
  deliver/rusefi_recovery_erase_mfs.bin once and then rusefi.bin again.

Open follow-ups:
- Bench check after reflash: knocktest should show ~160 kHz real rate, ADCD1
  states no longer all-ACTIVE, and tapping AA3 should move PA0 only. Then
  calibrate knockBaseNoise/gains on the car.

## 2026-08-17 - m74_9 software knock: bench verification of the shared-ADC1 steal

- The rebuilt firmware (single adcStart for ADCD1) boots cleanly: slow ADC1
  channels live, ADC3 F-port channels (CLT/IAT) live, fast ADC (MAP) live.
- knocktest initially showed bursts=0 after the divider fix: the slow
  background chain now keeps ADCD1 ~100% ACTIVE (continuous by design) and the
  diagnostic only did check-and-start. Fixed: knockBurstSample now steals the
  ADC exactly like a real knock window (adcStopConversionI + adcStartConversionI
  under osalSysLock) and hands it back via slowAdcResumeAfterKnockWindowI,
  factored out of the ISR resume path. adcdivtest now saves/restores the real
  RCC->CFGR / ADC->CCR instead of hardcoded historical values.
- Bench results (user log, knocktest after reflash): 1024-sample burst
  6600 us -> real rate ~155 kHz (expected ~160 kHz, KNOCK_SAMPLE_RATE matches);
  bursts=200/200 on all channels; PA0 idle rms ~2180 counts (1.76 V) with
  p2p ~50 counts - the quiet conditioner bias, as expected.
- knockpin scan confirms PA0 = 1.758..1.762 V (spread 5) - the AA3 -> SGM321
  -> PA0 chain is healthy; PA1 reads Vrefint because the fast ADC (MAP) owns
  that channel in the slow chain.
- Remaining bench step: tap/click AA3 during knocktest - PA0 p2p must jump
  while PA1..PA3 stay flat. Then on-car: verify the steal fires with real
  spark events (watch knock windows in TS/logs) and calibrate knockBaseNoise
  and per-cylinder gains.

## 2026-08-17 - 21129.msq final tune fixes (flap work deferred by the user)

- Applied to 21129.msq (backup: 21129.msq.pre-7000fix.bak):
  - secondVeTable 7000 rpm column corrected: the converter had clamped the
    stock model's last rpm node (6250) into the 7000 bin, so interpolation
    ran 2-3% lean in the 6000-6250 zone. New column values extrapolate the
    model shape so the table hits the model at 6250 (values in
    todo-21129.md).
  - cylinderBore 87.5 -> 82.0 (21129 is 82 x 75.6 mm; only feeds the auto
    knock frequency, which is overridden by knockFrequency=7000 - correctness
    fix, no tuning effect).
  - lambdaTable top-right corner: the three 0.816 cells leveled to 0.823
    (the 1/147 storage-grid value of 0.82), keeping the console-exact text
    format so the table is not re-burned on every load.
- Intake flap + blend (gppwm on AA2, secondVe/secondIgnitionBlendParameter)
  deferred by the user; the plan stays in todo-21129.md.

## 2026-08-17 - 21129 cranking/idle review against the M74 reference

- Reference: chiptuner.ru M74.8 idle diagnostics (840+-40 rpm, УОЗ 9+-5 deg,
  MAP 0.38-0.41 bar, fill 115-125 mg/cycle, TPS 1-4%).
- Verified the cranking fuel path in fuel_math.cpp (getCrankingFuel3):
  crankingCycleBaseFuel is in mg, multiplied by crankingFuelCoef(CLT) and
  crankingTpsCoef(TPS). The tune's values (20 mg flat, coef 2.8..1.0, TPS 3.0
  at closed throttle - flood-clear direction) were cross-checked against
  observed pulse widths earlier; left unchanged.
- Checked the stock ignition maps (.clb) at the idle cell: the M74.9 base
  map really carries ~23-30 deg at 800-1000 rpm / 100-140 mg - in the stock
  ECU the idle governor retards to ~9-14 deg. The converted rusEFI table
  inherits this, so idle timing was too advanced.
- Applied to 21129.msq (backup 21129.msq.pre-idle-cranking.bak):
  - postCrankingFactor: 6x6 afterstart taper (2.2x at -20 C -> 1.0 over
    ~200 revolutions, 1.0 flat warm); duration bins fixed to 0..200 revs.
  - useSeparateAdvanceForIdle = enabled with idleAdvanceBins 0..2000 rpm and
    idleAdvance 16/15/13/12/11/10/10/12 deg (anti-stall slope; blends to the
    normal table between 2.5-5% pedal TPS via idlePidDeactivationTpsThreshold).
- Left for on-car work: idle PID gains, warm idle target 900 rpm (stock 840),
  idleTimingPid off (ETB idle uses the throttle).

## 2026-08-17 - 21129: injector flow corrected for the 3.8 bar rail

- Read injector_model.cpp: with injectorCompensationMode = "Fixed rail
  pressure" the fuel reference IS used without any sensor: differential =
  fuelReferencePressure + baro - MAP, flow x sqrt(differential/reference).
  So the tune's fuelReferencePressure = 51 (kPa) was not inert - at idle
  (MAP 38 kPa) it produced a sqrt(113/51) = 1.49x flow inflation, making the
  idle mixture ~18% lean; at WOT it was 1.0.
- The 215 cc/min figure is the standard 3 bar rating of the stock 21129
  injectors; at the stock 3.8 bar rail the real flow is 242 cc/min.
- Applied to 21129.msq (backup 21129.msq.pre-injector.bak):
  injector_flow 215 -> 242, fuelReferencePressure 51 -> 380 kPa.
- Expected mixture change vs the previous settings: WOT ~11% leaner (the old
  setup ran 12.6% rich - the systematic richness flagged earlier), idle ~22%
  richer (was lean from the 51 kPa bug), cranking ~7% leaner. The VE/lambda
  tables (stock model) stay as they are; if a wideband shows a systematic
  offset, injector_flow is the single knob to trim.
- Cranking 20 mg base is a fuel MASS, independent of rail pressure - the
  pulse width auto-adjusts through the (now correct) flow constant, so it
  stays valid for the 1.6 at 3.8 bar.

## 2026-08-17 - 21129: injector identified (Pekar 28346052), flow corrected again

- The user found the injector part number: Pekar 28346052 (OEM
  21127-1132010-00), the stock 21127/21129 injector. Official PEKAR spec:
  static flow 75.0 ml/30 s at 300 kPa = 150 cc/min at 3 bar, tolerance +-4%,
  coil 12 +- 0.5 Ohm; dynamic 8.0 ml/30 s at 2.5/20 ms.
- The previous 215 cc/min assumption was the Bosch 0280158237 figure, not
  this injector: with 215 + ref 51 kPa the engine actually ran ~21% lean at
  WOT and ~43% lean at idle (configured flow overstated, plus the 51 kPa
  reference bug inflating the idle flow ratio).
- Corrected 21129.msq (backup 21129.msq.pre-injector2.bak):
  injector_flow = 168.8 cc/min (150 x sqrt(3.8/3.0)), fuelReferencePressure
  = 380 kPa unchanged. The lambda table is now honored: WOT is a real
  0.84-0.82 instead of ~1.05.
- Peak duty at 6250 rpm / lambda 0.84 = ~83% - typical OEM sizing, no margin
  left; do not raise rpmHardLimit. STFT will re-learn at idle/part load.
- Cranking stays at 20 mg base: warm pulse ~28 ms (~2.7x stoich) - if warm
  starts feel rich, trim crankingTpsCoef down from 3.0.

## 2026-08-17 - "lambdaTable restored on every load" investigation

- The recurring "To restore previous calibrations ... lambdaTable" message on
  every msq load comes from the tune-migration framework
  (AfrLambdaTableMigrator / DefaultTuneMigrator): it fires when the file's
  table text differs from the text the console generates for the ECU's stored
  bytes. The burn itself re-writes the whole page, so the message only
  persists when the console's view of the ECU differs from the file.
- Verified the console's own round-trip byte-perfectly with a new unit test:
  java_console/ui .../Lambda21129RoundTripTest takes the exact 21129.msq
  lambdaTable text, writes it through ConfigurationImageGetterSetter.setValue2
  (the burn path) and reads it back through getStringValue (the ECU read
  path) - text is identical and neither migrator fires. The file's values are
  on the 1/147 grid and formatted console-exact (niceToString precision 3 =
  round(x*1000)/1000 + Double.toString), including the 0.823 corner cells.
- The console always treats lambdaTable as lambda/1/147 (IniField.parseDouble
  takes the true branch of the useLambdaOnInterface template, [tag:lambdaTable]);
  the AFR/Lambda interface flag only affects TS display - no data corruption.
- The remaining explanation for the user's persistent message is a
  console/firmware version mismatch: the user ran console 20260816 while the
  ECU runs firmware built 2026-08-17 (signature
  rusEFI master.2026.08.17.m74_9.727755639). The config changed between the
  builds (immo fields at offset 15768 - after lambdaTable, so offsets before
  8946 look stable, but any earlier-layout drift produces exactly this
  symptom). Fix: run the console from the freshly built bundle
  firmware/rusefi.snapshot.m74_9/console/rusefi_console.jar (20260817) and
  verify the ECU signature on connect.

## 2026-08-17 - lambda restore loop resolved (user confirmation)

- The user updated to the matching console (20260817 from the freshly built
  bundle) and the tune now writes cleanly - the recurring lambdaTable restore
  message is gone. Confirms the root cause was the console/firmware version
  mismatch (console 20260816 vs firmware 2026.08.17), not the msq content.
- All 21129.msq fixes are now on the ECU: injector flow 168.8 cc/min @ 3.8 bar
  (Pekar 28346052), fuelReferencePressure 380 kPa, afterstart taper, separate
  idle advance table, lambda corner cleanup, secondVeTable 7000 column,
  cylinderBore 82, knock on PA0.
- Next: first start on the car - watch idle (900 rpm, УОЗ 10-14 deg, MAP
  38-41 kPa, STFT +-10%), then knock calibration logs.

## 2026-08-17 - ECU reboot during cranking: assert->rebootNow mechanism + knockpin hang fixed

- Root cause of the bench hang: adc3SlowConvert was simplified to a plain
  adcConvert() on ADCD3, which asserts ("already waiting" / "not ready") when
  the knockpin diagnostic (console thread) collides with the 500 Hz slow-loop
  conversion of the same driver. chDbgPanic3 -> criticalError + (non-main
  thread) rebootNow() -> "Reset Cause: Unknown" with RTC preserved - exactly
  the in-car symptom. Fixed: adc3SlowConvert is back to the atomic
  check-and-start under osalSysLock, skipping the cycle when ADC3 is busy
  (commit aca1c3af113).
- Key mechanism recorded for future debugging: ANY assert in a non-main
  thread silently reboots the ECU (chDbgPanic3 in error_handling.cpp calls
  criticalError("assert fail ...") then rebootNow()); the message usually
  reaches the console but can race the reboot. "Reset Cause: Unknown" +
  preserved RTC = software reset (NVIC_SystemReset sets no CSR flag).
- Rebuilt deliver/rusefi.bin (19:01). Next: re-test knockpin/knocktest on
  the bench, then re-try the car start with the console connected - if it
  still crashes during cranking, the "assert fail" line will identify the
  location.

## 2026-08-18: macOS PCAN console TX root cause - 64-bit DWORD in the JNI bridge (DLC=0)

Symptom (m74_9 bench + PCAN-USB on Apple Silicon): console over MacCAN received
the ECU's flood (~3800 fps) but the ECU never answered the 0x710 ISO-TP hello.
MacCAN trace showed every TX frame as `Tx 0710 0` - DLC=0 - so the ECU's isotp
receiveFrame early-return dropped them.

Investigation path (all static + runtime analysis of libPCBUSB 0.13 arm64):
- A/B tested MacCAN 0.10.1..0.13 (Universal_64) with a dlopen harness - all
  identical (0.9/0.11 fail ILLHW on this macOS, unrelated). Version hunt was a
  dead end; the driver was healthy.
- Disassembled pcan_usb_can_write: the device-type halfword (channel table
  entry +0x6, table base = __DATA+0x1E7E0, stride 0x6025c8) was 0x000C
  (classic PCAN-USB) at runtime -> the LEGACY encoder was selected, and it
  encodes DLC at byte 2 correctly. So the bug was not the FD-encoder theory.
- The clue: the trace prints msg->LEN from offset +5 and the ID from +0; ID was
  right (0x710) but LEN read as 0. Both offsets come from the same struct, so
  the dylib's TPCANMsg (4-byte DWORD) and OUR struct disagreed - our
  `typedef unsigned long DWORD;` is 8 bytes on LP64 macOS, shifting LEN to +9.
  The dylib read +5 = the high byte of our 8-byte ID = 0 -> DLC=0 on the wire.
  RX "worked" by luck (SetIntField truncates the 64-bit ID read to the real
  low 32 bits).

Fix (one line): `typedef unsigned int DWORD;` in
java_console/PCANBasic_JNI_macos.c (and the C test harnesses). Rebuilt
java_console/libpcanbasic_jni.dylib (arm64).

Validation:
- C harness (java_console/pcan_mac_test.c): ECU answers 0x710 with
  0x720 `10 30 00 2A 00 72 75 73` ("rus..."); trace now shows
  `Tx 0710 8 07 00 01 53 20 60 EF C3`.
- Java end-to-end (java_console/PcanMacHello.java, real peak.can.basic classes
  + the dylib): same 0x720 response through the exact stack the console uses
  (PCanHelper.send -> PCANBasic.Write -> JNI bridge).

Bundling: firmware/bundle.mk adds ../java_console/libpcanbasic_jni.dylib to
CONSOLE_FOLDER_SOURCES on Darwin; misc/console_launcher/rusefi_updater.sh now
runs java with -Djava.library.path=. so the bundle console finds the dylib
next to rusefi_console.jar.

Follow-ups:
- The arm64-only dylib must be rebuilt (cc -dynamiclib with JDK headers) if the
  JNI bridge changes, and a universal build is needed for Intel Macs.
- The ECU-side crash during cranking (silent reset, RTC re-sync) is still open;
  the PCAN link fix removes the console-connectivity noise from that picture.

## 2026-08-18 (2): local commits + cleanup

Committed the pending bench-session work on maccan-tx-fix (all local, not pushed):
- c84d82f5fa0 m74_9: real AT32 reset-cause decoding from RCC->CSR
- a87e11fe8bc core: re-print crash report + reset cause every 5 s for the first minute
- f6054d6963f isotp: FC counter snapshot BEFORE the FIRST frame + m74_9 DL_OUTPUT_BUFFER 16384
- 75b1f06fdde console: PCAN detection via the driver on macOS + local_proxy pcan/socketcan/tcp
Validation: unit_tests testCanSerial - 6/6 pass; m74_9 bundle build includes the dylib.
Deleted java_console/PCBUSB-Library (MacCAN binary archive, 14 MB) - the bridge
dlopens /opt/homebrew/lib or /usr/local/lib/libPCBUSB.0.dylib, the clone was
only used for the version A/B comparison and is ignored by git anyway.

## 2026-08-18 (3): bench logs - NRST reset + L9779 OUT_DIS latch; console CPU fix

User bench logs:
- Spontaneous reset with the new decoder: "Reset from NRST pin", BKP0R=0 (no
  firmware crash). NRST on m74_9 goes MCU -> 10-pin DAP/JTAG connector; the
  KiCad dump shows no supervisor and unpopulated R* placeholders nearby -
  a floating-ish reset line (MCU ~40k pullup) is EMI/ESD-sensitive and fits
  the rare bench resets. To discriminate: run without console, scope NRST,
  or add 100nF + 10k at NRST. Car crash may be the same mechanism - next
  car test will show the cause via the decoder.
- L9779: frame_err=1 in 1.7M frames (single glitch) and OUT_DIS=1 with all
  DIA10 flags clean. DIA reads clear the fault flags, so the clean flags
  are expected after the event; OUT_DIS stays latched until START. Added
  instrumentation: log the latch transition with the raw flags + self-heal
  a stale latch with START (commit on maccan-tx-fix).
- TLE9201 diag drift: TLE9201 sits on SPI2, separate from the L9779 SPI1 -
  not a bus corruption; the transitions are real chip state (bridge
  enable/coast). Benign unless it correlates with etb errors.

Java console CPU: the PCAN reader thread busy-spun on MacCAN's non-blocking
Read (returns QRCVEMPTY immediately; Windows PCANBasic blocks). 100% of one
core in jstack at PCANBasic.Read. Fixed PCanIoStream.readOnePacket to sleep
1 ms on QRCVEMPTY; console jar rebuilt. User restarts the console to pick
it up.

## 2026-08-18 (4): console thread-limit storms fixed; uptime command

- User: java console still lost the ECU link and burned CPU on macOS. The
  CPU part was already fixed (busy-spin on MacCAN's non-blocking Read, now
  ~9% for UI rendering); the link drop was a SECOND issue: the serial port
  scanner's per-port probe threads (each up to a minute, 3 attempts) piled
  up on the macOS Bluetooth/wlan debug UARTs until the JVM hit the native
  thread limit ~1 min after start - "OutOfMemoryError: unable to create
  native thread" storms in the console log at 00:48:28 and 00:49:48.
  Fixes: -Dserial.port.scan=false skips the scan (PCAN setups don't need
  it), -Xss512k in the bundle launcher doubles the thread budget.
- Firmware: new 'uptime' console command (getTimeNowS-based) for correlating
  bench events with resets.

## 2026-08-18 (5): console reconnect over PCAN failed forever after a link drop

Morning bench: console started WITHOUT the adapter (ILLHW storm), then the
adapter was plugged in mid-session - MacCAN never recovered (zombie claim),
console restart + replug fixed the init but ~20 s later a transient ISO-TP
desync ("dropPending: Unexpected pending data: 1 byte(s)") fired the
ConnectionWatchdog, and every reconnect then failed with
PCAN_ERROR_INITIALIZE: PCanIoStream never uninitialized the channel, so the
same process held the single-client MacCAN device forever.

Fix: PCanIoStream.close() now calls can.Uninitialize. Console jar rebuilt;
user restarts the console (adapter plugged in FIRST). The desync itself
(the watchdog trigger) is still to be characterized on the next run.

## 2026-08-18 (6): overnight bench - console dies every 10 s, root cause BCM flood vs ISO-TP TX

Morning: console connects but every session dies in ~10 s cycles - "Got only
309 byte(s) while expecting 1029", no ECU confirmations, isotp rate ~1 fps
(yesterday: 1379 fps). The unpaused m74_9 BCM emulation floods ~3 kfps and
contends with the calibration response for the CAN TX mailboxes; the
response trickles, the console times out, reconnects, and because
pauseCANdueToSerialUntil was only extended on RX the flood never quiets -
death spiral. Fix: extend the pause on every serial TX frame (isotp.cpp).
Console-side reconnect already self-heals (previous commit); user flashes
the new firmware.

## 2026-08-18 (7): reset causes explained - user flashes via ST-Link + OpenOCD

The "mysterious" bench resets are the flash procedure: the user flashes with
`openocd -f stlink-dap.cfg ... -c "program rusefi.bin 0x08000000 verify reset exit"`.
- "Reset Cause: NVIC_SystemReset or by debugger" = OpenOCD's `reset` (SYSRESETREQ
  via the debug probe) after each flash - expected, not a bug.
- "Reset from NRST pin" = the ST-Link asserts the NRST line (its reset pin is
  wired to the DAP connector's NRST) on connect/disconnect/probe events.
Recommendation: unplug the ST-Link after flashing while working with the console.
Post-flash state is healthy: isotp rate back to ~1379 fps (the BCM-flood pause
fix works), console polls normally.

## 2026-08-18 (8): assert line=220 mystery solved - every ChibiOS halt stores chsys.c:220; knock crash = SV#4 chSysLock-from-IRQ

The crash marker kept reporting "assert (line=220, ...)" during start attempts
with software knock enabled, and nothing with knock disabled. Two discoveries:

1. The stored line is ALWAYS 220 and is meaningless: ChibiOS 7 chDbgAssert
   expands to chSysHalt(reason), and CH_CFG_SYSTEM_HALT_HOOK(reason) in
   chconf_common.h expands chDbgPanic3(reason, __FILE__, __LINE__) at its
   INVOCATION site - chsys.c:220 (verified: __LINE__ in a macro body resolves
   at the invocation, not the definition). So any assert/halt (chmtx "not
   owner", hal_adc "not ready", SV#1..SV#11 system-state checks, chDbgCheck)
   reports line 220. The panic MESSAGE is the only identifier.
2. The real crash: onStartKnockSampling runs in ISR context (angle-scheduled
   spark event, executed by the TIM5 scheduling-timer callback, which does
   OSAL_IRQ_PROLOGUE -> isr_cnt=1). It called the thread-only osalSysLock()
   (chSysLock), whose __dbg_check_lock halts with "SV#4" when isr_cnt != 0.
   First knock window after sync = guaranteed reboot; with
   enableSoftwareKnock=false the function returns before locking = no crash.
   This matches every symptom: crash exactly at first catch with knock on,
   bench OK (no sparks = no windows), RTC time preserved across the reboot.

Fixes:
- software_knock.cpp onStartKnockSampling: osalSysLock()/osalSysUnlock() ->
  chibios_rt::CriticalSectionLocker (X-class chSysGetStatusAndLockX, legal in
  thread AND ISR contexts, no-ops when the executor already holds the lock -
  same pattern as SingleTimerExecutor).
- error_handling.cpp: the assert crash marker now packs the panic message
  (BKP2R..BKP5R, 16 bytes) and file (BKP6R..BKP8R, 12 bytes) and prints them
  on boot, so a future crash identifies itself ("SV#4" vs "not owner" etc.)
  without a connected console. Line stays for compatibility.

Validation: m74_9 firmware build OK, unit tests build+Knock suite pass.
Next: user re-flashes, re-enables software knock, retries start.

## 2026-08-18 (9): start-attempt log analysis - wave chart decoded, displacement fixed

Decoded the wave_chart dump from the controlled start attempt (knock off):
- engine sniffer x unit is 10 us (ENGINE_SNIFFER_UNIT_US); clean 60-2 signal,
  gap ratio 3.02, RPM ~325 -> 281 decelerating, coils fire in wasted pairs
  (c1+c4, c3+c2), dwell ~6.7 ms (overcharged - matches the C935x warnings),
  injectors fire every ~106 ms (~180 deg, simultaneous mode) with ~21.4 ms
  pulses (~44 mg per squirt at 168.8 cc/min) - 4-5x the mass-based cranking
  math prediction (20 mg x CLT coef / 4 squirts ~= 3-5 ms). The tune's
  TPS "MS Adder" accel table (up to 32 ms) adds fuel when the pedal is
  pumped during cranking but does not explain a steady 21 ms. To be resolved
  from live channels (crankingFuel, injectionDuration, CLT) at the next
  connected start attempt.
- The trigger CSVs are useless for now: the teeth capture only records
  timeout/overflow rows (0 events), no real edges.
- displacement was 2.0 L on a 1596 cc engine: +25% SD airmass -> +25% running
  fuel right at the crank->run transition. Fixed to 1.6 in 21129.msq.
- Knock disabled + engine not staying running: no sync errors in these
  sessions (one C9002 got 58/0 in the 10:53 attempt), coils/injectors fire
  correctly, so the no-start is fueling/air, not trigger.

## 2026-08-18 (10): catch -> stall in 160 ms decoded from 14:03 MLG; idle PID + taper + ignition table fixes

Decoded the 14:03:47 MLG (32 records, ~30 ms cadence with ~90 ms gaps =
console-side record loss over PCAN). Timeline of the catch:

- rec15 t=1004: cranking 293 rpm, cranking advance ~11 deg, MAP 90
- rec16 t=1092: catch 511 rpm (instant 701 - flare), MAP 68, adv 15.0 (main
  table), ETB target 2.1%, dwelloverchargecounter already growing
- rec17 t=1116: 632 rpm, MAP 41.1, adv 22.7 (main table, load 40), fuel
  8.37 mg, ETB target 1.2%, TPS actual 0.62% - healthy-looking idle state
- rec18 t=1197: SAG 453 rpm, MAP 60.7, adv 14.6, ETB target 4.13%, TPS
  actual 6.78% (overshoot), idle closed-loop PID output 19.96 (p-term 24.1),
  dwell actual ratio 174% (sparks firing late), overcharge counter 10
- rec19 t=1274: rpm 0. Death spiral total ~160 ms.

Root cause chain:
- The crank-to-idle taper (afterCrankingIACtaperDuration) completes almost
  immediately after the catch (looksLikeCrankToIdle bit flips 1->0 within
  24 ms in the MLG, isIdleClosedLoop goes 1 at rec17) - closed-loop idle
  engages while the engine is still in the post-catch flare.
- The idle PID with p=0.05 sees ~400-500 rpm error (target 1042) and slams
  the throttle: target 1.2% -> 4.13%, actual 0.62% -> 6.78% in 80 ms.
- MAP jumps 41 -> 61, the main ignition table collapses advance 22.7 -> 14.6
  deg exactly when torque is needed (load 55-60 / rpm 650-800 cells were
  14.4-15.8 deg), the angle-scheduled sparks arrive late (dwell ratio 174%,
  C935x overcharge), and the engine dies.
- Knock is NOT involved: m_knockRetard=0 everywhere, levels -58..-68 dBv.
- STFT is disabled (fuelClosedLoopCorrectionEnabled=disabled) and in any
  case gated by startupDelay + minClt 60 C, so the stuck lambda reading
  (1.56, sensor 226905987R / 14Point7 input reads constant 6.48 V) does not
  affect the start. Wiring/calibration of the WB input is a separate task.

Tune fixes in 21129.msq (user applies via TS, mirror in Downloads):
- idleRpmPid: p 0.05 -> 0.02, i 0.002 -> 0.001 (the 24% p-term throttle slam)
- afterCrankingIACtaperDuration 10 -> 30 cycles (keep CL idle out of the
  post-catch flare longer)
- ignitionTable loads 50-75 @ rpm 650-800 raised from 14.4-18.3 to
  16.0-19.0 deg - smooths the 22.7 -> 14.6 advance collapse when MAP rises
  during the sag; load 45 (18.1 deg) and above left untouched

Firmware rebuilt (deliver/rusefi.bin 14:48) - the ECU still runs the 09:24
build which predates the 12:02 knock-ISR fix (chSysLock from IRQ -> SV#4),
so re-flash rusefi.bin together with the msq.

Open items:
- MLG field list order differs from the firmware data_logs.ini order (MLG
  header starts with oilpressure, ini with sd_present) - the console packs
  fields in its own map order so values are self-consistent, but a few
  fields read implausible values (m_knockThreshold=100 with a -20 dB curve,
  trgSynchronizationCounter jumping 1->8->18->24 in 88 ms). Needs a
  cross-check once the new firmware is flashed; do not over-interpret those
  fields until then.
- The stall analysis assumes the 09:24 firmware; re-verify with the 14:48
  build (Compiled: timestamp must change).
- Lambda input stuck at AFR 22.9 / lambda 1.561 - check the 14Point7
  controller wiring to "Oxygen sensor 1 signal" and the curve.

## 2026-08-18 (11): user's open-loop tune runs longest - idle advance table was the killer

The user hand-tuned 21129_new.msq (open-loop idle only, ETB idle range 2%,
hot idle positions 0.5%, knock disabled, taper 10, cranking timing 10 deg,
lambda input disconnected) - the engine catches and flares to ~595 rpm with
MAP 40.9 kPa and 22.4 deg advance, the longest run so far.

The 14:48:21 MLG shows the exact death: at t=818 the engine is healthy
(595 rpm, MAP 40.9, adv 22.42 from the main table). At t=896 the separate
idle-advance table engages (phase -> Idling) and, with the user's values
(-5/0/5/8 deg at 0-700 rpm), retards the timing from 22.42 to 0.38 deg at
422 rpm -> instant stall. A 22 deg retard at 400 rpm cannot be survived.

Fixes applied to 21129_new.msq (now the canonical 21129.msq in the repo,
synced to ~/21129_new.msq and ~/Downloads/21129.msq):
- idleAdvance [-5,0,5,8,11,10,10,12] -> [20,21,22,20,18,16,16,16] - the
  idle table now matches the main table's low-load region (~22 deg), so the
  engagement retards only ~1-2 deg.
- ignitionTable loads 50-75 @ rpm 650-800 raised to 16.0-19.0 deg (the
  user's export predated this edit; it smooths the advance collapse if MAP
  rises during a sag).
Everything else kept from the user's tune (Open Loop idle, ETB range 2,
knock off, taper 10).

Note: the ECU is still on the 09:24 firmware (no knock-ISR fix); the
14:48 build in deliver/ has it. Knock is disabled in the tune so the crash
cannot trigger, but re-flash anyway for the crash-marker improvements.

## 2026-08-18 (12): best run so far still stalls - C9002 gap rejection + cranking-mode flip at 450 rpm

The 15:00 MLG (user's open-loop tune + fixed idleAdvance): the engine
catches, flares to 742 rpm with MAP 34.6 and 21.6 deg, then decays
742 -> 457 -> 353 -> 0 in ~200 ms. Observations:

- The throttle blade sits at the mechanical stop (~0.95-1.2% TPS) the whole
  time - all idle targets (cranking 0.43%, idle 0.011% with ETB range 2%)
  are below the stop, so the taper completing instantly does not change the
  actual airflow. Air is not the variable in this stall.
- 14:58 and 14:59 runs logged C9002 "too many teeth between sync points:
  expected 58/0 got 58/0" - the count is right, the gap ratio check failed.
  triggerSyncGapRatio reads 1.352 during the decay, outside the gap1 window
  [0.85, 1.15] - the REAL missing-teeth gap is rejected during hard
  deceleration (misfire/rich wobble stretches the gap ratio).
- When rpm falls below cranking_rpm=450 during the decay, the control flips
  back to cranking mode: advance 21 -> ~10-13 deg (cranking curve), fuel
  -> 28 mg (rich) - an 8-11 deg retard + fuel jump exactly when torque is
  needed most.
- Mixture is rich through the run: SD math with the VE table gives lambda
  ~0.7 (injected 1.4x base with CLT 1.09 + afterstart ~1.28).

Tune changes (21129.msq, synced to ~/21129_new.msq and ~/Downloads):
- cranking_rpm 450 -> 300: the decay stays in running mode down to 300 rpm,
  no mode flip with cranking advance/fuel in the 350-450 zone.
- crankingFuelCoef warm end trimmed (1.3/1.1/1.0/1.0 -> 1.2/1.0/0.9/0.85
  at 20/35/50/65 C): the catch is strong (flare to 742), less wall fuel.
- triggerGapOverrideTo2 (gap1 upper) 1.15 -> 1.35: accept the real gap
  distorted by deceleration instead of rejecting it (C9002). Risk of false
  sync per the 60-2 window notes, but sync rejection kills the engine
  deterministically while a false sync is recoverable.

Still to do: flash the 14:48 firmware (ECU is on 09:24), re-enable the
lambda input eventually (mixture tuning is blind without it), and get a
full MLG captured from before the key-on to the stall.

## 2026-08-18 (13): idle fueling uses the main VE table; tip-in stall = hot TPS accel table

User question: which VE table fuels idle, and why does pressing the gas
stall the engine. Answers:

- Idle uses the MAIN veTable (useSeparateVeForIdle=disabled, the idleVeTable
  is all zeros and unused) with the MAP load axis (veOverrideMode=MAP). At
  idle load ~35 the VE is 61-68% -> base fuel 7-8 mg -> lambda ~0.7 during
  the afterstart phase (CLT 1.09 x afterstart ~1.28) settling to ~0.92
  warm. The idle-region VE is stock-plausible; the rich first seconds are
  the afterstart, not a table error.
- Tip-in stall: the TPS accel "MS Adder" table added 10-37 ms of pulse on a
  ~5 ms idle base (2-7x fuel) once the 40%/cycle TPS threshold was crossed
  -> instant rich bog -> stall. Moderate presses below 40%/cycle got NO
  adder and relied on the lagging MAP-averaged SD fuel -> lean tip-in.

Fixes in 21129.msq:
- tpsTpsAccelTable scaled down ~3x (10-37 -> 4-16 ms).
- tpsAccelEnrichmentThreshold 40 -> 15 %/cycle so moderate presses also
  get a small enrichment instead of a lean spike.

## 2026-08-18 (14): enable the separate idle VE table, seeded from the main VE table

Per user request - make idle fueling tunable in its own 4x4 table:
- useSeparateVeForIdle disabled -> enabled
- idleVeOverrideMode None -> MAP (the idle table's load axis is raw MAP
  kPa, same as the main table)
- idleVeLoadBins 30/40/50/60 kPa, idleVeRpmBins 600/800/1000/1200
- idleVeTable seeded from the main veTable idle region (load 30-60,
  rpm 600-1200): rows [57,67,68,68],[63,69,71,71],[65,69,72,73],[67,71,72,74]
The resulting idle VE at MAP 38/rpm 700 is ~65, matching the main table's
~64 - no fueling step at the switchover. The table engages only when
isIdlingOrTaper() and blends to the main table above
idlePidDeactivationTpsThreshold (5% pedal).

## 2026-08-18 (15): shape the cranking fuel cycle table

Cranking fuel in rusEFI is mass-based (useRunningMathForCranking=Fixed):
crankingCycleBaseFuel(CLT, cycle) x crankingFuelCoef(CLT) x
crankingTpsCoef(pedal) - this is the "separate cranking table" the user
asked for; it already exists as a 4x8 (CLT 0-60 C x cycle 1-8) table.
Shaped it: 24/24/22/22/20/20/19/19 mg over the 8 cycles (all CLT rows -
the CLT dependence lives in crankingFuelCoef). Compensates the warm-end
coef trim from f527b0d6302: first catch stays rich (~26.6 mg at 29 C),
steady cranking tapers to ~21 mg.

## 2026-08-18 (16): cranking fuel table extended to -40..+60 C (config layout change!)

Per user request the crankingCycleBaseFuel table now has CLT rows from
-40 to +60 C instead of 0..60:
- rusefi_config.txt: CRANKING_CYCLE_CLT_SIZE 4 -> 6
- crankingCycleFuelCltBins: -40/-20/0/20/40/60
- crankingCycleBaseFuel shaped by temp (mg, cols = cycles 1-8):
  -40C: 30 30 28 28 26 26 25 25
  -20C: 27 27 25 25 23 23 22 22
    0C: 25 25 23 23 21 21 20 20
   20C: 23 23 21 21 19 19 18 18
   40C: 21 21 19 19 17 17 16 16
   60C: 19 19 17 17 15 15 14 14
  At 29 C this gives ~22.1 mg on cycle 1 (x coef 1.11 = ~24.5 mg) -
  close to the previous flat-table value, the working point barely moves.

IMPORTANT: this changes the persistent config layout (struct grew by
2 rows = 68 bytes) -> the TS signature changed (319381849 vs 727755639)
and the config stored in MFS will be INVALID after flashing the new
firmware. The user MUST re-apply 21129.msq via TunerStudio right after
flashing. Firmware rebuilt: deliver/rusefi.bin 15:35.

## 2026-08-18 (17): console msq validation failed on the 6-row cranking table

The user loaded the new msq with the console from the bundle and got
"crankingCycleBaseFuel: 48 values while expecting 4 by 8 total 32" - the
console validates msq tables against the ini, and both the console jar and
the ini in the bundle were stale (old CRANKING_CYCLE_CLT_SIZE=4 layout).
Fixed:
- UiVersion.CONSOLE_VERSION 20260817 -> 20260818
- console jar rebuilt (console/rusefi_console.jar 15:45) - VariableRegistry
  now carries CRANKING_CYCLE_CLT_SIZE=6
- bundle zip refreshed with the new jar + new ini + firmware bin/hex.
  NOTE: the snapshot bin/hex were dangling symlinks (relative paths) so the
  compile.sh zip step silently skipped the firmware files in both bundle
  builds; replaced with real copies and re-zipped.
The msq load error also appears while the ECU still runs the old firmware:
the console reads the ini from the ECU, so the new msq can only be loaded
AFTER flashing the 15:35 firmware. Correct order: flash rusefi.bin ->
connect console -> load 21129_new.msq -> apply.

## 2026-08-18 (18): msq signature was stale - patched to the new firmware signature

The "expecting 4 by 8" error persisted after flashing because the msq
still carried the OLD signature (2026.08.17.m74_9.727755639). The console
resolves the ini by the msq signature (RealIniFileProvider: bundled ini ->
current folder -> download -> manual picker), so it fetched the OLD ini
with the 4-row layout and rejected the 6-row table. Patched the versionInfo
in 21129.msq to the new signature (2026.08.18.m74_9.319381849) and
firmwareInfo 20260818. Now the console matches the msq against the new ini
from the bundle/ECU and accepts the 6x8 table.

## 2026-08-18 (19): crankingFuelCoef units mismatch fixed in msq

After the signature fix the msq loaded, but the console skipped
crankingFuelCoef: the msq had units="coef" while the firmware ini declares
the field with units "ratio" (rusefi_config.txt line 2147). The console
refuses to apply fields whose units string differs. Patched the msq:
crankingFuelCoef units coef -> ratio. Values unchanged. Re-load and burn
again so the curve actually lands in the ECU (the previous burn left it at
the firmware default).

## 2026-08-18 (20): idle features round - timing PID, taper tables, return ramp, delayed CL engagement

User request: enable the idle stabilisation features.

Firmware (idle_thread.cpp):
- idlePidActivationTime was declared in the config but never read anywhere -
  wired it up: getClosedLoop() holds off the closed-loop PID until the
  engine has been in the Idling phase for idlePidActivationTime seconds
  (0 = legacy behaviour, no delay). Also reset m_timeInIdlePhase in init().
- Unit test idle_v2.closedLoopActivationDelay added (hold-off -> soft
  re-entry ramp -> full output; legacy path with 0 delay). All 20 idle_v2
  tests pass.

Tune (21129.msq):
- idleMode Open Loop -> Open Loop + Closed Loop (the user's 20 s delay
  request implies closed loop)
- idlePidActivationTime 0 -> 20 s - no throttle PID during the first 20 s
  of idle, the engine settles in open loop first
- idleRpmPid p 0.05 -> 0.02, i 0.002 -> 0.001 (gentle gains - the old
  p=0.05 slammed the throttle)
- useIdleTimingPidControl no -> yes (fast ignition-timing PID, p=0.1,
  +-10 deg) - catches RPM dips much faster than the throttle can
- useSeparateIdleTablesForCrankingTaper disabled -> enabled (idle VE and
  idle advance tables now apply during the post-crank taper too)
- idleReturnTargetRamp no -> yes (target RPM ramps down from +500 over 3 s
  when returning to idle - no step change when releasing the pedal)

modeledFlowIdle NOT enabled yet: it needs the throttle flow curve
(idleFlowEstimatePosition % vs idleFlowEstimateFlow kg/h, 8 points over
the idle range) plus idleMaximumAirmass (mg/cyl at 100% idle position).
The user will try to extract the flow characteristic from the stock ECU
firmware; we will convert it into the rusEFI curve and enable the feature
then. Without real data the guess would risk the idle again.

Firmware rebuilt (16:22), bundle refreshed with the new bin/hex/ini/jar.

## 2026-08-18 (21): stock M74 start tables - the missing "vzhuh" (start flare)

The user extracted the stock M74 ECU calibration (.clb files):
- "Положение д.заслонки на пуске": start throttle 18.5% @ -40C down to
  ~5-6% @ 100C (X axis = relative cranking speed 0.6/0.8/1.0 of exit rpm,
  Z = CLT -40..110). At 30C the stock opens the throttle to 8.8% during
  cranking - our tune had 0.5% (25% position x 2% range). That is why the
  stock flares to ~1500 rpm ("вжух") and rusEFI only reached 742.
- "Обороты выхода из пуска": 1300 rpm @ -40C down to 850 rpm @ 30C+.
  Our cranking_rpm was 300 - the start phase ended 3x too early.
- "Относительная/Обратная характеристика расхода д.заслонки": throttle
  flow characteristic, 0-100% relative. With the user's anchors (4 kg/h
  closed, 1350 kg/h full capacity) this gives 9.0 kg/h at 1% throttle -
  matches our measured ~8.5 kg/h at the stop.

Tune changes (21129.msq):
- etbIdleThrottleRange 2 -> 15 (rusEFI default): idle position % maps to
  real throttle % correctly, and the PID +-20 authority is +-3% throttle
- cltCrankingCorr = stock start positions / 0.15: [124, 84, 67, 65, 55,
  48, 41, 38] at CLT -40..100 -> 18.5%..5.7% throttle
- cltIdleCorrTable = [53, 33, 20, 13, 11, 10, 10, 10] -> 8%..1.5% throttle
  (stock idle is 1-3%)
- cranking_rpm 300 -> 850 (stock warm exit rpm) - the start phase now
  lasts through the flare, no mode flip at 300-450 rpm
- afterCrankingIACtaperDuration 10 -> 40 cycles (the throttle now steps
  from ~9% to ~1.7%, give the taper more room)
- idleFlowEstimateFlow/Position filled from the stock throttle curve
  ([6.57, 9.0, 11.67, 14.62, 17.99, 25.42, 34.8, 73.45] kg/h at [0, 6.5,
  13.5, 20, 26.5, 40, 53.5, 100]% position) + idleMaximumAirmass 190 mg.
  modeledFlowIdle stays OFF: the firmware's linear position->airmass model
  (idleMaximumAirmass caps at 500 mg) cannot represent the throttle's
  stop-leak nonlinearity (would need ~1300 mg) - enabling it would collapse
  the open-loop position. The tables are seeded for a future firmware
  change that lifts the cap.

The stock start-advance table (-5.25 deg below 280 rpm, 14.6 deg at
680-1080) was NOT copied: our catch works, negative advance is only
anti-kickback.

## 2026-08-18 (22): stock idle advance 9+-5 deg + ignition retard authority -30 deg

From the chiptuner M74 reference (https://chiptuner.ru/content/typ-m74-0/):
stock warm idle: 840+-40 rpm, advance 9+-5 deg, throttle 1-4%, MAP
0.38-0.41 bar, air flow 7-12 kg/h, injection 3.0-5.0 ms - our measured
MAP/flow already match; the advance did not (we ran 20-22 deg).

The user's stock parameter "минимальный УОЗ на выходе из пуска = -30 deg"
is the retard authority of the ignition-based idle control: the stock can
pull the advance down to -30 deg to tame the post-start flare.

Tune changes:
- idleAdvance 20-22 -> 10-14 deg (stock-shaped: 10@0, 12@400, 14@700,
  12@900, 10@1100, 9@1300-1600, 10@2000)
- idleTimingPid_minValue -10 -> -30 (stock retard authority; p=0.1 means
  -30 deg only at ~300 rpm error, normal idle corrections stay ~+-5 deg)
The transition cranking (14.5 deg at 850) -> taper idle table (~13 deg)
-> idle (~10-12 deg) is now smooth and matches the stock start advance
table (14.6 deg at 680-1080, 11.25 at 1200).

## 2026-08-18 (23): tune-load restore loop - board was force-reverting crankingFuelCoef

The user reported the same "To restore previous calibrations..." lines on
every tune load (cltCrankingCorr, crankingFuelCoef, vvtControlMinRpm).
Root cause for crankingFuelCoef: m74_9_boardConfigOverrides() in
board_configuration.cpp forced the default curve {2.8,2.2,1.8,1.55,1.3,
1.1,1.0,1.0} on EVERY boot (leftover from before cold-start tuning), so
the deliberately tuned 21129.msq curve {2.8,2.2,1.8,1.5,1.2,1.0,0.9,0.85}
was silently reverted at each boot and the console re-restored it on each
load - a genuine loop. Removed the force (the tune owns the curve now).
Also fixed the msq value formatting for cltCrankingCorr (digits 0 -> 2,
values 124.00 etc.) and crankingFuelCoef (2.80 etc.): the console compares
value STRINGS, so digit mismatches made the migration fire even when the
values were equal (cosmetic loop). vvtControlMinRpm was a one-off restore
after the layout change - resolves once the burn persists.
Firmware rebuilt (16:54), bundle refreshed. Re-flash rusefi.bin, re-apply
the msq once - the restore lines must not repeat on the next load.

## 2026-08-18 (24): restore-loop string format - verified via round-trip harness

The restore lines kept repeating because the console compares value STRINGS:
the msq carried "124.00"/"2.80" while the console generates "124.0"/"2.8"
(trailing-zero-stripped) from the page image - so the comparison never
matched even with equal values. Verified with a temporary round-trip test
through CalibrationsHelper.mergeCalibrationsWithPartialFailure: after
rewriting the msq strings to "124.0"-style / TS-style "2.8", the second
load of the same tune produces ZERO restore lines (harness deleted).
The remaining diffs (byFirmwareVersion, ignitionKeyDigitalPin) are
ignored by the migrators.

IMPORTANT: the ECU still runs the 16:22 firmware (Compiled: 16:22:32 in
the 16:55 log) - the crankingFuelCoef board force is still active there.
The restore loop for crankingFuelCoef cannot stop until the 16:54
firmware (force removed) is flashed. Order: flash 16:54 -> burn the
updated msq -> reboot -> load again - no restore lines expected.

## 2026-08-18 (25): cranking uses the tables now - VE fuel map + cranking advance table

The user noticed that during cranking neither the fuel table nor the
ignition table was active (cranking ran on the fixed mass tables and the
fixed timing angle). Switched to the table-driven modes:

- useRunningMathForCranking Fixed -> Fuel Map: cranking fuel now comes
  from the normal speed-density math (main VE table, MAP load). At
  cranking MAP ~88 kPa / VE ~72% this gives ~20 mg x crankingFuelCoef -
  the same ballpark as the fixed tables, but baro/IAT-corrected and
  VE-tunable. NOTE: in Fuel Map mode crankingCycleBaseFuel (the 6x8 table
  built earlier) is bypassed - it stays in the tune for the Fixed mode.
- useSeparateAdvanceForCranking "Fixed (auto taper)" -> "Table": the
  crankingAdvance curve is now used directly. Filled with the stock
  "УОЗ при пуске" shape: -5.25 deg at 0-280 rpm (anti-kickback),
  10.9 at 560, 14.6 at 850 - hands off smoothly to the idle advance table
  (~13 deg) at the cranking_rpm=850 transition.

## 2026-08-18 (26): cold cranking flood - cut crankingFuelCoef and afterstart taper

The engine now catches and briefly reaches idle (one log record: rpm 955,
MAP 72.8 kPa) but floods. Parsed the 17:17/17:18 MLGs with the local
parser (artifacts/tmp_mlg_parse): during cranking at CLT 17 C the chain
was base 27.3 mg (SD math, airmass 360 mg / target AFR 13.2 - correct)
x crankingFuelCoef 1.56 = 42.7 mg, x wall-fuel ~1.06 = ~45 mg injected
per cylinder. The SD base already includes the target-lambda and airmass
correction, so crankingFuelCoef was pure extra enrichment (1.56 at 17 C).
After the catch, running fuel was base 17.9 x coolant 1.14 x postCranking
1.44 = 28.7 mg at 955 rpm - the afterstart taper peak (2.2x row) was also
drowning it.

Changes to 21129.msq (committed a59c9ae8835, synced to ~/21129_new.msq):
- crankingFuelCoef -20/.../90: 2.8 2.2 1.8 1.5 1.2 1.0 0.9 0.85 ->
  1.6 1.4 1.15 1.0 0.9 0.8 0.72 0.65. At 17 C the multiplier drops
  1.56 -> ~1.03, cranking dose 45 -> ~29 mg. Warm bins now lean the
  dose below the SD base (0.65 at 90 C = ~18 mg warm start).
- postCrankingFactor rows re-scaled: peak at -20 C 2.2 -> 1.5, the 0 C
  row 1.7 -> 1.32, the 20 C row 1.4 -> 1.2, decay to 1.0 within
  120-200 revolutions unchanged. At 17 C the afterstart start value is
  now ~1.22 instead of ~1.44.

Next: load ~/21129_new.msq in the console, cold-start, send the next
text+MLG pair. If still rich: next levers are the 5-20 C coef bins and
the 0/20 C postCranking rows; the VE table stays untouched (SD math
verified correct).

## 2026-08-18 (27): warm start - backfire on throttle, catches at 650 rpm and dies

Parsed the 17:44 MLGs (engine warm, CLT 95 C). Two findings:

1. The ECU is STILL on the 16:22 firmware (Compiled: 16:22:32 in the
   log) - the every-boot crankingFuelCoef force is active, so the warm
   cranking multiplier is the old 0.85 (log shows cClt=0.85 at 95 C)
   instead of the new 0.65. deliver/rusefi.bin (16:54) must be flashed.

2. Warm cranking with the pedal blip: user pressed ~24% pedal during
   cranking (ETB 5.8 -> 29%, TPS 16.3%), cranking fuel stayed ~21 mg -
   lean pop into the intake (crankingTpsCoef was flat 1.0). Also the
   cranking advance ran ~3 deg below the table: useAdvanceCorrectionsForCranking
   was yes, and the ignitionIatCorrTable pulls -1..-3 deg at 100-140 kPa
   loads - at 650 rpm the actual advance was ~9 deg (stock M74 gives
   ~14 deg there), so the engine could not climb past the 850 rpm
   cranking exit and died when the starter released. Coil overcharge
   warnings (C9351-54, ~8.2 ms) appeared right before engine stopped.

Changes to 21129.msq (committed 2ad1320077e, synced to ~/21129_new.msq):
- crankingAdvance: -5.25/-5.25/10.9/14.6 -> -5.25/-3.75/12.0/14.6
  (stock mid-curve shape).
- useAdvanceCorrectionsForCranking yes -> no: cranking now runs the
  pure table, no IAT/high-load pull - ~+3 deg at 650 rpm.
- crankingTpsCoef: flat 1.0 -> 1.0/1.12/1.25/1.3/1.0/0.5/0/0 (TPS
  enrichment up to 43% then flood-clear cut at WOT).
- primeValues warm bins: 50/45/40 -> 30/15/10 mg at 60/80/100 C (the
  key-on 41 mg prime at 95 C was wetting a warm engine).

Driver note: do not touch the pedal while cranking - the ETB opens the
throttle itself (cltCrankingCorr). The TPS enrichment above is only a
safety net. Order of operations: flash 16:54 -> load msq -> warm start
without touching the gas.

## 2026-08-18 (28): trigger false syncs - the real source of the intake backfires

The 18:03-18:07 logs show C9003 "not enough teeth: got 12/14/16" plus
"newerr TRG gap=1.7-2.0 expected 1.6-4.2" events. Decoded the sequence:

- A stretched tooth pair mid-revolution (gap0 1.70-1.97, gap1 ~1.26)
  passes BOTH ratio windows and false-syncs the decoder 12-16 teeth
  before the real gap. The first sync has no tooth count to validate
  against, so the decoder accepts it and fires spark/injection at the
  wrong phase for ~1/4 revolution - the intake backfire the user sees.
- The real gap (gap0 3.91, gap1 1.33) arrives 12-16 teeth later, the
  count check fails (expected 58 got 12-16) -> C9003 -> sync lost, then
  re-acquisition. The C9002 58/58 case is the same mechanism in the
  other direction: during a combustion kick the real gap ratio leaves
  the [1.6, 4.2] window at the expected position -> gap missed -> sync
  lost. The ratio windows cannot be tuned out of this: the false pairs
  (1.7-2.0) and the first-combustion real gap (<2.25, the reason the low
  side is 1.6) overlap.

Fixes (commit 0aac4db56e1, firmware 18:44 build, all 1131 unit tests pass):

- trigger_decoder: once synchronized, the sync point is evaluated ONLY at
  the expected gap position (eventCount == expected, same condition as
  the noise filter's isGapExpected; primary-gap wheels only, cam-pattern
  syncs keep the ratio-only check). Mid-rev stretched pairs are now just
  long teeth - a running engine no longer gets kicked out of sync by
  them. test_trigger_decoder_2 updated to the new semantics.
- trigger_central + new board override custom_board_requireValidatedSync
  (m74_9 enables it): ignition/injection stay off for the first crank
  revolution after a fresh sync, until the next gap arrives at the
  expected tooth count and validates the position. Cost: first spark
  comes ~0.2-0.3 s later at cranking rpm - same as a stock ECU that
  validates sync before firing.

To do on car: flash firmware/deliver/rusefi.bin (18:44). Expected:
occasional C9002/C9003 warnings may still appear during rough cranking
(they now mean "gap not validated", no wrong-phase outputs), but no more
intake backfires from trigger false syncs. If C9002 58/58 persists after
the catch, revisit the gap0 low side vs the first-combustion compression.

## 2026-08-18 (29): tooth profile learning + 'toothdump' - oscillogram without TunerStudio

The user has no TunerStudio, so a board-local console command replaces
the TS Tooth Logger. New m74_9_tooth_diag.cpp (commit 66d0d8ca3be,
firmware 18:59 build):

- boardTriggerCallback() override (the weak default in trigger_central)
  captures every synchronized primary tooth timestamp into a 6-rev ring,
  indexed by tooth position 0..57 via the decoded engine phase.
- Learning: per-tooth EMA period profile (alpha 0.05) - the stock-ECU
  'wheel non-uniformity' learning. Each tooth's time share of the
  revolution differs consistently (VR threshold offsets + compression
  ripple); tooth 57 carries the missing-teeth gap interval.
- 'toothdump' command prints rpm, the learned profile in us and
  normalized to the regular-tooth mean, plus the last 4 raw revolutions
  as period lists. Output is chunked at 12 values per efiPrintf line
  (256-char log line cap). The user pastes it back and we plot.

Planned consumers once the profile is captured: gap validation and
instant-rpm should use the learned per-tooth factors instead of raw time
ratios (that is what makes the stock decoder robust on this wheel).

## 2026-08-18 (30): tooth profile persisted in MFS + crank signal path review

User questions answered:
1. Two-wire crank sensor without shield - yes, distortion is plausible.
   The KiCad netlist shows the VR path: CRANK VR+/VR- -> RC passives ->
   74HC14 Schmitt trigger -> PF8. Fixed thresholds (~1/3, 2/3 VCC), no
   adaptive threshold like the stock M74 conditioner, high-impedance
   input with no shield on the harness - noise pickup and low-rpm
   threshold jitter are both possible. The toothdump capture will show
   which one: smooth periodic thick/thin pattern = compression physics,
   random single-tooth anomalies = noise/threshold.
2. Persistent auto-learning implemented (commit 7b5329eee18, firmware
   19:11 build): new EFI_TOOTH_PROFILE_RECORD_ID=6 with weak
   toothProfileStorageRead/Write hooks in storage.cpp dispatched from the
   storage manager thread (serialized with settings writes, no MFS
   race). The m74_9 module stores the EMA profile with magic/version/
   crc32, loads it at boot and continues learning from it, auto-saves on
   engine stop (>= 5 learned revolutions), manual save via 'toothsave'.
   'toothdump' now shows stored=yes/no and minRev.

## 2026-08-18 (31): profile-normalized gap check - digital adaptive VR

The user asked for the stock ECU's adaptive VR conditioning. The m74_9
input is a passive RC + fixed-threshold 74HC14 - there is nothing to
adjust in analog from firmware. The digital equivalent is implemented
instead (commit 7b5329eee18's follow-up): isSyncPoint now normalizes
each tooth duration by the learned per-tooth profile factor
(triggerGetToothProfileFactor, weak default 1.0, strong m74_9 override
from the EMA profile, clamped 0.5-1.5; gap tooth keeps 1.0). Multiply
form keeps the ISR division-free. Effect: the systematic compression
ripple is divided out, so the real gap ratio stays ~3.0 under
combustion - directly fixing the C9002 58/58 window-exit case. False
transient pairs stay ~2.0; once the toothdump data confirms the
normalized distributions, gap0 can be tightened to ~[2.2, 3.9] to
reject them outright. All 1131 unit tests pass.

## 2026-08-18 (32): toothdump stack overflow crash - fixed

First on-car toothdump hard-faulted the ECU (BKP0R=0xC0FFEE01, fault
type=3, cfsr=0x04 IMPRECISERR, pc in idle/sleep code - the imprecise
fault surfaced after the console command thread blew its stack). The
command built ~3.5 KB of locals (ordered[348] + two float[58] + a
224-byte line buffer) on the console thread stack. All moved to static
storage (commit, firmware 20:33 build).

On-car positives from the same session: the cam sensor WORKS - VVT input
reports VVT_SINGLE_TOOTH with 5/5 rising/falling edges during cranking
(the AG2 -> PB9 path is live). The C9003 at 20:29 (gap=1.735 mid-rev,
17 teeth before the real gap) shows the false-sync acquisition path is
still there until the learned profile loads and the gap window gets
tightened - expected, next step after the toothdump data.

## 2026-08-18 (33): first toothdump data - clean VR signal, indexing fix, gap window tightened

First real capture (20:48, 60 revs at ~290 rpm): the tooth profile is a
smooth 4-cycle compression ripple, NO random spikes - the VR signal is
healthy, the false syncs are pure decoder-window artifacts. The gap
reads 36.4 ms = 3.1x its neighbors.

Two bugs found in the capture itself:
- The phase-derived tooth index was shifted (the engine phase wraps
  around tdcPosition: the sync tooth mapped to ~39 instead of 0 - the
  gap landed at slot 37 and slots 38/39 never learned). Fixed by using
  the decoder index (sync = 0); the ring boundaries now land on sync
  events and the gap sits at slot 57 as designed. Profile version
  bumped to 2 so the misindexed stored record is rejected.
- Gap0 window tightened 1.6-4.2 -> 2.2-3.9: observed raw gap ratio is
  3.05-3.11 (known cranking stretch to 3.6-3.75, both inside), observed
  false pairs 1.7-2.0 (now outside). With the stored profile loaded the
  normalized gap holds ~3 under combustion kicks.

Also: the 20:34 'stack overflt/sr' marker was the same toothdump stack
overflow caught by the ChibiOS stack check (still on the 19:49 build);
the 20:47:52 marker was a plain power cycle (BKP0R=0, not a crash).

## 2026-08-18 (34): LTO silently killed the weak board hooks - profile capture/persistence/normalization were dead code

The 19:49 firmware never printed any 'tooth:' messages even after a
clean toothdump: the strong m74_9 definitions of boardTriggerCallback,
toothProfileStorageRead/Write and triggerGetToothProfileFactor were
dead-code-eliminated by GCC LTO. Root cause, confirmed in the map file:
GCC binds a call to a weak definition that lives in the SAME translation
unit as the call site (dead-call elimination / const folding - the
noinline attribute does not prevent this) BEFORE the linker can select
a board's strong override. The storage dispatch folded to 'return true'
and handleShaftSignal dropped the capture callback entirely.

The triggerGetToothProfileFactor hook survived only by accident: its
header declaration has no 'weak' attribute, so the call sites saw a
plain declaration and emitted a real relocation.

Fix: the weak defaults moved to separate translation units -
firmware/controllers/trigger/trigger_board_hooks.cpp and
firmware/controllers/storage_weaks.cpp; call sites see plain non-weak
declarations. Verified in the new m74_9 build (disassembly):
- storageWriteID id=6 now does the real profile write (TOOT magic,
  crc32, storageWrite 248 bytes)
- handleShaftSignal runs the capture code (ring/profile arrays live)
- triggerGetToothProfileFactor is the strong 58-tooth body

Rule for this codebase: a weak board hook must never share a
translation unit with its call site, and its declaration at call sites
must not carry 'weak'.

unit_tests: 1131/1131 pass. Committed da385042cb3.

## 2026-08-18 (35): position gate was disabled for 60-2 - false sync at 402 rpm killed the first start attempt

First start attempt with the persisted profile (22:11): the engine
caught, reached 402 rpm, then C9003 'expected 58/0 got 42/0' and died.
The mid-rev stretched tooth pair grows with RPM: 1.7-2.0 at ~300 rpm,
but 2.476/1.284 at 402 rpm - INSIDE the [2.2, 3.9] gap0 window, so no
ratio window can reject it anymore.

Root cause: the position gate (commit 0aac4db56e1) was keyed on
useOnlyPrimaryForSync, which initializeSkippedToothTrigger leaves false
for 60-2 (the cam/VVT events must reach the decoder) - the gate was
silently bypassed exactly on the wheel it was built for.

Fix: the gate is now wheel-aware and asymmetric for every gap-synced
wheel. A real gap can only arrive LATE (noise inserts spurious teeth),
never early, so candidates before expectedEventCount - 2 are false
pairs and are ignored - the decoder keeps counting, the real gap
re-syncs cleanly, no desync of a running engine. Candidates at/after
the expected position are accepted and the too-many/few-teeth counters
report mismatches as before. Verified against all observed false-sync
positions on the car (12/17/42/48/51 teeth - all early, all rejected).

Tune: triggerGapOverrideFrom1 2.2 -> 1.6 - the false pairs are now
rejected by position, and the low side again covers the first-combustion
gap compression (the 60-2 cranking-transition window rationale).

Tests: new crankingTransition60_2FalseSyncAtRunningRpmRejectedByPositionGate
uses the exact on-car 2.476/1.284 pair. The gate changes the error-path
distribution on noisy real-data recordings (Renix 44-2-2 counter 84->115,
GM 24x finder 27->29, noisy 36-2 dwell bails 25/22->29/26) - pins updated
with comments, functional assertions (sync recovery, RPM, no overdwell)
unchanged. 1132/1132 pass. Committed 506d8504610.

## 2026-08-18 (36): cam-phase cross-check + cranking-band sync-by-position skip

Two more layers on top of the position gate, both opt-in for m74_9 via
custom_board_* overrides:

1. Cam-phase drift cross-check (custom_board_vvtDriftLimit, m74_9 = 15
   deg): a fixed cam must report the same VVT phase every cam
   revolution. A crank-sync basis error shifts the reading by the sync
   error - a jump beyond the limit warns (CUSTOM_VVT_PHASE_JUMP = 6728,
   was the unused CUSTOM_ERR_6728) and desyncs the crank decoder, which
   re-syncs cleanly on the next real gap. VVT-phaser engines stay
   disabled (0 = off).

2. Cranking-band sync-by-position skip
   (custom_board_syncByPositionWhileCranking, m74_9 = true): at the
   first-combustion catch the REAL missing-teeth gap can compress below
   the ratio window; rejecting it at the exact gap position costs a
   full revolution and fires C9002 right when the engine caught. At the
   exact expected position (event count == expected) the sync is now
   accepted regardless of ratio while rpm < 2 * crankingRpm. Risk: a
   single noise-inserted/missed tooth can make this accept a regular
   pair one slot off the real gap - a silent 6-degree phase error that
   self-corrects within one revolution (the next real gap syncs at
   count expected-1 with a C9003 mismatch). Harmless at cranking speed,
   the skip is off once running.

Both were first implemented globally and reverted to board opt-in after
the full test suite showed the global behavior change rippled across
~20 unrelated wheel tests (4/1, VQ40, Coyote, NB2, Neon, K20, noisy
36-2) - the m74_9 bring-up aids stay board-scoped.

Debugging note: the mock trigger decoder tests run with
engineConfiguration = nullptr; any new engineConfiguration deref in
decodeTriggerEvent must be lazy and guarded by #if EFI_UNIT_TEST (the
firmware build has -Werror=address - engineConfiguration is a reference
there).

Tests: cam drift (stable phase no-fire, 30-deg jump warns + desyncs,
re-syncs); compressed 1.4x gap accepted while cranking, rejected above
2 * crankingRpm; stretched 4.2x kick accepted while cranking. 1138/1138
pass. Committed 4714f5071c5 + dbcc6162c3c.

## 2026-08-19 - m74_9: ETB autocal overcurrent trip + TPS zero offset

On-car session (Lada 21129): with cranking_rpm 500 and etbIdleThrottleRange
2% the engine catches cleanly (sync in ~1 revolution, no trigger errors,
MAP drops 97.9 -> 75.3 kPa at 260 rpm with the throttle at its stop).

Two findings:

1. The TPS zero is offset from the mechanical stop: during the autocal
   grab the plate parks at -0.4% on the current scale, i.e. the true
   closed position is -0.4% and a 1.0% reading is physically 1.4% open.
   With tpsMin=4.5 V / tpsMax=0.37 V (span 4.13 V) the stop sits at
   ~4.517 V. The grab could not fix this because it never completed.

2. The grab aborts on TLE9201 overcurrent shutdown (diag 0xCF, CL bit):
   the autocal Close phase holds -50% duty against the closed stop for
   the full 1 s; the stalled motor trips the chip's protection.

Fix (7b36dccd370): the autocal Close phase now detects the stop from the
frozen primary TPS reading (5 mV per 500 Hz loop, 50 ms minimum) and
captures immediately instead of holding duty into the stop - no stall,
no fault. The 1 s timeout remains as a fallback for slow throttles, so
slow boards keep the old behavior exactly.

Build: m74_9 firmware OK. Unit tests 1138/1138 pass.

Follow-ups:
- Re-run the ETB grab on the car: it should now complete and set
  tpsMin/tps1SecondaryMin so the stop reads 0% (both sensors).
- Manual alternative without reflashing: tpsMin 4.50 -> 4.517 V and
  tps1SecondaryMin 4.51 -> 4.517 V in TS; then targets near 0% command
  the plate truly closed (0.4% more closure for every target).
- C6899 "Invalid MAP at 3.52" is a one-time boot artifact (fast ADC
  starts in initAdcInputs before initMap attaches the MAP converter;
  first sample rejected with m_function == nullptr) and is replayed by
  the console on every USB reconnect - not a recurring fault. Cosmetic
  fix candidate: skip the warning until the converter is attached.

## 2026-08-19 - m74_9: OpenBLT bootloader over CAN

Goal: flash firmware from the car without JTAG/SWD. Implemented OpenBLT
(32 KB @ 0x08000000) with XCP-over-CAN transport (CAN1 500 kbps, XCP
msg IDs 0x667 TX / 0x7E1 RX), matching the standard rusEFI OpenBLT setup
(java_console/bin/blt_can.bat uses the same IDs).

Firmware changes (commit 118e23d081b):
- AT32F435ZMxx.ld got the STM32F4-style bootloader layout: bl region,
  flash_start = 32 KB offset under HAS_BOOTLOADER, 16-byte OpenBLT
  shared RAM params (.bl/.shared sections). Guard pages are off on AT32,
  so the F7-style 32-byte alignment requirement does not apply here.
- bootloader/Makefile INCDIR: BOARDINC now precedes ALLINC - with the
  old order config/stm32f4ems/efifeatures.h (EFI_USB_SERIAL=TRUE) won
  over the board's own header and pulled the STM32 USB LLD into the
  AT32 build ("USB driver activated but no USB peripheral assigned").
- openblt_usb.cpp guarded by EFI_USB_SERIAL; m74_9 disables the RS232
  transport (BOOT_COM_RS232_ENABLE=0 - the rusEFI RS232 layer is
  USB-CDC-only) and sets the CAN pins: PG0 RX / PG1 TX, channel 0,
  backdoor timeout 1000 ms.
- The board's runtime C++ sources are excluded from the bootloader
  (board_configuration/m74_9_can/m74_9_tooth_diag/board_storage drag in
  sensors+engine+MFS); a minimal board_bootloader.cpp provides boardInit
  and the LED pins. board.c (__early_init, GPIO/clock) stays.
- New hw_layer/ports/at32/at32f4/flash_int.cpp: intFlash*/flashSizeKb on
  the ChibiOS EFL driver (EFLD1 bank 1, 4 KB uniform sectors, async
  erase via start/query) - the STM32 register-based flash_int.cpp does
  not exist on AT32; the EFL driver is the same one MFS settings storage
  uses on this chip.
- App side: jump_to_openblt() in at32_common.cpp now compiles (extern "C"
  shared_params.h include, same pattern as stm32_common.cpp); canOpenBLT
  = yes in 21129.msq - the running app watches the OpenBLT RX ID and
  jumps to the bootloader when the host starts an XCP session, so
  flashing from the car needs no console at all.

Host tooling: BootCommander/libopenblt builds with NO_CAN_OR_USB on
macOS, so the CAN host tool needs Windows (PCANBasic + BootCommander,
blt_can.bat) or a Mac CAN flasher (the console already links MacCAN for
mcp_can - a Java XCP-over-CAN flasher is the natural follow-up).

Build fixes along the way: hex2dfu submodule bumped to the -b (binary
output) commit and hex2dfu.mac rebuilt (the old binary predates -b and
bundle.mk uses it); DEVICE_BIN_FOLDER in bundle.mk is .FORCE now - its
parent is rm -rf'd by the .FORCE BIN_FOLDER rule on every bundle run,
which broke the openblt bundle symlink on re-runs.

Verified: deliver/rusefi.bin = bootloader + app merged by hex2dfu
(SP/reset vectors at 0x0 and 0x8000, app CRC + size at 0x801C,
'BL09' stamped at 0x24, BLT_CURRENT_VERSION 0x39304C42 matches the
stamping script). Bootloader itself: 13.3 KB flash / 32 KB region.
Unit tests 1138/1138 pass.

On-car procedure: flash deliver/rusefi.bin once via ST-Link (do NOT
flash app-only images with openocd afterwards - a full erase kills the
bootloader); burn the msq (canOpenBLT=yes); then updates via
BootCommander over PCAN with rusefi_..._update.srec. Risk to verify on
the car: the CAN transceiver must stay powered/active while the
bootloader runs (no enable pin in the tune - likely always-on).

---

## 2026-08-19 - m74_9: Java XCP-over-CAN flasher (openblt_can) + app-side jump trigger fix

The Java console "OpenBLT via CAN" menu item cannot flash from macOS: the
bundled libopenblt.dylib has only the NET transport compiled (no CAN/USB),
and the JNI wrapper (misc/libopenblt_jni/openblt_jni.cpp) initializes the
USB transport in its CAN setup anyway. Verified with nm on the shipped
libraries. So instead of fixing the native path, implemented a pure-Java
XCP 1.0 client over the already-working MacCAN/PCANBasic stack.

What was done:

| Change | File |
| --- | --- |
| New Gradle module: XCP client, SREC parser, CLI flasher, PcanLink | `java_console/openblt_can/` |
| Gradle project registration | `settings.gradle` |
| CLI wrapper (builds fat jar on demand) | `java_console/bin/openblt_can.sh` |
| Fixed app-side OpenBLT trigger: extended-ID frames never matched | `firmware/controllers/can/can_rx.cpp` |

Protocol facts pinned down from the target sources (ext/openblt/Target/
Source/xcp.c + bootloader/openblt_chibios/openblt_can.cpp):
- One XCP packet = one CAN frame, max 8 bytes; no multi-frame counter
  byte. PROGRAM_MAX carries 7 bytes, PROGRAM carries 1..6, both
  auto-increment the MTA.
- SET_MTA/PROGRAM_CLEAR/BUILD_CHECKSUM read data[4..7], so DLC=8 is
  required for those frames. Addresses/lengths little-endian.
- CONNECT (0xFF) is the only command accepted before a session; the
  bootloader answers 8 bytes with max CTO = 8.
- Bootloader entry: BackDoorEntryHook always true + the 1000 ms backdoor
  window, or the app-side jump (canOpenBLT) via shared params. The
  flasher's CONNECT frame is DLC=2, exactly the app-side trigger, so one
  retry loop covers both entry paths.
- BUILD_CHECKSUM is an 8-bit additive sum (ADD11) - used for post-program
  verification per segment.
- The bootloader rejects erase/write below the app base (32 KB offset);
  the flasher also refuses such images client-side.

App-side trigger bug: can_rx.cpp compared CAN_SID(frame) (11 bits) to
BOOT_COM_CAN_RX_MSG_ID = 0x80010667 - impossible to match for both
standard and extended frames, so the canOpenBLT jump never fired on the
extended-ID m74_9 build. Fixed by normalizing the frame id the same way
the config constant encodes it (EID | 0x80000000 for extended frames).

Validation:
- openblt_can unit tests 25/25: SREC parser (S1/S3, checksums,
  segmentation), XCP wire format (DLC=8 little-endian frames, error
  packets, foreign-frame filtering, timeouts), end-to-end flash against
  an in-memory bootloader simulation (erase chunking, PROGRAM_MAX +
  PROGRAM tail, checksum verify catches corruption, late-bootloader
  retries, bootloader-area protection, no-reset mode).
- m74_9 firmware rebuilt with the can_rx.cpp fix: deliver/rusefi.bin
  updated (BL09 marker intact).

On-car procedure now: build the jar (`./gradlew :openblt_can:fatJar`),
then `java_console/bin/openblt_can.sh firmware/build/rusefi.srec` with
the PCAN adapter on the bench. First `--probe` to confirm the bootloader
answers, then a full flash. Still requires the one-time ST-Link flash of
the new deliver/rusefi.bin (contains the trigger fix).

Open follow-ups:
- Console integration: wire a menu item that runs this flasher instead of
  the broken OpenbltJni CAN path.
- Optional faster transport: the 7-bytes-per-frame PROGRAM_MAX gives
  ~700 KB image in roughly 1.5-2 min at 500 kbps; acceptable but could be
  improved with a pipelined mode (target forbids it via ctoPending, so it
  would need a bootloader change).

## 2026-08-19 - m74_9: EFI_USE_OPENBLT was FALSE - CAN trigger dead + probe failure root cause

The first on-car --probe got no bootloader response. Root cause was NOT the
Java flasher: the m74_9 app build had EFI_USE_OPENBLT = FALSE, so the whole
app-side OpenBLT machinery was compiled out - the canOpenBLT CAN trigger in
can_rx.cpp, jump_to_openblt's body (at32_common.cpp), the reboot_openblt
console action and show_blt_version. Only stm32f4ems/efifeatures.h defines
the flag (default FALSE); no board overrides it. USE_OPENBLT=yes in
meta-info.env only adds shared_params.c to the build - it does NOT set the
C++ define. So the trigger fix landed earlier today was dead code, and the
only bootloader entry left was the 1 s post-reset backdoor window (the
probe ran 8 s without a power cycle -> silence).

Fixed by defining EFI_USE_OPENBLT TRUE in boards/m74_9/efifeatures.h BEFORE
the stm32f4ems include (that header guards its FALSE default with #ifndef).

Diagnosis lessons:
- Do not verify preprocessor state with 'strings' on the ELF: the DWARF
  debug sections contain source text and match string literals that were
  compiled out. Verify with objdump -s -j .rodata or by disassembling
  references (constant 0x80010667 in the trigger comparison).
- LTO object files (.o) are GIMPLE bitcode: objdump/strings on them do not
  show the final codegen; check the linked ELF instead.
- efifeatures.h changes do not rebuild objects: the .o dependency is on
  pch/pch.h, not on headers included by the pch. touch pch/pch.h (or make
  clean) after any efifeatures.h edit.

Also added --verbose to the flasher (logs every TX/RX frame - tells "no
bus traffic at all" (wiring) from "traffic but no bootloader" (ECU state)),
and the connect-failure message now explains the power-cycle backdoor.

Verified in the 14:00 build: .rodata contains show_blt_version, the binary
contains the 0x80010667 trigger comparison constant, jump_to_openblt is
linked and called, deliver/rusefi.bin re-merged with BL09 marker.

On-car next step: flash the new deliver/rusefi.bin once via ST-Link, then
--probe must answer without any power cycling (the CAN trigger works now).

## 2026-08-19 - m74_9: OpenBLT-over-CAN fully working on the car bench (end-to-end)

The CAN flasher now works end to end on the bench: full 663 KB image
flashed over CAN, checksum-verified, ECU rebooted into it, and the
bootloader passes the app through on plain power-on (boot-time CRC ok).

What was actually broken (three stacked bugs, all found with on-hardware
register dumps via openocd):

| Bug | Evidence | Fix |
| --- | --- | --- |
| Java flasher used 0x667/0x7E1, but the configured ids are 0x10667/0x107E1 (efi_blt_ids.h, blt_can.bat agrees) | bootloader's debug echo showed it received our frames as id 0x0667 and its beacons went out on 0x107E1 | XcpConstants TX/RX ids corrected |
| Bootloader never programmed CAN filters: bxCAN reset state has all filter banks inactive (FA1R=0) - nothing is received at all (the app does this in can_hw.cpp; the bootloader never did) | FIFO stayed empty; after the fix the CONNECT frames land in FIFO0 | canSTM32SetFilters(default accept-all) before canStart in openblt_can.cpp CanInit |
| AT32 flash rejects a second program to a word that is not fully erased (EPPERR on the 2nd 7-byte PROGRAM_MAX frame - the EFL driver pads each word with 0xFF, re-programming the previous word) | PROGRAM at 0x08008007 failed with XCP 0x31 (generic) consistently | buffered writes in openblt_flash.cpp (one program per 4-byte word, like the H7 ECC path) + PROGRAM size=0 finalize in the flasher to flush the tail |

Also fixed along the way:
- EFI_USE_OPENBLT is FALSE by default (stm32f4ems efifeatures) and nothing
  overrode it for m74_9: the canOpenBLT trigger, jump_to_openblt body and
  reboot_openblt were all compiled out. Defined TRUE in the board
  efifeatures.h before the stm32f4ems include.
- canOpenBLT is forced true in the board ConfigOverrides (like the CAN
  pins) so a stale/unsynced msq cannot silently disable CAN flashing.
- can_rx.cpp trigger id comparison now normalizes extended frames
  (EID | 0x80000000) - the old SID comparison never matched.
- Java flasher: --verbose frame logging, PROGRAM(size=0) finalize,
  fixed failedAt accounting in the error message, power-cycle backdoor
  hint in the connect-timeout message.

Hardware verification flow used (all with the ST-Link still attached):
- halt-in-window pc sampling showed the bootloader runs its 1 s backdoor
  (app starts at ~1000 ms).
- CAN register dumps (MCR/MSR/TSR/ESR/BTR/FMR/FA1R/RF0R) + AFR/MODER
  dumps proved the bootloader CAN setup matches the app exactly.
- A temporary boot beacon + echo (OPENBLT_DEBUG_ECHO, since removed)
  exposed the id mismatch: the echo of our CONNECT arrived as
  "EE 02 67 06" - the bootloader saw id 0x0667 on the wire while its
  beacons went out on 0x107E1.

Final state: full CAN flash of 663312 bytes in 180 s (7 bytes/frame,
synchronous XCP), BUILD_CHECKSUM verify passed, PROGRAM_RESET booted the
new app, and the bootloader CRC gate passes on the next power-on. The
tune (MFS) survives CAN updates - only the app region is touched.

Remaining:
- The flasher speed is protocol-bound (~180 s); the target forbids
  pipelining via ctoPending, so speeding up needs a bootloader change
  (e.g. accept the next frame without waiting for the previous response).
- Wire the flasher into the Java console menu (replacing the broken
  OpenbltJni CAN path).
- First in-car test: ignition cycle while running the flasher (backdoor
  window) and trigger-based reflash.

### Session close (same day)

- The console could not connect after probing because a probe session holds
  the bootloader forever (wasConnected) - the app never restarted. Fixed:
  --probe now sends PROGRAM_RESET at the end and restarts the app.
- Second cause: the rusEFI console app holding the PCAN channel kills the
  bus for everyone else (no ACKs, no RX) - console and flasher must not run
  at the same time. Confirmed by killing all Java and watching the bus come
  back. User confirmed the console connects after the ECU was returned to
  the application.

## 2026-08-19 - Merge maccan-tx-fix into master + idle/ASE calibration

What: merged the whole m74_9 line (trigger profile learning, OpenBLT-over-CAN,
toothdump, board support) into local master (no push) and re-calibrated the
21129.msq idle path for the throttle flow data the user supplied.

Merge:
- Local master was fast-forwarded to origin/master (62028395b8b), then a real
  merge of maccan-tx-fix (3149889524d) was done in a git worktree so the dirty
  main checkout stayed untouched. Merge commit 41d42e2acdc, local only.
- 11 conflicted files resolved: report.md (concatenated both append streams),
  bundle.mk (upstream .FORCE bootloader-bin recipe + our DEVICE_BIN_FOLDER
  recipe), limp_manager.cpp (kick-start block and immobilizer hook combined),
  m74_9.yaml (kept our ADC3 slow-channel pin mapping for IAT/CLT), 4 Java
  files (our PCAN/flasher/migrator fixes on top of upstream), UiVersion bumped
  to 20260819, rusefi_updater.sh (combined exec/"$@" with our macOS flags).
- The 3 conflicted generated files (rusefi_generated_f407-discovery.h,
  rusefi_f407-discovery.ini, VariableRegistryValues.java) were regenerated
  with gen_config_board.sh after building config_definition shadowJar.
- Validation on the merged tree: unit tests 1189/1189 pass; m74_9 firmware
  + OpenBLT bootloader compile and link (build/rusefi.bin, rusefi.srec).

Idle state machine (confirmed in code, idle_thread.cpp):
- Cranking (rpm < cranking_rpm=500): open loop, position = cltCrankingCorr
  x etbIdleThrottleRange / 100.
- CrankToIdleTaper: afterCrankingIACtaperDuration = 20 ENGINE CYCLES (720
  deg each, ~3 s at 800 rpm; revolutionCounterSinceStart counts cycles, not
  revolutions), position blends cranking -> open-loop idle. PID off.
- Idling: open loop base + RPM PID, but the PID is held off for
  idlePidActivationTime = 20.0 s after Idling phase entry. That is the "20 s
  open loop then closed" the user observed. Lambda STFT is disabled in the
  tune, so "closed" = idle position PID only.

Throttle flow interpretation: the stock M74 .clb curves are the ME17 throttle
flow MODEL (direct + inverse), and the kg/h numbers are modeled outputs (no
MAF on 21129, speed-density). The curve is inconsistent at the origin (0%
position -> 0.191% flow = 2.6 kg/h vs the stated 4 kg/h closed-throttle min),
so absolute positions from the curve are unreliable - only the slope (~+2.4
kg/h per 1% position) is usable. Honest warm-idle band: ~1.5-5% position.
Empirical anchor: kg/h = 0.000557 x MAP(kPa) x rpm for the 1.6 L; healthy
warm idle MAP 25-28 kPa -> ~11-13 kg/h. Calibration criterion: warm idle
holds target RPM at MAP 25-28 kPa, whatever % that turns out to be.

Tune changes (21129.msq, variant A - scale-up so idle has headroom while
cranking keeps its exact absolute openings):
- etbIdleThrottleRange 2 -> 8.
- cltCrankingCorr / 4 (124,84,67,65,55,48,41,38 -> 31,21,16.75,16.25,13.75,
  12,10.25,9.5): absolute cranking position unchanged (1.3% at +20C).
- cltIdleCorrTable 100,75,50,30,25... -> 100,85,65,45,40,40,40,40: warm idle
  45 x 8/100 = 3.6% (was 0.5%, which per the curve was ~4-5 kg/h - the
  stalling regime), cold -40C = 8%.
- ASE: postCrankingDurationBins 0,40,80,120,160,200 -> 0,40,80,120,240,400
  cycles; postCrankingFactor only 0C@240 bumped 1.02 -> 1.03. This removes
  the hard fuel-cut step at 200 cycles (was ~30 s at 800 rpm) and tapers to
  1.0 by 400 cycles (~60 s) without adding fuel early.

Follow-ups:
- Bench test: burn 21129.msq, check cranking opening still ~1.3% (good
  starts must not change), warm idle position ~3.5-4%, MAP 25-28 kPa.
- If idle overshoots, trim cltIdleCorrTable warm column 45 -> 40/35.
- Commit the msq change after bench confirmation, then ff-merge master again.

## 2026-08-19 - m74_9: sync-counter semantics + real-car-profile replay tests

What: settled what trgsynchronizationcounter and revolutionCounterSinceStart
actually count on this 60-2 crank + half-moon cam setup, and added decoder
replay tests driven by the REAL per-tooth periods learned on the car.

Findings (unit tests + code reading, not speculation):
- revolutionCounterSinceStart counts ENGINE CYCLES (720 deg = 2 crank revs):
  rpmShaftPositionCallback increments only when trgEventIndex == 0, and on a
  4-stroke the trigger index gets syncCtr%2 x getSize() added, so only every
  OTHER sync point lands on index 0. The ASE/cranking tables therefore step
  once per two crank revolutions - the user's "cycles" question answered.
- trgsynchronizationcounter is NOT a revolution counter: +1 per sync point
  (including error syncs - onShaftSynchronization(wasSynchronized=true)),
  +1 per cam phase-alignment shift (syncEnginePhase parity loop), reset to 0
  by the next sync after any decode error. The 21_54_53 log's 25 syncs for
  ~12 revolutions can only come from noise-induced sync points, NOT from the
  stable cam (phaseResyncCounter stayed 0 there) and NOT from clean-signal
  behavior (the clean replay test proves exactly 1 sync/rev).
- The tune sets silentTriggerError = "don't print": C9002/C9003-style decode
  errors on the car happen WITHOUT text-log output. "No errors in the log"
  does not mean no decode errors - the MLG trgtriggercounterserror/istriggererror
  samples (2 Hz) can also miss transient errors. This hid the fact that in
  the 21_55_46 attempt only 3 of ~8 revolutions validated.
- A single inserted noise tooth (300 us after a real tooth - above the 100 us
  debounce) shifts the decoder index: at the end of the revolution the index
  overflows the wheel -> silent C9002 + desync WITHOUT a sync point; the next
  clean revolution re-syncs unvalidated (sync counter restarts), and only the
  cycle after that counts again. Persistent noise (every revolution) keeps the
  decoder in the desync -> unvalidated re-sync loop forever: the validated-sync
  gate (custom_board_requireValidatedSync) never releases injection/ignition -
  a crank that spins with no fuel/spark at all while the decoder "sees" teeth.
  A missed VR tooth (low amplitude at slow crank) behaves the same way.
- The toothdump profile from 21:54:50 (new firmware) is correctly rotated:
  the gap sits at slot 57. The rotated profiles with the gap at slot 37 were
  from the OLD 20:48 firmware (pre-position-gate false syncs) - not a current bug.

New tests (test_60_2_cranking_transition.cpp, +5):
- crankingTransition60_2RealCarProfileCleanReplay: the real 58-tooth period
  profile replayed for 12 revolutions - 1 sync/rev, 1 validated cycle per 2
  revs, ~300 rpm, zero warnings.
- crankingTransition60_2RealCarProfileIntermittentNoise / NoiseEveryRevolution
  / MissedTooth: the desync -> unvalidated re-sync -> validated cycles
  documented above, with exact sync-counter and cycle-counter expectations.
- crankingTransition60_2RealCarProfileWithStableCam: crank + half-moon cam
  (VVT_SINGLE_TOOTH, rise edge every 2 revs at the same tooth) - the cam adds
  exactly one initial phase-alignment increment, then nothing; no phase-jump
  warnings, no drift.

Also committed with this: the board opt-in VR input debounce
(custom_board_triggerDebounceUs, m74_9 = 100 us) - drops edges closer than
100 us to the previous accepted edge (noise bursts measure <50 us; the
60-2 tooth period at 8000 rpm is 125 us). The pre-existing isToothExpectedNow
rejection only runs above 1000 rpm, so cranking noise was unfiltered before.

Validation: full unit-test suite 1147/1147 pass (was 1142); m74_9 firmware
builds with the debounce.

Follow-ups:
- On the car: with the debounce flashed, a fresh crank attempt + MLG will
  show whether triggerIgnoredToothCount stays 0 and whether the validated
  revolutions now track real engine cycles. The persistent-noise failure
  mode (no fuel/spark at all) vs intermittent noise (bursts of firing) is
  now distinguishable from the counter values alone.
- ETB autograb moving only toward open: deferred (not start-critical), to
  investigate after the first clean start.
- Consider surfacing silent decode errors in the MLG for this board (or a
  console counter) so bench diagnosis does not require the replay tests.

## 2026-08-19 - m74_9: noise-storm false syncs + minimum-elapsed-time sync gate

What: found the mechanism behind the ~2-3x sync-counter race that survived
the debounce (23:10 log: syncCtr +17 over ~7 crank revolutions at 270 rpm,
revCtr +9 over ~3.5 engine cycles, zero visible errors, cam stable at 212 deg).

The mechanism (proven by a replay test before the fix): a dense noise storm
- edges ~250 us apart, above the 100 us debounce - inflates the decoder's
event count, so a storm edge becomes the 58th event since the previous sync.
If that edge lands on a tooth-pair ratio inside the sync windows (chaotic
storm spacings occasionally do: 625/250 us = 2.5), it passes the position
gate AND the count check and false-syncs the decoder CLEANLY: no error, no
desync, just a silent +1 to the sync counter and a shifted phase basis.
Every other storm sync also increments the engine-cycle counter, racing the
ASE/cranking tables ~2-3x. No count- or ratio-based check can reject this -
the count genuinely reads 58 and the ratio genuinely reads in-window.

Fix (trigger_decoder): the only robust discriminator is TIME. A real gap can
only arrive roughly one full revolution after the previous sync point; storm
syncs arrive within a few percent of a revolution. Both sync paths (the
ratio check and the custom_board_syncByPositionWhileCranking override) now
require the candidate to arrive at least a quarter of the previous
revolution's duration after the previous sync (lastFullRevolutionDurationNt,
measured at each sync). The first sync and re-syncs after a desync stay
exempt. The quarter is deliberately loose: the first-combustion catch
compresses the next revolution to ~0.3-0.5x, and no real engine accelerates
4x within one revolution. A stormed revolution now ends in a proper C9002
(index overflow) + desync and recovers through the validated-sync path -
visible behavior instead of silent miscounting.

New test: crankingTransition60_2RealCarProfileNoiseStormDoesNotFalseSync
replays a 53-edge storm with a window-matching candidate at exactly count 58
on the real car tooth profile. Fails without the gate (sync counter advances
on the storm edge), passes with it.

Also learned while instrumenting:
- The engine-sniffer trigger CSVs on this firmware do NOT contain the real
  cranking edges: the ring buffer holds ~1000 stale pairs spaced exactly one
  32-bit-microsecond wrap (71.6 min) apart - the buffer content is garbage
  for diagnosis. Do not trust those CSVs for tooth-level analysis; use the
  toothdump command (board-level capture) instead.
- The MLG output-channel samples can be written in bursts (two records 14 ms
  apart), so counter deltas between adjacent rows do not map to that time
  delta - aggregate over the whole spin window.
- printTriggerTrace in unit tests must be set AFTER EngineTestHelper
  construction and setTriggerType - both reset it.

Validation: unit tests 1148/1148 on GCC and clang; m74_9 firmware built and
flashed over CAN (checksum verified).

Follow-ups:
- Fresh crank attempt on the car: syncCtr should now advance ~1 per crank
  revolution (~4.5/s at 270 rpm) and revCtr ~1 per engine cycle (~2.25/s).
- If the storm gate rejects the real gap at the first-combustion catch
  (extreme single-rev acceleration), the symptom is a C9002 right at the
  catch - the syncByPositionWhileCranking override path would then need a
  catch-specific time exemption; verify with a catch log.
- ETB autograb one-direction issue still deferred.

## 2026-08-19 - m74_9: sync-gap hardening (round 2) - the 23:43 log

What: the 23:43 log (with the 25%-elapsed-time gate flashed) still showed
syncCtr ~2.7x the crank rate (+11 over ~4 crank revolutions at 251-268 rpm,
revCtr +5 over ~2 engine cycles) and one C6728 VVT phase jump of -170.8 deg
(the cam drift net catching a big phase shift). Conclusion: storms that land
late in the revolution pass the 25% elapsed-time gate, and a second hole was
found in the sync-by-position override.

Two more gates, board opt-in via custom_board_syncGapHardening (m74_9 true):
- Real-tooth-before-the-gap: the tooth immediately before a sync candidate
  must have a period >= 1/4 of the previous revolution's mean tooth time.
  The real gap always follows a REAL tooth (compression ripple 0.6-1.4x,
  never below a quarter); a storm-inflated candidate follows a storm edge
  (hundreds of us vs ~4 ms at cranking) regardless of WHERE the storm sits.
  This closes the late-rev storm hole the elapsed-time gate could not reach.
- Override ratio floor 1.2: the sync-by-position skip previously accepted
  ANY failing ratio at count == expected, including a trailing VR ringing
  edge 250 us after the last real tooth (ratio ~0.07) - silently syncing
  one tooth early. The physical gap is 3 tooth slots and cannot compress
  below ~1.2 even at the hardest catch, so ratios below 1.2 are noise.

Both gates are board opt-in because generic wheels trip them on legitimate
gaps: realCrankingNB2/realk20 replay logs (extreme per-tooth cranking
variation), subaru.overrideGap (legitimate sub-1.2 override), and the
AllTriggers fixture all failed when the gates were applied unconditionally -
the opt-in restores them (1149/1149).

New test: crankingTransition60_2RealCarProfileTrailingNoiseEdgeNotAccepted
(trailing ringing edge at count 58, ratio 0.07 - must be rejected, then the
real gap errors with count 59 and the decoder recovers).

Validation: unit tests 1149/1149 GCC + clang; m74_9 built and flashed over
CAN (checksum verified).

Follow-ups:
- Next crank attempt: expect syncCtr ~1/rev (~4.3/s at 260 rpm) and revCtr
  ~1/cycle (~2.1/s). C9002 storms are now possible and expected when noise
  inflates a revolution - each one forces the validated-sync recovery, which
  is the correct visible behavior.
- ETB autograb one-direction issue still deferred.

## 2026-08-20 - m74_9: first real catch - C9002 at the catch, floor lowered to 0.8

What: the 00:05 log (full hardening flashed) showed the first REAL catch:
rpm 262 -> 328 (the engine fired - first combustion happened), counters
finally correct (syncCtr 1/rev, revCtr 1/engine-cycle - the storm racing is
gone), the cam stable at 212 deg. But a C9002 "expected 58 got 58" fired
right at the catch and the engine died: the REAL missing-teeth gap
compressed below the 1.2 override floor, so the sync was rejected, one full
revolution was counted and the decoder desynced - exactly the failure mode
the sync-by-position override was built to prevent.

Fix: lower the override ratio floor 1.2 -> 0.8. Physics: the gap spans 3
 tooth slots; even the hardest first-combustion acceleration cannot compress
it below ~1.0 tooth times (the crank speed would have to more than triple
across a single 6-degree tooth) - and the observed m74_9 catch went below
1.2. The 0.8 floor sits below any physical gap and remains 10x above the
trailing-noise case (0.07) the floor was built for; that noise case also
desyncs via the wheel index overflow at the noise edge, so rejecting it is
the visible path either way.

Tests updated: the trailing-noise test now models the noise edge right after
tooth 57's rise (ratio 0.08) and pins the full chain (floor rejection ->
C9002 -> absurd-ratio gap cannot re-sync -> unvalidated re-sync -> validated
cycle). New test crankingTransition60_2CatchGapBelow1_2AcceptedWithHardening
pins the catch (gap ratio 1.0 must be accepted with hardening on; it fails
with the 1.2 floor). 1150/1150 on GCC and clang.

Known limitation (documented, not fixed): a VR ringing edge ~250 us after
the LAST tooth's FALL sits ~1.2-1.3 tooth-times after the previous rise -
above the 0.8 floor and indistinguishable from a compressed catch gap by
ratio or duration. If accepted, it shifts the phase basis by one tooth
(~6 deg, below the 15 deg cam drift limit) and self-corrects never. The
stock ECU's adaptive VR conditioning prevents this class at the analog
level; for us it is a minor phase error, not a start blocker.

Follow-ups:
- Reflash (PCAN adapter was unavailable at commit time - PCAN_ERROR_ILLHW)
  and take the next crank attempt: the catch should now survive (no C9002),
  and the engine should keep running after the first combustion.
- ETB autograb one-direction issue still deferred.

## 2026-08-20 - m74_9: VR path mapped on hardware; L9779 VRS switched to full adaptive mode

What: the user buzzed out the board (the KiCad schematic is known-unreliable,
e.g. it draws OUT_VRS on L9779 pin 25 while the datasheet says pin 8).
Measured path: crank sensor connector AA1/AB1 -> 10k -> L9779 pin 6 (VRSP) /
pin 7 (VRSN); L9779 pin 8 (OUT_VRS, open drain) -> series R -> 74HC14 pin 1
-> pin 2 -> PF8 (triggerInputPins[0] = Gpio::F8). The 10k series resistors
are the ST reference circuit: the datasheet hysteresis table is quoted
"with 10 kOhm ext resistors".

Key finding: the rusEFI l9779 driver never wrote CONFIG_REG1/CONFIG_REG5,
so the VRS interface ran on power-on defaults: limited adaptive mode
(CONFIG_REG1 reset 0x08, bit1=0), auto temporal filter OFF (CONFIG_REG5
reset 0xd8, VRS_MODE=01), hysteresis floor 17 uA (347 mV with 10k). In that
mode the interface behaves almost like a fixed-threshold comparator with a
4 us mask - the noise storms pass through to PF8 and false-sync the decoder
during cranking. The hardware always had the adaptive conditioning; it was
never switched on.

Fix: vrs_configure() in hw_layer/drivers/gpio/l9779.cpp writes
CONFIG_REG1=0x0a (full adaptive) and CONFIG_REG5=0xf9 (VRS diag on, auto
hysteresis + auto filter both on, hysteresis floor 5 uA / 100 mV so weak
cranking amplitude still passes) from chip_init() after START. Fully
adaptive mode scales the hysteresis with the actual tooth amplitude (peak
detector + 5-level quantizer) and enables the adaptive masking filter -
the stock ECU's conditioning of the same sensor through the same chip, no
hardware mod needed. Config registers are write-only and reset on power
cycle, so a bad value is always recoverable.

Validation: m74_9 firmware build clean (BUILD SUCCESSFUL). Unit tests not
run: the driver compiles only into firmware builds (ChibiOS HAL, not part
of the host test binary).

Follow-ups:
- Flash over CAN and take a crank: the console must show "L9779 VRS: full
  adaptive mode" at boot; then rawtrg/toothdump should show clean tooth
  spacing without the 50-500 us noise bursts.
- The pullup on the OUT_VRS net (open drain) is not yet located on the
  board - verify it exists.
- 74HC14 inverts the signal; trigger edge polarity may need re-checking
  once the analog stage is clean.

## 2026-08-20 - m74_9: VRS full adaptive mode verified on the bench (rawtrg/toothdump/MLG)

What: flashed 6dbec586c14, boot line confirms "l9779 VRS: full adaptive mode
(CONFIG_REG1=0x0A CONFIG_REG5=0xF9)". Two bench captures analyzed
(rawtrg/toothdump + one MLG).

Results:
- Noise storms are GONE: both rawtrg rings (1047 and 1857 edges) have zero
  deltas below 2 ms; the old captures were full of 50-500 us bursts. Edges
  alternate F/R strictly (6 same-polarity anomalies in 1047 edges).
- The 60-2 gap is seen cleanly and consistently: in capture 1 the big
  deltas land exactly every 116 edges = 58 teeth (9 gaps, spacing exactly
  116); the toothdump captured 4 full revolutions with the gap at slot 57
  in every revolution (39.5 ms vs 15 ms tooth = ratio 2.6, inside the
  [1.6, 3.75] window - the hand-spin decelerates through the gap).
- The ECU synced on the bench wheel; no trigger errors anywhere
  (istriggererror=0 in the MLG, no C9002/C9003 in the text logs).

Open items:
- The 09:56:52 MLG crank at 248-264 rpm shows syncCtr racing (1->16 in
  ~0.6 s) and a C6728 (CUSTOM_VVT_PHASE_JUMP) at spin end: the cam
  cross-check (custom_board_vvtDriftLimit 15 deg) forced re-syncs while
  the crank decelerates. triggersecondaryrise=0 throughout - the cam sensor
  appears disconnected (it was unplugged on 2026-08-18 for a test). Confirm
  cam wiring; the crank side itself is clean.
- Real-car crank is the next validation step: watch istriggererror,
  syncCtr ~1/rev, revCounter 1/engine-cycle, and whether it catches.

## 2026-08-20 - m74_9: correction - car-session logs fully consistent, no sync racing

Correction to the previous entry: the 09:55-09:57 captures were from the car,
not the bench. Session-wide totals (MLG timestamps are write times, never
compare adjacent rows): triggerprimaryrise=929 over ~15 s of slow rotation
(~60-100 rpm, wheel spin-down) + ~1 s of cranking at 248-264 rpm - exactly
consistent with the clean rawtrg rings (no delta below 2 ms). syncCtr=16 over
the same window matches ~1 sync/s slow rotation + crank syncs + cam
phase-alignments: no racing.

One C6728 (VVT phase jump -177.6 deg) fired at ~60 rpm: the decoder synced
one crank revolution off (60-2 alone cannot resolve the 720-degree phase),
the cam cross-check (custom_board_vvtDriftLimit 15 deg) caught it and forced
a clean re-sync. The protection works as designed.

Zero C9002/C9003/newerr in both car logs - the first session ever without
trigger decode errors. Next step: a real start attempt (ignition on, crank to
catch); the previous catch blocker (C9002 right at first combustion) should
be gone.

## 2026-08-20 - m74_9: trigger pipeline verified healthy on the car; remaining blocker is ignition timing

What: analyzed the car sessions with cam sync off (10:16) and on (10:25).
Session totals across the two cam-on binary logs: 1537 and 1306 rising
edges for 27 and 23 syncs = 57-58 teeth per sync - exactly the 60-2 wheel,
1 sync per revolution. Cam events 12 per 27 syncs = 1 per 2 revs (the
expected half-rate). rpm reads correctly (~285). Zero trigger errors. The
earlier "3-6x counter inflation" reading was an artifact of MLG record
timestamps (write-time, not sample-time) - aggregate totals scale perfectly
with crank duration. The trigger pipeline (L9779 adaptive VRS + debounce +
gates) is healthy.

Remaining blocker: the engine does not catch. Evidence: coils fire (51
sparks) but nearly all overcharge (42/51, dwell 8.24 ms, dwellactualratio
120-154%) - the discharge does not land at the scheduled angle. One cam
phase re-sync per crank (phaseresync=1) shifts the basis mid-dwell. The
leading hypothesis: the ignition timing reference moved - the VRS full
adaptive mode switches near the tooth zero-crossing while the old limited
mode (347 mV floor) switched early on the tooth slope, so the tune's 114
deg trigger offset no longer matches the edge the decoder uses. The stock
ECU uses the same adaptive conditioning, so matching the stock timing with
a timing light is the reference.

Next steps: timing light on cyl 1 during cranking (compare with the stock
ECU), then adjust globalTriggerAngleOffset around 114 deg and retry; if
timing is right, move to cranking fueling.

## 2026-08-20 - m74_9: offset sweep (120..108) - no catch; cam phase flips mid-crank

What: four car cranks with different globalTriggerAngleOffset values
(current tune ends at 108, one attempt at 120 was reverted to 114 by the
console's tune migrator). No catch in any session (rpm never above ~303).

Findings:
- Trigger pipeline stays healthy everywhere: 56-58 rises/sync, rpm ~285-303,
  zero trigger errors.
- The offset sweep DID improve ignition scheduling: dwell overcharge dropped
  from 42/51 (offset 114, earlier session) to 2-14 per crank.
- Cam phase basis flips ~175-187 deg mid-crank in 2 of 4 sessions (10:52:56:
  200 -> -27 deg; 10:54:19: 218 -> -33 deg; drift-limit cross-check fired,
  forced re-sync, basis stays flipped -> sparks on the wrong stroke). The
  other two sessions hold a stable 206-218 deg (matching the known-good
  212 deg) and STILL do not catch.
- MAP reads garbage (C6899 Invalid MAP 3.52 kPa at boot, fallbackmap 60 kPa
  is used for fueling).

Conclusion: trigger/sync is solved; no-catch is now a calibration/hardware
issue, not a decoder issue. Remaining suspects in order: (a) actual spark
landing (timing light on cyl 1, compare with the stock ECU - the offset
range tried may still be wrong or coils are swapped), (b) coil order
(ignition bench test per coil), (c) cranking fuel with the broken MAP.

Also noted: the drift-limit cross-check (custom_board_vvtDriftLimit) forced
re-syncs can leave the phase basis flipped 180 deg after a mid-crank
desync - worth gating to above cranking rpm or making the re-sync validate
against the last known basis.

## 2026-08-20 - m74_9: cam-less (two-stroke) experiment rolled back

What: attempted cam-less operation (early-return on VVT_INACTIVE cam events
+ two-stroke mode for 360-degree firing). Rolled back per user: the cam sync
works fine and cam-less operation is not wanted.

Key insight retained: with the cam SENSOR physically connected and vvtMode
= Inactive in the tune, the cam events still arrive and the drift
cross-check desynced the crank decoder on every cam event (today's
syncCtr=0 / nothing-scheduled session). With the sensor physically
UNPLUGGED (2026-08-18 session) no cam events arrive, so cam-less operation
worked and fuel was delivered. Keep the cam connected and vvtMode = Single
Tooth; the no-start problem is ignition/fuel calibration, not trigger.

## 2026-08-20 - m74_9: ROOT CAUSE of no-start - L9779 direct-channel enables broken

The injectors never opened and bench sparks were irregular because the
L9779 direct-driven outputs (IGN1..4 on PF12..15, OUT1..4 on PE8..11) are
an AND of the SPI enable bit and the parallel input pin, and two bugs
broke the enable: update_output() clobbered its own o_data with o_state
(enable followed the pin state, so a low pin = disabled channel), and
writePad() did not wake the driver thread for direct pins (the enable
update arrived only on the ~105 ms watchdog cycle). Bench pulses and
injection pulses were truncated to a random 0-5 ms or never happened:
missing coil clicks, no rail pressure drop in fuelbench2, no fuel to the
engine - the whole no-start. Fixed in 6b3a686c30d: enables now carry
o_oe_mask permanently, the parallel pins alone switch the channels in
real time, and the driver thread is woken on every write. Flashed
(verified, reset OK). Next: re-run the bench tests, then crank.

## 2026-08-20 - m74_9: crank-only sync forced in firmware, cam drift check removed

What: per user request the engine now runs crank-only. ConfigOverrides
forces vvtMode[0]=VVT_INACTIVE AND camInputs[0]=Gpio::Unassigned on every
boot (stored tune cannot re-enable cam sync), and the
custom_board_vvtDriftLimit opt-in (15 deg) is removed from the board
overrides (8a2795d7639).

Why: the cam variable was masking the start sequence - the drift cross-check
forced crank re-syncs during cranking-band phase jitter and one session left
the basis flipped 180 deg. Both the cam pin AND the mode are forced off:
with VVT_INACTIVE alone the decoder still processes cam edges and logs
CUSTOM_VVT_MODE_NOT_SELECTED per edge, and (from the 2026-08-18 session)
cam events with an inactive mode previously desynced the crank decoder.
Unassigning the pin is equivalent to physically unplugging the sensor - no
events, no warnings, no cross-check.

Consequence: without cam sync the engine runs half-sync - each coil fires
once per crank revolution (wasted COP), injection batches. This is the
intended crank-only fallback; sequential operation requires removing the
ConfigOverrides block.

Validation: m74_9 firmware builds clean (BUILD SUCCESSFUL), rusefi.bin /
rusefi.srec refreshed in deliver/. Not flashed yet - PCAN adapter was not
attached during the last flash attempt. Flash via openblt_can before the
next crank.

## 2026-08-20 - m74_9: cam-off is a tune setting, not a firmware force (correction)

Per user: the ConfigOverrides forcing of vvtMode[0]=VVT_INACTIVE and
camInputs[0]=Unassigned from 8a2795d7639 is reverted (7a833ca5e70). The
crank-only switch now lives in the tune: 21129.msq sets vvtMode1 = Inactive
and camInputs1 = NONE. The drift-cross-check removal stays - it has no TS
parameter and was the actual crank-desync source when cam events arrived
with an inactive mode (2026-08-18).

Note for the future: loading an older 21129.msq (or any tune with
vvtMode1 = Single Tooth) re-enables cam sync silently; the drift check is
gone either way now, so the only side effect is log noise
(CUSTOM_VVT_MODE_NOT_SELECTED) unless the cam pin is unassigned too.

## 2026-08-20 - m74_9: flashed 7a833ca5e70 over CAN (OpenBLT/XCP)

Flash of the cam-off-in-tune build (7a833ca5e70) succeeded on the second
attempt: the first run died mid-erase at 0x08020000 with PROGRAM_CLEAR
failed (the known intermittent XCP glitch, same as 2026-08-19 on
6b3a686c30d), the retry passed with verified=true, reset=true.
The ECU now runs: injector parallel map fix (c52438880cc), L9779
enable/wakeup fix (6b3a686c30d), drift-check removal, no firmware-side
cam force. Next on the car: load the updated 21129.msq (vvtMode1 =
Inactive, camInputs1 = NONE), then fuelpumpbench + fuelbench2 1, then
crank with a binary log.

## 2026-08-20 - m74_9: trigger edge polarity - falling is the stable edge (data-proven)

Question was whether the 74HC14 inversion means the decoder must use the
falling edge. Answered from data, not theory: rawtrg dump analysis
(2026-08-20 10:16 log, 1021 teeth, ~60-90 rpm) compares tooth periods
anchored on each edge type. F-anchored (rising) periods: 6.02% detrended
std; R-anchored (falling): 2.63% - the falling edge is the zero-crossing
reference (speed/amplitude independent), the rising edge carries the
hysteresis-return jitter. Cross-check: the F->R split is a constant 38% of
the tooth in two independent dumps (10:16 and 10:18), a fixed hysteresis
asymmetry, not noise.

Fix (978264b9309): 21129.msq invertPrimaryTriggerSignal Rising -> Falling.
The sync reference moves ~2.3 deg later (38% of a 6 deg tooth), so
globalTriggerAngleOffset needs roughly -2 deg (114 -> ~112) and a timing
light check once the engine runs. Sync-ratio windows stay valid: the gap
duration on falling edges is the same physical span.

Analysis script kept at .tmp_edge_analysis.py (parses rawtrg rows from a
text log; dump convention: rXXX[i] = type of the edge STARTING delta
dXXX[i]).

## 2026-08-20 - m74_9: edge-finding caveats (user review)

User correctly pointed out that the digital stream does not expose the
analog domain. Clarified in CLAUDE.md: the rawtrg variance analysis proves
only that the PF8 falling edge is the most REGULAR edge in the digital
stream (captured pre-debounce, so the only shapers upstream are the L9779
conditioner and the 74HC14). The zero-crossing interpretation is a
hypothesis - final confirmation is a strobe at fixed cranking timing
(10 deg in the tune): the mark must not drift with starter speed.

Also flagged: custom_board_triggerDebounceUs (floor 100 us) debounces BOTH
polarities; with the constant 38/62 F/R duty the useful edge arrives
61-98 us after the previous opposite edge at 6500 rpm - below the floor
(tooth period 159 us). Fine at cranking (2-5 ms gaps), but the debounce
must be tied to the selected polarity or the floor lowered before
sustained high-rpm running.

## 2026-08-20 - m74_9: ETB autocal Close-phase stop detector fixed (a498a11bca5)

Symptom: after the earlier stop-detector fix the throttle only drove open -
the autocal never visibly rode it closed. Root cause: the detector fired on
a single pair of equal TPS reads at 50 ms, but the TPS ADC updates slower
than the ETB loop, so two equal reads mean "no new sample yet", not "plate
stopped". The calibration captured tpsMin mid-travel, cut the closing drive
after ~50 ms and stored a broken TPS scale (which also explains the
one-direction behavior in normal operation).

Fix: the stop is declared only when the reading has been frozen for >= 100 ms
CONSECUTIVELY (freeze clock restarts on every new sample) AND the plate has
travelled >= 0.2V from the open capture. A never-moving throttle falls
through to the 1s timeout and the existing |max-min| < 0.5V wiring check.

Validation: m74_9 build OK, full unit test suite 1151/1151. Not flashed yet
(PCAN adapter not visible on USB). After flashing: run the autocal on the
bench - the plate must open ~1s, then ride closed to the stop and the
calibration must report sane min/max (no Overcurrent shutdown diag).

## 2026-08-20 - m74_9: MAP is healthy, boot C6899 was an init race (correction)

Earlier sessions assumed MAP was broken (C6899 + fallback 60 kPa). Wrong:
MLG analysis shows isMapValid=1 throughout and physical pressures - 100.7 kPa
at key-on engine-off, 91.6 kPa cranking with the throttle open, 79.3 kPa at
the 462 rpm catch as vacuum builds. Fueling uses the REAL MAP; the
fallbackMap channel (60) is the estimation-table value and is only engaged
when isMapValid=0, which never happened in these logs.

The one-shot "Invalid MAP at 3.53" at power-on is a startup init race: the
first fast-ADC sample arrives before the MAP sensor configuration (curve,
divider) is applied. Fixed cosmetically (warning gated on
hasEverDecodedMap, map_averaging.cpp) - a genuinely broken sensor is still
reported after its first successful conversion and by the sensor checker.
Baro capture was already reading correctly (101.32 kPa at boot).

Validation: m74_9 build OK, unit tests 1151/1151.

## 2026-08-20 - m74_9: noise-inflated decoder cycles - noiseless filter enabled

On-car rawtrg (15:43) shows noise edges at 1.9-3 ms during cranking; bench
captures were clean (>= 4.6 ms). The debounce cannot stop them: at 280 rpm
the legit F->R gap is 1.44 ms and the adaptive debounce is already at
1.23 ms - any higher and the useful falling edge dies. The noise inflates
the decoder event count so fake 58-event revolutions complete with EXACT
counts (no C9002/C9003), syncCtr advances ~29/s while the wheel turns
4.7 rev/s, revCtr races ~5x, and each fake sync re-anchors the 720-degree
basis - spark and injection fire from a moving zero, the engine never
catches. RPM reads correctly (averaged), which masked the problem.

Fix: useNoiselessTriggerDecoder = yes (rejects off-schedule events before
the decoder counts them; sync windows keep syncRatioAvg=3 for its gap
prediction). Also triggerSimulatorRpm 1200 -> 0 (self-stimulation would
gate out the real trigger input entirely).

Observation rule of thumb: if syncCtr advances faster than revolutions
while istriggererror stays 0, the decoder is completing exact-count fake
revolutions from noise - enable the noiseless filter, do not raise the
debounce (F->R gap ceiling).

## 2026-08-20 - m74_9: flashed (ETB autocal fix + MAP C6899 suppression)

Flashed over CAN: 665352 bytes, verified=true, reset=true (237 s, no XCP
retry needed this time). The ECU now runs a498a11bca5 (ETB autocal Close
phase: sustained-freeze + travel stop detector) and be414f3d80e (boot-time
C6899 suppression). Tune changes (Falling edge, cam off, noiseless filter,
triggerSimulatorRpm=0) live in 21129.msq and are NOT in the firmware - the
user must load ~/21129.msq in the console.

## 2026-08-20 - m74_9: injectors dry despite events - permanent-enable fix was dead code

Symptom: plugs absolutely dry while the ECU reported 40 injection events
(3.2 ms pulses, actuallastinjectionratio ~1.0), no fuel cut, no faults;
bench clicks worked and rail pressure was present.

Root cause: update_output() computed o_data (permanent enables from
o_oe_mask) but the CONTR macros read the o_state member - 6b3a686c30d was
dead code. Every direct-pin LOW also cleared the SPI enable bit; on the
bench the driver thread re-wrote CMD=1 while the pin was still high (50-100
ms pulses), but during engine run (3.2 ms pulses, ~76 pin toggles/s) the
CMD=1 write arrived after the pin fell - the AND gate never opened.

Fix: macros read o_data; writePad skips o_dirty for direct pins (their
CONTR bits are static). Now the parallel pin alone switches the channel in
real time. Flashed next.

## 2026-08-20 - m74_9: L9779 WDA watchdog window misses during cranking (109d17209ae follow-up)

Symptom on the car: occasional spark/fuel bursts ("иногда попадает
таймер"), mostly dead outputs, plugs dry; bench worked right after cranking.
pins during cranking showed the smoking gun: l9779 WDA ec=7 wda_int=1
ok=479 fail=0 - the driver's answers kept missing the chip's ~12.6 ms VDA
2.0 answer window because the thread ran at PRIO_GPIOCHIP (NORMALPRIO+8),
below main loop/ADC (+10), ETB (+9) and CAN RX (+11). EC > 4 forces
OUT1..4 + IGN1..4 off in hardware; the driver saw zero SPI errors so no
fault was ever flagged. After cranking the workload dropped, answers landed
in the window, EC recovered to 0 and the bench clicked again.

Fix: the L9779 driver thread now runs at NORMALPRIO+12 - above all cranking
workloads. Rule of thumb added to CLAUDE.md-adjacent knowledge: when ECU
counters are healthy but physical outputs are intermittent only during
cranking, dump pins and check l9779 WDA ec/wda_int before touching the
event path.

## 2026-08-20 - m74_9: stock L9779 VRS config extracted from the stock firmware dump

User pointed out the stock ECU runs the same board flawlessly, so the
hardware is fine and OUR VRS register values were the suspect. Found the
stock L9779 module in Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin
('IC_EMS' string, WDA response table matching ours byte-for-byte at
0x4EF4C) with the config script at 0x4EF8C.

The stock RAMPS the VRS registers (REG5: 0x0C->0x0D->0x0E->0x0F = VRS_HYST
100..111, i.e. the MCU raises the hysteresis floor as amplitude grows -
the software version of the adaptive conditioning). Our REG5=0xF9 had
picked the SMALLEST floor (001 = 5 uA = ~100 mV with the 10k resistors) -
maximum noise sensitivity; the cranking noise passed straight through.

Implemented the stock scheme: vrs_configure() writes the ramp START
(REG1=0x02 REG4=0x0B REG5=0x0C REG6=0x07), the driver thread writes the
ramp END (REG4=0x08 REG5=0x0F REG6=0x06) once rpm >= 300. Flashed next;
if the noise persists, extract the exact ramp timing from the stock code
(the table pairs suggest per-crank stepping).

## 2026-08-20 - m74_9: C9003 'got 56/0' kills the engine at the first-combustion catch - early-gap acceptance + sync trace

Symptom (17:45 session, stock VRS ramp already flashed): the engine caught
like the stock ECU (rpm 189-281, first real combustion behavior) and died
right at the catch. Log: C9003 'not enough teeth between sync points:
expected 58/0 got 56/0' at rpm=189 with newerr gap0=2.171 gap1=1.309 -
BOTH gap ratios inside the tune's override windows [1.6, 3.9] / [0.85, 1.35]
(21129.msq: overrideTriggerGaps=yes), the position gate passed (56 + 2 >=
58), only the count check failed. countersError=-2 confirmed in the MLG
(trgtriggercounterserror). The desync cut fuel/spark exactly when the
engine first fired.

Input forensics: the rawtrg ring (1003 edges, same crank) is PERFECT -
strict F/R alternation, zero edges < 500 us (the stock L9779 VRS ramp fixed
the noise storms), exactly 58 fall-to-fall teeth between the real gaps in
the running region. Offline simulation of the full pipeline (debounce 1.5
ms adaptive, noiseless filter, position gate, sync-by-position bypass) on
that stream gives 8 clean syncs and NO C9003 - the 2-event deficit cannot
be reproduced from the captured pin stream. The decoder state carried over
from before the ring buffer start (intermittent cranking), so the lost
events happened outside the captured window; where exactly the 2 events
were lost (L9779 analog edge swallowing during the catch is the prime
suspect - the spin-down shows it loses dozens below ~70 rpm) stays open
until the new sync trace catches the next one.

Fix 1 - cranking-band early-gap acceptance (board opt-in
custom_board_syncEarlyGapWhileCranking, m74_9 sets true): a
ratio-validated sync candidate arriving 1-2 events EARLY (count deficit)
is accepted as a valid sync while rpm < 2 * crankingRpm instead of
desyncing. A deficit cannot be noise (noise only inserts events, shifting
the count up); the 6-12 deg phase offset of the lost events only affects
the already-elapsed part of the revolution (the sync re-anchors at the
gap). Count excess and deficits >= 3 keep the classic C9002/C9003 paths.
The acceptance prints 'earlygap' via printGaps for the logs.

Fix 2 - diagnostics: boardTriggerSyncEvent() board hook (weak default in
trigger_board_hooks.cpp) fires at every sync point with kind/count/gap0/
gap1; m74_9 records a 32-event ring + 'synctrace' console command which
also prints the new triggerDebounceDropCount/triggerNoiseFilterDropCount
and the existing ignoredTooth/ordering counters. Next crank will show the
decoder-side truth next to the pin-side rawtrg.

Unit tests (test_60_2_cranking_transition.cpp): new helper
fire60_2RevolutionWithLostEvents (1-3 events lost mid-rev, merged tooth
carries their time); 4 tests - early gap accepted while cranking, C9003
without the opt-in, rejected when running (above 2 * crankingRpm), and
3-event deficit still desyncs via C9002 index overflow. Full suite 1154
tests pass (host clang). Firmware built for m74_9.

Also fixed: map_averaging hasEverDecodedMap is now inside #if HAL_USE_ADC
(the unit-test build with HAL_USE_ADC=0 failed -Wunused-variable), and the
unit-test build needed touch firmware/integration/rusefi_config.txt after
the stale value_lookup_generated.cpp checkout stamping issue.

Open: flash the new firmware (PCAN adapter was not attached -
PCAN_ERROR_ILLHW), then one 2-3 s crank with binary log + immediate
rawtrg + synctrace; expect either 'earlygap' acceptance keeping the engine
alive at the catch or a clean 58/58 sync.

## 2026-08-20 - m74_9: +1 phase-basis latch (whole crank 6 deg off) - profile gated to running rpm + offset 114->112

User report after the early-gap fix: the engine now catches almost every
crank, but sometimes it will not and it "hits the wrong ignition angle".
The 19:53:34 synctrace showed the smoking gun: 9 consecutive 'S' syncs with
gap0 1.0-1.08 at 265-271 rpm - the decoder was synchronized on an ORDINARY
tooth for the whole crank, every scheduled event fired 6 degrees late.

Seed chain (reconstructed + unit-tested):
  1. 1 event lost mid-revolution AND the missing-teeth gap compresses to
     ~1.0-1.2x at the catch -> the gap arrives at count 57 with a ratio
     below the windows; no sync there.
  2. At count 58 the tooth AFTER the gap arrives with ratio 1/gapRatio
     (~0.83-1.0, above the 0.8 bypass floor) and the cranking-band
     sync-by-position bypass accepts IT -> phase basis +1 tooth (6 deg).
  3. With the shifted basis the profile-normalized gap check applies the
     wrong slots' factors (the sliding slot window overlaps slot pairs
     between the correct and the shifted basis) and could keep the REAL gap
     out of the windows forever - the latch persisted for the whole crank.

Fix (per the user's design intuition "survive cranking first, the learned
table only after"): the learned-tooth-profile normalization now applies
ONLY at running rpm (>= 2 * crankingRpm). During cranking the normalized
check IS the raw check, the real gap always reads ~2-3 regardless of the
slot belief, so a latched basis re-anchors at the real gap within one
revolution through the existing early-gap acceptance (kind 'A'). The
combustion ripple the profile exists for starts to matter at running rpm;
idle (~840) is still below the threshold but the count-58 bypass covers a
compressed idle gap there. Count-58 bypass (0.8 floor), count-56/57 early
gap and the classic C9002/C9003 paths unchanged.

Tune: globalTriggerAngleOffset 114 -> 112 (falling-edge reference moved the
sync edge ~2-3 deg later; MUST be strobe-verified at fixed cranking timing
- the L9779 zero-crossing duty varies with amplitude/speed).

Tests: new test_60_2_early_gap_latch.cpp - captures sync events via a
strong boardTriggerSyncEvent override; seed (lost event + 1.2x gap ->
bypass on the ordinary tooth, kind 'S' gap0 0.83) then heal at the next
real gap (kind 'A', countErr -1, gap0 3.0) then a clean 'S'; plus a clean
revolution never false-fires. 1156 tests pass. Firmware flashed, tune
updated (user must re-load ~/21129.msq after the flash for the new offset).

Note: a static-profile distortion cannot be unit-tested for the latch (any
static slot distortion that breaks the shifted-basis check also corrupts
the first sync through the sliding window) - the profile gate removes the
mechanism by construction instead.

## 2026-08-20 - m74_9: per-polarity trigger debounce + newest-first rawtrg dump

The 20:45 car log showed the +1-basis latch AGAIN even with the profile
gated to running rpm (nine 'S' syncs at gap0 0.93-1.12, 210-290 rpm) - so
the profile distortion was not the only latch mechanism. The 20:38 ring
(min delta 2346 us) shows the L9779 output duty compresses with rpm: at
295 rpm the tooth is 3.5 ms and one half-tooth can be ~1.16 ms against the
debounce threshold toothUs/3 = 1.17 ms - the any-edge debounce can eat a
REAL edge right at that boundary, which shifts the decoder's phase basis
and makes the gap ratio check fail every revolution (the bypass then
anchors the basis on an ordinary tooth).

Fix: the debounce window is now applied PER POLARITY (a noise burst is a
same-polarity double edge; a real same-polarity edge is a full tooth away,
far above toothUs/3). The opposite edge of a real tooth no longer arms the
debounce, so the high-rpm duty compression cannot eat useful edges.

Also: 'rawtrg' now dumps NEWEST FIRST - a console capture truncated
mid-dump (the user closing the console) still contains the most recent
edges, the interesting end. Lines keep their real edge indices; parse and
sort when reconstructing the stream.

Note: MFS settings writes failed with status -1 (MFS_ERR_INV_STATE) after
a tune burn - the MFS driver had wedged into MFS_ERROR on a transient flash
failure; a power cycle cleared it. If it recurs, look at SPI contention
between the MFS flash and the L9779 driver during burns.

## 2026-08-20 - m74_9: SV#6 crash root cause = rawtrg print loop wrap; fault marker now survives

The recurring on-car crash (SV#6 assert, "at catch") was chased through
three instrumented builds and turned out to be a red herring chain:

1. REAL root cause: the newest-first 'rawtrg' dump loop did
   'start -= 16' on a size_t. With an edge count not divisible by 16 the
   final partial chunk underflows the counter; the loop then walks the
   whole address space printing garbage and hard faults. PRECISERR,
   faultAddr=0x1FFEFF00 = ordered + begin*16 with begin negative (=-11241),
   pc/lr in m74_9RawTriggerDump. The user always ran rawtrg after a catch,
   hence the fake catch correlation - the engine itself never crashed.
2. SV#6 was SECONDARY: logHardFault called efiPrintf inside the HardFault
   handler; CriticalSectionLocker -> chSysGetStatusAndLockX sees IPSR != 0
   (fault handlers have no OSAL_IRQ_PROLOGUE, isr_cnt == 0) -> routes to
   chSysLockFromISR -> SV#6 assert. Its marker (C0FFEE02) overwrote the
   real fault marker (C0FFEE01), hiding pc/lr/cfsr.
3. Instrumentation that found it (kept, cheap - two word stores per
   X-class lock):
   - ChibiOS chdebug.c __dbg_check_lock_from_isr records the
     chSysLockFromISR caller + lock/isr counters;
   - ChibiOS chsys.c chSysGetStatusAndLockX records its caller + IPSR;
   - error_handling.cpp persists them in BKP9..BKP12 and prints them with
     the assert report ("sv6 caller=...", "xlock caller=... ipsr=...").
   The ipsr value alone identified the HardFault context (ipsr=3).

Fixes (rusEFI commit 72f348db1d8, ChibiOS e8525e0cc1):
- rawtrg loop terminates explicitly after the partial chunk; the ring copy
  uses a one-shot head snapshot (edges arriving during the dump no longer
  shift the copy base).
- logHardFault writes the fault crash marker BEFORE any printf.
- efiPrintfInternal and chDbgPanic3 skip lock-taking prints / marker writes
  while isInHardFaultHandler is set, so a fault-context print can never
  trip SV#6 again.

Validation: m74_9 firmware builds, unit tests 1156/1156, flashed (666616
bytes verified). On-car: rawtrg now survives any edge count - pending the
user's next crank session.

## 2026-08-20 - m74_9: sync/raw trace timestamps lie between cranks; clock keep-alive + systime

USER GROUND TRUTH (recorded): the crank attempts are short starter
impulses (1-1.5 s each, never >10 s in the whole project). The synctrace
of ONE impulse shows the stored time running from ~30 ms (boot events) to
~30 s and then to ~90 s while only 1-1.5 s of real time passed - the
stored timestamps jump FORWARD at the cluster boundaries. Verified across
all five sessions (22:27-22:31): inter-cluster stored gaps of 42.3/46.2/
63.3 s and a 251 s rawtrg gap, while the intra-crank times are correct
(sync every ~229 ms at 262 rpm, etc.). The teeth NEVER disappeared - the
40-65 s "VRS dropout" was a clock artifact, and the L9779 VRS changes
(4402c35d398, 1307b3cbede) were reverted as wrong-theory meddling.

Mechanism: getTimeNowNt() = TIM5 CNT (4 MHz) extended by WrapAround62
(2^30-tick window = 268 s). The clock is sampled on trigger edges and by
console traffic; between cranks nothing polls it, and the classifier
miscounts the gap on the next sample - the error surfaces as forward time
jumps at cluster boundaries. The low 32 bits of the NT clock are always
the raw TIM5 counter, and the angle scheduler uses only those (compare vs
CNT), so spark/injection timing was NEVER affected - only the diagnostic
timestamps and 64-bit durations lied.

Fixes (3d74ff9b23c):
- doPeriodicSlowCallback samples getTimeNowNt() at 20 Hz - the wrap
  classifier never sees a stale gap again (50 ms vs 268 s window = 5000x
  margin; the cost is one timer-register read per tick).
- synctrace timestamps switch to TIME_I2MS(chVTGetSystemTimeX()) - the
  1 kHz kernel-maintained SysTick, trustworthy by construction (the whole
  RTOS timing - thread sleeps, L9779 WDA windows, console timeouts - runs
  on it and behaves). rawtrg keeps the NT clock for us-resolution tooth
  deltas (its intra-crank deltas were always correct).
- The RTC (wall clock) drift the console keeps correcting is the separate
  LSI-clocked RTC - expected on this board, unrelated to the NT clock.

Stall analysis (five text + four MLG logs): sparkCut=0 everywhere (no
spark cut), sparkOutOfOrder=0, trgErr=0, no C9xxx errors in the best
runs; coil overcharge warnings every run (dwell 5.5 ms > ~3 ms coil
saturation). Two distinct failure patterns: (a) the basis latches on an
ordinary tooth (gap0 -> 1.0 at ~313 rpm, events off by one tooth = 6 deg,
engine rough-stalls) - the user's "events not on time"; (b) a fully clean
run (gap0 2.0-3.0 the whole time) reaching 969 rpm that still decays and
stalls - events ON time, so fuel/air: sequential injection above
cranking_rpm=500 without cam sync. The Simultaneous injectionMode tune
change (0fe5651b5b1) targets (b); the systime/keep-alive firmware targets
the misleading diagnostics. Re-test discriminates the two.

## 2026-08-21 - m74_9: the "phantom time jump" is REAL elapsed time; MLG time source identified

CORRECTION of the 2026-08-20 entry: the inter-cluster time gaps are not a
clock artifact. Cross-checking the 22:31:06 session (efi_log + the MLG the
user recorded simultaneously) against BOTH independent timebases:

| Evidence | Crank #1 | Crank #2 | Delta |
| --- | --- | --- | --- |
| Console PC wall clock (coil overcharge + "engine stopped" lines) | 22:31:42.0 | 22:32:46.8 | 64.8 s |
| ECU NT clock (synctrace t= column) | 33.9 s | 97.3 s | 63.4 s |

Both clocks agree to the second - the gap between the two sync-event
clusters IS the real pause between two start attempts (engine caught at
622 rpm and died, user tried again a minute later, caught at 969 rpm and
died). The synctrace ring (32 events) simply concatenates all attempts
since boot, which looked like "one crank with time jumps" when pasted.
The WrapAround62 miscount theory from 2026-08-20 is wrong on its face:
the classifier window is 2^30 ticks = 268 s, so a 63 s sampling gap cannot
be misclassified. rawtrg's 1516 s max delta (17:43 log) is likewise the
ring holding edges from previous cranks (span = real idle time).

Where each log's time comes from (user asked; his PC-time guess was right):
- rusEFI_outputChannels_*.mlg = written by the JAVA console
  (SensorLogger -> BinarySensorLog). Record header timestamp =
  System.currentTimeMillis()*100 mod 65536 (PC wall clock), one record
  per ~330 ms output-channels response. The console MLG has NO packedTime
  field: mlg2csv hardcodes the "time" CSV column and reconstructs it from
  the 16-bit record-header ts, so it is always smooth by construction and
  starts at 0 relative to the first record.
- The ECU's own SD-card MLG (writeSdBlock, binary_mlg_logging.cpp) stamps
  packedTime = getTimeNowMs() = the NT clock (TIM5 4 MHz). Different
  producer, different timebase - not what the user's files were.
- synctrace (HEAD): SysTick ms via chVTGetSystemTimeX, plus a new nt= ms
  column (getTimeNowMs) printed in the SAME event so the two timebases can
  be compared inside one crank without any external reference.
- rawtrg: NT clock us deltas (correct for intra-crank tooth deltas).

Added 'timecheck' console command (board_configuration.cpp +
m74_9_tooth_diag.cpp): prints systick_ms + nt_lower_ticks (raw TIM5->CNT) +
nt_ms in one line. Run it twice with a known wall-clock gap - each field
must advance by the same amount. This settles any future timebase dispute
from one paste instead of a cross-log archaeology session.

Status: m74_9 firmware built (00:18 build/rusefi.srec). Not flashed yet:
PCAN re-plug needed (PCAN_ERROR_ILLHW). This build also carries the
reverted L9779 VRS config (b8af57617b0, 617fe9aa6af) which the car has
NOT run yet - the 22:31 session still ran the VRS-commit firmware
("l9779 VRS: stock ramp" lines in the log).

## 2026-08-21 - m74_9: rawtrg printed NT ticks as us/ms (4x); inter-crank gaps are real pauses; key cycle does NOT reset the MCU

USER GROUND TRUTH (recorded): the ignition lock does not allow a second
starter crank without returning the key to OFF first; the user is certain
every ignition cycle boots the firmware fresh ("fresh boot -> synctrace is
empty"). Both true for LONG key-off, both false for the quick OFF->ON->START
flick used between start attempts - proven by the 22:31 session:

| Evidence between the 22:31:42 and 22:32:46 cranks | Says |
| --- | --- |
| synctrace NT clock: 33.9 s -> 97.3 s, no t-reset to 0 | no MCU reset |
| no boot-synthetic sync events (gap0=3.0) between the groups | sync state survived |
| console USB link up the whole time, one connect, one RTC sync | no power loss |
| rawtrg ring holds BOTH cranks in one continuous timestamp stream | RAM + TIM5 survived |

The L9779 SBC on m74_9 keeps VCC up through a quick key cycle (KEY is only
a logic input read back via DIA_REG9 KEY_ON_STATUS for the ETB gate; the
firmware has no key-off shutdown path). Only a longer key-off (seconds+)
collapses VCC - that is the "fresh boot" the user observes between
sessions. Consequence: a synctrace dump always mixes ALL start attempts
of the current boot; the inter-group gaps are the real pauses between
attempts. Average across every dump of 2026-08-20: 14 gaps, min 14.0 s,
max 237 s, mean 65.2 s, median 48.4 s - matches the PC-clock distances
between "engine stopped" lines (e.g. 22:31 session: NT 63.3 s vs PC 64.8 s).

RAWTRG UNITS BUG (root cause of the "phantom time jump"): the rawtrg
dump printed NT ticks (4 MHz, 250 ns) labeled as us/ms - every value 4x
too large. The 22:32:52 header "span 262312.5 ms max 251419151 us" is
really 65.6 s / 62.85 s = exactly the pause between the two cranks. The
morning "span 15598.6 ms" = 3.9 s of cranking at ~272 rpm; the 17:43
"span 1533951.5 ms" = 383.5 s of ring retention across attempts. No clock
ever jumped. Fixed: m74_9RawTriggerDump now divides by
US_TO_NT_MULTIPLIER (span, deltas, histogram).

Collateral note: the noise-storm widths quoted from the old dumps ("50-500
us", "<50 us") were also 4x - real noise bursts are <125 us, sub-12.5 us
ringing included. The RPM-adaptive debounce was tuned against the buggy
prints; it is empirically validated (C9003 fixes) and left unchanged.
The falling/rising-edge duty analysis (38/62 split) is ratio-based and
unaffected by the 4x scale.

## 2026-08-21 - m74_9: systemic AT32 slowdown root cause - flash timing never configured (DIVR)

Symptom chain: engine catches (up to ~850-970 rpm) but dies on the catch with
C9002 (expected 58/0 got 58/0) + coil overcharge x4; the trigger ISR was
measured (lockstats histograms) at ~335 us per tooth (trgPreDecode ~63 us,
trgDecode ~77 us, trgPostDecode ~195 us), i.e. 11-23% CPU at cranking rpm,
overlapping the next tooth edge at ~1.2 ms tooth period -> lost teeth ->
C9002 on the catch. maxLockedDuration=0 (no chSysLock), but systick_ms fell
behind nt_ms by hundreds of ms per crank - the periodic SysTick starved by
the slow EXTI ISR (EXTI prio 0, SysTick prio 8), without any lock.

Flash hypothesis (confirmed by register forensics, not just timing):
- The old ChibiOS AT32 port's stm32_clock_init had the flash setup #if 0'd
  out: flash_clock_divider_set(FLASH_CLOCK_DIV_3) was never compiled - the
  function does not even exist in the fork's minimal Artery headers.
- AT32F435/437 has no FLASH ACR at all. The flash clock divider is
  FLASH->DIVR[1:0] (offset 0x60: 0=/2, 1=/3, 2=/4) and the non-zero-wait
  boost is FLASH->PSR bit 12 (NZW_BST). The fork's FLASH_TypeDef stopped at
  ADDR (0x14), so DIVR was not even addressable.
- Verified the PLL configuration is CORRECT against the official
  AT32F435_437 CMSIS header (ChibiOS-Contrib): PLLMS[3:0]@0, PLLNS[8:0]@6,
  PLLFR@16, PLLRCS@22 - the old port's STM32-style writes land on the right
  bits (PLLMS=1, PLLNS=144, FR=2=div4, RCS=HEXT 8 MHz -> VCO 1152 MHz,
  SYSCLK 288 MHz, HCLK 288, APB 144). Corroborated by working USB 48 MHz,
  CAN 500 kbps flasher and systick/NT agreement - the core is at 288 MHz,
  the slowdown is flash-side, not PLL-side.
- flashperf baseline: PSR=0x00000330, 1M-iteration multiply loop = 808-858k
  NT ticks (202-215 ms) vs the expected ~10-20 ms at 288 MHz with proper
  flash timing -> every instruction fetch pays maximum wait states.

Fixes (ChibiOS fork AT32 port + m74_9 board):

| Change | File |
| --- | --- |
| FLASH_TypeDef extended to official F435/437 layout (DIVR @ 0x60) + FLASH_PSR/FLASH_DIVR bit defs | `firmware/ChibiOS/os/common/ext/Artery/AT32F4xx/at32f435xx.h` |
| stm32_clock_init: DIVR=/3 (flash 96 MHz, RM limit 100 MHz) before PLL switch; NZW_BST off at boot | `firmware/ChibiOS/os/hal/ports/AT32/AT32F4xx/hal_lld.c` |
| flashperf now prints DIVR/FDIV too | `firmware/config/boards/m74_9/board_configuration.cpp` |
| new `flashnzw on|off` console command: live A/B of PSR.NZW_BST without reboot | `firmware/config/boards/m74_9/board_configuration.cpp` |

ChibiOS fork commit: e4d262bbd7 (detached HEAD in the submodule; submodule
pointer bump to be done in the main repo commit).

Validation: m74_9 firmware builds clean (bin/compile.sh -b, 13:40). Hardware
verification PENDING at time of writing: the user's ECU did not answer the
OpenBLT CAN flasher within 30 s (ECU off / PCAN not seated). Planned bench
sequence after flashing:
1. `flashperf` - loop time must drop to ~10-20 ms, DIVR must read FDIV=1.
2. `flashnzw on` / `flashnzw off` + `flashperf` A/B - bake NZW_BST=1 into
   boot if it measurably helps.
3. `lockstats` -> crank -> `lockstats` -> `synctrace` - trigger ISR
   histograms should collapse below ~50 us; no C9002 on the catch.
4. If still slow: add a `clockinfo` command (RCC->CR/CFGR/PLLCFGR dump) and
   audit HICK/HSE/PLLFR against the RM, but PLL forensics above say it is
   already correct.

Update (same day, hardware validation of the flash fix, m74_9):

- DIVR=/3 alone: flashperf 205 -> 136 ms; + CONTR continue-read: 136 ->
  88-95 ms. Total ~2.3x. FDIV=1 and contRead=1 confirmed in the register
  readback. The trigger ISR cost scales accordingly (~335 -> ~145 us/tooth
  estimated at cranking rpm).
- NZW_BST (PSR bit 12) is UNUSABLE on this chip in every phase: live write
  at full HCLK hangs the flash read path (no fault, no crash marker,
  power-cycle recovery), AND so does setting it at boot before the PLL
  switch - the app never starts (console dead, CAN dead, bootloader still
  answers on reset). Reverted; the Artery SDK reference configs never
  enable it either. flashnzw console command is now read-only.
- ELF forensics: the flashperf loop lives at 0x0807118C (~463 KB into the
  668 KB image), i.e. deep in the non-zero-wait flash area - the measured
  ~25 cycles per 7-instruction iteration is the NZW-area fetch cost with
  continue-read, not a ZW-area number. EOPB0 (USD + 0x10) reads 0x5FA, not
  a documented value - the zero-wait area size on this chip is unresolved,
  but even the maximum (512 KB) would not cover the whole image.
- PCAN gotchas observed again: PCAN_ERROR_ILLHW (reseat adapter),
  PCAN_ERROR_INITIALIZE (the open rusEFI Java console holds the adapter -
  kill it before flashing), SET_MTA null once mid-flash (rerun fixed it;
  the bootloader stays in session and reconnects immediately).

Next decision point: crank with the 2.3x faster fetch path and check C9002.
If the catch still dies, the remaining lever is moving the trigger decoder
hot path (and/or the trigger ISR) into RAM (CCM SRAM), not more flash
register twiddling.

## 2026-08-21 - build date now refreshes on every build: console (auto), firmware signature, board .msq stamping + ini_database stale-cache drop

Symptom chain: console showed "Console version 20260818" and the msq
produced "No INI file available for this tune (signature: rusEFI
maccan-tx-fix.2026.08.18.m74_9.319381849). The tune was not opened." after
the console was rebuilt on a later day - UiVersion.CONSOLE_VERSION was a
manually bumped constant and the board default tune (21129.msq) carried the
signature of the firmware build that saved it. A stale msq signature cannot
be resolved: dev-branch inis are not on rusefi.com, and the ini_database
cache is keyed by config hash only, so a rebuild on a later day (same hash,
newer date) leaves a cached ini whose embedded signature diverges.

Changes:

| Change | File |
| --- | --- |
| CONSOLE_VERSION now generated from the build date (YYYYMMDD, Europe/London, same tz as gen_signature.sh) by a new Gradle task; UiVersion delegates to GeneratedUiVersion; the generator only rewrites when the date rolled over, so same-day builds stay incremental | `java_tools/version/build.gradle`, `java_tools/version/src/main/java/com/rusefi/UiVersion.java` |
| gen_config_board.sh stamps every *.msq in the board folder (signature=/firmwareInfo=/bibliography author=) with the just-generated signature, only when changed | `firmware/gen_config_board.sh`, `firmware/bin/stamp_msq_signature.sh` |
| SignatureHelper.downloadIfNotAvailable(p, signature) validates a cache hit against the requested signature and drops the ini_database entry when the embedded signature diverges, falling through to download/manual picker | `java_console/shared_io/.../core/SignatureHelper.java`, `java_console/io/.../binaryprotocol/RealIniFileProvider.java` |
| tests for signature extraction and staleness detection (date diverged, missing line, unreadable) | `java_console/shared_io/src/test/java/com/rusefi/core/SignatureHelperCacheStalenessTest.java` |

Validation: `:shared_io:test` and `:ecu_io:test` green; `:ui:shadowJar`
rebuilt (jar carries GeneratedUiVersion with ConstantValue 20260821);
gen_config_board.sh for m74_9 stamped 21129.msq with
rusEFI maccan-tx-fix.2026.08.21.m74_9.1930129764 / firmwareInfo 20260821
(idempotent second run, xmllint OK). Firmware signature date already
refreshed automatically on plain `make` (SIG_FILE .FORCE + gen_signature.sh
cmp/mv) - the console and msq were the manual/stale parts.

Open follow-ups: none; if the date ever looks stale again, rebuild the
console jar and re-run config gen (no make clean needed on either side).

## 2026-08-21 (same day, follow-up) - macOS bundle was missing the srec/bin: symlink rules used non-relative ln targets

The full bundle zip (artifacts/rusefi_bundle_m74_9.zip) contained no firmware
files at all: zip follows bundle symlinks to store content, but on macOS the
srec/bin/bootloader rules used `ln -fs $< $@` without `-r`, so the links
stored the literal relative target (build/rusefi.srec) - wrong relative to the
bundle folder, unresolvable, silently skipped by zip. The ini/console-jar
links were absolute (the FOLDER_TARGETS rule already used `abspath` on Darwin)
and were stored fine. Fixed: the SREC_TARGET/FIRMWARE_OUTPUTS/
BOOTLOADER_BIN_OUT/dfu/BIN_TARGET rules now use `$(abspath $<)` on Darwin,
mirroring FOLDER_TARGETS (`firmware/bundle.mk`).

Validation: fresh `make bundle` for m74_9 - zip now carries
rusefi_development_260821_m74_9_1930129764_local.bin (702896 bytes),
..._update.srec (2010446), bin/device/openblt_..._local.bin, rusefi_m74_9.ini
and console/rusefi_console.jar; no stale 319381849/yymmdd entries; staging
symlinks resolve through the link. Bundle zip = 57 MB.

## 2026-08-21 - console ini resolution: recursive search + remembered location; bundle tune folder; 21129.msq enum migration

User pain: the console asked for the ini path on every start and on every
tune load ("failed locate to ini"), and loading 21129.msq failed with
"verboseIsoTp: Enum name not found "true"".

Changes:

| Change | File |
| --- | --- |
| IniLocator.findIniFileRecursively(root, signature, maxDepth): descends into subfolders (depth 3, hidden dirs skipped) so an ini unpacked into a nested folder is found without prompting | `java_console/io/.../ini/IniLocator.java` |
| RealIniFileProvider now tries bundled ini -> cwd -> cwd recursive -> bundled-root recursive, and remembers any locally found ini by copying it into ~/.rusEFI/ini_database/<hash>.ini (best effort, once) | `java_console/io/.../binaryprotocol/RealIniFileProvider.java` |
| Cache staleness refined: an entry is dropped only when it carries no signature line or has the SAME config hash with a diverged (newer-date) signature; a DIFFERENT embedded hash is a deliberate manual-picker import and is kept - this is what stops the pick-then-prompt-again loop when the ECU still runs an older firmware than the picked ini | `java_console/shared_io/.../core/SignatureHelper.java` |
| Bundle now ships the board default tunes in tune/ (real copies, not symlinks) so the bundled ini + tune of one build always match | `firmware/bundle.mk` |
| 21129.msq: verboseIsoTp "true" -> "yes" - the only enum mismatch after the config hash change 319381849 -> 1930129764 (old bit labels "false"/"true", new "no"/"yes"); full scan of every msq constant against the new ini shows 0 invalid now | `firmware/config/boards/m74_9/21129.msq` |
| tests: recursive search (found/depth limit/hidden dirs), staleness matrix incl. different-hash-kept | `IniLocatorTest`, `SignatureHelperCacheStalenessTest` |

Validation: :shared_io:test + :ecu_io:test green; console jar rebuilt and a
jshell smoke test against the jar finds a nested ini recursively; bundle
rebuilt - zip now contains tune/21129.msq (verboseIsoTp="yes", signature
2026.08.21.m74_9.1930129764) alongside the ini, srec and bin.

## 2026-08-21 (evening) - console "no data / raw gauge names" scare: ECU-side ISO-TP stall, not the ini changes

User reported the gauges showing raw names (RPMGauge/CLTGauge instead of
titles), no values and no fuel-pump/ETB/main-relay indicators after the
console update. Forensics (console logs + a headless PcanProbe run against
the live ECU with the same console jar):

- The new ini auto-discovery worked: signature read OK, ini found in the
  unzipped bundle folder via the recursive search, model loaded, gauges
  resolve (verified in jshell: getGauge("CLTGauge") -> "Coolant temp").
- The data path worked: PcanProbe (same jar, same cwd) got
  requestOutputChannels=true every ~160 ms with real values (vbatt 12.14 V).
- The actual failure: one mid-response ISO-TP stall on the ECU side -
  "IncomingDataBuffer ... output channels body: timeout 10000ms. Got only
  921 byte(s) while expecting 1029" right at a tune-burn/ECU-reset, then
  the watchdog reconnected. This is the known ECU-side TX stall the branch
  has been fighting (b42a7e99b89 etc.), not the console.
- Recovery: ECU power cycle - graphs and gauges came back immediately.

Follow-ups: (1) verboseIsoTp=yes in the burned tune floods the TS text
channel with one line per CAN frame ("*** INFO: CanTsListener decodeFrame
N") - heavy overhead during any ISO-TP exchange, keep it off unless
diagnosing. (2) SensorGauge now logs "Gauge resolved: <name> channel=...
title=..." at INFO so a future UI-side ini problem is visible in the
console log instead of requiring forensics.

## 2026-08-21 (night) - engine runs; catch C9002, gauges-over-CAN stall and cold idle hunt

The RAM-moved trigger path worked on the car: the engine starts and runs
(lockstats: triggerIsr avg ~38 us, 98% of runs <100 us, vs ~220 us avg and
45% in the 250-500 us bucket before the move; clean sync to 600+ rpm while
cranking, no lost teeth). Three issues remained and were fixed:

1. C9002 "expected 58/0 got 58/0" at EVERY start attempt. Root cause: the
   first-combustion gap acceptance (m74_9 board opt-in) had a ceiling of
   2x crankingRpm, but at the catch the per-tooth INSTANT rpm spikes past
   that (cranking_rpm=500 -> ceiling 1000 rpm; the catch pushes 1050-1250
   instant) - the real gap was rejected at the exact expected position and
   the decoder fired C9002. Raised both acceptance ceilings to 4x; all
   other gates (position, count 58, elapsed-time, real-tooth, 0.8 ratio
   floor) unchanged. Non-fatal before (decoder re-syncs), but each error
   message carries a ~700-byte wave_chart over the same ISO-TP TX as the
   gauges.
2. Gauges dead while the engine runs, power cycle "does not help". The
   logs show the signature/config exchanges fine but output-channels
   responses truncated mid-burst ("Got only 348 byte(s) while expecting
   1029", isotp rate 0) while the ECU's own BCM CAN frames keep flowing.
   Two contributors fixed: the m74_9 BCM emulation (~660 f/s) did not
   yield to serial sessions while the engine was running (now 1/4 rate
   during the 3 s pause; engine-off full mute unchanged, IMMO outside the
   gate), and the SERIAL TX mailbox budget was 1 s (now 5 s - bounded
   because the ECU announce shares the category from a 250 ms periodic
   task). caninfo prints txErrorCount + TEC/REC if the stall returns.
3. Cold idle hunted 530-1100 rpm (pedal 0, fixed tps 4.4-5.0%, timing
   ~22 deg) and stalled. The idle RPM PID was held off for 20 s after
   entering Idling and, at idle tps ~4.7%, the 5% idlePidDeactivationTps
   Threshold attenuated the PID gain ~94% and kept the idle advance table
   (~10-12 deg, flat) blended out - timing followed the swinging main
   table instead. msq: activation 20->4 s, threshold 5->10%,
   pidExtraForLowRpm 0->20%, mapExpAverageAlpha 0.8->0.5.

Validation: m74_9 build clean; unit tests - all 178 trigger/sync tests
pass (the two running-rpm rejection tests moved to 2500 rpm, above the
new 4x band). To verify on the car: flash the srec, burn tune/21129.msq,
then check lockstats/synctrace after a start (no C9002) and the idle PID
(isIdleClosedLoop flag) ~4 s after the catch.

## 2026-08-21 (late night) - high-idle lockout -> 2 Hz idle limit cycle -> narrowband lambda on

Post-burn observations from the binary logs (console MLGs, mapvalue = real kPa):
- 20:17 log: rpm held at ~1650 with tps 8.4%, isidling=0 - the base air
  (cltIdleCorrTable 40 x range 20 = 8.4%) settled the engine ABOVE the idle
  band (target 900 + idlePidRpmUpperLimit 500), so the phase stayed Coasting
  and both idle PIDs stayed off. Fixed by scaling the table 0.55x (warm base
  4.4%). Note: rawmap in the MLG is raw VOLTS (1.8 V at idle); the console
  MAP gauge shows mapvalue - at 1650 rpm/8.4% tps it was a real 51 kPa (the
  user was right, the sensor curve 0.4V=20kPa/4.65V=115kPa checks out).
- 20:22 log: with the air fix burned, rpm oscillated 527-1176 at ~2 Hz.
  Both PIDs were fighting: idleclosedloop +-7.8 of +-20, timingpidcorrection
  +10..-14.6 (saturating), ETB overshooting the command (tps 0.6<->7.1 vs
  commanded 2.8-6.0). Fix: useIdleTimingPidControl=no (air-only idle; the
  20:17 log proved air-only stable) - the timing PID was the troublemaker
  all evening.
- Rich exhaust smell: enabled the narrowband lambda - O2 sensor 1 signal =
  AK3/PF3/EFI_ADC_37 (ADC3 slow path), curve 0.1V->15 AFR / 0.9V->14 AFR,
  fuelClosedLoopCorrectionEnabled=on, heater left off as requested. STFT
  authority +-5% per cell, timeConstant 30 s, minClt 60 C.

## 2026-08-21 (night, in car) - idle: timing-primary regime, dashpot, softened+widened timing PID

Progress and decisions after the 2 Hz limit-cycle diagnosis:
- The 20:22 air-only config was a dead end for the stock-behavior goal: the
  user wants the STOCK architecture - nearly stationary throttle (1-3%),
  RPM held by ignition timing. Re-enabled useIdleTimingPidControl=yes with a
  GENTLE air PID (idleRpmPid p=0.008 i=0.0005) as the slow trim. Steady
  idle ~850-920 rpm with tps ~2.6-2.7% fixed and timing PID ~+-5 deg - the
  timing-primary regime is confirmed correct: angle reacts instantly, the
  throttle barely moves, no manifold delay.
- Timing PID authority widened but made gentler (this commit): maxValue
  12->16, minValue -15->-20, pFactor 0.07->0.05, idleTimingSoftEntryTime
  0->3.0 s. The soft entry ramps error amplification 0->1 over 3 s after
  the crank taper, so a wider min cannot slam entry negative (the earlier
  0.1 pFactor entry slam to -10 deg is documented as a pitfall). p=0.05
  needs a 320 rpm error to reach the -16 limit - swings stay small, limits
  only exist as headroom.
- Throttle-release stall/oscillation: enabled the iacByTps dashpot -
  iacByTpsDecayTime 4.0 s, iacByTpsHoldTime 1.5 s, iacByTpsTaper 8%. This
  is the "catch at ~1500 and glide down" behavior the user asked for.
- idleVeTable restored to 38-50 (the 32-43 step with the big lean-out
  caused stall on the drive test - do NOT re-apply it in one step; the fuel
  model is not yet calibrated so lean-out must move in small increments).
- Lambda: afr_type=Custom, 0.1V->15 AFR, 0.9V->14 AFR on PF3 (AK3). Sensor
  0258006537 confirmed present and honest once the exhaust is hot (PF3
  drops from the 6.4 V rail to ~1.2-1.6 V after a drive; cools back up at
  idle without heater). Cold-idle afrvalue is rail voltage through the
  curve - meaningless; closed loop needs the heater (board edit, not msq).
- Snapshot/bundle propagation is now mandatory for every msq edit: source
  firmware/config/boards/m74_9/21129.msq -> artifacts/rusefi.snapshot.m74_9/
  tune/21129.msq + firmware/rusefi.snapshot.m74_9/tune/21129.msq, then
  'cd artifacts && zip -q rusefi_bundle_m74_9.zip
  rusefi.snapshot.m74_9/tune/21129.msq'. The snapshot dirs and the zip are
  NOT git-tracked - the source msq is the only committed copy.

Validation: m74_9 board build clean (compile_m74_9.sh), unit tests - all
178 trigger/sync tests pass. On-car validation: steady warm idle logged at
~850-920 rpm with timing PID active and tps ~2.6-2.7%; the widened PID,
soft entry and dashpot from this commit still need a drive check.

Open follow-ups: warm idle still sits 50-100 rpm below target with timing
pinned at +10 (the +16 max now gives headroom - verify the droop is gone);
step the idleVeTable lean-out in SMALL steps once lambda closed loop works;
heater (L9779 OUT6=PG6 -> HEAT_OX_1/AC4) requires board config + rebuild,
not msq.

## 2026-08-21 (night, in car) - cranking_rpm 700->1200 for the post-catch flare

The "vzhuh to ~1500" was impossible with cranking_rpm=700: the engine left
the Cranking phase right at the catch (~700), so the 4% cranking ETB
position (cltCrankingCorr 20 x range 20) never had a chance to rev the
engine - the after-cranking taper pulled the throttle toward idle base
immediately. cranking_rpm=1200 (pure msq) keeps the engine in cranking
fuel + cranking timing + 4% position until 1200 rpm, then
afterCrankingIACtaperDuration (30 warm cycles ~3 s) glides to the 913
idle target; the idle target itself is untouched. Timing interpolates from
crankingAdvance 10 deg to full running advance across the rev-up, which
adds a natural push near 1200. Trigger first-combustion ceiling is
4x cranking_rpm = 4800 rpm - a ~1500 flare is far from it.
Risk to watch: if the engine cannot reach 1200 (cold, weak battery) it
stays in cranking mode indefinitely (cranking PW at 1000+ rpm, idle PIDs
off). Next levers if the flare is too small: warm cltCrankingCorr
20/18/16 -> ~24/22/20; if the settle is too fast: warm taper 30 -> 40-50
cycles.

## 2026-08-21 (night, in car) - idle transition speed after the 1400 cranking catch

cranking_rpm=1400 (burned in TS, synced into msq) gives the wanted flare:
the engine holds the 4% cranking position until 1400 rpm. The post-catch
settle has two serial ramps, both pure msq:
  1. CrankToIdleTaper: throttle walks 4% -> idle base over
     afterCrankingIACtaperDuration ENGINE CYCLES (taper fraction =
     revolutionCounterSinceStart / duration). Warm cells 20 -> 15 cycles
     (~1.3 s at 1400).
  2. Idling entry: idle target ramps idle+idlePidRpmUpperLimit (913+500)
     -> 913 over idleReturnTargetRampDuration, now 1.5 s (was 3.0). Air
     PID holds off 1 s (idlePidActivationTime), timing PID ramps in over
     3 s (idleTimingSoftEntryTime) - total settle ~2.5-3 s.
Known trap (22:20 stall): revolutionCounterSinceStart resets only in
setStopSpinning (no-trigger timeout or explicit stop). A quick re-crank
after a stall inherits the old counter, the taper fraction is already ~1
at the catch, the throttle dumps to idle base instantly and the engine
stalls again. A SHORTER taper duration worsens this; if recrank-stalls
appear, lengthen the warm taper cells back or force a key-off between
attempts.

## 2026-08-21 (night, in car) - cranking_rpm=1400 rejected: flare must come from the taper, not cranking mode

Two-start-log evidence (22:25): cranking_rpm=1400 is unreachable by the
warm engine. In cranking phase the ignition is interpolation-capped
(crankingAdvance 10 deg -> full running advance only AT cranking_rpm, so
~16 deg at 1100) and fuel is the fixed cranking PW - the engine tops at
~1115-1470 rpm. Attempt 1 crossed 1400 for a moment: phase flipped to
CrankToIdleTaper, the (racing) cycle counter had already consumed the 15
cycle taper, the throttle dumped to idle base in 0.2 s, rpm fell back
under 1400 into cranking fuel and the engine died. Attempt 2 never crossed
1400: phase stayed Cranking forever, engine idled ~900 on the fixed 3.4%
cranking throttle with zero idle control - the audible 'idling on the
throttle'. Fix (msq): cranking_rpm 700 (clean one-way exit at the catch),
taper warm cells 30 (flare = cranking position held by the taper; longer
budget absorbs the ~2x counter race), cltCrankingCorr warm 22/20/18.
Also note: the taper cycle counter (revolutionCounterSinceStart /
afterCrankingIACtaperDuration) runs ahead of real cycles on m74_9 (up to
~2.7x during the catch surge, seen in the 22:25a log) - treat taper
durations as roughly half their nominal value when tuning.

## 2026-08-21 (night, in car) - flare works, fall-through fixed: idle advance table + base air

22:33 logs: with cranking_rpm=700 the flare is back (catch 1394-1466,
instant 1522-1528) but the engine fell through to 292-412 rpm and then
hung at 550-700, never reaching 900. Root causes, both visible in the
log:
- useSeparateAdvanceForIdle + useSeparateIdleTablesForCrankingTaper are
  BOTH enabled, so the whole CrankToIdleTaper phase (fuel AND timing)
  comes from the idle tables. idleAdvance had 9-10 deg in the 1100-1600
  band -> ~7-8 deg effective at the catch (minus IAT corr ~-2) - far too
  retarded to hold the falling rpm, hence the 292 rpm near-stall.
- warm cltIdleCorrTable base 10 units = 2.0% tps: the gentle air PID
  (+0.2-0.4%) plus the timing PID pinned at +16 (26 deg total) could not
  lift 600 -> 900.
Fix: idleAdvance 17 deg at 700-900, 11-15 deg at 1100-1600 (timing PID
modulates around it; min -20 gives retard headroom), cltIdleCorrTable
warm 14/13/13 (2.6% base) so the PIDs trim instead of fighting a deficit.
Note for later: 2.6% is still above the stock 1-1.5% target - the rich
mixture demands the extra air; drop the base only AFTER lambda closed
loop is live and the mixture is honest.

## 2026-08-21 (night, in car) - the cranking_rpm flap: engine reached idle then fell back into cranking

22:38/22:39 logs: the engine reached idle (937-1074) but oscillated
480-1074 and repeatedly re-entered cranking mode. cranking_rpm=700 sat
INSIDE the idle band: every dip below 700 switched fuel to the cranking
path (useRunningMathForCranking=Fuel Map x warm coef 0.65-0.72 = ~30%
less fuel than running) and cranking timing interpolation - a relay
oscillation around the threshold. Second amplifier: the taper cycle
counter runs 3-5x fast in the catch band (the early-gap and
sync-by-position acceptance paths each re-validate and add increments),
so the 30-cycle taper collapses in ~0.6 s, dumps the throttle and digs
the dip below the threshold. msq fix: cranking_rpm 600 (below the steady
dip range 620-660; 4x trigger acceptance ceiling 2400 vs observed 1726
catch surge), taper warm 50 (~1.5 s real), warm crankingFuelCoef
0.9/0.9/0.85 (smaller fuel step if crossed). The taper counter race is
a firmware bug to fix later (count one increment per REAL revolution);
until then treat afterCrankingIACtaperDuration as ~2x its nominal value
when tuning.

## 2026-08-21 (night, in car) - post-start settle: first stable idle reached, entry slam damped

22:43 log: first time the full chain works end to end - catch 1670,
taper dip only 545 (the 50-cycle budget absorbed the recrank
pre-accumulation), recovery, and a stable 810-890 idle with tps
2.9-3.2% and timing correction modulating +2-6. Two remaining issues:
- the first ~5 s oscillate 556->1062->602->926: the timing PID enters
  saturated (+16 from the +500 return-target error, error 770 rpm),
  overshoots, then the -6.7 retard overshoots the other way. Note the
  idleTimingSoftEntryTime ramp did NOT engage on this start - it keys
  off m_crankTaperEndTime which is only initialized once per power
  cycle (the engine had been started before this log), so on any
  re-start after a stall the soft entry is already expired. msq lever
  used instead: pFactor 0.05->0.03.
- steady state sat 50-90 below target with a permanent +2-6 timing
  bias: the 2.65% base air was short of the ~3.0% the engine needs
  (rich mixture), so the air PID crept +2.3. Base raised to 15/15/14.
Follow-up when allowed to touch code: reset m_crankTaperEndTime /
m_idleTimingSoftEntryEndTime on engine stop so the soft entry works on
every start, and make the taper counter count real revolutions.

## 2026-08-21 (night, in car) - controlled 2-3s descent, PXH-style release catch, iFactor restore message explained

22:48 log: catch 1654 then free-fall to 627 in 0.36 s - the throttle was
still 3.2-3.5%, so the fall was carried by nothing: during
CrankToIdleTaper the timing comes from the idleAdvance table
(useSeparateAdvanceForIdle), whose 1100-2000 cells were 8-12 deg
effective. Raised those cells to 18/17/16/14 (700-900 steady cells
untouched) so the engine holds rpm during the descent and the throttle
walk controls the rate; taper warm 80 (~2.5-3 s real walk with the
counter race), idleReturnTargetRampDuration 2.5 s for the final
1400->905 glide - the stock-PXH profile. Throttle release: dashpot
iacByTpsTaper 10 (2.0% extra air on top of 3.0% base), decay 5 s, hold
1.5 s - catch ~1500 and glide down. ETB always adds the idle position
(including dashpot) via interpolateClamped(0, idleAddition, 100, 100,
pedalTarget), so the dashpot reaches the throttle on release even in
Coasting phase.

The recurring 'To restore previous calibrations ... idleRpmPid_iFactor
0.0005' message: the generic firmware default (engine_configuration.cpp)
is idleRpmPid.iFactor=0.002, the tune has 0.0005, so every flash the
calibration restore prints the line while writing the CORRECT 0.0005
back. Harmless by design; a board-default override was offered and the
user declined - the message will keep appearing on every flash.

## 2026-08-21 (night, in car) - DFCO on: overrun fuel cut above 1500, resume at 1450

The dashpot-only coast-down was rejected by the user: it decelerates
with fuel across the whole rpm range, stock behavior is fuel cut on
overrun. Enabled DFCO in the tune: coastingFuelCutRpmHigh 1500 /
RpmLow 1450 (hysteresis), Tps 5, Map 60 (cut only in vacuum), Clt 60
(warm only), VSS 0/0 unused. Resume smoothness: dfcoRetardDeg 10 retards
timing during cut and ramps it back in over 0.5 s after fuel restore
(DfcoController::getTimingRetard), then the dashpot (base 3.0% + 2.0%
for 1.5 s, 5 s decay) catches ~1500 and glides to idle. Note: the msq
already carried a coastingFuelCut block (disabled/Map 30/RpmLow 1300/Tps
2) - a second block added by hand would duplicate fields; always update
the existing block in place.

## 2026-08-21 (night, in car) - post-start rpm hold: taper + target-ramp as a two-stage profile

23:12 log: the catch flare is not held - rpm fell 1459 -> 588 in 0.6 s
(dip below cranking_rpm 600 re-enters cranking timing/fuel, deepening
the dip), because the taper-phase timing (idleAdvance table) gave only
13-15 deg effective and the throttle walked to base in ~2 s. The
requested profile (hold ~1300 for ~5 s, then descend) is built from two
msq levers: a long afterCrankingIACtaperDuration (120 warm cycles ~
2.5-3.5 s real with the 1.5-2x counter race) keeps the throttle near the
cranking position through the flare, and idleReturnTargetRampDuration
5.0 s makes the idle TARGET glide 1402 -> 905 over 5 s once Idling
starts - the timing/air PIDs then follow the moving target down.
idleAdvance 1100-1600 raised to 20/19/18 so the engine holds rpm during
the taper (also keeps it from crossing back under 600). The profile is
approximate because the taper counter race is firmware-side; a code fix
(count real revolutions) would make the timing exact.

## 2026-08-21 (night, in car) - the start-flare dip was DFCO, not the taper

23:15 log: the catch flares to 1611 rpm which crosses
coastingFuelCutRpmHigh=1500 - DFCO cut the fuel AND applied its 10 deg
retard right on the flare (timing dropped to 4.88 deg), collapsing
1611 -> 660. DfcoController::getTimingRetard applies the retard whenever
the DFCO STATE is true, it is not gated by dfcoDelay - so the delay
alone cannot protect the flare. msq fix: dfcoDelay 2.0 s (cut only after
2 s of sustained overrun; the flare is shorter), dfcoRetardDeg 0 (no
retard to cripple the catch). Hold extended: taper warm 200 (~6 s real
with the counter race) + idleReturnTargetRampDuration 5 s -> ~10 s
catch-to-idle. Cleanest fix is firmware: gate DFCO on idle phase
(skip during Cranking/CrankToIdleTaper), then 1500/1450 and the retard
ramp can return as spec'd. Also note: any DFCO retard hurts the start
flare whenever RpmHigh sits below the flare peak (~1600-1700 on this
engine) - keep that in mind when re-tuning DFCO.

## 2026-08-21 (night, in car) - post-catch fall: timing arrest + MAP EMA speed

23:20 log: the taper hold works (1030-1100 rpm for ~5 s at 3.4-3.7%
throttle) but the first 0.3 s after the catch still fall 1608 -> 920 ->
600. In CrankToIdleTaper there is no PID; the only rpm control is the
idleAdvance table - 13-15 deg effective at 600-1600 rpm (minus IAT corr)
cannot arrest the fall. Raised idleAdvance low cells to 18/21/21
(400/700/900) and 22/21/20 (1100/1300/1600) - the descent should now
stop around 1100-1300. Secondary: mapExpAverageAlpha 0.5 -> 0.85 - the
windowed-minimum sampling already rejects pulse noise so the EMA can be
fast; MAP (fuel/load axis) then tracks within ~1 cycle. The idleVeTable
is nearly flat (45-48) in this region so the MAP lag was only a few %
of fuel - timing was the real arrestor. Note the user's 'hold right
after idle entry, then lower' is exactly idleReturnTargetRamp (5 s,
target 1402 -> 905) - it engages only AFTER the taper completes.

## 2026-08-21 (night, in car) - the taper->idle handoff hole: PID hold-offs are the hidden transition timers

23:25 log: after the taper hold (1000-1120 rpm for ~11 s) a SECOND dip
appears at the taper->Idling handoff (955 -> 743). Cause: two
deliberate PID hold-offs act exactly there - idleTimingSoftEntryTime
3.0 s (timing PID error amplification 0->1, correction stayed 0.0 in
the log while the target was already 1407) and idlePidActivationTime
1.0 s (air PID off). The user's 'transition timer' intuition was right;
cranking itself ends by rpm (cranking_rpm), the cycle-based timer is
the taper, but the handoff killers were these two. Set soft entry 1.0 s
(safe with p=0.03 - the 3 s originally protected a 0.1 pFactor slam),
air PID hold 0.5 s, idleAdvance 400/700 to 20/23 for a higher arrest of
the catch undershoot (665 rpm bottom). Remaining profile after this
change: catch ~1700 -> brief undershoot ~800 -> taper hold ~1100-1200
-> PIDs engage in 0.5-1 s -> target glide 1407->905 over 5 s.

## 2026-08-21 (night, in car) - dip removal: raise the post-catch equilibrium

23:30 log: dip bottom 632 (was 588-665), taper hold ~1000 rpm solid.
The remaining undershoot is physical: the catch flare (1736) decays
toward the equilibrium that the 3.8% cranking throttle holds (~1000),
overshooting to 632 on the way. Fix by raising the equilibrium itself:
cltCrankingCorr warm 26/24/22 (4.4-4.6% cranking throttle -> flare and
undershoot center around ~1300), idleAdvance 1600/2000 -> 22/18 (less
torque deficit at the flare top). DFCO is safe: the 2 s delay + zero
retard cannot touch a flare of any height. If the flare now sits too
high (>1800), lower cltCrankingCorr in 1-unit steps; if the dip
persists below ~1000, next lever is the fuel transition (cranking coef
step at the catch) or a firmware ASE.

## 2026-08-21 (night, in car) - transition shortening + why the catch dips

Phase chain (user asked): Cranking (rpm < cranking_rpm 600) ->
CrankToIdleTaper (pure open loop: throttle walk + idle tables, no PIDs)
-> Idling (open-loop base + closed-loop PIDs). The transition duration
= afterCrankingIACtaperDuration; 200 warm cycles x the 1.5-2x counter
race kept the engine in open loop for ~6-9 s. Shortened to 60 (~2-3 s
real) so the PIDs and the 5 s idleReturnTargetRamp take over quickly.
Why the rpm dips after the catch (three stacked causes): (1) the flare
overshoots the equilibrium the cranking throttle can hold (~1300 at
4.4-4.6%, was ~1000 at 3.8%) and undershoots it on the way down -
pure mechanics; (2) the fuel steps from cranking PW to running VE at
the catch, a momentary torque hole; (3) the timing is lowest right at
the flare peak. (1) is addressed by raising cltCrankingCorr; (2)/(3)
are next if the dip persists (fuel step at catch, higher 1600-2000
advance cells).

## 2026-08-21 (night, in car) - taper anchored to the first catch

Counter math from the 23:42 log: taper counter ~13 at the first catch
(3.43 s), taper (60) ended at 4.95 s -> 1.5 s of open-loop descent,
which is exactly where the dip bottomed (612 at 3.78 s). Warm taper set
to 15 so the fraction reaches 1 at the catch: Idling + the 1407 target
ramp + timing PID take over immediately and catch the flare from above.
Side benefit: the recrank taper-exhaustion stall class disappears - the
PIDs own the catch regardless of the counter. The remaining fuel step
at the catch (cranking PW -> running VE) is still in the pocket if the
dip persists: raise warm crankingFuelCoef toward 1.0.

## 2026-08-21 (night, in car) - the catch handoff stall: PIDs had no positive error to act on

23:47 log: taper=15 stalled the engine. At the handoff the throttle
snapped 4.6% (cranking) -> 3.0% (idle base) while rpm was 1429; the
engine needed ~4.6% there (~1.6% missing). The PIDs could not help
because the return-target ramp started at only 1403
(idlePidRpmUpperLimit=500) - rpm was ABOVE the target, so both PIDs saw
zero/negative error and added nothing; the engine fell through to 427
and died. Fix: idlePidRpmUpperLimit 700 (ramp starts 1603, above the
typical flare -> positive error at the handoff -> catch from below),
air PID p/i 0.01/0.0008, timing soft entry 0.5 s, taper 30 (short walk,
no instant snap). Lesson: a closed-loop catch from ABOVE (rpm > target)
is weak in rusEFI idle code because the open-loop base is calibrated
for the target rpm; the ramp target must sit above the transient rpm
for the PIDs to add air/timing during the descent.

## 2026-08-22 (night) - ECU-side ISO-TP TX stall finally closed (silent reconnect / gauges dead loop)

The user's 'console silently reconnects, no data, binary log resets' is
the branch's long-running ECU-side ISO-TP TX stall (b42a7e99b89 lineage).
Three remaining holes found in the firmware and closed:

1. can_msg_tx.cpp: the serial send happens in the CanTxMessage destructor,
   its result never reached the ISO-TP layer, and there was a single 5s
   mailbox attempt - a dropped frame silently truncated the multi-frame
   response ('Got only N bytes while expecting M'). Now: 3 x 500ms retry
   attempts; on persistent failure canTryAbortX (ABRQ) clears all 3 TX
   mailboxes so an un-ACKed frame cannot wedge the bus forever.
2. isotp.cpp: the flow-control wait (1s) could expire while the FIRST
   frame was still delayed in the mailbox (up to 5s) - the host's FC
   arrived late, the consecutive frames never went out. FC wait now has
   +2s slack.
3. isotp.h: CAN_SERIAL_PAUSE_MS 3000 -> 10000. The 3s window lapsed
   during the console's 10s watchdog/reconnect cycle, the BCM flood
   resumed at full rate and contended with the first post-reconnect
   response - every reconnect died the same way. 10s keeps the flood at
   1/4 rate through the whole reconnect window.

Validation: m74_9 build clean, 45 CAN unit tests + 190 trigger tests
pass. The console's own reconnect already re-runs connectAndRead
Configuration and restarts the pull thread (startPullThread on connect),
so with the ECU stall gone the link should stay up continuously; the
binary-log file reset on reconnect remains a console-side follow-up if
ever needed. Flash the new srec and watch isotpinfo: fcWaitTimeout and
canWriteNotOk should stop growing.

## 2026-08-22 (night) - bench flash over CAN: stock-rollback + ISO-TP fixes on the ECU

Flashed the m74_9 via openblt_can.sh (PCAN, XCP over CAN 500 kbps):
rusefi_development_260821_m74_9_1930129764_local_update.srec, 669628
bytes, checksum verified, ECU reset into the new firmware. The flash
carries both this night's changes: the trigger stock-rollback (all
board sync-validation switches off) and the ISO-TP TX stall fixes
(serial frame retries + mailbox abort, FC-wait slack, 10s flood pause).
Next: console connect, burn tune/21129.msq, then a start - watch
synctrace for C900x/backfires (stock decoder test) and isotpinfo
counters (should stay at zero).

## 2026-08-22 (day) - m74_9 idle/trigger marathon: VRS ramp per start, ETB bias, gap windows

Full car-tuning session (Lada 21129, m74_9, console over CAN). Landed
commits e1d82f4a7e1 and 2b024763f11, flashed twice via openblt_can.sh.

### Trigger: gap window top 3.9 -> 4.5 (measured, not guessed)
- synctrace 14:15 (clean run to 1804 rpm): gap0 = 2.19-2.63, gap1 =
  0.97-1.07 - windows [1.6,3.9]/[0.85,1.35] hold fine in a clean run.
- 14:09 failed run: trgtriggersyncgapratio = 4.1 at the catch (the real
  missing-teeth gap stretched by the flare collapse) -> rejected ->
  C9002 -> desync -> sync storm (trgSync 0->20->31 in 0.3 s) -> the
  revolution counter raced -> the idle taper collapsed -> stall.
- Low side 1.6 and gap1 stay: the 1.60/1.20 false pair and the noise
  false-sync are the load-bearing cases.

### VRS ramp: re-arm per start, debounced (the user's diagnosis, confirmed)
- The L9779 VRS config is write-only and survives a quick key cycle
  (SBC holds VCC): the ramp END (max hysteresis floor) from the previous
  run swallowed the next cranking's low-amplitude teeth -> C9002 at the
  catch. Fixed: re-write ramp START when the engine is stopped OR the
  ignition key goes off.
- First version used rpm==0 as the stop trigger: during a trigger storm
  the rpm sensor flaps 0/300+ at ~1 kHz and the re-arm toggled the VRS
  config start/end at the same rate (800 SPI frames in 1.6 s, 14:34
  log), saturating SPI and feeding the storm. Debounced: isStopped()
  state + 500 ms minimum stopped time.

### ETB: negative bias zone was killing the idle (root cause of 'only range 50 works')
- On this board positive duty = open (the pedal works through +21 bias).
- The old bias curve was -17..-20 duty at 0-4% targets - exactly the
  idle target zone at range 20-30. The idle target got a CLOSING
  feedforward, the blade sat on the physical stop (0.6%) and the engine
  stalled after cranking.
- Range 50 'worked' by accident: the target (5.5%) landed in the
  positive zone, and the blade hung on the limp-home spring equilibrium
  (~6%) while the start-rail integral (-30) kept the net duty negative.
- Fixed: bias 0/8/12/18/20/21/22/25 - no negative zone (0 is the
  physical stop, the blade cannot go negative - the user's point).
- etbIdleThrottleRange: 20 -> 30, calculated: proven-good warm idle
  (12:45 log) = TPS 3.0-3.1% / 960-990 rpm at base 15 x range 20; warm
  base table now 11%, so 3.25 / 11 ~= 30.

### Ignition-off power-stage cut
- LimpManager now clears m_allowEtb(ClearReason::IgnitionOff) when the
  key is off (fuel/spark were already cut); Clearable got restore() so
  the ETB comes back on key-on without touching permanent faults.
- Note: bypassed under directSelfStimulation (bench mode), by design.

### Idle air/fuel tuning (msq, iterated on logs)
- idleVeTable cut 38-50% -> 28-40% (~37% less idle fuel; the Delphi
  28346052 injector spec 150 cc/min @ 3 bar matches injector_flow
  168.8 @ 380 kPa exactly - left alone).
- veTable idle corner (20-45 kPa x 650-1400 rpm) cut 45-72% -> 38-54%
  (the 2x fuel jump when the phase flaps onto the main table).
- Dashpot: iacByTpsHoldTime 1.5->3 s, iacByTpsDecayTime 5->8 s.
- cltIdleCorrTable warm cells 15/15/14/10 -> 11/11/11/9; fan1ExtraIdle
  2 -> 0 (the fan adder stepped in at ~95C and the idle oscillation was
  tied to it).

### Known-open: the 14:39 firmware wedge (priority, not yet fixed)
- Signature: C9007 (tooth 134.8 deg error) + C9002 -> TLE9201 outputs
  disabled -> MFS SD write 2441 ms (status 2) -> 'CRITICAL error: gap in
  time' flood with IDENTICAL now=1845193mS forever. getTimeNowMs() (the
  NT/TIM5 clock) froze: the trigger ISR storm starves the TIM5 ISR, the
  whole angle scheduler dies, the ETB duty freezes (throttle 'not
  released'), ignition-off does not help (the gates live in the dead
  slow loop), only a power cycle recovers.
- Planned fix: efiWatchdog detects a frozen NT clock and issues
  NVIC_SystemReset instead of limping forever. Not yet implemented.

### Validation
- Unit tests: idle_v2 (20), limp (14) green.
- compile_m74_9.sh clean; flashed 669628 and 669804 bytes via
  openblt_can.sh, checksum verified both times.
- Trigger errors (C9002/C9008/DPKV log spam) gone after the gap-top +
  ramp fixes (15:00-15:04 logs).

## 2026-08-22 (evening) - root fix of the 14:39 firmware wedge: defer MFS writes while running

The m74_9 stores ALL persistent data in the ChibiOS MFS on the AT32
internal flash bank 2 (at_start_f435 board_storage.cpp reused). A sector
erase on this silicon stalls the whole CPU (no read-while-erase). The
periodic LTFT save (~5 s, ID 3, 2048 bytes) that hit MFS garbage
collection froze the firmware for ~2.4 s mid-run ('MFS: Write done after
2441 mS MFS status 2') and wedged the NT clock: 'CRITICAL error: gap in
time' flood with identical now=1845193 forever, the angle scheduler dead,
the throttle staying driven, ignition-off not helping, only a power
cycle recovering.

The engine-stopped write deferral existed only for the settings record
under EFI_STORAGE_INT_FLASH=TRUE; on this board INT_FLASH=FALSE (the
settings live in the MFS too), so every write ran un-gated while the
engine ran.

Changes (commit 03c428e6776):
- storage.cpp: storageAllowWriteID consults custom_board_allowFlashNow
  for ALL ids (was: settings-only under INT_FLASH). Deferred writes are
  flushed by the storage manager once the engine stops.
- storage_mfs.cpp: suspendLinearTimeWatcher() around mfsWriteRecord so a
  deferred GC write at engine-off (2.4 s stall) does not trip the
  linear-time watchdog.
- m74_9 board: custom_board_allowFlashNow = isStopped() || self-stim.

Validation: unit tests 1156/1156 green, m74_9 build clean. Flashing was
attempted but the PCAN adapter reported PCAN_ERROR_ILLHW (detached) -
reflash pending the adapter.

Open follow-ups: burn 21129.msq (bias+/range 30/fan 0/gap 4.5), log warm
idle; decide on the noise filter (currently off); if a trigger storm ever
returns, the frozen-NT-clock auto-reset remains an option but should no
longer be needed.

## 2026-08-22 (late evening) - pedalToTpsTable made 1:1

The user asked for the throttle to follow the pedal 1:1 ("14 on the
pedal is 14 at the throttle"). The old table was a row-flat map (any
pedal above zero opened a fixed TPS per rpm row), which made the
throttle feel disconnected from the pedal.

Changes (commit d25713ff6b6):
- 21129.msq pedalToTpsTable: every rpm row is now the diagonal
  0/14/29/43/57/71/86/100 against the same pedal bins, so pedal % =
  TPS % everywhere.
- The 700 rpm row stays all-zero as requested ("zeros at the start"),
  so at the lowest table row the throttle request is still zero.

Deliberately NOT touched: etbBiasValues (it is a feedforward DUTY
curve, not a position map) and the idle path (the idle target still
sums on top of the pedal map via etbIdleAddition, so idle behavior is
unchanged).

Open follow-ups: reflash the storage-deferral build once the PCAN
adapter is reconnected, burn the updated msq, then re-check warm idle
and pedal feel from a fresh binary log.

## 2026-08-22 (night) - ETB bias curve rebuilt from logs: zero at the spring rest

Analyzed the 14:46-15:04 binary logs (tpsvalue / etb1dutycycle /
etb1etbfeedforward). Two findings:

1. The ECU was still running the OLD default bias curve
   (-20/-18/-17/0/20/21/22/25): the logged feedforward values (5.22 at
   target 4.78%, 11.0 at 5.65%, 2.83 at 4.42%, 13.56 at 6.03%)
   interpolate exactly onto that curve, while the new positive curve
   would give ~18.3-18.7 there. So the previous positive-bias msq was
   never burned in (or was burned before the bias edit).

2. Measured static behavior of this throttle: the return spring rests
   the blade at ~6.5% with zero duty (coast). Below that, holding the
   blade needs CLOSING duty: ~-15..-21 at 4.65%, ~-24 at 3.0%, ~-29 at
   the stop. With the old curve feeding +7.3 (opening) at the 5.1%
   idle target, the iTerm was pinned at the -30 rail and the blade hung
   at 6.5-6.8% - idle stuck at 1500-1700 rpm.

Fix (commit 95743edec54): etbBiasValues 0/8/12/18/20/21/22/25 ->
-12/-10/-8/-5/0/20/21/22/25 (bins unchanged 0/1/2/4/7/98/99/100). The
zero crossing now sits at the spring rest (7% bin), a gentle closing
assist below it (not the old deep -17..-20 that could slip the blade to
the stop on the catch), and the proven positive high end is untouched.

Also answered the TLE9201 overcurrent question from the datasheet: the
8A chopper current limitation limits torque but does NOT disable the
outputs (only SC latch / overtemperature / VS undervoltage do), and the
driver only logs DIA changes - it has no path to reset the MCU. No
overcurrent messages in this session's logs.

Open follow-ups: reflash the storage-deferral build once PCAN is back,
burn this msq, log warm idle and check that duty at the ~5% target is
-15..-20 with the integral off the rail, then tune point by point with
bias += (duty - ff) from steady-state logs.

## 2026-08-22 (afternoon) - m74_9 lambda log analysis: AK3/PF3 input is 1:1, divider 2.0 was wrong

User request: find the correct reference voltage for the narrowband
lambda - the displayed voltage was too high (narrowband range is
0.1-0.9 V; mixture on the car was known very rich).

Analysis (logs only, KiCad not trusted for this board):

| Source | PF3 raw | "input" (x2) | Meaning |
| --- | --- | --- | --- |
| bench MLGs 15:00-15:02 (22.08) | 3.23 V | 6.46 V | pull-up rail: sensor cold/disconnected |
| adcdebug efi_log 21_20_17 (21.08) | 3.231 -> 1.479 -> 0.623 -> 0.79-0.95 -> 1.19-1.34 V | - | sensor warming on the running engine, re-catch C9002 at 21:35:21 |
| MLG 13_02_39 (22.08) | min 0.843 V | 1.686 V | hot sensor, very rich mixture |
| MLG 14_46_52 (22.08) | 1.488 -> 0.770 V (still falling) | 1.541 V min | warmup trace, gauge AFR 11.4->13.2 = pure extrapolation, loop inactive (stft 100%) |
| MLG 15_04_25 (22.08) | rising 2.26 -> 2.51 V | 4.5 -> 5.0 V | cooling back up at idle (heater off) |

Conclusion: AK3/PF3/EFI_ADC_37 has NO divider - with a very rich mixture
the sensor saturates at ~0.85-0.9 V and the pin reads exactly 0.77-0.87 V
(a 2:1 divider would read ~0.42-0.45 V, the 1.555 chain ~0.55 V). Correct
reference for the AFR curve: 0.1-0.9 V direct (stoich ~0.45 V, very rich
~0.85-0.9 V). The 2.0 global analogInputDividerCoefficient was applied to
the channel, doubling every reading - the "voltage too high" complaint.
The 6.46 V rail is the input pull-up for open-circuit detection, not a
mixture value. adcdebug also confirms PF3 is the only pin that leaves its
rail with exhaust heat (PF4 switched once and stayed; PF5/6/7/9/10, PC5
flat; PC0/1/3 move only with the pedal).

Fix (this commit): getAnalogInputDividerCoefficient() in
board_configuration.cpp returns 1.0f for EFI_ADC_37 (precedent: s105 does
the same for its AFR channel). MAP/TPS/PPS stay 1.555, everything else
keeps the global 2.0. Board build (compile_m74_9.sh) clean.

Corrections to earlier notes: the O2 heater IS enabled in the tune
(o2heaterPin = L9779_OUT_6, msq line 3202) - "heater left off as
requested" is outdated. m74_9 has no "Narrow Band" afr_type preset, only
Custom (msq curve: 0.1 V -> 15 AFR, 0.9 V -> 14 AFR).

Open follow-ups:
- With divider 1.0 the current curve tops out at 14.0 AFR at full rich, so
  "very rich" displays ~14.0 and a closed-loop target of 13.2 is
  unreachable (STFT enriches to its authority). Curve re-anchor (e.g.
  0.9 V -> 12.5 AFR) is a separate tuning decision - user to confirm.
- There is no voltage-based readiness gate for the analog narrowband
  (FunctionPointerSensor always valid, lambdaCurrentlyGood is the
  protection monitor): closed loop can chase a cold sensor on the 3.23 V
  rail. Needs a voltage threshold (~<1 V) and/or the heater working.

## 2026-08-22 (evening) - bundle build: BUNDLE_DATE now defaults to today

The per-board bundle (console + merged bin + OpenBLT srec + ini + default
tune in one zip) is built with:

  cd firmware
  export PATH="/opt/arm-gnu-toolchain/bin:$PATH"
  bash bin/compile.sh -b config/boards/m74_9/meta-info.env

Output: artifacts/rusefi_bundle_m74_9.zip (rusefi.snapshot.m74_9/ with
console/rusefi_console.jar, rusefi_development_260822_m74_9_1930129764_local.bin
and _update.srec, rusefi_m74_9.ini, tune/21129.msq, bin/device/openblt_*.bin,
updater launchers, drivers).

bundle.mk change: BUNDLE_DATE defaults to $(shell date +%y%m%d) instead of
the literal "yymmdd", so local builds no longer need the env var (CI passes
it explicitly and still wins). First run of the day must clean the stale
rusefi.snapshot.m74_9/ and the old zip before re-zipping - zip -r never
removes obsolete entries, and a stale 11:48 srec without the lambda divider
fix had lingered in the archive from the earlier build.

The zip warnings "name not matched: rusefi.snapshot.m74_9/bin/*" are normal:
bundle.mk symlinks java_console/bin helper scripts with ln -fs (no -r on
macOS), the relative targets dangle and zip skips them - not a deliverable.

This bundle carries the lambda divider fix (getAnalogInputDividerCoefficient
returns 1.0 for EFI_ADC_37): deliver/rusefi.bin 17:09 (702644 bytes),
rusefi_development_260822_*_local_update.srec 17:05 - flash the srec via
OpenBLT to get the fix on the ECU.

## 2026-08-22 (evening) - OpenBLT CAN flashing: 4-5 min -> ~1.5-2 min (host poll fix)

Analysis of the 4-5 min CAN flash of the ~670 KB app: 95.7k PROGRAM_MAX
frames x ~2.75 ms/frame. Per frame: CAN bus 500k request+response ~0.52 ms,
bootloader XCP + 2-3 AT32 word programs ~0.2-0.4 ms, host poll ~1.5-2 ms -
the bottleneck was PcanLink.readFrame's Thread.sleep(1) (MacCAN Read does
not block, so every empty read cost a full 1 ms+). Fixed: 1 ms
Thread.onSpinWait spin + LockSupport.parkNanos(100 us) fallback in
java_console/openblt_can/src/main/java/com/rusefi/openblt/PcanLink.java.
:openblt_can:test + fatJar green, jar rebuilt. Expected total ~1.5-2 min.

Remaining levers (not done): 1 Mbit/s CAN (bootloader efi_blt_ids.h + CanInit
B1MBPS + PCAN_BAUD_1M) - bench-only, the vehicle bus is 500k and the fork has
no XCP baudrate-switch; multi-frame XCP block mode - the real floor of the
single-frame request/response protocol (bxCAN has only 3 RX mailboxes, so
pipelining more than 2-3 frames risks overrun).

## 2026-08-22 (evening) - msq: etbBiasValues 9 values -> 8 (tune load fix)

Console tune load failed with `IllegalStateException: etbBiasValues: 9 values
while expecting 8 by 1 total 8` - the ETB bias array is fixed 8x1
(ETB_BIAS_CURVE_LENGTH=8) but the msq carried 9 values
(-12/-10/-8/-5/0/20/21/22/25), an editing slip when the zero crossing was
moved to the spring-rest bin (7%): the old positive value 20 at bin 7 stayed
as a stray entry. Fixed to -12/-10/-8/-5/0/21/22/25 (bins unchanged
0/1/2/4/7/98/99/100): bins 98/99/100 keep the proven 21/22/25 high end,
zero stays at the spring rest. Scanned the whole msq: every other multiline
array constant now matches its rows/cols declaration. Bundle rebuilt so the
shipped default tune is loadable.

## 2026-08-22 (evening) - root cause of the recurring mid-run stall: the storage deferral gate was DEAD CODE

The engine stalled again at 19:23:48 and 19:36:09 (C9002 "expected 58/0 got
58/0" + C9007, TLE9201 outputs disabled, MFS write 2451 ms status 2) - the
14:39 wedge signature, despite the 15:48 "defer storage writes" fix being
flashed. The user also noticed writes to flash every ~5 s without pressing
burn (LTFT auto-saves).

Root cause: the firmware build passes -DEFI_UNIT_TEST=0 on the command line,
so defined(EFI_UNIT_TEST) is TRUE in EVERY build. The fix's gate was
guarded by `#if EFI_SHAFT_POSITION_INPUT && !defined(EFI_UNIT_TEST)` -
never compiled, in firmware OR unit tests (1156/1156 green could not catch
it). Proven two ways: preprocessing the exact build command (the gate
vanished from storageAllowWriteID) and disassembling the storage manager
loop (straight storageIsIdAvailableForId -> storageWriteID, no gate; exactly
one reference to custom_board_allowFlashNow - the assignment).

Fixes:
- storage.cpp: the all-IDs gate guard is now `!EFI_UNIT_TEST` (value).
  Verified in the rebuilt ELF: the manager loop now consults
  get_board_override_result(custom_board_allowFlashNow) and defers writes
  while the engine runs.
- AT32 mpu_util.cpp: mcuCanFlashWhileRunning() returned true "for dual-bank"
  while the TODO admits the CPU still freezes during a bank-2 write - now
  returns false, so the settings path also takes the deferral gate.
- CLAUDE.md: recorded the -DEFI_UNIT_TEST=0 gotcha (defined() is always true;
  use the value).

Firmware rebuilt (build/rusefi.srec 19:46, deliver/rusefi.bin 19:47) and the
bundle re-zipped. The ECU must be reflashed for the fix to take effect.

## 2026-08-22 (night) - open-loop idle table expanded 2 -> 16 target-RPM bins; flashed

The cltIdleCorrTable had only 2 RPM bins (700 and 7000) with a flat row
(30/26/20/15/11/11/11/9), so the 5 s idleReturnTargetRamp (target+700 ->
target) rode a flat curve: after cranking the blade dropped too fast and the
engine stalled, and a flat value high enough to survive the entry made the
settled idle too high (user: 20 works but idle high, 10 stalls).

Change (commit e70b80fd92a):
- firmware/integration/rusefi_config.txt: CLT_IDLE_TABLE_RPM_SIZE 2 -> 16,
  FLASH_DATA_VERSION 260805 -> 260822 (struct layout change).
- firmware/config/boards/m74_9/21129.msq: rpmIdleCorrBins now 700..2200 step
  100; cltIdleCorrTable now 16x8, warm columns (CLT 60..140) go 8 -> 9.9
  (at 1000 rpm) -> 21.5 (2200 rpm), i.e. +0.9 per 100 rpm; cold columns keep
  the old shape shifted +0.9 per 100 rpm (27..40.5 at 700..2200). The settled
  warm idle (~840 rpm) now requests ~9.3% instead of 11%, while the ramp
  entry (1540 rpm) requests ~16.5% instead of 11% - more air during entry,
  less at settle. This is a first cut; validate on the car.

Validation: bundle rebuilt (artifacts/rusefi_bundle_m74_9.zip 20:15,
signature 1767756910); CAN flash of build/rusefi.srec: 669756 bytes, checksum
verified, 266.3 s; ECU reset into the new firmware.

IMPORTANT follow-up for the user: FLASH_DATA_VERSION changed, so the stored
tune is incompatible with the new firmware - load 21129.msq in TunerStudio
and Burn once before running. Then watch: baseidleposition should ramp down
with the target during the 5 s idleReturnTargetRamp; no MFS "Writing storage
ID 3" lines while running; no C9002/C9007; steady warm idle status near 0.

## 2026-08-22 (late night) - console restart roulette: raw gauge names + missing indicators fixed

User: gauges showed raw key names (RPMGauge/CLTGauge) and the front-page
indicators were missing; restarting the console a few times made values
appear. Logs (artifacts/logs/efi_log_2026-08-22_20_42/20_44_*.log.0) show
every session began with "PrimeTunerStudioCache - .ini file not located"
and "Gauge not found by RPMGauge (no ini model)" at startup, then resolved
only after a connection-status rebuild fired post-connect.

Root cause: PrimeTunerStudioCache.findLocalIniFile searched only the TOP
level of "." and ini_file_path (".."). The console runs from artifacts/
with the ini one level down (artifacts/rusefi.snapshot.m74_9/rusefi_m74_9.ini),
so the startup IniFileModel was null: gauges could not resolve and
TuningPane.buildFrontendIndicatorPanel returned null (the indicator panel
was built once at startup and never again). The connect-time provider
already searched recursively, which is why a restart eventually repaired
the UI via the connection-status rebuild.

Fix (commit 5ff9ae591c6, Java only - no reflash needed):
- findLocalIniFile mirrors the connect-time order: top-level ".", then
  ini_file_path, then both recursively (depth 3, hidden dirs skipped).
- TuningPane hosts the indicator panel in a rebuildable container and
  rebuilds it when connection status becomes connected (safety net for a
  stale or wrong-board startup model).
- IniLocatorTest +2: nested-subfolder fallback, top-level priority.
Validation: :ecu_io IniLocatorTest 10/10, :ui 407 tests 0 failures.
Bundle rebuilt (rusefi_bundle_m74_9.zip 21:05) and the console jar in the
user's unzipped artifacts/rusefi.snapshot.m74_9/console/ replaced.

Also noted (bench): the ~5 s ECU resets remain the known debug behavior of
the bench build (reset-cause label differs between builds) - marked RED in
CLAUDE.md. The ECU-side ISO-TP TX stall is still present (TS_GET_TEXT got
181/338 bytes at 20:48:36) despite 93d404e7e00 - still the open branch bug.

## 2026-08-22 (night) - catch storm: VRS 4-step ramp + time-gated revolution counter

The 20:55 MLGs showed revolutionCounterSinceStart racing ~5x at the catch
(8->37 in 0.2 s vs ~6 real) with ALL board trigger validations already off
(db9c155d820 + f0447b280ce). The race is the stock decoder: once the cycle
anchor drifts mid-revolution, every exact-58-event false sync re-anchors
and ticks the counter -> afterCrankingIACtaperDuration/ASE fast-forward.

Fixes (two commits, firmware build clean, 1157/1157 unit tests):
- 439d6b7efa8: L9779 VRS ramp 2-step latch -> the stock's 4-step script
  (REG5 0x0C->0x0D->0x0E->0x0F, REG4 0x0B->0x0A->0x09->0x08, REG6
  0x07->0x05->0x06), advanced at 150/300/600 rpm (last = cranking_rpm).
  Re-arm per start unchanged.
- d1868f1658c: rpm_calculator rate-limits the counter by real time
  (>= 30/rpm s per revolution, 2x acceleration slack; no rate limit until
  the first cycle RPM is measured). New test
  RpmCalculator.revolutionCounterRateLimitedByTime.

Noted while testing: the host mock NT tick is 10 ns (US_TO_NT_MULTIPLIER
100) and the mock clock starts at 0 - a 'lastNt == 0' sentinel collides
with a real timestamp; use a bool flag instead (see CLAUDE.md).

Firmware rebuilt (firmware/build/rusefi.srec). The ECU still runs the old
build - reflash needed for the VRS ramp + counter gate to take effect.

## 2026-08-22 (late night) - flashed: VRS 4-step ramp + dynamics-aware revolution gate

Flashed build/rusefi.srec (HEAD 54e87e6abae) over CAN: 669988 bytes,
checksum verified, 265.6 s. Contains 439d6b7efa8 (VRS stock 4-step ramp)
and 54e87e6abae (revolution counter time gate with rpmRate-based
prediction). FLASH_DATA_VERSION unchanged (260822) - the stored tune
stays compatible, no msq re-load needed.

Analysis of the catch race (m2055/m2055a): the counter races only in a
~0.2-0.3 s window at the first-combustion catch (~5x), then is exact
(28.6 rev/s vs ~1700 rpm in m2055). No trigger-error flags - the race
goes through clean re-syncs after the cycle anchor drifts (stale VRS
adaptation: Tfilter = 1/32*Tn uses the previous period, 6x too long at
the catch; gap ratio up to 4.1 vs the 3.75 window). The time gate, not
sync validation, is the correct guard.

## 2026-08-22 (night) - counter race resolved; recorded 720-deg semantics; open: catch-then-stall

Post-flash logs (23:32-23:35): the revolution counter now tracks engine
cycles exactly (rpm/120) with only a residual +2-8 cycle spike in the
first 0.1-0.2 s of the catch. Key facts recorded in CLAUDE.md:
- The counter counts 720-deg ENGINE CYCLES (getCrankDivider=2), not crank
  revolutions - afterCrankingIACtaperDuration 20-30 cycles = 1.6-2.4 s at
  1500 rpm.
- The console MLG 'time' axis compresses during the ISO-TP TX stall -
  always time analysis by the ECU-side running_timesincecrankinginsecs
  (23:32:34 showed tSinceCrank=2.3 s at MLG t=+0.3 s).
- Taper now lives ~2.3 s of ECU time (27/30 cycles) instead of ~0.2 s.
- VRS at running rpm is clean: synctrace 32/32 countErr=0.

OPEN for the next session: the car catches well (~1100-1678 rpm) and then
falls/stalls (23:32:34: rpm 0 at tCrk 3.5 s). Suspects: post-catch airflow
(taper blend 21.94 -> 17.78 -> 10.x), idleMode = Open Loop, warm idle
table ~9-10% at 944-960 rpm target.

## 2026-08-23 - TLE9201 blade drop root cause: L9779 WDA watchdog kill chain

Decoded the "Outputs disabled" stall. TLE9201 diag 0x5C = EN=0 = DIS pin
HIGH (datasheet: EN bit reflects the DIS input, not an internal fault;
OT=1, CL=1 = no fault). Only two drivers of DIS on m74_9: PB13 (driven
high once in board init, never again) and the ETC_WD chain L9779 WDA
(pin 38, open-drain low-active) -> Q5B -> DIS.

L9779 datasheet 6.15: WDA goes LOW when the VDA 2.0 watchdog error
counter EC > 4 (also on any L9779 reset - EC starts at 6 - and on
external AB1 low). So the blade drops whenever the watchdog is
unanswered (EC>4) or the chip resets (UV dips).

Proof from the 2026-08-23 00:18-00:30 log, two perfect pairs:
- 00:19:02 / 00:20:13: Diag 5C appears INSIDE an MFS settings write that
  stalled 2379/2360 ms (status 2), and DF returns ~200 ms after the
  write completes. The MFS lives in AT32 internal flash bank 2 (EFLD2);
  a sector erase stalls instruction fetch (no read-while-erase) -> ~20
  watchdog cycles unanswered -> EC>4 -> WDA low -> DIS high -> bridge
  tristate -> spring slams the blade shut -> engine stalls. This is the
  same chain as the 08-22 19:36:09 / 00:22:14 car stalls (MFS write
  2419-2451 ms in the same second as the drop), fixed at the source by
  the storage-deferral gate (periodic LTFT saves deferred while running).
- The 08-23 00:22:14 stall had NO flash write in flight - same actuator,
  different trigger (candidates: VS/ignition dip -> L9779 RST_UV, EMI on
  SPI1 while running, AB1 low). Coil overcharge C9351-4 at the same
  timestamp is a CONSEQUENCE of the rpm collapse (dwell overflow), not a
  cause.

Cranking-time 0x5C clusters (00:19:25, 3 toggles in 1.2 s) = starter
battery sag resetting the L9779 (EC=6 -> WDA low -> DF after two good
answers).

Diagnostic for the next stall: console command 'pins' prints
"l9779 WDA: req=.. ec=.. wda_int=.. ok=.. fail=.." and spi error
counters - run it right after a drop (key still on). wda_int=1/ec>4
confirms the watchdog path; healthy ec means PB13/AB1/hardware.
Fix directions (not implemented yet): forbid TS Burn while running on
this board; explicit warning on EN=0 while running; hardware decision on
the ETC_WD kill chain (Q5B/R20 populated or not - TLE9201 has its own
chopper/OC/OT protections).

## 2026-08-23 - blade-drop fixes: TS burn gate, EN=0 warning with WDA counters, heater verified

Following the WDA kill-chain analysis, three firmware changes (board
compile + 1158 unit tests green):
1. TS Burn forbidden while the engine runs (new board hook
   custom_board_allowTsBurn in board_overrides.h, defined in
   tunerstudio.cpp; m74_9 sets it like custom_board_allowFlashNow:
   allowed only when engine stopped or bench self-stimulation). The
   settings page was already deferred by the storage manager, but the
   extra-page burns (secondary tables, lua) went STRAIGHT to the flash
   from the TS thread - a mid-run burn of page 4/5 would stall the CPU
   and re-trigger the WDA kill chain. Skipped burns print
   "WARNING: TS burn skipped - engine is running (board policy)".
2. TLE9201 driver now logs ONE warning line when the outputs get
   disabled (EN=0, i.e. DIS pin high) while the engine is running,
   merged with the L9779 watchdog counters via the new
   l9779_getWdaCounters() accessor:
   "WARNING: TLE9201 outputs disabled while engine running (DIS pin
   high; l9779 WDA ec=N wda_int=N ok=N fail=N)". ec>4/wda_int=1
   confirms the watchdog-answer starvation; healthy counters mean the
   kill came from elsewhere (PB13/AB1/hardware).
3. O2 heater (point 4) VERIFIED - no code change needed: OUT6 is the
   L9779 LSb low-side driver (5 A, datasheet "R,L Load (Heater)"), so
   it switches to GND by construction. Chain: o2heaterPin L9779_OUT_6
   -> OutputPin::setValue(1) -> gpiochips_writePad -> L9779::writePad
   (driver pin 9) -> palSetPort PG6 -> IN6 -> OUT6 conducts to GND.
   Positive logic, CONTR enable bit latched at boot (o_oe_mask bit 9 ->
   CONTR_REG2 bit 0), nothing in board logic disables it; the only gate
   is forceO2Heating || isRunning() (engine.cpp). Remaining hardware
   unknowns if the heater does not heat: PG6<->IN6 wiring (schematic
   only, not buzzed out) and the +12V feed side of the heater.

## 2026-08-23 - openblt_can host-side rework: blind-spin fix, stale-jar trap, RTT histogram

The 2026-08-22 poll fix did not speed up flashing (265.4 s for 669924
bytes = 2.77 ms/frame, unchanged; the jar DID contain the fix). Two
host-side bugs found and fixed:
- PcanLink.readFrame spun blind for 1 ms between queue checks - a frame
  arriving during the spin was read only after the spin ended, so every
  frame paid ~1.0-1.2 ms host dead time regardless of reply timing.
  Now: tight Read-poll during a 1 ms hot window (checks every few tens
  of us), then 0.1 ms parks for slow replies (erase/connect).
- openblt_can.sh rebuilt the fat jar only "if missing" - a stale jar
  could silently run old code. Now always runs :openblt_can:fatJar
  (gradle no-ops when up to date).
- The flasher prints a per-frame XCP RTT histogram at the end
  (XcpClient.rttStats, <1/1-2/2-3/... ms buckets) so the next run
  settles where the remaining ~2.7 ms/frame goes.

Working hypothesis (firmware-side, documented only - NOT fixed): the
ECU answers each PROGRAM_MAX frame in ~1-2 ms; the bootloader main loop
runs BootTask() tight while connected (no sleep), so it is the CAN RX ->
XCP handler -> 3-word flash program (each AT32 word program stalls
instruction fetch) -> TX response chain plus the MacCAN USB pump
(~1 ms, host floor Java cannot remove). Future levers if needed: 1 Mbit
CAN, block-mode XCP, batching buffered PROGRAM_MAX frames.

## 2026-08-23 - 1 Mbit CAN for OpenBLT/main firmware: analysis, NOT enabling

Asked: can we run 1 Mbit CAN for both OpenBLT and the main firmware.
Answer: NO for the main firmware, NOT worth it for the bootloader.

- Main firmware MUST stay at 500k (tune canBaudRate): the bus speed is
  a property of the whole bus and CAN1 carries live car traffic (BCM
  immobilizer handshake 0x0713/0x0714, ABS, cluster) plus the
  console-over-CAN PCAN link. 1M would put two bit timings on one bus -
  mutual error frames, IMMO/ABS/cluster dead, console-over-CAN dead.
- Bootloader at 1M is technically possible (AT32 bxCAN + PCAN support
  B1MBPS) but bench-only: with ignition on the other car nodes transmit
  at 500k and would wreck the XCP frames. A permanently-1M bootloader
  would also make in-car CAN flashing impossible - would need a separate
  bench build or a custom baudrate-switch command (OpenBLT has no XCP
  baud switch).
- Expected gain is ~8% anyway: wire time per frame 0.45 -> 0.25 ms at
  1M, out of 2.77 ms/frame total; the bottleneck is the per-frame
  request/response, not the bus. Decision deferred until the RTT
  histogram from the next flash settles where the time goes.
- Real levers: block-mode XCP / PROGRAM_MAX batching in the bootloader
  (firmware-side, documented only), or a dedicated CAN2 flashing bus -
  PB5/PB6 are free on m74_9 (PIN5/PIN6, unused) and could be wired to a
  separate connector at any speed without touching the car bus.

## 2026-08-23 - fresh flash: 182.3 s, RTT histogram settles the bottleneck

Flashed 670,272 bytes over CAN (new host fix + RTT histogram, log:
artifacts/logs/openblt_flash_260823_rusefi_srec.log):
- 182.3 s vs 265.4 s before the readFrame rework - the host fix removed
  ~0.9 ms/frame. Verified=true, reset into the new app (blade-drop
  fixes + WDA warning + TS burn gate).
- RTT: 95,802 frames, avg 1.90 ms, max 466.74 ms (erases). Buckets
  <1/1-2/2-3/3-4/4-5/5-7/7-10/>=10 ms: [0, 79592, 11462, 3243, 1414, 68,
  0, 23]. 83% of program frames land in 1-2 ms = ECU processing
  (~0.5-1 ms) + wire (~0.45 ms) + MacCAN USB pump (~0.5-1 ms).
- Conclusion: the bus is no longer the bottleneck. 1 Mbit CAN would save
  only ~0.2 ms of 1.9 (~10%, ~165 s); the real lever is block-mode XCP /
  PROGRAM_MAX batching in the bootloader (firmware-side, documented
  only, not implemented).
- openblt_can.sh now resolves relative file args against the caller's
  cwd (the first 2026-08-23 attempt failed with NoSuchFileException
  because the script cd's into java_console/ before the flasher
  resolves its path).

## 2026-08-23 - cranking blade drops: WDA path ruled out, PB13/3.3V prime suspect

The new warning line fired twice on the car (efi_log_2026-08-23_11_42_36_849)
with HEALTHY watchdog counters both times: ec=4 wda_int=0 ok=1007 fail=0
(11:43:16) and ec=4 wda_int=0 ok=123 fail=0 (11:44:13). ec=4 is the
watchdog's healthy floor, fail=0 since boot -> the EC>4 -> WDA-low ->
ETC_WD kill chain is ruled out for the cranking drops, and so is an
L9779 reset (EC would read 6). The battery recovers to 12.2 V ~2 s
BEFORE the 11:43:16 drop (MLG 11_43_11_631: 10.7 V only at starter
inrush) - a battery sag does not explain it.

DIS high physically requires Q5A to stop conducting (PB13 high holds
DIS low regardless of the 5V rail), so the prime suspect is a 3.3V
MCU-supply dip during cranking dropping PB13's GPIO drive while the
core keeps running; secondary suspect is the Q5B/ETC_WD chain if
populated. The drop is INTERMITTENT - not every cranking produces the
0x5C (user-confirmed): the 11:44:13 event recovered in ~0.8 s right at
the catch (engine started fine - the blade rests on the closed stop
during cranking, so the drop is harmless for starting), the 11:43:16
event showed no DF until the next power cycle (starter held longer /
key-off; the tle9201 diag thread stops polling when ignition is off,
so 'no DF' can be a polling artifact).

Open steps (hardware): buzz Q5B/R20 population; scope PB13 and DIS
during cranking in the car. Firmware option: add PB13 readback +
key_on_status + VREFINT to the warning line.

## 2026-08-23 - high-rpm periodic sync loss root cause: decoder consumed the "disabled" tooth profile

The 12:01-12:02 events (C9002 x4 + one C9007 tooth #114 error 11.7
deg, intervals 23/10/12 s at stable ~1850 rpm, engine jerks and
recovers) were NOT a VRS signal problem - the profile learning was
supposed to be disabled but the decoder still consumed it:
- Disabled: only the load/save housekeeping
  (custom_board_periodicSlowCallback commented out).
- Active: the learner (trigger hook boardTriggerCallback, 3113 revs in
  RAM) AND the consumer - trigger_decoder.cpp isSyncPoint() applies
  triggerGetToothProfileFactor at rpm >= 2*cranking (1200) via the
  useProfile gate. The setup comment "without the periodic load the
  factors stay 1.0" was wrong.
- Effect at stable high rpm: smooth teeth (real jitter ~1%) divided by
  cranking-learned factors (0.79..1.51x compression shape) = fake
  ripple in the sync ratio -> periodic false sync decisions -> C9002 +
  phase jump (C9007, 11.7 deg = 2 slots) + coil recharge jerks at
  constant rpm, clean re-sync a revolution later. Matches the observed
  smooth->jerk->smooth cycle exactly.
- Fix: m74_9 triggerGetToothProfileFactor now returns 1.0
  unconditionally - the decoder compares raw durations; the learner
  keeps feeding toothdump/rawtrg diagnostics only. Board build green,
  1158 unit tests green.

## 2026-08-23 - engine died at 12:20:00: WDA watchdog trip (ec=5, wda_int=1), diagnostics extended

Session 12:19:21 log: the car started and ran, then the L9779 "switched
off" at 12:20:00 - this time the WDA path tripped for real:
12:20:00.326 drop with ec=4 wda_int=0 (the healthy-watchdog DIS glitch
class), then ~200 ms later a second drop with ec=5 wda_int=1 -> WDA pin
low -> L9779 disabled OUT1-4/IGN1-4 (ignition cut = "l9779 turned off")
+ ETC_WD killed the ETB -> engine stopped 12:20:01.9. No power sag
involved - it is a watchdog TIMING trip: an answer landed outside the
~12.6 ms window and the EC climbed 4->5. fail=0 was misleading - it
only counts SPI-level failures; timing misses (REQUHI RESP_TO_EARLY /
NO_RESP flags) silently increment the EC.

Diagnostics extended (built + 1158 tests green):
- l9779 driver counts timing misses separately (wd_timing_miss_cnt) and
  caches the DIA_REG10 byte (CRK_RST=0x20 short-VDD-UV reset,
  V3V3_UV=0x04 3.3V regulator UV, OV_RST, VDD5_OV, TNL_RST, F1/F2) -
  refreshed every 100 ms; the existing "OUT_DIS set! DIA10=0xNN" print
  captures the flags for every event.
- The TLE9201 drop warning now prints miss=N and dia10=0xNN, so the
  next event shows BOTH whether the watchdog answers were late and
  whether the chip saw a power event (CRK_RST/V3V3_UV).

## 2026-08-23 - executor lateness instrumentation (user report: rpm floats, misfires, bad tooth counting at stable high rpm)

User symptom: at stable high rpm the rpm starts to float, misfires
begin, tooth counting degrades. Question: does the L9779 act late, or
do we send the commands late? The output switching itself is direct
(parallel pins PF12-15 / PE8-11 via palSetPort from the scheduler ISR,
SPI enable bits latched at boot - no float there); the float can only
come from the executor dispatch (TIM5 ISR) or from upstream (trigger
edge timing feeding the angle->time conversion).

Instrumented the dispatch: EventQueue::executeOne now reports the
scheduled moment and the actual start time; SingleTimerExecutor
accumulates lateness stats (maxLateNt, events >= 10 us late, 7-bucket
histogram <1/1-4/4-16/16-64/64-256/256-1k/>=1k us). The m74_9
lockstats command prints and resets them:
  sched exec=N late>=10us=N maxLateUs=N hist<1/...>=...
A clean histogram (<4 us) proves the executor is deterministic and the
jitter is upstream (trigger edges); a fat >=16 us bucket means the
dispatch floats (IRQ locks / long ISRs). The coil overcharge 6.06-6.45
ms at the events is the spark turn-off firing late or being cancelled
by the sync loss - the stats will show which.

## 2026-08-23 - ROOT CAUSE + fix: TIM5 executor ran at priority 7 (LLD default), below the trigger handoff and the ADC

The lockstats runs confirmed the float: at stable 1887 rpm 11-30% of all
scheduled commands went out >=10 us late, worst 389 us (bench with the
CAN console streaming: 11% late even with the engine off). The trigger
ISR histograms showed the trigger handoff (priority 3) at 44 us average
with 100-1000 us tails, ~2200/s at 1887 rpm - and the executor was
BELOW it.

Root cause: the AT32 mcuconf defines no STM32_PWM_TIM5_IRQ_PRIORITY,
and the shared STM32 TIMv1 LLD defaults it to 7. On the car the
actual layout was: raw EXTI capture 0 (fast IRQ, correct), trigger
handoff decode 3, ADC 6, **executor (TIM5 CC1) 7**, SysTick 8, CAN 11.
So EVERY trigger decode (44 us avg, tails to ~1 ms) and every ADC
completion delayed the spark/dwell/injection dispatch - the measured
floating command execution. (The EFI_IRQ_SCHEDULING_TIMER_PRIORITY=4
define existed but was never consumed: the LLD reads
STM32_PWM_TIM5_IRQ_PRIORITY, not the EFI define; assertInterruptPriority
is a no-op stub on the AT32 port, so nothing caught it.)

The fix (flashed 13:35, verified 671020 bytes):

| Change | File |
| --- | --- |
| Executor now the highest-priority kernel IRQ: EFI_IRQ_SCHEDULING_TIMER_PRIORITY 3 + STM32_PWM_TIM5_IRQ_PRIORITY defined from it; trigger handoff moved to 4 | `firmware/hw_layer/ports/at32/interrupt_priority.h` |
| schedule() no longer runs due events inline from ISR context (unit tests/simulator keep inline execution) | `firmware/controllers/system/timer/single_timer_executor.cpp` |
| Per-command-class lateness stats (dwell/spark/overdwell/fuel/other), printed by lockstats | `single_timer_executor.cpp/.h`, `firmware/config/boards/m74_9/board_configuration.cpp` |
| overFireSparkAndPrepareNextSchedule de-static'd for attribution | `firmware/controllers/engine_cycle/spark_logic.cpp/.h` |

Why this is deterministic now: spark/dwell/injection moments fire from
the TIM5 ISR at priority 3 (CORTEX_MAX_KERNEL_PRIORITY), which
preempts the trigger handoff (4), the ADC (6) and everything else;
only the raw EXTI capture (0, ~1 us) can preempt it. Tooth timestamps
are captured in the raw EXTI ISR into a 32-entry queue BEFORE any
decode, so the decode being preempted by due events cannot distort
tooth timing. Expected dispatch latency: fixed ~1-3 us per command.

Validation: 1158 unit tests green; firmware builds; compile-time probe
confirms STM32_PWM_TIM5_IRQ_PRIORITY=3 and handoff=4 through the real
mcuconf chain. On-car validation pending: run lockstats at ~1887 rpm -
all of dwell/spark/overdwell/fuel must show late>=10us=0 and small
maxLateUs.

Build pitfall hit along the way: commas outside parentheses split
addConsoleAction macro arguments (the C standard protects only
parentheses, GCC too) - a brace-initializer array inside the lockstats
lambda broke the macro; the kind names now come from a helper function.
Also: a stray `gmake` at the firmware root rebuilds for the DEFAULT
board (f407-discovery) into the SAME build/ dir and poisons the
incremental m74_9 build - always rebuild the board via its compile
script after such an accident (gmake clean first).

## 2026-08-23 - ETB hardware PWM verified working; phantom ETB#2 soft-PWM flood found + disabled

Goal: move the m74_9 ETB throttle PWM off the microsecond executor (soft-PWM) onto
hardware TIM12 (PB14 = TIM12_CH1 AF9, the only non-complementary timer channel for
PB14 on AT32F435) and confirm idle control still works.

State verified on the bench (18:14 flash, boot log 18:27): `hardPWM ETB Enable pin 32
freq 800: HW PWM acquired ch0 AF9 -> OK` (pin 32 = Gpio::B14). Autocalibrate sweeps the
blade 0-100%, proving the hardware PWM path AND the DIR (PB15) switching both work.

Important: the lockstats lower table ("PWM channels (softPwmEvents=...)") only lists
channels registered in g_pwmList; the hardware-PWM path (startSimplePwmHard success)
does NOT call weComplexInit, so a hardware ETB shows up as NO soft-PWM line (not even
as "hard/stopped"). So "soft 800Hz softPwmEvents=111427" in lockstats is NOT the real
ETB - it is the phantom second ETB.

Root cause of the executor flood: the stored tune carried etbFunctions2="Throttle 2"
with all pins NONE. doInitElectronicThrottle() still creates a DcHardware for it, which
runs startSimplePwmHard on pin 0 -> "no timer map -> soft PWM" -> a soft-PWM channel at
etbFreq (800 Hz) that re-arms the executor ~100k+ times (lockstats othercb 0806DBE3)
while writing to NO physical output. The board has exactly one TLE9201 H-bridge.

Fix (firmware, applied in ConfigOverrides so it overrides any stored tune on every boot):

| Change | File |
| --- | --- |
| Force etbFunctions[1] = DC_None in setupEtb() | `firmware/config/boards/m74_9/board_configuration.cpp` |
| etbFunctions2 -> "None" in the default tune | `firmware/config/boards/m74_9/21129.msq` |

Also added a comment documenting why etbFunctions[1] must stay None.

Validation: fresh m74_9 build succeeds; not yet flashed/on-car. Expected after flash:
lockstats loses the "soft 800Hz" line and sched exec total drops sharply; the real ETB
(PB14 hardware) is simply absent from the soft table. On-car retest of idle pending.

Open follow-ups:
- Re-check idle closed-loop now that TPS is freshly autocalibrated on the hard-PWM
  build (earlier "blade freewheels at idle" was observed on a pre-autocalibrate state;
  hardware PWM itself is proven by autocalibrate).
- Hard-PWM channels are invisible to printPwmStats; if per-channel visibility of the
  hardware PWM is ever wanted, register hardware channels too (they'd show
  softPwmEvents=0 / "hard").

## 2026-08-23 - lockstats callback address map + delay analysis; enable alwaysInstantRpm + idle closed loop

Decoded the othercb addresses in lockstats against the 20:36 m74_9 ELF
(addr2line): 08055169=startAveraging (MAP start), 08055075=endAveraging (MAP end),
080491CD=startKnockSampling, 08048E19=onTdcCallback, 0806CFFD=watchDogBuddyCallback,
0803D711=timerValidationCallback.

Key delay-analysis findings (goal: ideal ignition/injection timing):
- startAveraging (MAP) is ALWAYS ~100% late (20:54 n=5640 late=5618; 20:56 n=227 l=227)
and showed the single worst hitch maxLateUs=22694 (22.7 ms). Root:
onEnginePhase schedules samplingStart-currentPhase inside [currentPhase,nextPhase) -
tiny lead from now, executor cannot dispatch on time. MAP, not ignition - cosmetic.
- spark/dwell/fuel late of 10-80 us (38-67%) is scheduling jitter, NOT the timing enemy:
at 6000 rpm 1 deg=27.8 us, 1000 rpm=167 us; 10-20 us typical = 0.1-0.4 deg. Cannot be
removed (near-future event queued at exec ISR finish always pays dispatch latency).
- REAL timing inaccuracy is invisible to late: scheduleByAngle uses oneDegreeUs*angle
from edgeTimestamp, and oneDegreeUs is RPM-averaged over 720 deg when
alwaysInstantRpm=no. Under accel/decel the linear extrapolation places the event at the
wrong absolute moment - fires on time (late~0) but at wrong crank angle.

Changes to 21129.msq:
| field | was | now | effect |
| alwaysInstantRpm | no | yes | oneDegreeUs from ~90-deg instant rpm -> better angular accuracy in transients; does NOT change late |
| idleMode | Open Loop | Open Loop + Closed Loop | close idle RPM PID loop for idle hold |
| idleTimingSoftEntryTime | 0.5 | 0.1 | faster soft entry into idle timing PID |

Notes: useIdleTimingPidControl already yes; measureMapOnlyInOneCylinder stays no (shared
plenum); idleReturnTargetRamp stays no. Validation: tune only, no flash. Expected better
angular accuracy in throttle-release/accel - watch MLG; late will NOT improve. Risk:
instant rpm noisier at idle - revert that one bit if idle hunts.

## 2026-08-23 - scheduling dispatch jitter analysis (spark/dwell/fuel late) - deferred

Asked: can the scheduler's `late` (spark/dwell/fuel) be reduced? Analyzed the dispatch
pipeline (single_timer_executor.cpp + event_queue.cpp + trigger_scheduler.cpp).

Where `late` comes from (3 levels):
1. PHYSICAL dispatch floor (not removable): TIM5 ISR entry -> CriticalSectionLocker ->
   queue walk -> unlink -> execute = ~1-3 us minimum even for an ideally-placed event.
2. CLOSE-LEAD planning (main, partly removable): spark/dwell/fuel are scheduled from the
   CURRENT tooth (edgeTimestamp) to an enginePhase lying inside the current tooth
   interval. 60-2 tooth = 6 deg; at 6000 rpm 1 deg = 27.8 us, so an event can sit only a
   few us ahead of now. Inside the 8 us pool hysteresis (EventQueue(US2NT(8))),
   executeOne spin-waits then measures late = how much it "rotted" - the bigger the
   rot, the higher the RPM/share of late (matches: spark 36%, fuel 50%, dwell 65%).
3. Queue batching: executeAllPendingActions runs all due events in one ISR pass;
   a long callback (dwell cbmaxUs=167) delays the following events in the same pass.

What could reduce it (honest cost/benefit):
- SCHEDULE 1 TOOTH AHEAD (always place spark/dwell/fuel from the NEXT tooth, not the
  current one) -> big lead >> floor -> removes the "chasing" late. Risk: angular
  accuracy under hard accel (already helped by alwaysInstantRpm). Needs bench/car
  validation. This is THE candidate if we ever act.
- Lower the 8 us pool hysteresis: marginal (dispatch floor remains), raises timer
  re-arm overhead - not worth it.
- Exclude sub-hysteresis-lead events from `late` stats: cosmetic only, changes no
  real jitter.
- long callback in same ISR pass: already tracked via cbmaxUs/cbDuration.

KEY: this late barely affects ideal timing - 10-20 us typical = 0.1-0.4 deg at 6000,
~0.1 deg at idle; maxLate 156 us = ~0.6-1 deg (rare). Fuel 1-10 ms injection:
10-20 us is 0.1-2% duration - negligible. The things that DID improve ideal timing:
lost-TIM5-compare fix (removed brick/wedge) and alwaysInstantRpm (angular accuracy).
`late` is a cosmetic dispatch floor, not real timing loss.

DECISION (deferred): do NOT touch scheduling code yet. First validate MLG with
alwaysInstantRpm=yes on throttle-release/accel. If transients are clean, leave `late`
alone (don't fix what isn't broken). Only if MLG shows unexplained UOZ jitter in
transients, revisit scheduling-1-tooth-ahead. Pick this thread back up with fresh MLG.

## 2026-08-24 - m74_9: console self-burn root cause (the "nobody clicks Burn" mystery)

Question: MFS settings writes (ID 1/2, 17544 bytes, 168 ms each) appear every ~1-3 min
in both sessions although nobody presses Burn and nothing is edited in the console.
Also "TS -> Burn, waiting for CRC" + "TS burn CRC timeout" + "Finishing pending TS burn".

Answer: it IS the console burning - its own TS-protocol burn, triggered by its Tune tab
upload loop, not by the Burn button. The writes are full-config burns, NOT fuel trim
(LTFT is the separate small ID 3 = 2048 bytes, 17-38 ms).

### Established mechanism (all facts from the logs + code)

1. The console Tune tab keeps a "session image" (the model). Any edit event in the
   dialogs calls TuningToolbarWidget.onEdit -> 100 ms upload timer ->
   BinaryProtocol.uploadChangesWithoutBurn(sessionImage) -> diffs sessionImage against
   the console's cached ECU image (getControllerConfiguration) -> writes only differing
   regions ("Need to patch: Pair{first=..., second=...}, size=N" log lines) ->
   sets isBurnPending=true.
2. uploadChangesWithoutBurn deliberately does NOT update the cached ECU image
   (comment in BinaryProtocol.java L321-324). So once the cache and the session image
   diverge, EVERY subsequent edit event re-diffs and re-writes the SAME regions.
   Observed: the identical patch fired twice 26 ms apart at 110250.792/.808 (two
   upload passes over the same stale diff).
3. A burn follows via bp.burn() (logs "Need to burn" + "BURN OK"). On the ECU, page-0
   burns are deferred: "TS -> Burn, waiting for CRC" -> the console NEVER sends the
   CRC-check command (BurnCommand.execute only checks the BURN_OK response, by design)
   -> firmware times out after 2 s ("TS burn CRC timeout") -> "Finishing pending TS
   burn" -> "we are allowed to burn" -> requestBurn() -> setNeedToWriteConfiguration()
   -> writeToFlashNowImpl() writes BOTH settings copies (ID 1 AND ID 2) UNCONDITIONALLY.
   No comparison against existing flash content, no CRC skip. On AT32/MFS: 168 ms per
   copy, ~2.3 s when MFS GC fires, no read-while-erase -> full CPU stall.
4. Why it never converges: the ECU resets every ~5 s (debug bench build, known
   behavior). Resets reload config from flash; the console's cache refreshes only on
   full reconnect (~every 1-3 min). Meanwhile the values on the ECU, in the console
   cache, and in the session image keep drifting apart because the tune is being
   actively edited - see the field-level evidence below.
5. Session 1 additionally ran the Load Tune flow 3 times on 21129.msq (101511.462,
   102321.776, 102340.161: "Loading 21129.msq..." -> "Applying tune fields..." ->
   migration "Field X is going to be restored" -> "Uploading and burning to ECU...").
   Those produced the DefaultTuneMigrator restores (etb_iFactor 3.0->1.0 etc.) and
   their burns.

### Field-level proof (patched offsets vs the msq commits)

Patched offsets in session 2 map to: 1580=idleRpmPid_iFactor, 1624=idlePidRpmDeadZone,
1774=fan2ExtraIdle, 2108/2110/2112/2116=coastingFuelCutRpmLow/Tps/Clt/Map,
1077=noFuelTrimAfterDfcoTime, 3201=dfcoDelay, 3989=dfcoRetardRampInTime. Session 1
additionally patched 816/882/1576-1586 (idle PID), 6572=cltCrankingCorr,
6628=afterCrankingIACtaperDuration, 6648=idleAdvance, 7308+=cltIdleCorrTable, and
migrated etb_iFactor/etb_pFactor/idle_antiwindupFreq.

These are EXACTLY the fields changed by the tune commits around the sessions
(8837fa256e5 "upd msq" 11:13, 3b8de553e13 iFactor, ed1ff009170 deadzone, ...):
coastingFuelCutMap 60->100, dfcoDelay 0->1.0, fan2ExtraIdle 2->0, idleRpmPid_iFactor
0.003->0.001, noFuelTrimAfterDfcoTime 5->1.0, etb_iFactor 3->1, idlePidRpmDeadZone
10->50. The values the console pushed during the sessions (fan2ExtraIdle=1,
coastingFuelCutMap=10, noFuelTrimAfterDfcoTime=0, idleRpmPid_iFactor=0.0) are the
INTERMEDIATE values of that same ongoing tuning - the ECU ran the pre-commit tune
(noFuelTrimAfterDfcoTime=5.0, fan2ExtraIdle=2, coastingFuelCutMap=60 all observed as
"old" in the patches), the session image held the evolving edited tune.

Conclusion: the "mystery" writes are the console repeatedly pushing the evolving tune
onto the ECU. Nobody pressed Burn in TunerStudio, but the console's own edit-upload
timer + upload/burn paths ran the TS-protocol burns. The burns repeat forever because
uploadChangesWithoutBurn never updates the local cache and the ECU keeps resetting.

### Fixes (recommended, not yet implemented)

- FIRMWARE (strongest, kills the 168 ms / 2.3 s stall class): in
  writeToFlashNowImpl() (firmware/controllers/flash_main.cpp L93) compare the new
  persistentState.crc (or full bytes) with the last-written content and skip the
  storageWrite calls when identical. Cheapest correct version: read back the stored
  crc from the settings record (read is ~ms on MFS vs 168 ms write) or keep a RAM
  "lastWrittenCrc" (resets on boot - acceptable, first burn after boot writes once).
- CONSOLE: uploadChangesWithoutBurn should advance the cached image to the uploaded
  snapshot (it is the source of the infinite re-diff); burnToEcuAndThen /
  uploadChanges should no-op when isBurnPending is false and the image is unchanged.
- CONSOLE diagnostic: log field NAMES in "Need to patch" (currently raw offsets) -
  would have turned this multi-hour analysis into a one-line answer.
- The ECU-side burn already waits 2 s for a CRC check the console never sends; the
  timeout path is the console's normal burn completion (not a bug by itself), but it
  means every console burn costs an extra 2 s - cosmetic.

### Answers to the user's specific questions

- "Может быть это fuel trim correction?" - NO. LTFT = ID 3 (2048 B). The ID 1/2
  writes are full-config burns of the console's own upload loop (above).
- "Почему запись идет, конфигурация не изменяется" - the config DID differ (ongoing
  tuning + stale cache + ECU resets), and even when it did not, the firmware writes
  unconditionally - there is no change-skip.
- "Кто съедает процессорное время" - on AT32 the 168 ms x2 MFS write (and ~2.3 s GC)
  per burn stalls the whole CPU including ISRs; several WDA kills sit next to these
  writes (105345, 105510, 105523, 110309, 110424, 110646). The 105531 kill had no MFS
  write nearby - that one is the thread-latency mode B (fixed separately by the WDA
  executor move).

## 2026-08-24 - m74_9: console Burn-button-only writes (implemented)

User decision: the console must write configuration ONLY on the Burn button - no live
parameter updates, no automatic re-writes (the self-burn loop from the previous entry
pushed intermediate tune values into the ECU while the user was tuning).

Changes (java_console):
- TuningToolbarWidget: removed the 100 ms edit-upload timer entirely. onEdit now only
  does undo bookkeeping + state refresh; setFirmwareUpdateInProgress/onDisconnect no
  longer touch a timer. burnToEcuAndThen is the single write path: it calls
  BinaryProtocol.uploadChanges(image) (diff + upload changed regions + burn + advance
  the cache) with an error dialog on failure, then writes/burns the dirty secondary
  TS pages as before.
- BinaryProtocol.uploadChangesWithoutBurn: now advances the cached controller image to
  the uploaded snapshot (setConfigurationImage after the chunk loop - writeData throws
  on failure, so the advance only happens when every chunk landed). This kills the
  infinite re-diff of the same regions regardless of the caller.
- TuningPane: updated the stale comment about the uploadChangesWithoutBurn diff
  baseline on reconnect.

Behavior now: edits stay local ("Pending changes not burned" label), pressing Burn
uploads only the changed regions and burns; a Burn with no changes is a complete
no-op (no chunks, no TS burn command, no MFS write). Remaining user-gated writers
outside the Burn button: the offline-reconcile dialog on connect (explicit choice),
the KnockPane start/stop toggle, wizard panels.

Validation: ./gradlew :ui:compileJava :ecu_io:compileJava -> OK; ./gradlew :ui:test
:ecu_io:test -> all pass.

Still pending: firmware-side CRC-compare skip in writeToFlashNowImpl() so that any
burn (console, TS, wizard) costs nothing when the config is byte-identical - the
strongest remaining guard against the 168 ms / 2.3 s MFS stall class.

## 2026-08-24 - m74_9: WDA-executor brick fixed (SPI preemption deadlock)

Symptom after the WDA-executor bundle (260824): the block does not come online at
all - fuel pump never switches on, the console cannot connect.

Root cause: the WDA feed moved into the TIM5 executor ISR (kernel priority 3), and
the thread-side SPI batches were assumed to be protected by
chibios_rt::CriticalSectionLocker. That assumption is WRONG: chSysLock /
CriticalSectionLocker do NOT mask kernel-priority IRQs (ARMv7-M BASEPRI is set to
CORTEX_BASEPRI_KERNEL, which leaves the kernel IRQs enabled). So the executor CAN
preempt the driver thread mid-batch. The batch runs spiStart() on every call, and
spi_lld_start() re-programs CR1 by clearing SPE first:

  CR1 &= ~SPE; CR1 = cfg; CR2 = cfg; CR1 |= SPE;   (hal_spi_lld.c)

If the executor lands in that SPE=0 window, its spi_lld_polled_exchange()
busy-waits on RXNE:

  DR = frame; while ((SR & RXNE) == 0);            (hal_spi_lld.c)

With the peripheral disabled RXNE never sets -> the TIM5 kernel ISR spins forever
-> the whole system is dead (no fuel pump drive, no USB CDC/console, watchdog
thread can no longer run). The collision is a matter of seconds at boot: the diag
refresh re-runs spiStart every ~7 ms and the feed fires every ~105 ms.

Fix (firmware/hw_layer/drivers/gpio/l9779.cpp):
- volatile bool spi_busy: set for the whole thread-side batch in spi_rw() and
  spi_rw_array(); the executor feed (wdFeedFromExecutor) checks it and defers
  2 ms instead of exchanging. The answer window is ~12.6 ms, so a 2 ms deferral
  can not miss it.
- spiStart() is now called only ONCE (spi_configured guard): the CR1/CR2 config
  persists, the bus is dedicated, and re-running it only re-opens the SPE=0
  window. Belt-and-suspenders on top of the flag.
- Removed the now-misleading CriticalSectionLocker wrappers around the SPI
  batches and corrected the comments (the real serialization is spi_busy).
  wake_driver()/writePad() keep their lockers - those protect non-SPI state.

Validation: compile_m74_9.sh -> BUILD SUCCESSFUL.

## 2026-08-24 - m74_9: OUT_DIS heal after L9779 power event (Mode A root cause)

The 18:04 bench log proved the blade-drop root cause is INSIDE the L9779 power
tree: DIA10=0x8F = OV_RST + VDD5_OV + V3V3_UV + TNL_RST right after a throttle
blip. The L9779WD-SPI is a System Basis Chip that CONTAINS the board regulators -
5V precision regulator (external NMOS pass element driven via charge pump on pin
CP, 400 mA), 3.3V regulator (from VDD5, 100 mA, this feeds the MCU) and 2x 5V
tracking sensor supplies VTRK1/2. So "the L9779 side stops supplying" means the
chip's own regulator monitors tripped: 5V overvoltage + 3.3V undervoltage + a
smart-reset (TNL_RST) -> all config registers wiped, OUT_DIS=1, drivers off.

Two consequences fixed in firmware (l9779.cpp):
1. The old OUT_DIS self-heal was gated on !fault_flags && !wd_int and never
   fired: F1/F2 (0x50) are set in the steady state, so ANY flag blocked it. The
   chip stayed latched ~574 ms and the engine died before the EC=7 path
   recovered it.
2. A chip reset wipes the whole config (RESPTIME back to 0x3f, VRS back to
   limited-adaptive + filter off, CONTR1..4 cleared) - re-issuing START alone
   leaves the WDA feed missing the ~112 ms default window every cycle and the
   VRS conditioner unfiltered at speed.

New chip_heal_out_dis(): START + (on a reset event) RESPTIME + VRS at the
CURRENT ramp step + CONTR restore, rate-limited to 200 ms, called from
refresh_diag_cache() the moment OUT_DIS is seen (~100 ms worst case). DIA10
reset-event mask = TNL_RST | CRK_RST | OV_RST; VDD5_OV/V3V3_UV/F1/F2 cuts keep
their config and only need START + CONTR (the datasheet's recipe for a VDD5_OV
driver cut). Datasheet facts verified: VDD5_OV flag "does not inhibit the
drivers switch on"; OUT_DIS is cleared only by START/SW_RST/RST, not by reads.

Hardware action (user side): scope VB, VDD5, the external NMOS gate (pin CP),
V3V3 during a throttle blip. The charge-pump cap on CP and the VDD5/V3V3
decoupling are the prime suspects for the 5V overvoltage transient.

Validation: compile_m74_9.sh -> BUILD SUCCESSFUL; bundle rusefi_bundle_m74_9.zip
built (contains the heal).

## 2026-08-24 - m74_9: the VRS ramp was mis-writing the power-management registers (WDA time-base flip during cranking)

Answering "is the L9779 regulator configurable": the 5V/3.3V setpoints and
OV/UV thresholds are NOT (fixed pre-trimmed references). But the power-management
register CONFIG_REG6 IS - and the driver has been mis-writing it for weeks.

The "stock VRS ramp" copied from the stock firmware's config script writes THREE
registers per step: REG4 (0x04), REG5 (0x05), REG6 (0x06). Per the datasheet
register map only REG5 is VRS (VRS_HYST/VRS_MODE/VRS_DIAG). REG4 is power
management (PWL_TIMEOUT_CONF, ISO_SRC, LOCK) and REG6 is power management + the
WDA time base (PWL_EN_N, PSOFF, VDD5_UV RST/WDA masks, and CONFIG6 bit1 =
f_clk 64/39 kHz - cross-referenced in datasheet 6.15: "f_clk depends on CONFIG6
bit1 value").

The stock's REG6 steps are 0x07 -> 0x05 -> 0x05 -> 0x06. The 0x05 values (ramp
steps 1..2, active at 150..600 rpm) have bit1=0 -> the WDA time base flips to
39 kHz exactly during cranking. With RESPTIME=10: response time (1+101*10)/39kHz
= 25.9 ms, window [25.9, 38.5] ms, while the executor feed is clamped to 27 ms -
it drifts against the 38.5 ms cycle and misses ~2/3 of cycles -> EC climbs ->
WDA kill pulses during the catch. This matches the miss=5..8 counters right at
the catch in the 16:02/16:09 logs and is very likely the cranking blade-drop
mechanism (Mode B at cranking), independent of the thread-latency misses of the
old thread-based feed.

Fix:
- vrs_ramp_to_step() now writes REG5 ONLY; the REG4/REG6 writes are gone.
- CONFIG_REG6 = 0x06 (L9779_CONFIG6_PWR, the stock's steady-state value: power
  latch enabled, VDD5_UV->WDA masked, 64 kHz time base) is applied ONCE in
  chip_init() BEFORE the RESPTIME anchor, and re-applied in
  chip_heal_out_dis(true) after a chip reset (the reset reverts it: power latch
  off, VDD5_UV->WDA UNMASKED - unmasked, a VDD5 undervoltage pulls WDA low and
  kills the blade on every cranking rail dip).
- The WDA time base now never flips mid-run.

Validation: compile_m74_9.sh BUILD SUCCESSFUL, bundle rebuilt.

## 2026-08-24 - m74_9: WDA RESP_CNT desync fixed - single atomic answer burst (19:09 session)

After the time-base fix the engine still died: `ec=7 wda_int=1 ok=950 fail=0
miss=2 delay=22ms defer=14 kills=2 dia10=0x50`. Hardware is fine (stock
firmware runs the same chip perfectly), so this was ours. The counters were
the clue: 950 BYTE0 frames sent, only 2 timing misses, yet EC pinned at 7
with WDA_INT latched.

Root cause (datasheet 6.15 "Response comparison"): the chip tracks the
response progress in the 2-bit RESP_CNT counter ('11' waiting BYTE3 ... '00'
waiting BYTE0) and compares EVERY received byte against the expected value of
the CURRENT position (the expected bytes depend on RESP_CNT via the
RESP_SOLL7..0 formulas). The two-phase feed (BYTE3..1 in a "prepare" event,
BYTE0 5 ms later) left a gap in which a cycle could end unanswered - a
deferred/failed BYTE0, or one landing after the window end - and the window
end resets RESP_CNT to '11'. The late BYTE0 is then compared AS IF IT WERE
BYTE3: wrong value, and every subsequent byte stays off by one position
forever. Each cycle then completes with a wrong value: the EC increments
every cycle (invisible - only timing flags are counted, `miss=2`), pins at 7
and the WDA kills the blade until the SW_RST latch-heal fires (that is why
`kills` climbed slowly and the warnings showed ec=7 across hundreds of
"ok" answers). An out-of-order byte can never re-align because RESP_CNT only
resets at a sequencer run, and the sequencer runs on the WRONG byte position.

Fix: the feed is now ONE executor callback per cycle doing pipelined
REQUHI/REQULO reads + adaptation + the whole RESP_BYTE3..0 as a single
atomic 4-frame burst, positioned so BYTE0's END lands at the window center
(schedule = previous BYTE0 end + wd_delay_ms - WDA_BURST_LEAD_US=80us).
A perturbation now costs at most one missed cycle: RESP_CNT resets at every
sequencer run and the contiguous burst re-aligns. First kick moved +5 ms ->
+17 ms so the first BYTE0 lands inside the first window anchored by the
RESPTIME write (no boot miss). The prepare/BYTE0 split, wd_prepare_isr/
wd_byte0_isr/wdPrepareFromExecutor/wdByte0FromExecutor are gone.

Validation: compile_m74_9.sh BUILD SUCCESSFUL, bundle rebuilt.

## 2026-08-24 - m74_9: restore REG4/REG6 in the VRS ramp (time base pinned to 64 kHz)

After the WDA single-burst fix the engine runs, but the trigger started losing
exactly one tooth per revolution at ~2000 rpm (C9003 "expected 58/0 got 57/0",
newerr storm at time=55, then C9007/C9008 tooth errors + coil overcharge at
3454 rpm). The only VRS-adjacent change in the previous commit was removing the
REG4/REG6 writes from the ramp (REG5 only).

Even though per the datasheet only REG5 is a VRS register, the stock firmware
writes all three at every ramp step, and it runs this same chip/wheel
flawlessly - so its register traffic is reproduced byte-for-byte. The ONE
deliberate deviation: CONFIG6 bit1 (WDA time base) is pinned to 1 (64 kHz):
the stock's 0x05 values at steps 1..2 would flip it to 39 kHz during cranking
and break the executor's 22 ms feed again. REG6 ramp values are now
0x07/0x07/0x07/0x06 instead of the stock's 0x07/0x05/0x05/0x06.

Observations for the next session if the tooth loss persists: the C9009
"skipped spark" lines at 19:31:21/36/49 sit right on the bench debug-ECU's
~5 s reset cadence, and every MCU reset re-runs init() -> chip_reset() (SW_RST)
-> outputs cut + VRS re-armed at step 0 while the engine spins - skipped spark
and coil overcharge mid-run are expected from that alone (bench-only, does not
exist on the car). The steady per-rev tooth loss at ~2000 rpm between resets is
what the REG4/REG6 restore targets.

Validation: compile_m74_9.sh BUILD SUCCESSFUL, bundle rebuilt.

## 2026-08-24 - m74_9: VRS switched to the datasheet config - full adaptive + auto filter, no ramp

The restored stock-ramp build still lost exactly one tooth per revolution at
~2000-3400 rpm (C9003 57/58 at 3129 rpm, C9007 tooth #114 +11.9, C9008 tooth
#30 -11.3, coil overcharge) - so the stock ramp bytes themselves are the
problem, not the REG4/REG6 traffic. Datasheet analysis (6.14):

- the stock's ramp values run VRS_MODE=01 = auto hysteresis ON, auto-adaptive
  time filter OFF at every step, and step VRS_HYST 100..111 (32/51/17/0 uA);
  the datasheet says VRS_HYST only selects the hysteresis when auto-hyst is
  OFF ("If the auto adaptive hysteresis is OFF..."), and 111 = "test purpose
  only" - the ramp is largely a no-op that ends in a test value.
- the datasheet's own design for a VR sensor is fully adaptive mode with BOTH
  mechanisms ON: auto hysteresis (peak detector + 5-level quantizer) AND the
  auto-adaptive time filter Tfilter = 1/32*Tn (4..200 us), which adapt to rpm
  automatically - no software ramp needed. The chip's reset default REG5=0xd8
  is exactly VRS_MODE 11 + 17 uA floor.

New vrs_configure(): REG1=0x02 (full adaptive) + REG5=0xd8 (auto hyst ON +
auto filter ON + 17 uA floor), applied once at chip_init and re-applied by
chip_heal_out_dis(true) after a chip reset. The whole ramp machinery is gone:
vrs_ramp_to_step, the rpm-threshold stepping and the stop/key re-arm in the
driver thread, vrs_step/vrs_stop_ts. This also removes the REG6 3-vs-4-step
ambiguity (REG6 is now written once as L9779_CONFIG6_PWR=0x06, time base
pinned to 64 kHz).

Also confirmed in this session: the OUT_DIS heal works on a real power event -
the 19:49:03 DIA10=0x8F event was recovered in ~1 ms ("OUT_DIS heal: config
re-applied ... OUT_DIS cleared") and the engine kept running.

Validation: compile_m74_9.sh BUILD SUCCESSFUL, bundle rebuilt.

## 2026-08-24 - m74_9: WDA read batch hardened (4 frames) + REQUHI instrumentation

The pre-VRS build log (19:56) showed the WDA feed collapse WHILE DRIVING:
fail 9 -> 816 and miss 10 -> 712 in ~20 s, delay clamped at 17 ms,
ec=4 wda_int=1 (kill-active with a healthy EC - the chip's WDA_INT flaps as
EC crosses 4 during the collapse). The user's read is right: this is a
timing problem, and the errors accumulate/feed themselves.

Root cause: the DO reply to a read request arrives one or TWO frames after
the request (the init() IDENT probe sees the same). The feed's read batch
had only THREE frames (REQUHI/REQULO/filler), so a 2-frame-delayed REQULO
reply landed in the first answer write - the question was lost, the cycle
failed (fail++), the window expired unanswered (miss++), and once a few
cycles destabilize the phase walks and the collapse feeds itself: late
answers keep the NO_RESP flag up, the delay adaptation clamps at 17 ms, and
every window-end sequencer run + late response double-increments the EC
(WDA_INT sticks on -> blade killed in motion).

Fix:
- the read batch is now FOUR frames (REQUHI/REQULO/REQUHI/REQUHI): the
  REQULO reply is captured even with a 2-frame delay.
- REQUHI instrumentation, exposed in the warning line and l9779 debug:
  reqhi=0xNN (raw DIA_REG15), wrong= (W_RESP: value rejected), cntbad=
  (RESP_CNT != 11 at read time: answer-stream desync). These increment the
  EC WITHOUT setting the timing flags - they made the 19:09 'ec=7 miss=2'
  session look healthy; next time the exact mechanism is visible in one line.

Validation: compile_m74_9.sh BUILD SUCCESSFUL, bundle rebuilt.

## 2026-08-24 - m74_9: WDA adaptation direction fix + stream-shift skip (20:12 session)

The 4-frame read fix killed the fail storm (fail=0), and the new
instrumentation paid off immediately: reqhi=0xDB = RESP_CNT 11 (aligned) +
NO_RESP + RESP_TO_EARLY both set - the datasheet's signature of a TOO-LATE
response ("a too late response is at the same time a too early response
concerning the next WDG cycle"). Two bugs found from that single byte:

1. Adaptation direction: the code checked RESP_TO_EARLY before NO_RESP, so
   the both-flags (late) case was classified as EARLY and the delay walked UP
   (+5) until it pegged at the 27 ms clamp - sitting on the window edge,
   late answers never decrementing it back (delay=27ms in the log). Fixed:
   NO_RESP is checked FIRST; a both-flags byte is now correctly treated as
   late (-5 ms).
2. Stream shift: cntbad=8 means RESP_CNT was != 11 at read time 8 times - a
   stray late byte shifts the answer stream by one position, and writing the
   burst into the shifted stream completes a wrong-value response (EC++) AND
   keeps the shift. Fixed: on RESP_CNT != 11 the burst is SKIPPED - the
   window expires unanswered (one EC via NO_RESP), the sequencer resets
   RESP_CNT to 11 and the next burst re-aligns deterministically.

Also explained: ec=4 wda_int=1 in the warning is the chip's WDA_INT/EC
update race during the EC 4<->5 flapping the marginal delay=27 state caused
(the blade kill follows WDA_INT, hence the drop); with the direction fix the
delay converges to 22 ms (6 ms margins) and EC never touches 5.

Validation: compile_m74_9.sh BUILD SUCCESSFUL, bundle rebuilt.

## 2026-08-24 - m74_9: diag refresh avoids the WDA burst window + delay recenter (20:28 session)

The 20:28 warning: ec=4 wda_int=1 ok=10793 fail=0 miss=28 wrong=4 cntbad=11
delay=27ms defer=123 kills=8 reqhi=0xF8. The fail storm stays gone (fail=0),
and reqhi=0xF8 = RESP_CNT 11 (aligned) + RESP_ERR (a wrong byte in the last
cycle) with CLEAN timing flags - value-only misses do not carry timing flags,
so the adaptation could not move the delay off the 27 ms clamp (1.4 ms
window-close margin).

The chain behind the remaining kills: the diag refresh (thread, every ~100 ms)
collides with the WDA burst -> the executor defers +1 ms (defer=123) -> at
delay=27 the deferred BYTE0 lands past the window close -> a late byte shifts
the answer stream -> wrong values (EC++ without timing flags) -> EC 4<->5
flapping -> 8 kill pulses.

Fixes:
- the driver thread skips a diag-refresh chunk while the WDA burst is imminent
  (wd_sched.getMomentNt() within the next 2 ms) - the refresh is 100 ms
  cadenced, a skipped chunk costs ~7 ms;
- on a value rejection (W_RESP) or a stream shift (RESP_CNT != 11) the delay
  is recentered to the nominal 22 ms (WDA_DELAY_INIT_MS) - these misses carry
  no timing flags, so the +-5 ms adaptation alone cannot leave the clamp edge.

Validation: compile_m74_9.sh BUILD SUCCESSFUL, bundle rebuilt.

## 2026-08-24 - m74_9: drive analysis (20:43-21:14) - MFS writes land on trigger storms; flash gate debounced

Full drive log analyzed (efi_log_2026-08-24_20_43_06_592.log.0, 31 min):
- WDA/blade: ZERO "TLE9201 outputs disabled" warnings the whole drive - the
  watchdog category is CLOSED (ec=0 wda_int=0 miss=0 wrong=0 cntbad=0
  delay=22ms reqhi=0xC0 at 20:47).
- Remaining: repeating trigger clusters (C9003 57/58 + C9007 tooth#114 +11.9
  + C9002 58/58 + C9009 skipped spark + C9353 coil overcharge), 19x C9003 /
  17x C9002 / 32x C9009 over the drive, every 1-5 min, rpm-banded 2000-3400.

Correlation found: the LTFT MFS writes (ID 3, 2048 bytes, 17-38 ms full-CPU
stall on AT32 - no read-while-erase) land MILLISECOND-EXACT on the C9003
clusters: 20:45:49.267, 20:49:33.142, 20:55:46.392, 20:56:22.989,
21:04:40.674 (plus 2x full ID-1/2 burns of 168 ms each at 20:56:56 and
20:59:54). The chain: a trigger storm flaps isStopped() true -> the single
poll in custom_board_allowFlashNow admits the deferred write -> the 17-38 ms
stall coalesces ~90 tooth edges (3000 rpm) -> teeth lost -> the storm
deepens. The executor lockstats on the move confirm the stall class:
maxLateUs=44143 (44 ms) on the WDA callback = exactly one such write.

Fix: custom_board_allowFlashNow / custom_board_allowTsBurn now debounce the
stopped condition - 10 consecutive stopped polls (~1 s at the storage
manager's 100 ms cadence) before the flash is allowed. A storm flap can no
longer admit a write.

Still open (next step): the FIRST desync of each cluster (e.g. 20:43:33, no
MFS write before it) - the VRS gap-region distortion at 2000-3400 rpm. To
decide analog vs decoder-side: capture 'rawtrg' at ~3000 rpm right after a
C9003 (rawtrg = the EXTI edges BEFORE the noise filter; if the tooth is
missing there it is analog, if present but dropped it is the decoder).

Validation: compile_m74_9.sh BUILD SUCCESSFUL, bundle rebuilt.

## 2026-08-24 - m74_9: L9779 VRS filter conditions + running-band tooth-loss tolerance

Goal: make rusEFI fully understand the L9779 VRS output. The 20:43-21:14 drive
showed 19 C9003 clusters at 1763-4005 rpm (avg 2220), 837/837 'newerr' lines
with the gap ratio windows PASSING ('Y') and the count short by exactly one
(57/58) - the signature of a single lost decode edge, not noise. The user
confirmed the noiseless filter was on for only ~5 min of the session, so the
tooth loss is in the analog domain (the chip), not the software filter.

L9779 VRS time-filter conditions (datasheet 6.14.2), the key facts:

- Rising edge of int_vrs (= OUT_VRS rise = PF8 fall = the rusEFI decode edge):
  if the high level lasts < Tfilter the edge is SUPPRESSED entirely; otherwise
  it appears DELAYED by Tfilter. This is the only edge the chip can eat.
- Falling edge: not delayed; masked for Tfilter after the edge.
- Tfilter(n+1) = 1/32*Tn (one period behind), clamped 4-200 us, reset to
  200 us on every re-enable of the flying-wheel function (every vrs_configure,
  i.e. boot and every OUT_DIS heal). The 'if int_vrs > Tfilter(n)' guard only
  blocks a ~32x speedup, so a heal eats at most 1-2 teeth before re-adapting.
- Measured on the car: the 46/54 edge duty = Tfilter + Td_off (~15 us) delay
  on the decode edge; the post-gap edge arrives ~0.5 pitch early (gap reads
  2.4-2.5 instead of 3.0) - the auto-hysteresis re-quantizing on the missing
  teeth' small peak. Steady-state eat margin is ~15x (t_high 0.46*T vs
  Tfilter T/32), so a real tooth is eaten only when the auto-hysteresis
  momentarily reaches the local amplitude near a peak-detector quantization
  boundary - the 1763-4005 rpm band.

Fix (decoder tolerates the chip's documented behavior; no VRS re-tuning):

| File | Change |
| --- | --- |
| firmware/hw_layer/board_overrides.h | new hook custom_board_syncAcceptToothLoss |
| firmware/hw_layer/hardware.cpp | hook definition |
| firmware/controllers/trigger/trigger_decoder.cpp | running-band acceptance: a ratio-validated gap candidate with a 1-2 event count DEFICIT is a validated sync (kind 'T' via boardTriggerSyncEvent, 'toothloss' printGaps). Same guards as the cranking early-gap path: deficit-only bound (noise inserts, never deletes), ratio/position/elapsed-time gates already passed. 3+ deficit keeps the strict C9002 path. |
| firmware/controllers/trigger/trigger_decoder.h | 'T' kind documented |
| firmware/config/boards/m74_9/board_configuration.cpp | hook enabled: cranking..7000 rpm, off for directSelfStimulation (bench stays strict) |
| firmware/config/boards/m74_9/m74_9_tooth_diag.cpp | 'T' kind documented in the synctrace ring comment |
| unit_tests/tests/trigger/test_60_2_cranking_transition.cpp | 3 new tests: accepted while running, desync without tolerance, 3-missing still desyncs |

Validation: unit tests 1161/1161 pass (new tests included), compile_m74_9.sh
BUILD SUCCESSFUL.

Still open: the definitive discriminator for the eat position (mid-rev vs
gap-adjacent tooth) is a 'rawtrg' capture at ~3000 rpm right at a C9003. The
tolerance covers both, but the capture would confirm the quantization-boundary
mechanism. Follow-up if a heal storm reappears: vrs_configure resets Tfilter
to 200 us, eats 1-2 teeth above ~2375 rpm, absorbed by the same tolerance.

## 2026-08-24 - m74_9: the compressed gap is a spurious edge - gap-anchor correction

Analysis of the full rawtrg dump (18 revolutions, 373-1129 rpm coast): the
measured gap compression (gap0 2.36-2.5 vs the physical 3.0) is NOT an early
real tooth edge. The peak detector re-quantizes DOWN during the missing-teeth
gap, the missing region's residual signal crosses the lowered threshold ~0.63
pitch early, and the squared-signal LATCH suppresses the real first-tooth
rising edge (output already high) until the true falling edge at 3.0 pitch.
Count is preserved (57 real + 1 spurious = 58, synctrace countErr=0), but the
decoder anchors the phase ~3.8 degrees ADVANCED. The shift is constant in
ANGLE across rpm (0.63 +-0.03 pitch at 373-1129 rpm) because the auto-H
tracks amplitude - the hysteresis quantization table made observable.

Fix: new board hook custom_board_syncGapAnchorCorrection (m74_9: cranking..
7000 rpm, off for self-stim). The decoder measures (syncRatioAvg -
measuredGap) per validated sync, EMA 0.5, clamped to [0, 1.5] pitch (a
stretched gap from acceleration never yields a negative correction), stores
gapAnchorCorrectionDeg in TriggerCentral, and TriggerCentral::
handleShaftSignal retards the scheduling phase pair (current + next) by it.
The correction self-releases when a clean 3.0 gap returns.

| File | Change |
| --- | --- |
| firmware/hw_layer/board_overrides.h, hardware.cpp | custom_board_syncGapAnchorCorrection |
| firmware/controllers/trigger/trigger_decoder.cpp | syncValidated flag + per-sync correction update (EMA, clamped) |
| firmware/controllers/trigger/trigger_central.h | gapAnchorCorrectionDeg member |
| firmware/controllers/trigger/trigger_central.cpp | retard the phase pair at mainTriggerCallback |
| firmware/config/boards/m74_9/board_configuration.cpp | hook enabled (cranking..7000, off for self-stim) |
| unit_tests/tests/trigger/test_60_2_cranking_transition.cpp | 2 new tests: learn/release/re-learn at 2.36/3.0 gap; stretched gap (3.6) never updates |

Validation: unit tests 1163/1163 pass, compile_m74_9.sh BUILD SUCCESSFUL.

Still open: the eaten-tooth mechanism at 1763-4005 rpm (57/58) was NOT present
in the coast dump - the gap structure there preserves the count. The definitive
capture is a rawtrg at ~3000 rpm right at a C9003; the anchor correction and
the tooth-loss tolerance both cover the known failure until then.

## 2026-08-24 - m74_9: VR amplitude model (hysteresis level estimator)

The L9779 auto-hysteresis quantizes the VR peak into 5 levels (PV1-4: 930/
1600/2300/3000 mV; HI1-5: 5/10/17/32/51 uA = 50/100/173/322/510 mV on this
board's 10k). The VR peak scales with rpm (Vp = k*rpm), so one calibration
scalar k predicts from rpm alone: the hysteresis level, the expected gap
shift per level, and the eat-risk (Vp near a PV quantization boundary is
where the auto-H can momentarily reach the local amplitude).

| File | Change |
| --- | --- |
| firmware/config/boards/m74_9/m74_9_vr_model.h/.cpp | NEW: model + tables, 'vrmodel'/'vrk' console commands (k in RAM, 0 = off) |
| firmware/config/boards/m74_9/board.mk | new source added |
| firmware/config/boards/m74_9/board_configuration.cpp | custom_board_vrGapShiftPitch + console command registration |
| firmware/hw_layer/board_overrides.h, hardware.cpp | custom_board_vrGapShiftPitch (float hook) |
| firmware/controllers/trigger/trigger_decoder.cpp | out-of-band gap measurement falls back to the model estimate instead of freezing |
| unit_tests/tests/trigger/test_60_2_cranking_transition.cpp | model-fallback test |

The expected shift table is SEEDED with the coast measurement (0.63 pitch
per level); the per-level split needs the 2000-7000 rpm car captures. k is
calibrated on the car with 'vrk <mV/rpm>' (one bench VR-peak measurement at
a known rpm, or a fit from the measured shift). Persisting k as a tune field
(board_engine_configuration.txt + settings storage) is the follow-up once
the model is validated.

Validation: unit tests 1164/1164 pass, compile_m74_9.sh BUILD SUCCESSFUL.

## 2026-08-24 - m74_9: the VR model is the correction state (model as default)

Architecture flip: the per-level shift TABLE is now the gap-anchor correction
state (the model is the DEFAULT), and the per-sync gap measurement TRAINS it
(alpha 0.1, in-band only). The decoder applies table[level] directly - no
fast EMA, no fallback - and observes in-band measurements via the weak hook
triggerObserveGapShift (strong override in m74_9_vr_model.cpp). A stretched/
corrupted measurement never trains the table, and the model value keeps
applying. Level switches apply instantly (the scalar EMA used to re-converge
through every rpm sweep).

Persistence: the table + k persist to MFS on engine stop
(EFI_VR_MODEL_RECORD_ID = 7, magic/version/CRC, 'VRML'), loaded at boot -
after the first drive the car starts already calibrated. The save goes
through the debounced flash gate (custom_board_allowFlashNow), cooldown 15 s,
requested from custom_board_periodicSlowCallback (m74_9VrModelPeriodic).

| File | Change |
| --- | --- |
| firmware/config/boards/m74_9/m74_9_vr_model.h/.cpp | learned table (alpha 0.1), triggerObserveGapShift override, MFS record handlers, m74_9VrModelPeriodic, vrmodel prints the whole table |
| firmware/controllers/trigger/trigger_decoder.h/.cpp | model applies directly; measurement only trains |
| firmware/controllers/trigger/trigger_board_hooks.cpp | weak triggerObserveGapShift default |
| firmware/hw_layer/board_overrides.h | custom_board_vrGapShiftPitch is the state, not a fallback |
| firmware/controllers/storage.h/.cpp, storage_weaks.cpp | EFI_VR_MODEL_RECORD_ID + dispatch + weak handlers |
| firmware/config/boards/m74_9/board_configuration.cpp | custom_board_periodicSlowCallback = m74_9VrModelPeriodic |
| unit_tests/tests/trigger/test_60_2_cranking_transition.cpp | model-applies + in-band-training tests |

Validation: unit tests 1163/1163 pass, compile_m74_9.sh BUILD SUCCESSFUL.

## 2026-08-25 - m74_9: 13:41-13:55 drive analysis - flap storm at 3400-4300 rpm + persistence fix

Full parse of efi_log_2026-08-25_13_41_54_666 / _13_51_47_528 plus the 4 rawtrg
blocks and the trigger-log CSVs.

Progress vs the 08-24 20:43 drive: ZERO 'newerr' lines (was 837), the tooth-loss
tolerance keeps sync alive through the events, the engine reached 4276 rpm for
the first time (previous ceiling 4005). rawtrg: below ~2500 rpm the gap is
perfect - F-to-F gap ratio 2.36 +-0.02 at 1295/1821/2155/2463 rpm, countErr=0,
zero drops. Anchor correction stable 3.84-3.90 deg, the L1 table trains
(0.635 -> 0.651 pitch over ~9500 revs).

New signature at 3400-4300 rpm (13:53 storm): the gap anchor FLAPS between the
normal 2.35 and 1.91-2.01 at the SAME rpm (3927: 1.976 vs 3939: 2.378) with the
sync eventIndex jumping 112 <-> 110 - a bistable state flip, not a smooth rpm
effect. Consequence chain: flap sync lands one tooth early -> the position gate
misses the next sync point -> C9002 'too many teeth' (code path: !isValidIndex
= sync point MISSED, not extra teeth) -> C9007 +11.9 deg early / C9008 -15.8
deg late -> instant-rpm reset -> C9011 duplicate queue element + C9009 skipped
sparks + C9351/C9353 overcharges. C9011 is a validation-only warning in
trigger_scheduler.cpp (element already in the queue after a time-based
re-schedule) - a symptom of the storm, not an independent bug.

Root cause: this is the L9779 peak-detector PV level transition (the model's
risk=YES band made observable) - bistable at fixed rpm, threshold-correlated,
index jumps by a whole tooth. NOT the Tfilter eat (t_high/T = 0.46 >> 1/32 at
any rpm) and NOT Tout-min (F phase 126-160 us at 3400-4300 rpm > 100 us spec).
The flap's gap1 is also elevated (1.05-1.08 vs 1.00) - the post-gap tooth pair
distorts in flap mode.

Why the model did not engage: k=0 (first boot, seed) -> Vp = 0 -> everything
trains into L1, so 'no level transition' is by design until k is calibrated;
the flap samples (measuredPitch ~1.09) land inside the [0, 1.5] training band
and pull the single L1 EMA against the normal 0.63-0.65 samples.

FIXED: persistence never worked on a first boot - vrModelStorageRead() left
vrLoaded=false on the 'no stored record'/'invalid record' paths, and
m74_9VrModelPeriodic() gates the saver on !vrLoaded, so the model was never
written (no 'Writing storage ID 7' in either session log). Both failure paths
now set vrLoaded=true (load attempt complete, RAM is authoritative).

| File | Change |
| --- | --- |
| firmware/config/boards/m74_9/m74_9_vr_model.cpp | vrLoaded=true on both read-failure paths |

Validation: unit tests 1163/1163 pass, crankingTransition suite 30/30.

Open: k calibration (flap band 3400-4300 = a PV boundary; trial k via 'vrk',
cross-check 'vrmodel level boundaries at rpm:' against the flap band), rawtrg
DURING the flap (the missing capture), decoder hardening for accepted-flap ->
next-revolution C9002 (position gate vs re-anchor interaction).

## 2026-08-25 (afternoon) - m74_9: VR model save contract + idle-vs-mid shift step (k evidence)

When the model saves (m74_9VrModelPeriodic, 20 Hz): async MFS read done
(vrLoaded=true even on 'no record' after the fix), vrDirty (any training or
vrk), isStopped() (~0.5-1 s debounce), the 10-consecutive-stopped-polls flash
gate (~0.5 s more), 15 s cooldown -> storageRequestWriteID(7). Expected visible
signature ~1.5-2.5 s after 'engine stopped': 'MFS: Writing storage ID 7 ... 40
bytes' + 'vrmodel: stored (N revs)'. The 14:07-14:09 car paste shows stored=no
forever because the flashed build predates the vrLoaded fix - the saver is dead
on first boot there.

New k evidence from the same paste: the L1 EMA converges to 0.610-0.631 at
idle/stop (~1000 rpm, 231 syncs over 14 s of steady idle) vs 0.644-0.651 at
1300-3900 rpm - a ~0.04 pitch step between ~1000 and ~1300 rpm = a
peak-detector level transition (PV1 = 930 mV) at ~1100-1300 rpm -> k ~ 0.7-0.85
mV/rpm. Cross-check: k=0.78 puts PV4 (3000 mV) at 3850 rpm, inside the observed
3400-4300 flap band. Two independent features point at k ~ 0.6-0.8; calibrate
on the car with 'vrk 0.7' and compare 'vrmodel level boundaries at rpm:' with
the observed flap band and the idle step.

Bundle rebuilt with the vrLoaded fix: artifacts/rusefi_bundle_m74_9.zip
(rusefi_development_260825_m74_9_1244431765_local_update.srec).
Validation: unit tests 1163/1163, crankingTransition 30/30, compile_m74_9.sh
BUILD SUCCESSFUL, bundle BUILD SUCCESSFUL.

## 2026-08-25 (late afternoon) - m74_9: root cause of high-rpm sync loss = software edge-polarity read

Retracted the earlier "Mode B = L9779 Tout-min hardware wall" conclusion: the
stock ME17 runs the same L9779 to 7000 rpm, so the chip is not the limit. New
log (efi_log_2026-08-25_14_50_27_870): engine reached 5465 rpm, gap ratio stays
2.34-2.40 at 4900-5500 rpm - the conditioner output is fine at high rpm.

Root cause found in the capture path (analysis, no fix yet):

- m74_9 capture: FAST IRQ (priority 0, handleExtiIsr in digital_input_exti.cpp)
  latches only {timestamp, channel} into the 32-deep queue and STIRs the
  handoff. The EDGE DIRECTION is not captured there.
- The handoff ISR (I2C1_EV, priority 4) pops the queue and calls shaft_callback
  (trigger_input_exti.cpp), which reads the pin level via palReadLine AT
  HANDOFF TIME and calls hwHandleShaftSignal. The whole decode chain runs in
  this same ISR (44 us average, tails to ~1 ms in the triggerIsr histograms).
- When the handoff latency exceeds the inter-edge interval (113 us at 4557 rpm,
  ~250 us at 4000), the level read returns the state of a LATER edge -> the
  logical edge type flips. For RiseOnly decoding one misread = one phantom
  eaten decode edge (deficit) or one phantom extra decode edge (excess).
- This produces exactly the observed high-rpm signatures: count deficits at
  syncs (the "toothloss" lines), C9007 +11.9 deg early / C9008 -11.3 deg late
  pairs (phantom eat -> teeth arrive early, phantom extra -> late), C9002
  missed syncs when 3+ events corrupt in one revolution. Corruption rate grows
  with rpm (interval shrinks, ISR tail does not) - clean below ~2500, ~10% of
  revolutions corrupted at 3500-5500.
- The rawtrg "same-edge runs" at 4557 rpm are the SAME misreads recorded into
  the raw edge ring: the deltas stay a clean ~113 us cadence (timestamps are
  correct) while the R/F flags are wrong. Zero runs at 1181 rpm.
- orderingErrorCounter is disabled for RiseOnly wheels (useOnlyRisingEdgeForTrigger
  guard in trigger_decoder.cpp:526), so the corruption never shows as
  ordering errors.

Fix direction (agreed, not yet implemented): push the pin LEVEL into the queue
from the fast IRQ (2-3 us after the edge - 40x margin at 7000 rpm) and pass it
through the ExtiCallback signature instead of palReadLine in the handoff; or
move to timer input-capture (hardware direction latch). The stock ECU latches
the direction in hardware - that is why it tolerates 7000 rpm.

The VR model side: model persisted across power cycles (stored record loaded,
k=0.699, per-level table), risk=YES at 5465 rpm (near PV4 boundary at k=0.7).

## 2026-08-25 (evening) - m74_9: root-cause fix - edge polarity captured in the EXTI fast IRQ

The high-rpm trigger corruption (phantom eaten/extra teeth, C9002 sync misses,
C9007 +11.9 deg / C9008 -11.3 deg pairs, rawtrg same-edge runs) was NOT the
L9779 output degrading (the stock ME17 runs the same chip to 7000 rpm; the new
log reached 5465 rpm with clean 2.34-2.40 gaps). The polarity of each edge was
RECONSTRUCTED by palReadLine in the handoff ISR (priority 4), which runs the
whole decode chain and can be delayed up to ~1 ms - longer than the 113 us edge
interval at 4557 rpm, so the read returned the level of a LATER edge.

Fix: the fast EXTI IRQ (priority 0, ~2-3 us after the edge) now captures the pin
level together with the timestamp into the queue {timestamp, channel, level};
the handoff passes it to the callback. shaft_callback/cam_callback and every
other efiExtiEnablePin user no longer read the pin. The L9779 output filter
clamps the minimum edge spacing to ~4 us, so the fast-IRQ read is always inside
the quiet zone (20x margin at 7000 rpm).

ExtiCallback signature changed to void(*)(void*, efitick_t, bool level) - all
10 call sites updated (trigger PAL + ADC paths, flex, hellen board ID, logic
analyzer, wifi, mc33810, ion sense, frequency sensor, hella oil level).

Why not the timer ICU (hardware direction latch): STM32_ICU_USE_TIM12/13/14
exists in the at32 mcuconf and the TIMv1 icu_lld compiles, but (1) PF8's timer
AF on AT32F435 is unverified (no AF table in the repo; F429-style TIM13_CH1/AF9
is likely), (2) the ICU driver has never been exercised on the AT32 port, (3)
TMR13 shares its vector with TMR8_UP (F4 layout). The fast-IRQ read closes the
actual bug now; ICU remains a precision/robustness follow-up if the AF is
confirmed.

| File | Change |
| --- | --- |
| firmware/hw_layer/digital_input/digital_input_exti.h/.cpp | level captured in handleExtiIsr, carried in ExtiQueueEntry, ExtiCallback takes bool level, channel stores its line |
| firmware/hw_layer/digital_input/trigger/trigger_input_exti.cpp | shaft/cam callbacks use the captured level, dropped palReadLine + line arrays |
| firmware/hw_layer/digital_input/trigger/trigger_input_adc.cpp | same for the ADC-triggered path |
| 8 other efiExtiEnablePin users | callback signatures updated |

Validation: compile_m74_9.sh BUILD SUCCESSFUL, unit tests trigger suite 54/54
(full 1163 pass earlier), bundle rebuilt
(artifacts/rusefi_bundle_m74_9.zip, srec 260825_1244431765). On-car validation:
rawtrg at 4000+ rpm must now show clean alternating F/R (no same-edge runs)
and the high-rpm C9002 events must be gone.

## 2026-08-25 (night) - m74_9: live dash values over CAN (rpm/tach/CLT/Vbatt/fuel flow/advance)

The BCM emulation (m74_9_can.cpp) was sending STATIC byte fields where the
original ECU broadcasts live values - the user saw frozen dash packets. All
four fields were re-derived from orig_1/2/3.trc (PCAN-View, stock ECU):

- 0x0186[0:1] = instantaneous fuel flow, mL/h big-endian: 0x0000 stopped,
  0x004D..0x00ED (77..237) cranking, peak ~0x0360 (864) at the catch, ~0x026C
  (620) at warm idle, 0x0000 during DFCO (fuel cut) even while rpm climbs.
  Was a static 0x281C (the old author read the idle value ~642 and hardcoded it
  as if it were rpm*16).  Fuel flow is NOT rpm and NOT pulse width - both would
  not zero out in DFCO while rpm rises.
- 0x0189[2:3] = battery voltage, little-endian millivolts: 0x3220 = 12.83 V at
  rest, 0x38CD..0x3CE9 (14.5..15.6 V) while the alternator/bench supply runs.
  Was static 0x2032.
- 0x066A[3] = 0x066A[4] = coolant temperature, raw degC in both bytes: climbs
  only after engine start, saturates at the thermostat (95..98 degC in
  orig_1/orig_3), frozen after shutdown - the dash gauge signature.  Was
  static 0x1D/0x01.
- 0x018A[4] = ignition advance: 256 - 2*advance while running (0xFA = 3 deg at
  the catch -> 0xE2 = 15 deg warm idle, retarding to ~0xF2 = 7 deg during the
  DFCO blip), 0x02 cranking, 0x00 at IGN-on.  Was static 0xE8/0xFE.
- 0x0186[4] = rpm - 768 (dash tach byte) - confirmed exact from the traces:
  775 rpm -> 0x07, 800 -> 0x20, 889 -> 0x79; 0x0189[0:1] = 0x018A[0:1] =
  rpm*16 BE, all three carry the same live value.
- 0x0189[4] = rolling down-counter F0->00 sawtooth while running (freshness
  check), 0x00 at rest; 0x0189[5] = 0xB9 IGN / 0xB8 running; 0x018A[3] =
  0x07 IGN / 0x06 running.

Fuel-flow source: engine->module<TripOdometer>()->getConsumptionGramPerSecond()
* 3600 / 0.745 (g/s -> mL/h at gasoline density).  If the BC reads ~25% low
against a known-good flowmeter, the stock unit may be g/h - drop the density
division.  CLT: Sensor::getOrZero(SensorType::Clt), Vbatt:
Sensor::getOrZero(SensorType::BatteryVoltage), advance:
engine->engineState.timingAdvance[0].

| File | Change |
| --- | --- |
| firmware/config/boards/m74_9/m74_9_can.cpp | encodeFuelFlowMlPerHour / encodeBatteryMillivoltsLE / encodeCltDegC / encodeIgnitionAdvanceByte; 0x0189/0x0186/0x018A/0x066A live fields |

Validation: compile_m74_9.sh BUILD SUCCESSFUL.  On-car check: warm idle should
show live rpm on the tach, ~0.6-0.8 L/h instant consumption on the BC, and the
CLT gauge tracking warm-up.

## 2026-08-25 (night 2) - m74_9: LIN generator parameters extracted from the stock firmware

The 21129's alternator is controlled over LIN by the stock ECU. The user asked
to extract the LIN parameters from Read_FULLFLASH_I865LB52_w2404b1 (Itelma
M74.9, Largus 21129, build 2024-01-10, 4 MB flash dump @ 0x08000000, Cortex-M
Thumb). Static analysis (objdump + table forensics, no Ghidra):

- Flash layout: bank1 (0x08000000-0x081FFFFF) holds the app + calibration
  (vectors at 0x08000000; real startup images at 0x08080000 and in bank2 at
  0x08201000; VRS ramp table verified at 0x0804EF8C). Module name table with
  "LIN", "UART", "GPIO", "IGNCTRL", "KNOCK_DRIVER", "IC_ETC", "IC_EMS" etc.
- LIN driver code at 0x08016680-0x08017000: PID calculator (mask 0x3F +
  P0/P1 parity - classic LIN protected-ID) and the checksum function (16-bit
  sum + carry folds + invert; r3==1 adds the PID = classic LIN 1.x, else
  enhanced LIN 2.x). Baud: 10 MHz / baud divisor computed at 0x0801688E
  (10,000,000 / 19200 = 520) - standard 19200.
- Frame table at 0x08048CC1, 16 slots of {id, len|cks_type}: bit 4 of the
  second byte selects classic(1)/enhanced(0) checksum, bits 3:0 = length.
  Non-trivial frames: ID 0x16 (protected 0xD6) len 6 TX enhanced (the
  alternator CONTROL frame), ID 0x08 (protected 0xC8) len 8 RX CLASSIC (the
  alternator status response), ID 0x08 len 2 and len 3 TX enhanced, ID 0x1D
  (protected 0x9D) len 4 TX enhanced, ID 0x00 = pauses/service slots.
- Frame buffers (flash initial payloads) at 0x08062FB0-0x08063310: contain
  15.0 V in two encodings (0x0096 = 150 dV, 0x3A98 = 15000 mV), 14.8 V
  (0x0094), 800 (0x0320), 0x7FFF sentinels - the ECU's default alternator
  setpoint is 15.0 V.
- Voltage calibration block at 0x08064720-0x08064780 (mV): setpoints/thresholds
  12.8 / 13.6 / 14.0 / 14.4 / 14.8 / 15.2 V plus protection thresholds
  16.4 / 17.6 / 18.4 / 19.2 / 20.0 / 20.8 / 21.6 / 22.4 / 24.0 V. Duplicated
  calibration block at 0x08064D60/0x08065060 (two copies) with 13.0/14.0/
  14.75/12.75 V entries (/32 scale). PID gain block at 0x0806D65A (Q12:
  Kp 0.6, Ki 0.2, Kd 0.125, plus 0.8/7.8/0.278 groups) - the alternator
  voltage regulator gains.
- Schedule machinery: 20-slot task array in RAM 0x2000D89E (12-byte records,
  counter vs period fields).

NOT yet decoded: the exact byte position/scale of the voltage setpoint inside
the 6-byte 0x16 payload (the buffers prove it carries voltage, but the live
frame layout needs either a car LIN capture or deeper decompilation of the
regulator task). Follow-up: LIN sniff on the car (logic analyzer on the
alternator LIN line, or PLIN-USB), then implement a LIN master for m74_9
(rusEFI has no LIN support yet).

## 2026-08-25 (night 3) - m74_9: LIN generator bus protocol from the ST L9918 datasheet (VDA)

The exact 21129 payload layout still needed the "how to work with the bus"
reference. Downloaded the ST L9918 datasheet (alternator voltage regulator
with LIN, "compliant to VDA LIN-Generator-Regulator specification") - the
generic VDA protocol the 21129 regulator follows:

- Physical/data link: LIN 2.2A, 19200, single wire; responses of 2/4/8 bytes
  only; checksum classic (LIN 1.3) or enhanced (2.x) selectable.
- Master control frame (RX for the regulator): setpoint + LRC + current limit
  + output selection. SETPOINT ENCODING (the key): 6-bit A6 = (V - 10.6) x 10
  (0.1 V steps, 10.6-16 V), 8-bit A8 = (V - 10.6) x 40 (0.025 V steps);
  code 0 = 10.6 V = OFF / pre-excitation command (VBSPLINRG = 10.6 V keeps
  EXC off until a valid setpoint arrives).
- LRC fields: B (LRC-Rise, 4 bits: 0-15 s ramp), C (LRC-Cut speed, 4 bits:
  2400-8000 rpm / "always active"), D (excitation current limit 5/7/8 bits:
  0 = no limit, else 0.1-10 A+).
- Regulator response frames: F_T/F_M/F_E fault flags (thermal/mechanical/
  electrical), F_L1/F_L0 LIN error flags, DCE5/8 excitation PWM duty, EXC6/8
  excitation current, alternator ID (AltS/AltL), RB output-confirmation code,
  and the K feedback byte selected by the master's R field: VSPFBK8 setpoint
  echo ((V-10.6)x10), BV8 B+ voltage, TJ8 junction temperature, RPM8
  alternator speed (567-6000+ rpm).

Cross-check with the 21129 firmware frame table: master 0x16 (PID 0xD6) 6
bytes enhanced = the control frame (VDA-style: setpoint + LRC + current limit,
6 bytes = A8 variant with extra fields), response 0x08 (PID 0xC8) 8 bytes
CLASSIC = the regulator's status frame (VDA-style flags + duty + current +
feedback byte; the L9918's Version F Tx_2F is also 8 bytes). The L9918's own
frame IDs (0x20/0x29/0x2A/0x11-0x16) differ - the 21129 chip is a sibling in
the VDA family with a customer frame set, but the field semantics are shared.

Implementing rusEFI LIN master for m74_9: send header PID 0xD6 + 6 bytes
(byte0 = setpoint (V-10.6)x10, 0 = off; LRC/current-limit bytes) with enhanced
checksum at ~100-500 ms, read the 0xC8 8-byte response with classic checksum.
Exact byte positions of the 6-byte 21129 frame still to be confirmed with one
car capture (or by identifying the exact regulator chip).

## 2026-08-25 (night 4) - m74_9: LIN alternator master implementation

Implemented the LIN master + smart-alternator control for m74_9 (the 21129
alternator is LIN-controlled, VDA generator-regulator family; protocol facts
extracted earlier from the stock firmware and the ST L9918 datasheet):

- New board-local driver firmware/config/boards/m74_9/m74_9_lin.cpp/.h:
  USART3 (SD3, PB10 TX / PB11 RX) @ 19200 8N1 with LINEN in CR2, hardware
  break via the SBK bit, classic protected-ID calculator, classic (LIN 1.x)
  and enhanced (LIN 2.x) checksums. Master frame 0x16 (PID 0xD6, 6 bytes,
  enhanced) = setpoint + LRC; status poll on 0x08 (PID 0xC8, 8 bytes, classic).
- Control policy (per user request): OFF while cranking/below 600 rpm; smooth
  load pickup via the LRC-Rise field; OFF above 5000 rpm; OFF during hard
  acceleration (MAP > 80 kPa) capped at 60 s continuous (battery protection).
  Setpoint = code6 (V - 10.6) x 10 from alternatorVoltageTargetTable
  (load x rpm, filled in the msq: 13.8-14.6 V); code 0 = 10.6 V = OFF.
- Tunable board fields via board_engine_configuration.txt (first use on m74_9):
  m74_9LinAltEnabled/MinRpm/MaxRpm/MapOffKpa/MapOffMaxSeconds/LrcRiseCode/
  LrcCutCode/AltFeedbackSel - defaults set in m74_9_boardDefaultConfiguration.
- mcuconf: STM32_SERIAL_USE_USART3 = TRUE (USART6 stays the console).
- Debug: console command 'linalt' prints the gate state, setpoint, LRC codes,
  the last TX frame and the last RX status frame with checksum validation.

The 6-byte payload layout is PROVISIONAL (VDA field order, padded with 0xFF) -
to be confirmed against the real regulator with 'linalt' and adjusted in
buildControlFrame(). Wiring: connector AF3 "Alternator DFM signal" is the
stock LIN wire; it must reach PB10/PB11 on the m74_9 board.

| File | Change |
| --- | --- |
| firmware/config/boards/m74_9/m74_9_lin.cpp/.h | new LIN master + alternator control |
| firmware/config/boards/m74_9/board_engine_configuration.txt | new tune fields |
| firmware/config/boards/m74_9/board_configuration.cpp | defaults, init + periodic hooks |
| firmware/config/boards/m74_9/board.mk | compiles m74_9_lin.cpp |
| firmware/config/boards/m74_9/21129.msq | alternator voltage table + bins filled, new fields |
| firmware/hw_layer/ports/at32/at32f4/cfg/mcuconf.h | STM32_SERIAL_USE_USART3 = TRUE |

Validation: compile_m74_9.sh BUILD SUCCESSFUL (config regen picked up the new
board fields; ini has m74_9LinAlt*). On-car: enable m74_9LinAltEnabled, watch
'linalt' (TX 6 bytes, RX 8 bytes + classic checksum OK) while the engine runs.

## 2026-08-25 (night 5) - m74_9 LIN alternator: 5 s off-hold after gate events

Added hysteresis to the alternator OFF gates: after any of the immediate OFF
events clears (MAP gate, low rpm / cranking, over-rev above 5000) the
generator stays off for m74_9LinOffHoldSeconds (default 5 s) more. During a
gear shift the MAP dips below the 80 kPa threshold for a moment - without the
hold the generator switches on and immediately off again; with the hold the
OFF state persists through the dip. The timer re-arms while any event is
still active and counts down once all events clear. New tune field
m74_9LinOffHoldSeconds (board_engine_configuration.txt, default 5, shown in
the 'linalt' console output as hold=X.X/Y.Ys). Build: BUILD SUCCESSFUL.

## 2026-08-26 - m74_9: burn-gate fix + LIN tick moved off SysTick (crash fix)

Two user-blocking regressions found and fixed on the bench:

1. TS burn always refused with the engine stopped. custom_board_allowTsBurn
   (board_configuration.cpp) copied the stoppedPolls >= 10 debounce from
   custom_board_allowFlashNow, but unlike the storage-manager gate (polled
   every 100 ms while a write is pending) the TS-burn hook runs ONCE per burn
   request - the counter could never reach 10, so every burn was skipped
   ("TS burn skipped - engine is running" with the engine off, config never
   persisted). Fixed: single isStopped() check, debounce removed. The
   flash-now gate keeps its debounce - it is polled periodically, so it works
   as designed. Also retitled the misleading log message in tunerstudio.cpp
   ("TS burn skipped (board policy)").

2. ECU halt on m74_9LinAltEnabled=Enabled. Crash marker: SV#10 (misplaced
   I-class), xlock caller inside L9779::writePad with ipsr=15 - the debug aid
   proved the m74_9 slow callback (custom_board_periodicSlowCallback) runs in
   the ChibiOS virtual-timer callback INSIDE the SysTick ISR (periodic_task.h
   even warns about it). The LIN tick was hooked there and did blocking
   serial I/O (chnWrite/chnReadTimeout, S-class) from ISR context -> ChibiOS
   class check -> halt -> reboot loop, config not applied. Fixed: the LIN
   control tick now runs in its own LOWPRIO thread (lin alt, 100 ms) started
   in initM74_9LinAlternator(); the call was removed from the board slow
   callback. m74_9VrModelPeriodic stays in the slow callback (no blocking
   I/O there - its flash gate defers while running).

LIN RX hardening in the same change: the status-poll parse now takes the LAST
9 bytes of whatever was received (robust to the UART queueing the own-break
as a 0x00 in LIN mode - the old fixed 2-byte echo read desynced the response
by one byte), and linalt prints rxBytes (raw byte count of the last poll) to
distinguish a dead wire (0) from a responding-but-unparseable slave (>0).

PID parity note: the stock frame-table extraction recorded PID 0xC8 for
status ID 0x08, but standard LIN parity makes that impossible (P0 =
ID0^ID1^ID2^ID4 = 0 for 0x08, so the PID can only be 0x08 or 0x48). 0xC8
was a mis-extraction; linComputePid(0x08) = 0x08 is correct and matches the
0x16->0xD6 / 0x1D->0x9D standard-parity records.

Validation: compile_m74_9.sh BUILD SUCCESSFUL; on the bench linalt now shows
state=charging, target=14.5V -> code6=39 (0x27), tx=27 A5 00 02 FF FF,
txFrames climbing with no reboot. RX still 0 bytes - the alternator is not on
the PB10/PB11 wire yet (LIN wiring unconfirmed then); rxBytes=0 in the next dump
will confirm the wire is dead, >0 means the slave answers and the
layout/checksum needs work.

## 2026-08-26 (late) - m74_9 KiCad: ALT DFM wiring is read-only DFM monitor, NOT LIN

Traced the ALT DFM net in /Users/vladimir/rusefibuildcontainer/repo/m74.9/hardware/m749.kicad_sch
(MCU instance U5 at (860.425, 351.79), pins resolved via lib_symbol pin offsets):

- connector label "ALT DFM" (left sheet edge, y=335.28) is the alternator wire;
  same-name labels chain it through the hellen module to "I_ALT DFM"
- "I_ALT DFM" (module right edge, (911.86, 379.73)) is wired DIRECTLY to MCU
  PB5 (pin 135, abs (890.905, 379.73) - verified by pin-coordinate match)
- the net passes through input conditioning (series R, C to GND) and a
  74HC14 Schmitt gate (U4 unit 3 at (661.67, 156.21) sits between the two
  wire segments at y=156.21)
- PB10 (pin 69, abs (890.905, 367.03)) and PB11 (pin 70, abs (890.905,
  364.49)) have NO wires at all on the schematic - they float

Consequence: the board reads the stock DFM signal (one-directional, Schmitt-
conditioned) into PB5. It cannot carry the bidirectional LIN bus, and PB5 has
no USART3 AF. The LIN master (USART3 on PB10/PB11) is unwired on this board;
rxBytes=0 on the bench is expected. Hardware rework is required to connect
the alternator LIN line to PB10/PB11 (+ ~1k pullup to VBAT, tap the wire on
the connector side before the 74HC14 so PB5 keeps working as the DFM monitor).

## 2026-08-26 (correction, user fact) - m74_9 LIN wiring: schematic is NOT trustworthy

User fact (recorded verbatim): the m74_9 block is a STOCK ECU and the stock
firmware controls the alternator over LIN - so the LIN path exists in the
hardware. The KiCad schematic is badly drawn (it already disagrees with the
physical board elsewhere, e.g. the DIS->+3V3 vs +5V netlist error noted for
the ETB chain). Therefore the ALT DFM -> 74HC14 -> PB5 trace above must NOT
be treated as the real board state; it is one more schematic-vs-board
disagreement. Source of truth for the LIN wire: buzz the connector pin to the
MCU pins on the physical board (and check the pullup), not the schematic.

## 2026-08-26 - m74_9 LIN pin options on AT32F435 (all LIN-capable UART pins)

User asked how many AT32F435 pins can drive a LIN bus (since the stock box is
wired for LIN and we get no answer - we may be on the wrong pins). Facts:

- AT32F435 has 7 UARTs: USART1/2/3, UART4/5, USART6, UART7 (no UART8,
  AT32_HAS_UART8=FALSE in the AT32F435_437 registry). ALL support LIN mode
  (SBK break generation + LINEN) - LIN is a standard USART feature.
- Pin options (Artery AF table, mirrors STM32F4 AF layout; USART1-3 = MUX 7,
  UART4/5 + USART6/7 = MUX 8):
  USART1: TX PA9/PB6, RX PA10/PB7
  USART2: TX PA2/PD5, RX PA3/PD6
  USART3: TX PB10/PC10/PD8, RX PB11/PC11/PD9
  UART4:  TX PA0/PC10, RX PA1/PC11
  UART5:  TX PC12, RX PD2
  USART6: TX PC6/PG14, RX PC7/PG9
  UART7:  TX PE8/PF7, RX PE7/PF6
  13 TX/RX pairs, 26 pins total.
- On m74_9 known-taken pins that rule out UART7 and part of USART6: PF5=CLT,
  PF6=IAT, PF8=trigger(VR), PF7/PF9/PF10 analog/digital inputs; PC6/PC7 are
  ADC channels. Free candidates: PB10/PB11 (currently wired in code, no
  answer), PC10/PC11 + PD8/PD9 (still USART3 - same SD3 driver, only the two
  pin constants change), PA2/PA3 (USART2), PA9/PA10 (USART1), PD5/PD6, PD2/
  PC12, PG9/PG14.
- The L9779 SBC has K_LINE/K_TX/K_RX (ISO9141 K-line transceiver), NOT LIN.
  The board has NO LIN transceiver and no master pullup in the schematic -
  direct UART drive (push-pull TX + RX on one wire) is the only option, with
  the pullup added on the wire.
- Next step: buzz the connector pins to find the real MCU pin (schematic
  unreliable), then repoint LIN_TX_PIN/LIN_RX_PIN (and SDx if not USART3).

## 2026-08-26 - m74_9 dash CAN: tach byte fit + rpm hold + flow smoothing

Re-derived the dash encodings from orig_1/2/3.trc (bench captures, revs to
~915 rpm):

- CORRECTION (user, verbatim): 0x0189[0:1] and 0x018A[0:1] ARE live rpm -
  the bench captures sit in the 771-790 idle band simply because the engine
  idled; 0x0186[2:3] carries the same rpm with more lag. All three rpm*16
  fields are live in the stock. The dash rpm must mirror the Java console
  rpm exactly (no dropouts there -> a dash dropout is an encoding problem,
  not a sensor problem).
- Tach byte fit: 0x20 at the 800 rpm rest/idle baseline, 0x33-0x35 at the
  ~1100 rpm rev blip -> byte = rpm/16 - 18 (~16 rpm per count). The old
  rpm-768 formula saturates at 1023 rpm and does not match the captures.
- Fuel flow 0x0186[0:1]: the stock zeroes it during DFCO (flow 0x0000 while
  rpm climbs - the DFCO signature).

Changes in m74_9_can.cpp:
- encodeTachByte(): rpm/16 - 18, 0x20 at rest.
- Tach byte encoding is switchable on the car: 'cantach <0..3>' console
  command (0=rpm/16-18 capture fit, 1=rpm-768 old, 2=rpm/8-68, 3=rpm/32+7).
  The bench captures never exceed ~915 rpm, so the dash's real scale above
  idle is unknown - pick the mode whose needle agrees with the console rpm
  at a known speed. NO rpm hold-through-dropout: the dash rpm is the live
  sensor value exactly as the console shows it (per user: the console has no
  dropouts, so the dash problem is the encoding, not the source).
- Fuel flow: zeroed during DFCO + EMA smoothing (~200 ms) - the trip-odometer
  per-event rate is spiky and froze garbage at the 10 ms sampling rate.
- CLT was already live in 0x066A[3:4]; no change.

Open: the bench captures never exceed ~915 rpm, so the tach byte above idle
is a fit, not a measurement. The 700-800-at-real-2500 dash symptom is also
consistent with the trigger desync at 2500-3000 zeroing the rpm sensor (the
active high-rpm desync issue) - the hold mitigates it, the desync is the root
cause. A car-side capture of OUR CAN frames while driving would settle the
dash's exact scale.

## 2026-08-26 - m74_9 DFM wiring buzzed out: AF3 -> 74HC14 -> PC10

User buzzed the physical board: the ALT DFM signal goes AF3 -> R/C network ->
74HC14 (Schmitt input) -> 74HC14 output -> AT32 **PC10**. (The KiCad
schematic's PB5 drawing is wrong, as suspected.)

Firmware: PC10 = USART3_TX (MUX7), PC11 = USART3_RX (MUX7) - the SAME USART3
(SD3) the LIN master already uses, so only LIN_TX_PIN/LIN_RX_PIN changed in
m74_9_lin.cpp (B10/B11 -> C10/C11).

Hardware for the bidirectional LIN bus (still needed):
- TX: PC10 must reach the wire on the 74HC14 INPUT side - the 74HC14 is a
  one-way buffer, TX cannot cross it backwards. Jumper PC10 to the AF3 wire
  (or cut the 74HC14-output trace); the 74HC14 output on PC10 weakly fights
  the UART driver through its series R - bypass it for a clean bus.
- RX: PC11 must reach the same wire point.
- Add the ~1k master pullup to VBAT (no LIN transceiver on the board; the
  L9779 only has K-line, so the UART drives the bus directly).

## 2026-08-26 - m74_9 buzzed wiring: final LIN + DFM map (user-buzzed, authoritative)

- DFM monitor: sits on **PC11 through the 74HC14** (user fact, verbatim) -
  the AF3 "ALT DFM" wire -> R/C -> 74HC14 -> PC11 chain is the alternator
  DFM read, NOT the LIN path.
- LIN physical layer = the L9779 ISO9141 K-line transceiver:
  PD8 (USART3_TX) -> L9779 K_TX (pin 47); L9779 K_RX (pin 46) -> PD9
  (USART3_RX); L9779 K_LINE (pin 45) -> AG3 (the LIN wire to the alternator). All
  buzzes confirmed. Matches the stock firmware (LIN driver uses USART3, port D).
- Firmware now uses LIN_TX_PIN=D8 / LIN_RX_PIN=D9 (same SD3, AF7).
- Closed: K_LINE (pin 45) -> AG3 confirmed. Open: verify K-line polarity/
  ISO_SRC (CONFIG_REG4) empirically via the echo test in linalt.

## 2026-08-26 - m74_9 LIN wiring COMPLETE (all buzzed, user facts)

- L9779 K_LINE (pin 45) -> **AG3** (buzzed, user fact) - the LIN wire to
  the alternator. The DFM monitor is a SEPARATE connector pin: AF3
  "ALT DFM" -> R/C -> 74HC14 -> PC11.
- Full chain:
  PD8 (USART3_TX) -> L9779 K_TX (47); L9779 K_LINE (45) -> AG3 ->
  alternator; L9779 K_RX (46) -> PD9 (USART3_RX). Bus pullup to VBAT
  present on AG3. DFM monitor separately: AF3 -> 74HC14 -> PC11.
- Firmware: LIN_TX_PIN=D8, LIN_RX_PIN=D9 (SD3/USART3, AF7). Built.
- Next: flash, linalt -> expect the own-echo through the K-line transceiver
  (rxBytes >= 2-3 even with the alternator disconnected); with the
  alternator connected, rxBytes >= 11-12 and cks=OK. Watch polarity - if
  the echo parses, the K-line transceiver is non-inverting as expected.

## 2026-08-26 - m74_9 ignition-gated power stage: analysis (no code yet)

User request: the L9779/TLE power side must be OFF with ignition off and
fully re-initialize on ignition on. Analysis + isIgnVoltage() verification.

isIgnVoltage() chain (firmware/controllers/ignition_controller.cpp:12):
- EFI_PROD_CODE: if isBrainPinValid(ignitionKeyDigitalPin) -> efiReadPin
  (pin, mode). The digital pin ALWAYS wins over the analog fallback.
- Fallback (only when no valid pin): SensorType::IgnKeyVoltage if present,
  else BatteryVoltage > 6.0 V.

On m74_9 the tune CANNOT unset the pin: m74_9_boardConfigOverrides()
(board_configuration.cpp:199) assigns ignitionKeyDigitalPin =
Gpio::L9779_PIN_KEY (280) on EVERY boot - loadConfiguration() runs
custom_board_ConfigOverrides AFTER readFromFlash()
(engine_configuration.cpp:673), so the board assignment always wins over
the stored msq. The "if the tune does not set ignitionKeyDigitalPin"
concern does NOT apply to this board.

If it were NOT set (other boards / hypothetical m74_9 misconfig): the
fallback reads BatteryVoltage > 6 V. The m74_9 MCU is permanently powered
from VBAT through the L9779 (KEY is only a logic input to the chip), so
the fallback would be TRUE with the key off - the ignition gate would
never close: LimpManager keeps fuel/spark/ETB enabled, MainRelayController
never shuts off, cranking protection gone, and a future L9779 power gate
would never disarm. The forced pin assignment is load-bearing.

L9779_PIN_KEY read path: efiReadPin -> gpiochips_readPad(280) ->
L9779::readPad(0): returns 1/0 from the key_on_status cache (DIA_REG9
bit 7, KEY_ON_STATUS), refreshed by the driver thread at the ~100 ms diag
cadence (refresh_diag_cache, l9779.cpp:837). key_on_valid=false until the
first refresh, so isIgnVoltage() reads FALSE for the first ~100 ms after
boot (safe default, but a real KEY latency for the planned gate). The
DIA_REG9 read clears nothing on the chip, so polling it fast is safe.

What isIgnVoltage() gates today:
- LimpManager::updateState (limp_manager.cpp): m_ignitionOn=false clears
  allowFuel/allowSpark (ClearReason::IgnitionOff) and cuts the ETB power
  stage, except under directSelfStimulation.
- IgnitionController::onSlowCallback: broadcasts onIgnitionStateChanged to
  all engine modules (0.2 s transient filter on the falling edge).
- MainRelayController::onSlowCallback (main_relay.cpp): main relay +
  delayed shutoff.
- start_stop.cpp: cranking is refused without ignition.
- tle9201.cpp diag thread: ETB diagnostics only while ignition on.

Power-stage gate plan (L9779, not implemented yet):
- Disable: CONFIG_REG6 PSOFF (bit 4) = 1 (write 0x16) kills the power
  stages but keeps the chip logic alive (regulators, SPI, WDA, KEY_ON).
  The MCU keeps running - the regulators are unaffected by PSOFF.
- Current boot behavior to change: chip_init() runs at power-on and writes
  L9779_CONFIG6_PWR = 0x06 (PSOFF=0 - stages ON) + START + RESPTIME + VRS
  before ignition, so the power side arms with the key off.
- Planned life cycle: ignition off -> no boot chip_init (or REG6=0x16), no
  WDA feed, slow KEY_ON polling; ignition rising edge -> need_init = true
  -> chip_reset() + chip_init() + update_output() + WDA kick (the existing,
  proven re-init path); ignition off after running -> write REG6=0x16,
  stop the WDA feed.
- TLE9201 side is separate: PB13/ETC_EN is driven high at board init
  (m74_9_boardInitHardware) and is not gated by ignition yet.

## 2026-08-26 - m74_9: "throttle alive for hours" root cause + BCM main-relay hypothesis

User facts: the ETB blade stays energized for hours after key-off and only
full power removal (unplugging the ECU) resets it; hypothesis that the
stock firmware sends the BCM a packet on some timeout to cut the main relay.

Throttle-alive root cause (fully explained by the board wiring + our code):
- The L9779 SBC holds VCC from VBAT (KEY is a logic-only input), so the MCU
  runs indefinitely with the key off.
- m74_9_boardInitHardware() drives PB13 (ETC_EN) HIGH at boot ->
  Q5A (NPN, inverts) on -> TLE9201 DIS low -> H-bridge enabled. Nothing in
  the current firmware ever de-asserts PB13 or de-arms the TLE at runtime.
- Result: the blade stays energized while VBAT is present. Unplugging
  collapses VCC -> PB13 floats -> Q5A off -> DIS pulled high -> tristate.
  That is the observed "reset".
- Fix = the planned ignition gate: drive PB13 low + PSOFF on the L9779 when
  isIgnVoltage() is false.

BCM main-relay hypothesis - analysis of the 8 trc captures:
- ignoff.trc (IGN off, 2186 frames, ~4 s): ZERO ECU dash frames - 0x0189,
  0x0186, 0x018A, 0x066A, 0x01F6, 0x0217 all absent. The remaining traffic
  (0x0090, 0x0242, 0x029A, 0x029C, 0x0211, 0x0214, 0x0352 + the ~100 ms
  burst group) is other nodes (dash/cluster chatter).
- Every IGN-on capture (ignon, ignon_and_start, orig_1/2/3, rusefi_*) streams
  the ECU dash frames at 10 ms.
- Conclusion: at ignition-off the stock ECU's CAN node is COMPLETELY SILENT.
  No dedicated "cut the main relay" packet exists in any capture. The
  observed "timeout" is most likely the BCM-side heartbeat timeout: the BCM
  watches the 0x0189 10 ms stream and drops the main relay when it dies.
- Caveat: no capture spans the key-off transition (all end with the stream
  still flowing), so a final "going to sleep" packet right before silence
  cannot be fully excluded. Settle it with a bench capture across key-off:
  start recording with IGN on, turn the key off, keep recording ~30 s.

Consequence for rusEFI: the ignition gate must also stop the dash CAN stream
when isIgnVoltage() is false (stock = silence). Keeping 0x0189 alive with a
"not running" state would make the BCM/dash treat the ECU as alive and the
main relay may never drop.

## 2026-08-26 - m74_9 ignition-gated power stage: IMPLEMENTED

The gate from the analysis above is implemented (build passes, not yet
flashed/bench-verified):

- l9779.cpp: `L9779_CONFIG6_PSOFF (0x16)`; `chip_power_off()` (driver thread
  context): wd_running=false FIRST, then scheduler.cancel(&wd_sched), then
  REG6=0x16. The executor feed checks wd_running at its entry and fizzles
  out without rescheduling - a plain cancel is racy because the feed
  self-reschedules at its end (a concurrent executor run would resurrect
  it). The driver thread's need_init/WDA-kick/output pushes are all gated
  on power_stage_on; the diag refresh (KEY_ON polling) runs while parked.
  The OUT_DIS heal is gated on power_stage_on (chip_heal_out_dis(true)
  writes REG6=0x06 and would fight the gate). l9779_setPowerStage(on):
  ISR-safe flag + thread wake; default ON, so non-m74_9 boards are
  unchanged. Boot safety: the flag is set false in
  m74_9_boardInitHardware BEFORE gpiochips_init creates the driver thread,
  so the boot chip_init never arms the stages with the key off - the chip
  stays in its power-on-off state (OUT_DIS=1).
- board_configuration.cpp: PB13 (ETC_EN) boot default changed HIGH -> LOW
  (deterministic tristate); m74_9IgnitionGatePeriodic() in the 20 Hz slow
  callback (SysTick ISR - flags/GPIO only) flips PB13 and calls
  l9779_setPowerStage() on isIgnVoltage() changes. Key-on-at-boot costs one
  extra PSOFF write + ~100 ms (DIA_REG9 KEY cache latency) before the
  stages arm; key-off PSOFFs immediately.
- m74_9_can.cpp: sendPeriodic() returns immediately when !isIgnVoltage() -
  the whole dash/IMMO/keepalive stream stops (stock = total ECU CAN silence
  at ign-off, the BCM drops the main relay on the heartbeat timeout). The
  console ISO-TP path is rx-driven and unaffected.

Boot sequence now: power-on -> PB13 low, L9779 parked (single REG6=0x16
write, no chip_init, no WDA) -> KEY_ON polling fills the cache -> gate
raises PB13 + full SW_RST re-init on key-on. Fail-safe: if the L9779 SPI
never answers, the gate stays parked (stages off).

Open/not done: bench+car verification (expect: no power-stage activity with
ignition off; init sequence on key-on; BCM drops the main relay once the
dash stream dies; no re-init storm on quick key flicks), the LIN thread is
not gated (it keeps sending the OFF setpoint - harmless), and the self-stim
bench path stays parked without ignition (turn the bench key on for output
tests).

## 2026-08-27 - m74_9 LIN on the car: transceiver alive (own echo), regulator silent

First on-car linalt after the K-line rewiring (260827 bundle): rxBytes=2 vs
0 before - the L9779 K-line transceiver echoes the master's own poll header
(sync + PID) back on RX. The physical path PD8 -> K_TX -> K_LINE(45) -> AG3
-> wire and K_RX(46) -> PD9 is CONFIRMED alive; the 2-byte echo (not 3)
shows the break does not queue as a byte - clean LIN-mode UART.

But the regulator does not answer: rxBytes=2 = header echo only, no 8-byte
status frame in the 30 ms window, frames=0. Prime suspect: the status poll
PID. The stock's frame table stores the FULL PID byte 0xC8 for id 0x08
(LIN 1.x parity); we computed the LIN 2.x PID 0x08 and the regulator stays
silent. Note 0xD6 (control) is identical under both conventions, so only
the status frame was affected.

Changes: linStatusPid defaults to 0xC8 (stock literal); 'linpid <hex>'
console switch for live A/B on the car; the classic checksum verification
now uses linStatusPid too (classic cks covers PID+data, so the PID byte
must match); linalt prints the RAW received bytes - the next log shows the
echo content: 55 C8 ... = non-inverting transceiver + correct header on the
wire, AA xx ... = inverting transceiver, >= 11 bytes = slave responding.

Next on the car: run linalt. If raw=55 C8 (2 bytes, no response) try
'linpid 08' to close the A/B; if still silent the next suspects are the
control-frame checksum convention (enhanced vs classic), the LRC/control
payload layout and the CONFIG_REG4 ISO_SRC K-line config.

## 2026-08-27 - m74_9 LIN: polarity verified, PID ruled out, control-frame checksum next

On-car iteration:
- raw echo = 00 55 C8 / 00 C8 with the engine running at 974-1808 rpm:
  the full poll header (break+sync+PID) is visible on RX, PID 0xC8 confirmed
  on the wire - regulator still silent (rxBytes 2-3, frames=0). The PID
  hypothesis is CLOSED (0x08 and 0xC8 both tested).
- readpin PD9 = 1 at idle: the K-line transceiver is NON-inverting (double
  inversion ruled out) - the regulator sees the same clean signal we echo.
- Remaining top suspect: the regulator rejects the CONTROL frame (checksum
  convention: we send enhanced/LIN 2.x, the extraction also notes the
  regulator speaks LIN 1.x classic) and stays inactive, never answering the
  status poll. Added 'linck classic|enhanced' runtime toggle + ctlCks in the
  linalt print. If classic does not wake it either: break length (SBK = 13
  bits exactly, the spec minimum) and the provisional control payload layout
  are next.

## 2026-08-27 - m74_9 LIN: checksum ruled out, baud suspect (echo is baud-blind)

User confirmed the regulator has power. Both control-frame checksum
conventions tested on the running engine - still silent. Datasheet check
(l9779wd-spi.txt, Table 39): VKOUTL row proves K_TX low -> K_LINE low
(non-inverting TX); readpin PD9=1 + clean echo prove non-inverting RX -
the regulator sees the same clean signal we echo.

KEY INSIGHT: the own echo CANNOT reveal a baud error - TX and RX share the
same UART clock, so the echo always self-parses. Everything verified so far
(echo/polarity/PID/checksums) is baud-blind. If the AT32 USART clock tree
gives anything but a true 19200, the regulator sees garbage break/frame
timing and stays silent.

Added: lastBreakUs measured around the SBK spin (NT clock) and printed in
linalt - at a true 19200 the 13-bit break reads ~677 us (1354 us = 9600).
Plus 'linbrk double|single' (extended break) and 'linctl on|off' (poll-only
mode) toggles to close the break-length and control-frame-poisoning
hypotheses in one flash.

## 2026-08-27 - m74_9 LIN ROOT CAUSE FOUND: wrong control id (0x16 -> 0x1D)

Disassembled the stock LIN driver (0x8016778 frame-builder loop) and read
its literal: the frame table IS at 0x08048CC1, 16 slots of 2 bytes
{id, len|cx}, stride 2 (ldrb [r5, r4<<1] / ldrb [r5+2s+1]). Slots 2-5:
{0x1D len 4 enhanced}, {0x08 len 8}, {0x16 len 6 enhanced}, {0x08 len 2}.
The stock's PID function (0x80166e2) is the STANDARD LIN 2.x parity.

L9918 datasheet (Table 11, 15 configs): NONE listen on 0x16/0x08. The
control (Rx) frame is id 0x1D (29) = Rx_A/Rx_D, 4 bytes, in Version A/B/D.
The stock's 0x1D len-4 TX frame matches Rx_A EXACTLY. The earlier
"0x16 = alternator control" extraction was WRONG - 0x16 is an L9918 TX
(slave->master) id (LIN2/LIN4 Version A Tx_3A), the regulator never
listens on it. Everything we sent was correct EXCEPT the control id.

L9918 Rx_A 4-byte layout (5.3.1.1): [0] A6 = setpoint code6 (bits 5:0);
[1] B1 = LRC-rise (3:0) | C1 = LRC-cut (7:4); [2] D5 = excitation current
limit (4:0); [3] R (2:0: 001=setpoint echo, 010=VB+, 011=Tjunction,
100=rpm) | BZ (3) | F (6:4) | WB (7).

Firmware changed: control id 0x16/6B -> 0x1D/4B (PID 0xDD computed by the
same standard parity), enhanced checksum, L9918 Rx_A layout. Status poll
stays on 0x08 (the stock's 8-byte RX frame - the actual chip is an
L9918-family part whose TX is 0x08/8B, not the bare L9918).

## 2026-08-27 - m74_9 LIN: control layout fixed - 8-bit setpoint (Version B), not 6-bit

L9918 datasheet: id 0x1D is used by Version B/D/E/F, whose Rx setpoint is
8-bit (A8, Table 62 VSPFBK8B: V = 10.6 + code x 0.025; 0x98 = 14.4 V) -
Version A (6-bit code6) lives on id 0x29, which the stock does NOT use.
The stock's default frame buffer parses cleanly as the Version-B layout:
[0x98 = 14.4 V, 0x3A (LRC rise 10/cut 3), 0x1E = 30 (excitation limit),
0xFF (RB invalid, BZ/F/WB defaults)]. Firmware now sends [setpoint8, LRC,
0x1E, 0xFF] on 0x1D - the previous code6 frame read as ~11.6 V garbage on
this scale. Status poll unchanged (0x08, 8 bytes).

## 2026-08-27 - m74_9 LIN: correct frame still silent - status-id scan tooling

The 0x1D control frame with the 8-bit setpoint (stock default bytes
[0x98,0x3A,0x1E,0xFF] format) is still unanswered. The stock's frame-table
region turned out to be 184-byte per-frame structs at 0x08048CC0 (index
map at 0x08048C78, `mla r0, r0, #184` in the init fn), so the earlier
2-byte {id,len} read was an artifact - the status id is NOT proven to be
0x08. 'linpid <id>' now takes the RAW LIN id and computes the standard PID
(the stock's PID fn is the standard parity, verified by disasm), so all
candidate status ids (L9918 TX family 0x0B/0x0C/0x0D/0x0E/0x0F/0x10/0x11/
0x12/0x14/0x15/0x16/0x18/0x21 + 0x08) can be scanned live on the car in
one flash. rxBytes >= 3 beyond the echo = the regulator answered.

## 2026-08-27 - m74_9 LIN BREAKTHROUGH: regulator answers id 0x12, control is 0x29

Remote PCAN scan on the running engine (MCP ecu server + linpid):
- id 0x12 answered: raw=00 55 92 32 01 3A -> echo + response 32 01 with
  classic checksum 3A = ~(0x92+0x32+0x01) - VERIFIED on the car.
- 0x12 = L9918 Tx Frame 1 version-B (Table 17: 0x12 (LIN1)) = the
  identification frame (AltS/AltL/DieS/DieL).
- The control id is therefore Rx_B = 0x29 (Table 16: 0x29 (LIN1)) - the
  datasheet column is HEX; my decimal conversion (29 -> 0x1D) was the error
  that cascaded through the last iterations. Tx_2B (0x15, the 4-byte
  status) stayed silent because it waits for a valid control frame on 0x29.
- Firmware: control id now 0x29/PID 0xE9 (4B, Version B 8-bit setpoint),
  runtime switch 'linctlid <id>'; status default 0x12/PID 0x92, parse =
  2 data + classic cks, decoded as altS/altL/dieS/dieL; 'linpid' still
  switches the poll id live.

## 2026-08-27 - m74_9 LIN: link live, tuning the charging (WB=1 was killing it)

Remote linalt on the car (MCP over PCAN): the regulator answers id 0x12
steadily (frames=40, rx=32 01 cks=3A). Two issues:
- RX read split: a single chnReadTimeout returned 2 or 5 bytes (the burst
  available at first-byte wake-up) and missed the trailing checksum -
  the parse flickered (rxBytes=5, BAD). Fixed with a read loop (30 ms first
  wait, 10 ms follow-ups) until >= 6 bytes or an idle gap.
- The user reports NO CHARGE with the accepted control frame [setpoint,
  LRC, 0x1E, 0xFF]: 0xFF has WB=1 = L9918 "regulation Without Battery"
  loop parameters, and 0x1E caps the excitation at 30 units. Corrected
  defaults: byte2 = 0x7F (max limit), byte3 = 0x02 (RB=2 Vmeas, WB=0).
  Live tuning switches added: 'linctl2 <hex>' / 'linctl3 <hex>', and the
  linalt first line now prints vbatt for the remote charging check.

## 2026-08-27 - m74_9 LIN: charging restored (WB=1 was the killer), RX windows widened

Remote verification (MCP over PCAN, engine idling): vbatt=13.9V rising -
the corrected byte2/3 (0x7F excitation limit, 0x02 RB=2/WB=0) restored the
charging that 0xFF (WB=1 = "regulation Without Battery") had killed. The
regulator answers id 0x12 but intermittently: some polls get the full
32 01 3A response, others only the first data byte (rxBytes=4 = echo + 32)
- the answer sometimes starts tens of ms late, and the 30/10/10 ms read
windows gave up mid-response. Windows widened to 50/20/20/20 ms (worst
case ~130 ms per silent poll, tick period grows to ~230 ms - the regulator
LIN COM timeout tolerates this easily).

## 2026-08-27 - m74_9 LIN: classic checksum confirmed, linset runtime override

Live A/B on the car (MCP over PCAN): with 'linck classic' on control id
0x29 the battery climbed 13.13 -> 14.05+ V and holds - the regulator
follows the setpoint (battery reads ~0.35 V below the alternator B+
setpoint - normal harness drop at idle). With enhanced checksum the frame
is ignored (voltage floats at the regulator's ~13.9 V default). So the
control frame = id 0x29, 4 bytes, 8-bit setpoint, CLASSIC checksum.
Added 'linset <volts>' - runtime setpoint override (0 = back to the
alternatorVoltageTargetTable) and made classic the default cks. The table
field itself: array S16 @16264, [4x4], scale 0.1 (ini).

## 2026-08-27 - m74_9 LIN: poll-outcome counters + control on alternate ticks

The regulator answers only ~4% of polls (47/1174) - the misses correlate
with polls sent right after the control frame (its LIN block busy with the
setpoint). Changed: the control frame goes out every OTHER tick, so the
status poll alternates with a quiet ~100 ms window; added poll outcome
counters (full/partial/none) and the last response delay to linalt.
Charging itself is unaffected (vbatt=14.17 following the 14.6 setpoint).

## 2026-08-27 - m74_9 LIN: post-flash analysis of the poll counters (16:08 log)

First live data from the alternate-tick build (eaea6891a2d) on the car:
polls=270, full=27 (10%), part=50 (18.5%), none=193 (71.5%), respMs=69,
txFrames=135 = polls/2 - the every-other-tick control send works exactly.

Key findings:
- Charging confirmed live: vbatt=14.16V at a 14.6V setpoint (control
  frame A0 A5 7F 02 accepted). Regulation does NOT depend on the status
  response - the setpoint rides on the control frame alone.
- Every response that ARRIVES parses 100% (frames=27 == full=27): the
  response is 2 data bytes + checksum 3A = ~(0x92+0x32+0x01), a
  PID-inclusive sum (the code's "classic"). The 'BAD' label in the
  linalt rx line refers ONLY to the LAST poll (2 echo bytes read), not
  to the accumulated good frames - no checksum bug.
- respMs=69: the regulator answers ~65-70 ms after the header. A LIN
  slave must answer in-slot (~1-5 ms); 69 ms means the chip services the
  LIN block on its own slow internal loop and answers only headers it
  catches idle. Inter-byte gaps >20 ms truncate started responses
  (part=50) against the 50+4x20 ms read window.
- The tick period drifts: the thread sleeps 100 ms AFTER the tick, and
  the tick itself blocks 0-130 ms on the reads -> real poll period
  100-230 ms, sampling the chip's internal schedule at a drifting phase
  - the likely reason only 10-28% of polls get any answer.
- The raw= line mixes the current poll (2 echo bytes 00 92) with stale
  bytes from the previous poll (92 32 01 3A = PID + data + cks) -
  lastRxRaw is not zeroed between polls; rxBytes= belongs to the current
  poll only.

Open leads (analysis-first, no code yet per user request):
- The stock polls status on id 0x08 (PID 0xC8), 8 data bytes (the
  frame-table extraction) - we poll 0x12 (Tx_1B identification, 2
  bytes). 'linpid 08' live test first; if it answers better, the parser
  must handle an 8-byte response.
- Slow the poll cadence (every 2nd/3rd tick) to give the chip's slow
  LIN loop more quiet windows; or make the tick period deterministic
  (sleep = 100 - tick duration) to stop the phase drift.

## 2026-08-27 - m74_9 dash tach: 0x0186[4] is a plausibility cross-check, not the needle

Symptom: at 800-900 idle the dash tach showed 100-200 (new firmware),
and earlier at a real 2500 it showed 700-800 (old firmware).

Re-analysis of all trc captures + the firmware history:
- The stock byte 0x0186[4] = rpm - 768 EXACTLY across the full captured
  range (702-912 rpm, slope 1.0, three files): 775->0x07, 800->0x20,
  889->0x79, rest = 0x20. No capture exceeds 912 rpm, so the high-rpm
  byte was previously unproven.
- The dash needle follows the 16-bit rpm fields (0x0189[0:1] = rpm*16
  BE, same in 0x0186[2:3]/0x018A[0:1]); byte 4 is a PLAUSIBILITY
  CROSS-CHECK. When it diverges from the cluster's own rpm estimate,
  the cluster ignores/holds the needle (stale or limp value).
- Evidence: old firmware (clamp(rpm-768)) read correctly at idle (byte
  82 = 850-768 passes) but failed at 2500 (saturated 255 vs the stock's
  wrapped 196) -> needle held at ~700-800. The rpm/16-18 default failed
  at idle (byte 35 vs 82) -> needle held at ~100-200. No simple
  needle = f(byte4) formula fits all observations; the cross-check
  model fits every one.
- Fix: new default tach mode 4 = (uint8_t)(int32_t)(rpm - 768.0f) -
  the stock formula with u8 wrap-around (196 at 2500). The float->uint8
  direct cast would be UB for negative rpm, so it goes through int32.
  Old modes 0-3 stay switchable live via 'cantach <0..4>' for the
  on-car A/B. Built: m74_9 compile passes, fresh
  firmware/build/rusefi.srec ready to flash.
- To validate on the car WITHOUT flashing first: 'cantach 1' (clamped
  rpm-768, byte-exact vs stock at idle) - the needle must jump to
  ~800-900 at idle, confirming the cross-check model at idle; then
  flash mode 4 and check a held ~2500 rpm reads ~2500.

## 2026-08-27 - m74_9 idle torque reserve via ignition advance (stock scheme, msq)

User decision: the stock ECU regulates idle torque ONLY through ignition
advance - fixed throttle blade, no rpm-target or air adders (fan, brake
creep, electrical loads must not sag rpm). The architecture was already
in the tune (useIdleTimingPidControl=yes, air RPM PID zeroed) but the
reserve was EMPTY: idleAdvance base 26-30 deg at 700-1100 rpm vs
idleTimingPid_maxValue +30 -> ~3 deg of headroom, so any load saturated
the PID instantly; plus idleTimingPid_iFactor=0 (pure P -> permanent
droop).

msq changes (21129.msq):
- idleAdvance 700/900/1100/1300: 30/26/27/27 -> 10/10/12/14 (retarded
  base = reserve; 1600: 22 -> 20 for a smooth ramp, 400 stays 20 as the
  dip floor).
- idleTimingPid: max +30 -> +20 (total ceiling base 10 + 20 = 30 deg),
  min -30 -> -10 (total floor 10 - 10 = 0 deg, never over-retard),
  p 0.08 -> 0.15, i 0 -> 0.03, soft entry 3.0 -> 1.0 s.
- Air loop untouched (idleRpmPid_* still zero, fan adders 0) per the
  user's scheme. If timing-only (~15-20% torque) is not enough for the
  stock brake-creep feel, the second layer is the air RPM PID with small
  authority - same knobs.

Also committed in this file (user's own console edits, part of the same
work): alternatorVoltageTargetTable all 14.6 V, cltIdleCorrTable cold
row +4%, postCrankingFactor tail columns zeroed.

## 2026-08-27 - m74_9 idle: open-loop base explained, base set to 3.5% (msq)

Question answered (no code change): the idle open-loop base is
cltIdleCorrTable[targetRpm][CLT] x etbIdleThrottleRange / 100 - the
table is indexed by the TARGET rpm (m_lastTargetRpm in
IdleController::getRunningOpenLoop), not instant rpm. With range=20 the
warm (90C, 900rpm) cell 11.8 -> 2.36% real opening.

Dashpot/Taper Initial IAC Adder = iacByTpsTaper: extra air held at full
value for iacByTpsHoldTime after TPS drops below
idlePidDeactivationTpsThreshold, then decayed to zero over
iacByTpsDecayTime (idle_thread.cpp getRunningOpenLoop L188-211). Same
units as the table (scaled by range/100). Currently 0/0/0 = off. With
useIacTableForCoasting=enabled the coasting table iacCoasting replaces
the whole open-loop path during Coasting, so the dashpot only acts once
the engine is in Idling.

User decision: warm idle base ~3.5%. Applied surgically - only the
cltIdleCorrTable 900rpm row warm cells changed (40/60/90/110/140C:
14.0/15.0/17.5/18.0/18.5), cold cells untouched, so cranking/fan/PID
scaling stays as-is. 17.5 x 20/100 = 3.5%; fan adds +5 -> 4.5%.
Deliberately NOT etbIdleThrottleRange 20->30 (would give 3.54% but
scales cranking +50% and everything else - would need cltCrankingCorr
/1.5).

Commit 7a3815ce570 carries the full current on-car tune state (the msq
also holds the user's own console edits since e1dd91948d0: idleRpmPid
p=1.5/i=0.009/d=0.5 +/-6, fan1/2ExtraIdle 5, pidExtraForLowRpm 20,
useIdleTimingPidControl=no, iacCoasting 6/5/3.5/3/3.5/3, coastingFuelCut
disabled, cranking_rpm 400, LIN minRpm 300/mapOff 90/hold 2s, lambda
table 0.898 low-load, fan soft start 10s, disable fans when stopped).

## 2026-08-28 - OpenBLT CAN flash speed: analysis supplemented (code-verified)

Old hypothesis "ECU takes 1-2 ms per PROGRAM_MAX frame" disproven by
tracing the whole path: bootloader_main.cpp loops BootTask() with zero
sleeps while connected; BootTask = CopService+TimerUpdate+ComTask+
BackDoorCheck; ComTask polls one CanReceivePacket(TIME_IMMEDIATE) per
pass -> XcpCmdProgramMax -> FlashBufferedWrite (4-byte line buffer) ->
intFlashWrite (1 word flashProgram ~20-40 us). ECU = 50-200 us/frame.

Budget of the measured 1.90 ms/frame: PCAN-USB dongle USB round trip
~1.0-1.2 ms (host floor - the <1 ms histogram bucket has ZERO frames),
bus pair 0.55 ms @500k, ECU 0.1-0.2 ms. Single-frame protocol floor at
500k = ~0.55 ms/frame = ~53 s. The fork's xcp.c advertises no block
mode (blockSize=0), but both protocol ends are ours.

Plan: (A) host-only pipelining (K=8-16 frames burst, drain K ACKs) ->
~60-75 s, zero firmware risk, do first; (B) true batch mode (RAM page
buffer + one NvmWrite + one ACK per batch) -> ~35-45 s @500k; (C) 1
Mbit only as multiplier with B -> ~20-25 s (bench-only). bxCAN 3 RX
mailboxes are safe: ECU consumption (~100-200 us) > bus delivery
(~265 us/frame); erase busy-wait never overlaps host traffic.

## 2026-08-28 - OpenBLT CAN fast flash: plan A implemented (host pipelining)

Implemented the host-only pipelined PROGRAM_MAX path in the Java flasher
(commit 6d896aacb70), no bootloader change:

- XcpClient: writeProgramMax() (fire) + readProgramAck() (in-order ACK
  collection) + getInFlight() bookkeeping; per-frame RTT histogram stays
  accurate for pipelined frames (send timestamps queued).
- OpenBltCanFlasher.programSegment: pipelined mode keeps at most
  PIPELINE_WINDOW = 3 frames in flight (bxCAN 3 RX + 3 TX mailboxes, so
  the ECU can never block on the ACK transmit or overflow RX - verified
  against openblt_can.cpp / com.c) and paces the TX at 620 us/frame
  (~265 us frame + ~265 us ACK at 500 kbit/s) so ACKs interleave with
  frames instead of the dongle bursting back-to-back. Tail settles the
  pipeline and finishes with single-frame PROGRAM. Lost ACK -> clear
  "no acknowledgement" FlashException (no resync magic: the AT32 flash
  rejects reprogramming words, so a resend is not safe anyway).
- --no-pipeline flag restores the one-request-one-reply path.
- Config.pipelinePaceNanos (0 in unit tests - the synchronous fake link
  has no bus, and pacing would add ~7 s to the 80 KB test).
- Tests: pipelinedFlashProgramsEveryFrame (142 PROGRAM_MAX + tail,
  byte-exact), lostAckAbortsPipelinedFlash (dropProgramMaxResponses hook
  in FakeCanLink), noPipelineFlagStillFlashes. 28/28 green, fatJar builds.

Expected on the bench: 182 s -> ~65-90 s (pace 620 us/frame vs measured
1.90 ms/frame). Next levers if needed: plan B (true batch mode with a
RAM page buffer + one ACK per batch, ~35-45 s @500k) and plan C (1 Mbit
only together with B, bench-only).

## 2026-08-28 - OpenBLT CAN fast flash: plan A finished, window-only mode default

Bench iterations (694,840 bytes, 99,313 frames, same jar+link):
- window 3 + 620us pace after write: 125.1 s (ACK wait serialized)
- window 8 + 620us pace after write: 134.6 s (pace loop spun 1ms/empty read)
- window 8 + poll-based pace: 111.9 s
- window 8 + pure-sleep pace measured from frame START: 90.8 s
  (write 418 us/frame = blocking MacCAN Write, pace 216, ack-wait 171)
- window 8 + --pace-us 0 (window-only): 68.5 s, 0.69 ms/frame
  (write 312, pace 0, ack-wait 271) - verification passed, zero frame loss

0.69 ms/frame = the 500 kbit/s bus cycle floor: 265 us host frame + ~150 us
ECU + 265 us ACK. Plan A (host-only) is done at ~68 s; window 8 is loss-free
against the hardware bound of 10 (3 processed + 1 in progress + 6 FIFO0/1
slots). Window-only flow control made the default (pipelinePaceNanos = 0);
--pace-us 620 restores the spaced-out mode. RTT histogram is a drain-latency
artifact in pipelined mode - trust "Program loop timing" / "Effective rate".

Next (needs firmware + host): plan B batch mode - RAM page buffer, one
NvmWrite + one ACK per batch -> ~35-45 s @500k; plan C 1 Mbit multiplies B.

## 2026-08-28 - OpenBLT fast flash: plans B+C implemented (firmware + host)

Plan B - deferred-ACK batch programming (firmware ext/openblt submodule +
Java flasher):
- New XCP command 0xC8 PROGRAM_BATCH [len_le32]: declares a batch of up to
  2048 bytes at the current MTA; the following PROGRAM_MAX frames fill the
  static 2 KB RAM buffer WITHOUT per-frame responses; the last frame triggers
  one NvmWrite of the whole batch + a single response. The AT32 per-word
  program cost (512 words ~13 ms) is amortized; ctoPending semantics verified
  (ComTransmitPacket clears it, intermediate frames set ctoLen=0).
- Erase/program-reset cancel any pending batch. MTA still auto-increments by
  7/frame; the host SET_MTA's before each batch (1 frame per 2 KB).
- Host: programSegmentBatched() is the DEFAULT; no flow control needed (the
  blocking MacCAN write ~0.3 ms/frame is slower than the bus, the bootloader
  consumes far faster than either). --no-batch falls back to the window-8
  pipelined mode, --no-pipeline to single-frame. Friendly hint when an OLD
  bootloader rejects 0xC8.

Plan C - runtime CAN baudrate switch (1 Mbit only for erase+program):
- New XCP command 0xC7 SET_CAN_BAUDRATE (data[1]: 0=500k, 1=1M). The
  response is sent at the OLD speed; the bootloader MAIN LOOP applies the
  switch (canStop+canStart+filter re-program) ~5 ms later via
  OpenBltCanApplyBaudrate(). Safety: if no traffic arrives within 5 s at the
  switched speed, the bootloader reverts to 500k on its own, so the
  console/car bus can never be stranded at 1M.
- Host: --1mbit flag; switchBaudrate() = SET_CAN_BAUDRATE -> 30 ms settle ->
  adapter re-init -> GET_STATUS retries at the new speed. The switch back to
  500k happens in a finally around erase+program, so verify/reset and the
  console/car bus always end at 500k. PcanLink.setBaudrate() re-initializes
  the dongle (Uninitialize + Initialize with PCAN_BAUD_1M).
- Bootloader build passes (openblt_m74_9.elf, +2 KB BSS for the batch
  buffer). Java: 30/30 tests green (new: batchedFlashProgramsEveryFrame,
  noBatchFlagUsesPipelinedMode, lostBatchAckAbortsFlash,
  oneMbitSwitchHappensAroundProgramming; FakeCanLink mirrors the batch +
  baudrate protocol).

Deployment note: the new flasher REQUIRES the new bootloader on the ECU
(0xC8/0xC7 are rejected by old ones). Install via a full rusefi.bin flash
(bundle build merges the rebuilt bootloader); rusefi_update.srec does NOT
touch the bootloader region. Expected on the bench: batch @500k ~35-40 s
(host write-bound at ~0.3 ms/frame); --1mbit ~30-35 s (the MacCAN write is
the floor, the 1M wire helps only marginally on this dongle).

## 2026-08-28 - OpenBLT 1M wedge: IWDG safety net (bootloader + app)

Field result: plan B works (45.2 s, 42 ms per 2 KB batch, write-bound), but
--1mbit WEDGED the ECU: after "Firmware ... 1 segments" the link died and
only a power cycle recovered. Root cause chain: the bootloader had NO
hardware watchdog (HAL_USE_WDG off, CopInitHook empty), and the CAN restart
path (canStop/canStart in OpenBltCanApplyBaudrate) wedges the main loop on
the AT32 port (likely the INRQ wait during the error state - the exact hang
point is still unproven, needs the crash markers / a bench probe). The 5 s
software fallback never fired because the main loop itself was dead.

Fix (commit 07aad871352):
- Bootloader: HAL_USE_WDG=TRUE + IWDG started in CopInitHook (500 ms
  nominal, ~410 ms real at the 40 kHz LSI; > the ~50 ms erase and ~13 ms
  batch-write busy-waits). Any wedge resets the bootloader back to 500k in
  <1 s; the reset-loop counter (max 10 WDG resets) keeps a wedged-flash
  scenario from boot-looping into erased flash.
- App (m74_9): the IWDG cannot be stopped across the jump, so the app
  re-configures it to 4 s (nominal; ~3.3 s real, > the 2.3 s MFS GC stall)
  in m74_9_boardInitHardware and feeds it every 50 ms from the 20 Hz slow
  callback (SysTick virtual timer - keeps firing through thread stalls, so
  no spurious resets while the core runs). Board-scoped HAL_USE_WDG=TRUE
  in m74_9/board.mk; STM32_WDG_USE_IWDG=TRUE in the shared at32 mcuconf
  (inert for other boards).
- Host: switchBaudrate retries use 300 ms GET_STATUS timeouts (bounded
  ~7 s instead of the previous 20 x 5 s = 100 s "hang"); the switch-back
  in the finally logs a warning instead of masking the original error;
  the 1M failure message tells the user the ECU self-recovers.

Status: 1M may still not come up on the MacCAN dongle (unproven); the ECU
is now safe either way. Normal flash (batch @500k) = 45 s. Open follow-up:
pinpoint the CAN-restart hang (crash markers after a wedge, or a bounded
INAK wait in the AT32 CAN LLD).

## 2026-08-29 - OpenBLT 1M switch reworked as a REBOOT (commit 8da0208d689)

The runtime canStop/canStart switch kept wedging the AT32 CAN peripheral
(user re-test after the IWDG work: "does not answer at 500k either", the
main app + console link stay healthy - the wedge is specific to the
bootloader's runtime CAN restart, and the IWDG did not recover it because
the wedge is not a dead main loop). Decision: the only exercised CAN init
path is boot-time CanInit, so the baudrate switch is now a REBOOT.

Change inventory:
| Area     | Change                                                       |
|----------|--------------------------------------------------------------|
| boot     | SET_CAN_BAUDRATE answers at the old speed, stores the rate   |
|          | in SharedParams slot 4 (survives reset - .shared section),   |
|          | reboots; CanInit applies it on the next boot                 |
| boot     | 1 Mbit backdoor window 2 s -> 6 s: the 5 s no-traffic        |
|          | fallback to 500k now fires while the bootloader still runs;  |
|          | 6 s > 5 s is a load-bearing ordering invariant               |
| boot     | fix extern "C" on shared_params.h in openblt_can.cpp         |
|          | (first build failed: C++-mangled SharedParamsWriteByIndex)   |
| host     | switchBaudrate re-CONNECTs at the new speed (the XCP session |
|          | dies with the reboot - GET_STATUS could never work); tight   |
|          | no-sleep retry loop bounded by baudReconnectMs               |
| host     | 1M failure is no longer fatal: adapter-refuses-1M and        |
|          | ECU-silent-at-1M both wait for the ECU's self-recovery and   |
|          | continue the flash at 500k (MacCAN has no 1M support)        |
| host     | PcanLink: currentBaudrate nulled on failed Initialize, so a  |
|          | failed 1M switch does not leave the adapter uninitialized    |
| tests    | FakeCanLink models the ECU-side reboot (session drop, speed  |
|          | gate) + the 500k self-recovery; 2 new fallback tests         |

Recovery chain (why the ECU can never be stranded): host cannot/does not
follow to 1M -> no valid traffic -> 5 s fallback reboots to 500k -> 500 ms
backdoor -> app at 500k; a host CONNECT at 500k triggers the app jump back
into the bootloader, so every ECU state answers.

Validation: :openblt_can:test 14/14 green (incl. oneMbit switch + 2
fallbacks), bootloader builds, full m74_9 bundle builds (zip refreshed with
the merged image). NOT yet bench-tested: user must full-flash
deliver/rusefi.bin (openocd), then: --probe, normal flash (~45 s), --1mbit
(expected: completes ~35 s, OR logs "continuing at 500 kbit" if MacCAN has
no 1M - either way the ECU must stay responsive without a power cycle).

## 2026-08-29 (2nd) - OpenBLT: 1M erase died on the FIRST PROGRAM_CLEAR - the 32 KB chunk raced the IWDG

The reboot-based 1M switch WORKS ("Link switched to 1 Mbit" - re-CONNECT at
1M succeeded, MacCAN CAN do 1M). The new failure was the very first erase:
PROGRAM_CLEAR(0x08008000, 32768) timed out after 15 s and the ECU never
answered again at 1M - it had reset itself (console finds the app at 500k
afterwards). Root cause: the AT32 erase is ASYNC (start sector, poll the
OBF bit every 5 ms), and a 32 KB XCP chunk = 8 x 4 KB sectors busy-waits
~400-460 ms (measured 461 ms max RTT on the bench) INSIDE NvmErase - the
bootloader main loop never runs, so BootTask/CopService never feeds the
IWDG (window ~411 ms at the 40 kHz LSI). Pure phase race per chunk: the
500k batch run (45.2 s, 22 chunks) survived by feed phase, the 1M run
tripped on the first one. The old "flash erase ~50 ms" comment was per
SECTOR, not per chunk - that estimate was wrong.

Fix (commit 35def4be192): wdgResetI between sectors inside intFlashErase
(HAL_USE_WDG guard). A long healthy multi-sector erase is now safe at any
window; a HUNG single sector still trips the watchdog (~50 ms un-fed).
Bootloader rebuild + bundle refresh done. Open follow-up: user full-flashes
deliver/rusefi.bin and retests --probe / plain flash / --1mbit.

## 2026-08-29 (3rd) - OpenBLT: 1M switch+erase now WORK, but batch-at-1M silently corrupts the image

The reboot-based switch and the IWDG fix held: all 22 erase chunks completed
at 1 Mbit and the switch back to 500k worked. The new failure: the final
BUILD_CHECKSUM caught a corrupted image (ADD11 0x9C vs 0x74 - the deferred-
ACK batch mode had shifted bytes). Root cause: batch mode sends ~293
PROGRAM_MAX frames per 2 KB batch with ONE ack; a single lost frame silently
shifts the whole batch buffer. At 1 Mbit the wire margin is 4x smaller, so
dropped frames are a real risk on this MacCAN/PCAN-USB bench (the ECU cannot
overrun - batch-mode consumption is a 7-byte memcpy per frame, far faster
than the host's ~0.2 ms/frame write rate; the loss is on the wire/dongle).

Fix (host-only, commit e60e08f9024): when the 1M switch succeeds, the
flasher forces the ACK-per-frame pipelined mode for the program phase (the
500k path keeps the faster batch mode). Any lost frame now aborts loudly
instead of corrupting the image. No bootloader change - no re-flash needed;
the script auto-rebuilds the jar. Expected 1M time: ~30-45 s (pipelined),
500k batch stays ~45 s.

Open follow-up: the SECOND run in the same session showed dirty-session
artifacts (garbage GET_ID station-id length 16842760 and a BUILD_CHECKSUM
answered with a bare FF) - almost certainly fallout from the corrupted app
left on the ECU by the failed 1M run. Re-verify a clean 500k flash after
this fix; if the garbage recurs on a clean bench, instrument the response
matching.

## 2026-08-29 (4th) - OpenBLT: 1M program phase DISABLED - MacCAN TX corrupts frames after the switch

The ACK-per-frame 1M run reproduced the EXACT same checksum offset as the
batch run (+40 ADD11, 0x9C vs 0x74) while every frame was CRC-valid and
ACKed by the ECU. CAN CRC is end-to-end: a delivered frame's data bytes are
bit-exact what the transmitter sent - so the wrong bytes were formed
HOST-side (MacCAN driver or the PCAN-USB dongle after the mid-session
baudrate re-init). The bootloader is exonerated; nothing firmware-side can
fix a host-side TX corruption.

Second, even on a perfect adapter 1 Mbit cannot pay for itself on this
stack: two switch reboots (~7 s) + the ACK-per-frame program mode (~38 s)
measure SLOWER than the 500k deferred-ACK batch (45.0 s measured today,
verified). Plan C is therefore retired: --1mbit now flashes at 500 kbit
with an explanatory message; the whole switch mechanism (bootloader
reboot-based SET_CAN_BAUDRATE + host switchBaudrate + fallback + tests)
stays in the tree behind --1mbit-force for a future working driver.

State of the flashing story: 265 s -> 68.5 s (plan A) -> 45.0 s (plan B,
500k batch, VERIFIED twice today). Open follow-up: none for speed - the
remaining floor is the MacCAN USB round trip (~1 ms/frame), hardware.

## 2026-08-29 (5th) - m74_9: ДПКВ через ICU (TMR13) - полный анализ, сравнение задержек с EXTI

Code-verified analysis (no hardware test yet) of moving the crank input
from the EXTI two-stage capture to the timer ICU, superseding the option-B
notes of 2026-08-25.

Hardware facts:
- PF8 has ONLY TMR13_CH1 as its capture timer (already mapped in
  stm32_icu.cpp getIcuParams: F8 -> ICUD13/CH1/GPIO_AF_TIM13=9). TMR13 is
  a ONE-channel timer, so the stock-style atomic direction latch
  (PWM-input mode, IC1+IC2) is IMPOSSIBLE on this pin - the ME17 does it
  with a 2-channel timer. A 2-channel option exists only if the AT32F435
  has a second timer AF on PF8 (TIM8_CH3 candidate, unverified - the repo
  has no AT32 AF table). TIM8 is free on m74_9: its PWM pins are
  C6/C7/C8/C9 and no board output uses them (C6/C7 are ADC).
- TMR13 clock = TIMCLK1 = PCLK1 x 2 = 288 MHz (PPRE1=DIV2, hal_lld.h) ->
  PSC=71 gives exactly the NT domain (4 MHz, 250 ns, 32-bit wrap 1073 s,
  WrapAround62-compatible). A boot-time offset calibration (TIM5.CNT -
  TIM13.CNT, drift 0 - one PLL) puts captures in the NT timebase.

Latency comparison (edge -> decode):
- EXTI today: timestamp read in the fast IRQ ~1.5-3 us after the edge with
  +-0.5-1.5 us jitter (IRQ entry + EXTI PR RMW + palReadLine + TIM5 read);
  polarity read 2-3 us after the edge against the L9779 ~4 us min spacing
  (margin 1-2 us - the tightest point of the chain). Handoff pri 4 start
  ~5-10 us + up to ~100 us executor preemption; decode 44 us avg.
- ICU B1 (TMR13_CH1, both edges via CC1P=0/CC1NP=1, pin read for
  direction): timestamp latched in CCR1 AT the edge, jitter 0, quantization
  250 ns; ISR ~1-2 us (SR+CCR1+pin, no EXTI PR RMW); direction race
  UNCHANGED (1 channel). Handoff/decode unchanged.
- Absolute gain is small: 2-3 us = ~0.1 deg at 7000 rpm (148 us tooth),
  fractions of a percent in the [1.6, 3.75] gap ratio - the C9002 storms
  were polarity (a full tooth = 6 deg), already fixed by option A. ICU is
  hardening, not a fix for a live problem.

Improvements vs the old option-B list:
1. PARALLEL EXTI+ICU validation mode (the key addition): timer AF input
   and EXTI coexist on one pin (EXTI watches the level regardless of GPIO
   mode). Run both, tag rawtrg entries with the source, compare - one test
   proves the PF8->TMR13 AF9 candidate AND measures the real timestamp
   error before cutover.
2. CC1OF overcapture flag -> new lost-edge counter (coalesced EXTI edges
   are invisible today).
3. Keep the hardware ICxF input filter OFF by default (it would blind
   rawtrg noise diagnostics); keep the software debounce untouched.
4. Reuse the 32-deep queue and the pri-4 handoff; priority invariants
   (executor 3 > handoff 4) unchanged.

Blocker (NEW, concrete): VectorF0 (TMR8_UP/TMR13) is ALREADY emitted by
  the PWM driver because STM32_PWM_USE_TIM8=TRUE without
  STM32_TIM8_SUPPRESS_ISR (hal_pwm_lld.c emits OSAL_IRQ_HANDLER(
  STM32_TIM8_UP_HANDLER)=VectorF0). Enabling STM32_ICU_USE_TIM13 would
double-define VectorF0. Fix: STM32_TIM8_SUPPRESS_ISR TRUE (safe on m74_9)
  + STM32_TIM13_SUPPRESS_ISR TRUE + one combined VectorF0 handler serving
  icu_lld_serve_interrupt(&ICUD13). Both-edge capture needs CC1P=0/CC1NP=1
  written after icuStart (outside the ChibiOS ICU API, port-local code).

Open follow-ups: verify the PF8 AF on the bench (parallel mode), then an
opt-in B1 build; B2 (atomic direction) only if a 2-channel AF on PF8 is
found in the Artery RM.

## 2026-08-29 (6th) - m74_9: CLT/IAT cold-range failure + voltage wander - root cause is the 5V-bias/3.3V-ADC topology + the 2.0 divider

Analysis of why CLT shows ~-30 at +5C and why the values wander with
supply voltage (code-verified, no hardware measurement yet).

Chain (all verified): NTC + 1500 ohm pull-up to +5V (L9779 VTRK,
battery-tracking) -> ADC3 slow sampling (VDDA 3.3V from L9779 V3V3) ->
adcGetRawVoltage (counts x tune adcVcc 3.3 / 4095, NO VREFINT correction
on this path - getMcuVrefVoltage() is diagnostics-only) -> x 2.0 (global
analogInputDividerCoefficient; EFI_ADC_32/39 are not special-cased, only
MAP/TPS/PPS 1.555 and lambda EFI_ADC_37 1.0 are) -> ResistanceFunc with
HARDCODED 5.0V supply (init_thermistors.cpp configure(5.0f, ...), no bias
voltage config field exists) + open-circuit check V>4.9V -> Steinhart-Hart
-> Sensor::getOrZero.

Root cause math:
- With a 5V bias and a 3.3V ADC reference the measurable range ends at
  R = 1500 x 3.3/(5-3.3) = 2912 ohm: CLT ~+24C, IAT ~+19C. Everything
  colder saturates the ADC at 3.28V - the cold range is INVISIBLE by
  topology, no firmware can recover it.
- The 2.0 divider pushes the operating point onto the diverging hyperbola
  R = 1500/(5/V - 1): at V=4.83V R=42k (-24C), V=4.87V R=58k (-30C),
  V=4.89V R=82k (-45C). "+5C shows -30C" is exactly this edge - tiny
  voltage movements there swing the temperature by tens of degrees.
- The hot range is ALSO wrong with the 2.0 divider: real 90C -> ~66C,
  real 100C -> ~78C. The fan thresholds 95-97C are UNREACHABLE - the fan
  can never turn on with the current coefficients.

Voltage dependence (user question) - CONFIRMED structurally: the reading
depends on TWO independent rails and the firmware assumes both fixed:
(a) the 5V bias rail VTRK (battery-tracking; VDD5_OV/V3V3_UV transients
already observed via DIA10) with supply hardcoded 5.0, and (b) the 3.3V
ADC reference with a nominal adcVcc and no VREFINT rescaling. The
sensitivity to the rails is MAXIMAL at the cold end (near the saturation
knee / hyperbola edge) - exactly where the user sees the values wander
(cold start, cranking).

Fix plan:
1. Firmware-only, immediate: return 1.0 for EFI_ADC_32/39 in
   getAnalogInputDividerCoefficient (same as the lambda EFI_ADC_37 fix).
   The whole measurable range becomes exact (90C -> 90C), the fan becomes
   reachable, the cold end gives an honest monotonic ~24C clamp instead of
   phantom/invalid. Tradeoff: the 4.9V open-circuit check can never fire
   on a 3.3V ADC, so a disconnected sensor shows ~24C.
2. Optional: rescale raw volts with getMcuVrefVoltage() instead of the
   nominal adcVcc - removes the VDDA-sag component.
3. Hardware (the real cold-range + rail-independence fix): move the
   1500 ohm pull-ups from 5V to VDDA 3.3V - bias and reference become one
   rail (fully ratiometric, sag cancels), the full -40..+130C range is
   visible, and the firmware supply must become 3.3 (needs a bias-voltage
   config field instead of the hardcoded 5.0).

Diagnostics available now: rawClt/rawIat gauges (~6.5V saturated x2
before the fix, ~3.28V after) and cltResistance/iatResistance output
channels (0 = invalid before the fix, ~2912 ohm clamp after). Also verify
the msq curve against the real sensor (measure R at +5C with a
multimeter; if it reads ~1.4k and not ~8.4k the curve is not this NTC).

## 2026-08-29 (7th) - CORRECTION of the entry above: the thermistor bias rail is battery-TRACKING VTRK (VBATT/2.5), not regulated 5V

The previous entry's core assumption (1500 ohm pull-up to a fixed 5V,
therefore the 2.0 divider is wrong and the hot zone reads low) is WRONG.
The user's observation that the hot zone reads correctly (a few degrees
off at most) is what exposed it; the logs then confirmed:

Hard facts from the MLGs:
- internalVref (measured VDDA via VREFINT) = 3.37V in 08-24 (car), 08-25
  and 08-28 (bench) - the L9779 V3V3 stabilizes the ADC reference
  properly (the user's "L9779 should stabilize power" instinct - correct
  for the 3.3V rail).
- 08-25 bench fit: rawclt = 0.702V (scaled x2.0) and coolant = 89.5C are
  EXACTLY consistent with the resistance math (0.702 -> 245 ohm ->
  89.5C). The chain is exact for whatever it is fed; there IS a real
  2:1 divider between the thermistor junction and the ADC pin (otherwise
the cold readings would saturate into P0118, not show -30).
- 08-24 car warm idle: rawclt = 0.66V = a ~95C thermostat reading only
  if the bias rail is ~5.6V (VBATT ~14.0), NOT 5.0V.

Model that fits ALL observations: the thermistor pull-up rail is the
L9779 VTRK tracking supply = VBATT/2.5 (battery-proportional by design,
"5V" only at a 12.5V battery), while the firmware hardcodes 5.0V in
init_thermistors.cpp configure(5.0f, ...). At VBATT 14.4 (VTRK 5.76):
+5C real reads -33C (the user's -30), 90C reads 86.4, 100C reads 94.7 -
the hot zone is "a few degrees off" exactly as the user said. At VBATT
12.5 (VTRK 5.0) everything reads exact. This is ALSO the mechanism
behind "values wander with voltage": the bias is f(VBATT) by
construction. The cold-end error explodes because the junction voltage
approaches the rail, where the R = 1500/(5/V-1) hyperbola is steepest.

Corrected fix plan:
1. Firmware (the fix): use the REAL bias = measured VBATT x 0.4
   (VTRK = VBATT/2.5 - confirm the ratio in the L9779 datasheet section
   on VTRK1/2; make it a board constant). VBATT is already measured on
   PA6 (8.02 divider, verified). The ResistanceFunc supply must be
   updated from the battery measurement, not fixed at 5.0. DO NOT change
   the 2.0 divider to 1.0 - that was the wrong fix and would break the
   hot zone.
2. Optional: VREFINT rescale of raw volts - cosmetic, VDDA is stable.
3. Hardware (final): move the 1500 ohm pull-ups to VDDA - fully
   ratiometric, battery-independent, full cold range visible.

Bench verification (5 min, multimeter): the CLT connector pull-up rail
at two battery voltages - expect VBATT/2.5 (12.5V -> 5.0, 14.4 -> 5.76).
P0118/P0113 in the logs are the CORRECT open-circuit detection (open
input = rail through the 2:1 divider = >4.9V scaled), not a bug.

## 2026-08-29 (8th) - mcp_can: PCAN-USB .trc dump tool (can_dump.sh)

The user needed a way to capture CAN dumps with the STOCK ECU from the
Mac (previously done on Windows with PCAN-View). Implemented
com.rusefi.candump.CanDump in the mcp_can module + java_console/bin/can_dump.sh.

Design: PCAN-View .trc v1.1 output (opens in the same tooling and the
existing parsers), listen-only mode by default (the adapter never ACKs,
so the capture does not disturb the ECU), all std+ext frames with the
driver's 1 us timestamps (host nanoTime fallback), inline status rows
for bus transitions via PCAN_ALLOW_STATUS_FRAMES, header with the
PCAN-View STARTTIME serial (local-as-UTC / 86400000 + 25569 - verified
against orig_1.trc), Ctrl+C flush via shutdown hook, --duration option.

Validation: 8/8 unit tests (CanDumpFormatTest - row layout, STARTTIME
serial, hex, status names); smoke-tested on the bench - captured live
m74_9 dash frames (0x0189/0x0186/0x018A at the 10 ms cadence) with
correct sub-ms timestamps. Committed 3a0e973cda4.

Known limits: error frames only if the driver delivers them (USB
adapters usually don't); RTR frames carry DLC but no data bytes; the
adapter is exclusive - close the console/flasher before capturing.

## 2026-08-29 (9th) - stock-ECU dash CAN dumps analysis (19:14/19:27 PCBUSB captures)

Two fresh stock-ECU captures from the car (ignition-on + warmup at idle)
were decoded to re-verify the dash encodings against rusEFI's
implementation (m74_9_can.cpp):

Verified (rusEFI already matches):
- 0x0189/0x0186/0x018A [0:1] = rpm x 16 BE; rest baseline 0x3200 = 800
  (BCM "calibration loaded" gate); real idle today = 0x30B2 = 779 rpm.
- 0x0186[4] = rpm - 768 u8 wrap: 779 -> 0x0B, 800 -> 0x20 - exact.
- 0x0189[2:3] = battery LE mV (12.83 rest -> 15.86 with the stock's
  alternator; the stock charges HIGH on these captures).
- 0x0189[4] = rolling down-counter while running / 0x00 at rest.
- 0x066A[3]=[4] = coolant degC duplicated (orig_1: 53 -> 95 plateau at the
  thermostat = raw degC confirmed).

New/changed facts:
- 0x0186[5] = 0x00 at IGN-only / 0x06 while running - APPLIED to rusEFI
  (commit 019c1919163).
- 0x0186[0:1] fuel flow IS load-dependent: ~0x026C (620 mL/h) unloaded
  idle but ~0x29D8 (10.7 L/h) with the alternator at 15.9 V - the field
  is real flow, not a constant.
- 0x018A[2]/[3] and 0x0189[5] low bits vary between sessions
  (AC/load-dependent accessory bits: 0x018A[2] 0x00/0x04/0x08, [3]
  0x06/0x07/0xC6, 0x0189[5] 0xB8/0xB9/0xBA) - not load-bearing for the
  needle; rusEFI keeps the unloaded-state constants.
- The 066A temp byte climbs with a first-order filter (tau ~15-20 s,
  boot default 1). In BOTH today's captures the byte climbed to 255
  (max) = the stock CLT input was OPEN (rail): the stock's failsafe
  tripped the fan bit (0x066A[5] 0xC0 -> 0x40 at byte ~215). The user
  should check the CLT connector on the stock-ECU harness.

rusEFI status: rpm (16-bit + tach byte) and temp (degC) encodings are
correct; the dash temp gauge will come alive once the CLT sensor reads
right (the VTRK fix from earlier today).

## 2026-08-29 (evening): dash tach + temperature re-derived from scratch

Context: the user reports the stock ECU drives the dash needle correctly
while rusEFI shows 100-200 on the needle at a console-800 rpm idle. The
previous day's conclusions (0x0186[5] = 0x06, 066A[3:4] = CLT degC) were
WRONG - both led to wrong firmware behaviour, so every field was
re-derived from the raw captures (orig_1/3, today's 19:14/19:27) with a
full byte-by-byte correlation scan instead of spot checks.

Method that worked: for each (frame id, byte) compute Pearson correlation
against the rpm16 field of 0x0189 (rpm) and against a time ramp (warmup
proxy). The dash's OWN frames (present in ignoff.trc where the ECU is
silent: 0x0350/0x0352/0x05D7/0x04AC/0x0242/0x029A/0x029C/0x0303...) were
separated from the ECU's frames the same way, which exposed the dash-side
consumption integrators (0x029A[0:1] = 0x029C[0:1] = a counter that climbs
~2 units/20 ms at idle, decays to 0 when the dash considers the engine
stalled, and stays frozen at 0 when the ECU stream is not accepted).

Corrected encodings (all verified across >= 2 captures):

- 0x05DA[0] = CLT x 2 (0.5 degC units) - THE dash coolant temperature.
  orig_1: 0x40->0x41 = 32->32.5 C, orig_3: 0x43->0x44 = 33.5->34 C,
  19:14: 0x5C->0x61 = 46->48.5 C, 19:27: 0x75->0x79 = 58.5->60.5 C.
  The 13-minute gap between the two evening sessions shows up as exactly
  +26 units (13 C) of residual coolant heat - unambiguous.
  (rusEFI used to send a frozen 0x41 = 32.5 C there - the dash gauge was
  reading a fixed 32.5 C. THIS was the dash temp bug.)

- 0x05DA[1] = battery voltage x 10 (0.1 V units): 0x89 = 13.7 V at IGN,
  0x70 = 11.2 V cranking sag, 0x9D = 15.7 V charging right after start.

- 0x0186[5] = X = throttle position x 10 (0.1 % units) - the stock's
  "fuel is flowing" signal, NOT a constant 0x06. 0x09..0x0E (0.9..1.4 %)
  at warm idle, 0x4C (7.6 %) during a rev blip, 0 at closed throttle / DFCO
  / cranking. Cold-start captures (orig_1/3, CLT ~32 C) show X = 0 even at
  900 rpm - the stock's idle controller keeps the blade shut until warm.
  The dash gates its consumption integrators (0x029A/0x029C) on X > 0.

- 0x018A[2:3] = 16-bit BE crank-angle counter, 2 deg/unit (3 x rpm units/s),
  published in 128-deg (64-unit) bursts, advanced ONLY while the throttle is
  open (frozen at closed throttle/DFCO - verified: frozen 0x0006 for 5+ s
  with X = 0, then +1152 units over 489 ms at 785 rpm = exactly 3 x rpm).
  The old rusEFI sent static 0x0006/0x0007 there. The dash mirrors this
  accumulator into its own 0x029C/0x029A frames.

- 0x066A[3:4] = fuel-gated 100 ms engine-time counter (NOT temperature!):
  +1 per 100 ms while fuel flows at up to ~820 rpm, +2 per 100 ms above,
  paused at closed throttle; keeps its value across key cycles (the stock's
  cumulative hour-meter). orig_1's 0x35->0x5F over 10 s was this counter,
  not a 53->95 C warmup ramp - the car was at 32 C per 0x05DA.

- 0x0186[0:1] = fuel flow in 1/16 mL/h units (0x04B0..0x09D4 cranking =
  75..157 mL/h, ~0x3A4C warm idle = 933 mL/h) - the field is NOT plain
  mL/h (that mis-scaling is what produced the phantom "10.7 L/h idle").

- 0x0189[5] = 0xBA while running (today's captures; orig showed 0xB8 - the
  low bit is an accessory/load flag). 0x01F6[3] = 0x1A (was 0x2D).

Applied to rusEFI (m74_9_can.cpp, commit below): CLT x 2 + Vbatt x 10 in
0x05DA; throttle-X byte in 0x0186[5]; throttle-gated 128-deg-quantized
angle accumulator in 0x018A[2:3]; fuel flow x 16; 066A engine-time counter;
0x0189[4] rolling counter now steps by 0x10; 0x0189[5] = 0xBA; 0x01F6[3] =
0x1A. The tach byte 0x0186[4] = rpm - 768 u8 wrap (mode 4) stays - it was
re-confirmed with corr = 1.000 vs rpm in orig_1.

Open follow-ups:
- The exact dash acceptance gate for the needle is still INFERRED, not
  PROVEN: the needle plausibility model needs an on-car test of this build.
  If the needle is still low, the next suspects are:
  1) 0x0217[3:5] - non-zero while running at the stock: [3] = 0x01/0x02,
     [4:5] = 0xA010/0x700E/0x800E (looks like a load-gated accumulator);
     rusEFI still sends zeros there.
  2) the exact 0x0189[4] rolling-counter sequence - rusEFI now steps 0x10
     upward; the stock's pattern is mixed up/down, ~5 changes/s, and
     correlates 0.64 with the battery-voltage field, so it may be an
     alternator/load value rather than a pure freshness counter.
- 0x05DA[4] and 0x065C[0] vary between sessions (likely alternator/LRC
  state, not needed for the needle/temp, left static in rusEFI):
  0x05DA[4] = 0xB9/0xBA/0xBB in orig_1/orig_3, 0xC0 -> 0x8B -> 0xC1 in
  19:14, 0xC1 in 19:27; 0x065C[0] = 0x7A -> 0x66 over 57 s in 19:14 vs
  0x9E -> 0x98 in 19:27 (jumps at engine start to a session-dependent
  value, then slowly decays).

## 2026-08-29 (night): spark/injection latency vs CKP - scheduler bottleneck analysis

Question: how much do spark and injection lag behind the ДПКВ, and where
are the scheduler bottlenecks (the user reports the stock ECU pulls
harder/faster with identical VE/UOZ tables - suspicion of systematic
retard).

Architecture (verified in code): every engine command (dwell start, spark
fire, injection start) is converted angle->time at the tooth IMMEDIATELY
before the event angle (isPhaseInRange = [currentPhase, nextPhase), 60-2
tooth = 6 deg). Lead = 0..1 tooth = 0..166 us at 6000 rpm, 0..143 us at
7000. The whole software chain must fit inside that lead:
  EXTI fast IRQ timestamp (prio 0, ~2-3 us)
  -> handoff ISR (prio 4): debounce + decodeTriggerEvent (44 us avg,
     tails to ~1 ms measured on the car 2026-08-23) + post-decode
     listeners (rpm callback/oneDegreeUs, spark logic, fuel, angle queue
     processing, ~10-30 us)
  -> SingleTimerExecutor TIM5 CC1 (prio 3, 1-2 us entry).

Latency budget (fixed in us, so in degrees it grows with rpm):
- L9779 VRS conditioner: Tfilter = Tn/32 + Td_off ~4% T -> 0.43 deg
  CONSTANT retard (adaptive, rpm-independent; same for the stock ECU -
  not a differentiator, tables compensate it).
- typical chain ~65 us: worst case (event angle just after the tooth)
  2.3 deg @6000 / 1.2 @3000 / 0.58 @1500; average over random tooth
  alignment (L^2/2T) ~0.5 deg @6000.
- tail chain ~1 ms (handoff backed up by a long tooth): the event fires
  immediately at scheduling time - up to 36 deg late @6000 (6 teeth of
  queue backlog per 1 ms tail).
- injection END = start + PW in the time domain, so a late start shifts
  the whole pulse; the fuel MASS is computed one cycle earlier (720 deg
  pipeline) - an accel-transient lag that is tuning, not scheduler.

Why the stock is better: ME17 fires ignition/injection edges from
HARDWARE compare outputs of the angle-clock timers (GTM) - tooth IRQs
only update the angle clock, the output edges fire with ~1-5 us constant
latency. rusEFI has no hardware output compare on this port - the
software chain + short lead is the structural difference.

Bottleneck ranking:
1. STRUCTURAL: lead <= 1 tooth + the decode/scheduling chain in the same
   ISR. 65 us eats ~40% of the tooth window at 6000 rpm.
2. The ~1 ms decode tails (source not yet localized: trgDecode vs
   trgPostDecode histograms will say - the m74_9 gap-validation code vs
   the spark/fuel listeners).
3. Executor preempts the handoff (WDA feed ~100 us every ~105 ms) - adds
   up to 100 us to the chain of whatever tooth is being decoded then.
4. Late dwell start -> dwellUnderChargeCounter / dwellActualRatio < 0.8
   (weak spark at high rpm on top of the retard).

Proposed fixes (not implemented yet):
1. MEASURE on the car first: 'lockstats' right after a hard pull prints
   everything needed: 'sched exec ... late>=10us maxLateUs' + per-kind
   lines (dwell/spark/overdwell/fuel) + triggerIsr/trgPreDecode/
   trgDecode/trgPostDecode histograms + triggerMaxDuration/
   maxEventCallbackDuration. spark/fuel late>=10us > 0 and maxLateUs in
   the hundreds of us = analysis confirmed.
2. MAIN FIX (cheap): convert queued angle events to time ONE TOOTH
   EARLIER (lead 2 teeth = 333 us @6000) - the prediction error over
   12 deg is negligible (oneDegreeUs is a 90-deg moving average;
   first-order accel lag at 4000 rpm/s over 12 deg ~= 0.003 deg). This
   absorbs the typical chain with margin and most tails.
3. Localize and cut the 1 ms tails per the histograms.
4. ICU option B does NOT fix this: it only removes the 2-3 us EXTI
   timestamp jitter; the stock's advantage is hardware OUTPUT compares.
5. Tuning: check the injection phase (EOIT) against the stock - late
   injection end under acceleration gives the same symptom.

Follow-up (same session): hardware angle-clock firing like the stock GTM
is BLOCKED by the board pins on the unmodified m74_9:
- ignition IGN1..4 = PF12-15 (no timer AF), injectors PE11/PE10/PE9/PE8
  (only PE9/PE11 have TIM1_CH1/CH2). Classic angle-clock+OC is impossible
  for ignition and covers 2/4 injectors only.
- The only full-hardware path for arbitrary pins is a DMA->GPIO_BSRR
  pattern player on a per-tooth reloaded timer (2 DMA streams, double-
  buffered, gap = one long tick). But its refill has the SAME one-tooth
  lead constraint, and it removes only ~5-10 us of dispatch (executor
  entry + callback + quantization) from the ~65 us typical chain - it
  does not remove the decode latency and does not relax the lead.
- Conclusion: ~90% of the stock's advantage is the multi-tooth
  scheduling lead (reproducible in software by converting events one
  tooth earlier) + its preloading compares ahead, not the hardware edge
  itself. Revisit the DMA scheme only if residual jitter still matters
  after the software fix + tail localization.

CORRECTION (same session): the stock-style scheme IS implementable on
this board without output-pin AFs - via an angle-clock timer whose OC
channels work as pure software comparators (CCxE=0, CCxIF fires at
CNT==CCR), the ISR writes the GPIO edge. The stock dump supports it:
TMR2 base 0x40000000 appears 68x (vs single digits for other timers) -
TMR2 (32-bit, 4-ch, free on m74_9) is the right angle clock. Design:
TMR2 @ 4 MHz ticks (PSC=71, 0.0105 deg/tick @7000); tooth reset in the
fast EXTI ISR (CNT=0, ARR = smoothed period x 4; hardware reset
unavailable - PF8 has only TMR13_CH1 and TMR13->TMR2 TRGO/ITR chaining
does not exist on the F4 layout); events = 4 OC channels programmed ONE
TOOTH AHEAD from the angle queue (fraction x predicted ARR); TMR2 ISR at
prio 3 runs the same actions (dwell/spark/injection GPIO writes) and
re-arms; TIM5 stays as the fallback for current-tooth events, multispark
and WDA/soft-PWM/TS. Honest limit: doubles the programming slack (full
tooth = 166 us @6000) but does NOT cure the 1 ms tails - they still miss
the teeth they span (those events fall back to TIM5). Sequence: lockstats
+ tail localization first, then the angle clock as the target
architecture with the TIM5 fallback as the safety net.

## 2026-08-29 - m74_9: TMR2 hardware angle clock implemented (one-tooth-ahead firing)

Goal: eliminate the spark/injection timing jitter vs CKP. The stock ME17 fires
edges from hardware compare outputs of an angle-clock timer; rusEFI converted
every event angle->time at the tooth immediately before the event, so the whole
decode chain (~65 us typical, 1 ms tails) had to fit inside one tooth (166 us
@6000 rpm) - late handoffs fired events late by up to the whole tail. Per the
user decision, implement the angle clock rather than verify the stock scheme in
Ghidra first.

What was built:

| File | Change |
| --- | --- |
| firmware/hw_layer/angle_clock/angle_clock.h/.cpp | NEW TMR2 driver: 4 OC channels as pure software comparators (CCxE=0, OCxM=1, CCxIF fires at CNT==CCR), free-running 32-bit @ 4 MHz (PSC=71), prio-3 ISR executes the armed action |
| firmware/hw_layer/hw_layer.mk | angle_clock added to the hw-layer build + include path |
| firmware/controllers/system/timer/trigger_scheduler.cpp/.h | scheduleEventsUntilNextTriggerTooth arms events due in [nextPhase, nextNextPhase) one tooth ahead on TMR2; TIM5 fallback when the tick passed or all channels busy; cancel() also kills an armed TMR2 compare |
| firmware/controllers/trigger/trigger_central.cpp | computes nextNextPhase (findNextTriggerToothAngle(index+1), same anchor correction) and passes it down |
| firmware/controllers/engine_cycle/main_trigger_callback.cpp/.h | passes nextNextPhase through |
| firmware/hw_layer/hardware.cpp | initAngleClock() right after the TIM5 executor init |
| firmware/config/boards/m74_9/efifeatures.h + config/stm32f4ems/efifeatures.h | EFI_ANGLE_CLOCK flag (TRUE on m74_9, default FALSE) |
| firmware/hw_layer/ports/{at32,stm32}/interrupt_priority.h | EFI_IRQ_ANGLE_CLOCK_PRIORITY (3 on at32 = same as the TIM5 executor) |
| firmware/config/boards/m74_9/board_configuration.cpp | lockstats prints angclk fired/lateArm/noChannel/maxLateUs + NVIC prio check |
| unit_tests/tests/trigger/test_angle_clock.cpp | 4 host tests for the tick math |

Key design decisions:
- FREE-RUNNING counter, no per-tooth CNT reset, ARR = 0xFFFFFFFF. The
  originally planned scheme (reset CNT/ARR in the fast EXTI ISR) cannot fire
  one-tooth-ahead events: an event armed for tooth N+1 as fraction x ARR would
  be reached by CNT while it still counts tooth N - spurious early fire unless
  the priority-0 fast IRQ also clears flags and toggles the channel IEs every
  edge (grows the fast IRQ from ~2-3 to ~4-5 us). With the free-running counter
  events are armed as ABSOLUTE ticks (edge timestamp + NT offset + angle
  delay); the fast EXTI ISR is not touched at all. The only cost is carrying
  the prediction error of BOTH the epoch tooth and the fraction tooth (both
  negligible: oneDegreeUs is a 90-deg moving average, ~0.003 deg at 4000 rpm/s).
- The NT<->angle-clock conversion is a constant offset measured at init: TMR2
  and TIM5 both free-run at 4 MHz from the same TIMCLK1 (288 MHz, PSC=71).
  DBGMCU freezes TMR2 together with TIM5 on core halt so the offset survives
  debugger halts.
- Arming order in the handoff (prio 4) is race-proof against the prio-3 ISR
  preempting mid-arm: clear stale CCxIF -> write future CCR -> store action ->
  enable CCxIE. The ISR releases the channel (clear flag, disable IE, move the
  action out) BEFORE executing it, so a re-arm from inside the action is safe.
- The overdwell contract is preserved: arming cancels the overdwell TIM5 event
  one tooth earlier (the same cancel the due-tooth branch did). If no handoff
  ever arms the event, overdwell still fires at 1.5x dwell. If TMR2 fires after
  a trigger desync, the spark still goes out at the predicted time (time-based
  counter) - same safety envelope as the TIM5 path.
- spark fire + all other TriggerScheduler queue events (VVT/aux) get the angle
  clock. Dwell start and injection start are still scheduled edge-based on TIM5
  at their due tooth (they are not queue events) - a follow-up if the on-car
  measurement shows their 0-1 tooth lead still matters.

Validation:
- m74_9 firmware builds (EFI_ANGLE_CLOCK path active; VectorB0 handler
  reads TMR2 SR/DIER/CNT at 0x40000000 as expected).
- Full unit-test suite: 1169/1169 pass, including the new angle-clock math
  tests and the updated test_aux_valves/test_hpfp call sites.

On-car validation tool: 'lockstats' now prints
'angclk fired=N lateArm=N noChannel=N maxLateUs=N nvic=3 (want 3)'. Expect
fired to carry most spark events, lateArm/noChannel near zero, maxLateUs in
single digits. The sched dwell/spark/fuel lines show what still runs on TIM5.

Open follow-ups:
1. The 1 ms decode tails are NOT cured - a 6-tooth tail still misses arming
   the teeth it spans (their events fall back to TIM5). Localize via
   trgDecode/trgPostDecode histograms.
2. Dwell start + injection start still use the 0-1 tooth lead (scheduled at
   their due tooth). Move them to the angle clock if lockstats shows residual
   lateness in the dwell/fuel classes.
3. Revisit the stock-dump TMR2 disassembly now that the scheme exists - the
   CCR layout of the stock (compare values vs predicted ARR fractions) would
   confirm whether the stock uses per-tooth reset (needs the fast-IRQ work
   this design avoided) or absolute ticks.

## 2026-08-29 - m74_9: dwell start and injection start moved to the angle clock (one-tooth-ahead)

Follow-up to the TMR2 angle clock: the queue events (spark fire) were already
fired one tooth ahead; dwell start (scheduleSparkEvent) and injection start
(InjectionEvent::onTriggerTooth) were still scheduled edge-based on TIM5 at
their due tooth with the old 0-1 tooth lead, so a late handoff still shifted
them.

What changed:

| File | Change |
| --- | --- |
| firmware/controllers/engine_cycle/spark_logic.cpp/.h | onTriggerEventSparkLogic gains nextNextPhase; dwell window checked as [nextPhase, nextNextPhase) (one tooth early) with the old [currentPhase, nextPhase) kept for the first cycle after a (re)sync; scheduleSparkEvent arms the dwell on the angle clock first, TIM5 fallback; dwellStartArmed flag set at scheduling, cleared in prepareCylinderIgnitionSchedule |
| firmware/controllers/engine_cycle/main_trigger_callback.cpp | InjectionEvent::onTriggerTooth gains nextNextPhase; same dual window; start armed on the angle clock first (ends stay time-based); injectionStartArmed + injectionStartArmedAt |
| firmware/controllers/engine_cycle/fuel_schedule.cpp/.h | FuelSchedule::onTriggerTooth passes nextNextPhase; InjectionEvent::update() clears injectionStartArmed (new cycle = new angle) |
| firmware/controllers/algo/event_registry.h | IgnitionEvent::dwellStartArmed |
| unit_tests (test_ignition_scheduling, test_injection_scheduling, test_trigger_decoder, test_fasterEngineSpinningUp) | updated call sites + regression coverage |

Key design decisions:
- Dual window instead of replacing the old one: an angle that is already
  inside the current tooth when the schedule is (re)built (first sync) would
  NEVER be caught by the shifted window - it would fall between the checks.
  The old [currentPhase, nextPhase) window stays as the fallback.
- Double-hit guard: an angle scheduled at tooth k-1 via [nextPhase,
  nextNextPhase) ALSO matches the old window at tooth k. Injection: the guard
  is injectionStartArmed + the armed target time - a match is suppressed only
  while the armed target is still in the future; once the target has passed a
  match is the next cycle's scheduling (at high duty the next window arrives
  BEFORE the injection end recomputes the angle, and the pre-existing
  behavior schedules with the stale angle - testFuelSchedulerBug299 keeps it).
  The flag is additionally cleared in update() (end of injection = new
  schedule cycle; the fasterEngineSpinningUp mode-transition test covers the
  queue-wiped-without-firing case). Spark: dwellStartArmed cleared in
  prepareCylinderIgnitionSchedule at spark fire (the next dwell window is
  always after the fire, so no overlap hazard).
- Non-angle-clock builds (all other boards, unit tests) are unchanged: the
  early window is compiled out under EFI_ANGLE_CLOCK, so the behavior is
  exactly the old single-window one.

Validation:
- Full unit-test suite: 1169/1169 pass (bug299 and fasterEngineSpinningUp
  caught two real flag-lifecycle bugs during development and now pin them).
- m74_9 firmware builds.

Remaining on the old path: multispark (time-based by design), the injection
END events (time domain by design), knock windows, MAP averaging windows.

On-car check: lockstats 'angclk fired' should now count dwell + injection
starts on top of the spark fires; 'sched dwell/fuel' lines should show only
the fallback cases.

## 2026-08-29 - m74_9: WDA feed moved off the executor to a TIM10 one-shot GPT below the handoff

Goal: the L9779 WDA query-answer feed ran as a self-rescheduling event on the
TIM5 executor at priority 3 - ABOVE the trigger handoff - so its ~100 us
polled-SPI burst preempted the trigger decode every ~28 ms and added up to
100 us to the in-flight tooth (one of the decode-tail contributors). The
priority was a side effect of the 2026-08-24 move thread->executor (which
fixed the wd_timing_miss/EC>4 kill mode, a THREAD-starvation symptom), not a
requirement: the answer window is ~12.6 ms wide, so an ISR at any priority
above the threads lands the answer on time.

What changed:

| File | Change |
| --- | --- |
| firmware/hw_layer/drivers/gpio/l9779.cpp | feed runs in a dedicated one-shot GPT (TIM10, 1 MHz, 1 tick = 1 us): self-re-arm via gptStartOneShotI (wdArmIsr), boot kick via gptStartOneShot (wdArmThread), stop via gptStopTimer in chip_power_off; CH_IRQ_HANDLER(STM32_TIM1_UP_TIM10_HANDLER) -> gpt_lld_serve_interrupt(&GPTD10); wd_sched (scheduling_s) replaced by wd_next_moment (efitick_t) for the thread's burst-imminent check |
| firmware/hw_layer/ports/at32/at32f4/cfg/mcuconf.h | STM32_GPT_USE_TIM10 TRUE + STM32_TIM10_SUPPRESS_ISR TRUE (the vector is shared with TIM1_UP, the GPT LLD does not own the ISR) |
| firmware/hw_layer/ports/at32/interrupt_priority.h | EFI_IRQ_L9779_WDA_PRIORITY = 5 (below the handoff at 4) |

Why the lower priority is safe:
- The window is ~12.6 ms: a 1 ms handoff tail preempting the burst delays
  RESP_BYTE0's end by 1 ms, far inside the window. A single miss costs one EC
  climb, recovered in 2 correct cycles (EC decrements per correct answer).
- The feed stays ISR-context (never a thread): thread starvation under
  cranking load was the original kill mode.
- The spi_busy serialization vs the diag thread is unchanged and
  race-free at ANY ISR priority: the flag write is a single atomic store,
  and the thread only resumes after the feed ISR completes, so the
  check-then-burst cannot interleave with a thread batch.

Validation:
- m74_9 firmware builds; VectorA4 (TIM10 GPT ISR) and GPTD10 verified in
  the ELF.
- Full unit-test suite: 1169/1169.

**2026-08-30: the first GPT build BRICKED the bench ECU** (flash verified,
reset into the new app, then no console link ever; recovered by flashing the
revert). Exhaustive static re-verification of the GPT path found NO defect:
- VectorA4 <-> IRQ 25 <-> TMR10 (at32_isr.h + CMSIS + the ELF vector table
  slot at 0x080080A4). The weak-default chain is overridden by the strong
  handler from l9779.cpp.
- RCC bits (APB2ENR/APB2RSTR bit 17) and TMR10 base (APB2PERIPH + 0x4400)
  match the authoritative ChibiOS-Contrib AT32F435 CMSIS.
- GPT state machine of THIS fork: gptStart() is a function setting GPT_READY
  (not the old GPT_CONTINUOUS macro), gptStartOneShotI asserts GPT_READY,
  the one-shot fire sets GPT_READY + stops the timer BEFORE the callback, so
  the self-re-arm from the callback is legal; osalDbgCheckClassI holds
  (the ISR prologue bumped isr_cnt).
- TIMCLK2 = 288 MHz divides 1 MHz exactly (psc=287, the frequency assert
  passes). TIM1 is unused on m74_9 (no spurious shared-vector IRQs).
- The handler shape is identical to the proven TIM5/TIM8/ADC handlers
  (prologue -> serve -> epilogue -> __port_irq_epilogue); serve_interrupt's
  byte-masked SR clear is the same code path the fast-ADC GPTD6 uses
  continuously on this exact hardware.
- Boot ordering: the kick (+17 ms) always follows chip_init() -> spiStart(),
  and spi_busy defers the feed during any thread batch - the documented
  RXNE-spin brick mode is guarded in both versions.

So the brick is a runtime subtlety, not a static defect. The GPT build is
RE-APPLIED with instrumentation that cannot lock up and reports each stage:
- spi_frame_isr: the polled exchange is BOUNDED (1 ms of NT ticks; DWT CYCCNT
  is NOT enabled on the AT32 port - baseMCUInit only zeroes it) and times out
  into a counted frame failure instead of spinning forever; a wedged SPI now
  degrades to WDA misses, visible on the console.
- The feed fizzles (defer + re-arm) when !spi_configured - defense in depth
  for a pre-spiStart fire.
- Prints: GPT10 CR1/DIER/SR at init, the kick (state before/after arm), and
  a 1 Hz liveness line (ok/fail/defer/pollto/delay/gpt state) from the
  driver thread - a wedged feed shows up as frozen counters.
- wd_poll_timeouts counter for the bounded-poll hits.

Bench checklist: flash, connect the console, watch for the init/kick/1 Hz
lines. If the console comes up with climbing ok/fail, the GPT mechanism is
fine and the original brick was the unbounded poll (now neutralized); frozen
counters = the GPT ISR never fires (timer/vector issue); no console at all =
the brick is upstream of USB (gptStart/nvicEnableVector in init).

**2026-08-30 (bench): the INSTRUMENTED GPT build ALSO bricked** - no console
link at all. With the bounded poll the feed cannot spin, so the brick is NOT
the SPI hang: it is the GPT DRIVER itself (the gptStart/gptStartOneShot path
in L9779::init / the boot kick). Static re-verification of every layer came
out clean (vector, RCC bits, TMR10 base, state machine, clock, handler shape
- all identical in kind to the PROVEN fast-ADC GPTD6 and the TIM5/TIM8
handlers), so the defect is a runtime property of the GPT driver API on this
port (the fork's chSysLock __dbg_check_lock HALTS when the kernel is already
locked - gptStart takes the non-reentrant lock, unlike the reentrant
scheduler.schedule the executor feed used).

**FIX (2026-08-30): the feed now drives TMR10 DIRECTLY, no GPT driver.**
wdaTimerInit/wdaTimerArm/wdaTimerStop are plain register writes (PSC=287 ->
1 MHz, ARR=interval-1, OPM one-cycle mode, UIF/UIE update event) cloned from
the PROVEN angle-clock (TMR2) pattern: no locks, no asserts, no state
machine. The bare VectorA4 handler clears UIF, disarms and dispatches the
feed; the feed re-arms via wdaTimerArm (plain registers, ISR-safe). The
boot kick (thread) and chip_power_off's stop are plain register writes, so
the wd_running flag + ISR-vs-thread ordering is the only concurrency
mechanism - verified: the feed fizzles without re-arming when wd_running is
false, and a thread-side arm cannot be preempted into a half-armed state
(SR=0 is written before DIER|UIE, so a preempting ISR sees no UIF). mcuconf
STM32_GPT_USE_TIM10 is back to FALSE - the GPT LLD is no longer involved.
The bounded SPI poll, the !spi_configured fizzle and the prints stay.

**2026-08-30 (bench): the direct TMR10 driver WORKS** - console up, the feed
runs, fail=0 pollto=0 (the SPI path is healthy), the bounded poll and the
guards did their job. One defect found in the first run: **TMR10 ticks on
PCLK2 = 144 MHz, NOT the 288 MHz the fork's STM32_TIMCLK2 (PCLK2 * 2)
assumes** - with PSC=287 the armed 21.92 ms one-shot fired at 43.8 ms
(ok climbed ~21.5/s instead of ~45/s), every answer landed outside the
chip's ~28.4 ms window (miss=3882, NO_RESP+EARLY per reqhi=0xFB, delay
walked 22 -> 17) and EC oscillated 7 <-> 4. APB1 timers are NOT affected:
TIM5/TMR2/TMR6 run 288 MHz (the NT 4 MHz clock is validated by tooth
physics). PSC is now 143; the liveness print shows the fire-to-fire period
measured in NT ticks (per=...us) to confirm ~21920 us on the next bench.

**2026-08-30 (bench, second run): the 144 MHz PSC is CONFIRMED but the
verdicts exposed a THIRD defect - the chip's WDA time base is 39 kHz, not
64 kHz.** With per=~27-35 ms (delay walked 22 -> 27 on EARLY flags, then
NO_RESP back down, miss=120) and a fail storm at delay=22 (35 -> 251/s,
per collapsed to 10-13 ms = the failure-path 10 ms re-arms - the chip NACKs
the early answers), the window behaves like [25.9, 46.6] ms (39 kHz), NOT
[15.8, 28.4] (64 kHz). The driver's CONFIG6 bit1 mapping was INVERTED:
bit1=1 (the old 0x06) gives 39 kHz, bit1=0 gives 64 kHz. The first 2x-slow
build had looked clean ONLY because 43.8 ms happens to sit inside the
39 kHz window. FIX: L9779_CONFIG6_PWR 0x06 -> 0x04 (bit1=0 -> 64 kHz,
bit2=1 VDD5_UV WDA mask unchanged), L9779_CONFIG6_PSOFF 0x16 -> 0x14 for
consistency. The stock runs 0x06 = 39 kHz deliberately (its own feed
matches); rusEFI needs the 64 kHz base for the RESPTIME=10 / 22 ms design.
Expected on the bench: per=~21920 us, delay pinned at 22, miss=0, fail=0,
EC settling <= 4.

**2026-08-30 (bench, third run): flipping CONFIG6 bit1 (0x06 -> 0x04) changed
NOTHING - the chip still runs 39 kHz (delay walked 22 -> 27 on EARLY again,
answers at 26.9 ms accepted, at 16.9 ms EARLY + W_RESP), so bit1 is NOT the
f_clk select (or the write is not latched): both values behave identically.
The out-of-phase thrash also produced the miss/wrong/cntbad storms
(miss=5822, wrong=657, fail=3112). FINAL FIX: tune the feed to the MEASURED
39 kHz base - response 1011/39kHz = 25.9 ms, window 20.7 ms, cycle 46.6 ms,
center 36.3 ms: WDA_DELAY_INIT_MS 22 -> 36, MIN 17 -> 28, MAX 27 -> 44,
boot kick +17 -> +26 ms (just inside the first window opening). CONFIG6
stays 0x04 (bit2=1 VDD5_UV WDA mask; bit1 immaterial). The direct evidence
for the 39 kHz window: the 2x-slow build's 43.8 ms period was accepted
cleanly for minutes. Expected on the bench: per=~35920 us, delay pinned
~36, miss=0, fail=0, EC settling <= 4.

Fork bug to follow up: STM32_TIMCLK2 in the AT32 port (hal_lld.h) doubles
PCLK2 like STM32F4, but the AT32F435 silicon clocks its APB2 timers
(TMR1/8/9/10/11) at PCLK2 - any future GPT/PWM/ICU on those timers would
be 2x off (TIM8 PWM is declared but unused on m74_9, so it never showed).
Verify against the Artery RM before fixing the fork header; APB1
(TIMCLK1 = PCLK1 * 2) matches the STM32 convention on this silicon.

Open: the feed's own dispatch is no longer telemetry-recorded (no lockstats
line) - only the chip-side counters (ok/miss/kills/defer) remain observable.

## 2026-08-30 (final) - WDA feed: TMR10 detour ABANDONED, reverted to the TIM5 executor

The whole one-shot-TMR10 saga (GPT driver -> direct register driver ->
PSC clock corrections -> CONFIG6 bit flips -> delay re-tunes) is REVERTED
per the user's decision ("с таймерами было все в порядке" - the timers
were fine). The WDA feed is back on the TIM5 executor exactly as the
car-drove build (b8d85a87604): l9779.cpp, the at32 mcuconf and
interrupt_priority.h are restored byte-for-byte from that commit.

Why the original motivation no longer requires the move: the angle clock
(TMR2, landed 2026-08-29) fires spark/dwell/injection one tooth ahead
(2-tooth lead ~333 us at 6000 rpm), so the feed's ~100 us preemption of
the trigger handoff every ~28 ms is absorbed by the lead and does not
affect command timing. The priority question is thus answered by the
angle clock, not by moving the feed.

What the detour cost and what it proved:
- The GPT-driver build bricked the bench ECU twice (the fork's
  chSysLock/__dbg_check_lock halts when the kernel is already locked;
  gptStart takes the non-reentrant lock). Static verification of every
  layer (vector, RCC bits, TMR10 base, state machine, clock, handler)
  came out clean - the defect is runtime-only.
- The direct register driver ran, but the chip's WDA window could not be
  matched to any datasheet model: the verdicts oscillated EARLY/LATE at
  every delay value, every re-tune made it worse (addr_err storms of
  ~19k, miss=5822, wrong=2894), and the fire-to-fire period wandered
  45-59 ms against armed 44 ms. The window behaves free-running and
  phase-sensitive; the [17, 27] ms delay constants are what the CAR ran
  with, and the car-validated build is the executor one.
- Kept learnings: DWT CYCCNT is NOT enabled on the AT32 port
  (baseMCUInit only zeroes it); the fork's STM32_TIMCLK2 (288 MHz for
  APB2 timers) is unresolved against the measured 144 MHz TMR10 behavior
  - do not trust either without a scope; CONFIG6 bit1 did not change the
  WDA time base (0x06 and 0x04 identical).

Open: the handoff-decode tails (~1 ms) remain the real scheduling item -
trgDecode/trgPostDecode localization, as before.

## 2026-08-30 (14:13, CAR) - angle clock ran 2x slow: all four coils overcharged, 15A fuse blown

First car start with the angle-clock build: C9012 out-of-order coil off +
C9351-4 coil overcharge 4.5-8.2 ms at the catch, then the 15A ECU fuse
blew. Root cause: TMR2 ticks at HALF the assumed rate - the fork's
STM32_TIMCLK1 claims PCLK1*2 = 288 MHz for all APB1 timers, but the
AT32F435 does not double this timer's clock (same silicon behavior the
TMR10/APB2 measurement showed). With the 2x-slow counter, both the dwell
start and the spark fire (both armed as absolute TMR2 ticks) fired 2x
late - the dwell duration doubled: 8.17 ms overcharge = 2x the ~4 ms
nominal cranking dwell, all four coils simultaneously = the fuse.

Why nothing caught it earlier: the NT domain (TIM5) is a DIFFERENT timer
and its 4 MHz is load-bearing and validated by tooth physics/rpm
readings, so the time base was fine; the bench never runs an engine, so
the angle clock was only ever exercised mechanically (ISR fires, actions
execute) - the absolute timing error is invisible without combustion.

FIX: initAngleClock() now MEASURES the TMR2 rate against the NT timer at
init (10 ms busy-wait, divide the counter deltas) and programs the PSC
from the measured rate, so the counter always ticks at exactly the NT
4 MHz regardless of the silicon. The boot log prints the measured ratio
("angle clock: measured rate N/N ticks (PSC 71 -> N)") - the bench run
confirms the 2x diagnosis in the same flash. The NT<->TMR2 offset is
re-measured AFTER the PSC re-programming (it is only valid at the final
rate). Unit tests: AngleClock 4/4.

Follow-ups: verify the AT32F435 timer clock tree against the Artery RM
before fixing STM32_TIMCLK1/2 in the fork (TMR10 measured 144 MHz on
APB2, TMR2 now measured on APB1); check the coils after the overcharge.

## 2026-08-30 (follow-up) - TMR2 driver verified against the AT32F435 datasheet layout

Field-by-field check of the angle-clock driver's register usage against
the authoritative CMSIS TMR_TypeDef (ChibiOS-Contrib AT32F435_437):
the fork's TIM_TypeDef maps 1:1 to the AT32 TMR layout (CR1=CTRL1@0x00,
CR2=CTRL2@0x04, SMCR=STCTRL@0x08, DIER=IDEN@0x0C, SR=ISTS@0x10,
EGR=SWEVT@0x14, CCMR1/2=CM1/2@0x18/0x1C, CCER=CCTRL@0x20, CNT=CVAL@0x24,
PSC=DIV@0x28, ARR=PR@0x2C, CCR1-4=C1DT..C4DT@0x34..0x40). The DIV (PSC)
latch via SWEVT-UG matches the mechanism the PWM LLD already uses on
TMR5/8/12 (which work). The OC-compare-with-CCxE=0 comparator trick and
the rc_w0 status-flag clears are empirically confirmed (the ISR fires on
the bench). The DBGMCU TMR2-pause bit (APB1FZ bit 0) matches the CMSIS
DEBUG_APB1_PAUSE_TMR2_PAUSE. Nothing in the register usage explains the
2x-slow rate: the defect is the fork's TIMCLK1/TIMCLK2 clock model
(PCLK*2 for all APB timers), which the AT32F435 does not honor for
TMR2/TMR10 - while TIM5 IS doubled (the NT 4 MHz is physics-validated).
The init-time rate measurement (angle_clock.cpp) makes the driver correct
on any silicon; the boot print reports the measured ratio. Full unit
suite: 1169/1169.

For the WDA: the feed stays on the TIM5 executor - TIM5 is the only
timer whose rate is hardware-validated. Any future TMR10 reuse must
apply the same measure-at-init pattern.

## 2026-08-30 (17:05, bench) - the TMR2 clock theory is DISPROVEN; angle clock disabled pending the scheduling investigation

The init-time measurement printed "angle clock: measured rate 40000/40000
ticks (PSC 71 -> 71)" - TMR2 runs at exactly the NT rate (288 MHz). The
driver is datasheet-correct (field-by-field CMSIS check) and the clock
was never the problem. The 15A fuse incident is therefore a BUG IN THE
ONE-TOOTH-AHEAD SCHEDULING LOGIC: the warning sequence (C9012
out-of-order coil off + all four coils overcharged within 1 ms of each
other) shows the fire events got scrambled/lost at the catch and the
overdwell protection rescued all four coils simultaneously at 1.5x dwell
- four simultaneous charges = the fuse.

SAFETY ACTION: EFI_ANGLE_CLOCK is now FALSE on m74_9 (efifeatures.h) -
the early windows compile out and ignition/fuel return to the proven
time-based paths the car ran for months. The angle clock code stays in
the tree for the bench investigation (SPARK_EXTREME_LOGGING on the bench
with the flag TRUE will show the exact event scramble).

## 2026-08-30 (19:00) - WDA feed back on TMR10: measured-rate init, priority 5, reset self-heal

User directive: make a working WDA on TMR10 (the earlier TMR10 detour is
un-abandoned), so the WDA feed no longer runs inside the TIM5 executor
(priority 3) where its ~100 us polled-SPI burst preempted the trigger
handoff and delayed spark scheduling ("wda обрабатывал события не
вовремя"). The TMR10 ISR sits BELOW the handoff (priority 5). From the
working WDA the spark-control investigation continues.

What was verified against the datasheet before writing code:

| Fact | Source | Value |
| --- | --- | --- |
| timer resolution tres(TMR) | at32f.txt Table 53 | 1/fTMRxCLK, fTMRxCLK = 288 MHz max |
| APB1 bus max | at32f.txt section 2.2/2.6 | 144 MHz (TMR2/3/4/5/6/7/12/13/14) |
| APB2 bus max | at32f.txt section 2.2/2.6 | 144 MHz (TMR1/8/20, TMR9/10/11) |
| fork STM32_TIMCLK1/2 model | ChibiOS hal_lld.h | PCLK x 2 when PPRE != DIV1 -> claims 288 MHz on both APBs |
| TMR2 measured (APB1) | angle clock init print, 17:05 | 288 MHz (PSC 71 -> 71, NT-validated) |
| TMR10 measured (APB2) | 13:04 bench (PSC 287 armed 21.92 ms fired 43.8 ms) | 144 MHz |

The fork's blanket PCLK x 2 model is therefore WRONG for APB2 timers on
this silicon (TMR10 = 144 MHz, not 288) while APB1 timers do double
(TMR2/TIM5 = 288). The datasheet alone does not state the per-bus rule -
the RM's CRM chapter would - so the driver does not rely on either
claim: wdaTimerInit() MEASURES the TMR10 rate against the NT domain
(10 ms busy-wait, divide the deltas) and programs the PSC so the counter
ticks at exactly 1 MHz on any silicon. The boot log prints the measured
input rate ("l9779 wda: TMR10 measured N/N NT ticks, input N MHz").

Changes (all in firmware/, m74_9 only):

| File | Change |
| --- | --- |
| hw_layer/drivers/gpio/l9779.cpp | direct-register TMR10 one-shot (wdaTimerInit/wdaTimerArm/wdaTimerStop, no GPT driver), measured-rate PSC init, VectorA4 ISR dispatches the feed, feed re-arms via wdArmIsr, bounded SPI poll (1 ms NT timeout, wd_poll_timeouts), spi_configured entry guard, per= fire-to-fire NT print in the 1 Hz liveness line, reset self-heal re-kick when CR1 has no CEN |
| hw_layer/ports/at32/interrupt_priority.h | EFI_IRQ_L9779_WDA_PRIORITY = 5 (below the handoff at 4) |
| hw_layer/ports/at32/at32f4/cfg/mcuconf.h | comment only: TMR10 is driven directly by l9779.cpp, GPT LLD stays off |

Kept from the car-proven executor feed: the 64 kHz timing model
(REG6=0x06, RESPTIME=10, delay init/min/max = 22/17/27 ms, burst lead
80 us, +-5 ms adaptation), the single atomic 4-byte burst, the
spi_busy deferral contract, the wd_running fizzle contract for
chip_power_off, the +17 ms boot kick. NOT re-applied: the failed 39 kHz
re-tune (CONFIG6 0x04, delays 36/28/44) - that was a misdiagnosis of the
same saga and the 13:10 bench run proved the 64 kHz model healthy on
TMR10 for 10+ minutes (ok=3074, fail=0) before the miss storm.

New vs the old TMR10 build: (1) measured-rate PSC instead of hardcoded
143, (2) reset self-heal - a system reset kills the TMR10 peripheral
while the RAM flags survive (the bench debug resets do exactly this),
the old build then fed nothing forever; the 1 Hz block re-arms a dead
timer, (3) the 13:10-era miss storm is still unexplained - the per=
printout (NT-domain fire-to-fire) now settles the timer rate on the
console every second, and a wrong PSC would show up as per != delay.

Validation: compile_m74_9.sh builds clean. Bench checks (user, next
flash): "TMR10 measured ... input 144 MHz (PSC 143 -> 143)" at boot,
then the 1 Hz line with per ~= delay (27 ms -> ~27000us), ok climbing
~35/s, fail=0, defer small, pollto=0. If per != delay the PSC math is
wrong; if ok stalls but CR1 has no CEN the self-heal is expected to
re-arm within 1 s.

Follow-ups: re-enable EFI_ANGLE_CLOCK on the bench and catch the exact
spark-event scramble at the catch (SPARK_EXTREME_LOGGING); the open
candidate is missing cancelAll of armed TMR2 compares on re-sync.

## 2026-08-30 (18:03 bench run) - TMR10 measured 288 MHz; per != delay needs disambiguation

The measured-rate init printed "TMR10 measured 20002/40004 NT ticks, input
288 MHz (PSC 143 -> 287)" - TMR10 DOES run at 288 MHz on this silicon. The
earlier "144 MHz" conclusion (commit 9120288ae49) was inferred from the
ok-climb rate, not a clean measurement, and was WRONG. The fork's
STM32_TIMCLK2 (PCLK2 x 2) is correct, consistent with the datasheet
Table 53 (fTMRxCLK up to 288 MHz). The measure-at-init pattern made the
driver correct despite the wrong prior belief - this is exactly why the
frequencies are measured, not assumed.

New anomaly in the same run: the 1 Hz liveness line printed per=~52 ms
while delay=22 (per must track delay). ok climbed ~19/s (a 52 ms period)
and miss=70 with ec=4 wda_int=1 - the chip latched a kill pulse because
every other 28.4 ms chip cycle went unanswered. addr_err=697 (7% of
frames) appeared, pollto stayed 0, fail climbed to 82 at the end.

Two hypotheses: (1) the ISR entry is delayed ~30 ms per cycle (bench
debugger halts - TMR10 has no DBGMCU freeze bit, so it fires during a
halt and the ISR runs on resume; or flash stalls), or (2) the timer
itself fires 2.4x late. To disambiguate, the ISR now records TWO
independent latency probes, printed in the 1 Hz line:
  - lateUs: NT domain, ISR entry minus wd_next_moment (the expected fire)
  - cnLatUs: TMR10's own counter, CNT-ARR read before disarming (the
    counter now free-runs past ARR instead of OPM, so the excess is the
    ISR entry latency in us at 1 MHz)
Decision table for the next bench run:
  - late~30 ms AND cnlat~30 ms -> ISR blocked (environment); timer fine
  - late~30 ms AND cnlat~0 -> timer genuinely slow -> PSC/divider issue
  - both ~0 but per~52 -> the re-arm chain (feed) is slow

## 2026-08-30 (18:26 bench run) - per tracks delay exactly; the ISR enters late; addr_err storm

The 18:26 run (the cnlat build, before the UG fix) showed the per-late
identity holding PERFECTLY at both delays: per - late = 22042 us at
delay=22 and 27042 us at delay=27 (= feed ~122 us + armed interval). The
arm-to-ISR-entry time is ~51.4 ms at delay=22 and ~64.4 ms at delay=27 -
the interval tracks the armed delay at a ~2.37x ratio. cnlat stayed
clamped at 60000 (CNT < ARR at every ISR entry) - the ISR always enters
after the counter passed/wrapped ARR. addr_err climbed to 64027 (20% of
309834 frames) and miss=7122 with ec=4 wda_int=1: the chip rejected
every other cycle.

Working hypothesis: the ARR/PR write in wdaTimerArm never becomes active
(PR is preload-only on the AT32 and latches only at the update event -
the PWM LLD does EGR|UG after writing PSC/ARR), so the fire point stays
at the init PR=0xFFFF (the wrap). wdaTimerArm now generates EGR|UG after
the ARR write (fix), and the 1 Hz print additionally dumps CNT/ARR/PSC
so the next run reads the active register state directly.

Open after the fix lands: the ~30-40 ms ISR-entry delay relative to the
expected fire (bench debugger halts - the console prints bunch in
multi-second bursts - or a prio<=5 blocker) and the addr_err mechanism
(the bounded-poll -3 abort desyncs the chip's 16-bit frame counter if
it ever fires; pollto stayed 0 in all runs, so the addr_err source is
still open).

## 2026-08-30 (18:35 bench run) - the TMR10 timer is VERIFIED CORRECT; per=52 ms is the bench ISR-entry delay

The 18:35 run (the UG-fix build) dumped CNT in the 1 Hz line and it
settles the timer question: all 19 thread-side CNT samples are < ARR
(0x559F=21919) - the counter wraps to 0 AT ARR (probability of 19/19
below ARR by chance ~1e-9). The between-print CNT deltas (~13-21k per
second) match 1 MHz mod 21920 (~13.6k). So with the EGR|UG latch fix:
PSC=286 active (1 MHz ticks), ARR=21919 active, the UEV fires every
21.92 ms - the timer hardware and the arm sequence are CORRECT.

per stayed ~52 ms while the timer fires every 21.92 ms - the ISR entry
is delayed ~30 ms on the bench. The same ~17-30 ms entry delay appeared
in EVERY session across ALL generations of the timer code (13:10 old
PSC=143+OPM build, 18:03, 18:17, 18:26, 18:35) - it is a property of
the bench environment (debugger halts; the console prints bunch in
multi-second bursts, the ~5 s NVIC debug resets), not of the timer.

The cnlat probe was structurally broken: CNT-ARR is ALWAYS negative at
ISR entry because the counter wraps to 0 AT ARR - it clamped to 60000
constantly and measured nothing. Fixed: cnlat now prints the raw CNT at
entry (= ISR latency mod (ARR+1) in us; 0-100 = prompt entry).

Chip-side: miss=0 wrong=0 cntbad=0 over 364 answers at a ~52 ms period -
the chip accepted EVERY answer, which is only possible if its own WDA
cycle is >= 52 ms - the chip's f_clk is slower than the datasheet's
64 kHz claim (consistent with the old "39 kHz" saga readings). The
bench halts smear any phase measurement, so the chip model must be
settled ON THE CAR: with no debugger, per must track delay (~22/27 ms)
and cnlat must read 0-100. ec=7 wda_int=1 is the known latched-fault
state (heals via the need_init SW_RST path), not a live miss.

## 2026-08-30 (18:47 bench run) - TMR10 final rate 10000/40004 = 1 MHz: the WDA timer is DONE

The final-rate measurement printed "TMR10 final rate 10000/40004 NT ticks
(want 10000/40000 = 1 MHz)" - the counter runs at exactly 1 MHz with the
programmed PSC=287 (288 MHz input, datasheet-consistent). With cnlat=2-8us
(prompt ISR entry), addr_err=0, fail=0, miss=0, wrong=0, cntbad=0 the
whole TMR10 WDA chain is verified: measured-rate init, UG-latched ARR,
wrap-at-ARR, ISR at priority 5 below the handoff, single atomic burst,
bounded poll, reset self-heal.

The per=42-55ms-in-wall-time mystery is RESOLVED: the bench debugger
halts the core ~50% of the time (console prints bunch in multi-second
bursts, ~5 s debug resets). The counter freezes during the halts (the
AT32 DBGMCU pauses APB2 timers), so the 21.92 ms armed interval of
running time stretches to 42-55 ms of wall time, and late = per - armed
- feed tracks it exactly. Every session since 13:10 showed the same
17-32 ms per-cycle delay for the same reason - the timer code was never
at fault. On the car (no debugger) per must track delay (~22/27 ms).

Remaining chip-side question for the car: miss=0 at 42-54 ms wall period
means the chip accepted every answer - its own WDA cycle is >= 54 ms in
wall time, so either the chip's f_clk is slower than the datasheet's
64 kHz claim or the window is wider. If miss climbs on the car at the
22-27 ms feed, re-center the delay range to the measured cycle. ec=7
wda_int=1 is the known latched-EC chip quirk (healed by the need_init
SW_RST path, harmless on the bench - no TLE9201 kill warnings).

## 2026-08-30 (19:00) - per=42-55 ms is a TIM5-freeze artifact: the WDA answers every 21.92 ms of WALL time

Root cause of the measurement inflation, fully established:

- rusEFI FREEZES TIM5 (the NT clock) on core halt ON PURPOSE:
  microsecond_timer_stm32.cpp sets DBGMCU APB1FZ TIM5_STOP (so the
  TMR2/TIM5 offset survives debugger halts - the angle clock contract).
- TMR10 has NO DBGMCU freeze bit set (nobody writes APB2FZ), so it keeps
  counting through the halts and fires every 21.92 ms of WALL time; the
  ISR enters 2-8 us later (cnlat) and the chip accepts EVERY answer
  (miss=0 at the 64 kHz window model - the chip model is correct).
- The bench debugger halts the core ~50% of the wall time (its ~5 s
  events + session halts - the known bench "debug", NOT firmware). NT
  freezes through the halts, so the NT-based per/late stretched ~2x:
  per = 21.92 ms of wall time read as 42-55 NT-ms.

FIX (diagnostics, this build): the fire-period and late measurements now
run on a free-running TMR11 reference (same APB2 clock, same measured
1 MHz PSC, DBGMCU pause bit explicitly CLEARED - never frozen on halt):
per/late now read TRUE WALL TIME (~22/27 ms) even on the bench. The
1 Hz line also dumps DBGMCU APB1FZ/APB2FZ so any debugger-set freeze
bits are visible directly. The WDA itself needed no fix - it was
answering on time the whole session.

## 2026-08-30 (19:01 bench run) - WALL-TIME PROOF: per = 22048/17048 us = the armed interval exactly

The TMR11 wall-clock reference (1 MHz, DBGMCU pause cleared) settles the
whole question with hard numbers: per=22048us at delay=22 and
per=17048us at delay=17 - the fire-to-fire equals the armed interval to
within a few us, late=8-9us, cnlat=4-5us. The feed answers ON TIME in
wall time; the earlier NT-based per=42-55ms was the TIM5 freeze artifact
(TIM5_STOP is set by rusEFI on purpose; DBG2 reads 0x0 - the debugger
sets NO timer-freeze bits). Nothing firmware-side halts the core
(lockstats maxLockedDuration=0, executor hist <50us).

At the end of the run the adaptation walked delay 22 -> 17 on a NO_RESP
and miss climbed to 134 with reqhi=0xFA (W_RESP + RESP_TO_EARLY): the
chip's window phase drifted beyond the [17, 27] clamp - either the
chip's CLK1 oscillator (its own, +-5%) or the bench halt jitter. The
car settles it: with per tracking delay exactly, a miss climb means the
chip's cycle is longer than the 64 kHz datasheet model and the delay
range needs re-centering to the measured cycle.

## 2026-08-30 (19:10) - WDA bench fixes: delay range [12,55], chip SW_RST re-sync, masked burst

The 19:02-19:03 run proved the WDA was failing ON THE BENCH (miss 134 ->
2605, reqhi=0xFA = RESP_ERR|RESP_Z0|W_RESP, addr_err 16301 = 20% of
frames, ec=4 wda_int=1, the OUT_DIS heal / VRS re-init loop). Three root
causes, all fixed:

1. The delay clamp [17, 27] ms cannot reach the chip's real window: the
   EARLY flag at 17 ms means the window opened AFTER 17 ms - the chip's
   f_clk is ~39 kHz (window [25.9, 46.6] ms), not the 64 kHz the
   constants assumed. The adaptation pinned at the clamp and every answer
   missed. Range widened to [12, 55] ms (covers f_clk 32..64 kHz), init
   stays 22.
2. A scrambled chip never re-syncs: torn bursts (debugger halts /
   preemption) leave the chip's question engine in a bad state
   (RESP_Z0/RESP_ERR/W_RESP, addr_err storms) and byte-level re-alignment
   cannot repair it. New: 10 consecutive W_RESP/RESP_Z0/RESP_ERR cycles
   -> need_init -> full SW_RST + re-init (5 s cooldown) - the chip
   restarts clean instead of degrading forever.
3. The 8-frame sequence can be torn mid-flight by a preempting handoff:
   the SPI sequences (4 reads + 4-byte burst) now run under
   __set_BASEPRI(4 << 4) - ISRs at priority >= 4 are masked during the
   frames, the executor (priority 3, spark) stays unmasked. The debugger
   halt remains unmaskable and is covered by fix 2.

Validation: compile_m74_9.sh builds clean. Bench expectations: delay
walks to the chip's window (22 -> ~36 if the chip is 39 kHz) and locks;
miss stops climbing; the heal/VRS loop stops; a W_RESP storm triggers
one SW_RST per 5 s at most.

## 2026-08-30 (evening) - WDA reload storm root cause + console noise cleanup

The 19:26 bench run showed the WDA in a self-sustaining storm: fail=5435,
addr_err=111670 (30% of frames), miss=852 wrong=860, per=10001us
(10 ms retry loops), and the chip_init triple (RESPTIME readback / VRS /
OUT_DIS cleared) repeating endlessly - the "reload" the user saw. Root
cause chain, all confirmed in code:

1. THE RELOAD LOOP: the feed treated EC=7 + WDA_INT as a permanent latch
   and asked for a full SW_RST (need_init) every ~220 ms (10 cycles at
   22 ms + 5 s cooldown). But EC=7 is what every bench debugger halt
   leaves behind - the chip's WDA cycle runs on ITS OWN oscillator, so a
   core halt lets cycles expire unanswered and EC climbs. It is NOT a
   latch: the question keeps repeating and a correct atomic burst is
   accepted later, decrementing EC (19:14 proved it - the question
   advanced 0x0 -> 0x4 -> 0x8 -> 0xD for minutes while miss=0 wrong=0).
   The latch-triggered SW_RST is REMOVED. SW_RST stays only for the
   scrambled-question-engine evidence (10 consecutive
   W_RESP/RESP_Z0/RESP_ERR), which has real proof.
2. THE FAIL/ADDR_ERR FLOOD: every SW_RST wipes the chip's reply pipeline
   while the MCU-side rd_pending queue kept its stale entries - post-reset
   replies were matched against dead requests, every frame misattributed,
   REQULO replies were lost (fail) and the whole loop fed itself.
   chip_reset() now clears rd_pending_cnt/head after the SW_RST frame.
3. THE DELAY ADAPTATION chased garbage: the REQUHI verdicts (NO_RESP /
   RESP_TO_EARLY) are only meaningful for a clean previous burst; a torn
   stream makes them noise (the 19:02 walk 22 -> 17 in the wrong
   direction). New wd_prev_cycle_clean flag: verdicts are applied only
   when the previous cycle went out as one clean atomic burst; fail /
   defer / cnt_bad / wrong-value cycles clear it and reset the
   consecutive-fault evidence.

Console noise cleanup (user request, same commit):
- The crash marker / Reset Cause pair is now printed ONCE per boot - the
  5 s re-print for the first minute (reprintPendingBootReport) is gone.
- The 1 Hz "l9779 wda: ok=..." liveness line no longer prints to the
  console; the last 10 one-second samples are kept in a ring buffer and
  dumped by the 'pins' diagnostic (L9779::debug).
- The chip_init reload lines (RESPTIME readback / VRS / OUT_DIS cleared)
  stay as-is - with the storm gone they only appear on real re-inits.

Validation: compile_m74_9.sh builds clean. Bench expectation: no more
reload triples, fail/addr_err stay ~0, delay adapts only on clean
verdicts; after each debugger halt EC recovers by itself within a few
cycles.

## 2026-08-30 (night) - WDA per-cycle event ring: the ec=7 mystery needs raw data

The 19:48-19:50 bench run (new build): fail=0, addr_err ~0 (one +77
transient right after a reload), the reload triples only ~every 2.5 min
(via the 10-consecutive-wrong-value SW_RST escape), delay dithering
22 -> 17. BUT ec=7 wda_int=1 pinned the WHOLE session while reqhi=0xC0
(clean) and the question changed 0x8 -> 0xB -> 0xC -> 0x9 - three facts
that contradict the datasheet's "EC decrements on accepted answers"
model (accepted answers + clean flags + EC pinned at 7 cannot all be
true). The 1 Hz samples are too sparse to settle it, and every chip
model (64 kHz / 39 kHz f_clk) fails on some observation.

Added a per-cycle WDA event ring (64 slots, file-static in l9779.cpp):
every feed cycle records NT-ms, req, ec, wda_int, the raw REQUHI byte,
the delay and a flags byte (bit0 = core was halted since the last fire,
detected via the never-frozen TMR11 wall period > 2x the armed delay;
bit1 = burst sent cleanly). The pins diagnostic now prints the last 64
cycles plus one-line counters of EC transitions (ecUp/ecDown/ecSame,
reqChg) since the previous dump. One pins run will show per-cycle EC
movement and the exact REQUHI bytes at rejections - the decision data
for the delay adaptation (direction, acceptance detector, halt
handling).

## 2026-08-30 (night, follow-up) - NO runtime reloads: the wrong-value SW_RST escape removed

User directive: no reloads (chip_init/SW_RST triples) at runtime, period.
The 19:48-19:50 log had two (19:48:39, 19:51:09), both from the
10-consecutive-wrong-value -> need_init -> SW_RST escape - and both were
useless: the wrong counter kept climbing after each reset, so the
rejections were not a scrambled question engine that a reset could fix.

The WDA feed now NEVER resets the chip at runtime: the wrong-value branch
counts only (wd_wrong_cnt / consecutive wd_bad_value_cnt, both
diagnostic). Rationale, code-commented:
- the atomic burst cannot stay desynced: RESP_CNT resets at every
  sequencer run and the question is re-read fresh every cycle, so the
  next burst answers the current question in the correct byte order;
- the old SW_RST escapes (EC=7 latch trigger + wrong-value trigger) are
  what produced every reload storm and the addr_err/fail floods;
- the only remaining runtime chip reset is the ignition key-on
  transition (power-stage gate), which wipes the parked PSOFF state -
  that is the designed boot path, not a WDA reaction.

Validation: compile_m74_9.sh builds clean. Expected on the bench: zero
chip_init triples between boots; wrong/miss events still counted, no
reset action.

## 2026-08-30 (late night) - WDA root cause proven by the event ring: 39 kHz chip + early answers + FIFO desync

The 20:16/20:20 pins dumps settle everything:

1. NOTHING halts in this session: uptime tracks wall time 1:1 and ok
   climbs at the full 22 ms rate (per=22060us wall on the never-frozen
   TMR11). The earlier "bench halts 50%" claim was true for the
   18:xx-19:xx sessions only.
2. The chip's question is RANDOM per cycle (the ring shows a fresh value
   every cycle) - it is NOT an acceptance detector. Over 2233 clean
   cycles: ecUp=2, ecDown=1 - EC moves ONLY on reject/accept and sits
   saturated at 7 while reqhi stays 0xC0: the chip rejects our 22 ms
   answers silently (its TO_EARLY/NO_RESP flags are cleared by the next
   sequencer run before our next read). The bench chip runs ~39 kHz
   (CONFIG6 bit1 ignored), so the RESPTIME=10 window is [25.9, 38.5] ms
   and 22 ms is EARLY every cycle. This is the whole ec=7 mystery.
3. The second dump's req=2/0 ec=4 int=1 is an OFF-BY-ONE artifact: with
   the reply pipeline shifted by one skipped reply, the feed read REQUHI
   bytes (0xC0/0xC2) at the REQULO position: 0xC2&0xF=2, (0xC2>>4)&7=4,
   bit7=1. The FIFO-position matching turned ONE skipped reply into a
   permanent desync (addr_err 23%, fail 1981, miss/wrong thousands) -
   with no reset involved.

Fixes (commit): (a) EC-saturation walk - on clean, un-halted cycles with
ec==7 for 6 cycles the delay steps +5 (direction reverses at the clamps),
hold as soon as ec<7: 22 -> 27 lands inside BOTH candidate windows
([15.8,28.4] for 64 kHz, [25.9,38.5] for 39 kHz), a 64 kHz chip never
walks (ec sits at 4). Halt detection via the wall period (TMR11). The
flag-based steps stay as a secondary hint, the wrong-value/cnt_bad
recenters are removed (they fought the walk). (b) spi_validate now
matches replies by CONTENT (the reply's own sub-address field) instead
of FIFO position: a skipped reply self-heals on the first mismatch, an
unknown reply is dropped without popping. (c) stale-entry guard: >4
queued reads at the feed's batch start = accumulated skips -> queue
cleared.

Validation: compile_m74_9.sh builds clean. Bench expectation: delay
walks 22 -> 27 within ~1 s and EC drops to 4; addr_err/fail stay ~0 even
after the reply stream shifts; no reloads.

## 2026-08-30 (final) - RESPTIME shortening is DEAD: the chip keeps the default ~[99,112] ms window

The 20:35/20:36/20:39 dumps killed the 39 kHz/[25.9,38.5] model for good:
the EC-saturation walk swept the delay across the whole 15..55 ms range
(55->45->40->25->22->37->47->52->50->45->...->15) and EC NEVER dropped
below 7 at ANY delay (ecUp=0/ecDown=0/ecSame=everything, ~1400 cycles),
while reqhi=0xC1 showed the chip flagging the answers as too EARLY even
at 55 ms. Conclusion: the RESPTIME=10 write lands in the register (the
readback shows 0x0A) but the chip's WDA engine KEEPS the default
response time - the window opens after 55 ms, i.e. it is the default
~[99.4, 112] ms @ 64 kHz. The ONLY proven-healthy configuration in the
whole saga was exactly that: the 2026-08-24 16:21:41 lock at ~115 ms
(zero misses over 206 answers), which was measured with the DEFAULT
RESPTIME. The whole "shorten RESPTIME to 10" optimization rested on the
write taking effect - it does not.

Revert to the stock regime (commit): RESPTIME is no longer written
(chip_init and chip_heal_out_dis keep the chip's reset default 0x3f -
the stock never touches it either); the feed period is ~105 ms
(INIT 105, walk range [85, 195] covering 64 kHz [99,112] and 39 kHz
[163,184] with CLK1 drift); the boot kick arms at wd_delay_ms. The TMR10/
TMR11 tick is changed 1 MHz -> 250 kHz (4 us) because the 16-bit ARR at
1 MHz caps at 65.5 ms and the new answer periods are 105..195 ms (the
per/late/cnlat prints scale by 4). The EC walk converges slower at the
long cycle (6 cycles x ~105 ms per 5 ms step, ~8-12 s worst case to walk
105 -> 165 on a 39 kHz chip) - acceptable for a boot-time search.

Validation: compile_m74_9.sh builds clean. Bench expectation: delay
walks 105 -> ~115 within a few seconds and EC drops 7 -> 4 (the
2026-08-24 lock); miss stops climbing; no reloads; addr_err/fail ~0.

## 2026-08-30 (final, post-power-cycle) - the bench chip ACCEPTS answers; its EC decrement is broken

The 21:01 post-power-cycle dump settles the chip semantics. reqChg (the
question-advance rate) matches the in-window hit rate in every session:
45-47% at 22 ms AND at 105-115 ms sampling (12.6 ms window / 28.4 ms
chip cycle = 44%), 12-13% in the 20:50 session (12.6 / 112 = 11%). The
chip's question advances EXACTLY when an answer lands in its window -
the chip has been accepting answers all along, at any feed delay. What
is broken on the bench chip is ONLY the EC decrement: EC went 6->7 on
the first answer (increments work) and never decrements again even with
thousands of accepted answers - a partially broken chip, exactly the
user's suspicion. SPI shows no errors because the SPI block is fine;
the EC counter is a separate chip function.

Conclusion: the delay is irrelevant to acceptance (the free-running
cycle + uniform phase gives the same hit rate at any period), so the
EC-saturation walk was hunting a phantom and is REMOVED. The delay is
fixed at the proven 105 ms (the 2026-08-24 zero-miss regime). The
acceptance health signal is the question-advance rate (reqChg in pins),
not EC. On the car's chip the EC decrement works (proven 08-24/08-26,
ec=4), so the same feed drives EC to the floor there.

Validation: compile_m74_9.sh builds clean. Expected on the bench:
delay stays 105, ec=7/wda_int=1 forever (chip defect, ignore), reqChg
~44% or ~11% depending on the chip's RESPTIME state - the acceptance
proof.

## 2026-08-30 (21:07) - correction: reqChg is NOT an acceptance signal either

The 21:07 event ring disproves the acceptance inference from the
question-advance rate: the question has several different successors per
value (0->D and 0->4, 9->C/9->D/9->4) - it is NOT a deterministic
counter/LFSR - and its rate drifts with the chip's own internal clock
(49 ms in the 22 ms-feed session, ~1 s in the 20:50 session, 159 ms in
the 21:06-21:07 session, speeding up over minutes after a power cycle -
an RC-type internal oscillator warming up). reqChg is chip-internal
noise, not acceptance.

Final bench state: the bench chip yields NO usable WDA health signal -
EC decrement broken (pinned at 7), no REQUHI flags ever (miss=0
wrong=0 cntbad=0), question = noise. The bench can only validate the
feed mechanics, and those are clean: fail=0, addr_err~0, delay frozen
at the proven 105 ms, no reloads, cnlat=0, per exactly the armed
interval. The health verdict is the car: the wire bytes are identical to
the 08-24/08-26 build that showed ec=4 there.

## 2026-08-30 (21:18, with real loads) - the WDA kill is real; the question-freeze is the acceptance signal

The power cycle + real loads session changed everything: Reset Cause =
Power on/power-down, RESPTIME readback = 0x3F (the chip's config reset
- proving the RESPTIME write DOES take effect and survives soft resets
only), and the question FROZE at 0x4 for 400+ cycles while the delay
sat at 105 ms. The datasheet's "question repeats until answered
correctly" makes the freeze the rejection signature: the chip rejected
every 105 ms answer (the 39 kHz window at 0x3f sits at [163, 184] ms -
105 is early). With EC pinned at 7 (bench chip's decrement broken), the
WDA kill (EC > 4 -> WDA_INT) forced OUT1-4/IGN1-4 off AND pulled the WDA
line low -> Q5B -> TLE9201 DIS -> blade dead - with real loads connected
the whole power stage went dark (throttle, coils, injectors, fuel pump).
The bench never showed it before: open-load diagnostics work regardless
of the kill, and there were no loads.

Conclusions and fixes (commit):
- The question CHANGE = accepted answer; the question FREEZE = rejected.
  This is the chip-independent acceptance signal (the bench chip's EC
  decrement is broken and EC cannot serve).
- The RESPTIME write DOES take effect (the "no effect" conclusion was
  wrong - it was based on the EC, not the question). RESPTIME=10 is
  written again at init and on the configWiped heal for a deterministic
  short window ([15.8, 28.4] @ 64 kHz / [25.9, 38.5] @ 39 kHz).
- The delay walk is restored, driven by the question-freeze signal:
  clean, un-halted cycles with an unchanged question for 6 cycles ->
  step +5 (reversing at the clamps); a question change -> hold. From
  the 22 ms init it reaches either window in 1-3 steps (~0.5 s), which
  also clears the boot-time WDA kill fast (EC 6 -> 4 needs 2 accepts).
- The feed MUST keep the question advancing: a stuck question means a
  stuck EC=7 means the power stage stays killed.

Validation: compile_m74_9.sh builds clean. Bench expectation: delay
walks 22 -> 27 within a second and the question starts changing every
cycle (reqChg high); EC stays 7 on this chip (broken decrement - the
power stage will NOT recover on the bench unit even with a perfect
feed). The car is the validation target: there EC decrements and the
walk + short window will bring EC to 4 and re-enable the stages within
~0.5 s of ignition on.

## 2026-08-30 (late night, user directive) - WDA feed reverted to the proven ce32509e timings on TMR10

User directive: keep the TMR10 one-shot feed (do NOT revert to the
executor), but take ALL timings and settings from ce32509e - the feed
that demonstrably ran with ec=4 wda_int=0 on the car (2026-08-24/26).
Stop the timing churn: the question-freeze delay walk is removed.

Changes (firmware/hw_layer/drivers/gpio/l9779.cpp):
- WDA_DELAY_MIN_MS 12 -> 17, WDA_DELAY_MAX_MS 55 -> 27: the delay is
  now clamped to the proven RESPTIME=10 window band [17, 27] ms
  (window center 22.1 ms, +-5% CLK1 drift -> [16.6, 27.0] ms).
- The question-freeze walk (wd_frozen_cnt/wd_walk_dir, 6-cycle freeze
  -> step, direction reversal at the clamps) is REMOVED. The delay is
  adapted ONLY by the REQUHI timing verdicts (NO_RESP -> -5,
  RESP_TO_EARLY -> +5), exactly the ce32509e policy.
- Restored the ce32509e recenter points: W_RESP/RESP_Z0/RESP_ERR ->
  wd_delay_ms = WDA_DELAY_INIT_MS; RESP_CNT != 11 -> recenter + skip
  the burst + re-arm.
- Kept (unchanged): WDA_RESPTIME=10 written at init and on the
  configWiped heal, WDA_DELAY_INIT_MS=22, the wd_prev_cycle_clean gate
  on the verdict adaptation, NO runtime SW_RST (user directive), the
  TMR10 one-shot at priority 5, the 250 kHz measured-rate tick.

Validation: compile.sh for m74_9 -> BUILD SUCCESSFUL (build/rusefi.srec
regenerated). Not yet flashed to the bench unit.

Bench expectation (honest): the unit currently on the bench runs the
stale 19:45:13 build (delay frozen at 105 ms, no RESPTIME write per the
readback 0x3F) - that build cannot land in any window of the bench
chip, which is why the question froze and the power stage stayed dead.
The new build writes RESPTIME=10 and feeds at 22 ms. If the bench chip
honors the RESPTIME write, the answer lands inside the window and the
kill should clear once EC decrements; if it ignores the write (its
39 kHz / no-write quirks), no delay in [17, 27] can reach its window
and the bench power stage will stay dead regardless of the feed - the
car chip demonstrably honors the write (ec=4 at 22 ms).

## 2026-08-30 (23:00, user logs) - the bench chip honors RESPTIME; 22 ms is too early, 27 ms covers both rates

The 21:46 flash of the ce32509e-timings build produced the decisive data
and refutes the "bench chip EC decrement is broken" conclusion:
- RESPTIME readback = 0x0A (the previous build's 0x3F was because THAT
  build did not write RESPTIME at all, not because the chip ignored it).
- delay 22 ms, miss=0, wrong=0, reqhi=0xC0 (no verdict flags) - but the
  per-cycle ring shows the question freezing for 30+ cycle stretches
  (req=E x31, req=8 x18, req=C x9) with EC pinned at 7.
- Interpretation: the bench chip runs 39 kHz (the known quirk), so its
  RESPTIME=10 window is [25.9, 38.5] ms - a 22 ms answer is EARLY and
  rejected SILENTLY (the TO_EARLY flag is cleared by the next sequencer
  run before the next read), exactly like the 21:19 session. The car
  chip is 64 kHz (window [15.8, 28.4]) where 22 ms is the center - that
  is why the same feed showed ec=4 on the car.
- The kill latches BEFORE the feed can help: while the MCU boots (~1 s)
  the chip is un-fed, EC climbs 6 -> 7 and the power stage is forced off;
  clearing OUT_DIS does not clear the WDA kill, only EC <= 4 does.

Fix: WDA_DELAY_INIT_MS 22 -> 27. 27 ms sits inside BOTH windows
([15.8, 28.4] @ 64 kHz and [25.9, 38.5] @ 39 kHz) with +-5% CLK1 drift
margin, so the very first burst after the RESPTIME write is accepted on
either chip rate and EC decrements 7 -> 4 within ~3 cycles (~0.1 s) -
the power stage comes up ~0.3-0.5 s after key-on without any delay
walking. 27 is the proven ce32509e max-clamp value. Validation:
compile.sh m74_9 -> BUILD SUCCESSFUL; bench confirmation pending (the
expected signature: ec drops to 4, wda_int clears, injector/coil/pump
diagnostics go live).

## 2026-08-30 (late night, user directive) - root cause made visible + the 2:1 fire:feed anomaly instrumented

Primary task per user: fix the power stage. Two findings from the 22:08
bench run (delay 27 ms, RESPTIME readback 0x0A):
- EC is STILL pinned at 7 with miss=0 wrong=2 reqhi=0xC0: 27 ms is also
  rejected silently on the bench (the question freezes in runs, reqChg
  44%), so no fixed delay in the short-window band locks this chip while
  the feed runs at half rate.
- NEW: the feed completes ~18 cycles/s while the TMR10 fires every
  27 ms of wall time (per=27068us CONSTANT across all ten 1 Hz samples;
  cnlat/late prove prompt ISR entry - no halts). ok climbs 18/s, the
  event ring stamps are ~49 ms apart, yet fail/defer/pollto/cntbad all
  stay ~0 - half the cycles vanish through NO counted path. Static
  review found no mechanism; the remaining candidates are the shared
  TIM1_UP_TIM10 vector firing without the TMR10 UIF set, or the silent
  !wd_running early return (no gate flap prints, so unlikely).

Changes:
- l9779.cpp: root-cause console print on the kill-state transition
  ("WDA KILL: EC=N > 4 - power stage FORCED OFF by the chip" /
  "WDA RECOVERED: EC=N <= 4"), driven by ec_now crossing 4.
- l9779.cpp: dispatch-path counters irq/uif/foreign/disp/feed/
  notrunning, printed by 'pins' as "WDA path:" and reset per dump - one
  bench run pins down where the missing half of the cycles goes.
- output_channels.txt + status_loop.cpp: TS output channels
  l9779WdaEc / l9779WdaInt / l9779WdaKill / l9779OutDis /
  l9779WdaDelayMs / l9779WdaOk / l9779WdaWrong / l9779WdaMiss
  (populated under EFI_PROD_CODE && BOARD_L9779_COUNT>0, zeros
  elsewhere).

Validation: compile.sh m74_9 -> BUILD SUCCESSFUL (config regeneration
needed a touch of output_channels.txt after the first failed attempt -
the Lua output lookup regenerated but the struct header did not).

## 2026-08-30 (23:30) - root cause FOUND and fixed: TMR10 runs at 144 MHz (PCLK2 does not double), the feed answered at ~54 ms real, always outside the window

The 22:35 bench run with the dispatch-path counters is decisive:
irq=639 uif=639 foreign=0 disp=639 feed=639 notrunning=0 - the handler
serves EVERY timer fire and the feed completes EVERY dispatch; nothing
is lost anywhere in the path (the earlier "2:1" was never a dispatch
problem). Combined with the constants (per=6768 TMR11 ticks in ALL ten
1 Hz samples, cnlat=4us prompt entry, late=8-12us, ok climbing 17.3/s
of real time, the event ring stamped ~57 NT-ms apart): the timer really
fires every ~54 ms REAL, i.e. the TMR10 tick is ~8.4 us, i.e. the
TMR10/TMR11 input clock is ~144 MHz = PCLK2 - the APB2 timers do NOT
double on this AT32. The fork's STM32_TIMCLK2=288 MHz and the boot
print "measured 287 MHz" were both wrong: the NT-anchored measurement
(newPsc = acDelta*144*16/ntDelta) cancels the input rate out of the
ratio, so the derived "input MHz" always read ~288 by construction.

Consequence for the power stage: the chip received one answer every
~54 ms - AFTER the RESPTIME=10 window closes (28.4 ms @ 64 kHz /
38.5 ms @ 39 kHz) - so every answer was silently rejected, EC pinned at
7, WDA kill latched, power stage dead. No fixed delay value could ever
have fixed this: the armed "27 ms" was really 54 ms of wall time.

Fix (l9779.cpp): the TMR10 rate is now measured against SysTick
(chVTGetSystemTimeX, 1 kHz from HCLK - validated by the CAN bit timing
and the console), not against NT. input_Hz = acDelta*144*1000/stDelta,
newPsc = input_Hz/250000 - 1 -> a REAL 250 kHz (4 us) tick on any
silicon. Expected boot print now: input 144 MHz, PSC 575 (was 1150).
TMR11 uses the same corrected PSC, so per/late become real us again.
The liveness line also prints perNt (the NT-domain fire period) for the
cross-check: after the fix per and perNt must both read ~27 ms.

Expected after flash: the feed answers every real 27 ms, inside the
window on both chip rates, EC decrements 7 -> 4 within ~0.1 s, the
"WDA RECOVERED" line prints and the power stage comes up ~0.3-0.5 s
after key-on. Validation: compile.sh m74_9 -> BUILD SUCCESSFUL; bench
confirmation pending.

## 2026-08-30 (23:50) - the timer clock HALVES after init: the arm is now self-calibrating on the measured real tick

The 22:49 run settles the clock question: the SysTick-anchored
measurement at init reads 283 MHz / final rate 253.5 kHz (SysTick is
cross-validated: 19714 counts over 10 ms at PSC=143 is exactly the
expected 19660 for a 10 ms window at 283 MHz) - the TMR10 clock is
RIGHT at init. At runtime the same ~6730-tick interval takes ~55 ms of
NT (the ring stamps, ok climbing 17.9/s real over 1 kHz SysTick
samples, per=6769 ticks constant): the APB2 timer clock halves
somewhere after init (RCC quirk on this port, root cause still open).
So the armed "27 ms" was really ~54 ms of real time - every answer
landed after the RESPTIME=10 window close, EC pinned at 7, power stage
killed. The earlier 144 MHz theory was wrong about the measurement but
right about the EFFECT: the runtime rate is half the nominal.

Fix (l9779.cpp): wdaTicksForInterval() scales the armed tick count by
the measured real tick (per_ticks / perNt_us, NT-anchored, clamped
0.5x..2x nominal, converges on the first fire): whatever the timer
clock does, the fire lands delay_ms of REAL time after the previous
burst - inside the window. wdArmIsr/wdArmThread compute the ticks once
and pass them to wdaTimerArm(ticks) and the arm-ref for the late probe.
The perNt liveness print is fixed (it printed 0: the divisor was
US2NT(1000000) instead of the 4 ticks/us factor).

Validation: compile.sh m74_9 -> BUILD SUCCESSFUL. Bench confirmation
pending: perNt must read ~27 ms and EC must drop to 4 (WDA RECOVERED).

## 2026-08-30 (24:00) - power stage ALIVE on the bench: EC drops to 1, the chip accepts; the APB2 timer clock question

The self-calibrating arm (scaled by the NT-measured real tick) fixed the
feed: the 22:59 run shows the ring stamping every ~27 NT-ms, perNt
26-31 ms, and the chip ACCEPTING - EC marches 7 -> 4 -> 3 -> 2 -> 1
(WDA RECOVERED lines), the kill clears and the power stage re-enables.
Root cause of the whole saga, now measured directly: the runtime
TMR10/TMR11 clock is 144 MHz = PCLK2 - the AT32F435 APB2 timers do NOT
double (the fork's STM32_TIMCLK2 = PCLK2 x 2 = 288 MHz is wrong; the
authoritative ChibiOS-Contrib port has no timer-doubling concept and the
AT32 CRM CFG has no timer x2 bit). With PSC=1134 the armed "27 ms" was
really ~54 ms: every answer landed after the window close, EC pinned at
7, power stage killed - on the bench AND as a hazard on the car. The
NT-anchored init "measurement" printed 288 because the derivation
cancels the input rate (newPsc = acDelta*144*16/ntDelta), and the
SysTick-anchored re-measure printed 283 for the same reason the virtual
system tick itself is timer-derived (SYSTICKv1 LLD, ST_CLOCK_SRC =
STM32_TIMCLK1) - both inherit the fork's wrong x2 assumption. The
runtime perNt-vs-per ratio (27 ms of NT for 3405 TMR10 ticks = 7.9 us
ticks = 144 MHz) is the ground truth.

Follow-up (same commit): the delay clamp raised to [17, 38] ms - the
bench chip is the 39 kHz variant (TO_EARLY at 27 ms in the 23:00 run,
window [25.9, 38.5], center 32.2 ms), so the verdict adaptation can now
walk up to the 39 kHz center instead of sitting on the window edge; a
64 kHz chip flags LATE at 27 ms and walks down to 22 ms as before.
Validation: compile.sh m74_9 -> BUILD SUCCESSFUL. Remaining: confirm
EC stabilizes at <= 4 (no more kill pulses) on the bench.

## 2026-08-31 (00:30) - the window DRIFTS with temperature: freeze-walk restored as a servo, calibration stabilized

The 23:13 run with the [17, 38] clamp regressed: two TO_EARLY verdicts
walked the delay 27 -> 32 -> 37 and the chip REJECTED at 37 with the
question frozen for 500+ cycles (silent again) - while the 22:59 run
ACCEPTED at 27 with EC down to 1. The chip's f_clk drifts between
sessions (temperature; the saga measured the question-advance period
swinging wildly as the chip warms), so NO fixed delay can lock: the
window moves and the rejection is silent (the verdict flags are cleared
by the next sequencer run before our read, reqhi=0xC0 forever). The
only usable rejection signal is the question freeze.

Fixes (l9779.cpp):
- The question-freeze walk is RESTORED as a servo: on clean un-halted
  cycles with an unchanged question for 8 cycles, step the delay +-5 ms
  in the current direction, reversing at the [17, 55] clamps; a
  question change holds and resets the direction. The delay sweeps
  until the question unfreezes (accepted) and sits there.
- The real-tick calibration is stabilized: the perNt sample must pass a
  sanity band (interval/2 .. interval*2) before it updates the EMA
  (s_realTickMilliUs, starts at 4 us, 1:3 smoothing). The 23:13 run
  chased a 480 us garbage sample and oscillated its ARR 3989..9305 -
  that jitter is what produced the misses.
- The halt test for the walk uses the NT-domain fire period (correct at
  any timer tick), not the TMR11 tick count.

Validation: compile.sh m74_9 -> BUILD SUCCESSFUL. Expected on the
bench: the delay sweeps from 27 until the question advances steadily,
then holds; EC drops to <= 4 and stays; kill pulses stop.

## 2026-08-31 (02:00) - WDA feed moved to TMR7 (APB1): the drifting-timer root cause is GONE, not patched

User directive: stop patching the timer. The TMR10 one-shot saga
(measured-rate init -> SysTick re-measure -> runtime re-calibration ->
freeze-walk servo) was symptom treatment: the APB2 timer clock domain
on the AT32F435 is simply unreliable for precise timing. TIM5 worked
perfectly all along because it lives on APB1. The fix is structural:
the feed now runs on TMR7, an APB1 basic timer that shares TIM5's
validated clock.

Why every previous patch failed (root cause):
- The fork's STM32_TIMCLK2 = PCLK2 x 2 = 288 MHz claim is WRONG for
  the AT32F435 APB2 timers: the runtime TMR10 tick measured 7.9..9.7 us
  per run (144..117 MHz effective input, wandering run to run). A
  one-shot whose tick count assumes 288 MHz fires at 2x the armed
  interval -> every answer misses the chip window -> EC=7 -> kill.
- The old NT-anchored TMR10 PSC measurement was circular ACROSS clock
  domains (newPsc = acDelta*144*16/ntDelta) and always "confirmed"
  288 MHz; the SysTick re-measure inherited the same wrong clock
  (SYSTICKv1 LLD uses ST_CLOCK_SRC = STM32_TIMCLK1 and was only
  coincidentally near right). Neither measurement could see the drift.
- Runtime re-calibration (wdaTicksForInterval) chased garbage samples
  (480 us) and oscillated the ARR 3989..9305; the freeze-walk chased
  the chip's question, which is internal noise (NOT an acceptance
  signal), and walked the delay to 47 ms while the chip kept rejecting.

The fix (l9779.cpp, uncompiled->compiled this session):
- WDA_TIMER = TIM7 (APB1, basic timer, own vector IRQ 55, free: GPT
  FALSE). WDA_REF_TIMER (TMR11) and ALL reference-tick machinery
  removed (s_firePeriodRef/s_lastFireRef/s_wdArmRefTicks/
  s_lateRefUs/s_lateRefUsMax), the runtime re-calibration and the
  freeze-walk removed (wd_frozen_cnt/wd_walk_dir).
- wdaTimerInit() measures TMR7 against the NT domain (TIM5) and
  programs PSC = acDelta*(143+1)*16/ntDelta - 1 -> exactly 250 kHz
  (4 us, NT/16) BY CONSTRUCTION: TMR7 and TIM5 share the APB1 clock,
  so the ratio is exact on any silicon. The old "circular measurement"
  concern does not apply - it only matters across clock domains.
- The arm is a plain (intervalUs+3)/4 tick count. No runtime
  adaptation of the tick.
- Delay policy (ce32509e-proven): RESPTIME=10, init 27 ms, clamp
  [17, 38] ms, adaptation ONLY via REQUHI verdicts (NO_RESP checked
  BEFORE RESP_TO_EARLY, +-5 ms, gated by wd_prev_cycle_clean). 27 ms
  sits inside BOTH chip windows (64 kHz [15.8, 28.4] car / 39 kHz
  [25.9, 38.5] bench); verdicts walk the car down toward 22 ms and
  the bench up toward ~32 ms.
- Vector wiring VERIFIED: Vector11C is a strong symbol in the linked
  ELF (08059238 T Vector11C) and the .vectors table at 0x0800811C
  (IRQ 55) points at it - the weak vectors.S stub is overridden.

Validation: compile.sh m74_9 -> BUILD SUCCESSFUL. Expected on the
bench: boot prints "TMR7 measured ... NT ticks, input 288 MHz
(PSC 143 -> 1151)" and "final rate ... (want 2500/40000 = 250 kHz)",
the 1 Hz pins liveness shows per= ~= delay, EC marches to <= 4 and
stays, kill pulses stop. The bench can only validate mechanics (per/
cnlat stable); the health verdict is the car.

## 2026-08-31 (03:00) - the bench 2x WDA period is DEBUGGER HALTS, not a timer clock problem: TMR7 halt bit + on-time servo gate

The 23:37/23:42 bench sessions with the TMR7 build showed per=53.9..69 ms
for a 27 ms arm (min = exactly 2x), late = per - 27 ms exactly, cnlat=4 us
(the ISR always enters right at the wrap), and the init measurement printed
the TMR7 input at exactly 288 MHz (20002/40004 and 2500/40001 NT-tick
ratios). The only consistent model: the timer clock is fine; the bench
debugger halts the core periodically, and the halt pauses TMR7 mid-count
while NT (TIM5) keeps counting. Every halt stretches the WDA cycle by the
halt duration; the chip's own RC WDA clock never pauses, so each stretched
cycle misses the window and EC climbs (the bench kills).

Why TMR7 pauses and TIM5 does not: DBG1=0x8 = APB1_PAUSE bit 3 (TIM5) set,
bit 5 (TMR7) clear. On this silicon the debug pause bits behave INVERTED
vs STM32F4 (a SET bit keeps the timer running through a core halt, a CLEAR
bit pauses it) - consistent with the saga's own observation that TMR11
with its pause bit CLEARED kept pausing on halts. The fork's microsecond
timer init sets TIM5's bit (intending "freeze on halt"), so NT runs
through halts - which is why NT time stayed coherent all along and "TIM5
worked perfectly".

Fixes (l9779.cpp):
- wdaTimerInit() now sets DBGMCU APB1FZ bit 5 (TMR7): the one-shot wraps
  on wall time, the UIF pends through the halt, and the answer fires as
  soon as the core resumes - the feed keeps its cadence and the answer
  phase stays locked to wall time like the chip's RC window.
- ON-TIME GATE on the delay servo: REQUHI verdicts are applied only when
  the PREVIOUS cycle fired within +-50% of its armed delay. A stretched
  cycle's verdict is real but not actionable (the delay was right, the
  miss was the stretch); the saga's 19:02/23:13 wrong-way delay walks
  chased exactly such verdicts. Tracked via s_prevFirePeriodNt/
  s_prevFireDelayMs stashed in the handler before the feed runs.
- Liveness line gains stretch=N (off-time fires in the last second) and
  ratio=N% (per/delay) so one pins dump separates halts from a genuine
  clock problem; the event-ring halt flag uses the same 1.5x threshold.

Validation: compile.sh m74_9 -> BUILD SUCCESSFUL. On the bench expect
per to snap back to ~27 ms and ratio ~100% between halts; stretch counts
the halted cycles. The car (no debugger) is unaffected by the halt bit;
its behavior is determined by the init-proven 288 MHz timer clock.

Correction to earlier notes: the saga's APB2 "does not double / drifts
144..117 MHz" measurements were made under the same bench-halt pollution
(the TMR10/TMR11 windows were stretched by halts, and the old code had
the un-latched ARR bug on top). Re-verify APB2 clocking with a clean
no-debugger measurement before trusting any absolute APB2 rate.

## 2026-08-31 (04:00) - RETRACTION: the DBGMCU halt theory is wrong - the bench has NO debugger

User fact: the bench has no debugger attached - the Java console is the
only runtime tool. The DBGMCU APB1/APB2 pause bits only gate peripheral
clocks when a debugger halts the core (C_DEBUGEN), so they are INERT on
this bench and cannot explain the 2x WDA period. The previous entry's
"TMR7 pauses on debugger halt, TIM5 does not" mechanism is retracted.
(What DBGMCU is and who writes it: debug-support registers at
0xE0042000 - CTRL/APB1_PAUSE/APB2_PAUSE. Writers: the firmware at init
(microsecond timer sets TIM5 bit 3, angle clock TIM2 bit 0, l9779 now
TMR7 bit 5 - all for coherent freeze under a FUTURE debug session) and
a debugger itself on attach. Without a debugger they do nothing.)

The measurements stand and the mystery is therefore REAL:
- init: TMR7 input = exactly 288 MHz (20002/40004 and 2500/40001 NT-tick
  ratios, two independent windows).
- runtime: ok=4771 over ~276 s PC-wall time = ~58 ms per feed for a 27 ms
  arm -> effective tick ~8.6 us -> input ~130 MHz. NT stays coherent
  (per matches wall). per=53.9..69 ms = exactly 2x..2.56x the armed
  interval, late = per - 27 ms exactly, cnlat=4 us (ISR prompt, the WRAP
  itself is late in NT time).
- TIM5/TIM2 (32-bit, PSC 71) are stable; the drifting ones are the
  16-bit basic timers (TMR7 now, TMR10 in the saga - same ~144->112 MHz
  signature). The TMR10 saga's "APB2 does not double / drifts" numbers
  were probably measuring THIS effect, not halt pollution.

Open question (next measurement): a runtime re-measurement of the TMR7
rate against NT AND against SysTick (HCLK-derived, validated by CAN/USB
timing), plus a CRM register dump (CFG/APB1DIV, MISC1/MISC2, APB1EN),
added to the pins diagnostic - one dump then settles whether the timer
input clock really changed after init, and which CRM field (if any)
tracks it. Suspects to check against that dump: a runtime APB1DIV or
PLL re-lock, the auto-step machinery, or a per-timer clock mux on the
AT32F435 that the fork's headers do not model.

Code state after the retraction (l9779.cpp, commit after 8f0df3a033c):
- the TMR7 DBGMCU pause bit stays SET (same bit state as TIM5 - whatever
  the silicon polarity, both timers then behave identically under any
  future debugger; inert without one);
- the on-time gate on the delay servo stays and is now cause-agnostic
  (off-time cycles carry no chip-window phase info, so their verdicts
  are ignored regardless of what stretched them);
- comments rewritten to record the open question instead of the
  retracted halt theory.

## 2026-08-31 (05:00) - runtime timer cross-measurement added to pins (SysTick-anchored)

Tool for the open drift question: `pins` now ends with a three-way
cross-measurement (l9779.cpp wdaTimerCrossMeasure, called from
L9779::debug):
- Register dump: RCC->CFGR decoded (SW/SWS/HPRE/PPRE1/PPRE2) + PLLCFG +
  MISC1 + MISC2 + APB1EN, and TIM5/TIM2/TMR7 PSC/CNT/CR1.
- Measurement A (masked ~10 ms): NT (TIM5) and TMR7 tick rates against
  the CORE SysTick (STCLK = HCLK/8 = 36 MHz). The WDA ISR is masked
  (BASEPRI 5) so TMR7 free-runs without re-arms; the handoff/executor
  stay unmasked and do not touch these counters. Expect NT 4 MHz, TMR7
  250 kHz (input 288 MHz); a drifting timer prints ~125 kHz / ~144 MHz.
- Measurement B (unmasked ~100 ms): the ChibiOS virtual tick vs SysTick.
  NOTE: on this port the system tick runs on TIM2
  (STM32_ST_USE_TIMER=2, ST_CLOCK_SRC=STM32_TIMCLK1) - the same APB1
  domain as the timers under test, so it is NOT an independent
  reference; the HCLK-derived core SysTick is (HCLK validated by USB
  48 MHz PLL output + CAN timing).

Why this settles the saga: one pins dump now answers whether the TMR7
input really changed after init (288 -> ~144 MHz), whether NT stays at
4 MHz, and whether the TIM2 system tick drifts - each compared against
the same HCLK wall clock in the same second. Expected on the bench:
the mystery 2x per= either reproduces as wdaKhz~125 (timer input
halved -> hunt the CRM field) or as NT~4/tick~250 (the counters are
fine -> the stretch must come from ISR-level stalls, and cnlat would
be the next lead).

Validation: compile.sh m74_9 -> BUILD SUCCESSFUL. The masked window
costs the chip one WDA window (EC++) per pins - bench-only tool.

## 2026-08-31 (11:38 bench) - FIRST cross-measurement: timers EXACT at runtime, kernel tick is core SysTick (not TIM2), measurement tool fixed

The 11:38 pins runs printed (three samples):
- CFGR=0x3808900A: SW/SWS=PLL, HPRE=DIV1, PPRE1=DIV2, PPRE2=DIV2 - the
  CRM config is exactly as intended. MISC1=0, MISC2=0xB00D (AUTO_STEP
  off), APB1EN=0x12044078 (TIM5/6/7/12 on, TIM2 OFF).
- NT 4 MHz (want 4) | TMR7 tick 250 kHz input 288 MHz (want 250/288),
  tm=2500 nt=40000 per 360000 HCLK ticks - the WDA timer is EXACTLY
  right AT RUNTIME in this session. The "NT 3 MHz" in the third sample
  is an integer-truncation artifact of the first tool (399 -> 3), fixed
  with one-decimal prints.
- TIM2 PSC=0 CNT=0 CR1=0 and its APB1EN bit cleared: the ChibiOS system
  tick on this port runs on the CORE SysTick, NOT TIM2 -
  STM32_ST_USE_TIMER=2 in the mcuconf is not honored by the actual ST
  driver. Confirmed functionally: the virtual tick froze while the first
  measurement tool had the core SysTick disabled (ST 0 ms per 3600051
  ticks). COROLLARY: the saga's claim that the virtual tick is
  TIMCLK1-derived (and therefore not an independent reference) is WRONG
  - the old SysTick-anchored TMR10 measurement was actually valid, and
  its ~283 MHz reading was a real ~1.7% off-288 measurement.
- Also observed: ARR=7979 -> wd_delay_ms walked 27 -> 32 ms - the chip
  IS reporting verdicts now (RESP_TO_EARLY, +5 ms), unlike the 23:42
  session where delay stayed 27 with reqhi=0xC0.

TOOL BUG FIXED (same commit): the first version of the measurement
hijacked the core SysTick (CTRL off/on with LOAD=0xFFFFFF) and did NOT
restore LOAD - the kernel tick then ran with a ~466 ms period instead
of 1 ms until the next reset (the bench's periodic resets masked it).
wdaTimerCrossMeasure now uses DWT->CYCCNT (the core HCLK cycle counter,
one-time enable, owned by nobody): zero disturbance, same reference.

STILL OPEN: whether per= is still 2x in THIS build - the 11:38 paste did
not include the wda[0..9] liveness lines. Next bench run: full pins
output. If per=2x while the cross-measure says 250 kHz, the stretch is
a counter-stall mechanism, not the clock; if the cross-measure says
~125 kHz, the run-to-run drift reproduced and the CRM hunt begins.

## 2026-08-31 (12:00) - ANSWERED: the 2x period is the STOP/RE-ARM cycle, not the clock - TMR7 converted to a free-running auto-reload timer

The 11:50 bench session settled the open question in one pins dump:
- the cross-measurement prints EXACT clocks every time while the live
  feed simultaneously runs at 2x: "ST 100 ms per 28800000 HCLK cycles",
  "NT 3.9 MHz", "TMR7 tick 250.1 kHz" - the timer clock is NOT drifting.
- the live lines show per=57953..72534 us for a 27 ms arm (2.1..2.7x),
  ratio=214-268%, stretch=16/16, late = per - 27 ms exactly, cnlat=4 us
  (ISR prompt, the wrap itself late in NT time) - the fire-to-fire
  interval stretched while the counter's rate was provably exact.
- wda[4] per=1100us: a ~1 ms fire right after the previous one - the
  signature of a spurious immediate UIF from the arm sequence
  (EGR|UG sets UIF; the SR=0 clear races with the re-enable), after
  which the next real fire comes a full period later.
- minimum per = exactly ~2x with nothing between ~1 ms and ~54 ms.

CONCLUSION: the stretch lives in the stopped/re-armed one-shot cycle
(CR1=0 -> CNT=0 -> ARR -> EGR|UG -> SR=0 -> CEN per fire), not in the
clock. The counter ran perfectly when not being re-armed every cycle
(the masked measurement), so the AT32 16-bit basic timer must not be
stopped between fires for periodic work.

THE FIX (this commit, l9779.cpp): TMR7 is now a FREE-RUNNING
auto-reload periodic timer - the TIM5 pattern the whole saga was
chasing.
- Started once (boot kick / post-reset self-heal full start:
  CR1=0 -> CNT=0 -> ARR -> EGR|UG -> SR=0 -> DIER|UIE -> CEN).
- The ISR clears UIF and dispatches; it NEVER disarms. The auto-reload
  restarts the count at the wrap and fires every ARR+1 ticks.
- wdaTimerArm is now "set the period": while running it latches a new
  ARR via EGR|UG (PR is preload-only on the AT32 - the 18:17 un-latched
  ARR lesson) ONLY when the period actually changed - an unchanged
  period is a no-op that preserves phase and avoids the UG spurious-UIF
  hazard. The UG re-inits CNT, so a changed period keeps the exact
  from-now semantics of the old one-shot (defer 1 ms / SPI-fail 10 ms
  retries and the +-5 ms REQUHI adaptation still work unchanged).
- CEN stays 1 while the feed runs; the thread's liveness heal
  (!CEN -> re-kick) can now only fire on a genuinely dead peripheral.
- The on-time verdict gate is KEPT as a cause-agnostic safety net: with
  the free-run timer it should idle at ratio~100% and only flag the
  short defer/retry cycles, letting the REQUHI verdicts move the delay
  again.
- Print bug fixed: the cross-measurement's TMR7 "input MHz" divided by
  10000 instead of 1000 ("28.7 MHz" should be "287.8 MHz").

Validation: compile.sh m74_9 -> BUILD SUCCESSFUL. Bench validation
pending (flash + pins): expect per~27 ms, ratio~100%, stretch=0,
delay walking to its window (up toward ~32 ms on the 39 kHz bench chip,
down toward ~22 ms on a 64 kHz car chip), EC settling <=4 with no KILL
pulses. The bench validates mechanics only; the health verdict is the
car. The cross-measurement stays as the ground-truth instrument.

## 2026-08-31 (12:19 bench) - the stop/re-arm theory is WRONG: the 2x period is the SLEEP-MODE CLOCK GATE (APB1LPENR)

The 12:19 bench run of the free-run build is decisive AGAINST the
stop/re-arm theory: per is STILL 2.1..2.6x (57321..71166 us for a 27 ms
arm, stretch on every cycle), while the masked cross-measurement prints
exact clocks in the SAME pins dump (tm=2500 nt=40000 cyc=2880009 ->
250 kHz). So the free-run conversion changed nothing measurable - the
stretch was never the CR1=0/CNT=0 stop cycle.

The real root cause, found in the fork's CRM macros
(firmware/ChibiOS/os/hal/ports/AT32/AT32F4xx/at32_rcc.h):

    rccEnableAPB1(mask, lp): APB1ENR |= mask;  if (lp) APB1LPENR |= mask;
                                             else  APB1LPENR &= ~mask;

- On this AT32 port, rccEnableTIMx(false) CLEARS the LPEN bit, which
  gates the peripheral clock OFF in SLEEP mode. ChibiOS idles (WFI)
  constantly, so a false-enabled timer only advances during CPU-awake
  windows - the effective rate tracks the console-poll wake duty, which
  is exactly why it looked like run-to-run drift (TMR10's 144..117 MHz
  saga) and why the stretch factor varies 2.1..2.6x.
- The WDA timer was enabled with rccEnableTIM7(false) -> TMR7LPEN=0 ->
  counter frozen in sleep -> fires stretch. TIM5 is immune because the
  TIMv1 PWM LLD enables it with rccEnableTIM5(true) (all 53 LLD timer
  enables in the fork pass lp=true) - that is why the executor feed
  "worked perfectly" and why NT measures exact.
- The masked pins cross-measurement always printed the exact rate
  because its busy-wait (do{}while on CYCCNT) keeps the CPU awake - the
  instrument was blind to the sleep gating by construction. Its "exact
  while the live feed is 2x" contrast is what finally localized the
  bug to sleep.
- Supporting evidence: the 11:38 triple pins dump shows TMR7 advancing
  only ~5k ticks over 7-46 s of TIM5 time (counter almost frozen between
  the busy pins commands), and the 1 Hz liveness CNT samples are random
  phase, not the steady 1010-tick/sample drift a continuously running
  250 kHz counter would show.

THE FIX (this commit):
- l9779.cpp wdaTimerInit: rccEnableTIM7(false) -> rccEnableTIM7(true).
- angle_clock.cpp initAngleClock: rccEnableTIM2(false) ->
  rccEnableTIM2(true). THIS ONE MATTERS ON THE CAR: TMR2 (the angle
  clock) was also sleep-gated while TIM5 (the NT domain) ran - the
  init-measured NT<->TMR2 offset drifts on every sleep period (key-on
  engine-off, console idle), so armed absolute ticks would fire at the
  wrong time after wake. The free-run conversion is KEPT: it is the
  TIM5 pattern, removes the per-cycle stop and the UG spurious-fire
  hazard, and the !CEN heal can only fire on a genuinely dead
  peripheral - but it was not the 2x cause.

Validation: compile.sh m74_9 -> BUILD SUCCESSFUL. Next bench run must
show per~27 ms / ratio~100% / stretch=0 with the SAME idle bench (no
console pokes needed) - if it does, the saga is closed and the car
verdict is the only open question.

## 2026-08-31 (12:41 bench) - the sleep-gate fix is CONFIRMED: per exact, EC=0; the pins diagnostic was destructive; delay-servo escapes added

The 12:41 flash of the lp=true build settled the saga:
- per=26919..26921 us for a 27 ms arm - EXACT (was 57..71 ms), ratio=99%,
  stretch=0, late=0, cnlat=4 us, on an IDLE bench with no console pokes.
  The sleep-mode clock gate (APB1LPENR) was the one and only root cause.
- **ec=0 wda_int=0 with ecDown=7**: the chip accepted every answer and
  decremented EC 7->0 for the FIRST TIME EVER on the bench. Two of the
  saga's chip-side conclusions are thereby REFUTED: (1) the bench chip's
  EC mechanism is NOT broken; (2) the RESPTIME=10 write DOES take effect
  (nothing near the default ~112 ms window would be accepted at 27 ms).
  The bench chip is 64 kHz: window [15.8, 28.4] - accepts 27, silently
  rejects 32. The "39 kHz bench chip" and "RESPTIME ignored" theories
  were artifacts of the 2x-stretched feed (answers at 54..69 ms landed
  past every candidate window; flags cleared between reads; EC saturated).
- The pins diagnostic itself was DESTRUCTIVE: right after confirmation_pins
  came "WDA KILL: EC=6". Chain: the cross-measurement's masked 10 ms window
  delayed one answer ~10 ms -> NO_RESP -> the chip's auto-restart advanced
  its cycle phase -> a cascade of misses knocked EC 0->6, and one of those
  perturbed cycles produced a verdict (1.37x off - invisible to the old
  +-50% on-time gate) that walked the delay 27->32. At 32 ms the chip
  rejects SILENTLY (reqhi=0xC0 forever - the flags are cleared by the
  chip's sequencer run before our next read), so the REQUHI servo is blind
  and EC pinned at 7 (the 12:42 dump: ecUp=5 ecDown=0, delay=32, miss=1).

Fixes in this commit (l9779.cpp):
- The on-time verdict gate is TIGHTENED from +-50% to +-25% of the armed
  delay: the pins perturbation (10 ms on 27 ms = 1.37x) now flags the
  cycle and its verdict is ignored - one perturbed cycle can no longer
  walk the delay out of the acceptance zone.
- EC-saturation escape (WDA_EC_SAT_ESCAPE_CYCLES=8): when EC >= 6 on
  clean on-time cycles while the delay is off-center, step the delay
  toward WDA_DELAY_INIT_MS (27 - inside BOTH candidate windows, the
  proven-healthy point). This recovers the silent-rejection zone the
  REQUHI servo cannot see. EC starts at 6 on reset and decrements on
  accepted answers, so a healthy 27 ms feed never arms the escape.
- The cross-measurement no longer masks the WDA ISR: the free-run feed
  does not touch the timer on normal cycles (the no-op arm), so pins is
  non-destructive again. A rare mid-window period change would corrupt
  that one dump's tmDelta (self-evident from the ARR line).
- The liveness stretch counter and the event-ring flag use the same
  +-25% threshold as the gate.

Validation: compile.sh m74_9 -> BUILD SUCCESSFUL. Next bench run should
boot to EC=0 at delay=27 and STAY there across repeated pins calls
(stretch=0, ratio~100%); the KILL-after-pins behavior must be gone.

## 2026-08-31 (12:52 bench) - BENCH VALIDATION COMPLETE: EC=0 held for 1000+ cycles across repeated pins calls, no KILL-after-pins

Two pins dumps 28 s apart, both clean:
- per=26918..26922 us (armed 26.92 ms) - EXACT, ratio=99%, stretch=0/1
  (the lone stretch = one 10 us entry delay), cnlat=4 us, late=0/10 us.
- First dump: ec=0 wda_int=0, ecUp=1 ecDown=7 (the boot walk 7->0).
  Second dump (28 s later): ecUp=0 ecDown=0 ecSame=1043 - EC held at ZERO
  for 1043 consecutive cycles. fail=0 wrong=0 cntbad=0 miss=0 addr_err=0.
- delay stayed at 27 ms - the tightened +-25% gate no longer lets the pins
  diagnostic walk it, and the EC-saturation escape was never armed (EC=0).
- NO "WDA KILL" after either confirmation_pins - the unmasked
  cross-measurement is non-destructive.
- foreign=12/6 entries: exactly the UG-latch pairs of the defer cycles
  (the pins dbg batch defers the feed 1 ms; each defer latches ARR 6729->
  229->6729, and each UG leaves one pending NVIC entry that re-enters with
  UIF clear). Harmless one-wasted-entry artifact, diagnostic evidence the
  latch race is understood.

BENCH VALIDATION COMPLETE. The bench now proves the whole chain: exact
sleep-proof timer period, chip acceptance at 27 ms with EC pinned at 0,
non-destructive diagnostics, stable servo. The only remaining verdict is
the CAR (its chip is the same 64 kHz window [15.8, 28.4] ms - 27 ms sits
inside with +-5% CLK1 drift margin).

## 2026-08-31 (13:04/13:17, CAR) - lockstats scheduler-lateness analysis at idle (~1200) and 2000 rpm

Three lockstats dumps analyzed (13:04:08/13:04:55/13:05:32 at ~1200 rpm,
13:17:26 at 2000 rpm). Semantics first (from the code): `late` =
actual-execution instant minus scheduled moment, captured with a fresh
getTimeNowNt() PER event inside the executor's batch (event_queue.cpp
`executeOne`, m_lateDelay = 8us: everything due within 8us of the CC1
fire executes in one batch with spin-waits). So an event behind a slow
callback in the same tick measures the predecessors' duration as its
"late" - the metric is batch order + ISR entry, not preemption (the
executor is prio 3; only the 2-3us fast EXTI preempts it). The 4-16us
mode of 85% of all events = executor entry + batch overhead, normal.

Resolved othercb addresses (addr2line on the matching ELF):
- 08058429 = startAveraging (MAP window start), 08058335 = endAveraging
  (MAP window end), 0804A0F5 = startKnockSampling (knock window start),
  08049D41 = onTdcCallback (TDC), 0807175D = watchDogBuddyCallback
  (scheduler watchdog), 0803E53D = timerValidationCallback (one-off),
  0804B871 = PrimeController::onPrimeStartAdapter, 08072257 =
  PrimeController::onPrimeEndAdapter (the priming pulse pair).

Idle (~1200): exec late>=10us 69-80%, maxLate 114us. Spark 88-93% late
but tight (max 20-21us in the clean sessions) - it rides behind the
fuel-END event that lands 8-15us earlier in the same batch. Fuel 82-97.5%
late, max 87-91us, fuel callback itself up to 80us. Worst measured
"victims": startAveraging up to 109us late (85-94% rate) - batched with
the spark tick; startKnockSampling (n == dwell count) behind dwell; TDC
behind dwell. Self-duration leaders: dwell cbmax 190us in session 1
(41-43us later - a rare outlier), fuel 74-80us, spark 49-57us.

2000 rpm: the same mechanism, the tails grow - tooth period 517us, so
1us = 0.012 deg. exec late>=10us 75.7%, maxLate 161us = 1.9 deg; fuel
94% late max 136us = 1.6 deg; spark max 161us; dwell late-rate stays low
(28.8%) but cbmax hits 202us AGAIN (1 in ~23k) and the first overdwell
event takes 189us. schedulerIsr now spends 64% of entries in 50-100us
(more events coalesce per tooth at 2k). 430 events in the 64-256us
bucket = 0.8-3.1 deg. Trigger side: avg 45us, one 764us decode in
~666k teeth - at 517us tooth period that tail overlaps the next tooth
and can miss the 1-tooth arming window (the known open item, now
rpm-relevant).

Conclusions:
- The idle/2k lateness is batch-order, not ISR load; the engine-timing
  commands float by the batch penalty. At 2k the tails (1.6-1.9 deg)
  become knock-relevant.
- EFI_ANGLE_CLOCK is FALSE in this build (disabled 2026-08-30 after the
  fuse incident), so ALL dwell/spark/fuel run the classic TIM5 path - the
  lockstats sched lines prove it (with the angle clock ON those events
  would be deleted from the angle queue and fired by TMR2, and the
  per-kind counts would be ~0).
- Two open items: (1) the dwell 190-202us outlier - the only non-trivial
  path in turnSparkPinHighStartCharging is the CUSTOM_OBD_SKIPPED_SPARK
  warning() console print from ISR; confirm by grepping the car log for
  "looks like skipped spark event"; (2) the 764us trgPostDecode tail.
- Next lever: re-enable EFI_ANGLE_CLOCK (TMR2 now fixed from the
  sleep-gate) - dwell/spark/injection-start move to the 2-tooth-lead
  hardware compare and out of these batch statistics entirely.

## 2026-08-31 (14:30) - EFI_ANGLE_CLOCK RE-ENABLED on m74_9 (TMR2 sleep-gate fix is the fuse-incident explanation)

Per user directive. Safety rationale before the flag flip: the 2026-08-30
14:13 fuse incident was never root-caused at 17:05 ("one-tooth-ahead
scheduling logic bug" was a hypothesis, not a finding). The sleep-mode
clock gate found today (rccEnableTIM2(false) cleared the AT32 APB1LPENR
bit) is the stronger explanation for the whole 14:13 signature: on the
car the CPU idles (WFI) between cranking teeth, so TMR2 was gated off
~80-90% of the time while TIM5 (NT) kept running - armed absolute ticks
were reached by the frozen counter late/scrambled at the catch -> C9012
out-of-order coil off + C9351-4 overcharge + overdwell rescue of all
four coils at once = the fuse. The counter now ticks in sleep like TIM5
(rccEnableTIM2(true)), so the freeze mechanism is gone.

Changes: efifeatures.h EFI_ANGLE_CLOCK FALSE -> TRUE (comment rewritten
with the history and the car-catch caveat); pch/pch.h touched (efifeatures
edits do not trigger a rebuild otherwise). Build: compile.sh m74_9 ->
BUILD SUCCESSFUL; VectorB0/angleClockArm/angleClockTickForNt linked in.

Validation plan: bench first - lockstats must show the 'angclk fired='
line with lateArm=0/noChannel=0 in self-stim, and the per-kind
sched dwell/spark/fuel counts must DROP (events now armed on TMR2 one
tooth ahead and deleted from the angle queue; only multispark/
injection-ends/windows/TS stay on TIM5). Then the car: the catch is the
verdict - watch C9012/C9351 at the first start (the overdwell protection
is the safety net).

## 2026-08-31 (13:32, CAR) - the angle clock blew the fuse AGAIN: DISABLED again, the scheduling defect is real

The re-enabled angle clock reproduced the 14:13 incident at the first
crank attempt - the 15A fuse blew. So the sleep-gate was NOT the cause
(or not the only one): the one-tooth-ahead scheduling defect that 17:05
flagged as a hypothesis is REAL and still unfixed (the bench
investigation planned at 17:05 - SPARK_EXTREME_LOGGING at simulated
cranking - was never done; the WDA saga consumed the time).

Code-level defect analysis (trigger_scheduler.cpp + spark_logic.cpp):
- The overdwell safety net is CANCELLED when the fire is armed on TMR2:
  trigger_scheduler.cpp's early branch does engine->scheduler.cancel
  (sDown) - sDown is the same eventScheduling instance the overdwell
  was scheduled on (spark_logic.cpp [tag:overdwell]). Any fire that goes
  through the angle clock therefore has NO overcharge protection. The
  14:13 C9351-4 warnings prove the rescue only ran for the events that
  fell back to TIM5; the TMR2-armed fires had no rescue at all.
- At the catch, the fire is armed 1-2 teeth ahead with the LAGGING
  oneDegreeUs (90-deg moving average) - the prediction error is ~2x the
  TIM5 path's (which converts at the last tooth). During the 250->1500
  rpm catch this is ms-scale: overcharge in the 4.5-8.2 ms band of the
  14:13 signature.
- The 4-channel limit at the catch (2 dwell starts + 2-4 injection
  starts + 2 fires all converging in the first cycle after sync) splits
  the events between TMR2 and the TIM5 fallback - mixed firing paths
  scramble the dwell/fire ordering (C9012 out-of-order coil off).

SAFETY ACTION: EFI_ANGLE_CLOCK is FALSE again (commit 154e10941f5,
BUILD SUCCESSFUL - flash firmware/build/rusefi.srec to restore the
proven time-based path). Re-enabling requires: (1) keep a redundant
overdwell rescue for TMR2-armed fires (the cancel must not remove the
last protection; note the single-writer contract on
prepareCylinderIgnitionSchedule - a double prepare would re-arm the
next dwell twice), (2) bench-validate the catch transient with
SPARK_EXTREME_LOGGING at simulated cranking rpm before any car test.

## 2026-08-31 (evening) - angle clock redesign: angle-domain arming, per-tooth refresh, kept rescue (IMPLEMENTED, not car-tested)

The 13:32 entry's fix plan assumed a bench that does not exist (no CKP
emulator) and proposed an rpm gate that is wrong in principle - the
angle clock must be accurate at ANY rpm. Redesigned instead, driven by
the log fact that all four coils overcharged in the SAME millisecond:

Root causes (code-verified):
- The armed tick was computed from oneDegreeUs (90-deg rpm average),
  which lags by revolutions at the catch: every armed event fired
  ms-late. The per-tooth refresh bounds the error to one tooth of
  acceleration instead.
- The overdwell rescue was cancelled at arm time
  (trigger_scheduler.cpp), so TMR2-armed fires had no charge cap.
- Stale events were executed unconditionally by the TMR2 ISR - a late
  charge start is what piles all coils onto one instant.

Implemented (all under #if EFI_ANGLE_CLOCK - the FALSE build is the
proven time-based path, verified symbol-clean):
- angle_clock.h/.cpp: channels now store {action, kind, targetAngle};
  angleClockOnTooth() feeds the fresh last-tooth basis (NT ticks per
  degree, from the decoder's toothDurations[0] and the real tooth span -
  the gap tooth carries its true ~18 deg); angleClockArm() arms in the
  ANGLE domain; angleClockRefresh() re-anchors every armed channel each
  tooth and applies the stale policy: CoilFire fires immediately when
  its angle arrives, a late Start (dwell/injection) is dropped, a stale
  phase basis (desync) cancels the channel; the ISR also drops Start
  events that are >1 ms late. angleClockCancelAll() on rpm==0 and
  firmwareError.
- trigger_central: lastToothTicksPerDegree measured in handleShaftSignal
  from the phase pair (span crosses the cycle boundary correctly).
- trigger_scheduler.cpp: the early branch KEEPS the overdwell rescue
  and arms fires by angle; the fallback fires by time on the same fresh
  basis (rescue superseded, cancel first - one event per scheduling_s).
- spark_logic.cpp: dwell start armed by angle; chargeTime/rescue anchor
  on the fresh basis; IgnitionEvent::sparkFiredSinceCharge makes the
  rescue idempotent with a TMR2 fire that already happened (single
  writer for the coil-off; set at fire, cleared at charge start).
- main_trigger_callback.cpp: injection start armed by angle; OnTooth +
  Refresh per tooth.
- lockstats angclk line gains 'dropped='.

Validation: compile_m74_9.sh BUILD SUCCESSFUL with the flag TRUE (nm
shows angleClockArm/angleClockOnTooth/angleClockRefresh/VectorB0) and
FALSE (zero angle-clock symbols); unit tests 1169/1169 PASSED
(test_angle_clock.cpp updated to the ticksPerDegree basis). NOT
car-tested - the car verdict is the first crank (watch C9351-4, angclk
dropped=, dwelloverchargecounter).

Open: the desync-recovery and multispark interaction are reasoned but
not measured; the per-tooth refresh adds ~4 CCR computations per tooth
in the handoff (not yet profiled with lockstats).

## 2026-08-31 (late evening, BENCH self-stim) - redesign caught by the bench: stale rescue anchors, two fixes

The redesigned build was run on the bench with self-stimulation and the
log reproduced the fuse mechanism: C9351-4 overcharges of 8.1, 213 and
264 ms + C9012 + C9002, angclk noChannel=126, maxLateUs ~2^32 (wrap).
Root causes found and fixed:

1. Garbage first-tooth basis: the first tooth pair after a long pause
   carries the decoder's 10 s clamp (or self-stim's slow ramp), so
   lastToothTicksPerDegree armed far-future ticks: channels stuck
   (noChannel=126), fallback fires/rescues anchored seconds out, the
   wrapped float->uint32 delay produced the wrap-scale maxLateUs.
   Fix: sanity-clamp the fresh basis to a 16x band around the rpm
   average in handleShaftSignal (after the rpm update) - the legit
   catch transient diverges at most a few x.
2. Stale rescue anchor: the overdwell rescue was anchored to the
   PREDICTED chargeTime; the per-tooth refresh re-anchored the TMR2
   dwell/fire to the correct times while the TIM5 rescue stayed on the
   stale prediction - the coil charged on time and nothing discharged
   it for 213/264 ms. Fix: the rescue is now anchored at the ACTUAL
   charge moment (turnSparkPinHighStartCharging, 1.5x planned dwell on
   the event's dwellStartTimer, armed only when sparksRemaining==0 -
   multispark reuses that struct); the fire cancels it; the rescue
   also cancels a pending time-based fallback fire (the C9012 double
   fire). The schedule-time rescue stays ONLY in the FALSE build.

Validation: TRUE + FALSE firmware builds OK (nm: 4 angle-clock symbols
vs 0), unit tests 1169/1169. Bench re-run pending - expect
noChannel~0, no C935x, angclk maxLateUs small.

## 2026-08-31 (16:00, BENCH) - rescue bound works (4.5 ms), three follow-ups fixed

The clamped+charge-anchored build was re-run on the bench: C9351-4
overcharges are now EXACTLY 4.5 ms (1.5x the 3 ms dwell - the
charge-anchored rescue bounding the charge precisely), no 213/264 ms
monsters, no fuse. Three remaining issues fixed:

1. noChannel still ~115: the relative 16x basis clamp cannot see the
   10 s decoder clamp when the rpm average itself is stale (NaN at the
   first teeth). Fixed with an ABSOLUTE ceiling on the basis - 5 ms/deg
   (~32 rpm equivalent) applied in angleClockOnTooth AND on the stored
   field, bounding any armed delay to 150 ms (MAX_LEAD_DEG x the cap).
2. maxLateUs ~2^32 telemetry wrap: the refresh re-anchored a channel
   whose old CCR had already matched while the TMR2 ISR was masked by a
   chSysLock section of the same handoff - the pending ISR then measured
   late = CNT - the NEW far-future ccr. Fixed: the refresh skips
   channels with a pending CCxIF (they re-anchor next tooth).
3. C9012 out-of-order coil off: a fire arriving after the rescue had
   already discharged the coil. Fixed: fireSparkAndPrepareNextSchedule
   no-ops on sparkFiredSinceCharge (the same idempotency flag the
   rescue uses).

Validation: TRUE build OK (one build, 4 angle-clock symbols). Bench
re-run pending - expect noChannel ~0, maxLateUs microseconds, no C9012.

## 2026-08-31 (evening, CAR) - the rpm==0 storm-flap cancelAll was the car defect

The 16:00/16:08/16:09 cranking logs are the CAR, not the bench (user
correction). Every attempt: C9012 once early, then all four coils
C9351-4 overcharge ~4.5 ms within the same 3-4 ms, then C9002 "expected
58/0 got 58/0", engine stopped; angclk fired=6..11 dropped=6..11
noChannel=115..156 maxLateUs~2^32 (wrap). "Events arrive late."

Root cause found in code: mainTriggerCallback's rpm==0 early-return
called angleClockCancelAll(). On this car the catch trigger storm flaps
SensorType::Rpm 0/300+ at ~1 kHz (long-documented), so every flap
cancelled the armed TMR2 events:
- armed CoilFires -> charged coils discharged by the 4.5 ms overdwell
  rescues instead of their armed fires (the C935x cluster - the fires
  did not happen at their scheduled moments);
- armed dwell Starts -> the coil never charged, dwellStartArmed stays
  set, and the cycle's fire (armed separately) still fires into an
  uncharged coil -> C9012 out-of-order coil off;
- the storm's garbage basis additionally saturated the 4 channels (the
  noChannel storm), so the rest fell back to TIM5 and drained in
  bursts.

The FALSE build has none of this: its events are scheduled by time and
fire on time regardless of the rpm flap - which is why it has been
proven on this car for months.

Fixes (all EFI_ANGLE_CLOCK-guarded, the FALSE build untouched):
1. Removed the angleClockCancelAll() calls from the rpm==0 and
   firmwareError handoff paths. Armed events fire at their bounded
   ticks exactly like the time-based build (by time, regardless of the
   rpm flap); the charge-anchored overdwell rescue remains the coil
   safety net, and the m74_9 ignition gate cuts the power stage at
   key-off.
2. angleClockRefresh now LEAVES armed channels alone on a phase-basis
   jump (desync/re-sync) instead of cancelling them - the armed tick is
   absolute time and fires within 1-2 teeth, mirroring the time-based
   build. Cancelling there produced the same lost-fire signature.
3. The refresh CCR re-anchor is race-proof vs the prio-3 TMR2 ISR: the
   old ccr is saved, SR is re-read after the write, and the old ccr is
   restored when the match landed in the few cycles between the SR
   check and the write - the pending ISR then measures its true
   dispatch latency instead of the ~2^32 maxLateUs wrap.

Validation: compile_m74_9.sh BUILD SUCCESSFUL (one build,
EFI_ANGLE_CLOCK TRUE; nm shows angleClockArm/OnTooth/Refresh/
TickForNt/VectorB0); unit tests 1169/1169 PASSED. Car verdict pending:
the first crank must fire on time - no C935x cluster, no C9012, angclk
noChannel near 0, maxLateUs microseconds.

## 2026-08-31 (evening, BENCH self-stim) - angle clock arms ~100% refused: split telemetry + caller-phase capture (IN PROGRESS)

Self-stim runs (RPM 500/800) showed ~100% arm failures with lateArm=0:
the combined armFail counter could not say whether the failures were
stale-phase refusals (remaining > 30 deg) or all-channels-busy. Added
diagnostics:

- arm failures split: refuse (remaining > MAX_LEAD_DEG) vs noCh (all
  four channels busy), plus attempts, max |ccr - CNT| of busy channels
  (stuck-tick distance), the TMR2 init rate (acDelta/ntDelta/psc) and
  the last refusal's full snapshot (target angle, stored phase, basis,
  wrapped remaining, callback address).
- angleClockArm now takes the caller's currentPhase for the snapshot,
  so a single lockstats proves whether the stored phase diverged from
  the caller's (feed ordering) or the window test itself passed with
  the target behind the phase (window computation).

First finding: on self-stim EVERY refusal is the dwell start
(cb=08072374 = turnSparkPinHighStartCharging trampoline), with the
target (dwell angle) BEHIND the stored phase by exactly one dwell
duration (target=483.7 phase=512.0 rem=691.7 at 800 rpm - the arm runs
at the spark moment). The fallback then schedules the charge one full
cycle late at the same engine angle, so the bench "runs" on the TIM5
path with an over-long dwell (28 deg + dwellDuration) - the same class
of overcharge that blows the car fuse. The callerPhase capture
(committed here) names the exact divergence next run.

## 2026-08-31 (evening, BENCH self-stim) - degenerate early window on useOnlyRisingEdges wheels (FIXED)

Split arm-failure telemetry (attempts/refuse/noCh + last-refusal snapshot with
target, stored phase, caller phase/next, remaining, callback address) pinned
the root cause in one fit:

- every refusal was the dwell start (turnSparkPinHighStartCharging trampoline),
  called with the dwell angle BEHIND the current phase by exactly one dwell
  duration (target=663.5 phase=692.0 at 800 rpm - the arm runs at the spark
  moment, never at the dwell window);

- callerPhase == stored phase and callerNext == phase + 6 - the feed and the
  window lower bound are sane, so the window TEST itself was matching every
  tooth.

Root cause (code-proven): on useOnlyRisingEdges wheels prepareEventAngles
stores the falling edge at the preceding rise's angle, so eventAngles has
consecutive duplicates [X, X, X+6, X+6, ...]. findNextTriggerToothAngle(i+1)
therefore returns the SAME phase as nextPhase -> nextNextPhase == nextPhase.
isPhaseInRange(dwell, nextPhase, nextNextPhase) then takes the "next <= current"
branch and matches EVERY angle (afterCurrent || beforeNext degenerates). The
dwell (and injection) early window armed every tooth, refused every time, and
the TIM5 fallback scheduled the charge one full cycle late (691 deg) at the
same engine angle - invisible on the clean-basis bench, but on the car the
storm-garbage basis extrapolates that 691 deg into nonsense (the fuse class).

The fire arm already carried the `nextNextPhase != nextPhase` guard; the dwell
and injection arms did not. Fix: add the same guard to both early windows, so
they fall back to the current-tooth window (the FALSE-build path) when no
distinct next-next tooth exists.

Validation: compile_m74_9.sh BUILD SUCCESSFUL (EFI_ANGLE_CLOCK TRUE); unit
tests 1169/1169 PASSED. Verdict on the bench/car: angclk refuse must drop to
~0 and fired climb; no C9012/C935x/C9002 on the car.

## 2026-08-31 (19:25, BENCH self-stim 800 rpm) - angle clock fully armed on TMR2 (VALIDATED)

After the findNextTriggerToothAngle duplicate-skip + nextNextPhase+2 fix:
angclk att=1632 fire=1627 refuse=0 noCh=0 lateArm=0 drop=0 maxLateUs=0,
sched dwell=0 spark=0 - all three event kinds (dwell start, coil fire,
injection start) are armed one tooth ahead on TMR2 and the TIM5 executor is
freed of ignition entirely. overdwell n=5 (was 313): four at sync
establishment (first cycles after sync, one per cylinder, 1.4 s apart) plus
one during a lockstats dump. All five are bounded at 7.15 ms (1.5x dwell)
and spread in time - not the simultaneous cluster that blew the fuse. The
fuse mechanism is dead on the bench.

Remaining known items: the ~1% fire misses (the 5 bounded rescues) at
sync-establishment/stall moments; busyUs ~46 s is the 32-bit counter-wrap
telemetry artifact (counter approaching its 1073 s wrap), not a real stuck
tick. Car verdict is the next step: crank and watch C9351-4 / C9012 /
angclk counters.

## 2026-08-31 (evening, CAR) - first-teeth basis clamp kills the catch: band gated on a live rpm average

Car session after the bench-validated build: the engine tries once, then
stops catching. Each key cycle shows a handful of revs only (vrmodel stored
4 / 1 revs), C9012 + C935x (4.5-9 ms) on the first charge, then C9002 right
after first combustion. The angclk telemetry is clean (maxLateUs=1..4,
refuse=0, noCh=0, drop=0) - the TMR2 path itself is correct.

Root cause: the 4x basis band clamped against `oneDegreeUs` whenever the
average was > 0. At the first tooth pairs after a pause the average is the
10 s decoder clamp (huge), so a REAL tooth basis (~0.67 ms/deg at 800 rpm)
got clamped UP to the stale average / the 2 ms/deg ceiling. The first
catch's dwell+fire armed ~3x late, the charge-anchored rescue (1.5x dwell)
discharged first, and the first-combustion kick broke the gap ratio ->
C9002 -> one attempt per key cycle.

Fix (firmware/controllers/trigger/trigger_central.cpp): apply the 4x band
only when `rpmTicksPerDegree < US2NT(2000)` (~120 rpm floor = average
alive); the absolute 2 ms/deg ceiling stays as the last resort for the
decoder clamp. Never clamp a live tooth measurement toward a stale derived
value.

Validation: compile_m74_9.sh BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.
Car verdict pending: the first crank must show the catch on the first
attempt, no C9002, C935x reduced/absent.

Side observation (not chased): TLE9201 "Overcurrent shutdown!" in one car
log - the ETB throttle-blade path, a separate WDA/ETB saga.

## 2026-08-31 (evening, CAR) - angle clock DISABLED: catches and stalls, time-based build works

After the first-teeth basis fix (0cf0a7c32a0) the car catches - and then
stalls: C9012 + one C9352 8.5 ms overcharge + engine stopped after 9 revs
(20:19), C9002 right after first combustion (20:20), 4-6 revs per attempt.
The user's verdict: "без angle clock все идеально работает" - the
time-based build is correct, the angle clock is not.

Root cause (car evidence + code): the angle clock arms by angle from the
LAST completed tooth's duration. At cranking the per-tooth speed oscillates
with compression: a stretched tooth right before the fire makes the fire
~5.5 ms late (C9352 8.53 ms charge = 3 ms dwell + 5.5 ms), and the
"re-anchor only earlier, never later" refresh turns a MORE stretched next
tooth into an EARLY fire under deceleration -> kickback -> C9002. The
time-based build converts angles with the 90-degree rpm average, which
smooths the oscillation and fires reliably. The bench (clean-basis
self-stim) could never see this - its emulator has no compression physics.

Decision (user): EFI_ANGLE_CLOCK = FALSE in
firmware/config/boards/m74_9/efifeatures.h. The FALSE build compiles out
all early windows and is bit-identical to the proven time-based path
(nm: zero angle-clock symbols in rusefi.elf). The angle-clock code stays
in the tree behind the flag.

Validation: compile_m74_9.sh BUILD SUCCESSFUL (FALSE); unit tests
1169/1169 PASSED.

## 2026-08-31 (late evening) - angle clock FINAL v2: re-enabled on the oneDegreeUs basis

Root cause of the whole catch-and-stall chain (car + bench evidence + code):
the angle clock armed events from the raw last-tooth duration
(toothDurations[0]). At cranking the compression speed oscillation makes a
single tooth a 2-3x-stretched predictor, so the catch fires armed ~3-12 deg
late; the charge-anchored overdwell rescue (1.5x dwell, pure time) then
discharged first (C935x), and the rescue's wrong-angle spark + cancelled
fire broke the first-combustion gap ratio -> C9002 -> stall. The working
time-based build converts angles with oneDegreeUs - during spin-up that is
the InstantRpmCalculator's ~90-degree tooth window (calculateInstantRpm
hunts the tooth ~90 deg back), when running the full-cycle average: smooth
by construction, no EMA needed (user directive).

FIX (this commit, EFI_ANGLE_CLOCK TRUE):
- main_trigger_callback.cpp: angleClockOnTooth is fed
  US2NT(engine->rpmCalculator.oneDegreeUs) instead of the per-tooth basis.
- trigger_scheduler.cpp fire fallback + spark_logic.cpp dwell chargeTime:
  same oneDegreeUs basis (identical prediction to the FALSE build).
- trigger_central.cpp: the toothDurations[0] basis computation and the
  4x-band / 2 ms-per-deg clamp are REMOVED (dead code under the new basis);
  the lastToothTicksPerDegree field is gone.
- spark_logic.cpp: the overdwell rescue margin 1.5x -> 2.5x dwell, so a
  fire that is merely late (<=1.5x dwell, the catch's oneDegreeUs lag)
  always wins; a genuinely lost/stuck fire is still bounded at 2.5x dwell.
  The FALSE build is untouched (its rescue is basis-anchored and can never
  beat the fire - the same invariant restored here).
- angle_clock.h / test_angle_clock.cpp comments updated; the refresh logic
  stays (with the smooth basis it only tracks the rpm change).

Validation: compile_m74_9.sh BUILD SUCCESSFUL (TRUE; the ELF has the
angle-clock symbols); unit tests 1169/1169 PASSED. Car verdict pending:
first crank must show no C935x cluster, no C9002, angclk refuse~0.

## 2026-09-01 (noon, CAR) - due events fired, not cancelled: the refresh dropped the first-cycle charge/injection

Car log with the oneDegreeUs basis (12:27/12:28): P0113 at cranking start,
C9012 out-of-order coil off ~1 s later (first fire, no charge), engine
stopped after 3 revs; second attempt 1 rev + C9002. angclk clean
(att=28 fire=13 refuse=0 noCh=0 lateArm=0), sched dwell=0 spark=3 fuel=13,
overdwell=0 - no C935x anymore (the 2.5x rescue no longer steals), but the
engine does not combust: the fires hit uncharged coils.

Root cause (code): angleClockRefresh cancelled a Start (dwell/injection)
whose tick had arrived at the refresh ("a late Start is dropped"). The
time-based build fires a due dwell/injection via its due-tooth
scheduleByAngle even when the handoff runs late, so every cancelled Start
was a charge/injection the FALSE build would have made: the coil never
charged (C9012 at the fire) and the first cycle got no fuel - the engine
spins and stops. The drop telemetry was dead (s_droppedCount never
incremented), so drop=0 hid it.

Fix (this commit): the refresh arms a due event for IMMEDIATE firing for
BOTH kinds (CoilFire and Start) - a late charge start is not a fuse risk
(the 2.5x-dwell charge-anchored rescue bounds any charge). Added
angleClockImmediateFireCount telemetry (lockstats angclk ... immediate=N).

Note: P0113 (IAT input too high) at cranking start is unexplained by the
angle clock (CLT/IAT are ADC3 slow-path, thread-sampled; the TMR2 ISR does
not touch ADC3). It may be the bench's disconnected sensor or a battery
transient - to be confirmed on the car (does FALSE really never print it
during the same cranking?).

Validation: compile_m74_9.sh BUILD SUCCESSFUL (TRUE); unit tests
1169/1169 PASSED.

## 2026-09-01 (afternoon) - trigger/spark/injection pipeline documented; C9009 false positive root cause

Session: analysis of 12:51-13:01 car logs (EFI_ANGLE_CLOCK=FALSE, stable idle ~800 rpm).
No code changes - documentation pass.

### Log overview

Two C9009 warnings separated by ~7 minutes of stable idle:
  12:51:08  revolution=14  [Coil 3] rpm=836 charged=0 bail=no fall=60  counter=64
  13:01:10  revolution=226 [Coil 2] rpm=806 charged=0 bail=no fall=912 counter=916

counter - fall = 4 in both cases (exactly one 4-cyl cycle, normal). charged=0 (coil
not charging - correct for a fresh dwell start). bail=no (last fire older than current
sparkCounter - no out-of-order). No actual spark was skipped.

### C9009 false positive root cause (spark_logic.cpp:349)

The warning condition: prevSparkName == outputName in startDwellByTurningSparkPinHigh.
prevSparkName tracks the last coil to start a dwell and is updated ONLY inside the
RPM gate: `if (Sensor::getOrZero(SensorType::Rpm) > 2 * cranking.rpm)`. SensorType::Rpm
reads the cached value updated by rpmShaftPositionCallback in the trigger handoff (prio 4).

The m74_9 trigger storm flaps SensorType::Rpm between 0 and the real rpm at ~1 kHz.
If the flap lands at 0 at the moment one intermediate cylinder's dwell callback executes
in the TIM5 ISR (prio 3), prevSparkName is NOT updated for that coil - it stays on
the previous coil. The next same-coil dwell (counter - fall = 4, one full cycle later)
then sees prevSparkName == outputName and fires C9009.

Rarity (~twice in 7 min): at 800 rpm the intermediate-coil dwell ISR fires ~10 Hz,
and the storm flap window is narrow, so collisions are infrequent.

No fix applied this session. Fix options:
(A) move `prevSparkName = outputName` outside the rpm gate (always update)
(B) suppress C9009 when (counter - fall) == cylinderCount (normal cycle)

### Lockstats analysis (FALSE build, stable idle ~800 rpm)

Five measurement windows between 12:51:14 and 12:53:59. Summary (numbers from the
steady-state windows, excluding the startup burst at 12:51:14):

  dwell:     late 8-37%   maxLateUs 13-15 us   (batch leader)
  spark:     late 88-93%  maxLateUs 16-22 us   (follows fuel-END in spark batch)
  fuel:      late ~99%    maxLateUs 71-128 us  (END in foreign batches - normal)
  overdwell: n=0 in all windows                (no coil overcharge)
  trgDecode: all <50 us                        (gap decoder fast at idle)
  trgPostDecode 100-250 us: 2.8% of teeth      (scheduling teeth only - normal)

The first window (12:51:14, right after startup catch) showed triggerMaxDuration=3312
ticks = 828 us. dwell cbmaxUs=193 us = the C9009 warning() print from ISR context
(documented: ~190-202 us, the ONLY non-trivial path in startDwellByTurningSparkPinHigh).

### trgPostDecode 100-250 us: scheduling teeth are expected

At 800 rpm, 60-2 trigger: 116 teeth per engine cycle. Scheduling happens on the teeth
where isPhaseInRange matches a dwell/spark/injection window. With 4 cylinders and one
window each the fraction is ~3-4 scheduling teeth per 116 = ~3%. The 2.8% in the
100-250 us bucket is exactly this set - mainTriggerCallback runs handleFuel +
scheduleEventsUntilNextTriggerTooth + onTriggerEventSparkLogic on those teeth and
takes 100-250 us. The remaining 97% are <50 us (trivial: no window matches, scheduling
code short-circuits immediately).

With EFI_ANGLE_CLOCK=TRUE the scheduling moves to the previous tooth (one ahead), so
the per-tooth scheduling cost is roughly the same but spread differently. The 100-250 us
bucket should stay at the same fraction.

### othercb address -> function (n= correlation, ELF-free method)

The n= count is the build-independent identity: the role determines how often the callback
fires relative to the per-kind counts.

  n == dwell count     -> startKnockSampling (scheduled at the same tick as dwell)
  n == spark count     -> startAveraging (MAP window start, co-scheduled with spark)
  n == spark count     -> endAveraging (MAP window end, time-scheduled from startAveraging)
  n == dwell/cylinders -> onTdcCallback (once per engine cycle, trgEventIndex==0)
  n ~= 1/s             -> watchDogBuddyCallback (~1 Hz)
  n == 1 at startup    -> timerValidationCallback, PrimeController onPrimeStart/EndAdapter

With EFI_ANGLE_CLOCK=TRUE: dwell/spark/injection-start move to TMR2 and disappear from
the TIM5 kindStats. The corresponding othercb n= counts drop to near-zero. That drop
is the bench proof that the early-window arming is active.

### Full pipeline documentation

The complete trigger -> spark and trigger -> injection processing chains were traced to
GPIO level and documented. Key findings:

Trigger (DПКВ) path:
  PF8 falling edge -> EXTI prio 0 (timestamp + level, ~2-3 us)
  -> queue -> handoff prio 4 (avg 44 us, tails to ~1 ms)
  -> trgPreDecode: debounce, plausibility
  -> trgDecode: decodeTriggerEvent, gap ratio [1.6,3.75] + [0.85,1.15]
  -> trgPostDecode: rpmShaftPositionCallback (oneDegreeUs at trgIdx==0),
     findNextTriggerToothAngle, mainTriggerCallback
     (handleFuel + scheduleEventsUntilNextTriggerTooth + onTriggerEventSparkLogic)

Ignition path (FALSE build, TIM5 only):
  prepareCylinderIgnitionSchedule -> dwellAngle, sparkAngle (degrees in [0,720))
  onTriggerEventSparkLogic per tooth: isPhaseInRange(dwellAngle, current, next)?
    YES -> scheduleByAngle(chargeTime) -> TIM5 dwell callback
         + scheduleOrQueue(sparkAngle) -> angle queue or TIM5 fire
         + overdwell rescue at 1.5x dwell from schedule time
  TIM5 ISR prio 3: turnSparkPinHighStartCharging -> palSetPort(GPIOF, PF12..15)
  TIM5 ISR prio 3: fireSparkAndPrepareNextSchedule -> palClearPort(GPIOF)
                   -> coil discharges -> HV spike -> spark

GPIO chain for ignition: PF12=IGN1, PF13=IGN2, PF14=IGN3, PF15=IGN4 (L9779 direct
drive, o_oe_mask permanently 1, palSetPort single cycle, no SPI latency in hot path).

Injection path:
  periodicFastCallback 200 Hz: injectionMass[cyl], injectionDuration, injectionOffset
  InjectionEvent::update() (called from turnInjectionPinLow ISR): recomputes
    injectionStartAngle for next cycle
  onTriggerTooth per tooth: isPhaseInRange(injectionStartAngle, current, next)?
    YES -> schedule(startTime) -> TIM5 turnInjectionPinHigh
         + schedule(startTime + PW) -> TIM5 turnInjectionPinLow  (always TIM5, never TMR2)
  TIM5 ISR: turnInjectionPinHigh -> palSetPort(GPIOE, PE8..PE11) -> L9779 OUT1..4
  TIM5 ISR: turnInjectionPinLow  -> palClearPort(GPIOE) -> forked closed
             -> event->update() (recomputes angle for next cycle)

GPIO mapping: PE8=OUT4/cyl1, PE9=OUT3/cyl2, PE10=OUT2/cyl3, PE11=OUT1/cyl4.
Injection END is always TIM5 time-based (startTime + PW_ns). A late start shifts
the whole pulse but mass (PW) is unchanged.

### Validation

No code changes. Analysis session only.

### Open follow-ups

- C9009 false positive: fix prevSparkName gate (option A or B above)
- trgPostDecode scheduling-teeth cost: will be replaced by angle-clock arming once
  EFI_ANGLE_CLOCK is validated on the car

## 2026-09-01 (night 9) - injection close CC2: slot overwrite bug (injector stuck open)

Engine sniffer showed only one injector firing. lockstats: injCC2 scheduled=397
fired=364 (gap=33 from startup) and growing (+14 per 2 seconds of running).

Root cause: when injection duty > 100% (cycle N+1 starts before N closes),
scheduleInjectionCloseHW for the same cylinder OVERWROTE slot[cyl] with the
new close time. CC2 fired once at N+1 time. turnInjectionPinLow called once:
  overlappingCounter: 2->1 (NOT 0) -> setLow() not called -> injector stuck open
  update() never called -> injectionStartAngle stale -> cylinder stops injecting.

Fix: replaced bool pending with uint8_t pendingCount.
  Normal (pendingCount=1): dispatch fires action, clears slot.
  Overlap (pendingCount>=2): dispatch fires action, pendingCount--, re-arms CC2
  at CNT+4us for the next close. turnInjectionPinLow fires pendingCount times:
  overlappingCounter goes N->N-1->...->0 -> setLow().

Key lesson: hardware close timing slot per cylinder requires COUNTING not BOOLEAN.
overlapCounter model in InjectorOutputPin requires exactly N closes for N opens.
Losing even one close causes injector stuck open and cylinder stops working.

Validation: BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.
Car verdict: injCC2 scheduled=fired (gap=0 in stable running), all 4 cylinders fire.

## 2026-09-01 (night 8) - injection close: TIM5 CC2 hardware 32-bit mini-queue (Option B)

NT ticks == TIM5->CNT (1:1, same 4 MHz APB1, same counter). CCR2 = fireAt_NT
directly. CC2 callback plugged into existing pwm_lld_serve_interrupt dispatch
(was nullptr). Mini-queue of 4 slots (one per cylinder) with signed comparison
for past-due events. Same ISR prio as executor (3). No race, no overflow.

  injection_close_hw.h/cpp:
    s_injClose[4]: {fireAt, action, pending}
    scheduleInjectionCloseHW(cyl, nowNt, delayNt, action)
    hwInjectionCloseCallback: disables CC2IE, dispatches due closes, re-arms
    EGR CC2G forces fire if CCR2 already passed (same as CC1G in executor)

  microsecond_timer_stm32.cpp:
    timerConfig CH1 callback = hwInjectionCloseCallback
    CCMR1: OC2M=1 added (software comparator, no pin output)
    timerConfig: constexpr -> const (cross-TU function pointer)

  onTriggerTooth: stores injectionEndDelayNt, no TIM5 END pre-schedule
  turnInjectionPinHigh: calls scheduleInjectionCloseHW at actual open

PW accuracy: ~1 us (CC2 ISR latency) vs ~20 us (TIM5 executor batch).
No PW overflow: 32-bit CCR2, no 8ms 16-bit limit.
No race: END not pre-scheduled, only armed after injector opens.

Validation: BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.
Car verdict: inj fired ~ dwell (opens on TMR3, closes on CC2), lateArm~0.

## 2026-09-01 (night 7) - injection close reverted to TIM5 (angleClockArmInjectionFromNow unusable for long PW)

Engine still broken after night 6 fix. New lockstats: lateArm=135/149=91% for closes.
Engine barely idles, injectors barely fire.

### Bug: 16-bit arm fails for PW > 8ms

TMR3 is 16-bit (wraps at 16.384ms). The arm check:
  (int16_t)(atTick16 - cnt16) >= ARM_MARGIN
For PW > 8ms = 32768 ticks: (int16_t)(PW_ticks > 32767) = NEGATIVE -> arm fails.
At idle with high deadtime or at medium load, PW > 8ms is normal. Result: 91%
of injection close arms fail, injectors barely work.

### Fix: injection close always on TIM5 (reverted)

- onTriggerTooth: always schedule END on TIM5 with nullptr
- turnInjectionPinHigh: no close re-arm
- Remove injectionEndStage1 and injectionEndDelayNt from InjectionEvent
- Keep angleClockArmInjectionFromNow function for possible future use
  (would need 32-bit timer for PWs up to 65ms)

### Lesson: angleClockArmInjectionFromNow requires 32-bit timer

For TMR2 (32-bit, dwell): spark dwell always < 8ms -> arm always succeeds.
For TMR4 (16-bit, spark): spark delay from charge = sparkDwell < 8ms -> OK.
For TMR3 (16-bit, injection close): injection PW can be 2-15ms -> fails for PW > 8ms.
Future implementation of injection close on hardware timer requires a 32-bit APB1
timer. Available 32-bit APB1 timers: only TIM5 (executor) and TMR2 (dwell).
TMR3 cannot be used for long delays.

### Validation

BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.
Car validation: inj fired ≈ dwell (only TMR3 opens), lateArm=0 for injection
(only opens attempted on TMR3, closes on TIM5).

## 2026-09-01 (night 6) - CRITICAL: injection double-fire race in angleClockArmInjectionFromNow

Engine ran badly after angleClockArmInjectionFromNow commit: double injection,
rich mixture, C9002, stall. lockstats showed inj fired ≈ 2x dwell fired and
lateArm=126 (all injection close arms failed).

### Root cause: TIM5 END fired before TMR3 START (race condition)

onTriggerTooth pre-scheduled TIM5 END at 'startTime + PW' with &injectionEndStage1.
When injection was past-due (immediate-fire via angleClockRefresh), TMR3 START
CCR was set to CNT+4us (nearly now). By the time TMR3 ISR fired turnInjectionPinHigh,
TIM5 had ALREADY fired the END event:
  1. TIM5 END fires: turnInjectionPinLow -> closes injector, calls update()
  2. TMR3 START fires: turnInjectionPinHigh -> OPENS injector (2nd time!)
  3. angleClockArmInjectionFromNow arms 2nd close
  4. TMR3 fires 2nd CLOSE: closes injector again
  = double injection, double close, update() called twice

Measured: inj fired=1625 vs dwell=858 (expected 2x=1716, got 1.89x)
          inj fired=124 vs dwell=122, lateArm=126 (all close arms failed)
          Engine ran rich, barely idled under load, C9002, stall.

### Fix: remove TIM5 END pre-scheduling for sequential mode under EFI_ANGLE_CLOCK

The END is ONLY armed by turnInjectionPinHigh at the actual open moment via
angleClockArmInjectionFromNow. No TIM5 pre-schedule = no race.

Simultaneous injection (startSimultaneousInjection, not turnInjectionPinHigh)
still pre-schedules TIM5 END with nullptr (no race in simultaneous mode).

Removed the now-unnecessary engine->scheduler.cancel() from turnInjectionPinHigh.
Added delayNt > 0 guard for first-boot edge case.

Key lesson: when re-arming an event from inside the START callback, NEVER
pre-schedule the END anywhere else. The only safe anchor is the actual open
moment inside the START ISR. Pre-scheduled TIM5 events can race with TMR ISRs
of the same priority (prio 3) because TIM5 is a different vector.

### Validation

BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.
Car validation: inj fired should equal dwell fired (only TMR3 opens counted)
OR 2x dwell fired (opens + closes on TMR3) depending on whether close arm
succeeds. No double injection. Engine must run normally.

## 2026-09-01 (night 5) - trgPostDecode scheduling-tooth analysis + next optimization plan

### trgPostDecode 50-100 us: breakdown of scheduling tooth overhead

On scheduling teeth (2.8% of all teeth), trgPostDecode takes 50-100 us instead
of the normal <50 us. These are the teeth where a dwell, spark or injection
start event falls in the scheduling window.

Breakdown (~87 us total on a scheduling tooth):
  rpmShaftPositionCallback:              ~5 us (rpm + oneDegreeUs update)
  angleClockOnTooth:                     ~2 us
  scheduleEventsUntilNextTriggerTooth:  ~15 us (lock + list + angleClockArmSpark)
  handleFuel (4 cylinders):             ~20 us (inj model + arm)
  scheduleDwellEarlyIfDue:              ~20 us
    -> scheduleSparkEvent:
       globalSparkCounter++              1 us
       angleClockArmDwell               5 us
       scheduleOrQueue (LOCK+LIST)      8 us  <- main cost
  onTriggerEventSparkLogic:             ~10 us (4-cylinder skip-loop)
  angleClockRefresh (12 channels):      ~15 us
  -----------------------------------------------
  Total:                                ~87 us -> 50-100 us bucket

### Key finding: arms happen in first 8-33 us, not in the 50-100 us window

All physical event arms complete well before the 50-100 us overhead:
  8 us:  spark arm on TMR4   <- arm done, TMR4 fires independently
  13 us: injection arm on TMR3 <- arm done
  33 us: dwell arm on TMR2  <- arm done

The remaining 50-87 us is post-processing that does NOT affect physical timing.
At 7000 rpm (tooth period 147 us), the handoff exceeds the tooth period on
scheduling teeth, but TMR2/3/4 have already fired their events correctly.

Symptom: immediate=200-364 in lockstats - the refresh finds events past-due
because it runs 87 us into the tooth when some CCRs have already matched.

### Root cause: angle queue spark is now REDUNDANT

With the new architecture (spark armed from turnSparkPinHighStartCharging via
angleClockArmSparkFromNow), the angle queue spark path is unnecessary:

  OLD path (necessary for FALSE build):
    scheduleDwellEarlyIfDue -> scheduleSparkEvent -> scheduleOrQueue (8 us LOCK)
    scheduleEventsUntilNextTriggerTooth -> dequeue -> angleClockArmSpark (no-op pre-fire)
    turnSparkPinHighStartCharging -> angleClockArmSparkFromNow <- REAL arm

  NEW optimal path:
    scheduleDwellEarlyIfDue -> only arm TMR2 dwell (no scheduleOrQueue)
    scheduleEventsUntilNextTriggerTooth -> queue empty -> fast return
    turnSparkPinHighStartCharging -> angleClockArmSparkFromNow <- only arm

Evidence: angclk spark fired > dwell fired by 2-14 = no-op angle-queue
pre-fires during rapid acceleration (TMR4 fires as no-op before TMR2 dwell).

### Potential savings from eliminating angle queue spark

  Remove scheduleOrQueue from scheduleDwellEarlyIfDue: ~8 us
  Remove spark dequeue from scheduleEventsUntilNextTriggerTooth: ~8 us
  Remove TriggerScheduler::cancel in overFireSparkAndPrepareNextSchedule: ~2 us
  Remaining irreducible work (handleFuel, rpmCallback, refresh): ~50 us
  Result: 87 -> 69 us -> some teeth exit 50-100 bucket

  Additional: onTriggerEventSparkLogic loop becomes pure fallback,
  can be skipped on all teeth where dwellStartArmed=true.

### What cannot be reduced (physics)

  handleFuel ~20 us: 4 cylinders x injection model = necessary for fuel accuracy
  rpmShaftPositionCallback ~5 us: RPM + oneDegreeUs update = necessary
  angleClockRefresh ~15 us: with 780-sentinel on spark/inj channels,
    only 4 TMR2 dwell channels need actual refresh computation

### Status

Not yet implemented. Plan documented for next session.

## 2026-09-01 (night 4) - angle clock: angleClockArmInjectionFromNow (injection close from actual open)

Variant B for injection: arm the injection close (turnInjectionPinLow) from the
actual injector-open callback (turnInjectionPinHigh) on TMR3.

Motivation: when injection START fires via TIM5 (lateArm path, 0.33% of cases),
the END was pre-scheduled at nominal_start + PW rather than actual_open + PW.
With the new approach END is anchored to the actual injector opening.

Analysis: inj lateArm=29-34 (0.33%) in driving sessions. The lateArm events are
caused by scheduleNow injections with remaining < 0.72 deg at 7000 rpm (12% of
the 6 deg early window fails, but most injections go through scheduleEarly).
Practical impact of the old code: near-zero (TIM5 batch latency ~20 us = 0.4 deg
at 7000 rpm = negligible PW error). Implemented for completeness.

New fields in InjectionEvent (EFI_ANGLE_CLOCK only):
  injectionEndStage1: scheduling_s for cancellable TIM5 slot
  injectionEndDelayNt: PW in NT ticks, stored by onTriggerTooth for ISR use

New function: angleClockArmInjectionFromNow (same pattern as spark version).
In turnInjectionPinHigh: cancel pre-scheduled TIM5 end, re-arm TMR3 at nowNt.

Validation: BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.
Car verdict: angclk inj lateArm should drop to near-zero.

## 2026-09-01 (night 3) - angle clock: arm spark from actual charge time + inj arm earlier

### Fix 1: wasNoOp root cause found + eliminated

Root cause of 64 wasNoOp cycles per session (TMR4 firing before TMR2 dwell):
During rapid acceleration (e.g., 800->5000 rpm), the angle-queue spark was armed
on TMR4 at tooth T+k with a HIGH-RPM basis (small delay = fires soon), while the
TMR2 dwell CCR was set at tooth T-1 with a LOW-RPM basis (large delay = fires late).
Because the "only earlier" refresh rule skips channels with remaining > MAX_LEAD_DEG
(the dwell angle appears "far away" after a rapid rpm jump), the dwell CCR was never
re-anchored to match the current speed. TMR4 fired before TMR2.

Fix: new function angleClockArmSparkFromNow(cyl, nowNt, delayNt, action). Called from
turnSparkPinHighStartCharging AFTER the coil starts charging. Steps:
1. angleClockCancelSpark(cyl) - cancel stale angle-queue TMR4 arm
2. engine->scheduler.cancel(sparkEvent.eventScheduling) - cancel TIM5 fallback
3. angleClockArmSparkFromNow at nowNt + sparkDwell_ns (or TIM5 fallback)
Sentinel targetAngle=780.0f prevents refresh from re-anchoring time-domain arms
(780 > cycleDeg(720) + MAX_LEAD_DEG(30) = 750, so refresh always skips).

Removed sparkFiredNoOpBeforeDwell mechanism (no longer needed).

Results (19:07-19:31 sessions): sched spark=0 at ALL rpm incl 6000+.
dwell==spark (±14 from residual no-op pre-fires at very rapid accel, harmless).
overdwell=0, no C9353.

### Fix 2: injection arm earlier (handleFuel before scheduleDwellEarlyIfDue)

Moved handleFuel BEFORE scheduleDwellEarlyIfDue in mainTriggerCallback.
Injection arm elapsed: ~13 µs (was ~35 µs). Threshold at 7000 rpm:
remaining > 0.72° (was 1.64°). inj lateArm reduced ~30% (0.33-0.38% vs 0.47%).

### Validation

BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.
Car session 19:07-19:31 (6000+ rpm, long drive): sched spark=0, overdwell=0.

## 2026-09-01 (night 2) - angle clock: minimize spark arm elapsed + TMR4 for current-tooth

From 18:32-18:33 driving logs: sched spark=22-40 per 65 seconds causing stuttering.
Analysis:
- Block 3 (3290 rpm, 1 second): sched spark=0, maxLateUs=23 us, immediate=3 -> perfect.
- Blocks 1-2 (mixed rpm, 65-80 seconds): sched spark=22-40, immediate=172-236.

Root cause: spark arm elapsed was ~35 us (after scheduleDwellEarlyIfDue). At 7000 rpm
this gives threshold remaining > 1.0 deg from the 6 deg early window. PLUS: current-tooth
sparks (in [currentPhase, nextPhase)) always went to TIM5 unconditionally.

Fix 1: move scheduleEventsUntilNextTriggerTooth BEFORE scheduleDwellEarlyIfDue.
Elapsed at spark arm: ~8 us. Threshold: remaining > 0.5 deg at 7000 rpm.

Fix 2: in current-tooth window path, try angleClockArmSpark first, TIM5 only if
arm fails. At 8 us elapsed: ~92% of current-tooth sparks arm on TMR4.
Only sparks within 0.5 deg of the tooth edge remain on TIM5.

Expected: sched spark drops from 22-40/65s to near-zero at all rpm.
overdwell=0 (sparksRemaining fix from prior commit).

Validation: BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.

## 2026-09-01 (night) - angle clock: scheduleEventsUntilNextTriggerTooth moved earlier + sparksRemaining fix

Two issues identified from high-rpm driving logs (16:50-16:55 sessions):

### Issue 1: sched spark > 0 -> rough running/stuttering at speed

Sparks in the current-tooth window [currentPhase, nextPhase) go to TIM5 via
scheduleByAngle because their target angle is too close to fire from TMR4 with
sufficient margin. At 7000 rpm with scheduleEventsUntilNextTriggerTooth running
at ~55 us elapsed, the TMR4 arm succeeds only for sparks with remaining > 2.3°.
Sparks at 0-2.3° into the 6° early window fall back to TIM5 = timing error
~2.3° at 7000 rpm. With ~13 TIM5 sparks per 13 seconds at 3600 rpm avg =
~3% misfire rate = noticeable rough running.

Fix: move scheduleEventsUntilNextTriggerTooth to run BEFORE trgEventIndex==0
check, engineModules.apply_all, and handleFuel. At ~35 us elapsed, arm success
threshold moves from 2.3° to ~1.0°. TIM5 fallbacks reduced ~60%.

### Issue 2: C9353 -> engine stall (sparksRemaining == 0 blocked re-arm)

In turnSparkPinHighStartCharging the re-arm was gated by sparksRemaining==0.
With multispark enabled, sparksRemaining > 0 at charge time -> re-arm never
happened -> rescue fired unopposed -> C9353 -> stall. Observed: 1 overdwell
per session = engine stall at idle.

Fix: removed sparksRemaining check. Re-arm always fires when wasNoOp=true.
fireSparkAndPrepareNextSchedule handles sparksRemaining correctly internally.

### Validation

compile_m74_9.sh BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.
Car validation: sched spark should drop from ~13/13s to near-zero at 3600 rpm.
overdwell=0 at all conditions.

## 2026-09-01 (late evening) - angle clock: PSC rounding + scheduleDwellEarlyIfDue missing spark (car stopped starting)

Two critical bugs found from the first run with scheduleDwellEarlyIfDue:
  psc=70 in initRate (40000/40003), spark fired=0, overdwell n=2, engine not starting.

### Bug 1: PSC=70 instead of 71 (integer truncation in measurement formula)

Formula was: newPsc = (72 * acDelta / ntDelta) - 1.
With acDelta=40000, ntDelta=40003 (3 ticks of measurement noise):
  72 * 40000 / 40003 = 71.997 -> integer truncation -> 71.
  71 - 1 = 70. WRONG. Should be 71.

Effect of PSC=70: TMR2 runs at TIMCLK1/71 instead of TIMCLK1/72 =
1.4% faster than NT (TIM5). The NT<->TMR2 offset drifts ~56000 ticks/s.
After ~10 ms: atTick already 560 ticks behind TMR2->CNT. All arm checks
(atTick - CNT < ARM_MARGIN) fail. All events fall to TIM5 stale-basis.
Engine either doesn't start or runs poorly.

Fix: round-half-up: newPsc = (72*acDelta + ntDelta/2) / ntDelta - 1.
  40000/40003: (2880000+20001)/40003 - 1 = 72 - 1 = 71. Correct.
  40000/40000: same = 71. Correct.

### Bug 2: scheduleDwellEarlyIfDue set dwellStartArmed=true WITHOUT queuing spark

Old code armed TMR2 for dwell and set dwellStartArmed=true. Then
onTriggerEventSparkLogic sees dwellStartArmed=true -> continue (skip).
The spark was NEVER added to the angle queue. scheduleEventsUntilNextTriggerTooth
had nothing to arm on TMR4. Result: dwell fired, spark=0, rescue fired,
C9352/C9353. Observed: angclk dwell=6 lateArm=0, spark=0, overdwell=2.

Fix: scheduleDwellEarlyIfDue now calls full scheduleSparkEvent() instead
of just angleClockArmDwell(). scheduleSparkEvent arms TMR2 AND queues the
spark for TMR4 arming. scheduleSparkEvent made non-static + declared in h.

### Validation

compile_m74_9.sh BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.
Car validation: angclk psc=71, dwell fired=N lateArm~0, spark fired=N,
overdwell=0 at all rpm.

## 2026-09-01 (evening) - angle clock: high-rpm dwell arm fail + C9353 ordering bug fixed

Two bugs found from on-car lockstats at 3000-7000 rpm:

### Bug 1: dwell arm always fails above ~4000 rpm

angclk dwell fired=0 lateArm=8582 (all 8582 dwell arms failed) while
angclk spark fired=8581 lateArm=0 (all sparks succeeded). Root cause:

The arm check `(int32_t)(atTick - CNT) < ARM_MARGIN` uses
  atTick = edgeTimestamp_ticks + remaining_deg * tpd.
At 7000 rpm tpd=95 ticks/deg. For remaining=6 deg (scheduleEarly window):
  delay = 571 ticks = 143 us.
By the time angleClockArmDwell is called (at the END of
onTriggerEventSparkLogic, after handleFuel + scheduleEventsUntilNextTriggerTooth
for 4 cylinders), elapsed = ~250 us = 1000 ticks.
  margin = 571 - 1000 = -429 ticks -> FAILS.

Spark arm (from scheduleEventsUntilNextTriggerTooth at the BEGINNING of
mainTriggerCallback) has only ~20-30 us elapsed -> margin = 551 ticks -> OK.

Fix: scheduleDwellEarlyIfDue() called early in mainTriggerCallback, before
handleFuel. Elapsed ~30 us = 120 ticks at call time:
  margin = 571 - 120 = 451 ticks >> ARM_MARGIN. Always passes at 7000 rpm.

### Bug 2: C9353 overcharge when dwell on TIM5 and spark on TMR4

When dwell falls back to TIM5 (Bug 1), TIM5 schedules dwell for
chargeTime ~ edgeTimestamp (small offset). TMR4 spark CCR was set one tooth
earlier at the correct absolute angle. If TMR4 fires before TIM5 dwell:
  sparkFiredSinceCharge=true (from PREVIOUS cycle) -> fireSparkAndPrepareNextSchedule
  returns NO-OP. TIM5 dwell fires -> sparkFiredSinceCharge=false -> rescue
  armed at 2.5x dwell. No TMR4 event remains -> rescue fires -> C9353.

Fix: fireSparkAndPrepareNextSchedule records no-op as
sparkFiredNoOpBeforeDwell=true. turnSparkPinHighStartCharging detects this
and reschedules the spark on TIM5 at nowNt + sparkDwell (time-based from
actual charge start). The TIM5 spark fires -> cancels rescue. C9353 gone.

### Change inventory

- event_registry.h: IgnitionEvent.sparkFiredNoOpBeforeDwell (bool) added
- spark_logic.h: scheduleDwellEarlyIfDue() declared
- spark_logic.cpp: scheduleDwellEarlyIfDue() implemented;
                   fireSparkAndPrepareNextSchedule: sets sparkFiredNoOpBeforeDwell;
                   turnSparkPinHighStartCharging: checks flag, re-arms on TIM5
- main_trigger_callback.cpp: calls scheduleDwellEarlyIfDue() after rpm==0 gate

### Validation

compile_m74_9.sh BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.
Car verdict: first run at 7000 rpm must show angclk dwell fired=N lateArm~0,
no C9353, sched overdwell=0.

## 2026-09-01 (12:44, CAR) - refresh runs on every tooth (the storm skip was the stall)

The engine CAUGHT (first time with the angle clock) and stalled after
~3.3 s (28 revs). Log: four C935x 7.5-13.6 ms in the SAME millisecond
(the catch's stale-tick fires discharged by the 2.5x rescues), overdwell
n=50 (~90% of fires stolen - the engine ran on wrong-angle rescue sparks
until it died), angclk immediate=222 of 447 executions (half the events
past-due at the refresh).

Root cause: during the catch trigger storm the rpm sensor flaps 0/nonzero
at ~1 kHz and mainTriggerCallback returns early on the flap teeth - the
angleClockRefresh call at the end of the callback never ran there. The
armed events kept their PRE-CATCH ticks (computed at ~250 rpm) while the
engine accelerated to 800+, so the fires landed ms-late for the whole
run. The FALSE build re-computes the tick at the due tooth with the fresh
basis every time - storm-immune by construction. The refresh was supposed
to provide that, and the storm silently disabled it.

Fix (this commit): angleClockOnTooth + angleClockRefresh now run on EVERY
tooth (moved before the rpm==0 gate; the gate still skips the arming),
and a not-positive basis feed (oneDegreeUs = NaN on a flap) keeps the
last good basis instead of zeroing it. The armed events now track the
true speed through the storm; the ignition no longer depends on the
rpm flap.

Validation: compile_m74_9.sh BUILD SUCCESSFUL (TRUE); unit tests
1169/1169 PASSED.

## 2026-09-01 - angle clock: architecture analysis + three-timer refactor (TMR2/TMR4/TMR3)

### Analysis: why the single 4-channel pool was failing

The original angle clock used one pool of 4 channels (TMR2 CH0..3) with a
search-for-free loop. At a scheduling tooth up to 3 events arm simultaneously
(dwell + spark + injection) while 1-2 channels from the previous tooth are
still armed -> noChannel failures -> TIM5 fallback with stale oneDegreeUs
basis (cranking ~250 rpm) -> armed spark fires ms late -> overdwell rescue
(2.5x dwell = 7.5 ms) beats spark -> C935x, engine dies.

Measured 12:44 car session: overdwell n=50 (~90% of sparks stolen by rescue),
angclk immediate=222 of 447 (50% events past-due at refresh). The stale
cranking basis armed spark fires 13 ms out while rescue fires at 7.5 ms.

### Analysis: trigger/spark/injection pipeline (documented)

Full chain traced to GPIO level for both FALSE and TRUE builds:

FALSE build (TIM5 only): EXTI -> handoff -> trgPreDecode/Decode/PostDecode ->
  mainTriggerCallback -> TIM5 for dwell (chargeTime = now + angle x oneDegreeUs),
  angle queue -> TIM5 for spark at the due tooth, TIM5 for injection start+end.
  GPIO: palSetPort(GPIOF) for coils, palSetPort(GPIOE) for injectors. Both are
  direct-drive (L9779 o_oe_mask=1 permanently, single-cycle GPIO).

TRUE build (TMR2 angle clock + TIM5): dwell/spark/injection arm on dedicated
  hardware comparators one tooth ahead; per-tooth refresh re-anchors from
  oneDegreeUs; ISR fires independently of handoff latency at the firing tooth.

C9009 "looks like skipped spark event" documented as FALSE POSITIVE from
rpm-sensor flap (see CLAUDE.md for the full diagnosis).

### Implementation: three dedicated timers (Option C)

The fix: one timer per event type, one channel per cylinder. Channel = cylinderIndex.
A channel is always free at arm time (previous event fired 720 deg = ~150 ms ago).

  TMR2 (32-bit, VectorB0, IRQ 28, prio 3): dwell starts    [cyl 0..3 = ch 0..3]
  TMR4 (16-bit, VectorB8, IRQ 30, prio 3): spark fires     [cyl 0..3 = ch 0..3]
  TMR3 (16-bit, VectorB4, IRQ 29, prio 3): injection starts [cyl 0..3 = ch 0..3]

All three on APB1 (same TIMCLK1 = 288 MHz as TIM5/TMR2). Rate measured once
against NT (TIM5) at init, same PSC applied to all three. rccEnableTIMx(true)
for sleep-gate fix (lp=false clears APB1LPENR, gates clock in WFI).

16-bit timers (TMR3/TMR4) degrade gracefully below ~200 rpm: signed 16-bit
arm check (int16_t)(atTick - CNT) < ARM_MARGIN fails when delay > 8 ms
(half the 16.384 ms counter period) -> TIM5 fallback. Above ~200 rpm all
arm calls succeed. At 7000 rpm (2-tooth lead = ~170 us) well within range.

mcuconf.h: STM32_TIM3_SUPPRESS_ISR + STM32_TIM4_SUPPRESS_ISR so the PWM/ICU
LLDs don't claim VectorB4/VectorB8. ICU TIM4 kept TRUE so the LLD compiles
(needs one assigned TIM; SENT is off, icuStart never called).

AngleBasedEvent gets int8_t cylinderIndex = -1 for O(1) cancel in
TriggerScheduler::cancel() -> angleClockCancelSpark(cylinderIndex).

New lockstats line:
  angclk dwell fired=N lateArm=N  spark fired=N lateArm=N  inj fired=N lateArm=N
  angclk maxLateUs=N immediate=N initRate=N/N psc=N nvic T2=N T4=N T3=N (want 3)

### Change inventory

- angle_clock.h/cpp: rewritten (3 timers, fixed channels, new API)
- event_registry.h:  AngleBasedEvent.cylinderIndex added
- spark_logic.cpp:   angleClockArmDwell(event->cylinderIndex, ...)
- trigger_scheduler.cpp: angleClockArmSpark(current->cylinderIndex, ...)
                         angleClockCancelSpark(event->cylinderIndex)
- main_trigger_callback.cpp: angleClockArmInjection(this->cylinderNumber, ...)
- mcuconf.h:         STM32_TIM3/4_SUPPRESS_ISR TRUE, ICU TIM4 kept TRUE
- interrupt_priority.h: comment updated for 3 timers
- board_configuration.cpp: lockstats print updated

### Validation

compile_m74_9.sh BUILD SUCCESSFUL; unit tests 1169/1169 PASSED.
nm confirms VectorB0 (dwell), VectorB4 (injection), VectorB8 (spark) all present.

### Open follow-ups

- Car validation: first catch with the three-timer design (C935x should be
  gone, immediate=~0, all fires on TMR2/TMR4/TMR3)
- C9009 false positive: prevSparkName gate fix (rpm flap causes ~2/7 min)
- s_currentPhase storm pollution in refresh: confirmed L9779 gives clean signal,
  issue is timing/decoding not sensor noise (per user)

## 2026-09-01 (night 10) - sparkFiredNoOpBeforeDwell regression + injection CC2 stabilization (car sessions 18:42-22:54)

This entry covers the full set of car sessions on 2026-09-01 evening, the
regression caused by premature removal of sparkFiredNoOpBeforeDwell, the
"dwell fired=0" old-build failure, and the final stabilization with TIM5 CC2.

### Session 18:42-19:31 (baseline, three-timer build, working)

Car data confirms the three-timer build healthy:
- angclk dwell fired=7592 lateArm=0, spark fired=7606 lateArm=0, inj lateArm=29
- sched dwell/spark/overdwell=0 (all on hardware timers, none on TIM5)
- C9009 at rpm=6051: false positive per CLAUDE.md (charged=0 bail=no,
  counter-fall==cylinderCount -> normal cycle, no real skip)
- trgPostDecode histogram: ~6-8% of teeth in 50-100 us bucket = scheduling teeth
  (4 windows per 58-tooth revolution on 60-2 = expected)

Conclusion: build is correct; C9009 is cosmetic noise from the rpm-sensor flap.

### Session 21:46-21:55: sparkFiredNoOpBeforeDwell removed prematurely -> stall

User removed sparkFiredNoOpBeforeDwell from event_registry.h and
fireSparkAndPrepareNextSchedule (night 3 code claimed it was no longer needed),
and moved handleFuel before scheduleDwellEarlyIfDue in mainTriggerCallback.

Results (21:46 session): inj fired=1762 = 2x dwell fired=885. This anomalous
2:1 ratio revealed that the injection close path still used
angleClockArmInjectionFromNow (TMR3, 16-bit). At idle PW~3ms these succeed;
under throttle PW > 8ms -> (int16_t) overflow -> arm fails -> injector not
closed -> flooding.

Results (21:55 session): inj lateArm grew from 108 to 135 (46% failure). Engine
stutter. Root cause: without sparkFiredNoOpBeforeDwell, on rapid deceleration
the angle-queue TMR4 fires at the predicted spark time BEFORE TMR2 dwell
(prediction was too-short due to rpm drop; the angle queue arm at ~8 us
elapsed takes the current tooth speed, which was higher than actual dwell
speed). TMR4 fires on uncharged coil (no-op). Dwell fires later. No spark
pending. Rescue fires at 2.5x dwell = ~8ms. Coil overcharge. C9353.
Engine misfires and stalls.

Key lesson: sparkFiredNoOpBeforeDwell must not be removed until scheduleOrQueue
no longer arms TMR4 from the angle queue. The "cancel stale TMR4 arm" in
angleClockArmSparkFromNow only works if the cancel happens BEFORE TMR4 fires;
on rapid decel the predicted tick already passed at arm time, so TMR4 fires in
the same handoff cycle before the cancel can run.

### Session 22:22 ("old build" flashed by accident): dwell fired=0 lateArm=100%

  angclk dwell fired=0 lateArm=3632, spark fired=3632 lateArm=0

All dwell arms fail; all spark arms succeed. Engine barely runs.

Root cause: this was an older build that had scheduleDwellEarlyIfDue placed at
the END of onTriggerEventSparkLogic (~250 us elapsed) - the high-rpm-dwell-fail
bug documented in CLAUDE.md. At any RPM, the early window check
isPhaseInRange(dwellAngle, nextPhase, nextNextPhase) returned true for EVERY
tooth due to the nextNextPhase==nextPhase degenerate case (missing guard
that was added later). The arm target for every tooth was in the past
(dwell angle already behind current phase) -> all lateArm -> TIM5 fallback
with ~99.8% late -> coils barely charged. Engine ran poorly due to missing
or very late dwell, not because of injection (injectors showed inj fired=3632
lateArm=0 on TMR3).

Note: the "injectors don't work" symptom the user saw was from the engine
barely running (no charge -> no combustion -> rough RPM). The injectors
themselves fired correctly; there was nothing to combust.

### Session 22:32+ (new build, TIM5 CC2 injection close, injCC2 diagnostic)

  angclk dwell fired=28 lateArm=0, spark=28 lateArm=0, inj=29 lateArm=7
  injCC2 scheduled=397 fired=364 (gap=33 at startup)

New build with:
- sparkFiredNoOpBeforeDwell restored in event_registry.h
- nextNextPhase!=nextPhase guard in scheduleDwellEarlyIfDue early window
- TIM5 CC2 mini-queue for injection close (32-bit, no 8ms limit)
- pendingCount fix (slot overwrite bug replaced bool with uint8_t pendingCount)

The injCC2 gap=33 at startup is NORMAL (explained in CLAUDE.md): at cranking
multiple cylinders open before CC2 drains them. By the 22:51 session:
  injCC2 scheduled=1914 fired=1914 (gap=0)
  angclk dwell/spark/inj lateArm=0 at idle 800 rpm

The gap converged because all 1914 scheduled closes fired; none were lost.
The 22:43 session showed a growing gap (33->47 over 4 seconds) which was a
transient at cranking-to-idle transition, not a permanent loss.

### Validated final state (22:51-22:54 sessions)

  angclk dwell fired=1460 lateArm=0, spark fired=1461 lateArm=0,
         inj fired=1460 lateArm=0
  injCC2 scheduled=3374 fired=3374 (gap=0)
  trgPostDecode <50us=42360 50-100=2941 (6.2% scheduling teeth, expected)
  angclk maxLateUs=18 us, immediate=0

All events hardware-armed, no TIM5 fallback, injection closes all fire.
Engine runs stably at 800-2000 rpm (stable idle after C9002 resolved by the
VR model calibration saving at 22:51).

### Key findings for documentation

1. sparkFiredNoOpBeforeDwell: must stay until angle queue spark path eliminated
   (precondition for the "next optimization" in CLAUDE.md).

2. injCC2 startup gap: normal transient, NOT a bug. scheduled==fired in steady
   state is the health signal. Growing gap would indicate the pendingCount bug.

3. MAP does not bottleneck fuel scheduling: PW is computed at 200 Hz in
   periodicFastCallback, one engine cycle ahead. No MAP read in any tooth ISR.

4. trgPostDecode scheduling teeth: 6-7% of total teeth take 50-100 us; the
   remaining 93-94% take <50 us. This is irreducible with the current code;
   the "next optimization" (eliminating angle queue spark) would save ~18 us,
   moving some scheduling teeth from 50-100 us to <50 us bucket.

### Validation

All sessions on actual car hardware (21129 Lada engine, m74_9 ECU, AT32F435).
Final build: BUILD SUCCESSFUL, unit tests 1169/1169 PASSED.

### Open follow-ups

- Eliminate angle queue spark path under EFI_ANGLE_CLOCK (then sparkFiredNoOpBeforeDwell
  can also be removed) - saves ~18 us on scheduling teeth.
- C9009 false positive: move prevSparkName update outside the rpm gate.
- Confirm injCC2 startup gap max value and decide if a startup-tolerance
  allowance should be printed in lockstats (e.g. injCC2 max_startup_gap=47).
- trgPostDecode 100-250 us: rare (~0.3% at 6000 rpm) but present; cause is
  the trigger decode tail, not scheduling logic.

## 2026-09-01 (night 11) - MAP averaging architecture analysis + sniffer n/a root cause

### Problem observed

Engine sniffer shows 4 injection events, but MAP events display:
  8.96ms  n/a   n/a
  8.93ms  n/a   n/a
  8.91ms  115.8  165.8
  8.91ms  295.4  345.5
Two cylinders always show n/a for MAP averaging.

### Root cause: second-revolution display gap in TunerStudio sniffer

All 4 MAP windows DO fire (confirmed: lockstats startAveraging n=1461 at 800 rpm
matches 4 windows/cycle * 6.67 cycles/s * ~55 s). The n/a is a display artifact.

Map window angles (samplingAngle=115.8 deg at 800 rpm, 50 deg window):
  cylinder 1 (i=0): 115.8-165.8 deg   <- 1st crank revolution (0-360 deg)
  cylinder 3 (i=2): 295.8-345.8 deg   <- 1st crank revolution
  cylinder 4 (i=3): 475.8-525.8 deg   <- 2nd crank revolution (360-720 deg)
  cylinder 2 (i=1): 655.8-705.8 deg   <- 2nd crank revolution

The sniffer associates MAP events with injection events by proximity in the
current sniffer frame. Windows at 475.8 and 655.8 deg fall in the second
crank revolution; the per-injection display does not span far enough forward
from those cylinders' injection events to capture the MAP pulse, so it shows
n/a. No firmware action needed for shared-plenum engines.

### currentMapAverager = 0 (static, never incremented - TODO in code)

All 4 windows write to the same MapAverager[0]. Each startAveraging() resets
m_counter/m_sum. Only the last completed window contributes to mapAveraged.
mapPerCylinder[i] is overwritten by each successive window.

For 21129 shared plenum: harmless. MAP is nearly identical across all
cylinders at any steady-state operating point.

### What cycling currentMapAverager through 4 would give

Increment: currentMapAverager = (currentMapAverager + 1) % cylindersCount.

Benefits:
- mapPerCylinder[i] = truly per-cylinder, no cross-cylinder overwrite
- averagedMapRunningBuffer receives 4 writes per cycle instead of 1;
  with mapMinBufferLength=4 gives min-of-4 (leanest cylinder, most airflow)
- mapAveraged smoother: updated 4x per cycle vs once
- Enables per-cylinder LTFT, misfire detection by MAP drop

For 21129 shared plenum: marginal improvement (same MAP on all cylinders).
For ITB or per-runner MAP sensor: significant - true per-cylinder fueling.

Engine sniffer n/a: NOT fixed by 4 averagers (display issue is independent
of averager count - same mapAveragingPin transitions, same association gap).

### Validation

Analysis only, no code changes. All facts verified from map_averaging.cpp
(MapAveragingModule::onFastCallback, onEnginePhase, startAveraging, endAveraging)
and engine_cylinder.cpp (getAngleOffset) and 21129.msq config values.

## 2026-09-03 (evening) - dwell lateArm=59% analysis: why dwell falls back to TIM5 (code + 19:14 session data)

### Observed

19:14:07 lockstats (period 18:54:22 -> 19:14:07, 20 min, avg ~1550 rpm, peak
3452 rpm per the 19:07 C9009):

  angclk dwell fired=25412 lateArm=36299  spark fired=61711 lateArm=0  inj fired=49353 lateArm=284
  sched dwell: n=36299 late>=10us=24490 maxLateUs=118 cbmaxUs=186
  sched fuel:  n=284   late>=10us=284   maxLateUs=134

Earlier snapshot 18:54:22 (3 h since boot, mostly idle/cranking): dwell
fired=29379 lateArm=8016 (21%). So the fail rate rose 21% -> 59% when the
session turned into sustained driving - rpm-dependent.

Engine was healthy throughout: zero C9002/C9003/C935x/C9012, one C9009
false positive (charged=0 bail=no, the known rpm-flap artifact).

### Verified 1:1 cross-checks (from the log, not from code)

- sched dwell n=36299 == angclk dwell lateArm=36299 EXACTLY: every failed
  TMR2 arm produces exactly one TIM5 "dwell" fallback event.
- sched fuel n=284 == inj lateArm=284: same 1:1 for injection.
- dwell attempts = fired + lateArm = 61711 == spark fired: exactly one dwell
  arm per spark, none skipped, none duplicated.

### Why dwell goes to TIM5 (code paths)

angleClockArmDwell (firmware/hw_layer/angle_clock/angle_clock.cpp L352-380)
returns false in exactly two places:

1. armGuard (L342-350): s_ticksPerDegree <= 0 (no valid angle->time basis -
   first teeth after start) OR remainingAngle(target) > MAX_LEAD_DEG=30
   (stale phase basis after desync). Both hit injection equally, so they
   cannot explain the dwell-vs-inj gap (inj lateArm=284).
2. Tick-past check (L365-369): (int32_t)(atTick - TMR2->CNT) < 4 us, i.e.
   the remaining lead TIME is smaller than the handoff elapsed time at the
   arm moment. This is the only dwell-specific, rpm-dependent mechanism.

Why dwell is exposed and injection is not (main_trigger_callback.cpp order):

| event | early window [next,nextNext) | current window [current,next) |
| --- | --- | --- |
| injection | armed in handleFuel ~13 us elapsed (both windows in ONE call) | same ~13 us - tick-past essentially never fails |
| dwell | scheduleDwellEarlyIfDue ~30-60 us elapsed, remaining 6-12 deg | onTriggerEventSparkLogic ~100-250 us elapsed, remaining 0-6 deg |

The early-window dwell arm (6-12 deg = 645-1290 us at 1550 rpm) cannot fail
the tick-past check below ~7000 rpm. The current-window arm fails with
probability ~= elapsed / 6 deg: ~0-10% at 800 rpm, 25-60% at 2500,
35-85% at 3450 (elapsed 100-250 us per the trgPostDecode 50-100us=76778 /
100-250us=9433 buckets). This matches the 21% (idle-heavy) -> 59% (drive)
trend exactly.

### Why the current window is reached at all

In steady state the early window at tooth T-1 covers exactly
[currentPhase(T), nextPhase(T)), so the current-window arm at T runs only
when the early arm was skipped:

- rpm==0 storm-flap teeth: the rpm==0 early return in mainTriggerCallback
  (L368-387) sits BEFORE scheduleDwellEarlyIfDue (L408). The flap storm is
  proven to exist at idle AND at 3452 rpm (that is exactly the C9009
  false-positive mechanism - the rpm sensor cache reads 0 during a dwell
  callback).
- First cycle after (re)sync (fresh prepareIgnitionSchedule).
- nextNextPhase==nextPhase guard: does NOT fire on m74_9 - verified
  findNextTriggerToothAngle skips RiseOnly duplicate angles and the caller
  passes +2 (trigger_central.cpp L1069).

### Consequences (why the car drives fine)

Each lateArm is NOT a lost event: scheduleSparkEvent falls back to
engine->scheduler.schedule("dwell", ..., chargeTime) with chargeTime =
edgeTs + angleOffset x oneDegreeUs (L562-567). The charge fires 0-6 deg
after the arm tooth by time, the spark is always re-armed from the actual
charge moment on TMR4 (angleClockArmSparkFromNow -> spark lateArm=0), and
the charge-anchored 2.5x-dwell rescue stays as the coil safety net. The
cost is precision: those events run with the old FALSE-build behavior
(0-1 tooth lead + executor lateness - 67% of the TIM5 dwells fired >=10 us
late, max 118 us) instead of the 2-tooth hardware lead.

### Fix candidates (not implemented, recorded)

(a) Run scheduleDwellEarlyIfDue on flap teeth too: it uses neither rpm nor
    dwellMs in the EFI_ANGLE_CLOCK build (both UNUSED), so it can move
    before/into the rpm==0 branch; the flap then stops skipping the early
    arm. Smallest change.
(b) Merge both windows into scheduleDwellEarlyIfDue (the injection pattern)
    and drop the late current-window arm from onTriggerEventSparkLogic.
(c) Telemetry to prove the split on the next drive: split s_lateArmDwell
    into basis/lead30/past buckets + caller (early/current) + a
    last-failure snapshot (target angle, s_currentPhase, remaining, delay,
    CNT) - the pattern used for the 2026-08-31 arm-refusal diagnosis.

### Validation

Analysis only, no code changes. Every claim cross-checked against the
console log artifacts/logs/efi_log_2026-09-03_15_48_22_078.log.0 (the two
lockstats dumps, the sched per-kind lines, trgPostDecode histograms, the
19:07:04 C9009 rpm=3452) and the firmware sources listed above. No unit
tests touched (the arm path is register-level TMR2 code, not host-testable
beyond the pure tick math already covered in test_angle_clock.cpp).
