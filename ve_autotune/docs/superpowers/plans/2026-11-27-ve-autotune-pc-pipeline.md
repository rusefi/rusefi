# VE Autotune — PC Pipeline Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build the CLI pipeline that turns one or more recorded drives into a
safety-checked, corrected VE table — a new `.msq` plus a before/after report —
without ever writing to the ECU except on an explicit, separately-confirmed
burn step.

**Architecture:** A Python CLI (`cli.py`) with four subcommands wrapping the
four pipeline stages. `record`/`burn` shell out to the existing
`mcp_drive.py`/`mcp_ecu` MCP tools (the only thing allowed to touch the live
ECU). `analyze` is pure Python: load CSV logs + their embedded tune snapshots,
filter to steady-state samples, bin into the 16×16 VE grid, compute a capped
correction, and write the output `.msq` + report. `burn` re-reads the live
tune and refuses to proceed silently if it has drifted from the snapshot the
correction was computed against.

**Tech Stack:** Python 3 (stdlib only: `xml.etree.ElementTree`, `csv`,
`argparse`), `pytest`.

**Spec:** `/home/whip/rusefi/ve_autotune/design.md`

## Global Constraints

- Minimum samples per cell to trust a correction: 20 (design.md, "Decided design").
- Maximum VE change per cell per pass: ±15%; the leaning-direction cap must
  never be looser than the richening-direction cap (design.md, "Decided
  design" and "Added after independent review" #1-3).
- RPM/MAP bin edges and the target AFR table (`lambdaTable`) must always be
  read from the tune snapshot embedded in the log being analyzed — never
  hardcoded (design.md, "Constraints").
- CSV columns are resolved by matching channel names from the `.ini`, never
  by fixed column position (design.md, "Constraints").
- Refuse to compute a correction for any log whose snapshot has
  `fuelClosedLoopCorrectionEnabled` enabled (design.md addendum #2).
- Reject a log (whole log) if its AFR channel shows no real variance across
  the recording, and reject individual samples pinned near a calibration
  endpoint (7.35 / 22.39 AFR) (design.md addendum #3).
- When pooling multiple logs, warn (never silently ignore) if any field
  other than `veTable`/`vinNumber` differs between the logs' embedded
  snapshots (design.md addendum #4).
- `burn` must diff the live ECU tune against the baseline snapshot
  (excluding `veTable`/`vinNumber`) and require explicit confirmation before
  writing if anything else differs (design.md addendum #1).
- Output `.msq` is the baseline snapshot with only `veTable` changed.

## Review Focus

- A log whose AFR channel is railed at a calibration endpoint (disconnected/
  faulted wideband) — must be rejected outright, not averaged in. (Task 3)
- A log recorded with closed-loop fuel correction enabled — must abort that
  log's contribution entirely, not apply a now-double-counted correction.
  (Task 7)
- Two pooled logs whose snapshots disagree on a non-`veTable` field (e.g.
  ignition timing changed between rides) — must warn, not proceed as if the
  car was in one consistent configuration. (Task 7)
- A cell whose raw measured/target ratio implies a large lean-out from one
  bad sample — must be capped, and the lean cap must never exceed the rich
  cap. (Task 5)
- Burning when the live ECU has drifted from the log's baseline in a field
  other than `veTable` (e.g. ignition timing edited in the interim) — must
  surface the exact fields that differ and require typed confirmation, never
  burn silently. (Task 8)

---

### Task 1: Tune snapshot I/O (`msq.py`)

**Files:**
- Create: `ve_autotune/pipeline/msq.py`
- Test: `ve_autotune/tests/test_msq.py`
- Test fixture: `ve_autotune/tests/fixtures/sample_tune.msq` (trimmed `.msq`
  XML containing just the tags this module reads/writes: `veTable`,
  `veRpmBins`, `veLoadBins`, `lambdaTable`, `lambdaRpmBins`,
  `lambdaLoadBins`, `fuelClosedLoopCorrectionEnabled`, plus one unrelated
  `<constant name="someOtherField">`-style tag to prove round-trip
  preserves it)

**Interfaces:**
- Produces:
  - `class TuneSnapshot` with fields `ve_table: list[list[float]]` (16×16),
    `ve_rpm_bins: list[float]` (16), `ve_load_bins: list[float]` (16),
    `lambda_table: list[list[float]]` (16×16), `lambda_rpm_bins: list[float]`
    (16), `lambda_load_bins: list[float]` (16),
    `fuel_closed_loop_enabled: bool`
  - `load_tune(path: str) -> TuneSnapshot`
  - `write_tune_with_ve_table(source_path: str, ve_table: list[list[float]], dest_path: str) -> None`
    — copies `source_path` byte-for-byte except the `<constant ... name="veTable">`
    block's values are replaced with `ve_table`.

- [ ] **Step 1: Write the failing tests**

```python
def test_load_tune_reads_ve_and_lambda_tables():
    snap = load_tune("ve_autotune/tests/fixtures/sample_tune.msq")
    assert len(snap.ve_table) == 16 and len(snap.ve_table[0]) == 16
    assert len(snap.lambda_table) == 16 and len(snap.lambda_table[0]) == 16
    assert len(snap.ve_rpm_bins) == 16
    assert len(snap.ve_load_bins) == 16
    assert snap.fuel_closed_loop_enabled is False

def test_write_tune_with_ve_table_changes_only_ve_table():
    snap = load_tune("ve_autotune/tests/fixtures/sample_tune.msq")
    new_ve = [[v + 1 for v in row] for row in snap.ve_table]
    write_tune_with_ve_table("ve_autotune/tests/fixtures/sample_tune.msq", new_ve, "/tmp/out.msq")
    reloaded = load_tune("/tmp/out.msq")
    assert reloaded.ve_table == new_ve
    assert reloaded.lambda_table == snap.lambda_table  # untouched
    # the unrelated fixture tag survived unchanged
    assert "someOtherField" in open("/tmp/out.msq").read()
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `pytest ve_autotune/tests/test_msq.py -v`
Expected: FAIL (module/fixture don't exist yet)

- [ ] **Step 3: Create the fixture file, then implement `msq.py`**

Build the fixture by hand from the real tags already seen in
`/home/whip/TunerStudioProjects/Falco_rusEfi/CurrentTune.msq` (trim
everything else). Implement `load_tune`/`write_tune_with_ve_table` with
`xml.etree.ElementTree`, parsing each `<constant ... name="X">` tag's text
as whitespace-separated floats, reshaping 256-value tables into 16×16 row
lists in document order. `fuel_closed_loop_enabled` is true iff the tag's
text is the literal string `"enabled"`.

- [ ] **Step 4: Run tests to verify they pass**

Run: `pytest ve_autotune/tests/test_msq.py -v`
Expected: PASS

- [ ] **Step 5: Commit**

```bash
git -C /home/whip/rusefi status --short ve_autotune/   # confirm nothing outside ve_autotune/ is staged
git -C /home/whip/rusefi add -N ve_autotune/ 2>/dev/null || true
```

(This repo is upstream rusEFI, not ours to commit to — see design.md "File
locations." Skip `git commit`; just leave the files on disk.)

---

### Task 2: CSV log loading (`csv_log.py`)

**Files:**
- Create: `ve_autotune/pipeline/csv_log.py`
- Test: `ve_autotune/tests/test_csv_log.py`

**Interfaces:**
- Consumes: nothing from Task 1.
- Produces:
  - `class MissingChannel(Exception)`
  - `load_csv_rows(path: str, channel_map: dict[str, str]) -> list[dict[str, float]]`
    — `channel_map` maps a logical name (e.g. `"rpm"`) to the exact CSV
    header text for that channel. Returns one dict per row, keyed by the
    logical names in `channel_map`, values parsed as `float`. Raises
    `MissingChannel(logical_name)` if a mapped header isn't found in the
    CSV (case/leading-trailing-space-insensitive match on the header).

- [ ] **Step 1: Write the failing tests**

```python
CSV_TEXT = "Time,RPM,MAP,AFR\n0.0,1200,35.0,14.6\n0.1,1210,35.2,14.5\n"

def test_load_csv_rows_maps_channels_by_name(tmp_path):
    p = tmp_path / "log.csv"
    p.write_text(CSV_TEXT)
    rows = load_csv_rows(str(p), {"rpm": "RPM", "map": "MAP"})
    assert rows[0] == {"rpm": 1200.0, "map": 35.0}
    assert rows[1] == {"rpm": 1210.0, "map": 35.2}

def test_load_csv_rows_missing_channel_raises(tmp_path):
    p = tmp_path / "log.csv"
    p.write_text(CSV_TEXT)
    with pytest.raises(MissingChannel):
        load_csv_rows(str(p), {"clt": "CoolantTemp"})
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `pytest ve_autotune/tests/test_csv_log.py -v`
Expected: FAIL

- [ ] **Step 3: Implement `csv_log.py`**

Use `csv.DictReader`. Build a header-text→actual-header lookup once (stripped,
case-folded), then resolve each `channel_map` value through it before
reading rows.

- [ ] **Step 4: Run tests to verify they pass**

Run: `pytest ve_autotune/tests/test_csv_log.py -v`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk, per Task 1 Step 5 note)

---

### Task 3: Steady-state and AFR-plausibility filtering (`filters.py`)

**Files:**
- Create: `ve_autotune/pipeline/filters.py`
- Test: `ve_autotune/tests/test_filters.py`

**Interfaces:**
- Consumes: rows shaped like Task 2's output (`dict[str, float]`), each with
  keys `rpm`, `map`, `tps`, `clt`, `afr`, `t` (seconds).
- Produces:
  - Module constants (documented inline as adjustable, per design.md
    "Defaults the builder should use"): `MAX_RPM_RATE = 500.0` (rpm/s),
    `MAX_MAP_RATE = 20.0` (kPa/s), `MAX_TPS_RATE = 50.0` (%/s),
    `MIN_CLT = 60.0` (°C), `AFR_RAIL_LOW = 7.4`, `AFR_RAIL_HIGH = 22.3`
    (just inside the 7.35/22.39 calibration endpoints).
  - `steady_state_rows(rows: list[dict]) -> list[dict]` — drops the first
    row (no prior sample to diff against) and any row where
    `abs(rpm[i]-rpm[i-1])/(t[i]-t[i-1])`, the equivalent for `map`, or for
    `tps`, exceeds its `MAX_..._RATE`, or where `clt < MIN_CLT`.
  - `class FlatlinedAfr(Exception)`
  - `check_afr_plausible(rows: list[dict]) -> None` — raises `FlatlinedAfr`
    if every row's `afr` is within `1e-6` of the first row's `afr` across
    the whole (unfiltered) log.
  - `reject_railed_afr(rows: list[dict]) -> list[dict]` — drops rows where
    `afr <= AFR_RAIL_LOW or afr >= AFR_RAIL_HIGH`.

- [ ] **Step 1: Write the failing tests**

```python
def _row(t, rpm, map_, tps, clt, afr):
    return {"t": t, "rpm": rpm, "map": map_, "tps": tps, "clt": clt, "afr": afr}

def test_steady_state_drops_rpm_transient():
    rows = [_row(0.0, 3000, 50, 10, 80, 13.0), _row(0.1, 3600, 50, 10, 80, 13.0)]
    assert steady_state_rows(rows) == []  # 6000 rpm/s > MAX_RPM_RATE

def test_steady_state_keeps_stable_row():
    rows = [_row(0.0, 3000, 50, 10, 80, 13.0), _row(0.1, 3005, 50.1, 10, 80, 13.0)]
    assert steady_state_rows(rows) == [rows[1]]

def test_steady_state_drops_cold_row():
    rows = [_row(0.0, 3000, 50, 10, 40, 13.0), _row(0.1, 3001, 50, 10, 40, 13.0)]
    assert steady_state_rows(rows) == []

def test_check_afr_plausible_raises_on_flatline():
    rows = [_row(i * 0.1, 3000, 50, 10, 80, 7.35) for i in range(10)]
    with pytest.raises(FlatlinedAfr):
        check_afr_plausible(rows)

def test_reject_railed_afr_drops_boundary_samples():
    rows = [_row(0, 3000, 50, 10, 80, 7.35), _row(0.1, 3000, 50, 10, 80, 13.2)]
    assert reject_railed_afr(rows) == [rows[1]]
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `pytest ve_autotune/tests/test_filters.py -v`
Expected: FAIL

- [ ] **Step 3: Implement `filters.py`** per the Produces block above.

- [ ] **Step 4: Run tests to verify they pass**

Run: `pytest ve_autotune/tests/test_filters.py -v`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk)

---

### Task 4: VE cell binning (`binning.py`)

**Files:**
- Create: `ve_autotune/pipeline/binning.py`
- Test: `ve_autotune/tests/test_binning.py`

**Interfaces:**
- Consumes: `ve_rpm_bins`/`ve_load_bins` shaped like Task 1's `TuneSnapshot`
  fields.
- Produces: `nearest_cell(rpm: float, map_kpa: float, rpm_bins: list[float], load_bins: list[float]) -> tuple[int, int]`
  — returns `(row, col)` where `row` indexes `load_bins` (Y axis) and `col`
  indexes `rpm_bins` (X axis), each the index of the closest bin edge.

- [ ] **Step 1: Write the failing tests**

```python
RPM_BINS = [800, 2000, 4000, 6000]
LOAD_BINS = [30, 60, 90, 120]

def test_nearest_cell_exact_match():
    assert nearest_cell(2000, 60, RPM_BINS, LOAD_BINS) == (1, 1)

def test_nearest_cell_rounds_to_closer_edge():
    assert nearest_cell(2900, 50, RPM_BINS, LOAD_BINS) == (0, 1)  # 2900 closer to 2000 than 4000; 50 closer to 60 than 30

def test_nearest_cell_below_range_clamps_to_first():
    assert nearest_cell(100, 10, RPM_BINS, LOAD_BINS) == (0, 0)

def test_nearest_cell_above_range_clamps_to_last():
    assert nearest_cell(9000, 500, RPM_BINS, LOAD_BINS) == (3, 3)
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `pytest ve_autotune/tests/test_binning.py -v`
Expected: FAIL

- [ ] **Step 3: Implement `nearest_cell`** with `min(range(len(bins)), key=lambda i: abs(bins[i] - value))`
for each axis.

- [ ] **Step 4: Run tests to verify they pass**

Run: `pytest ve_autotune/tests/test_binning.py -v`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk)

---

### Task 5: Correction math (`correction.py`)

**Files:**
- Create: `ve_autotune/pipeline/correction.py`
- Test: `ve_autotune/tests/test_correction.py`

**Interfaces:**
- Consumes: pooled per-cell AFR samples (`dict[tuple[int,int], list[float]]`),
  a `TuneSnapshot`-shaped `ve_table`/`lambda_table` pair plus axis lookups
  from Task 4 to map a VE cell's `(row, col)` to the matching lambda-table
  cell (lambda table has its own axes; reuse `nearest_cell` against
  `lambda_rpm_bins`/`lambda_load_bins` using the VE cell's own bin-edge
  values as the point to look up).
- Produces:
  - `MIN_SAMPLES = 20`, `MAX_RICH_PCT = 15.0`, `MAX_LEAN_PCT = 15.0` (module
    constants; `MAX_LEAN_PCT` must never be set above `MAX_RICH_PCT` —
    enforced with an `assert` at module load).
  - `class CellResult`: fields `row: int`, `col: int`, `old_ve: float`,
    `new_ve: float`, `delta_pct: float`, `sample_count: int`,
    `measured_afr: float`, `target_afr: float`, `status: str` (one of
    `"corrected"`, `"not_enough_data"`).
  - `compute_corrections(cell_samples: dict[tuple[int,int], list[float]], snapshot: TuneSnapshot) -> list[CellResult]`
    — one `CellResult` per cell present in `snapshot.ve_table` (16×16 = 256
    entries, in row-major order). For a cell with `len(samples) < MIN_SAMPLES`,
    status is `"not_enough_data"` and `new_ve == old_ve`. Otherwise:
    `measured_afr = mean(samples)`; `target_afr` from the matching lambda
    cell; raw `ratio = measured_afr / target_afr`; `raw_delta_pct = (ratio - 1) * 100`;
    clamp `raw_delta_pct` to `[-MAX_LEAN_PCT, MAX_RICH_PCT]`; `new_ve = old_ve * (1 + clamped_delta_pct/100)`.

- [ ] **Step 1: Write the failing tests**

```python
def _snapshot(ve_table, lambda_table, bins):
    # minimal stand-in with just the fields compute_corrections reads
    return SimpleNamespace(ve_table=ve_table, ve_rpm_bins=bins, ve_load_bins=bins,
                            lambda_table=lambda_table, lambda_rpm_bins=bins, lambda_load_bins=bins)

def test_cell_with_enough_samples_gets_corrected_and_capped():
    ve = [[80.0]]  # 1x1 for a focused test
    lam = [[13.0]]
    snap = _snapshot(ve, lam, [3000])
    samples = {(0, 0): [15.0] * 25}  # measured way leaner than target -> wants +15.4%, capped at MAX_RICH_PCT
    results = compute_corrections(samples, snap)
    r = results[0]
    assert r.status == "corrected"
    assert r.sample_count == 25
    assert r.delta_pct == pytest.approx(15.0)
    assert r.new_ve == pytest.approx(80.0 * 1.15)

def test_cell_without_enough_samples_is_unchanged():
    ve = [[80.0]]
    lam = [[13.0]]
    snap = _snapshot(ve, lam, [3000])
    samples = {(0, 0): [15.0] * 5}
    r = compute_corrections(samples, snap)[0]
    assert r.status == "not_enough_data"
    assert r.new_ve == 80.0

def test_lean_cap_is_never_looser_than_rich_cap():
    assert MAX_LEAN_PCT <= MAX_RICH_PCT
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `pytest ve_autotune/tests/test_correction.py -v`
Expected: FAIL

- [ ] **Step 3: Implement `correction.py`** per the Produces block above.

- [ ] **Step 4: Run tests to verify they pass**

Run: `pytest ve_autotune/tests/test_correction.py -v`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk)

---

### Task 6: Report rendering (`report.py`)

**Files:**
- Create: `ve_autotune/pipeline/report.py`
- Test: `ve_autotune/tests/test_report.py`

**Interfaces:**
- Consumes: `list[CellResult]` from Task 5.
- Produces: `render_report(results: list[CellResult]) -> str` — a Markdown
  document: a one-line summary (`"N/256 cells corrected, M not enough data"`),
  then a table with columns `row, col, old VE, new VE, Δ%, samples, measured AFR, target AFR, status`,
  one row per `CellResult`.

- [ ] **Step 1: Write the failing test**

```python
def test_render_report_includes_summary_and_corrected_cell():
    results = [
        CellResult(row=0, col=0, old_ve=80.0, new_ve=92.0, delta_pct=15.0,
                   sample_count=25, measured_afr=15.0, target_afr=13.0, status="corrected"),
        CellResult(row=0, col=1, old_ve=70.0, new_ve=70.0, delta_pct=0.0,
                   sample_count=3, measured_afr=0.0, target_afr=13.0, status="not_enough_data"),
    ]
    text = render_report(results)
    assert "1/2 cells corrected" in text
    assert "1 not enough data" in text
    assert "92.0" in text and "15.0" in text
```

- [ ] **Step 2: Run test to verify it fails**

Run: `pytest ve_autotune/tests/test_report.py -v`
Expected: FAIL

- [ ] **Step 3: Implement `render_report`** with an f-string/`str.join` table
builder (no templating library needed).

- [ ] **Step 4: Run test to verify it passes**

Run: `pytest ve_autotune/tests/test_report.py -v`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk)

---

### Task 7: Multi-log analyze orchestration (`analyze.py`)

**Files:**
- Create: `ve_autotune/pipeline/analyze.py`
- Test: `ve_autotune/tests/test_analyze.py`
- Test fixtures: two more small `.msq`/`.csv` pairs under
  `ve_autotune/tests/fixtures/` (`ride_a.csv`/`ride_a.msq`,
  `ride_b.csv`/`ride_b.msq`) — `ride_b.msq` identical to `ride_a.msq` except
  one unrelated field changed, to exercise the divergence warning.

**Interfaces:**
- Consumes: `load_tune` (Task 1), `load_csv_rows` (Task 2),
  `steady_state_rows`/`check_afr_plausible`/`reject_railed_afr`/`FlatlinedAfr`
  (Task 3), `nearest_cell` (Task 4), `compute_corrections` (Task 5),
  `render_report` (Task 6).
- Produces:
  - `class ClosedLoopActive(Exception)`
  - `class SnapshotDivergence`: field `fields: list[tuple[str, Any, Any]]`
    (field name, value in earlier log, value in later log).
  - `analyze(log_paths: list[str]) -> tuple[TuneSnapshot, list[CellResult], list[SnapshotDivergence]]`
    — for each path, expects a sibling `.msq` with the same stem (e.g.
    `ride_a.csv` → `ride_a.msq`); loads it with `load_tune`. If any
    snapshot has `fuel_closed_loop_enabled`, raises `ClosedLoopActive` naming
    that log before touching any data. Otherwise: loads each log's rows via
    `load_csv_rows`, runs `check_afr_plausible` per log (propagates
    `FlatlinedAfr` naming that log), filters with `steady_state_rows` then
    `reject_railed_afr`, bins each surviving row into a VE cell via
    `nearest_cell` against that log's own `ve_rpm_bins`/`ve_load_bins`, and
    pools samples per cell across all logs. The snapshot used as the
    baseline for `compute_corrections`/output is the one from the
    *last* path in `log_paths`. Compares every non-`veTable`/`vinNumber`
    scalar field across all loaded snapshots (fields already modeled:
    `fuel_closed_loop_enabled`, plus `lambda_table`/`lambda_rpm_bins`/
    `lambda_load_bins`/`ve_rpm_bins`/`ve_load_bins` as list-equality checks)
    and collects any difference into the returned `SnapshotDivergence` list
    (does not raise — Task 9's CLI prints these as warnings).
  - `write_output(baseline: TuneSnapshot, baseline_source_path: str, results: list[CellResult], out_dir: str) -> tuple[str, str]`
    — writes `out_dir/corrected.msq` via `write_tune_with_ve_table` (Task 1)
    and `out_dir/report.md` via `render_report` (Task 6); returns their paths.

- [ ] **Step 1: Write the failing tests**

```python
def test_analyze_pools_samples_across_two_logs(tmp_path):
    snap, results, divergences = analyze([
        "ve_autotune/tests/fixtures/ride_a.csv",
        "ve_autotune/tests/fixtures/ride_a.csv",  # same log twice == doubled sample count
    ])
    corrected = [r for r in results if r.status == "corrected"]
    assert corrected and all(r.sample_count % 2 == 0 for r in corrected)

def test_analyze_flags_snapshot_divergence_between_logs():
    _, _, divergences = analyze([
        "ve_autotune/tests/fixtures/ride_a.csv",
        "ve_autotune/tests/fixtures/ride_b.csv",
    ])
    assert len(divergences) == 1

def test_analyze_raises_when_closed_loop_was_active(tmp_path):
    # ride_c fixture's .msq has fuelClosedLoopCorrectionEnabled = "enabled"
    with pytest.raises(ClosedLoopActive):
        analyze(["ve_autotune/tests/fixtures/ride_c.csv"])
```

(Add the `ride_c.csv`/`ride_c.msq` fixture pair alongside `ride_a`/`ride_b` in
this task — same shape as `ride_a`, with `fuelClosedLoopCorrectionEnabled`
set to `"enabled"` in its `.msq`.)

- [ ] **Step 2: Run tests to verify they fail**

Run: `pytest ve_autotune/tests/test_analyze.py -v`
Expected: FAIL

- [ ] **Step 3: Implement `analyze.py`** per the Produces block above.

- [ ] **Step 4: Run tests to verify they pass**

Run: `pytest ve_autotune/tests/test_analyze.py -v`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk)

---

### Task 8: Burn-safety diff (`burn_guard.py`)

**Files:**
- Create: `ve_autotune/pipeline/burn_guard.py`
- Test: `ve_autotune/tests/test_burn_guard.py`

**Interfaces:**
- Consumes: nothing from earlier tasks directly — operates on plain
  `dict[str, Any]` field maps so it's testable without a real tune object
  (Task 9 is responsible for building those dicts from a `TuneSnapshot`
  plus whatever extra fields the live-tune read returns).
- Produces: `diff_fields(baseline: dict[str, Any], live: dict[str, Any], ignore: set[str] = {"veTable", "vinNumber"}) -> list[tuple[str, Any, Any]]`
  — for every key in `baseline` not in `ignore`, if `live` has that key and
  the values differ, append `(key, baseline_value, live_value)`. A key
  present in `baseline` but absent from `live` is not reported (nothing to
  compare).

- [ ] **Step 1: Write the failing tests**

```python
def test_diff_fields_reports_changed_non_ignored_field():
    baseline = {"veTable": [[1]], "globalTriggerAngleOffset": -2.0, "vinNumber": "A"}
    live = {"veTable": [[9]], "globalTriggerAngleOffset": -5.0, "vinNumber": "B"}
    assert diff_fields(baseline, live) == [("globalTriggerAngleOffset", -2.0, -5.0)]

def test_diff_fields_empty_when_only_ignored_fields_changed():
    baseline = {"veTable": [[1]], "vinNumber": "A"}
    live = {"veTable": [[9]], "vinNumber": "B"}
    assert diff_fields(baseline, live) == []
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `pytest ve_autotune/tests/test_burn_guard.py -v`
Expected: FAIL

- [ ] **Step 3: Implement `diff_fields`**.

- [ ] **Step 4: Run tests to verify they pass**

Run: `pytest ve_autotune/tests/test_burn_guard.py -v`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk)

---

### Task 9: CLI (`cli.py`)

**Files:**
- Create: `ve_autotune/cli.py`
- Test: `ve_autotune/tests/test_cli.py`

**Interfaces:**
- Consumes: `analyze`/`write_output` (Task 7), `diff_fields` (Task 8),
  `load_tune` (Task 1). Calls into `mcp_drive.py` only through an injectable
  `run_mcp: Callable[[str, list], str]` parameter (default implementation
  shells out to `python3 mcp_drive.py <jar/port args> <json>` exactly as used
  elsewhere this session) so tests never touch real hardware.
- Produces: `main(argv: list[str], run_mcp: Callable = _default_run_mcp) -> int`
  with subcommands:
  - `analyze <csv...> --out <dir>`: calls Task 7's `analyze` + `write_output`,
    prints the report path, the divergence warnings (if any, prefixed
    `"WARNING:"`), and the not-enough-data cell count.
  - `burn <msq_path>`: reads the live tune via `run_mcp` (one JSON call that
    exports the current tune to a temp file — reuse whatever existing
    `mcp_ecu` tool does this), loads both as field dicts, calls
    `diff_fields`. If non-empty, prints each differing field and requires
    the user to type `yes` on stdin before proceeding; if empty, proceeds
    immediately. On proceed, calls `run_mcp` with `write_tune` against
    `msq_path`, exactly as used earlier this session for the VSS pin
    migration.

- [ ] **Step 1: Write the failing tests**

```python
def test_burn_proceeds_without_prompt_when_no_diff(monkeypatch, tmp_path, capsys):
    calls = []
    def fake_run_mcp(tool, args):
        calls.append((tool, args))
        return '{"success": true}'
    # live tune export + msq_path both resolve to identical field dicts in this fake
    monkeypatch.setattr("ve_autotune.cli._read_live_tune_fields", lambda run_mcp: {"veTable": [[1]]})
    monkeypatch.setattr("ve_autotune.cli._read_msq_fields", lambda path: {"veTable": [[2]]})
    rc = main(["burn", "whatever.msq"], run_mcp=fake_run_mcp)
    assert rc == 0
    assert any(tool == "write_tune" for tool, _ in calls)

def test_burn_requires_confirmation_when_fields_diverge(monkeypatch, capsys):
    monkeypatch.setattr("builtins.input", lambda _: "no")
    monkeypatch.setattr("ve_autotune.cli._read_live_tune_fields", lambda run_mcp: {"x": 1})
    monkeypatch.setattr("ve_autotune.cli._read_msq_fields", lambda path: {"x": 2})
    calls = []
    rc = main(["burn", "whatever.msq"], run_mcp=lambda tool, args: calls.append((tool, args)))
    assert not calls  # declined -> write_tune never called
    assert rc != 0
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `pytest ve_autotune/tests/test_cli.py -v`
Expected: FAIL

- [ ] **Step 3: Implement `cli.py`** with `argparse` subparsers for `analyze`
and `burn`, and the two private helpers `_read_live_tune_fields`/
`_read_msq_fields` the tests monkeypatch (their real bodies call `run_mcp`
and `load_tune` respectively — wire them for real, tests replace them).

- [ ] **Step 4: Run tests to verify they pass**

Run: `pytest ve_autotune/tests/test_cli.py -v`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk)

---

## Not covered by this plan (explicitly deferred per design.md)

- `record`/`convert` subcommands are thin wrappers around the already-working
  `start_data_logging`/`stop_data_logging`/`convert_log_to_csv` MCP tools —
  no new logic to test beyond argument passing. Add them as a final
  thin-glue pass once Tasks 1-9 are reviewed, following the same
  injectable-`run_mcp` pattern as Task 9's `burn`.
- Real on-road validation: the engine isn't running on fuel yet (design.md,
  "Current state"). All tests above use fabricated fixtures; verification
  against a real drive log happens once one exists.
