"""Orchestrate VE autotune analysis across one or more CSV logs.

For each CSV log path, a sibling ``.msq`` tune snapshot (same stem) is
expected alongside it. Rows from every log are filtered, binned into VE
table cells, and pooled together before computing corrections against the
tune snapshot taken from the *last* log path.
"""

import os
from dataclasses import dataclass
from typing import Any

from ve_autotune.pipeline.binning import nearest_cell
from ve_autotune.pipeline.correction import CellResult, compute_corrections
from ve_autotune.pipeline.csv_log import load_csv_rows
from ve_autotune.pipeline.filters import (
    FlatlinedAfr,
    check_afr_plausible,
    reject_railed_afr,
    steady_state_rows,
)
from ve_autotune.pipeline.msq import TuneSnapshot, load_tune, write_tune_with_ve_table
from ve_autotune.pipeline.report import render_report

# Best-guess mapping from logical channel names to rusEFI .ini output-channel
# names, built from the real rusEFI .ini output-channel names confirmed
# elsewhere in this project. No real CSV log from the actual ECU logger has
# ever been observed (design.md flags this explicitly) -- update this once
# one is available.
CHANNEL_MAP = {
    "t": "Time",
    "rpm": "RPMValue",
    "map": "MAPValue",
    "tps": "TPSValue",
    "clt": "coolant",
    "afr": "AFRValue",
}

# TuneSnapshot fields (besides ve_table, which is the thing being tuned, and
# vinNumber, which isn't modeled here) compared across logs for divergence.
_DIVERGENCE_FIELDS = [
    "ve_rpm_bins",
    "ve_load_bins",
    "lambda_table",
    "lambda_rpm_bins",
    "lambda_load_bins",
    "fuel_closed_loop_enabled",
]


class ClosedLoopActive(Exception):
    """A log's tune snapshot had closed-loop fuel correction enabled."""


@dataclass
class SnapshotDivergence:
    fields: list[tuple[str, Any, Any]]


def _sibling_msq_path(log_path: str) -> str:
    stem, _ = os.path.splitext(log_path)
    return stem + ".msq"


def _find_divergences(snapshots: list[TuneSnapshot]) -> list[SnapshotDivergence]:
    divergences = []
    for earlier, later in zip(snapshots, snapshots[1:]):
        diffs = [
            (name, getattr(earlier, name), getattr(later, name))
            for name in _DIVERGENCE_FIELDS
            if getattr(earlier, name) != getattr(later, name)
        ]
        if diffs:
            divergences.append(SnapshotDivergence(fields=diffs))
    return divergences


def analyze(
    log_paths: list[str],
) -> tuple[TuneSnapshot, list[CellResult], list[SnapshotDivergence]]:
    snapshots = [load_tune(_sibling_msq_path(path)) for path in log_paths]

    # Closed-loop fuel correction invalidates logged AFR as a VE signal, so
    # check every snapshot before touching any log data.
    for path, snapshot in zip(log_paths, snapshots):
        if snapshot.fuel_closed_loop_enabled:
            raise ClosedLoopActive(path)

    divergences = _find_divergences(snapshots)

    cell_samples: dict[tuple[int, int], list[float]] = {}
    for path, snapshot in zip(log_paths, snapshots):
        rows = load_csv_rows(path, CHANNEL_MAP)
        try:
            check_afr_plausible(rows)
        except FlatlinedAfr as exc:
            raise FlatlinedAfr(f"{path}: {exc}") from exc

        rows = steady_state_rows(rows)
        rows = reject_railed_afr(rows)

        for row in rows:
            cell = nearest_cell(
                row["rpm"], row["map"], snapshot.ve_rpm_bins, snapshot.ve_load_bins
            )
            cell_samples.setdefault(cell, []).append(row["afr"])

    baseline = snapshots[-1]
    results = compute_corrections(cell_samples, baseline)
    return baseline, results, divergences


def write_output(
    baseline: TuneSnapshot,
    baseline_source_path: str,
    results: list[CellResult],
    out_dir: str,
) -> tuple[str, str]:
    size = len(baseline.ve_table)
    ve_table = [[0.0] * size for _ in range(size)]
    for r in results:
        ve_table[r.row][r.col] = r.new_ve

    os.makedirs(out_dir, exist_ok=True)

    msq_path = os.path.join(out_dir, "corrected.msq")
    write_tune_with_ve_table(baseline_source_path, ve_table, msq_path)

    report_path = os.path.join(out_dir, "report.md")
    with open(report_path, "w") as f:
        f.write(render_report(results))

    return msq_path, report_path
