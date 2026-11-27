"""Compute per-cell VE corrections from pooled AFR samples."""

from dataclasses import dataclass
from typing import Any

from ve_autotune.pipeline.binning import nearest_cell

MIN_SAMPLES = 20
MAX_RICH_PCT = 15.0
MAX_LEAN_PCT = 15.0

assert MAX_LEAN_PCT <= MAX_RICH_PCT, (
    "MAX_LEAN_PCT must never be looser (larger) than MAX_RICH_PCT"
)


@dataclass
class CellResult:
    row: int
    col: int
    old_ve: float
    new_ve: float
    delta_pct: float
    sample_count: int
    measured_afr: float
    target_afr: float
    status: str


def compute_corrections(
    cell_samples: dict[tuple[int, int], list[float]],
    snapshot: Any,
) -> list[CellResult]:
    """Return one `CellResult` per VE table cell, in row-major order.

    Cells with fewer than `MIN_SAMPLES` pooled AFR readings are left
    unchanged (`status = "not_enough_data"`). Otherwise the measured AFR is
    compared against the matching lambda-table cell's target AFR, and the VE
    cell is nudged toward the target, clamped to
    `[-MAX_LEAN_PCT, MAX_RICH_PCT]` per pass.
    """
    results: list[CellResult] = []
    for row, ve_row in enumerate(snapshot.ve_table):
        for col, old_ve in enumerate(ve_row):
            samples = cell_samples.get((row, col), [])
            sample_count = len(samples)

            if sample_count < MIN_SAMPLES:
                results.append(
                    CellResult(
                        row=row,
                        col=col,
                        old_ve=old_ve,
                        new_ve=old_ve,
                        delta_pct=0.0,
                        sample_count=sample_count,
                        measured_afr=0.0,
                        target_afr=0.0,
                        status="not_enough_data",
                    )
                )
                continue

            measured_afr = sum(samples) / sample_count

            rpm = snapshot.ve_rpm_bins[col]
            map_kpa = snapshot.ve_load_bins[row]
            lam_row, lam_col = nearest_cell(
                rpm, map_kpa, snapshot.lambda_rpm_bins, snapshot.lambda_load_bins
            )
            target_afr = snapshot.lambda_table[lam_row][lam_col]

            raw_delta_pct = (measured_afr / target_afr - 1) * 100
            delta_pct = max(-MAX_LEAN_PCT, min(MAX_RICH_PCT, raw_delta_pct))
            new_ve = old_ve * (1 + delta_pct / 100)

            results.append(
                CellResult(
                    row=row,
                    col=col,
                    old_ve=old_ve,
                    new_ve=new_ve,
                    delta_pct=delta_pct,
                    sample_count=sample_count,
                    measured_afr=measured_afr,
                    target_afr=target_afr,
                    status="corrected",
                )
            )

    return results
