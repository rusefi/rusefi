# Firmware engine operation review

Revisit the firmware for additional defects that can affect engine operation.
Prioritize unintended torque, incorrect fuel or spark delivery, failed engine
protection, loss of synchronization, stalls, and starting or restart failures.
Deliver ranked findings with reproducible evidence and useful test coverage.

**Status:** Complete. Planned source/host investigation and deliverables validated before the time limit.
Elapsed time includes initial task/guidance inspection; update at checkpoints.
**Time budget:** 8 hours total, or 480 minutes.
**Time consumed:** 266.5 minutes (final checkpoint 2026-10-21 20:47:53 UTC).
**Time remaining:** 213.5 minutes unused.

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

## Final review checkpoint

- Completed the bounded source/host investigation across ignition, synchronization, knock, fueling/transients, protection and torque actuators, sensors/configuration, start/restart, HPFP and persistence. The continuation resolved the remaining host-testable nitrous/VSS leads and two wall-film defects. Hardware- and policy-dependent premises remain explicit follow-ups.
- Inventory: 31 active / 24 fixed / 55 unique IDs. ECU-037 through ECU-055 are new across the review; ECU-012/032 have stronger delayed driver-request evidence. The continuation adds ECU-052 through ECU-055 and four passing reproductions. Production source is unchanged.
- Coverage grows by 24 normal host cases and six optional software-knock cases overall. Tests observe real controller transitions and scheduled callbacks where practical, with mocked inputs and physical limitations stated per finding.
- Final `make -C unit_tests -j12` passes with GCC ASan/bounds. All 1,875 tests / 304 suites pass normally and in a separate fresh full shuffled process (31337). The expanded 224-case selection passes three shuffled iterations (31337–31339).
- A fresh full shuffle exposed an old Lua sensor-read mock leak. Two existing tests alone reproduced it before cleanup; both getter tests now clear their mocks. The repaired three-test selection passes three repeats. Earlier static MAP fixture isolation is retained. Existing GPIO/EventQueue full-suite-repeat limitations remain documented and are not production findings.
- Earlier six-case optional knock validation passes three shuffled iterations. F407 and EG33 F7 builds and saved disassembly establish the relevant production paths; production source is unchanged since those checks. No physical flash stall, injector/coil waveform, fuel mixture or knock ADC/DSP completion is claimed as measured.
- Fetched master again: HEAD remains 3ff25e8539a, origin/master 439833ec0ef, ten commits ahead / 32 behind. Relevant upstream changes were inspected without merging. The user's intervening commit remains intact; the agent did not commit, push or rewrite history.
- Findings, candidates, rejected/narrowed leads, exact checks and limitations are in order_by_severity.md. Architecture notes and the ignored local docs/report.md draft are updated. New coverage files from the earlier review are tracked; the continuation creates no new files. Final tests/docs remain uncommitted, and generated files/artifacts remain unstaged.
- Completion audit: exact build/test pass counts, new-case totals, 55 unique IDs, rank/detail ordering, repository links, empty index and whitespace checks pass. No tool or approval is pending.
- Prioritized follow-ups: measure external injector removal and unsafe flash-write windows on hardware; exercise knock ADC/DSP overlap and target ISR/shared-state behavior; resolve oil-sensor-loss policy; measure wall-film cut recovery and full crank/cam restart transients.
- Accounting: carried 244.0 minutes into the continuation at 2026-10-21 20:25:24 UTC; excluded the between-session interval. Continuation 22.5 minutes; total 266.5 minutes. Stop before the ceiling because the planned source/host investigation and deliverable validation are complete.
