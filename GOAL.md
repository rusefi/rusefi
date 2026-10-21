# Firmware engine operation review

Revisit the firmware for additional defects that can affect engine operation.
Prioritize unintended torque, incorrect fuel or spark delivery, failed engine
protection, loss of synchronization, stalls, and starting or restart failures.
Deliver ranked findings with reproducible evidence and useful test coverage.

**Status:** Review running. Session started 2026-10-21 16:19:12 UTC.
Elapsed time includes initial task/guidance inspection; update at checkpoints.
**Time budget:** 8 hours total, or 480 minutes.
**Time consumed:** 234.9 minutes (checkpoint 2026-10-21 20:14:06 UTC).
**Time remaining:** 245.1 minutes.

## Budget and stopping rules

Start measuring elapsed wall time when execution of this review begins. Count
source inspection, experiments, tool execution, builds, tests, and reporting.
Carry consumed time across resumed sessions; exclude time when the review is
explicitly paused or no review session is running. Do not reset the budget on
resume or convert it into a token budget.

Update the consumed and remaining time at each hourly checkpoint and before
stopping. Keep a concise current checkpoint in this file: areas covered,
strongest unresolved lead, and next action. Reserve the final hour for
validation and reporting. Stop at eight hours, preserving unresolved leads
with concrete next steps. Finish earlier if the planned review is exhausted;
do not wait merely to consume the budget.

| Allocation | Minutes | Intended result |
| --- | ---: | --- |
| Baseline and existing evidence | 30 | Confirm source state, master changes, existing findings, and relevant coverage |
| Ignition, triggering, scheduling, and knock | 90 | Trace event identity, timing, cancellation, and synchronization transitions |
| Fuel delivery and transient calculations | 75 | Check commanded pulses, staging, compensation, and operating-mode transitions |
| Protection and torque actuators | 75 | Check cut arbitration, actuator commands, and fault recovery |
| Sensors, configuration, and shared state | 60 | Check validity, freshness, initialization, live updates, and asynchronous state |
| Reproduction and candidate resolution | 90 | Establish the strongest leads with tests or complete source paths |
| Final validation and reporting | 60 | Run checks, reconcile findings, and document remaining coverage gaps |
| Total | 480 | Complete the bounded review |

Reallocate investigation time toward stronger leads while preserving the
total budget and final validation hour. Add reproductions throughout the
review rather than postponing all testing to the reproduction allocation.

## Starting evidence

Read [CLAUDE.md](CLAUDE.md) and any applicable directory guidance first.
Inspect the working tree and preserve existing changes and generated files.
Fetch master for comparison and identify relevant changes; do not assume the
checkout is current or rewrite branch history as part of this goal.

Use [order_by_severity.md](order_by_severity.md) as the findings inventory and
severity standard. Read its active, fixed, candidate, rejected, and coverage
sections before investigating. The starting inventory contains 12 active and
24 fixed findings, with 19 defect reproductions in
[test_ecu_logic_review.cpp](unit_tests/tests/test_ecu_logic_review.cpp).
Reconcile those numbers against the actual source when the review starts.
Existing validation results are context, not evidence for subsequent changes.

Consult the relevant architecture notes in [docs/AI](docs/AI), especially
fueling, ignition, scheduling, sensors, and engine protection. Spend most of
the investigation on uncovered paths and distinct defects. Revisit an existing
finding when changed code, missing evidence, or a materially different trigger
justifies it; preserve its identifier and avoid duplicate reports.

## Investigation priorities

- **Ignition and synchronization:** queued dwell and spark actions during
  stop, restart, sync loss, phase changes, and configuration updates; shared
  coil and cylinder identity; odd-fire and multispark behavior; stale callbacks
  and outputs left energized. Resolve the delayed software-knock window
  identity gap in ECU-032 where feasible, including overlapping windows and
  restrikes. Check which paths the host feature flags actually compile.
- **Fuel delivery:** primary and secondary scheduling, pulse overlap, timing
  locks, deadtime, pressure compensation, cylinder and bank trims, wall film,
  acceleration enrichment, priming, cranking, and DFCO. Trace nonfinite or
  out-of-range values through to scheduling and outputs, including live tune
  writes before burn validation. Check stop and restart state retention.
- **Protection and actuators:** arbitration among rev, boost, oil, lambda,
  injector-duty, launch, traction, shift, and Lua cuts; throttle, wastegate,
  idle, VVT, fuel pump, nitrous, and starter behavior. Check activation,
  disabling, feedback loss, recovery, hysteresis, and competing requests.
- **Sensors and operating transitions:** missing, invalid, stale, and
  implausible feedback; cold start, hot restart, stalled engines, and rapid
  resynchronization. Verify units, bounds, calibration acceptance, fallback
  choices, and whether sensor availability is confused with configuration.
- **Asynchronous and production-only behavior:** state shared by trigger
  callbacks, periodic work, and configuration updates; timer conversions,
  event lifetime, interrupt assumptions, and hardware paths omitted from host
  tests. Follow I/O or persistence code when there is a concrete connection
  to delayed engine control or incorrect outputs.

## Evidence and coverage requirements

For each credible finding, record the triggering configuration and operating
sequence, reachable source path, actual and intended behavior, likely engine
consequence, severity, confidence, and test or other supporting evidence.
Separate demonstrated software behavior from inferred physical consequences.
Keep unresolved premises in the candidate section and explain why rejected
leads do not establish a defect. Do not require a minimum finding count.

Prefer a passing reproduction that explicitly asserts the current defect.
Follow the repository's coverage-first bugfix process: this goal produces
investigation and coverage, leaving production fixes for a separate change.
For already-fixed findings, retain or strengthen tests of correct behavior
and remove obsolete defect assertions only when replacement coverage is clear.

Exercise real controller updates and scheduled callbacks where practical.
Check meaningful outputs, state transitions, boundary conditions, and recovery;
an isolated arithmetic test must not be described as an end-to-end test.
Use deterministic invalid payloads instead of uninitialized storage, and do
not deliberately execute undefined behavior in the normal passing test suite.
Keep new host tests portable and account for persistent state between fixtures.

Build with `make -C unit_tests -j12` and run focused tests from `unit_tests/`.
Run the full host suite after the final test changes. Use shuffled runs where
state retention is a concern and targeted firmware builds where production
feature guards affect a claim. Record exact test selections and results;
document inaccessible hardware or feature paths as specific coverage gaps.

## Deliverables and completion

- Update [order_by_severity.md](order_by_severity.md) with new or revised
  ranked findings, fixed status, narrowed candidates, and validation limits.
  Preserve existing IDs and assign unused IDs to distinct new findings.
- Add focused coverage in the review test file or the appropriate existing
  subsystem suite. Report what each test establishes and what remains untested.
- Prepare the local commit-message draft in [docs/report.md](docs/report.md).
  Add durable, non-obvious tooling or architecture knowledge to `CLAUDE.md`
  only when it cannot be inferred readily from source or history.
- Report elapsed time, areas inspected, confirmed findings, checks performed,
  failures or blockers, and prioritized follow-ups for uncovered areas.

Stage new source and documentation files as required by repository guidance.
Keep `docs/report.md`, generated files, and build artifacts unstaged. Leave
changes uncommitted and do not push.

The review is complete when the planned investigation has been exhausted and
the deliverables are validated, or it stops at the time limit with a clear
handoff. Budget exhaustion does not mean all firmware paths were reviewed or
all suspected defects were resolved.

## Current review checkpoint

- Baseline: fetched origin/master; nine local commits ahead, zero behind. Initial inventory: 12 active / 24 fixed / 19 review reproductions. Five pre-existing generated-file modifications remain unstaged.
- Reviewed ignition/trigger/knock scheduling, fueling/staging/priming/transients, protection and lambda banks, launch/anti-lag/Lua lifecycle, ETB/VVT/idle, sensor lifecycle/calibration, HPFP, start/stop and configuration storage.
- Inventory now has 26 active and 24 fixed findings. ECU-037 through ECU-050 are new; ECU-012/032 have stronger delayed software-knock evidence. Production behavior remains unchanged.
- Current full normal host suite passes: 1,869 tests / 304 suites. All new focused tests pass individually, including LTFT learning from frozen corrections and fixed-timing retention of a released launch cut.
- Full-suite repeated shuffle exposed SensorInit.Map consuming a retained static MAP value, plus unrelated repeat-state failures. Added fixture invalidation; rebuild and rerun pending. EventQueue.simple's file-static previous value and GPIO test registration counters show repeat-state causes; production defects are not inferred from those test artifacts.
- Optional software-knock variant: six tests and three shuffled runs passed earlier; rebuild with all current additions is in progress. Driver boundary remains an observer, not ADC/DSP coverage.
- F407 production build passes; ELF contains knock/HPFP callbacks. Disassembly confirms extra-page burns reach forced settings writes. No hardware flash stall, DSP acquisition or physical engine effects were measured.
- Normal and optional builds completed. Expanded 118-test selection passes three shuffled runs; six optional knock tests pass three shuffled runs. MAP fixture isolation is validated.
- New strongest lead: external output removal loses the old pin address without sending an off command. MC33810 inherits a no-op mode setter. EG33 firmware builds and links this driver; host scheduled-pulse/removal test had a firing-order fixture mismatch, now corrected and awaiting execution.
- Existing EventQueue.simple and gpioext.testGpioExt fail their second iteration when run alone, independently confirming unrelated repeat-state limitations.
- Next: finish test isolation/validation, characterize remaining repeat-only failures independently, resolve remaining sensor/asynchronous candidates, and reconcile the report and coverage limits.
- Session accounting anchor: 2026-10-21 16:19:12 UTC; no explicit pauses.
