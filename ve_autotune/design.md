# VE Table Auto-Tune — Design

Date: 2026-11-27
Project: rusEFI standalone ECU, microRusEFI mre_f4, Aprilia SL1000 Falco (Rotax 998cc)

## Intent / why

The user wants a standalone program — not TunerStudio's UI — that learns from how
the bike is actually driven and produces a corrected VE (fuel) table file ready to
burn onto the ECU. This mirrors TunerStudio's built-in "VE Analyze Live" / Auto Tune
feature, but as an offline, reviewable pipeline that fits the MCP/CLI workflow
already used for this ECU all session (`mcp_drive.py`), rather than requiring
TunerStudio to be open.

The user explicitly rejected live/automatic writing to the ECU while driving: the
program **records a drive, then generates a map file to upload** — the user reviews
and burns it afterward, not during the ride.

## Current state (verified from the project, not guessed)

- Engine config: Speed Density, VE Table 16×16, X = RPM (`veRpmBins`), Y = MAP in kPa
  (`veLoadBins`), Z = `veTable` (VE %).
- Target AFR table (`lambdaTable`) already has sane values: 14.7 at low load down to
  10.7 at full load, same RPM/MAP axes family (`lambdaRpmBins`/`lambdaLoadBins`).
- `fuelClosedLoopCorrectionEnabled = disabled` — the ECU is not trimming fuel live,
  so a recorded AFR sample is the raw, uncorrected result of the current VE table
  (no STFT to back out).
- The ECU's own `.ini` already defines the VE-Analyze relationship used by
  TunerStudio: `veAnalyzeMap = veTableTbl, veAnalyzeTargetTableTbl (lambdaTable),
  veAnalyzeAfrLambda1, egoCorrectionForVeAnalyze (Gego)`. This program reproduces
  that same relationship instead of inventing new tuning math.
- AFR input is configured (`afr_hwChannel = "31 - AN volt 3"`, Innovate-style linear
  calibration, 0V→AFR 7.35, 5V→AFR 22.39) but whether a physical wideband controller
  is actually wired and healthy right now is **not confirmed**.
- `mcp_ecu` (used via `mcp_drive.py`) already has a field-tested, running data
  logger: `start_data_logging` / `stop_data_logging` write a full-channel-set
  `.mlg` log (with an embedded snapshot of the tune active during that recording),
  and `convert_log_to_csv` turns it into CSV. No new logging code is needed.
- No drive logs exist yet. The engine is not running on fuel yet (VSS was just
  fixed this session; ignition timing is still unverified). This tool is being
  built now so it is ready, not something to road-test today.

## Goals

- Record a real drive with the existing logger, turn it into a corrected VE table,
  with zero manual log-wrangling beyond running the tool.
- Only trust cells with enough steady-state samples; leave the rest of the table
  untouched.
- Cap how much any cell can move in one pass, so one bad drive can't wreck the map.
- Always produce a human-readable before/after report alongside the new tune file.
- Never burn anything to the ECU automatically — the user reviews the report and
  the diff, then explicitly asks for it to be written.

## Non-goals (out of scope for this version)

- Ignition timing auto-tune (needs knock feedback — separate problem).
- Warm-up / cold-start AFR tuning (`wueAnalyzeMap` territory).
- Live, in-drive closed-loop table writing.
- A GUI. Output is a text/markdown report plus a `.msq` file; a visual diff can be
  a later addition if wanted.
- Replacing TunerStudio — this is an alternate workflow for people already driving
  the MCP/CLI path, not a claim that TS's own Auto Tune is broken.

## Anti-goals (would make this a failure even if it "works")

- Applying a correction computed from too little or unstable data.
- Silently leaning out a cell more aggressively than richening it — leaning errors
  are the dangerous direction for engine safety, so the clamp logic must treat them
  at least as conservatively as richening, never more permissively.
- Burning to the ECU without an explicit, separate go-ahead after the user has seen
  the diff.

## Constraints

- Must use the existing `start_data_logging` / `stop_data_logging` /
  `convert_log_to_csv` MCP tools — not a hand-rolled polling logger.
- Must read RPM/MAP bin edges and the target AFR table from the tune snapshot
  embedded in the log being analyzed (not hardcoded), the same lesson learned
  earlier this session with the VSS pin enum values — never guess numeric/table
  values when they can be read from the source of truth.
- CSV column identification must be done by matching the `.ini`'s channel names,
  not by assuming a fixed column order, since the exact CSV layout from
  `convert_log_to_csv` hasn't been observed yet (no real log exists to inspect).
- Output `.msq` must be built from the tune snapshot that was embedded in the log
  at recording time, not whatever happens to be the live tune when the tool runs
  later — those can differ.
- No burn-to-ECU step without a separate, explicit user confirmation after review.

## Approaches considered

- **A — Standalone offline pipeline (chosen).** Record with the existing logger →
  CSV → bin/filter/compute correction → diff report + new `.msq`, reviewed, then
  burned on request. Matches what the user asked for, reuses proven logging infra.
- **B — Use TunerStudio's own built-in VE Analyze Live.** Already wired to this
  exact tune via `veAnalyzeMap`; zero new code. Rejected only because the user
  wants this outside the TS UI, as part of the existing MCP/CLI workflow — not
  because B is technically worse.
- **C — Hand-rolled live logger via repeated `read_output_channel` polling.**
  Rejected: reinvents the already-existing, field-tested `EcuDataLogger`, at a
  lower and less reliable sample rate.

## Decided design (Approach A)

**Pipeline, four stages:**

1. **Record.** Wrapper around `start_data_logging` (writes `.mlg`, embeds the
   active tune) and `stop_data_logging`. User starts it before a ride, stops it
   after.
2. **Convert.** Call `convert_log_to_csv` on the resulting `.mlg`.
3. **Analyze.**
   - Load the tune snapshot embedded in the log (not the live tune) to get
     `veRpmBins`, `veLoadBins`, `veTable`, `lambdaTable`, `lambdaRpmBins`,
     `lambdaLoadBins`, and `egoCorrectionForVeAnalyze`'s inputs.
   - For each CSV row, resolve RPM / MAP / measured AFR columns by matching
     channel names from the `.ini`, not fixed positions.
   - Filter to steady-state rows only: reject rows during warmup (coolant temp
     below a threshold), during cranking, during measurable throttle/RPM/MAP
     transients (rate-of-change above a threshold = accel enrichment or lift-off
     in progress), and during decel fuel cut.
   - Bin surviving rows into the 16×16 VE grid by nearest RPM/MAP bin edge.
   - Per cell with at least a minimum sample count: average measured AFR,
     interpolate the matching target AFR from `lambdaTable`, correction factor =
     measured / target (richens when measured is leaner than target, as in
     rusEFI's own `veAnalyzeMap`/Gego convention), apply with closed-loop factor
     neutral since `fuelClosedLoopCorrectionEnabled` is disabled here.
   - Clamp each cell's change to a maximum percentage per pass (default below);
     leaning clamps are at least as tight as richening clamps.
   - Cells below the minimum sample count are left unchanged and flagged in the
     report as "not enough data."
4. **Report + output.** Write a before/after table (old VE, new VE, Δ%, sample
   count, measured vs. target AFR) to a text/markdown report, and write a new
   `.msq` (a copy of the log's embedded tune with only `veTable` changed) next to
   it. Nothing is sent to the ECU at this stage.

**Burn step** is a separate, explicit action: once the user has looked at the
report and says to proceed, use the existing `write_tune` tool against the
generated `.msq`, the same way the VSS pin migration was burned earlier this
session.

**Defaults the builder should use unless told otherwise (my call, flagged as
such):**
- Minimum samples per cell to trust a correction: 20.
- Maximum VE change per cell per pass: ±15%, with the leaning direction capped at
  least as tight as richening (e.g. richening could stay at 15%, leaning should
  not exceed that).
- Steady-state thresholds (RPM/MAP/TPS rate-of-change, coolant-temp floor): pick
  reasonable starting values, document them clearly as constants near the top of
  the analysis code so they're easy for the user to tune once real data exists.

**Testing without a running engine:** since the bike isn't running on fuel yet,
real road data doesn't exist. The implementer should write the binning/filtering/
correction-math logic against synthetic/fabricated CSV fixtures (made up RPM/MAP/
AFR sequences, including transients and low-sample cells to verify they get
skipped) so the math is verified now. Actual behavior on a real drive log is
verified later, once the engine runs and a real log can be recorded.

## Addendum: Android recorder for Stage 1 (decided after design approval)

After approving the design above, the user asked for the drive-recording step to
work from their phone over the Bluetooth link already set up for TunerStudio
(see the microRusEFI BT project: XM-15B module, classic Bluetooth SPP, same
binary TunerStudio protocol as over USB, just a different transport). Given the
choice between "use a laptop over that same BT link, no new app" and "build a
dedicated Android app," the user chose the Android app.

**Scope, kept deliberately narrow:** the app's only job is Stage 1 — connect over
Bluetooth Classic (SPP) to the already-paired ECU module and record a drive to a
CSV log on the phone. It does **not** analyze anything and it **never writes to
the ECU** — Stages 3 (Analyze) and 4 (Burn) stay on the PC exactly as already
designed, including the burn-safety diff guard. This keeps the one operation
that can alter the ECU (`write_tune`) in the already-reviewed path, and limits
the new Android code to a read-only protocol client.

**Protocol:** the app must speak enough of rusEFI's TunerStudio binary protocol
to read output channels — not reimplement write/burn. Rather than guessing the
framing/CRC, port it from the existing, generated source of truth already in
this checkout: `java_console/models/.../config/generated/Integration.java` (command
bytes, e.g. `TS_OUTPUT_COMMAND`) and the CRC implementation exercised by
`java_console/io/src/test/java/com/rusefi/binaryprotocol/test/CRCTest.java`. Those
are generated from the same firmware definitions as everything else in this
project, so porting them is "read the source of truth," not "invent a new
guess" — consistent with the rest of this design.

**Transport:** Android `BluetoothSocket` using the standard SPP UUID, opened
against the already-paired XM-15B MAC address. Pairing itself happens once in
Android's own Bluetooth settings, outside the app — the app does not implement
pairing/discovery UI.

**Output:** a CSV with one column per channel, named after the `.ini` channel
names (not fixed column order) — the same convention already decided for Stage
3, so an Android-recorded CSV and a USB/BT-via-PC-recorded CSV (through the
existing `start_data_logging`/`convert_log_to_csv` tools) are interchangeable
inputs to Stage 3. Needed channels: RPM, MAP, the configured AFR channel,
coolant temperature (warmup filter), TPS (transient filter) — the app reads
which exact fields/offsets these are from the `.ini`, the same "never hardcode
a protocol value" rule as the rest of this project.

**Toolchain note:** this machine has no Android SDK/Gradle installed yet — first
build will need `cmdline-tools`/`sdkmanager` to fetch the SDK platform and
build-tools (roughly 1GB of downloads). JDK 17 is already present. Real
on-device verification (actually connecting to the ECU over BT from the phone)
can only happen once the user has the APK on their phone and in range of the
bike; until then, the protocol/CSV-writing logic gets unit tests against
synthetic byte sequences, the same "test the logic now, verify on real hardware
later" approach already decided for the VE-analyze math.

## File locations

New, uncommitted folder in the existing checkout (not part of upstream rusEFI, not
git-committed): `/home/whip/rusefi/ve_autotune/`
- `design.md` — this document.
- the pipeline script(s) (language: Python, consistent with `mcp_drive.py` and the
  rest of the MCP tooling already in use).
- `logs/` — recorded `.mlg`/`.csv` files.
- `output/` — generated reports and `.msq` files, one pair per run.

## Added after independent review

A fresh reviewer read this doc plus the actual MCP tool implementations
(`EcuMcpServer.java`, `mcp_drive.py`, the `.ini`, the java_console VE-analyze
helper classes) and found four real gaps. Three are settled here as engineering
calls; one went back to the user because it changes the CLI shape.

**1. `write_tune` merges the *whole* tune, not just VE — burn step needs a
safety check.** Confirmed in `EcuMcpServer.doWriteTune`: it merges every
compatible field from the input `.msq` into the live ECU except `vinNumber`.
If the user changes something else on the live ECU (e.g. ignition timing,
explicitly called "still unverified" above) between recording a drive and
burning the result, a naive burn would silently revert that interim work.
**Decided:** before burning, the burn step must read the current live tune and
diff it against the log's baseline snapshot, field by field, excluding
`veTable` and `vinNumber`. Any difference gets printed and the user must
explicitly confirm "burn anyway" — never a silent auto-proceed, never a silent
abort.

**2. The "reproduce TunerStudio's VE-Analyze" claim needs a guard, because that
algorithm isn't actually in this codebase to copy.** The `.ini`'s
`veAnalyzeMap` only names the four fields involved (`veTableTbl`,
`veAnalyzeTargetTableTbl`/`lambdaTable`, the AFR channel, and
`egoCorrectionForVeAnalyze`/Gego) — TunerStudio's internal correction math is
closed-source. The plain `correction = measured / target` this tool uses is a
reasonable reconstruction, not something pulled from source, and it stays
correct only while Gego is neutral. **Decided:** the analyzer must check the
log's embedded snapshot for `fuelClosedLoopCorrectionEnabled`. If it is
anything other than disabled, or if the logged Gego/STFT value isn't neutral
during the drive, refuse to compute a correction for that log and say why,
rather than silently double-counting a correction that's already live on the
ECU. Revisit this guard if closed-loop fueling is ever turned on.

**3. No confirmed physical wideband sensor — the analyzer needs a plausibility
check, not just a steady-state check.** A faulted or unplugged wideband can
output a flat, "steady" voltage (e.g. during its own warm-up or on a fault),
which would sail through the rate-of-change filter and look like trustworthy
data. **Decided:** per log, reject the whole log if its AFR channel shows no
real variance across the entire recording, and per cell, reject samples that
sit suspiciously pinned at a calibration endpoint (near `afr_value1`/
`afr_value2`, i.e. 7.35 or 22.39 AFR) rather than trusting them. Flag this
clearly in the report ("AFR channel looks flatlined/railed — check wideband
wiring") instead of quietly producing a correction from it.

**4. Single log per pass, or combine several rides before the first
correction?** With 256 cells and a 20-sample minimum, one short ride realistically
covers only a fraction of the table. **Asked the user — answer: combine several
rides.** Decided: `Analyze` accepts multiple CSV logs in one invocation and pools
samples per cell across all of them. Each input log carries its own embedded tune
snapshot; the tool uses the most recently recorded log's snapshot as the baseline
for the output `.msq` and the live-tune diff in guard #1, and it must warn (not
silently ignore) if the non-`veTable` fields differ between the snapshots of the
logs being combined, since that means the car wasn't in a consistent configuration
across those rides.

## What's left to the builder

- Exact column-name matching strategy against the `.ini` for the CSV header.
- Exact steady-state filter thresholds (flagged above as adjustable constants).
- Exact report format (plain text vs. markdown table) — either is fine.
- Whether RPM/MAP binning uses nearest-bin or linear interpolation across the four
  surrounding cells (TunerStudio's own VE Analyze interpolates across neighbors;
  nearest-bin is simpler and defensible for a first version — builder's call,
  state which was chosen in a comment).
