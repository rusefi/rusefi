"""Orchestrate VE autotune analysis across one or more CSV logs.

For each CSV log path, a sibling ``.msq`` tune snapshot (same stem) is
expected alongside it. Rows from every log are filtered, binned into VE
table cells, and pooled together before computing corrections against the
tune snapshot taken from the *last* log path.
"""

import os
import shutil
from dataclasses import dataclass
from typing import Any

from ve_autotune.pipeline.binning import nearest_cell
from ve_autotune.pipeline.burn_guard import diff_fields
from ve_autotune.pipeline.correction import CellResult, compute_corrections
from ve_autotune.pipeline.csv_log import load_csv_rows
from ve_autotune.pipeline.filters import (
    FlatlinedAfr,
    check_afr_plausible,
    reject_cold_rows,
    reject_cranking_rows,
    reject_dfco_rows,
    reject_railed_afr,
    reject_transient_rows,
)
from ve_autotune.pipeline.msq import TuneSnapshot, load_tune, read_all_constants, write_tune_with_ve_table
from ve_autotune.pipeline.report import render_report

# Base datalog-label text for the channels this pipeline needs, confirmed
# from firmware/tunerstudio/generated/rusefi_mre_f4.ini's [Datalog] section
# (the real .ini this project's ECU uses):
#   entry = time,      "Time",            float, "%.3f"
#   entry = RPMValue,  "RPM",             int,   "%d"
#   entry = coolant,   "CLT",             float, "%.3f"
#   entry = TPSValue,  "TPS",             float, "%.3f"
#   entry = MAPValue,  "MAP",             float, "%.3f"
#   entry = AFRValue,  "Air/Fuel Ratio",  float, "%.3f"
#
# The real convert_log_to_csv tool (java_console/mcp_ecu/.../MslToCsv.java,
# heading()) names each CSV column after this *label*, not the .ini output
# channel's own field name (e.g. "RPM", not "RPMValue") -- and appends a
# " (<units>)" suffix when the underlying output channel declares units
# (e.g. "RPM (RPM)", "MAP (kPa)"); csv_log.load_csv_rows strips that suffix
# before matching, so either form resolves correctly. See MslToCsv.java's
# `heading(name, units)` and CustomBinaryLogEntry.getUnit() for where the
# units half of that suffix comes from (the [OutputChannels] scalar
# declaration's own units string, e.g. RPMValue's "RPM", MAPValue's "kPa").
#
# Known gap, not fixed in this pass: for a *binary* .mlg log, SensorLogger
# marks "time" as a host-only channel with no raw output-channel field
# (see its "Computed and host-only channels (including TS 'time') have no
# raw field" comment), and MslToCsv's binary decoder explicitly discards the
# per-record time offset ("no synthetic time column: preserve the logged
# fields"). Whether a real binary-converted CSV ends up with a usable "Time"
# column at all is therefore unconfirmed -- flagged here rather than guessed;
# verify against a real log once one exists (no real log exists yet, per
# design.md).
CHANNEL_MAP = {
    "t": "Time",
    "rpm": "RPM",
    "map": "MAP",
    "tps": "TPS",
    "clt": "CLT",
    "afr": "Air/Fuel Ratio",
}

# Fields excluded from cross-log divergence checks (and from the burn-safety
# diff in burn_guard.diff_fields): veTable is the thing being tuned, and
# vinNumber is expected to vary/doesn't matter here.
_DIVERGENCE_IGNORE = frozenset({"veTable", "vinNumber"})


class ClosedLoopActive(Exception):
    """A log's tune snapshot had closed-loop fuel correction enabled."""


@dataclass
class SnapshotDivergence:
    """One log's tune snapshot disagreeing with the baseline snapshot.

    ``log_path`` is the (non-baseline) log whose snapshot differs;
    ``baseline_path`` is the log used as the correction baseline (the last
    path in ``analyze()``'s ``log_paths``). ``fields`` is
    ``(field_name, baseline_value, log_value)`` triples, built from *every*
    ``<constant>`` in both .msq files (via `msq.read_all_constants`), not
    just the handful of fields `TuneSnapshot` models -- so a divergence in
    an unmodeled field (e.g. ignition timing) is caught too, the same lesson
    already applied to the burn-safety guard.
    """

    log_path: str
    baseline_path: str
    fields: list[tuple[str, Any, Any]]


@dataclass
class FilterCounts:
    """How many rows of one log were rejected by each filter stage, so a
    report showing e.g. "0/256 corrected" isn't silent about why.
    """

    log_path: str
    total_rows: int
    rejected_transient: int
    rejected_cold: int
    rejected_cranking: int
    rejected_dfco: int
    rejected_railed: int
    kept: int


def _sibling_msq_path(log_path: str) -> str:
    stem, _ = os.path.splitext(log_path)
    return stem + ".msq"


def _find_divergences(log_paths: list[str]) -> list[SnapshotDivergence]:
    """Compare every log's full .msq constant set against the baseline's.

    Built from `read_all_constants` (every `<constant>` tag), not just the
    7 fields `TuneSnapshot` models -- Task 9's burn-guard fix made this same
    change for the live-tune diff; this is the matching fix for the
    cross-log divergence check. Each non-baseline log is compared directly
    against the baseline (the last path), not just consecutive pairs, so a
    divergence between log 1 and log 3 in a 3-log pool is caught even if log
    2 happens to agree with the baseline.
    """
    constants_by_path = [(path, read_all_constants(_sibling_msq_path(path))) for path in log_paths]
    baseline_path, baseline_constants = constants_by_path[-1]

    divergences = []
    for path, constants in constants_by_path[:-1]:
        diffs = diff_fields(baseline_constants, constants, ignore=_DIVERGENCE_IGNORE)
        if diffs:
            divergences.append(
                SnapshotDivergence(log_path=path, baseline_path=baseline_path, fields=diffs)
            )
    return divergences


def analyze(
    log_paths: list[str],
) -> tuple[TuneSnapshot, list[CellResult], list[SnapshotDivergence], list[FilterCounts]]:
    snapshots = [load_tune(_sibling_msq_path(path)) for path in log_paths]

    # Closed-loop fuel correction invalidates logged AFR as a VE signal, so
    # check every snapshot before touching any log data.
    for path, snapshot in zip(log_paths, snapshots):
        if snapshot.fuel_closed_loop_enabled:
            raise ClosedLoopActive(path)

    divergences = _find_divergences(log_paths)

    cell_samples: dict[tuple[int, int], list[float]] = {}
    filter_counts: list[FilterCounts] = []
    for path, snapshot in zip(log_paths, snapshots):
        rows = load_csv_rows(path, CHANNEL_MAP)
        try:
            check_afr_plausible(rows)
        except FlatlinedAfr as exc:
            raise FlatlinedAfr(f"{path}: {exc}") from exc

        total = len(rows)
        after_transient = reject_transient_rows(rows)
        after_cold = reject_cold_rows(after_transient)
        after_cranking = reject_cranking_rows(after_cold)
        after_dfco = reject_dfco_rows(after_cranking)
        after_railed = reject_railed_afr(after_dfco)

        filter_counts.append(
            FilterCounts(
                log_path=path,
                total_rows=total,
                rejected_transient=total - len(after_transient),
                rejected_cold=len(after_transient) - len(after_cold),
                rejected_cranking=len(after_cold) - len(after_cranking),
                rejected_dfco=len(after_cranking) - len(after_dfco),
                rejected_railed=len(after_dfco) - len(after_railed),
                kept=len(after_railed),
            )
        )

        for row in after_railed:
            cell = nearest_cell(
                row["rpm"], row["map"], snapshot.ve_rpm_bins, snapshot.ve_load_bins
            )
            cell_samples.setdefault(cell, []).append(row["afr"])

    baseline = snapshots[-1]
    results = compute_corrections(cell_samples, baseline)
    return baseline, results, divergences, filter_counts


def write_output(
    baseline: TuneSnapshot,
    baseline_source_path: str,
    results: list[CellResult],
    out_dir: str,
    filter_counts: list[FilterCounts] | None = None,
) -> tuple[str, str]:
    # Start from the baseline's own VE table (not a fresh all-zero grid) so
    # any future gap in compute_corrections's cell coverage can't silently
    # write 0.0 into a cell -- compute_corrections already emits all 256
    # cells today, but this removes the latent risk.
    ve_table = [row[:] for row in baseline.ve_table]
    for r in results:
        ve_table[r.row][r.col] = r.new_ve

    os.makedirs(out_dir, exist_ok=True)

    msq_path = os.path.join(out_dir, "corrected.msq")
    write_tune_with_ve_table(baseline_source_path, ve_table, msq_path)

    # Preserve the exact baseline tune (the snapshot corrections were
    # computed against) alongside the output. `corrected.msq`'s own veTable
    # is *expected* to differ from the live ECU's (that's the whole point of
    # this tool), so it can't be used to detect a hand-edited VE table on the
    # live ECU made between recording and burning -- `baseline.msq` can.
    baseline_msq_path = os.path.join(out_dir, "baseline.msq")
    shutil.copyfile(baseline_source_path, baseline_msq_path)

    report_path = os.path.join(out_dir, "report.md")
    with open(report_path, "w") as f:
        f.write(render_report(results, filter_counts))

    return msq_path, report_path
